VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "关于 Jonathan's Build Control Add-In"
   ClientHeight    =   1755
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4605
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1755
   ScaleWidth      =   4605
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '屏幕中心
   Begin VB.Frame Frame1 
      Height          =   75
      Left            =   90
      TabIndex        =   3
      Top             =   870
      Width           =   4395
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "CSDN ID：超级绿豆(supergreenbean)"
      Height          =   180
      Index           =   2
      Left            =   900
      TabIndex        =   2
      Top             =   570
      Width           =   2970
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "自己动手，丰衣足食"
      BeginProperty Font 
         Name            =   "楷体_GB2312"
         Size            =   21.75
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   435
      Index           =   1
      Left            =   300
      TabIndex        =   1
      Top             =   1140
      Width           =   4050
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "作者：Jonathan"
      Height          =   180
      Index           =   0
      Left            =   900
      TabIndex        =   0
      Top             =   270
      Width           =   1260
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Click()
    Me.Hide
End Sub

