VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CMyThumbnailProvier"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'***********************************************************************************************************************
'
'模 块 名: CMyThumbnailProvier
'
'作    者: Supergreenbean
'
'功能描述: 自定义的ThumbnailProvier
'
'日    期: 2009年03月10日
'
'使用说明:
'
'       1. 在IInitializeWithStream_Initialize中获得包含目标文件内容的IStream对象
'
'       2. 在IThumbnailProvider_GetThumbnail中创建用作缩略图的位图，然后赋值给phbmp；如果位图有Alpha通道，则设定pdwAlpha为WTSAT_ARGB
'
'***********************************************************************************************************************
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CMyThumbnailProvier"

'--------------------------------------------------------------------
'以下为Windows Vista及以上版本使用的接口及变量
'--------------------------------------------------------------------
'实现IInitializeWithStream接口
Implements IInitializeWithStream
'实现IThumbnailProvider接口
Implements IThumbnailProvider

'文件IStream对象
Private m_pStream As IStream

'--------------------------------------------------------------------
'以下为Windows XP及以下版本使用的接口及变量
'--------------------------------------------------------------------
'实现IPersistFile接口
Implements IPersistFile
'实现IExtractImage接口
Implements IExtractImage

'文件名
Private m_sFileName As String
'文件内容
Private m_sFileContents As String

'--------------------------------------------------------------------
'以下为共用变量
'--------------------------------------------------------------------
'返回的缩略图位图句柄
Private m_hBmp As Long

'读取文件内容
Private Function LoadFileContent() As String
    Dim lFreeFile As Long
    lFreeFile = FreeFile()
    
    m_sFileContents = ""
    Open m_sFileName For Input As #lFreeFile
        Dim sTmpLine As String
        Do While Not EOF(lFreeFile)
            Input #lFreeFile, sTmpLine
            m_sFileContents = m_sFileContents & sTmpLine
        Loop
    Close #lFreeFile
End Function


'从字符串指针得到字符串
Private Function StrFromPtr(ByVal lpString As Long, Optional fUnicode As Boolean = True) As String
    On Error Resume Next
    
    If lpString = 0 Then Exit Function
    
    If fUnicode Then
        StrFromPtr = String(lstrlenW(lpString), Chr(0))
        lstrcpyW ByVal StrPtr(StrFromPtr), ByVal lpString
    Else
        StrFromPtr = String(lstrlenA(lpString), Chr(0))
        lstrcpyA ByVal StrFromPtr, ByVal lpString
    End If
End Function

'类销毁
Private Sub Class_Terminate()
    '释放IStream对象
    If Not m_pStream Is Nothing Then Set m_pStream = Nothing
    
    '释放位图对象
    If m_hBmp <> 0 Then DeleteObject m_hBmp
End Sub

'--------------------------------------------------------------------
'以下为Windows Vista及以上版本使用的接口
'--------------------------------------------------------------------
'系统在初始化缩略图对象时先调用此方法，以传入对应文件的IStream对象
Private Sub IInitializeWithStream_Initialize(ByVal pstream As ThumbnailProvider.IStream, ByVal grfMode As Long)
    If pstream Is Nothing Then
        Err.Raise E_NOTIMPL
    Else
        Set m_pStream = pstream
    End If
End Sub

'系统调用此方法以获得相应的缩略图
Private Sub IThumbnailProvider_GetThumbnail(ByVal cx As Long, phbmp As Long, pdwAlpha As WTS_ALPHATYPE)
    Dim sFileName As String
    Dim st As STATSTG
    
    '取得IStream对象的信息
    m_pStream.Stat st
    
    '获取IStream对象的名称信息
    If st.pwcsName <> 0 Then
        sFileName = StrFromPtr(st.pwcsName, True)
        CoTaskMemFree st.pwcsName
    End If
    
    '获得流对象中的字节数据大小
    Dim lFileSize As Long
    'lFileSize = IntegerFromCurrency(st.cbSize)
    lFileSize = st.cbSize.LowPart
    
    If lFileSize <= 0 Then Exit Sub
    
    Dim sBuffer As String
    Dim abBuffer() As Byte
    ReDim abBuffer(lFileSize - 1)
    
    '读取所有数据
    If m_pStream.Read(abBuffer(0), lFileSize) <= 0 Then Exit Sub
    
    sBuffer = StrConv(abBuffer, vbUnicode)
    
    '创建缩略图
    m_hBmp = CreateThumbnail(sBuffer)
    
    phbmp = m_hBmp
End Sub

'--------------------------------------------------------------------
'以下为Windows XP及以下版本使用的接口
'--------------------------------------------------------------------
'返回缩略图
Private Sub IExtractImage_Extract(phBmpThumbnail As Long)
    phBmpThumbnail = m_hBmp
End Sub

'返回文件名，优先度等信息；传入要显示的缩略图的尺寸及色深
Private Sub IExtractImage_GetLocation(ByVal pszPathBuffer As Long, ByVal cch As Long, pdwPriority As Long, prgSize As ThumbnailProvider.SIZE, ByVal dwRecClrDepth As Long, pdwFlags As ThumbnailProvider.IEIFlags)
    If pszPathBuffer <> 0 Then
        PathCreateFromUrl StrPtr(m_sFileName), pszPathBuffer, cch, 0
    End If
    
    pdwPriority = 0
    
    m_hBmp = CreateThumbnailWithSize(m_sFileContents, prgSize.cx, prgSize.cy, dwRecClrDepth)
    
    pdwFlags = IEIFLAG_CACHE
End Sub

Private Sub IPersistFile_GetClassID(pClassID As ThumbnailProvider.VBGUID)
    Err.Raise E_NOTIMPL
End Sub

Private Function IPersistFile_GetCurFile() As Long
    Err.Raise E_NOTIMPL
End Function

Private Sub IPersistFile_IsDirty()
    Err.Raise E_NOTIMPL
End Sub

'系统调用此方法传入目标对象名
Private Sub IPersistFile_Load(ByVal pszFileName As Long, ByVal dwMode As ThumbnailProvider.STGM)
    m_sFileName = StrFromPtr(pszFileName)
    
    Call LoadFileContent
End Sub

Private Sub IPersistFile_Save(ByVal pszFileName As Long, ByVal fRemember As Long)
    Err.Raise E_NOTIMPL
End Sub

Private Sub IPersistFile_SaveCompleted(ByVal pszFileName As Long)
    Err.Raise E_NOTIMPL
End Sub
