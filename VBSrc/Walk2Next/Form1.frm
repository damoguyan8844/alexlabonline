VERSION 5.00
Object = "*\A工程2.vbp"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4485
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   4485
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.CheckBox Check2 
      Caption         =   "Check2"
      Height          =   825
      Left            =   3180
      TabIndex        =   9
      Top             =   3240
      Width           =   1215
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Option2"
      Height          =   465
      Left            =   1950
      TabIndex        =   8
      Top             =   3390
      Width           =   1035
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Command4"
      Height          =   705
      Left            =   570
      TabIndex        =   7
      Top             =   3450
      Width           =   1035
   End
   Begin VB.PictureBox Picture1 
      Height          =   2595
      Left            =   510
      ScaleHeight     =   2535
      ScaleWidth      =   3465
      TabIndex        =   0
      Top             =   480
      Width           =   3525
      Begin VB.TextBox Text1 
         Height          =   585
         Left            =   2790
         TabIndex        =   11
         TabStop         =   0   'False
         Text            =   "Text1"
         Top             =   1530
         Width           =   585
      End
      Begin VB.CommandButton Command5 
         Caption         =   "Command5"
         Height          =   315
         Left            =   2610
         TabIndex        =   10
         TabStop         =   0   'False
         Top             =   2100
         Width           =   585
      End
      Begin 工程2.UserControl1 UserControl11 
         Height          =   555
         Left            =   1830
         TabIndex        =   6
         Top             =   1650
         Width           =   615
         _extentx        =   1085
         _extenty        =   979
         font            =   "Form1.frx":0000
      End
      Begin VB.CommandButton Command3 
         Caption         =   "Command3"
         Height          =   375
         Left            =   1200
         TabIndex        =   5
         Top             =   1920
         Width           =   855
      End
      Begin VB.CheckBox Check1 
         Caption         =   "Check1"
         Height          =   615
         Left            =   1920
         TabIndex        =   4
         Top             =   900
         Width           =   1215
      End
      Begin VB.OptionButton Option1 
         Caption         =   "Option1"
         Height          =   405
         Left            =   570
         TabIndex        =   3
         Top             =   960
         Width           =   945
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Command2"
         Height          =   285
         Left            =   1800
         TabIndex        =   2
         Top             =   240
         Width           =   1005
      End
      Begin VB.CommandButton Command1 
         Caption         =   "Command1"
         Height          =   375
         Left            =   510
         TabIndex        =   1
         Top             =   90
         Width           =   675
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Text1_KeyDown(KeyCode As Integer, Shift As Integer)
    '根据按键决定下一步怎么走
    Walk2Next Text1.hWnd, KeyCode
End Sub
