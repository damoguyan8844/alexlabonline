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
   StartUpPosition =   3  '����ȱʡ
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

'���������Լ����Զ�����ṩԴ����
Public MySource As CMyAutoCompleteSource

Private Sub Form_Load()
    '�趨���������Ŀ�괰��
    g_hTargetWnd = Form1.hWnd
    '��__vbaSetSystemError������������������Ϳ��Բ������Ϳ��ֱ���ڶ��̻߳�����������ʹ��API����
    Call PatchSetSystemError
    '��ʼ���ദ��ǰ����
    Subclassing g_hTargetWnd
    
    '��ʼ���Զ���ɶ���
    Call BindToTextBox(txt(), 5)
End Sub

Private Sub Form_Unload(Cancel As Integer)
    'ȡ�����ദ��
    Subclassing g_hTargetWnd, True
End Sub

'�����ֲ�ͬ��AutoComplete��ʽ�ֱ�󶨵�5���ı���
Public Sub BindToTextBox(txt As Variant, ByVal lCount As Long)
    '����5���Զ���ɶ���
    Dim oAutoComplete(4) As IAutoComplete
    Dim oObjMgr As IObjMgr
    Dim oACLHistory As IACList
    Dim oACLListISF As IACList
    Dim oACLMRU As IACList
    
    Dim i As Long
    Dim hTxtWnd As Long
    
    '���ַ������͵�GUIDת��ΪGUID�ṹ
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
    
    '************* ��ʼ���Զ���Դ ��ʼ***************
    Set MySource = New CMyAutoCompleteSource
    '************* ��ʼ���Զ���Դ ���***************
    
    '************* ����ϵͳ������ṩ���� ��ʼ***************
    '�ṩ��ʷ��¼
    CoCreateInstance CLSID_ACLHistory, 0, 1, IID_IUnknown, oACLHistory
    '�ṩ�ҵĵ��Լ��ղؼ�����
    CoCreateInstance CLSID_ACListISF, 0, 1, IID_IUnknown, oACLListISF
    Dim oACList2 As IACList2
    Set oACList2 = oACLListISF
    oACList2.SetOptions ACLO_MYCOMPUTER + ACLO_FAVORITES
    '�ṩ������е�����
    CoCreateInstance CLSID_ACLMRU, 0, 1, IID_IUnknown, oACLMRU
    '************* ����ϵͳ������ṩ���� ���***************
    
    '�����Զ���ɶ���
    For i = 0 To lCount - 1
        CoCreateInstance CLSID_IAutoComplete, 0, 1, IID_IAutoComplete, oAutoComplete(i)
    Next
              
    For i = 0 To lCount - 1
        hTxtWnd = txt(i).hWnd
        
        SetOptions oAutoComplete(i)
        
        Select Case i
        Case 0
            '�󶨵���1���ı���History
            Call oAutoComplete(i).Init(hTxtWnd, oACLHistory, 0, 0)
        Case 1
            '�󶨵���2���ı���File System
            Call oAutoComplete(i).Init(hTxtWnd, oACLListISF, 0, 0)
        Case 2
            '�󶨵���3���ı���MRU
            Call oAutoComplete(i).Init(hTxtWnd, oACLMRU, 0, 0)
        Case 3
            '�󶨵���4���ı���User Defined
            Call oAutoComplete(i).Init(hTxtWnd, MySource, 0, 0)
        Case Else
            '�󶨵���5���ı���Compound
            CoCreateInstance CLSID_ACLMulti, 0, 1, IID_IObjMgr, oObjMgr
    
            oObjMgr.Append oACLHistory
            oObjMgr.Append oACLListISF
            oObjMgr.Append oACLMRU
            
            Call oAutoComplete(i).Init(hTxtWnd, oObjMgr, 0, 0)
        End Select
    Next
End Sub

'�趨�Զ���ɶ���Ĺ�����ʽ
'�������MSDN
Sub SetOptions(ByVal oIAutoComplete2 As IAutoComplete2)
    oIAutoComplete2.SetOptions ACO_AUTOSUGGEST + ACO_AUTOAPPEND + ACO_UPDOWNKEYDROPSLIST + ACO_SEARCH
End Sub

