Attribute VB_Name = "mduBuildControl"
Option Explicit
Private Declare Function GetSaveFileName Lib "comdlg32.dll" Alias "GetSaveFileNameA" (pOpenfilename As OPENFILENAME) As Long

Private Type OPENFILENAME
        lStructSize As Long
        hwndOwner As Long
        hInstance As Long
        lpstrFilter As String
        lpstrCustomFilter As String
        nMaxCustFilter As Long
        nFilterIndex As Long
        lpstrFile As String
        nMaxFile As Long
        lpstrFileTitle As String
        nMaxFileTitle As Long
        lpstrInitialDir As String
        lpstrTitle As String
        flags As Long
        nFileOffset As Integer
        nFileExtension As Integer
        lpstrDefExt As String
        lCustData As Long
        lpfnHook As Long
        lpTemplateName As String
End Type

Private Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyW" (ByVal lpString1 As String, ByVal lpString2 As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
Private Declare Function GetTempFileName Lib "kernel32" Alias "GetTempFileNameA" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
Private Declare Function GetTempPath Lib "kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long

Public g_fBuildDLL As Boolean
Public g_vStartUpObject As Variant
Public g_oBuildHook As New CVBBuildHook
Public VBInstance As VBIDE.VBE
Public g_sApp As String
Public g_sCommandLine As String
Public g_sDEFFileName As String
Public g_sBuildFileName As String

Function NoGap(sIn As String) As String '祛除空格及CHR$(0)
    On Error GoTo staErr
        Dim sTmp As String
        Dim lPosOfZero As Long
        Dim lLen As Long
        sTmp = sIn
        lLen = Len(sTmp)
        lPosOfZero = InStr(sTmp, Chr$(0))
        If lPosOfZero Then sTmp = Left$(sTmp, lPosOfZero - 1)
        NoGap = Trim$(sTmp)
    Exit Function
staErr:
End Function

Function GetSaveFile(Optional sFilename As String, Optional ByVal sFilter As String = "All Files|*.*", Optional sDiaTitle As String, Optional sFileTitle As String) As String
'sFilter: All Files|*.*;TextFile|*.txt
Dim udt As OPENFILENAME, s As String
With udt
    .hwndOwner = 0&
    .hInstance = App.hInstance
    .lpstrFilter = Replace(Replace(sFilter, "|", Chr(0)), ";", ";" + Chr(0))
    .nFilterIndex = 1
    .lpstrFileTitle = String(1000, Chr(0))
    .nMaxFileTitle = 1000
    .lpstrFile = String(1000, Chr(0))
    If sFilename <> "" Then
        Mid(.lpstrFile, 1, Len(sFilename)) = sFilename
    End If
    .nMaxFile = 1000
    If IsMissing(sDiaTitle) Then
        s = ""
    Else
        s = sDiaTitle
    End If
    .lpstrTitle = s
    .lStructSize = Len(udt)
End With

If GetSaveFileName(udt) Then
    sFileTitle = NoGap(udt.lpstrFileTitle)
    GetSaveFile = NoGap(udt.lpstrFile)
End If

End Function

Public Sub BuildHookProc_Before(ByVal lplpApplication As Long, ByVal lplpCommandLine As Long)
    Dim lpCommandLine As Long, lpApplication  As Long
    CopyMemory lpCommandLine, ByVal lplpCommandLine, 4
    CopyMemory lplpApplication, ByVal lplpApplication, 4
    
    g_sApp = StrFromPtr(lplpApplication)
    g_sCommandLine = StrFromPtr(lpCommandLine)
    
    '如果是编译DLL，那就执行下面的语句
    If Not g_fBuildDLL Then Exit Sub
    
    If Left(g_sCommandLine, 4) = "LINK" Then
        g_fBuildDLL = False
        If MakeDEF Then
            g_sCommandLine = Replace(g_sCommandLine, "/ENTRY:__vbaS", "/ENTRY:DllMain")
            g_sCommandLine = Replace(g_sCommandLine, "/BASE:0x400000", "/BASE:0x10000000")
            g_sCommandLine = g_sCommandLine + " /DLL " + "/DEF:" + """" + g_sDEFFileName + """"
            CopyMemory ByVal lplpCommandLine, g_sCommandLine, 4
            
            On Error Resume Next
            Set VBInstance.ActiveVBProject.VBComponents.StartUpObject = g_vStartUpObject
            VBInstance.ActiveVBProject.VBComponents.StartUpObject = g_vStartUpObject
            On Error GoTo 0
        End If
    End If
End Sub

Public Sub BuildHookProc_After(ByVal hProcess As Long)
    'hProcess为CreateProcess后得到的进程句柄
    
    '如果是编译DLL，那就执行下面的语句
    'If Not g_fBuildDLL Then Exit Sub
End Sub

'从指针获得ANSI字符串
Public Function StrFromPtr(ByVal lpString As Long) As String
    On Error Resume Next
    StrFromPtr = String(1024, Chr(0))
    CopyMemory ByVal StrFromPtr, ByVal lpString, Len(StrFromPtr)
    StrFromPtr = Left(StrFromPtr, InStr(StrFromPtr, Chr(0)) - 1)
End Function





Public Function AddTemplateModule() As Boolean
    On Error GoTo ErrHandler
    Dim oPrj As VBIDE.VBProject
    Dim oComponent As VBIDE.VBComponent
    Dim oCodeModule As VBIDE.CodeModule
    Dim sTemplate As String
    
    
    '添加模版
    Set oPrj = VBInstance.ActiveVBProject
    
    If oPrj Is Nothing Then Exit Function
    
    With oPrj
        On Error Resume Next
        Set oComponent = .VBComponents.Add(vbext_ct_StdModule)
        oComponent.Name = "mduDLL"
        If Err.Number <> 0 Then
            MsgBox "已有名为mduDLL的模块存在！"
            .VBComponents.Remove oComponent
            Exit Function
        End If
        On Error GoTo ErrHandler
    End With
    
    Set oCodeModule = oComponent.CodeModule
    
    sTemplate = "'VBDll 模版" & vbCrLf
'    sTemplate = sTemplate & "Option Explicit" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "Public Const DLL_PROCESS_ATTACH As Long = 1" & vbCrLf
    sTemplate = sTemplate & "Public Const DLL_THREAD_ATTACH As Long = 2" & vbCrLf
    sTemplate = sTemplate & "Public Const DLL_PROCESS_DETACH As Long = 0" & vbCrLf
    sTemplate = sTemplate & "Public Const DLL_THREAD_DETACH As Long = 3" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "'假起始点" & vbCrLf
    sTemplate = sTemplate & "Private Sub Main()" & vbCrLf
    sTemplate = sTemplate & "End Sub" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "'Dll入口点函数" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "Public Function DllMain(ByVal hinstDLL As Long, ByVal fdwReason As Long, ByVal lpvReserved As Long) As Long" & vbCrLf
    sTemplate = sTemplate & "    Select Case fdwReason" & vbCrLf
    sTemplate = sTemplate & "    Case DLL_PROCESS_ATTACH" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "    Case DLL_PROCESS_DETACH" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "    Case DLL_THREAD_ATTACH" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "    Case DLL_THREAD_DETACH" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "    End Select" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "    DllMain=True" & vbCrLf
    sTemplate = sTemplate & "End Function" & vbCrLf
    sTemplate = sTemplate & "" & vbCrLf
    sTemplate = sTemplate & "'在这里添加你的代码" & vbCrLf
    
    With oCodeModule
        .AddFromString sTemplate
    End With
    
    AddTemplateModule = True
    Exit Function
ErrHandler:
    
End Function




Public Function GetFunctionList() As Collection
    On Error GoTo ErrHandler
    Dim oCodeModule As VBIDE.CodeModule
    Dim oCodePane As VBIDE.CodePane
    Dim oMember As VBIDE.Member
    Dim sName As String
    Dim lStart As Long
    
    Set GetFunctionList = New Collection
    
    For Each oCodePane In VBInstance.CodePanes
        Set oCodeModule = oCodePane.CodeModule
        
        lStart = oCodeModule.CountOfDeclarationLines
        
        For Each oMember In oCodeModule.Members
            With oMember
                '获得所有除声明部分外的所有公共函数、过程
                If .Scope = vbext_Public And .Type = vbext_mt_Method And .CodeLocation > lStart Then
                    sName = .Name
                    GetFunctionList.Add sName
                End If
            End With
        Next
    Next
    
    Exit Function
ErrHandler:
    MsgBox Err.Description
End Function



Public Function MakeDEF() As Boolean
    Dim lFreeFile As Long
    Dim col As Collection
    Dim vName As Variant
    
    lFreeFile = FreeFile
    
    g_sDEFFileName = GetTmpFile
    
    If g_sDEFFileName <> "" Then
        Set col = GetFunctionList
        If col.Count = 0 Then
            Exit Function
        End If
        
        Open g_sDEFFileName For Output As #lFreeFile


        Print #lFreeFile, "LIBRARY VBTRUEDLL"
        Print #lFreeFile, "Description ""VB True Dll"""
        Print #lFreeFile, "EXPORTS"
        
        For Each vName In col
            Print #lFreeFile, "  " & vName
        Next
        
        Close #lFreeFile
        
        MakeDEF = True
    End If
    
    
End Function

Public Function GetTmpFile() As String
    On Error Resume Next
    Dim s As String
    Dim l As Long
    s = String(1024, Chr(0))
    l = GetTempPath(Len(s), s)
    s = Left$(s, InStr(s, Chr(0)) - 1)
    GetTmpFile = s & "JVBDLL.DEF"
End Function




