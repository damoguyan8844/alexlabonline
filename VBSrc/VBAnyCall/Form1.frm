VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "VBAnyCall测试"
   ClientHeight    =   4725
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4815
   LinkTopic       =   "Form1"
   ScaleHeight     =   4725
   ScaleWidth      =   4815
   StartUpPosition =   3  '窗口缺省
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'------------------------------ 窗体模块 Form1.frm ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "Form1"

Private goAnyCall As New CVBAnyCall

Private WithEvents cmd As CommandButton
Attribute cmd.VB_VarHelpID = -1
Private WithEvents lst As ListBox
Attribute lst.VB_VarHelpID = -1

Private Sub Form_Initialize()
    '使用SEH，防止程序崩溃
    Call InitExceptionHandler
End Sub

Private Sub Form_Load()
    '移动窗体
    Me.Height = 5130
    Me.Width = 4935
    
    '添加按钮
    Set cmd = Me.Controls.Add("VB.CommandButton", "Command1")
    With cmd
        .Default = True
        .Caption = "运行例子"
        .Move 1290, 3990, 2235, 405
        .Visible = True
    End With
    
    '添加列表
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

    '使用VTable方式
    goAnyCall.ThroughVTable = True
    
    '运行汇编语言字节字符串
    lst.AddItem "直接运行汇编语言字节字符串,调用约定为__stdcall"
    sByteCode = "8B 44 24 04 C2 04 00"
    goAnyCall.IsStandardCall = True
    lst.AddItem "返回值:" & goAnyCall.CallCodeBytes(sByteCode, 321)
        
    lst.AddItem "直接运行汇编语言字节字符串,调用约定为__cdecl"
    sByteCode = "8B 44 24 04 C3"
    goAnyCall.IsStandardCall = False
    lst.AddItem "返回值:" & goAnyCall.CallCodeBytes(sByteCode, 123)
    
    '按地址运行汇编语言字节
    lst.AddItem "按地址运行汇编语言字节,调用约定为__stdcall"
    sByteCode = "8B 44 24 04 C2 04 00"
    bBinCode = goAnyCall.ByteCodeStrToBin(sByteCode)
    goAnyCall.IsStandardCall = True
    lst.AddItem "返回值:" & goAnyCall.CallByAddress(VarPtr(bBinCode(0)), 123)
    
    lst.AddItem "按地址运行汇编语言字节,调用约定为__cdecl"
    sByteCode = "8B 44 24 04 C3"
    bBinCode = goAnyCall.ByteCodeStrToBin(sByteCode)
    goAnyCall.IsStandardCall = False
    lst.AddItem "返回值:" & goAnyCall.CallByAddress(VarPtr(bBinCode(0)), 123)
    
    '按地址运行VB函数
    lst.AddItem "按地址运行VB函数,调用约定为__stdcall"
    lst.AddItem "i=" & CStr(i)
    goAnyCall.IsStandardCall = True
    lst.AddItem "返回值:" & goAnyCall.CallByAddress(AddressOf CallTest, VarPtr(i))
        
    '调用Dll函数
    'C函数原型
    'int __stdcall CallTestS(int *a){
    '    int b;
    '    b=*a;
    '    *a+=111;
    '    return b;
    '}
    lst.AddItem "调用Dll函数,调用约定为__stdcall"
    goAnyCall.IsStandardCall = True
    lst.AddItem "返回值:" & goAnyCall.CallApiByName(sDll, "CallTestS", VarPtr(i))
    lst.AddItem "i=" & CStr(i)
    
    'C函数原型
    'int __cdecl CallTestC(int *a){
    '    int b;
    '    b=*a;
    '    *a+=111;
    '    return b;
    '}
    lst.AddItem "调用Dll函数,调用约定为__cdecl"
    goAnyCall.IsStandardCall = False
    lst.AddItem "返回值:" & goAnyCall.CallApiByName(sDll, "CallTestC", VarPtr(i))
    lst.AddItem "i=" & CStr(i)
    
    Exit Sub
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Sub


