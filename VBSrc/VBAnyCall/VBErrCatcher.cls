VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBErrCatcher"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------ 类模块 VBErrCatcher.cls ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBErrCatcher"

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

Private m_oAnyCall As CVBAnyCall
Private m_fIsPCode As Boolean
Private m_lTargetAddress As Long
Private m_lEHAddress As Long
Private m_lWhereAmI As VBEC_ENUM_WHEREAMI
Private m_lhModVBA6 As Long
Private m_lhModMSVBVM As Long
Private m_bOriEHBytes() As Byte
Private m_lCodeBufferAddress As Long
Private m_lCodeStartAddress As Long
Private m_lJumpAddress As Long
Private m_fIsInstalled As Boolean

Private Const CODE_BUFFER_SIZE As Long = 512
Private Const MAGIC_STRING As String = "E8 16 00 00 00 59 58 8B 59 1C 8B 71 14 8B 79 18 8B 61 0C 8B 69 10 8B 51 08 FF E2"

'******************************* 暴露的接口 *******************************
'安装自定义错误处理过程
'lMyEHAddress:自定义错误处理过程地址
Public Function InstallEH(ByVal lMyEHAddress As Long) As Boolean
    Select Case m_lWhereAmI
    Case evaNative
        ReDim m_bOriEHBytes(5)
        Call ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes())
        m_lCodeStartAddress = PrepareCode(lMyEHAddress)
        
        If m_lCodeStartAddress = 0 Then Exit Function
        
        m_lJumpAddress = VarPtr(m_lCodeStartAddress)
    
        CopyMemory m_bOriEHBytes(2), m_lJumpAddress, 4
    
        m_bOriEHBytes(0) = &HFF
        m_bOriEHBytes(1) = &H25
    Case evaDev, evaPCode
        ReDim m_bOriEHBytes(4)
        Call ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes())
        m_lCodeStartAddress = PrepareCode(lMyEHAddress)
        
        If m_lCodeStartAddress = 0 Then Exit Function
        
        CopyMemory m_bOriEHBytes(1), m_lCodeStartAddress - m_lTargetAddress - 5, 4
        m_bOriEHBytes(0) = &HE8
    Case Else
        Exit Function
    End Select
    
    If ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes(), False) Then     '写入我们的地址
        InstallEH = True
        m_fIsInstalled = True
    End If
End Function

'卸载自定义错误处理过程
Public Function UninstallEH() As Boolean
    If Not m_fIsInstalled Then Exit Function
    
    Select Case m_lWhereAmI
    Case evaNative, evaDev, evaPCode
        If ReadWriteMemory(m_lTargetAddress, m_bOriEHBytes(), False) Then    '恢复原来的地址
            If MemOp(False) Then
                UninstallEH = True
            End If
        End If
    End Select
End Function

'是否已安装自定义错误处理过程
Public Property Get IsInstalled() As Boolean
    IsInstalled = m_fIsInstalled
End Property

'当前程序的编译方式以及运行环境
'取参考枚举变量 VBEC_ENUM_WHEREAMI
Public Property Get WhereAmI() As Long
    WhereAmI = m_lWhereAmI
End Property

'当前程序是否为P代码方式编译
Public Property Get IsPCode() As Boolean
    IsPCode = m_fIsPCode
End Property

'VB的错误处理程序地址
Public Property Get EHAddress() As Long
    EHAddress = m_lEHAddress
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
Private Property Let EHAddress(ByVal lAddr As Long)
    m_lEHAddress = lAddr
    
    If (m_lhModVBA6 <> 0 And m_lEHAddress >= m_lhModVBA6) Then
        m_lWhereAmI = evaDev
        Exit Property
    End If
    
    If (m_lhModMSVBVM <> 0 And m_lEHAddress >= m_lhModMSVBVM) Then
        m_lWhereAmI = evaPCode
        m_fIsPCode = True
        Exit Property
    End If
   
    m_lWhereAmI = evaNative
End Property

Private Function GetEHAddress() As Long
    Dim sByteCode As String
    
    '64 A1 00 00 00 00 mov eax,fs:[00000000]
    sByteCode = "64 A1 00 00 00 00 "
    '8B 40 04          mov eax,dword ptr [eax+4]
    sByteCode = sByteCode & "8B 40 04 "
    'C3                ret
    sByteCode = sByteCode & "C3"
    
    GetEHAddress = m_oAnyCall.CallCodeBytes(sByteCode)
    
    EHAddress = GetEHAddress
End Function

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

Private Function PrepareCode(ByVal lMyEHAddress As Long) As Long
    If Not MemOp() Then Exit Function

    Dim lCodeStartPosition As Long, lLastPos As Long
    Dim bCodeBytes(CODE_BUFFER_SIZE - 1) As Byte
    Dim i As Long
        
    lCodeStartPosition = GetAlignedlCodeStartPosition(m_lCodeBufferAddress)
    lLastPos = lCodeStartPosition - m_lCodeBufferAddress
    
    For i = 0 To lLastPos - 1
        bCodeBytes(i) = &HCC
    Next
       
    Select Case m_lWhereAmI
    Case evaNative
        'int 3 ;测试用
        'AddByteToCode &HCC, bCodeBytes(), lLastPos
        
        'call lMyEHAddress
        AddByteToCode &HE8, bCodeBytes(), lLastPos
        AddLongToCode lMyEHAddress - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
                
        'jmp m_lOriEHAddressVBA
        CopyMemory bCodeBytes(lLastPos), m_bOriEHBytes(0), UBound(m_bOriEHBytes) + 1
        lLastPos = lLastPos + UBound(m_bOriEHBytes) + 1
    Case evaDev
        'push VBA6!StdThrow
        AddByteToCode &H68, bCodeBytes(), lLastPos
        AddLongToCode m_lTargetAddress + 5, bCodeBytes(), lLastPos
        'call lMyEHAddress
        AddByteToCode &HE8, bCodeBytes(), lLastPos
        AddLongToCode lMyEHAddress - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
        'ret
        AddByteToCode &HC3, bCodeBytes(), lLastPos
    Case evaPCode
        
        'call lMyEHAddress
        AddByteToCode &HE8, bCodeBytes(), lLastPos
        AddLongToCode lMyEHAddress - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
        
        'pop eax
        AddByteToCode &H58, bCodeBytes(), lLastPos
        
        'call unwind
        AddByteToCode &HE8, bCodeBytes(), lLastPos
        AddLongToCode m_lTargetAddress + 27 - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
        
        'jmp back
        AddByteToCode &HE9, bCodeBytes(), lLastPos
        AddLongToCode m_lTargetAddress + 5 - (m_lCodeBufferAddress + VarPtr(bCodeBytes(lLastPos)) - VarPtr(bCodeBytes(0))) - 4, bCodeBytes(), lLastPos
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
    asCodeByte = Split(sCodeBytes, " ")
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
    Select Case m_lWhereAmI
    Case evaNative
        GetTargetAddress = m_lEHAddress
    Case evaDev
        GetTargetAddress = GetProcAddress(m_lhModVBA6, "StdThrow") - 5
    Case evaPCode
        GetTargetAddress = LocateTarget
    Case Else
        Exit Function
    End Select
End Function

Private Function LocateTarget() As Long
    On Error Resume Next
    Dim ppvData As Long
    Dim pvData As Long
    Dim ppSA As Long
    Dim pSA As Long
    Dim lVal As Long
    Dim lpAddr As Long
    
    Dim bTarget(&H110000) As Byte
    Dim bFind() As Byte
    
    If m_lWhereAmI <> evaPCode Then Exit Function
    
    bFind = m_oAnyCall.ByteCodeStrToBin(MAGIC_STRING)
    
    lpAddr = m_lhModMSVBVM
    
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

Private Function SysDir() As String
    Dim s As String * 255
    Dim l As Long
    l = GetSystemDirectory(s, 255)
    SysDir = Left(s, l)
End Function
Private Function GetRegionSize(ByVal lAddr As Long) As Long
    Dim hProcess As Long
    Dim mi As MEMORY_BASIC_INFORMATION
    Dim lpAddress As Long
    Dim bTmp() As Byte
    
    lpAddress = lAddr
    
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0&, GetCurrentProcessId)
    If hProcess Then
        If VirtualQueryEx(hProcess, ByVal lpAddress, mi, Len(mi)) Then
            Stop
            GetRegionSize = mi.RegionSize
        End If
        CloseHandle hProcess
    End If
End Function
Private Sub Class_Initialize()
    m_lhModVBA6 = GetModuleHandle("vba6.dll")
    m_lhModMSVBVM = GetModuleHandle("msvbvm60.dll")
    
    Set m_oAnyCall = New CVBAnyCall
    With m_oAnyCall
        .IsStandardCall = False
        .ThroughVTable = True
    End With
    
    Call GetEHAddress
    m_lTargetAddress = GetTargetAddress
End Sub

Private Sub Class_Terminate()
    Set m_oAnyCall = Nothing
End Sub
'******************************** 私有函数 ********************************

