VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6135
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   6135
   ScaleWidth      =   4680
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.TextBox Text3 
      Height          =   1455
      Left            =   240
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   4
      Top             =   4560
      Width           =   4215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "½âÂë"
      Height          =   495
      Left            =   1560
      TabIndex        =   3
      Top             =   3930
      Width           =   1515
   End
   Begin VB.TextBox Text2 
      Height          =   1455
      Left            =   210
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   2
      Top             =   2340
      Width           =   4215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "±àÂë"
      Height          =   495
      Left            =   1530
      TabIndex        =   1
      Top             =   1680
      Width           =   1515
   End
   Begin VB.TextBox Text1 
      Height          =   1455
      Left            =   210
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   90
      Width           =   4215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private m_oAspCoding As New CVBAspCoding
Private Sub Command1_Click()
    Text2.Text = m_oAspCoding.Encoding(Text1.Text)
End Sub

Private Sub Command2_Click()
    Text3.Text = m_oAspCoding.Decoding(Text2.Text)
End Sub

Private Sub Form_Load()
    Text1.Text = "sub test()" & vbCrLf & vbTab & "alert 'hello world!'" & vbCrLf & "end sub"
End Sub
