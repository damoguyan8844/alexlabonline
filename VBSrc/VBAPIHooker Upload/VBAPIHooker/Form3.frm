VERSION 5.00
Begin VB.Form Form2 
   Caption         =   "Form2"
   ClientHeight    =   7485
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9480
   LinkTopic       =   "Form2"
   ScaleHeight     =   7485
   ScaleWidth      =   9480
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox Text2 
      Height          =   5835
      Left            =   4800
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   3
      Top             =   150
      Width           =   4665
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   405
      Left            =   1590
      TabIndex        =   2
      Top             =   6750
      Width           =   1245
   End
   Begin VB.TextBox Text1 
      Height          =   5835
      Left            =   90
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   1
      Top             =   150
      Width           =   4665
   End
   Begin VB.CommandButton cmdHook 
      Caption         =   "Hook"
      Height          =   465
      Left            =   1590
      TabIndex        =   0
      Top             =   6120
      Width           =   1275
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Sub encode Lib "J:\Gersang\Encode\Debug\Encode.dll" (ByVal tbladdr1 As Long, ByVal tbladdr2 As Long, ByVal tbladdr3 As Long, ByVal tbladdr4 As Long, ByVal msgbody As Long, ByVal ecxval As Long)

Private Const aGersang87654321 As String = "gersang87654321"
Private m_abTable(4095) As Byte

Private m_lpDataBuffer As Long

Private Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hWnd As Long, lpdwProcessId As Long) As Long

Private Const GERSANG_WINDOW_CAPTION As String = "Gersang"

Private WithEvents m_oApiHooker  As CVBAPIHooker
Attribute m_oApiHooker.VB_VarHelpID = -1

Private Sub cmdHook_Click()
    If cmdHook.Caption = "Hook" Then
        If m_oApiHooker Is Nothing Then
            Set m_oApiHooker = New CVBAPIHooker
        End If
        Dim pid As Long
        GetWindowThreadProcessId FindWindow(vbNullString, GERSANG_WINDOW_CAPTION), pid
        m_oApiHooker.SetTargetTo pid
        If m_oApiHooker.HookImport("GersangX.exe", "ws2_32.dll", "send") <> -1 And m_oApiHooker.HookImport("GersangX.exe", "ws2_32.dll", "recv") <> -1 Then
            cmdHook.Caption = "Unhook"
        End If
    Else
        m_oApiHooker.SetTargetTo
        cmdHook.Caption = "Hook"
    End If
End Sub

Private Sub Form_Load()
'    Dim ab(20) As Byte
'    ab(0) = &H16
'    ab(1) = &H0
'    ab(2) = &H93
'    ab(3) = &H8A
'    ab(4) = &H66
'    ab(5) = &H72
'    ab(6) = &H73
'    ab(7) = &H1F
'    ab(8) = &H6A
'    ab(9) = &O63
'    ab(10) = &H3B
'    ab(11) = &H3B
'    ab(12) = &H3B
'    ab(13) = &H3B
'    ab(14) = &H3B
'    ab(15) = &H3B
'    ab(16) = &H3B
'    ab(17) = &H3B
'    ab(18) = &H3B
'    ab(19) = &H3B
'    ab(20) = &H32
'
'    Debug.Print Array2Str(ab(), 1, 16)
'
'    DecodeMsg ab()
'
'    Debug.Print Array2Str(ab(), 1, 16)
'    Exit Sub
GetTable
End Sub

Private Sub m_oApiHooker_AfterAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal fBlocked As Boolean, lRetValue As Long)
    Static lCounter2 As Long
    Dim alArgs() As Long
    Dim abMsg() As Byte
    Dim lMsgLen As Long
    
    If sAPIName = "recv" Then
        If m_lpDataBuffer <> 0 And lRetValue > 0 Then
            lCounter2 = lCounter2 + 1
            
            With m_oApiHooker
                lMsgLen = lRetValue
                ReDim abMsg(lMsgLen - 1)
                .Pmemcpy2local VarPtr(abMsg(0)), m_lpDataBuffer, lMsgLen
                AddTxt2 CStr(lCounter2) & "." & Now() & vbCrLf
                AddTxt2 Array2Str(abMsg(), 1, 16, True) & vbCrLf & vbCrLf
            End With
        End If
    End If
    
    If sAPIName = "send" Then
        AddTxt CStr(lRetValue)
    End If
End Sub

Private Sub m_oApiHooker_BeforeAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal lpArgs As Long, fIntercept As Boolean, lArgCount As Long, ByVal lReturnAddress As Long)
    Static lCounter As Long
    Dim alArgs() As Long
    Dim abMsg() As Byte
    Dim lMsgLen As Long
    
    If sAPIName = "send" Then
        lCounter = lCounter + 1
        
        With m_oApiHooker
            If .GetArguments(alArgs()) Then
                lMsgLen = alArgs(2)
                ReDim abMsg(lMsgLen - 1)
                .Pmemcpy2local VarPtr(abMsg(0)), alArgs(1), lMsgLen
                'AddTxt Array2Str(abMsg(), 1, 16, True) & vbCrLf
                'AddTxt "解码为：" & vbCrLf
                AddTxt CStr(lCounter) & "." & Now() & vbCrLf
                Call DecodeMsg(abMsg())
                AddTxt Array2Str(abMsg(), 1, 16, True) & vbCrLf & vbCrLf
            End If
        End With
        m_lpDataBuffer = 0
    End If
    
    If sAPIName = "recv" Then
        If m_oApiHooker.GetArguments(alArgs()) Then
            m_lpDataBuffer = alArgs(1)
        End If
    End If
    fIntercept = False
End Sub

Private Sub AddTxt(ByVal sTxt As String)
    With Text1
        .SelStart = Len(.Text)
        .Text = .Text & sTxt
        .SelStart = Len(.Text)
    End With
End Sub

Private Sub AddTxt2(ByVal sTxt As String)
    With Text2
        .SelStart = Len(.Text)
        .Text = .Text & sTxt
        .SelStart = Len(.Text)
    End With
End Sub
Private Function Array2Str(v As Variant, ByVal lDataLen As Long, Optional lNewLinePos As Long = 4, Optional fDisplayChar As Boolean = False) As String
    If IsArray(v) Then
        Dim i As Long
        Dim s As String
        Array2Str = ""
        If fDisplayChar And (lDataLen = 1) Then
            Dim abTmp() As Byte
            ReDim abTmp(UBound(v))
            For i = LBound(v) To UBound(v)
                abTmp(i) = IIf(v(i) >= 32 And v(i) <= 128, v(i), AscB("."))
            Next
        End If
        For i = LBound(v) To UBound(v)
            If (i Mod lNewLinePos = 0) And i <> 0 Then
                If fDisplayChar And (lDataLen = 1) Then
                    Array2Str = Array2Str & "; " & StrConv(MidB$(abTmp, (i \ lNewLinePos - 1) * lNewLinePos + 1, lNewLinePos), vbUnicode)
                End If
                Array2Str = Array2Str & " " & vbCrLf
            Else
                Array2Str = Array2Str & " "
            End If
            Array2Str = Array2Str & Right$(String(lDataLen * 2, "0") & Hex(v(i)), lDataLen * 2)
        Next
        Array2Str = Trim(Array2Str)
        Array2Str = Array2Str & String$((lNewLinePos - ((UBound(v) + 1) Mod lNewLinePos)) * 3, " ") & "; " & StrConv(MidB$(abTmp, ((UBound(v) + 1) \ lNewLinePos) * lNewLinePos + 1, ((UBound(v) + 1) Mod lNewLinePos)), vbUnicode)
    End If
End Function

Private Function DecodeMsg(abMsg() As Byte) As Boolean
    On Error GoTo Error_Handler
    Dim abKey() As Byte
    abKey = StrConv(aGersang87654321, vbFromUnicode)
    
    Dim i As Long
    Dim j As Long
    Dim lKeyLen As Long
    Dim lMsgLen As Long
    
    If abMsg(0) >= &H13 Then
                
       
    End If
    
    lMsgLen = abMsg(0) - 3
    lKeyLen = UBound(abKey) + 1
    For i = 0 To lMsgLen - 1
        abMsg(i + 3) = abMsg(i + 3) Xor abKey(i Mod lKeyLen)
    Next
    DecodeMsg = True
Error_Handler:
End Function

Private Function GetXorCode(abMsgBody() As Byte) As Byte
    Dim i As Long
    GetXorCode = 0
    For i = 0 To UBound(abMsgBody)
        GetXorCode = GetXorCode Xor abMsgBody(i)
    Next
End Function

Private Function GetTable() As Boolean
    Dim s As String
    s = "D:\Program Files\GAMANIA\巨商\GersangX.exe"
    Open s For Binary As #1
        Seek #1, &H1DAFB0 + 1
        Get #1, , m_abTable
    Close #1
    GetTable = True
End Function
