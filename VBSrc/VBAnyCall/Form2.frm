VERSION 5.00
Begin VB.Form Form2 
   Caption         =   "VBErrCahter测试"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form2"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'------------------------------ 窗体模块 Form2.frm ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "Form2"
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)

Private WithEvents cmd As CommandButton
Attribute cmd.VB_VarHelpID = -1

Private Sub cmd_Click()
    '一定要加上on error resume next这句,否则程序便会中止
    On Error Resume Next
    Dim s As String
    MsgBox "就要发生错误了"
    MsgBox 1 / 0
    s = CDate("x")
    MsgBox "这是下一句"
End Sub

Private Sub Form_Initialize()
    '使用SEH，防止程序崩溃
    Call InitExceptionHandler
End Sub

Private Sub Form_Load()
    
    '添加按钮
    Set cmd = Me.Controls.Add("VB.CommandButton", "Command1")
    With cmd
        .Default = True
        .Caption = "运行例子"
        .Move 1520, 1170
        .Visible = True
    End With
    
    MsgBox "VB的错误处理代码地址为:&H" & Hex(g_oErrCather.EHAddress)
    
    '附加上我们自己的错误处理程序
    If g_oErrCather.InstallEH(AddressOf MyEH) Then
        MsgBox "错误处理程序安装成功!"
    End If
    
    MsgBox "当前程序是否为P代码:" & IIf(g_oErrCather.IsPCode, "是", "不是")
    
    Select Case g_oErrCather.WhereAmI
    Case 0 '开发环境
        MsgBox "我还在开发中……"
    Case 1 'Native编译
        MsgBox "我的编译模式为本地代码模式……"
    Case 2 'PCode编译
        MsgBox "我的编译模式为P代码模式……"
    Case Else
        MsgBox "有问题了……"
    End Select
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If g_oErrCather.IsInstalled Then
        '卸载错误处理程序
        g_oErrCather.UninstallEH
    End If
End Sub
