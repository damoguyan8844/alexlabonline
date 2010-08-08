VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "AutoComplete"
   ClientHeight    =   2265
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5655
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   ScaleHeight     =   2265
   ScaleWidth      =   5655
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox txt 
      Height          =   315
      Index           =   3
      Left            =   1290
      TabIndex        =   7
      Top             =   1350
      Width           =   4245
   End
   Begin VB.TextBox txt 
      Height          =   315
      Index           =   4
      Left            =   1290
      TabIndex        =   8
      Top             =   1740
      Width           =   4245
   End
   Begin VB.TextBox txt 
      Height          =   315
      Index           =   2
      Left            =   1290
      TabIndex        =   4
      Top             =   960
      Width           =   4245
   End
   Begin VB.TextBox txt 
      Height          =   315
      Index           =   1
      Left            =   1290
      TabIndex        =   2
      Top             =   570
      Width           =   4245
   End
   Begin VB.TextBox txt 
      Height          =   315
      Index           =   0
      Left            =   1290
      TabIndex        =   0
      Top             =   180
      Width           =   4245
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Right Justify
      Caption         =   "Compound:"
      Height          =   345
      Left            =   -150
      TabIndex        =   9
      Top             =   1770
      Width           =   1425
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Right Justify
      Caption         =   "User defined:"
      Height          =   345
      Left            =   -150
      TabIndex        =   6
      Top             =   1380
      Width           =   1425
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "MRU:"
      Height          =   345
      Left            =   -150
      TabIndex        =   5
      Top             =   990
      Width           =   1425
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "File System:"
      Height          =   345
      Left            =   -150
      TabIndex        =   3
      Top             =   600
      Width           =   1425
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "History:"
      Height          =   345
      Left            =   -150
      TabIndex        =   1
      Top             =   210
      Width           =   1425
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'定义我们自己的自动完成提供源对象
Public MySource As CMyAutoCompleteSource

Private Sub Form_Load()
    '设定处理操作的目标窗口
    g_hTargetWnd = Form1.hWnd
    '给__vbaSetSystemError函数打个补丁，这样就可以不用类型库而直接在多线程环境中声明并使用API函数
    Call PatchSetSystemError
    '开始子类处理当前窗口
    Subclassing g_hTargetWnd
    
    '开始绑定自动完成对象
    Call BindToTextBox(txt(), 5)
End Sub

Private Sub Form_Unload(Cancel As Integer)
    '取消子类处理
    Subclassing g_hTargetWnd, True
End Sub

'将几种不同的AutoComplete方式分别绑定到5个文本框
Public Sub BindToTextBox(txt As Variant, ByVal lCount As Long)
    '定义5个自动完成对象
    Dim oAutoComplete(4) As IAutoComplete
    Dim oObjMgr As IObjMgr
    Dim oACLHistory As IACList
    Dim oACLListISF As IACList
    Dim oACLMRU As IACList
    
    Dim i As Long
    Dim hTxtWnd As Long
    
    '将字符串类型的GUID转换为GUID结构
    CLSIDFromString StrPtr(CLSID_IAutoCompleteStr), CLSID_IAutoComplete
    CLSIDFromString StrPtr(IID_IAutoCompleteStr), IID_IAutoComplete
    CLSIDFromString StrPtr(IID_IAutoComplete2Str), IID_IAutoComplete2
    CLSIDFromString StrPtr(CLSID_ACLHistoryStr), CLSID_ACLHistory
    CLSIDFromString StrPtr(IID_IACListStr), IID_IACList
    CLSIDFromString StrPtr(IID_IACList2Str), IID_IACList2
    CLSIDFromString StrPtr(IID_IObjMgrStr), IID_IObjMgr
    CLSIDFromString StrPtr(CLSID_ACLMRUStr), CLSID_ACLMRU
    CLSIDFromString StrPtr(CLSID_ACListISFStr), CLSID_ACListISF
    CLSIDFromString StrPtr(CLSID_ACLMultiStr), CLSID_ACLMulti
    CLSIDFromString StrPtr(IID_IUnknownStr), IID_IUnknown
    
    '************* 初始化自定义源 开始***************
    Set MySource = New CMyAutoCompleteSource
    '************* 初始化自定义源 完毕***************
    
    '************* 创建系统定义的提供对象 开始***************
    '提供历史记录
    CoCreateInstance CLSID_ACLHistory, 0, 1, IID_IUnknown, oACLHistory
    '提供我的电脑及收藏夹内容
    CoCreateInstance CLSID_ACListISF, 0, 1, IID_IUnknown, oACLListISF
    Dim oACList2 As IACList2
    Set oACList2 = oACLListISF
    oACList2.SetOptions ACLO_MYCOMPUTER + ACLO_FAVORITES
    '提供最近运行的内容
    CoCreateInstance CLSID_ACLMRU, 0, 1, IID_IUnknown, oACLMRU
    '************* 创建系统定义的提供对象 完毕***************
    
    '创建自动完成对象
    For i = 0 To lCount - 1
        CoCreateInstance CLSID_IAutoComplete, 0, 1, IID_IAutoComplete, oAutoComplete(i)
    Next
              
    For i = 0 To lCount - 1
        hTxtWnd = txt(i).hWnd
        
        SetOptions oAutoComplete(i)
        
        Select Case i
        Case 0
            '绑定到第1个文本框History
            Call oAutoComplete(i).Init(hTxtWnd, oACLHistory, 0, 0)
        Case 1
            '绑定到第2个文本框File System
            Call oAutoComplete(i).Init(hTxtWnd, oACLListISF, 0, 0)
        Case 2
            '绑定到第3个文本框MRU
            Call oAutoComplete(i).Init(hTxtWnd, oACLMRU, 0, 0)
        Case 3
            '绑定到第4个文本框User Defined
            Call oAutoComplete(i).Init(hTxtWnd, MySource, 0, 0)
        Case Else
            '绑定到第5个文本框Compound
            CoCreateInstance CLSID_ACLMulti, 0, 1, IID_IObjMgr, oObjMgr
    
            oObjMgr.Append oACLHistory
            oObjMgr.Append oACLListISF
            oObjMgr.Append oACLMRU
            
            Call oAutoComplete(i).Init(hTxtWnd, oObjMgr, 0, 0)
        End Select
    Next
End Sub

'设定自动完成对象的工作方式
'具体请见MSDN
Sub SetOptions(ByVal oIAutoComplete2 As IAutoComplete2)
    oIAutoComplete2.SetOptions ACO_AUTOSUGGEST + ACO_AUTOAPPEND + ACO_UPDOWNKEYDROPSLIST + ACO_SEARCH
End Sub

