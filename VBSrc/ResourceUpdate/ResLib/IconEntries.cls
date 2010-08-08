VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIconEntries"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CIconEntries********************
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
Private Const THIS_MODULE_NAME As String = "CIconEntries"


'局部变量，保存集合
Private m_colCol As Collection

Private m_sFilename As String
'/**********************************属 性 名  : Filename*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let Filename(sFilename As String)
    m_sFilename = sFilename
End Property

Public Property Get Filename() As String
     Filename = m_sFilename
End Property


Friend Function Add() As CIconEntry
    'On Error Resume Next
    Dim oIconEntry As CIconEntry
    
    Set oIconEntry = New CIconEntry
    With oIconEntry
        .Filename = m_sFilename
        .IconName = "#" & CStr(m_colCol.Count + 1)
        '.ItemKey = "K:" & CStr(m_colCol.Count)
        m_colCol.Add oIconEntry, .ItemKey
    End With
    Set Add = oIconEntry
End Function

Public Property Get IconEntry(vKey As Variant) As CIconEntry
    On Error Resume Next
    Set IconEntry = m_colCol(vKey)
End Property



Public Property Get Count() As Long
    Count = m_colCol.Count
End Property


Public Sub Remove(lIconName As Long)
    m_colCol.Remove lIconName
End Sub


Public Property Get NewEnum() As IUnknown
Attribute NewEnum.VB_UserMemId = -4
Attribute NewEnum.VB_MemberFlags = "40"
    '本属性允许用 For...Each 语法枚举该集合。
    Set NewEnum = m_colCol.[_NewEnum]
End Property


Private Sub Class_Initialize()
    '创建类后创建集合
    Set m_colCol = New Collection
End Sub


Private Sub Class_Terminate()
    '类终止后破坏集合
    Set m_colCol = Nothing
End Sub


