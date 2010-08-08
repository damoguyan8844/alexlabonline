VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CResLib"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CResLaLaLa********************
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
Private Const THIS_MODULE_NAME As String = "CResLaLaLa"

'列表是否已经初始化
Private m_fIsConstructed As Boolean
Private m_sFilename As String
Private m_oResPackage As CResPackage

'/**********************************属 性 名  : ResPackage*********************/
'
'功能描述:资源包
'
'/**************************************************************************/
Public Property Get ResPackage() As CResPackage
    If Not m_fIsConstructed Then
        'Set ResPackage = Nothing
        Exit Property
    End If
    
    Set ResPackage = m_oResPackage
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


'/**********************************函 数 名  : CreateResList*********************/
'功能描述: 创建资源列表
'
'参    数:
'
'返 回 值: Boolean
'/**************************************************************************/
Public Function CreateResList() As Boolean
    On Error GoTo Error_Handler
    
    Set m_oResPackage = Nothing
    
    Set m_oResPackage = New CResPackage
    
    m_oResPackage.Filename = m_sFilename
    

    m_fIsConstructed = CreateResListWithObject(m_oResPackage)
    CreateResList = m_fIsConstructed
    Exit Function
Error_Handler:
    '自定义错误处理
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


Private Sub Class_Terminate()
    Set m_oResPackage = Nothing
End Sub
