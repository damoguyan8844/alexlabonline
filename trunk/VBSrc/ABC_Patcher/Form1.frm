VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "智能ABC内存补丁测试"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox Text1 
      Height          =   2115
      Left            =   480
      MultiLine       =   -1  'True
      TabIndex        =   0
      Text            =   "Form1.frx":0000
      Top             =   390
      Width           =   3645
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Initialize()
    Call InitExceptionHandler
    If g_oPatcher.InstallPatcher Then
        MsgBox "装好内存补丁了"
    End If
End Sub



Private Sub Form_Unload(Cancel As Integer)
    If g_oPatcher.IsInstalled Then
        g_oPatcher.UnInstallPatcher
    End If
End Sub
