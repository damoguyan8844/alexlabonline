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
'                 版权所有 (c), 2000 - 2010, 超级绿豆
'
' ************************   模 块 名   :CVBCopyHook********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年06月19日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年06月19日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
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
