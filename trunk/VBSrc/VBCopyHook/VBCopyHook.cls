VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBCopyHook"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 ��Ȩ���� (c), 2000 - 2010, �����̶�
'
' ************************   ģ �� ��   :CVBCopyHook********************
'�� �� ��: V1.0
'��    ��: �����̶�
'��������: 2004��06��19��
'����޸�:
'��������:
'�����б�:
'�޸���ʷ:
'��    ��: 2004��06��19��
'�޸���Ա: �����̶�
'�޸�����: ����
'******************************************************************************/
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CVBCopyHook"

Implements ICopyHookA
Implements ICopyHookW

Private m_lpOldFuncA As Long
Private m_lpOldFuncW As Long

Private Sub Class_Initialize()
    Call PrepareCodes
    
    Dim oICopyHookA As ICopyHookA
    Set oICopyHookA = Me
    m_lpOldFuncA = ReplaceVTableEntry(ObjPtr(oICopyHookA), 4, VarPtr(g_abCodesA(0)))
    
    Dim oICopyHookW As ICopyHookW
    Set oICopyHookW = Me
    m_lpOldFuncW = ReplaceVTableEntry(ObjPtr(oICopyHookW), 4, VarPtr(g_abCodesW(0)))
End Sub

Private Sub ICopyHookA_CopyCallback(ByVal hwnd As Long, ByVal wFunc As Long, ByVal wFlags As Long, ByVal pszSrcFile As Long, ByVal dwSrcAttribs As Long, ByVal pszDestFile As Long, ByVal dwDestAttribs As Long)
    'Ansi Version
End Sub

Private Sub ICopyHookW_CopyCallback(ByVal hwnd As Long, ByVal wFunc As Long, ByVal wFlags As Long, ByVal pszSrcFile As Long, ByVal dwSrcAttribs As Long, ByVal pszDestFile As Long, ByVal dwDestAttribs As Long)
    'Unicode Version
End Sub