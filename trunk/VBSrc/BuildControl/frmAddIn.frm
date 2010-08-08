VERSION 5.00
Begin VB.Form frmFunctions 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "公共函数列表"
   ClientHeight    =   4095
   ClientLeft      =   2175
   ClientTop       =   1935
   ClientWidth     =   4020
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4095
   ScaleWidth      =   4020
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton cmdOK 
      Caption         =   "确定(&O)"
      Height          =   435
      Left            =   1320
      TabIndex        =   2
      Top             =   3540
      Width           =   1275
   End
   Begin VB.ListBox lstFunctions 
      Height          =   3000
      Left            =   60
      Style           =   1  'Checkbox
      TabIndex        =   0
      Top             =   390
      Width           =   3885
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "选择要导出的函数名："
      Height          =   180
      Left            =   60
      TabIndex        =   1
      Top             =   120
      Width           =   1800
   End
End
Attribute VB_Name = "frmFunctions"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdOK_Click()
    Me.Hide
End Sub

Private Sub Form_Load()
    Call GetFunctionList
End Sub


