Attribute VB_Name = "MisFunctions"
Option Explicit

Public Function GetExcelApp(Optional ByVal CreateNew As Boolean = False) As Object
    ' return True if Create new App
    Set GetExcelApp = Nothing
    On Error Resume Next
    If Not CreateNew Then
        Set GetExcelApp = GetObject(, "Excel.Application")
    End If
    If GetExcelApp Is Nothing Then
        Set GetExcelApp = CreateObject("Excel.Application")
        If GetExcelApp Is Nothing Then
            MsgBox "Create Excel Application Failed!"
        End If
    End If
End Function

Public Sub OutPutToFile(ByVal filename As String, ByVal context As String)
    Open filename For Output As #1
    Print #1, context
    Close #1
End Sub
Public Sub AppendToFile(ByVal filename As String, ByVal context As String)
    Open filename For Append As #1
    Print #1, context
    Close #1
End Sub

Public Function FormExist(ByVal sName As String, _
        Optional ByRef vFrm As Variant, _
        Optional ByVal SetFocus As Boolean = True) As Boolean
    FormExist = False
    Dim frm As Form
    For Each frm In Forms
        If frm.Name = sName Then
            FormExist = True
            Exit For
        End If
    Next
    If FormExist Then
        If SetFocus Then frm.SetFocus
        If Not IsMissing(vFrm) Then
            Set vFrm = frm
        End If
    End If
End Function

Public Function GetFormCaption(ByVal sName As String) As String
    Dim nNum As Integer, strCap As String, strNum As String, strOrgCap As String, nPos As Integer, nCount As Integer
    Dim frm As Form
    strNum = ""
    nNum = 1
    nCount = 0
    For Each frm In Forms
        If frm.Name = sName Then
            nCount = nCount + 1
            strNum = ""
            strCap = frm.Caption
            nPos = InStrRev(strCap, "-")
            If nPos > 0 Then
                strNum = Trim(Mid(strCap, nPos + 2, Len(strCap) - nPos))
            End If
            If strNum <> "" And IsNumeric(strNum) Then
                If nNum <= CInt(strNum) Then nNum = CInt(strNum) + 1
            Else
                strOrgCap = strCap
            End If
        End If
    Next
    If nCount > nNum Then nNum = nCount
    If nNum > 1 Then
        GetFormCaption = strOrgCap & " - " & nNum
    Else
        GetFormCaption = strOrgCap
    End If
End Function
