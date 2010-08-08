VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBCallPatch"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'------------------------------ 类模块 CVBCallPatch.cls ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBCallPatch"

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

Private Declare Function VarPtrArray Lib "msvbvm60" Alias "VarPtr" (aArray() As Any) As Long

Private m_lTargetCodeAddress As Long
Private m_abPatchCodes() As Byte
Private m_fPatched As Boolean

Public Function DoPatch() As Boolean
    DoPatch = True
    If m_fPatched Then Exit Function
    m_lTargetCodeAddress = LocateTarget() + 2
    DoPatch = ReadWriteMemory(m_lTargetCodeAddress, m_abPatchCodes, False)
    m_fPatched = DoPatch
End Function

Public Function UndoPatch() As Boolean
    If m_fPatched Then
        UndoPatch = ReadWriteMemory(m_lTargetCodeAddress, m_abPatchCodes, False)
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

Private Function LocateTarget() As Long
    On Error Resume Next
    Dim ppvData As Long
    Dim pvData As Long
    Dim ppSA As Long
    Dim pSA As Long
    Dim lVal As Long
    Dim lpAddr As Long
    
    Dim bTarget(&H190000) As Byte
    Dim bFind() As Byte
       
    bFind = ByteCodeStrToBin("FF D0 3B FC 0F")
    
    lpAddr = GetModuleHandle("vba6.dll")
    
    If Err.Number <> 0 Then Exit Function
    
    ppSA = VarPtrArray(bTarget())
    pSA = GetArrayPtr(ppSA)
    ppvData = pSA + 12
    
    CopyMemory pvData, ByVal ppvData, 4
    CopyMemory ByVal ppvData, lpAddr, 4
    
    LocateTarget = 0
    
    Dim i As Long
    i = 0
    Do
        LocateTarget = InStrB(LocateTarget + 1, bTarget, bFind)
        If LocateTarget = 0 Then Exit Do
        If bTarget(LocateTarget - 7) = &HF Then
            If i = 0 Then
                i = i + 1
            Else
                Exit Do
            End If
        End If
    Loop
    
    CopyMemory ByVal ppvData, pvData, 4
    
    If LocateTarget > 0 Then
        LocateTarget = LocateTarget + lpAddr - 1
    End If
End Function

Private Function GetArrayPtr(ByVal ppSA As Long) As Long
    CopyMemory GetArrayPtr, ByVal ppSA, 4
End Function

Private Sub Class_Initialize()
    m_fPatched = False
    ReDim m_abPatchCodes(7) As Byte
    m_abPatchCodes(0) = &H8B 'mov esp,edi
    m_abPatchCodes(1) = &HE7 'nop...
    m_abPatchCodes(2) = &H90
    m_abPatchCodes(3) = &H90
    m_abPatchCodes(4) = &H90
    m_abPatchCodes(5) = &H90
    m_abPatchCodes(6) = &H90
    m_abPatchCodes(7) = &H90
End Sub

