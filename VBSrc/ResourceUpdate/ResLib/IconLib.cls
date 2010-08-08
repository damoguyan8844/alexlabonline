VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIconLib"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 陈缘
'
' ************************   模 块 名   :CIconLib********************
'版 本 号: V1.0
'作    者: 陈缘
'生成日期: 2004年03月10日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月10日
'修改人员: 陈缘
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CIconLib"


'列表是否已经初始化
Private m_fIsConstructed As Boolean
Private m_sFilename As String
Private m_oIconPackage As CIconPackage

'/**********************************属 性 名  : IconPackage*********************/
'
'功能描述:资源包
'
'/**************************************************************************/
Public Property Get IconPackage() As CIconPackage
    If Not m_fIsConstructed Then
        'Set IconPackage = Nothing
        Exit Property
    End If
    
    Set IconPackage = m_oIconPackage
End Property



'/**********************************属 性 名  : IsConstructed*********************/
'
'功能描述:列表是否已经初始化
'
'/**************************************************************************/
Public Property Let IsConstructed(fIsConstructed As Boolean)
    m_fIsConstructed = fIsConstructed
End Property

Public Property Get IsConstructed() As Boolean
     IsConstructed = m_fIsConstructed
End Property


'/**********************************属 性 名  : Filename*********************/
'
'功能描述:资源文件名
'
'/**************************************************************************/
Public Property Let Filename(sFilename As String)
    m_sFilename = sFilename
    m_fIsConstructed = False
End Property

Public Property Get Filename() As String
    Filename = m_sFilename
End Property


'/**********************************函 数 名  : CreateIconList*********************/
'功能描述: 创建资源列表
'
'参    数:
'
'返 回 值: Boolean
'/**************************************************************************/
Public Function CreateIconList() As Boolean
    On Error GoTo Error_Handler
    
    Set m_oIconPackage = Nothing
    
    Set m_oIconPackage = New CIconPackage
    
    m_oIconPackage.Filename = m_sFilename
    

    m_fIsConstructed = CreateIconListWithObject(m_oIconPackage)
    CreateIconList = m_fIsConstructed
    Exit Function
Error_Handler:
    '自定义错误处理
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

Private Sub Class_Terminate()
    Set m_oIconPackage = Nothing
End Sub

