VERSION 5.00
Begin VB.Form Form2 
   Caption         =   "frmTargetWindow"
   ClientHeight    =   3195
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   4680
   LinkTopic       =   "Form2"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.Menu mnu 
      Caption         =   "窗体2菜单"
      Begin VB.Menu mnuClick 
         Caption         =   "Click"
      End
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Initialize()
    '在窗体创建前安装挂钩
    If g_oWCHooker.InstallHooker(AddressOf WndHookProc_BeforeCreation, AddressOf WndHookProc_AfterCreation) Then
    End If
End Sub

Private Sub Form_Load()
    '创建完毕，卸载挂钩
    With g_oWCHooker
        If .IsInstalled Then
            .UnInstallHooker
        End If
    End With
    
    '更改完窗体类型后,重新赋值窗体标题
    Me.Caption = "子窗体2"
    Me.WindowState = vbMaximized
End Sub



Private Sub mnuClick_Click()
    MsgBox "Form2 菜单"
End Sub
