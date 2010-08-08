VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5685
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   5685
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.ListBox List1 
      Height          =   5460
      Left            =   360
      TabIndex        =   0
      Top             =   120
      Width           =   3795
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function IsWindow Lib "user32" (ByVal hwnd As Long) As Long
Private Declare Function CallTestC Lib "D:\我的程序\CallTest\Debug\CallTest.dll" (i As Long) As Long

Dim m_oCallConv As New CVBCallConvention

Private Sub Form_Load()
    m_oCallConv.CallConvention = 0
    MsgBox CallTestC(1)
End Sub
