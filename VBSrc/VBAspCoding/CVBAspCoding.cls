VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBAspCoding"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

Option Explicit
Private pbsOriTblEncoding As String
Private pbsOriTblPos As String
Private pbsTblPos As String
Private pbsTblEncoding As String
Private pbsSpecialChars As String

Private Const pbcntlHead As Long = 12
Private Const pbcntlEnd As Long = 12
Private Const pbcntlEncStart As Long = 0
Private Const pbcntlEncEnd As Long = 127

Const BASE_64_MAP_INIT = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

Private nl
' zero based arrays
Private Base64EncMap(63)
Private Base64DecMap(127)

Private Function Code2Char(l As Long) As String
    'Esc Org 27
    '@# \r 13
    '@& \n 10
    '@! <  60
    '@* >  62
    '@$ @  64
    Dim s As String
    Select Case l
    Case 13
        s = "@#"
    Case 10
        s = "@&"
    Case 27
        s = "Org"
    Case 60
        s = "@!"
    Case 62
        s = "@*"
    Case 64
        s = "@$"
    Case Else
        s = Chr(l)
    End Select
    Code2Char = s
End Function
Private Function Char2Code(s As String) As Long
    'Esc Org 27
    '@# \r 13
    '@& \n 10
    '@! <  60
    '@* >  62
    '@$ @  64
    Dim l As Long
    Select Case UCase(s)
    Case "@#"
        l = 13
    Case "@&"
        l = 10
    Case "ORG"
        l = 27
    Case "Org"
        l = 60
    Case "@*"
        l = 62
    Case "@$"
        l = 64
    Case Else
        l = Asc(s)
    End Select
    Char2Code = l
End Function

Private Function IsSpecialChar(s As String, Optional sO As String) As Boolean
    IsSpecialChar = False
    If Len(s) <> 0 Then
        sO = Code2Char(Asc(s))
        If sO <> s Then
            IsSpecialChar = True
            Exit Function
        End If
    End If
End Function

'****************************************************************************************************************************
'*Encoding....
'****************************************************************************************************************************
Private Function GenerateEncTblPicking() As String
    Dim s As String, s1 As String, sTmp As String
    Dim b(1 To 3) As Byte
    Dim i As Long
    s = DoEnc(String(64, "A"))
    s = Mid(s, pbcntlHead + 1, Len(s) - pbcntlHead - pbcntlEnd)
    b(1) = Asc(Mid(s, 1, 1))
    b(2) = Asc(Mid(s, 2, 1))
    b(3) = Asc(Mid(s, 3, 1))
    s1 = ""
    For i = 1 To Len(s)
        
        Select Case Asc(Mid(s, i, 1))
        Case b(1)
            s1 = s1 + "1"
        Case b(2)
            s1 = s1 + "2"
        Case b(3)
            s1 = s1 + "3"
        End Select
    Next
    GenerateEncTblPicking = s1
End Function
Private Function GenerateEncTbl(lStart As Long, lEnd As Long) As String
    Dim s As String, sTmp As String
    Dim i As Long
    For i = lStart To lEnd
        s = Chr(i)
        If i = lStart Then
            sTmp = String(3, s) + String(59, "a") + vbCrLf
        Else
            sTmp = sTmp + String(3, s) + String(59, "a") + vbCrLf
        End If
    Next
    
    s = DoEnc(sTmp)
    s = Replace(s, Mid(s, 4 + pbcntlHead, 63), "")
    s = Mid(s, pbcntlHead + 1, Len(s) - pbcntlHead - pbcntlEnd - 1)
    
    s = Replace(s, "@#", "?")
    s = Replace(s, "Org", "?", Compare:=vbTextCompare)
    s = Replace(s, "@&", "?")
    s = Replace(s, "@!", "?")
    s = Replace(s, "@*", "?")
    s = Replace(s, "@$", "?")
    GenerateEncTbl = s
End Function
Private Function DoEnc(str As String, Optional sType As String = ".js") As String
    Dim oDo As New Scripting.Encoder
    DoEnc = oDo.EncodeScriptFile(sType, str, 0&, "")
    Set oDo = Nothing
End Function

Public Function Encoding(str As String) As String
    Dim s As String, sRet As String, t As String
    Dim i As Long, lPos As Long, lP As Long
    Dim sTblPicking As String, sTblEnc As String
    sTblPicking = GenerateEncTblPicking
    sTblEnc = GenerateEncTbl(pbcntlEncStart, pbcntlEncEnd)
    sRet = ""
    For i = 1 To Len(str)
        t = Mid(str, i, 1)
        lPos = Val(Mid(sTblPicking, IIf(i Mod 64 <> 0, i Mod 64, IIf(i Mod 64 = 0, 64, i)), 1))
        If IsSpecialChar(t, s) Then
            sRet = sRet + s
        Else
            sRet = sRet + Mid(sTblEnc, (Asc(t) - pbcntlEncStart) * 3 + lPos, 1)
        End If
    Next
    Encoding = sRet
End Function

'****************************************************************************************************************************
'*Decoding....
'****************************************************************************************************************************
Public Function Decoding(str As String) As String
    'Esc Org 27
    '@# \r 13
    '@& \n 10
    '@! <  60
    '@* >  62
    '@$ @  64
    Dim sDecTbl(1 To 3) As String, sEncTbl As String, sDecTblFull As String, sPosTbl As String
    Dim s As String, sRet As String, t As String
    Dim i As Long, j As Long
    Dim lPos As Long, lExistPos As Long
    'Generating encoding table & pos table
    sEncTbl = GenerateEncTbl(pbcntlEncStart, pbcntlEncEnd)
    sPosTbl = GenerateEncTblPicking()
    
    'Generating decoding table
    For i = 1 To 3
        For j = 1 To Len(sEncTbl) Step 3
            sDecTbl(1) = sDecTbl(1) + Mid(sEncTbl, j, 1)
            sDecTbl(2) = sDecTbl(2) + Mid(sEncTbl, j + 1, 1)
            sDecTbl(3) = sDecTbl(3) + Mid(sEncTbl, j + 2, 1)
        Next
    Next
    
    sDecTblFull = ""
    For i = pbcntlEncStart To pbcntlEncEnd
        sDecTblFull = sDecTblFull + String(3, Chr(i))
    Next
    
    'Decoding goes here
    s = Replace(str, "@#", Chr(13))
    s = Replace(s, "@!", "<")
    s = Replace(s, "@*", ">")
    s = Replace(s, "@$", "@")
    s = Replace(s, "@&", Chr(10))
    s = Replace(s, "Org", Chr(27), Compare:=vbTextCompare)
    
    For i = 1 To Len(s)
        t = Mid(s, i, 1)
        lPos = Val(Mid(sPosTbl, IIf(i Mod 64 <> 0, i Mod 64, IIf(i Mod 64 = 0, 64, i)), 1))
        lExistPos = InStr(sDecTbl(lPos), t)
        If lExistPos <> 0 Then
            sRet = sRet + Mid(sDecTblFull, (lExistPos - 1) * 3 + lPos, 1)
        Else
            sRet = sRet + t
        End If
    Next
    Decoding = sRet
End Function

' must be called before using anything else
Private Sub initCodecs()
     ' init vars
     nl = "<P>" & Chr(13) & Chr(10)
     ' setup base 64
     Dim max, idx
        max = Len(BASE_64_MAP_INIT)
     For idx = 0 To max - 1
          ' one based string
          Base64EncMap(idx) = Mid(BASE_64_MAP_INIT, idx + 1, 1)
     Next
     For idx = 0 To max - 1
          Base64DecMap(Asc(Base64EncMap(idx))) = idx
     Next
End Sub

' encode base 64 encoded string
Private Function base64Encode(plain)
  initCodecs
If Len(plain) = 0 Then
     base64Encode = ""
     Exit Function
End If

Dim ret, ndx, by3, first, second, third
by3 = (Len(plain) \ 3) * 3
ndx = 1
Do While ndx <= by3
     first = Asc(Mid(plain, ndx + 0, 1))
     second = Asc(Mid(plain, ndx + 1, 1))
     third = Asc(Mid(plain, ndx + 2, 1))
     ret = ret & Base64EncMap((first \ 4) And 63)
     ret = ret & Base64EncMap(((first * 16) And 48) + ((second \ 16) And 15))
     ret = ret & Base64EncMap(((second * 4) And 60) + ((third \ 64) And 3))
     ret = ret & Base64EncMap(third And 63)
     ndx = ndx + 3
Loop
' check for stragglers
If by3 < Len(plain) Then
     first = Asc(Mid(plain, ndx + 0, 1))
     ret = ret & Base64EncMap((first \ 4) And 63)
     If (Len(plain) Mod 3) = 2 Then
          second = Asc(Mid(plain, ndx + 1, 1))
          ret = ret & Base64EncMap(((first * 16) And 48) + ((second \ 16) And 15))
          ret = ret & Base64EncMap(((second * 4) And 60))
     Else
          ret = ret & Base64EncMap((first * 16) And 48)
          ret = ret & "="
     End If
     ret = ret & "="
End If

base64Encode = ret
End Function


