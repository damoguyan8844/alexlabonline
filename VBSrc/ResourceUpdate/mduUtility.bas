Attribute VB_Name = "mduUtility"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :mduUtility********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年03月07日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月07日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "mduUtility"


Public Type OPENFILENAME
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


Public Declare Function GetOpenFileName Lib "comdlg32.dll" Alias "GetOpenFileNameA" (pOpenfilename As OPENFILENAME) As Long

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

Function GetOpenFile(frm As Form, Optional ByVal sFilter As String = "All Files|*.*", Optional sDiaTitle As String, Optional sFileTitle As String) As String
'sFilter: All Files|*.*;TextFile|*.txt
Dim udt As OPENFILENAME, s As String
With udt
    .hwndOwner = frm.hWnd
    .hInstance = App.hInstance
    .lpstrFilter = Replace(Replace(sFilter, "|", Chr(0)), ";", ";" + Chr(0))
    .nFilterIndex = 1
    .lpstrFileTitle = String(1000, Chr(0))
    .nMaxFileTitle = 1000
    .lpstrFile = String(1000, Chr(0))
    .nMaxFile = 1000
    If IsMissing(sDiaTitle) Then
        s = ""
    Else
        s = sDiaTitle
    End If
    .lpstrTitle = s
    .lStructSize = Len(udt)
End With
If GetOpenFileName(udt) Then
    sFileTitle = NoGap(udt.lpstrFileTitle)
    GetOpenFile = NoGap(udt.lpstrFile)
End If

End Function
