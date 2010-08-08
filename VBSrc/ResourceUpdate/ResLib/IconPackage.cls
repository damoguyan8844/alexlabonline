VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIconPackage"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CIconPackage********************
'版 本 号: V1.0
'作    者: 陈缘
'生成日期: 2004年03月07日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月07日
'修改人员: 陈缘
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CIconPackage"

Private m_sFilename As String
Private m_oIconEntries As CIconEntries
'/**********************************属 性 名  : Filename*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let Filename(sFilename As String)
    m_sFilename = sFilename
    m_oIconEntries.Filename = m_sFilename
End Property

Public Property Get Filename() As String
     Filename = m_sFilename
End Property

Public Property Get IconEntries() As CIconEntries
    On Error Resume Next
    Set IconEntries = m_oIconEntries
End Property


Private Sub Class_Initialize()
    Set m_oIconEntries = New CIconEntries
End Sub


Private Sub Class_Terminate()
    Set m_oIconEntries = Nothing
End Sub


