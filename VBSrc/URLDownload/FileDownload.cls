VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CFileDownload"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Implements IBindStatusCallback

'获得字符串的函数
Private Declare Function lstrlenA Lib "kernel32" (ByVal lpString As Long) As Long
Private Declare Function lstrlenW Lib "kernel32" (ByVal lpString As Long) As Long

Private Declare Function lstrcpyA Lib "kernel32" (ByVal lpString1 As Long, ByVal lpString2 As Long) As Long
Private Declare Function lstrcpyW Lib "kernel32" (ByVal lpString1 As Long, ByVal lpString2 As Long) As Long
   
'下载函数
Private Declare Function URLDownloadToFile Lib "urlmon" Alias "URLDownloadToFileA" (ByVal pCaller As Long, ByVal szURL As String, ByVal szFileName As String, ByVal dwReserved As Long, ByVal lpfnCB As Long) As Long

'控制下载的接口
Private m_oBind As IBinding
'是否在下载
Private m_fDownloading As Boolean
'对于下载控制接口的引用数
Private m_lRefCount As Long

'下载进度的事件
Public Event OnProgress(ByVal lProgress As Long, ByVal lMaxProgress As Long, ByVal lStatusCode As Long, ByVal sStatusText As String)

'初始化
Private Sub Class_Initialize()
    m_fDownloading = False
    m_lRefCount = 0
End Sub

'结束
Private Sub Class_Terminate()
    If m_lRefCount = 1 Then
        If Not m_oBind Is Nothing Then m_oBind.Release
    End If
    m_fDownloading = False
End Sub

'开始下载
Public Function StartDownloading(ByVal sSrc As String, ByVal sDest As String) As Boolean
    '如果已经在下载则退出
    If m_fDownloading Then Exit Function
    
    Dim oBindCallback As IBindStatusCallback
    
    '获得IBindStatusCallback接口对象
    Set oBindCallback = Me
    
    '开始下载
    StartDownloading = (URLDownloadToFile(ObjPtr(Me), sSrc, sDest, 0, ObjPtr(oBindCallback)) = 0)
End Function

'中止下载
Public Sub AbortDownloading()
    On Error Resume Next
    If m_lRefCount = 1 Then
        If Not m_oBind Is Nothing Then m_oBind.Abort
    End If
    m_fDownloading = False
End Sub

'从字符指针获得字符串
Public Function StrFromPtr(ByVal lpString As Long, Optional fUnicode As Boolean = False) As String
    On Error Resume Next
    If fUnicode Then
        StrFromPtr = String(lstrlenW(lpString), Chr(0))
        lstrcpyW StrPtr(StrFromPtr), ByVal lpString
    Else
        StrFromPtr = String(lstrlenA(lpString), Chr(0))
        lstrcpyA ByVal StrFromPtr, ByVal lpString
    End If
End Function

'*********************************************************************************************************************************************
'IBindStatusCallback接口成员
'*********************************************************************************************************************************************
Private Sub IBindStatusCallback_GetBindInfo(grfBINDF As Long, pbindinfo As Long)
'
End Sub

Private Sub IBindStatusCallback_GetPriority(pnPriority As Long)
'
End Sub

Private Sub IBindStatusCallback_OnDataAvailable(ByVal grfBSCF As Long, ByVal dwSize As Long, pformatetc As Long, pstgmed As Long)
'
End Sub

Private Sub IBindStatusCallback_OnLowResource(ByVal reserved As Long)
'
End Sub

Private Sub IBindStatusCallback_OnObjectAvailable(ByVal riid As Long, ByVal punk As URLMonLib.IUnknownVB)
'
End Sub

'下载进度
Private Sub IBindStatusCallback_OnProgress(ByVal ulProgress As Long, ByVal ulProgressMax As Long, ByVal ulStatusCode As Long, ByVal szStatusText As Long)
    RaiseEvent OnProgress(ulProgress, ulProgressMax, ulStatusCode, StrFromPtr(szStatusText, True))
    DoEvents
End Sub

'开始下载绑定
Private Sub IBindStatusCallback_OnStartBinding(ByVal dwReserved As Long, ByVal pib As URLMonLib.IBinding)
    m_fDownloading = True
    Set m_oBind = pib
    m_oBind.AddRef
    m_lRefCount = 1
End Sub

'结束下载绑定
Private Sub IBindStatusCallback_OnStopBinding(ByVal hresult As Long, ByVal szError As Long)
    m_fDownloading = False
    If m_lRefCount = 1 Then
        m_oBind.Release
        m_lRefCount = 0
    End If
End Sub

Private Sub IBindStatusCallback_RemoteGetBindInfo(grfBINDF As Long, pbindinfo As Long, pstgmed As Long)
'
End Sub

Private Sub IBindStatusCallback_RemoteOnDataAvailable(ByVal grfBSCF As Long, ByVal dwSize As Long, pformatetc As Long, pstgmed As Long)
'
End Sub
