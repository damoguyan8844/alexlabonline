VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CResEntries"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"No"
Attribute VB_Ext_KEY = "Collection" ,"CResEntry"
Attribute VB_Ext_KEY = "Member0" ,"CResEntry"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CResEntries********************
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
Private Const THIS_MODULE_NAME As String = "CResEntries"


'局部变量，保存集合
Private m_colCol As Collection
'本组资源条目的类型
Private m_lResTypeOfEntries As Long
Private m_sResTypeOfEntriesStr As String
'
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
'/**********************************属 性 名  : ItemKey*********************/
'
'功能描述:
'
'/**************************************************************************/
Public Property Get ItemKey() As String
     ItemKey = "K:" & CStr(m_lResTypeOfEntries)
End Property


'/**********************************属 性 名  : ResTypeOfEntries*********************/
'
'功能描述: 本组资源条目的类型
'
'/**************************************************************************/
Friend Property Let ResTypeOfEntriesLng(lResTypeOfEntries As Long)
    m_lResTypeOfEntries = lResTypeOfEntries
    m_sResTypeOfEntriesStr = GetResTypeStr(lResTypeOfEntries)
End Property

Public Property Get ResTypeOfEntries() As String
     ResTypeOfEntries = m_sResTypeOfEntriesStr
End Property



Public Function Add(lResName As Long) As CResEntry
    On Error Resume Next
    Dim oResEntry As CResEntry
    Dim sKey As String
    sKey = CStr(lResName)
    Set Add = m_colCol(sKey)
    If Add Is Nothing Then
        Set oResEntry = New CResEntry
        With oResEntry
            .Filename = m_sFilename
            .ResNameLng = lResName
            .ResTypeLng = m_lResTypeOfEntries
        End With
        m_colCol.Add oResEntry, sKey
        Set Add = oResEntry
    End If
End Function

Public Property Get OneResEntry(vKey As Variant) As CResEntry
Attribute OneResEntry.VB_UserMemId = 0
    On Error Resume Next
    Set OneResEntry = m_colCol(vKey)
End Property



Public Property Get Count() As Long
    Count = m_colCol.Count
End Property


Public Sub Remove(lResName As Long)
    m_colCol.Remove lResName
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

