VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CWinABCPatcher"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------ 类模块 CWinABCPatcher.cls ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CWinABCPatcher"

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
Private m_hWinABC As Long
Private m_bOriEHBytes() As Byte
Private m_lCodeBufferAddress As Long
Private m_lCodeStartAddress As Long
Private m_lJumpAddress As Long
Private m_fIsInstalled As Boolean
Private m_fIsWinABCPresent As Boolean
Private m_lVersion As Long

Private Const CODE_BUFFER_SIZE As Long = 512
Private Const MAGIC_STRING5 As String = "89 48 04 89 48 0C" '"74 22 0F B7 05" 'For ABC 5.0,5.1
Private Const MAGIC_STRING4 As String = "90 83 F9 0C 76" 'For ABC 4.0


'******************************* 暴露的接口 *******************************

Public Function InstallPatcher() As Boolean
    If m_fIsWinABCPresent And m_lTargetAddress <> 0 Then
        Select Case m_lVersion
        Case 4
            ReDim m_bOriEHBytes(6)
            
            Call ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes())
            
            m_lCodeStartAddress = PrepareCode()
            
            If m_lCodeStartAddress = 0 Then Exit Function
            
            m_lJumpAddress = VarPtr(m_lCodeStartAddress)
        
            CopyMemory m_bOriEHBytes(2), m_lJumpAddress, 4
        
            m_bOriEHBytes(0) = &HFF
            m_bOriEHBytes(1) = &H25
            m_bOriEHBytes(6) = &H90
        Case 5
            ReDim m_bOriEHBytes(5)
            
            Call ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes())
            
            m_lCodeStartAddress = PrepareCode()
            
            If m_lCodeStartAddress = 0 Then Exit Function
            
            m_lJumpAddress = VarPtr(m_lCodeStartAddress)
        
            CopyMemory m_bOriEHBytes(2), m_lJumpAddress, 4
        
            m_bOriEHBytes(0) = &HFF
            m_bOriEHBytes(1) = &H25
        End Select
        'ShowMemory m_lTargetAddress, 7
        'Exit Function
        If ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes(), False) Then     '写入我们的地址
            InstallPatcher = True
            m_fIsInstalled = True
        End If
    End If
End Function


Public Function UnInstallPatcher() As Boolean
    If Not m_fIsInstalled Then Exit Function
    
    If ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes(), False) Then    '恢复原来的地址
        If MemOp(False) Then
            UnInstallPatcher = True
        End If
    End If
End Function

'是否已安装补丁
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

Private Function PrepareCode() As Long
    If Not MemOp() Then Exit Function

    Dim lCodeStartPosition As Long, lLastPos As Long
    Dim bCodeBytes(CODE_BUFFER_SIZE - 1) As Byte
    Dim i As Long
        
    lCodeStartPosition = GetAlignedlCodeStartPosition(m_lCodeBufferAddress)
    lLastPos = lCodeStartPosition - m_lCodeBufferAddress
    
    For i = 0 To lLastPos - 1
        bCodeBytes(i) = &HCC
    Next
       
    Select Case m_lVersion
    Case 4
        'push m_lTargetAddress+7
        AddByteToCode &H68, bCodeBytes(), lLastPos
        AddLongToCode m_lTargetAddress + 7, bCodeBytes(), lLastPos
        
        'mov edx,edi
        'neg edx
        'and edx,3
        For i = 0 To UBound(m_bOriEHBytes)
            AddByteToCode m_bOriEHBytes(i), bCodeBytes(), lLastPos
        Next
        
        'cmp ecx,edx
        AddByteStrToCode "3B CA", bCodeBytes(), lLastPos
        
        'jb exit
        AddByteStrToCode "72 02", bCodeBytes(), lLastPos
        
        'mov ecx,edx
        AddByteStrToCode "8B CA", bCodeBytes(), lLastPos
        
        'exit:
        'ret
        AddByteStrToCode "C3", bCodeBytes(), lLastPos
    
    Case 5
        'push m_lTargetAddress+6
        AddByteToCode &H68, bCodeBytes(), lLastPos
        AddLongToCode m_lTargetAddress + 6, bCodeBytes(), lLastPos
        '
        For i = 0 To UBound(m_bOriEHBytes)
            AddByteToCode m_bOriEHBytes(i), bCodeBytes(), lLastPos
        Next
              
        
'150:              push eax
'0040BB10 50                   push        eax
        AddByteStrToCode "50", bCodeBytes(), lLastPos
'151:              mov ecx, ebp
'0040BB11 8B CD                mov         ecx,ebp
        AddByteStrToCode "8B CD", bCodeBytes(), lLastPos
'152:              mov eax, esp
'0040BB13 8B C4                mov         eax,esp
        AddByteStrToCode "8B C4", bCodeBytes(), lLastPos
'153:              shr ecx,8h
'0040BB15 C1 E9 10             shr         ecx,10h
        AddByteStrToCode "C1 E9 8", bCodeBytes(), lLastPos
'154:              shr eax,8h
'0040BB18 C1 E8 10             shr         eax,10h
        AddByteStrToCode "C1 E8 8", bCodeBytes(), lLastPos
'155:              cmp eax, ecx
'0040BB1B 3B C1                cmp         eax,ecx
        AddByteStrToCode "3B C1", bCodeBytes(), lLastPos
'156:              jz v1
'0040BB1D 74 06                je          v1 (0040bb25)
        AddByteStrToCode "74 06", bCodeBytes(), lLastPos
'157:              lea ecx, [esp+30h]
'0040BB1F 8D 4C 24 2C          lea         ecx,[esp+30h]
        AddByteStrToCode "8D 4C 24 30", bCodeBytes(), lLastPos
'158:              jmp v2
'0040BB23 EB 03                jmp         v2 (0040bb28)
        AddByteStrToCode "EB 03", bCodeBytes(), lLastPos
'159:  v1:
'160:              lea ecx, [ebp+10h]
'0040BB25 8D 4D 10             lea         ecx,[ebp+10h]
        AddByteStrToCode "8D 4D 10", bCodeBytes(), lLastPos
'161:  v2:
'162:              cmp [ecx],0xffff
'0040BB28 80 39 FF             cmp         byte ptr [ecx],0FFh
        AddByteStrToCode "80 39 FF", bCodeBytes(), lLastPos
'163:              jnz exit
'0040BB2B 72 06                jne          exit (0040bb33)
        AddByteStrToCode "75 06", bCodeBytes(), lLastPos
'164:              mov dword ptr [ecx],0
'0040BB2D C7 01 00 00 00 00    mov         dword ptr [ecx],0
        AddByteStrToCode "C7 01 00 00 00 00", bCodeBytes(), lLastPos
'165:              exit:
'166:              xor ecx,ecx
'0040BB33 33 C9                xor         ecx,ecx
        AddByteStrToCode "33 C9", bCodeBytes(), lLastPos
'167:              pop eax
'0040BB35 58                   pop         eax
        AddByteStrToCode "58", bCodeBytes(), lLastPos
'168:              ret
'0040BB36 C3                   ret
        AddByteStrToCode "C3", bCodeBytes(), lLastPos
    
    End Select
    
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
    Select Case m_lVersion
    Case 4
        GetTargetAddress = LocateTarget(MAGIC_STRING4)
        If GetTargetAddress Then
            GetTargetAddress = GetTargetAddress + 6
        End If
    Case 5
        GetTargetAddress = LocateTarget(MAGIC_STRING5)
        If GetTargetAddress Then
            GetTargetAddress = GetTargetAddress
        End If
    End Select
End Function

Private Function LocateTarget(ByVal sTargeString As String) As Long
    On Error Resume Next
    Dim ppvData As Long
    Dim pvData As Long
    Dim ppSA As Long
    Dim pSA As Long
    Dim lVal As Long
    Dim lpAddr As Long
    Dim bTarget() As Byte
    
    Select Case m_lVersion
    Case 4
        ReDim bTarget(&H20000) As Byte
    Case 5
        ReDim bTarget(&H16C00) As Byte
    End Select
    Dim bFind() As Byte
    
    bFind = ByteCodeStrToBin(sTargeString)
    
    lpAddr = m_hWinABC
    
    If Err.Number <> 0 Then Exit Function
    
    ppSA = VarPtrArray(bTarget())
    pSA = GetArrayPtr(ppSA)
    ppvData = pSA + 12
    
    CopyMemory pvData, ByVal ppvData, 4
    CopyMemory ByVal ppvData, lpAddr, 4
    
    LocateTarget = InStrB(bTarget, bFind)
    
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
Private Function GetIMEVer() As Long
    Dim sVer As String
    sVer = GetVersionInfo("winabc.ime", efviProductVersion)
    If sVer <> "" Then
        GetIMEVer = Val(Left$(sVer, 1))
    End If
End Function
Private Sub Class_Initialize()
    m_hWinABC = LoadLibrary("winabc.ime")
    
    m_fIsWinABCPresent = (m_hWinABC <> 0)
    
    m_lVersion = GetIMEVer
    
    If m_fIsWinABCPresent And m_lVersion Then
        m_lTargetAddress = GetTargetAddress
    End If
End Sub

Private Sub Class_Terminate()
    If m_fIsWinABCPresent Then
        FreeLibrary m_hWinABC
    End If
End Sub
'******************************** 私有函数 ********************************

