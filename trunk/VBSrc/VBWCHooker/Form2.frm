VERSION 5.00
Begin VB.Form Form2 
   Caption         =   "×Ó´°Ìå1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   630
   ClientWidth     =   4680
   LinkTopic       =   "Form2"
   MDIChild        =   -1  'True
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   1005
      Left            =   870
      TabIndex        =   0
      Top             =   870
      Width           =   2985
   End
   Begin VB.Menu t 
      Caption         =   "t"
      Begin VB.Menu x 
         Caption         =   "x"
      End
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    MsgBox "wa~~"
End Sub

Private Sub x_Click()
    Call Command1_Click
End Sub
