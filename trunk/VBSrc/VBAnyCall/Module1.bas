Attribute VB_Name = "Module1"
'------------------------------ 模块 Module1.bas ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "Module1"

Public g_oErrCather As New CVBErrCatcher

Public Function MyEH() As Long
    MsgBox "有错误发生!错误描述为:" & Err.Description, , "自定义错误处理函数"
End Function

Public Function testsub() As Long
Dg
End Function
