VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "VBAnyCall����"
   ClientHeight    =   4725
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4815
   LinkTopic       =   "Form1"
   ScaleHeight     =   4725
   ScaleWidth      =   4815
   StartUpPosition =   3  '����ȱʡ
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'------------------------------ ����ģ�� Form1.frm ------------------------------
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "Form1"

Private goAnyCall As New CVBAnyCall

Private WithEvents cmd As CommandButton
Attribute cmd.VB_VarHelpID = -1
Private WithEvents lst As ListBox
Attribute lst.VB_VarHelpID = -1

Private Sub Form_Initialize()
    'ʹ��SEH����ֹ�������
    Call InitExceptionHandler
End Sub

Private Sub Form_Load()
    '�ƶ�����
    Me.Height = 5130
    Me.Width = 4935
    
    '��Ӱ�ť
    Set cmd = Me.Controls.Add("VB.CommandButton", "Command1")
    With cmd
        .Default = True
        .Caption = "��������"
        .Move 1290, 3990, 2235, 405
        .Visible = True
    End With
    
    '����б�
    Set lst = Me.Controls.Add("VB.ListBox", "List1")
    With lst
        .Move 120, 120, 4575, 3480
        .Visible = True
    End With
End Sub

Private Sub cmd_Click()
    On Error GoTo Error_Handler
    Dim sByteCode As String
    Dim bBinCode() As Byte
    Dim i As Long
    Dim sDll As String
    i = 123
    sDll = "J:\User-C\CallTest\Debug\CallTest.dll"

    'ʹ��VTable��ʽ
    goAnyCall.ThroughVTable = True
    
    '���л�������ֽ��ַ���
    lst.AddItem "ֱ�����л�������ֽ��ַ���,����Լ��Ϊ__stdcall"
    sByteCode = "8B 44 24 04 C2 04 00"
    goAnyCall.IsStandardCall = True
    lst.AddItem "����ֵ:" & goAnyCall.CallCodeBytes(sByteCode, 321)
        
    lst.AddItem "ֱ�����л�������ֽ��ַ���,����Լ��Ϊ__cdecl"
    sByteCode = "8B 44 24 04 C3"
    goAnyCall.IsStandardCall = False
    lst.AddItem "����ֵ:" & goAnyCall.CallCodeBytes(sByteCode, 123)
    
    '����ַ���л�������ֽ�
    lst.AddItem "����ַ���л�������ֽ�,����Լ��Ϊ__stdcall"
    sByteCode = "8B 44 24 04 C2 04 00"
    bBinCode = goAnyCall.ByteCodeStrToBin(sByteCode)
    goAnyCall.IsStandardCall = True
    lst.AddItem "����ֵ:" & goAnyCall.CallByAddress(VarPtr(bBinCode(0)), 123)
    
    lst.AddItem "����ַ���л�������ֽ�,����Լ��Ϊ__cdecl"
    sByteCode = "8B 44 24 04 C3"
    bBinCode = goAnyCall.ByteCodeStrToBin(sByteCode)
    goAnyCall.IsStandardCall = False
    lst.AddItem "����ֵ:" & goAnyCall.CallByAddress(VarPtr(bBinCode(0)), 123)
    
    '����ַ����VB����
    lst.AddItem "����ַ����VB����,����Լ��Ϊ__stdcall"
    lst.AddItem "i=" & CStr(i)
    goAnyCall.IsStandardCall = True
    lst.AddItem "����ֵ:" & goAnyCall.CallByAddress(AddressOf CallTest, VarPtr(i))
        
    '����Dll����
    'C����ԭ��
    'int __stdcall CallTestS(int *a){
    '    int b;
    '    b=*a;
    '    *a+=111;
    '    return b;
    '}
    lst.AddItem "����Dll����,����Լ��Ϊ__stdcall"
    goAnyCall.IsStandardCall = True
    lst.AddItem "����ֵ:" & goAnyCall.CallApiByName(sDll, "CallTestS", VarPtr(i))
    lst.AddItem "i=" & CStr(i)
    
    'C����ԭ��
    'int __cdecl CallTestC(int *a){
    '    int b;
    '    b=*a;
    '    *a+=111;
    '    return b;
    '}
    lst.AddItem "����Dll����,����Լ��Ϊ__cdecl"
    goAnyCall.IsStandardCall = False
    lst.AddItem "����ֵ:" & goAnyCall.CallApiByName(sDll, "CallTestC", VarPtr(i))
    lst.AddItem "i=" & CStr(i)
    
    Exit Sub
Error_Handler:
    '�Զ��������

    '����Ĭ�ϴ�������
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Sub


