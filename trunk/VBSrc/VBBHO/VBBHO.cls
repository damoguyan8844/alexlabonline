VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBBrowserHelperObject"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 超级绿豆
'
' ************************   模 块 名   :CVBBrowserHelperObject********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年09月29日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年09月29日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBBrowserHelperObject"

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Implements IObjectWithSite

Private m_oSite As IUnknownVB
Private WithEvents m_oBrowser As InternetExplorer
Attribute m_oBrowser.VB_VarHelpID = -1

Private Sub IObjectWithSite_GetSite(ByVal priid As Long, ppvObj As Long)
    m_oSite.QueryInterface priid, ppvObj
End Sub

Private Sub IObjectWithSite_SetSite(ByVal pSite As BHOLib.IUnknownVB)
    If ObjPtr(pSite) = 0 Then
        CopyMemory m_oBrowser, 0&, 4
        Exit Sub
    End If
    
    Set m_oSite = pSite
    Set m_oBrowser = pSite
End Sub

Private Sub m_oBrowser_BeforeNavigate2(ByVal pDisp As Object, URL As Variant, Flags As Variant, TargetFrameName As Variant, PostData As Variant, Headers As Variant, Cancel As Boolean)
    On Error Resume Next
    '根据URL地址的末尾字符判断是否为要下载文件
    'FlashGet，NetAnts，影音传送带等下载软件的原理类似于此
    If IsDownloadingFile(URL) Then
        MsgBox "即将下载文件 " & URL
    End If
    
    '如果新窗口是没有工具条和菜单条的，那么就认为是弹出窗口，退出
    With m_oBrowser
        If .ToolBar = False And .MenuBar = False Then
            .Quit
        End If
    End With
End Sub

Private Sub m_oBrowser_NewWindow2(ppDisp As Object, Cancel As Boolean)
    '直接取消任何新窗口的创建
    'Cancel = True
End Sub


'根据URL地址判断是否为下载文件
Private Function IsDownloadingFile(ByVal URL As String, Optional sFileType As String) As Boolean
    IsDownloadingFile = True
    
    If UCase(Right$(URL, 4)) = ".RAR" Then
        sFileType = ".RAR"
        Exit Function
    End If

    If UCase(Right$(URL, 4)) = ".ZIP" Then
        sFileType = ".ZIP"
        Exit Function
    End If

    If UCase(Right$(URL, 4)) = ".ISO" Then
        sFileType = ".ISO"
        Exit Function
    End If
    
    If UCase(Right$(URL, 4)) = ".WMV" Then
        sFileType = ".WMV"
        Exit Function
    End If

    IsDownloadingFile = False
End Function
