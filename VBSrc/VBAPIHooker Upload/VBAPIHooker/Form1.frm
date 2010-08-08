VERSION 5.00
Begin VB.Form frm 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "API Hook 示例"
   ClientHeight    =   7620
   ClientLeft      =   150
   ClientTop       =   435
   ClientWidth     =   8985
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7620
   ScaleWidth      =   8985
   StartUpPosition =   2  '屏幕中心
   Begin VB.Frame Frame2 
      Caption         =   "其他进程拦截测试"
      Height          =   1155
      Left            =   480
      TabIndex        =   7
      Top             =   6330
      Width           =   8055
      Begin VB.CommandButton Command3 
         Caption         =   "启动空白记事本程序"
         Height          =   615
         Left            =   960
         TabIndex        =   11
         Top             =   330
         Width           =   2595
      End
      Begin VB.CommandButton Command4 
         Caption         =   "测试拦截记事本"
         Height          =   615
         Left            =   4590
         TabIndex        =   8
         Top             =   330
         Width           =   2595
      End
      Begin VB.Label Label5 
         Caption         =   "第2步"
         Height          =   255
         Left            =   4080
         TabIndex        =   13
         Top             =   540
         Width           =   465
      End
      Begin VB.Label Label4 
         AutoSize        =   -1  'True
         Caption         =   "第1步"
         Height          =   180
         Left            =   450
         TabIndex        =   12
         Top             =   540
         Width           =   450
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "本进程拦截测试（请编译后测试）"
      Height          =   1155
      Left            =   480
      TabIndex        =   4
      Top             =   5070
      Width           =   8055
      Begin VB.CommandButton Command2 
         Caption         =   "测试弹出消息框"
         Height          =   615
         Left            =   4590
         TabIndex        =   6
         Top             =   330
         Width           =   2595
      End
      Begin VB.CommandButton Command1 
         Caption         =   "拦截本进程 MsgBox(MessageBoxIndirectA)"
         Height          =   615
         Left            =   960
         TabIndex        =   5
         Top             =   330
         Width           =   2595
      End
      Begin VB.Label Label3 
         Caption         =   "第2步"
         Height          =   255
         Left            =   4080
         TabIndex        =   10
         Top             =   540
         Width           =   465
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "第1步"
         Height          =   180
         Left            =   450
         TabIndex        =   9
         Top             =   540
         Width           =   450
      End
   End
   Begin VB.ListBox List1 
      Height          =   1860
      Left            =   480
      TabIndex        =   1
      Top             =   3120
      Width           =   8055
   End
   Begin VB.TextBox Text1 
      Height          =   2205
      Left            =   480
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   390
      Width           =   8025
   End
   Begin VB.Label Label1 
      Caption         =   "堆栈内容："
      Height          =   285
      Index           =   1
      Left            =   480
      TabIndex        =   3
      Top             =   120
      Width           =   1365
   End
   Begin VB.Label Label1 
      Caption         =   "调用信息："
      Height          =   285
      Index           =   0
      Left            =   480
      TabIndex        =   2
      Top             =   2820
      Width           =   1365
   End
End
Attribute VB_Name = "frm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long
Private Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hWnd As Long, lpdwProcessId As Long) As Long

'声明钩子类
Private WithEvents m_oApiHooker As CVBAPIHooker
Attribute m_oApiHooker.VB_VarHelpID = -1
Private WithEvents m_oAPIHooker2 As CVBAPIHooker
Attribute m_oAPIHooker2.VB_VarHelpID = -1

Private Sub Command1_Click()
    Static lHookIndex As Long
    If Command1.Caption = "终止拦截" Then
        Command1.Caption = "拦截本进程 MessageBoxIndirectA"
        
        m_oAPIHooker2.Unhook lHookIndex
        
        Set m_oAPIHooker2 = Nothing
    Else
        '拦截本进程
        Set m_oAPIHooker2 = New CVBAPIHooker
    
        With m_oAPIHooker2
            .SetTargetTo -1
            
            If GetModuleHandle("vba6.dll") = 0 Then
                Command1.Caption = "终止拦截"
                
                lHookIndex = .HookImport("msvbvm60.dll", "user32.dll", "MessageBoxIndirectA")
            Else
                MsgBox "因为未知原因在IDE中无法触发BeforeAction和AfterAction事件，所以如果要测试本项请编译后再试。"
            End If
        End With
    End If
End Sub

Private Sub Command2_Click()
    Dim lRetVal As Long
    lRetVal = MsgBox("测试！", vbExclamation + vbYesNoCancel + vbDefaultButton3)
    AddMsg "你点了""" & IIf(lRetVal = vbYes, "是", IIf(lRetVal = vbNo, "否", "取消")) & """"
End Sub

Private Sub Command3_Click()
    Shell "notepad.exe"
End Sub

Private Sub Command4_Click()
    Static lHookIndex As Long, lHookIndex2 As Long
    If Command4.Caption = "终止拦截" Then
        Command4.Caption = "测试拦截"
        
        m_oApiHooker.Unhook lHookIndex
        m_oApiHooker.Unhook lHookIndex2
    
        Set m_oApiHooker = Nothing
    Else
        Command4.Caption = "终止拦截"
        
        '拦截记事本函数
        Set m_oApiHooker = New CVBAPIHooker
    
        With m_oApiHooker
            Dim pid
            GetWindowThreadProcessId FindWindow(vbNullString, "无标题 - 记事本"), pid
            .SetTargetTo pid
            lHookIndex = .HookImport("notepad.exe", "comdlg32.dll", "GetOpenFileNameW")
            lHookIndex2 = .HookImport("notepad.exe", "comdlg32.dll", "ChooseFontW")
        End With
    End If
End Sub

Private Sub AddMsg(ByVal sMsg As String)
    With List1
        .AddItem sMsg
        .Selected(.NewIndex) = True
    End With
End Sub

Private Sub AddTxt(ByVal sTxt As String)
    With Text1
        .Text = .Text & sTxt
        .SelStart = Len(.Text)
    End With
End Sub

Private Function Array2Str(v As Variant, ByVal lDataLen As Long, Optional lNewLinePos As Long = 4) As String
    If IsArray(v) Then
        Dim i As Long
        Array2Str = ""
        For i = LBound(v) To UBound(v)
            Array2Str = Array2Str & " " & IIf((i Mod lNewLinePos = 0) And i <> 0, vbCrLf, "") & Right$(String(lDataLen * 2, "0") & Hex(v(i)), lDataLen * 2)
        Next
        Array2Str = Trim(Array2Str)
    End If
End Function

Private Sub m_oApiHooker_BeforeAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal lpArgs As Long, fIntercept As Boolean, lArgCount As Long, ByVal lReturnAddress As Long)
    AddMsg "即将调用函数 " & sAPIName & " ,参数堆栈地址为：&H" & Hex(lpArgs)
    
    Select Case sAPIName
    Case "GetOpenFileNameW"
        '显示堆栈内容
        Dim abDumpData() As Byte
        Dim alArgs() As Long
        
        m_oApiHooker.DumpStack abDumpData(), 128
        m_oApiHooker.GetArguments alArgs(), 16
        
        AddTxt "单字节方式：" & vbCrLf
        AddTxt Array2Str(abDumpData(), 1, 8) & vbCrLf & vbCrLf
        AddTxt "四字节方式：" & vbCrLf
        AddTxt Array2Str(alArgs(), 4, 4) & vbCrLf & vbCrLf
        fIntercept = False
    Case "ChooseFontW"
        '阻止API
        fIntercept = True
        lArgCount = 1
    End Select
End Sub

Private Sub m_oApiHooker_AfterAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal fIntercepted As Boolean, lRetValue As Long)
    AddMsg "函数 " & sAPIName & IIf(fIntercepted, "被阻止", " 调用完毕,返回参数为：&H" & Hex(lRetValue))
    
    Select Case sAPIName
    Case "GetOpenFileNameW"
    Case "ChooseFontW"
    End Select
    
    '修改函数返回值
    'lRetValue = 1
End Sub

Private Sub m_oAPIHooker2_BeforeAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal lpArgs As Long, fIntercept As Boolean, lArgCount As Long, ByVal lReturnAddress As Long)
    AddMsg "即将调用函数 " & sAPIName & " ,参数堆栈地址为：&H" & Hex(lpArgs)
    
    '显示堆栈内容
    Dim abDumpData() As Byte
    Dim alArgs() As Long
    
    m_oAPIHooker2.DumpStack abDumpData(), 128
    m_oAPIHooker2.GetArguments alArgs(), 16
    
    AddTxt "单字节方式：" & vbCrLf
    AddTxt Array2Str(abDumpData(), 1, 8) & vbCrLf & vbCrLf
    AddTxt "四字节方式：" & vbCrLf
    AddTxt Array2Str(alArgs(), 4, 4) & vbCrLf & vbCrLf
     
    fIntercept = False
    lArgCount = 1
End Sub

Private Sub m_oAPIHooker2_AfterAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal fIntercepted As Boolean, lRetValue As Long)
    AddMsg "函数 " & sAPIName & IIf(fIntercepted, "被阻止", " 调用完毕,返回参数为：&H" & Hex(lRetValue))
    
    '修改函数返回值
    lRetValue = vbYes
End Sub
