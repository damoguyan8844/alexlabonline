Attribute VB_Name = "Module1"
Option Explicit
Public Declare Function LineTo Lib "gdi32" (ByVal hDC As Long, ByVal X As Long, ByVal Y As Long) As Long

Public Declare Function WideCharToMultiByte Lib "kernel32" (ByVal CodePage As Long, ByVal dwFlags As Long, ByVal lpWideCharStr As Long, ByVal cchWideChar As Long, ByRef lpMultiByteStr As Any, ByVal cchMultiByte As Long, ByVal lpDefaultChar As String, ByVal lpUsedDefaultChar As Long) As Long
Public Declare Function MultiByteToWideChar Lib "kernel32" (ByVal CodePage As Long, ByVal dwFlags As Long, ByRef lpMultiByteStr As Any, ByVal cchMultiByte As Long, ByVal lpWideCharStr As Long, ByVal cchWideChar As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpvDest As Any, lpvSource As Any, ByVal cbCopy As Long)

Public Const CP_UTF8 = 65001

'Purpose:Convert Utf8 to Unicode
Public Function UTF8_Decode(ByVal sUTF8 As String) As String

   Dim lngUtf8Size      As Long
   Dim strBuffer        As String
   Dim lngBufferSize    As Long
   Dim lngResult        As Long
   Dim bytUtf8()        As Byte
   Dim n                As Long

   If LenB(sUTF8) = 0 Then Exit Function

   If m_bIsNt Then
      On Error GoTo EndFunction
      bytUtf8 = StrConv(sUTF8, vbFromUnicode)
      lngUtf8Size = UBound(bytUtf8) + 1
      On Error GoTo 0
      'Set buffer for longest possible string i.e. each byte is
      'ANSI<=&HFF, thus 1 unicode(2 bytes)for every utf-8 character.
      lngBufferSize = lngUtf8Size * 2
      strBuffer = String$(lngBufferSize, vbNullChar)
      'Translate using code page 65001(UTF-8)
      lngResult = MultiByteToWideChar(CP_UTF8, 0, bytUtf8(0), _
         lngUtf8Size, StrPtr(strBuffer), lngBufferSize)
      'Trim result to actual length
      If lngResult Then
         UTF8_Decode = Left$(strBuffer, lngResult)
      End If
   Else
      Dim i                As Long
      Dim TopIndex         As Long
      Dim TwoBytes(1)      As Byte
      Dim ThreeBytes(2)    As Byte
      Dim AByte            As Byte
      Dim TStr             As String
      Dim BArray()         As Byte

      'Resume on error in case someone inputs text with accents
      'that should have been encoded as UTF-8
      On Error Resume Next

      TopIndex = Len(sUTF8)  ' Number of bytes equal TopIndex+1
      If TopIndex = 0 Then Exit Function ' get out if there's nothing to convert
      BArray = StrConv(sUTF8, vbFromUnicode)
      i = 0 ' Initialise pointer
      TopIndex = TopIndex - 1
      ' Iterate through the Byte Array
      Do While i <= TopIndex
         AByte = BArray(i)    ' fetch a byte
         If AByte < &H80 Then
            ' Normal ANSI character - use it as is
            TStr = TStr & Chr$(AByte): i = i + 1 ' Increment byte array index
         ElseIf AByte >= &HE0 Then         'was = &HE1 Then
            ' Start of 3 byte UTF-8 group for a character
            ' Copy 3 byte to ThreeBytes
            ThreeBytes(0) = BArray(i): i = i + 1
            ThreeBytes(1) = BArray(i): i = i + 1
            ThreeBytes(2) = BArray(i): i = i + 1
            ' Convert Byte array to UTF-16 then Unicode
            TStr = TStr & ChrW$((ThreeBytes(0) And &HF) * &H1000 + (ThreeBytes(1) And &H3F) * &H40 + (ThreeBytes(2) And &H3F))
         ElseIf (AByte >= &HC2) And (AByte <= &HDB) Then 'was (AByte >= &HC3) And (AByte <= &HC6)
            ' Start of 2 byte UTF-8 group for a character
            TwoBytes(0) = BArray(i): i = i + 1
            TwoBytes(1) = BArray(i): i = i + 1
            ' Convert Byte array to UTF-16 then Unicode
            TStr = TStr & ChrW$((TwoBytes(0) And &H1F) * &H40 + (TwoBytes(1) And &H3F))
         Else
            ' Normal ANSI character - use it as is
            TStr = TStr & Chr$(AByte): i = i + 1 ' Increment byte array index
         End If
      Loop
      UTF8_Decode = TStr    ' Return the resultant string
      Erase BArray
   End If

EndFunction:

End Function

'Purpose:Convert Unicode string to UTF-8.
Public Function UTF8_Encode(ByVal strUnicode As String, Optional ByVal bHTML As Boolean = True) As String
   Dim i                As Long
   Dim TLen             As Long
   Dim lPtr             As Long
   Dim UTF16            As Long
   Dim UTF8_EncodeLong  As String

   TLen = Len(strUnicode)
   If TLen = 0 Then Exit Function

   If m_bIsNt Then
      Dim lngBufferSize    As Long
      Dim lngResult        As Long
      Dim bytUtf8()        As Byte
      'Set buffer for longest possible string.
      lngBufferSize = TLen * 3 + 1
      ReDim bytUtf8(lngBufferSize - 1)
      'Translate using code page 65001(UTF-8).
      lngResult = WideCharToMultiByte(CP_UTF8, 0, StrPtr(strUnicode), _
         TLen, bytUtf8(0), lngBufferSize, vbNullString, 0)
      'Trim result to actual length.
      If lngResult Then
         lngResult = lngResult - 1
         ReDim Preserve bytUtf8(lngResult)
         'CopyMemory StrPtr(<B style="color:black;background-color:#ffff66">UTF8_Encode</B>), bytUtf8(0&), lngResult
         UTF8_Encode = StrConv(bytUtf8, vbUnicode)
         ' For i = 0 To lngResult
         '    <B style="color:black;background-color:#ffff66">UTF8_Encode</B> = <B style="color:black;background-color:#ffff66">UTF8_Encode</B> & Chr$(bytUtf8(i))
         ' Next
      End If
   Else
      For i = 1 To TLen
         ' Get UTF-16 value of Unicode character
         lPtr = StrPtr(strUnicode) + ((i - 1) * 2)
         CopyMemory UTF16, ByVal lPtr, 2
         'Convert to UTF-8
         If UTF16 < &H80 Then                                      ' 1 UTF-8 byte
            UTF8_EncodeLong = Chr$(UTF16)
         ElseIf UTF16 < &H800 Then                                 ' 2 UTF-8 bytes
            UTF8_EncodeLong = Chr$(&H80 + (UTF16 And &H3F))              ' Least Significant 6 bits
            UTF16 = UTF16 \ &H40                                   ' Shift right 6 bits
            UTF8_EncodeLong = Chr$(&HC0 + (UTF16 And &H1F)) & UTF8_EncodeLong  ' Use 5 remaining bits
         Else                                                      ' 3 UTF-8 bytes
            UTF8_EncodeLong = Chr$(&H80 + (UTF16 And &H3F))              ' Least Significant 6 bits
            UTF16 = UTF16 \ &H40                                   ' Shift right 6 bits
            UTF8_EncodeLong = Chr$(&H80 + (UTF16 And &H3F)) & UTF8_EncodeLong  ' Use next 6 bits
            UTF16 = UTF16 \ &H40                                   ' Shift right 6 bits
            UTF8_EncodeLong = Chr$(&HE0 + (UTF16 And &HF)) & UTF8_EncodeLong   ' Use 4 remaining bits
         End If
         UTF8_Encode = UTF8_Encode & UTF8_EncodeLong
      Next
   End If

   'Substitute vbCrLf with HTML line breaks if requested.
   If bHTML Then
      UTF8_Encode = Replace$(UTF8_Encode, vbCrLf, "<br>")
   End If

End Function


Public Function GetUTF8Code(ByVal sUnicode As String) As String
    Dim i As Long
    Dim lLength As Long
    Dim lBufferSize As Long
    Dim lResult As Long
    Dim bUTF8() As Byte
    
    GetUTF8Code = ""
    lLength = Len(sUnicode)
    If lLength = 0 Then Exit Function

    
    lBufferSize = lLength * 3 + 1
    ReDim bUTF8(lBufferSize - 1)
    
    lResult = WideCharToMultiByte(CP_UTF8, 0, StrPtr(sUnicode), lLength, bUTF8(0), lBufferSize, vbNullString, 0)
    
    If lResult <> 0 Then
        lResult = lResult - 1
        ReDim Preserve bUTF8(lResult)
        For i = LBound(bUTF8) To UBound(bUTF8)
            GetUTF8Code = GetUTF8Code & Hex(bUTF8(i))
        Next
    End If
End Function

Function Split(sIn As String, sSep As String) As String()
On Error Resume Next
Dim l As Long, s As String, sTmp As String, i As Long
Dim arr() As String
s = Trim$(sIn)
Do
  l = InStr(s, sSep)
  ReDim Preserve arr(i) As String
  If l <> 0 Then
    arr(i) = Left$(s, l - 1)
    s = Right$(s, Len(s) - (l + Len(sSep) - 1))
    i = i + 1
  Else
    arr(i) = s
    Exit Do
  End If
Loop
Split = arr
End Function
