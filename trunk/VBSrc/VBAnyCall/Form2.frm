VERSION 5.00
Begin VB.Form Form2 
   Caption         =   "VBErrCahter����"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form2"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '����ȱʡ
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'------------------------------ ����ģ�� Form2.frm ------------------------------
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "Form2"
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)

Private WithEvents cmd As CommandButton
Attribute cmd.VB_VarHelpID = -1

Private Sub cmd_Click()
    'һ��Ҫ����on error resume next���,�����������ֹ
    On Error Resume Next
    Dim s As String
    MsgBox "��Ҫ����������"
    MsgBox 1 / 0
    s = CDate("x")
    MsgBox "������һ��"
End Sub

Private Sub Form_Initialize()
    'ʹ��SEH����ֹ�������
    Call InitExceptionHandler
End Sub

Private Sub Form_Load()
    
    '��Ӱ�ť
    Set cmd = Me.Controls.Add("VB.CommandButton", "Command1")
    With cmd
        .Default = True
        .Caption = "��������"
        .Move 1520, 1170
        .Visible = True
    End With
    
    MsgBox "VB�Ĵ���������ַΪ:&H" & Hex(g_oErrCather.EHAddress)
    
    '�����������Լ��Ĵ��������
    If g_oErrCather.InstallEH(AddressOf MyEH) Then
        MsgBox "���������װ�ɹ�!"
    End If
    
    MsgBox "��ǰ�����Ƿ�ΪP����:" & IIf(g_oErrCather.IsPCode, "��", "����")
    
    Select Case g_oErrCather.WhereAmI
    Case 0 '��������
        MsgBox "�һ��ڿ����С���"
    Case 1 'Native����
        MsgBox "�ҵı���ģʽΪ���ش���ģʽ����"
    Case 2 'PCode����
        MsgBox "�ҵı���ģʽΪP����ģʽ����"
    Case Else
        MsgBox "�������ˡ���"
    End Select
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If g_oErrCather.IsInstalled Then
        'ж�ش��������
        g_oErrCather.UninstallEH
    End If
End Sub
