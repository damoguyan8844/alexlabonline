VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBCallConvention"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------ 类模块 CVBCallConvention.cls ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBCallConvention"

Private Declare Function GetCurrentProcessId Lib "kernel32" () As Long
Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Private Declare Function VirtualAlloc Lib "kernel32" (lpAddress As Any, ByVal dwSize As Long, ByVal flAllocationType As Long, ByVal flProtect As Long) As Long
Private Declare Function VirtualFree Lib "kernel32" (lpAddress As Any, ByVal dwSize As Long, ByVal dwFreeType As Long) As Long
Private Declare Function VirtualProtectEx Lib "kernel32" (ByVal hProcess As Long, lpAddress As Any, ByVal dwSize As Long, ByVal flNewProtect As Long, lpflOldProtect As Long) As Long
Private Declare Function VirtualQueryEx Lib "kernel32" (ByVal hProcess As Long, lpAddress As Any, lpBuffer As MEMORY_BASIC_INFORMATION, ByVal dwLength As Long) As Long
Private Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Private Declare Function ReadProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long

Private Type MEMORY_BASIC_INFORMATION
     BaseAddress As Long
     AllocationBase As Long
     AllocationProtect As Long
     RegionSize As Long
     State As Long
     Protect As Long
     lType As Long
End Type

Private Const PROCESS_ALL_ACCESS = &H1F0FFF

Private Const PAGE_READWRITE As Long = &H4
Private Const MEM_COMMIT As Long = &H1000
Private Const MEM_DECOMMIT As Long = &H4000


Private Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)
Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long

Private m_lCodeBufferAddress As Long
Private m_lCodeStartAddress As Long
Private m_lOldFunctionAddress As Long

Private Const CODE_BUFFER_SIZE As Long = 512

Private Enum eccCallConvention
    cc_stdcall = 0
    cc_cdecl = 1
End Enum
'调用约定
'0 - __stdcall约定
'1 - __cdecl约定
Private m_lCallConvention As eccCallConvention

Public Property Let CallConvention(lCallConvention As Long)
    m_lCallConvention = lCallConvention
    Call HookDllCallFunction
End Property

Public Property Get CallConvention() As Long
     CallConvention = m_lCallConvention
End Property

Private Function HookDllCallFunction() As Boolean
    Dim sImportModuleName As String, sExportModuleName As String, sTargetFunctionName As String
    Dim hImportModule As Long
    Dim hExportModule As Long
    
    sImportModuleName = vbNullString
    sExportModuleName = "msvbvm60"
    sTargetFunctionName = "DllFunctionCall"
    
    hImportModule = GetModuleHandle(sImportModuleName)
    If hImportModule <> 0 Then
        hExportModule = GetModuleHandle(sExportModuleName)
        If hExportModule <> 0 Then
            Dim lpTargetFunction As Long
            lpTargetFunction = GetProcAddress(hExportModule, sTargetFunctionName)
            If lpTargetFunction <> 0 Then
                Dim lpPEHeader As Long
                Dim lpIAT As Long
                Dim lIATSize As Long
                Dim lpIATEnd As Long
                
                lpPEHeader = hImportModule
                lpIAT = hImportModule + &H1000
                Do
                   If lpPEHeader > lpIAT Then
                      Exit Function
                   Else
                      If GetValueLng(lpPEHeader) = &H4550 Then
                         Exit Do
                      Else
                         lpPEHeader = lpPEHeader + 1
                      End If
                   End If
                Loop
                
                lIATSize = GetValueLng(lpPEHeader + &HDC)
                If lIATSize > 0 And lIATSize <= &HFFFFF Then
                    lpIATEnd = lpIAT + lIATSize
                    Do
                        If lpIAT > lpIATEnd Then
                            Exit Function
                        Else
                        
                        If lpTargetFunction = GetValueLng(lpIAT) Then
                           Exit Do
                        Else
                           lpIAT = lpIAT + 4
                        End If
                       End If
                    Loop
                    
                    Dim abTmp(3) As Byte
                    CopyMemory abTmp(0), PrepareCode(lpTargetFunction), 4
                    If ReadWriteMemory(lpIAT, abTmp(), False) Then
                        HookDllCallFunction = True
                    End If
                End If
                                
            End If
        End If
    End If
End Function

'读取或写入指定地址lAddr
Private Function ReadWriteMemory(ByVal lAddr As Long, buff() As Byte, Optional fRead As Boolean = True, Optional lPID As Long = -1) As Boolean
    Dim hProcess As Long
    Dim mi As MEMORY_BASIC_INFORMATION
    Dim lpAddress As Long, lOldProtect As Long
    Dim lBytesReadWrite As Long
    Dim bTmp() As Byte
    
    lpAddress = lAddr
    If lPID = -1 Then
        lPID = GetCurrentProcessId
    End If
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0&, lPID)
    If hProcess Then
        If VirtualQueryEx(hProcess, ByVal lpAddress, mi, Len(mi)) Then
            If VirtualProtectEx(hProcess, ByVal mi.BaseAddress, mi.RegionSize, PAGE_READWRITE, lOldProtect) <> 0 Then
                If fRead Then
                    ReadProcessMemory hProcess, ByVal lpAddress, buff(0), UBound(buff) + 1, lBytesReadWrite
                Else
                    ReDim bTmp(UBound(buff))
                    ReadProcessMemory hProcess, ByVal lpAddress, bTmp(0), UBound(bTmp) + 1, lBytesReadWrite
                    WriteProcessMemory hProcess, ByVal lpAddress, buff(0), UBound(buff) + 1, lBytesReadWrite
                    CopyMemory buff(0), bTmp(0), UBound(bTmp) + 1
                End If
                Call VirtualProtectEx(hProcess, ByVal mi.BaseAddress, mi.RegionSize, lOldProtect, lOldProtect)
                ReadWriteMemory = (lBytesReadWrite <> 0)
            End If
        End If
        CloseHandle hProcess
    End If
End Function

Private Function GetValueLng(ByVal lpAddress As Long) As Long
    CopyMemory GetValueLng, ByVal lpAddress, 4
End Function

Private Function PrepareCode(ByVal lpDllFunctionCall As Long) As Long
    If Not MemOp() Then Exit Function

    Dim lpTmpVar As Long
    Dim lCodeStartPosition As Long, lLastPos As Long
    Dim bCodeBytes(CODE_BUFFER_SIZE - 1) As Byte
    Dim i As Long
        
    lpTmpVar = m_lCodeBufferAddress + CODE_BUFFER_SIZE - 4
    lCodeStartPosition = GetAlignedlCodeStartPosition(m_lCodeBufferAddress)
    lLastPos = lCodeStartPosition - m_lCodeBufferAddress
    
    For i = 0 To lLastPos - 1
        bCodeBytes(i) = &HCC
    Next
    
    'int 3
    'AddByteToCode &HCC, bCodeBytes(), lLastPos
   
    'push [esp+4]
    'AddByteStrToCode "FF 74 24 04", bCodeBytes(), lLastPos
    
    'add esp,4
    AddByteStrToCode "83 C4 04", bCodeBytes(), lLastPos
    
    'Call DllFunctionCall
    AddByteToCode &HE8, bCodeBytes(), lLastPos
    AddLongToCode lpDllFunctionCall - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
    
    'pop ebx
    AddByteStrToCode "5B", bCodeBytes(), lLastPos
    
    'push eax
    AddByteStrToCode "50", bCodeBytes(), lLastPos
    
    'mov eax,lpTmpVar
    AddByteStrToCode "B8", bCodeBytes(), lLastPos
    AddLongToCode lpTmpVar, bCodeBytes(), lLastPos
    
    'mov [eax],ebx
    AddByteStrToCode "89 18", bCodeBytes(), lLastPos
    
    'pop eax
    AddByteStrToCode "58", bCodeBytes(), lLastPos
    
    'push ret_addr
    AddByteToCode &H68, bCodeBytes(), lLastPos
    AddLongToCode m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0)) + 6, bCodeBytes(), lLastPos
    
    'jmp eax
    AddByteStrToCode "FF E0", bCodeBytes(), lLastPos
    
    'ret_addr:
    If m_lCallConvention = cc_cdecl Then
        
    End If
    
    'mov ebx,[lpTmpVar]
    AddByteStrToCode "BB", bCodeBytes(), lLastPos
    AddLongToCode lpTmpVar, bCodeBytes(), lLastPos
    
    'push [ebx]
    AddByteStrToCode "FF 33", bCodeBytes(), lLastPos
    
    'ret
    AddByteStrToCode "C3", bCodeBytes(), lLastPos
    
    For i = lLastPos To CODE_BUFFER_SIZE - 1
        bCodeBytes(i) = &HCC
    Next
       
    CopyMemory ByVal m_lCodeBufferAddress, bCodeBytes(0), CODE_BUFFER_SIZE
    
    PrepareCode = lCodeStartPosition
End Function

Private Function AddByteStrToCode(ByVal sCodeBytes As String, bCodeBytes() As Byte, lPos As Long) As Long
    Dim i As Long
    Dim asCodeByte() As String
    asCodeByte = Split(Trim(sCodeBytes), " ")
    For i = 0 To UBound(asCodeByte)
        AddByteToCode CByte("&H" & asCodeByte(i)), bCodeBytes(), lPos
    Next
End Function
Private Function AddByteToCode(ByVal bCode As Byte, bCodeBytes() As Byte, lPos As Long) As Long
    bCodeBytes(lPos) = bCode
    lPos = lPos + 1
End Function

Private Function AddLongToCode(ByVal lCode As Long, bCodeBytes() As Byte, lPos As Long) As Long
    CopyMemory bCodeBytes(lPos), lCode, 4
    lPos = lPos + 4
End Function

Private Function GetAlignedlCodeStartPosition(lAddr As Long) As Long
    GetAlignedlCodeStartPosition = lAddr + (15 - (lAddr - 1) Mod 16)
    If (15 - (lAddr - 1) Mod 16) = 0 Then GetAlignedlCodeStartPosition = GetAlignedlCodeStartPosition + 16
End Function

Private Function MemOp(Optional fAllocate As Boolean = True) As Boolean
    If fAllocate Then
        m_lCodeBufferAddress = VirtualAlloc(ByVal 0&, CODE_BUFFER_SIZE, MEM_COMMIT, PAGE_READWRITE)
        MemOp = (m_lCodeBufferAddress <> 0)
    Else
        MemOp = (VirtualFree(ByVal m_lCodeBufferAddress, CODE_BUFFER_SIZE, MEM_DECOMMIT) <> 0)
    End If
End Function


Private Sub Class_Initialize()
    m_lCallConvention = 0
End Sub
