VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBBuildHook"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------ 类模块 VBWindowCreationHook.cls ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBWindowCreationHook"

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
Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Private Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Private Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long
Private Declare Function GetSystemDirectory Lib "kernel32" Alias "GetSystemDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long

Private Declare Function VarPtrArray Lib "msvbvm60" Alias "VarPtr" (aArray() As Any) As Long

Private Type SafeArrayStruct
    cDims As Integer
    fFeatures As Integer
    cbElements As Long
    clocks As Long
    pvData As Long
    cElements As Long
    lLbound As Long
End Type

Private Enum VBEC_ENUM_WHEREAMI
    evaDev = 0
    evaNative
    evaPCode
End Enum

Private m_lTargetAddress As Long
Private m_hVBA6 As Long
Private m_hTargetModule As Long
Private m_bOriEHBytes() As Byte
Private m_lCodeBufferAddress As Long
Private m_lCodeStartAddress As Long
Private m_lJumpAddress As Long
Private m_fIsInstalled As Boolean

Private Const CODE_BUFFER_SIZE As Long = 512
Private Const MAGIC_STRING As String = "BB 00 00 00 08"
Private Const MAGIC_STRING1 As String = "FF D7 3B C6"


'**************************************************************
'下面代码在使用的时候复制到模块中
'
'编译控制回调过程：调用CreateProcess以前
'Public Sub BuildHookProc_Before(ByVal lplpApplication As Long, ByVal lplpCommandLine As Long)
'
'End Sub

'编译控制回调过程：调用CreateProcess以后
'Public Sub BuildHookProc_After(ByVal hProcess As Long)
'    'hProcess为CreateProcess后得到的进程句柄
'End Sub
'**************************************************************

'******************************* 暴露的接口 *******************************
Public Function InstallHook(ByVal lpHookProcBefore As Long, ByVal lpHookProcAfter As Long) As Boolean
    If m_lTargetAddress <> 0 Then
        ReDim m_bOriEHBytes(8)
        
        Call ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes())
               
        m_lCodeStartAddress = PrepareCode(lpHookProcBefore, lpHookProcAfter)
        
        If m_lCodeStartAddress = 0 Then Exit Function
        
        m_lJumpAddress = VarPtr(m_lCodeStartAddress)
    
        CopyMemory m_bOriEHBytes(2), m_lJumpAddress, 4
    
        m_bOriEHBytes(0) = &HFF
        m_bOriEHBytes(1) = &H25
        m_bOriEHBytes(6) = &H90
        m_bOriEHBytes(7) = &H90
        m_bOriEHBytes(8) = &H90
        
        If ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes(), False) Then     '写入我们的地址
            InstallHook = True
            m_fIsInstalled = True
        End If
    End If
End Function

'卸载钩子
Public Function UninstallHook() As Boolean
    If Not m_fIsInstalled Then Exit Function
    
    If ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes(), False) Then    '恢复原来的地址
        If MemOp(False) Then
            UninstallHook = True
        End If
    End If
End Function

'是否已安装钩子
Public Property Get IsInstalled() As Boolean
    IsInstalled = m_fIsInstalled
End Property

'读取或写入指定地址lAddr
Public Function ReadWriteMemory(ByVal lAddr As Long, buff() As Byte, Optional fRead As Boolean = True, Optional lPID As Long = -1) As Boolean
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

'******************************* 暴露的接口 *******************************

'******************************** 私有函数 ********************************

Private Function GetByteString(b() As Byte, Optional fPrint As Boolean = False) As String
    Dim lLen As Long
    lLen = UBound(b) - LBound(b) + 1
    If lLen <= 0 Or Err.Number <> 0 Then
        Exit Function
    End If
    
    Dim i As Long
    For i = 0 To lLen - 1
        If b(i) < 16 Then
            GetByteString = GetByteString & "0" & Hex(b(i))
        Else
            GetByteString = GetByteString & Hex(b(i))
        End If
        GetByteString = GetByteString & " "
    Next
    
    If fPrint Then
        Debug.Print GetByteString
    End If
End Function

Private Function PrepareCode(ByVal lpHookProcBefore As Long, ByVal lpHookProcAfter As Long) As Long
    If Not MemOp() Then Exit Function

    Dim lCodeStartPosition As Long, lLastPos As Long
    Dim bCodeBytes(CODE_BUFFER_SIZE - 1) As Byte
    Dim i As Long
        
    lCodeStartPosition = GetAlignedlCodeStartPosition(m_lCodeBufferAddress)
    lLastPos = lCodeStartPosition - m_lCodeBufferAddress
    
    For i = 0 To lLastPos - 1
        bCodeBytes(i) = &HCC
    Next
       
    'int 3 测试用
    'AddByteToCode &HCC, bCodeBytes(), lLastPos
    
    'mov     [ebp+StartupInfo.wShowWindow], si
    'push    [ebp+lpMultiByteStr] ; lpApplicationName
    For i = 0 To 6
        AddByteToCode m_bOriEHBytes(i), bCodeBytes(), lLastPos
    Next
    
    'push eax
    AddByteToCode &H50, bCodeBytes(), lLastPos
    
    'lea eax,[esp+8]
    AddByteStrToCode "8D 44 24 08", bCodeBytes(), lLastPos
    
    'push eax
    AddByteToCode &H50, bCodeBytes(), lLastPos
    
    'lea eax,[esp+8]
    AddByteStrToCode "8D 44 24 08", bCodeBytes(), lLastPos
    
    'push eax
    AddByteToCode &H50, bCodeBytes(), lLastPos
    
    'call lpHookProcBefore
    AddByteToCode &HE8, bCodeBytes(), lLastPos
    AddLongToCode lpHookProcBefore - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
       
    'pop eax
    AddByteToCode &H58, bCodeBytes(), lLastPos
    
    'lea esi,[esp+24]
    AddByteStrToCode "8D 74 24 24", bCodeBytes(), lLastPos
    
    'mov esi,[esi]
    AddByteStrToCode "8B 36", bCodeBytes(), lLastPos
    
    'call edi->createprocessa
    For i = 7 To 8
        AddByteToCode m_bOriEHBytes(i), bCodeBytes(), lLastPos
    Next
        
    'push eax
    AddByteToCode &H50, bCodeBytes(), lLastPos
    
    'mov esi,[esi]
    AddByteStrToCode "8B 36", bCodeBytes(), lLastPos
    
    'push esi
    AddByteToCode &H56, bCodeBytes(), lLastPos
    
    'call lpHookProcAfter
    AddByteToCode &HE8, bCodeBytes(), lLastPos
    AddLongToCode lpHookProcAfter - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
    
    'pop eax
    AddByteToCode &H58, bCodeBytes(), lLastPos
    
    'xor esi,esi
    AddByteStrToCode "33 F6", bCodeBytes(), lLastPos
    
    'push return address
    AddByteToCode &H68, bCodeBytes(), lLastPos
    AddLongToCode m_lTargetAddress + UBound(m_bOriEHBytes) + 1, bCodeBytes(), lLastPos
        
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

Private Function GetTargetAddress() As Long
    GetTargetAddress = LocateTarget()
    
    If GetTargetAddress Then
        GetTargetAddress = GetTargetAddress - 7
    End If

End Function

Private Function LocateTarget() As Long
    On Error Resume Next
    Dim ppvData As Long
    Dim pvData As Long
    Dim ppSA As Long
    Dim pSA As Long
    Dim lVal As Long
    Dim lpAddr As Long
    Dim bTarget() As Byte
    
    ReDim bTarget(&H190000)
    
    Dim bFind() As Byte
    
    bFind = ByteCodeStrToBin(MAGIC_STRING)
    
    lpAddr = m_hTargetModule
    
    If Err.Number <> 0 Then Exit Function
    
    ppSA = VarPtrArray(bTarget())
    pSA = GetArrayPtr(ppSA)
    ppvData = pSA + 12
    
    CopyMemory pvData, ByVal ppvData, 4
    CopyMemory ByVal ppvData, lpAddr, 4
    
    LocateTarget = InStrB(bTarget, bFind)
    
    If LocateTarget <> 0 Then
        bFind = ByteCodeStrToBin(MAGIC_STRING1)
        LocateTarget = InStrB(LocateTarget, bTarget, bFind)
    End If
        
    CopyMemory ByVal ppvData, pvData, 4
    
    If LocateTarget <> 0 Then
        LocateTarget = LocateTarget + lpAddr - 1
    End If
End Function

Private Function GetArrayPtr(ByVal ppSA As Long) As Long
    CopyMemory GetArrayPtr, ByVal ppSA, 4
End Function

'汇编字节字符串转换为代码字节数组
'sByteCode:汇编字节字符串
Private Function ByteCodeStrToBin(ByVal sByteCode As String) As Byte()
    Dim s() As String
    Dim b() As Byte
    Dim i As Long
    
    s = Split(Trim(sByteCode), " ")
    
    If UBound(s) >= 0 Then
        ReDim b(UBound(s))
    End If
    
    For i = 0 To UBound(s)
        b(i) = CByte("&h" & s(i))
    Next
    ByteCodeStrToBin = b
End Function

Private Sub Class_Initialize()
    m_hVBA6 = GetModuleHandle("vba6.dll")
    m_hTargetModule = m_hVBA6
    
    m_lTargetAddress = GetTargetAddress
End Sub

'******************************** 私有函数 ********************************


