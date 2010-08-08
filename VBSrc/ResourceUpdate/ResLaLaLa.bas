Attribute VB_Name = "mduResLaLaLa"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :mduResLaLaLa********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年03月07日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月07日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "mduResLaLaLa"

Public g_oResLib(0 To 1) As CResLib
Public g_oIconLib As CIconLib
Public g_fIsIconMode As Boolean

Public Const RT_CURSOR = 1&
Public Const RT_BITMAP = 2&
Public Const RT_ICON = 3&
Public Const RT_MENU = 4&
Public Const RT_DIALOG = 5&
Public Const RT_STRING = 6&
Public Const RT_FONTDIR = 7&
Public Const RT_FONT = 8&
Public Const RT_ACCELERATOR = 9&
Public Const RT_RCDATA = 10&
Public Const RT_MESSAGETABLE = 11&
Public Const DIFFERENCE = 11&
Public Const RT_GROUP_CURSOR = RT_CURSOR + DIFFERENCE
Public Const RT_GROUP_ICON = RT_ICON + DIFFERENCE
Public Const RT_VERSION = 16&
Public Const RT_DLGINCLUDE = 17&
Public Const RT_PLUGPLAY = 19&
Public Const RT_VXD = 20&
Public Const RT_ANICURSOR = 21&
Public Const RT_ANIICON = 22&
Public Const RT_HTML = 23&

Function FileExists(sFileName As String) As Boolean
    On Error Resume Next
    FileExists = (FileLen(sFileName) <> 0)
End Function
