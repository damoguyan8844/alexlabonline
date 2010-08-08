VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIconEntry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CIconEntry********************
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
Private Const THIS_MODULE_NAME As String = "CIconEntry"

'大小
Private m_lIconSize As Long
'
Private m_sFilename As String
'
Private m_sItemKey As String
'
Private m_lRawDataOffset As Long
'
Private m_lIconHeight As Long
'
Private m_lIconWidth As Long
'
Private m_sIconName As String
'
Private m_lOpDrawIcon As Boolean
'
Private m_lIconType As Long
'/**********************************属 性 名  : IconType*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let IconType(lIconType As Long)
    m_lIconType = lIconType
End Property

Public Property Get IconType() As Long
     IconType = m_lIconType
End Property


'/**********************************属 性 名  : IconName*********************/
'
'功能描述:
'
'/**************************************************************************/
Public Property Let IconName(sIconName As String)
    m_sIconName = sIconName
End Property

Public Property Get IconName() As String
     IconName = m_sIconName
End Property


'/**********************************属 性 名  : IconWidth*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let IconWidth(lIconWidth As Long)
    m_lIconWidth = lIconWidth
End Property

Public Property Get IconWidth() As Long
     IconWidth = m_lIconWidth
End Property


'/**********************************属 性 名  : IconHeight*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let IconHeight(lIconHeight As Long)
    m_lIconHeight = lIconHeight
End Property

Public Property Get IconHeight() As Long
     IconHeight = m_lIconHeight
End Property


'/**********************************属 性 名  : RawDataOffset*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let RawDataOffset(lRawDataOffset As Long)
    m_lRawDataOffset = lRawDataOffset
End Property

Public Property Get RawDataOffset() As Long
     RawDataOffset = m_lRawDataOffset
End Property


'/**********************************属 性 名  : ItemKey*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let ItemKey(sItemKey As String)
    m_sItemKey = sItemKey
End Property

Public Property Get ItemKey() As String
     ItemKey = "K:" & m_sIconName 'm_sItemKey
End Property



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



'/**********************************属 性 名  : IconSize*********************/
'
'功能描述:资源大小
'
'/**************************************************************************/
Friend Property Let IconSize(lIconSize As Long)
    m_lIconSize = lIconSize
End Property

Public Property Get IconSize() As Long
     IconSize = m_lIconSize
End Property

'/**********************************函 数 名  : GetRawBytes*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Byte()
'/**************************************************************************/
Public Function GetRawBytes() As Byte()
    On Error GoTo Error_Handler
    Dim b() As Byte
    Dim lFreefile As Long
    lFreefile = FreeFile
    
    Open m_sFilename For Binary As #lFreefile
    
    Seek #lFreefile, m_lRawDataOffset + 1
    ReDim b(m_lIconSize - 1) As Byte
    Get #lFreefile, , b
    
    Close #lFreefile
    GetRawBytes = b
    Exit Function
Error_Handler:
    '自定义错误处理
    Close #lFreefile
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**********************************函 数 名  : OpDrawIcon*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Boolean
'/**************************************************************************/
Public Function OpDrawIcon(lhDestDC As Long) As Boolean
    On Error GoTo Error_Handler
    Dim hIcon As Long, lRet As Long
    Dim bRawBytes() As Byte
    
    bRawBytes = GetRawBytes()
    
    If SizeOfArray(bRawBytes) <= 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If
    
    hIcon = CreateIconFromResource(VarPtr(bRawBytes(0)), m_lIconSize, IIf(m_lIconType = 1, 1&, 0&), &H30000)
    
    If hIcon = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If
    
    lRet = DrawIconEx(lhDestDC, 0, 0, hIcon, 0&, 0&, 0&, 0&, DI_DEFAULTSIZE Or DI_NORMAL)
    
    DestroyIcon hIcon
    
    Exit Function
Error_Handler:
    '自定义错误处理
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
