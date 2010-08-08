VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   2070
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6315
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2070
   ScaleWidth      =   6315
   StartUpPosition =   3  '窗口缺省
   Begin VB.Frame Frame2 
      Caption         =   "收到的消息"
      Height          =   1575
      Left            =   3030
      TabIndex        =   4
      Top             =   240
      Width           =   3165
      Begin VB.ListBox List1 
         Height          =   1140
         Left            =   150
         TabIndex        =   5
         Top             =   240
         Width           =   2835
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "发送测试"
      Height          =   1575
      Left            =   150
      TabIndex        =   0
      Top             =   240
      Width           =   2835
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   1260
         TabIndex        =   2
         Text            =   "1234"
         Top             =   270
         Width           =   1335
      End
      Begin VB.CommandButton Command1 
         Caption         =   "发送消息"
         Height          =   465
         Left            =   630
         TabIndex        =   1
         Top             =   780
         Width           =   1575
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "消息ID = &&H"
         Height          =   180
         Left            =   240
         TabIndex        =   3
         Top             =   300
         Width           =   990
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

Private WithEvents m_oMsgWindow As CVBMiniWindow
Attribute m_oMsgWindow.VB_VarHelpID = -1

Private Sub Command1_Click()
    On Error Resume Next
    PostMessage m_oMsgWindow.hMsgWindow, CLng("&H" & Trim(Text1.Text)), 0, 0
End Sub

Private Sub Form_Load()
    Set m_oMsgWindow = New CVBMiniWindow
    m_oMsgWindow.Create
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_oMsgWindow = Nothing
End Sub

Private Sub m_oMsgWindow_IncomingMsg(ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long)
    AddMsg "收到消息ID为 " & Hex(uMsg) & "的消息"
End Sub

Private Sub AddMsg(ByVal sMsg As String)
    With List1
        .AddItem sMsg
        .Selected(.NewIndex) = True
    End With
End Sub
