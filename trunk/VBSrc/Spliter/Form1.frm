VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'------------------------  使用示例 --------------------------------
Option Explicit

Private oSpHorizontal As New CSimpleSplitter '垂直分割
Private oSpVertical As New CSimpleSplitter '水平分割

Private WithEvents p1 As PictureBox, p2 As PictureBox, p3 As PictureBox, p4 As PictureBox
Attribute p1.VB_VarHelpID = -1
Private Sub Form_Load()
    With Form1
        Set p1 = .Controls.Add("VB.PictureBox", "p1")
        Set p2 = .Controls.Add("VB.PictureBox", "p2")
        Set p3 = .Controls.Add("VB.PictureBox", "p3")
        Set p4 = .Controls.Add("VB.PictureBox", "p4")
    End With
    
    Set p3.Container = p1
    Set p4.Container = p1
    
    p1.Visible = True
    p2.Visible = True
    p3.Visible = True
    p4.Visible = True
    
    With oSpVertical
        'Picture1为左面板，Picture2为右面板，Form1为两个面板的容器
        .Attach p1, p2, Form1
        .IsVertical = True '以左右方式显示左右两个面板（默认）
        .SplitterBarSize = 20 '分隔条宽度
        .DrawSplitterBarSize = .SplitterBarSize '拖动时显示的分隔条宽度
        .SplitterBarColor = RGB(0, 128, 128)
    End With
    
    With oSpHorizontal
        'Picture3为左（上）面板，Picture4为右（下）面板，Picture1为两个面板的容器
        .Attach p3, p4, p1
        .IsVertical = False '以上下方式显示左右两个面板
        .SplitterBarSize = 5
        .DrawSplitterBarSize = .SplitterBarSize
        .SplitterBarColor = RGB(255, 255, 0)
    End With
    
End Sub

'------------------- 垂直分割调整 -------------------
'调整对象：oSpVertical
Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    oSpVertical.MouseUp Button, Shift, X, Y
End Sub
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    oSpVertical.MouseMove Button, Shift, X, Y
End Sub
Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    oSpVertical.MouseDown Button, Shift, X, Y
End Sub

Private Sub Form_Resize()
    oSpVertical.Resize
End Sub
'------------------- 垂直分割调整 -------------------


'------------------- 水平分割调整 -------------------
'调整对象：oSpHorizontal
Private Sub p1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    oSpHorizontal.MouseUp Button, Shift, X, Y
End Sub
Private Sub p1_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    oSpHorizontal.MouseMove Button, Shift, X, Y
End Sub
Private Sub p1_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    oSpHorizontal.MouseDown Button, Shift, X, Y
End Sub

Private Sub p1_Resize()
    oSpHorizontal.Resize
End Sub
'------------------- 水平分割调整 -------------------
