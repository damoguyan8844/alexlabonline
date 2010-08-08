VERSION 5.00
Begin VB.MDIForm MDIForm1 
   BackColor       =   &H8000000C&
   Caption         =   "MDIForm1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "MDIForm1"
   StartUpPosition =   3  '窗口缺省
End
Attribute VB_Name = "MDIForm1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function SetParent Lib "user32" (ByVal hWndChild As Long, ByVal hWndNewParent As Long) As Long

Private Sub MDIForm_Load()
    Me.WindowState = vbMaximized
    
    Form1.Show
    Form2.Show
    
    Me.Arrange vbCascade
End Sub

Private Sub MDIForm_Unload(Cancel As Integer)
    Unload Form1
    MsgBox IIf(UnRegClass, "成功注销类 " & MY_CLASS_NAME, "注销类 " & MY_CLASS_NAME & " 失败")
End Sub
