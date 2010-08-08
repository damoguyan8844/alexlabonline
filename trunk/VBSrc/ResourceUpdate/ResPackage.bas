Attribute VB_Name = "mduResPackage"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :mduResPackage********************
'版 本 号: V1.0
'作    者: 陈缘
'生成日期: 2004年03月07日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月07日
'修改人员: 陈缘
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "mduResPackage"




Public Function GetResTypeStr(ByVal lType As Long) As String
    If (lType And &HFFFF0000) <> 0 Then
        GetResTypeStr = StrFromPtr(lType)
    Else
        Select Case lType
        Case RT_ACCELERATOR
            GetResTypeStr = "RT_ACCELERATOR"
        Case RT_ANICURSOR
            GetResTypeStr = "RT_ANICURSOR"
        Case RT_ANIICON
            GetResTypeStr = "RT_ANIICON"
        Case RT_BITMAP
            GetResTypeStr = "RT_BITMAP"
        Case RT_CURSOR
            GetResTypeStr = "RT_CURSOR"
        Case RT_DIALOG
            GetResTypeStr = "RT_DIALOG"
        Case RT_DLGINCLUDE
            GetResTypeStr = "RT_DLGINCLUDE"
        Case RT_FONT
            GetResTypeStr = "RT_FONT"
        Case RT_FONTDIR
            GetResTypeStr = "RT_FONTDIR"
        Case RT_GROUP_CURSOR
            GetResTypeStr = "RT_GROUP_CURSOR"
        Case RT_GROUP_ICON
            GetResTypeStr = "RT_GROUP_ICON"
        Case RT_HTML
            GetResTypeStr = "RT_HTML"
        Case RT_ICON
            GetResTypeStr = "RT_ICON"
        Case RT_MENU
            GetResTypeStr = "RT_MENU"
        Case RT_MESSAGETABLE
            GetResTypeStr = "RT_MESSAGETABLE"
        Case RT_PLUGPLAY
            GetResTypeStr = "RT_PLUGPLAY"
        Case RT_RCDATA
            GetResTypeStr = "RT_RCDATA"
        Case RT_STRING
            GetResTypeStr = "RT_STRING"
        Case RT_VERSION
            GetResTypeStr = "RT_VERSION"
        Case RT_VXD
            GetResTypeStr = "RT_VXD"
        Case Else
            GetResTypeStr = "#" & CStr(lType)
        End Select
    End If
End Function

Public Function GetResNameStr(ByVal lName As Long) As String
    If (lName And &HFFFF0000) <> 0 Then
        GetResNameStr = StrFromPtr(lName)
    Else
        GetResNameStr = "#" & CStr(lName)
    End If
End Function

Public Function GetLangStr(ByVal wLang As Integer) As String
    Dim lLen As Long
    If wLang = 0 Then
        GetLangStr = "无"
        Exit Function
    End If
    lLen = GetLocaleInfo(LocaleIDFromLangID(wLang), LOCALE_SLANGUAGE, 0&, 0&)
    Dim s As String
    s = Space$(lLen)
    lLen = GetLocaleInfo(LocaleIDFromLangID(wLang), LOCALE_SLANGUAGE, s, lLen)
    GetLangStr = RTrim(Replace(s, Chr(0), " "))
End Function

Public Function LocaleIDFromLangID(ByVal wLang As Integer) As Long
    LocaleIDFromLangID = CLng(wLang)
End Function


Public Function StrFromPtr(ByVal lpStr As Long) As String
    Dim lLen As Long, l
    Dim b(255) As Byte
    
    lstrcpy ByVal VarPtr(b(0)), ByVal lpStr
    
    StrFromPtr = StrConv(b, vbUnicode)
    StrFromPtr = RTrim(Replace(StrFromPtr, Chr(0), Chr(32)))
    
End Function

Public Function StringsFromTable(bStrings() As Byte) As String
    On Error Resume Next
    Dim lLen As Long
    Dim i As Long
    Dim bTmp() As Byte
    Dim bLen As Byte
    Dim sTmp As String
    Dim sResult As String
    
    If UBound(bStrings) - LBound(bStrings) + 1 = 0 Then Exit Function
    
    sResult = ""
    For i = 0 To UBound(bStrings) Step 2
        bLen = bStrings(i)
        If bLen > 0 Then
            sTmp = Space$(bLen * 2)
            CopyMemory ByVal StrPtr(sTmp), ByVal VarPtr(bStrings(i + 2)), bLen * 2
            sResult = sResult & vbCrLf & sTmp
            i = i + bLen * 2
        End If
    Next
    
    StringsFromTable = Mid(sResult, 3)
    'Debug.Print sTmp
End Function

Public Function VersionInfoFromRes(bVersion() As Byte) As String
    Dim i As Long
    Call InitFileVersionInfoNames
    For i = 0 To UBound(g_FileVersionInfoEntryNames) - 1
        VersionInfoFromRes = VersionInfoFromRes & g_FileVersionInfoEntryNames(i) & ":" & GetVersionInfoFromResVerBytes(bVersion(), i) & vbCrLf
    Next
End Function

