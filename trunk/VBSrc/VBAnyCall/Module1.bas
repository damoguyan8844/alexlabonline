Attribute VB_Name = "Module1"
'------------------------------ ģ�� Module1.bas ------------------------------
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "Module1"

Public g_oErrCather As New CVBErrCatcher

Public Function MyEH() As Long
    MsgBox "�д�����!��������Ϊ:" & Err.Description, , "�Զ����������"
End Function

Public Function testsub() As Long
Dg
End Function
