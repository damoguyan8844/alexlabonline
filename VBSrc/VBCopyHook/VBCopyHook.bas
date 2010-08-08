Attribute VB_Name = "mduVBCopyHook"
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 超级绿豆
'
' ************************   模 块 名   :mduVBCopyHook********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年06月19日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年06月19日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "mduVBCopyHook"

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

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Private Declare Function lstrlenA Lib "kernel32" (ByVal lpString As Long) As Long
Private Declare Function lstrlenW Lib "kernel32" (ByVal lpString As Long) As Long

Private Declare Function lstrcpyA Lib "kernel32" (ByVal lpString1 As Long, ByVal lpString2 As Long) As Long
Private Declare Function lstrcpyW Lib "kernel32" (ByVal lpString1 As Long, ByVal lpString2 As Long) As Long

Private Declare Function IsBadCodePtr Lib "kernel32" (ByVal lpfn As Long) As Long

Private Declare Function VarPtrArray Lib "msvbvm60" Alias "VarPtr" (aArray() As Any) As Long

Public Const APP_NAME As String = "VBCopyHook"
Public Const SECTION_NAME As String = "Flags"

Public g_abCodesA() As Byte
Public g_abCodesW() As Byte
Public g_abPatchCodes(12) As Byte
Public g_lpPatchCodes As Long
Public g_fIsPatchPresent As Boolean


'************************************************************************************************************************************************************************************************************************************
'主要函数
'************************************************************************************************************************************************************************************************************************************

Public Function PatchFunction(ByVal lpLinklist As Long, ByVal lChoice As Long) As Long
    On Error Resume Next
    Dim abTarget(512) As Byte
    Dim abMagic(6) As Byte
    Dim abPatchCodes(6) As Byte
    
    PatchFunction = lChoice
        
    'pop edi
    'pop esi
    'pop ebx
    'leave
    'ret 24
    abMagic(0) = &H5F
    abMagic(1) = &H5E
    abMagic(2) = &H5B
    abMagic(3) = &HC9
    abMagic(4) = &HC2
    abMagic(5) = &H24
    abMagic(6) = &H0
    
    Dim lPatchPos As Long
    Dim lpAddr As Long
    
    lpAddr = lpLinklist
    
    'mov dword ptr [edi],0
    g_abPatchCodes(0) = &HC7
    g_abPatchCodes(1) = &H7
    g_abPatchCodes(2) = &H0
    g_abPatchCodes(3) = &H0
    g_abPatchCodes(4) = &H0
    g_abPatchCodes(5) = &H0
    
    'pop edi
    'pop esi
    'pop ebx
    'leave
    'ret 24
    CopyMemory g_abPatchCodes(6), abMagic(0), 7
    
    Call GetInfo
    
    If IsBadCodePtr(g_lpPatchCodes) <> 0 Then
        Call AllocMem
        Call SaveInfo
        ReadWriteMemory g_lpPatchCodes, g_abPatchCodes, False
    End If
    
    If ReadWriteMemory(lpAddr, abTarget()) Then
    
        lPatchPos = InStrB(abTarget, abMagic)
            
        If lPatchPos <> 0 Then
            lPatchPos = lpAddr + lPatchPos - 1
            
            lpAddr = g_lpPatchCodes - lPatchPos - 5
                        
            'jmp patchcodes
            abPatchCodes(0) = &HE9
            CopyMemory abPatchCodes(1), lpAddr, 4
            abPatchCodes(5) = &H0
            abPatchCodes(6) = &H0
            
            If ReadWriteMemory(lPatchPos, abPatchCodes(), False) Then
                
            End If
        End If
    End If
End Function

Public Function CopyCallbackA(ByVal this As Long, ByVal hwnd As Long, ByVal wFunc As Long, ByVal wFlags As Long, ByVal pszSrcFile As Long, ByVal dwSrcAttribs As Long, ByVal pszDestFile As Long, ByVal dwDestAttribs As Long) As Long
    On Error Resume Next
    CopyCallbackA = CopyCallback(this, hwnd, wFunc, wFlags, pszSrcFile, dwSrcAttribs, pszDestFile, dwDestAttribs, False)
End Function

Public Function CopyCallbackW(ByVal this As Long, ByVal hwnd As Long, ByVal wFunc As Long, ByVal wFlags As Long, ByVal pszSrcFile As Long, ByVal dwSrcAttribs As Long, ByVal pszDestFile As Long, ByVal dwDestAttribs As Long) As Long
    On Error Resume Next
    CopyCallbackW = CopyCallback(this, hwnd, wFunc, wFlags, pszSrcFile, dwSrcAttribs, pszDestFile, dwDestAttribs, True)
End Function


Public Function CopyCallback(ByVal this As Long, ByVal hwnd As Long, ByVal wFunc As Long, ByVal wFlags As Long, ByVal pszSrcFile As Long, ByVal dwSrcAttribs As Long, ByVal pszDestFile As Long, ByVal dwDestAttribs As Long, Optional fUnicode As Boolean = False) As Long
    On Error Resume Next
    Dim sMsg As String
    Dim sSrcFile As String, sDestFile As String
    sSrcFile = StrFromPtr(pszSrcFile, fUnicode)
    sDestFile = StrFromPtr(pszDestFile, fUnicode)

    sMsg = ""
    sMsg = sMsg & "父窗口句柄(hWnd):" & Hex(hwnd) & vbCrLf
    sMsg = sMsg & vbCrLf
    sMsg = sMsg & "操作名(wFunc):" & Hex(wFunc) & "->" & GetOpTypeStr(wFunc) & vbCrLf
    sMsg = sMsg & vbCrLf
    sMsg = sMsg & "操作标志(wFlags):" & Hex(wFlags) & "->" & GetOpFlagStr(wFlags) & vbCrLf
    sMsg = sMsg & vbCrLf
    sMsg = sMsg & "源文件(pszSrcFile):" & sSrcFile & vbCrLf
    sMsg = sMsg & "源文件属性(dwSrcAttribs):" & Hex(dwSrcAttribs) & "->" & GetAttrStr(dwSrcAttribs) & vbCrLf
    sMsg = sMsg & vbCrLf
    sMsg = sMsg & "目标文件(pszDestFile):" & sDestFile & vbCrLf
    sMsg = sMsg & "目标文件属性(dwDestAttribs):" & Hex(dwDestAttribs) & "->" & GetAttrStr(dwDestAttribs) & vbCrLf

    CopyCallback = MsgBox(sMsg, vbYesNoCancel + vbDefaultButton1 + vbQuestion, "****** VBCopyHook信息 ******")
End Function

'************************************************************************************************************************************************************************************************************************************
'主要函数
'************************************************************************************************************************************************************************************************************************************

'************************************************************************************************************************************************************************************************************************************
'以下是一些帮助函数
'************************************************************************************************************************************************************************************************************************************

Public Function GetKey() As String
    GetKey = Hex(GetCurrentProcessId)
End Function

Public Sub SaveInfo()
    SaveSetting APP_NAME, SECTION_NAME, GetKey, g_lpPatchCodes
End Sub

Public Sub GetInfo()
    On Error Resume Next
    Dim vSettings As Variant
    Dim sProcessID As String
    Dim sKey As String, sValue As String
    Dim i As Long
    
    sProcessID = GetKey
    g_lpPatchCodes = 0
    
    vSettings = GetAllSettings(APP_NAME, SECTION_NAME)
    
    For i = LBound(vSettings, 1) To UBound(vSettings, 1)
        sKey = vSettings(i, 0)
        sValue = vSettings(i, 1)
        If sKey = sProcessID Then
            g_lpPatchCodes = CLng(sValue)
        Else
            If CloseHandle(OpenProcess(PROCESS_ALL_ACCESS, 0, Val("&h" & sKey))) Then
                DeleteSetting APP_NAME, SECTION_NAME, sKey
            End If
        End If
    Next
End Sub

Public Sub AllocMem()
    g_lpPatchCodes = VirtualAlloc(ByVal 0, 128, MEM_COMMIT, PAGE_READWRITE)
End Sub

Public Sub PrepareCodes()
    Dim sCodes As String
    Dim lRetPos1 As Long
    Dim lRetPos2 As Long
    sCodes = "FF 74 24 20 FF 74 24 20 FF 74 24 20 FF 74 24 20 FF 74 24 20 FF 74 24 20 FF 74 24 20 FF 74 24 20 "
    sCodes = sCodes & "E8 00 00 00 00 "
    lRetPos1 = 4 * 8 + 5
    sCodes = sCodes & "50 "
    sCodes = sCodes & "FF 74 24 04 "
    sCodes = sCodes & "E8 00 00 00 00 "
    lRetPos2 = 4 * 8 + 5 + 5 + 3 + 1 + 1
    sCodes = sCodes & "C2 20 00"
    
    Call CodeStrToByteArray(sCodes, g_abCodesA)
    Call CodeStrToByteArray(sCodes, g_abCodesW)
    
    CopyMemory g_abCodesA(4 * 8 + 1), FuncPtr(AddressOf CopyCallbackA) - VarPtr(g_abCodesA(lRetPos1)), 4
    CopyMemory g_abCodesW(4 * 8 + 1), FuncPtr(AddressOf CopyCallbackW) - VarPtr(g_abCodesW(lRetPos1)), 4
    
    CopyMemory g_abCodesA(4 * 8 + 5 + 3 + 1 + 1 + 1), FuncPtr(AddressOf PatchFunction) - VarPtr(g_abCodesA(lRetPos2)), 4
    CopyMemory g_abCodesW(4 * 8 + 5 + 3 + 1 + 1 + 1), FuncPtr(AddressOf PatchFunction) - VarPtr(g_abCodesW(lRetPos2)), 4
End Sub

Public Sub CodeStrToByteArray(ByVal sCodes As String, abCodes() As Byte)
    Dim v As Variant
    v = Split(sCodes, " ")
    
    Dim i As Long
    ReDim abCodes(UBound(v))
    For i = 0 To UBound(v)
        abCodes(i) = CByte("&h" & v(i))
    Next
End Sub


Public Function ReadWriteMemory(ByVal lAddr As Long, buff() As Byte, Optional fRead As Boolean = True, Optional ByVal lProcessID As Long = -1) As Boolean
    Dim hProcess As Long
    Dim mi As MEMORY_BASIC_INFORMATION
    Dim lpAddress As Long, lOldProtect As Long
    Dim lBytesReadWrite As Long
    Dim bTmp() As Byte
    
    lpAddress = lAddr
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0&, IIf(lProcessID = -1, GetCurrentProcessId, lProcessID))
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

Public Function FuncPtr(ByVal lpFunc As Long) As Long
    FuncPtr = lpFunc
End Function

Public Function GetLngValue(ByVal lpAddr As Long) As Long
    CopyMemory GetLngValue, ByVal lpAddr, 4
End Function

Public Function StrFromPtr(ByVal lpString As Long, Optional fUnicode As Boolean = False) As String
    On Error Resume Next
    If fUnicode Then
        StrFromPtr = String(lstrlenW(lpString), Chr(0))
        lstrcpyW StrPtr(StrFromPtr), ByVal lpString
    Else
        StrFromPtr = String(lstrlenA(lpString), Chr(0))
        lstrcpyA ByVal StrFromPtr, ByVal lpString
    End If
End Function


Public Function ReplaceVTableEntry(ByVal lpObject As Long, ByVal lEntryNumber As Long, ByVal lpNewEntry As Long) As Long
    Dim lpVTable As Long
    Dim lpEntryPonit As Long
    Dim b(3) As Byte
    
    lpVTable = GetLngValue(lpObject)
    lpEntryPonit = (lEntryNumber - 1) * 4
    
    CopyMemory ByVal VarPtr(b(0)), lpNewEntry, 4
    ReadWriteMemory lpVTable + lpEntryPonit, b(), False
    CopyMemory ReplaceVTableEntry, ByVal VarPtr(b(0)), 4

End Function

Public Function GetOpTypeStr(ByVal lOpType As eOperationType) As String
    On Error Resume Next
    Select Case lOpType
    Case eOperationType.FO_COPY
        GetOpTypeStr = "复制"
    Case eOperationType.FO_DELETE
        GetOpTypeStr = "删除"
    Case eOperationType.FO_MOVE
        GetOpTypeStr = "移动"
    Case eOperationType.FO_RENAME
        GetOpTypeStr = "重命名"
    Case eOperationType.PO_DELETE
        GetOpTypeStr = "打印机删除"
    Case eOperationType.PO_PORTCHANGE
        GetOpTypeStr = "打印机端口改变"
    Case eOperationType.PO_REN_PORT
        GetOpTypeStr = "打印机端口名字改变"
    Case eOperationType.PO_RENAME
        GetOpTypeStr = "打印机重命名"
    Case Else
        GetOpTypeStr = "未知"
    End Select
End Function

Public Function GetOpFlagStr(ByVal lOpFlag As Long) As String
    On Error Resume Next
    Dim i As Long
    Dim asTmp() As String
    
    ReDim asTmp(12)
    
    i = 0
    
    If (lOpFlag And eOperationFlags.FOF_ALLOWUNDO) <> 0 Then
        asTmp(i) = "FOF_ALLOWUNDO"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_CONFIRMMOUSE) <> 0 Then
        asTmp(i) = "FOF_CONFIRMMOUSE"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_FILESONLY) <> 0 Then
        asTmp(i) = "FOF_FILESONLY"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_MULTIDESTFILES) <> 0 Then
        asTmp(i) = "FOF_MULTIDESTFILES"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_NOCONFIRMATION) <> 0 Then
        asTmp(i) = "FOF_NOCONFIRMATION"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_NOCONFIRMMKDIR) <> 0 Then
        asTmp(i) = "FOF_NOCONFIRMMKDIR"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_NOCOPYSECURITYATTRIBS) <> 0 Then
        asTmp(i) = "FOF_NOCOPYSECURITYATTRIBS"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_NOERRORUI) <> 0 Then
        asTmp(i) = "FOF_NOERRORUI"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_RENAMEONCOLLISION) <> 0 Then
        asTmp(i) = "FOF_RENAMEONCOLLISION"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_SILENT) <> 0 Then
        asTmp(i) = "FOF_SILENT"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_SIMPLEPROGRESS) <> 0 Then
        asTmp(i) = "FOF_SIMPLEPROGRESS"
        i = i + 1
    End If
    If (lOpFlag And eOperationFlags.FOF_WANTMAPPINGHANDLE) <> 0 Then
        asTmp(i) = "FOF_WANTMAPPINGHANDLE"
        i = i + 1
    End If
    
    ReDim Preserve asTmp(i - 1)
    GetOpFlagStr = Join(asTmp, ",")
End Function

Public Function GetAttrStr(ByVal lFileAttr As Long) As String
    On Error Resume Next
    GetAttrStr = ""
    If (lFileAttr And FILE_ATTRIBUTE_ARCHIVE) <> 0 Then
        GetAttrStr = GetAttrStr & "文档,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_COMPRESSED) <> 0 Then
        GetAttrStr = GetAttrStr & "压缩,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_DIRECTORY) <> 0 Then
        GetAttrStr = GetAttrStr & "目录,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_ENCRYPTED) <> 0 Then
        GetAttrStr = GetAttrStr & "加密,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_HIDDEN) <> 0 Then
        GetAttrStr = GetAttrStr & "隐藏,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_NORMAL) <> 0 Then
        GetAttrStr = GetAttrStr & "普通,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) <> 0 Then
        GetAttrStr = GetAttrStr & "未索引,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_OFFLINE) <> 0 Then
        GetAttrStr = GetAttrStr & "离线,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_READONLY) <> 0 Then
        GetAttrStr = GetAttrStr & "只读,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_REPARSE_POINT) <> 0 Then
        GetAttrStr = GetAttrStr & "REPARSE_POINT,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_SPARSE_FILE) <> 0 Then
        GetAttrStr = GetAttrStr & "SPARSE_FILE,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_SYSTEM) <> 0 Then
        GetAttrStr = GetAttrStr & "系统,"
    End If
    If (lFileAttr And FILE_ATTRIBUTE_TEMPORARY) <> 0 Then
        GetAttrStr = GetAttrStr & "临时,"
    End If
    If Right(GetAttrStr, 1) = "," Then
        GetAttrStr = Left$(GetAttrStr, Len(GetAttrStr) - 1)
    End If
End Function
