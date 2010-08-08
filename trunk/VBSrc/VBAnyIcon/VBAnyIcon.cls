VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBAnyIcon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 超级绿豆
'
' ************************   模 块 名   :CVBAnyIcon********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年06月04日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年06月04日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBAnyIcon"

Private Type ICONDIR
    idReserved As Integer
    idType As Integer
    idCount As Integer
End Type

Private Type ICONDIRENTRY
    bWidth As Byte
    bHeight As Byte
    bColorCount As Byte
    bReserved As Byte
    wPlanes As Integer
    wBitCount As Integer
    dwBytesInRes As Long
    dwImageOffset As Long
End Type

Private Declare Sub OleCreatePictureIndirect Lib "olepro32.dll" (pPictDesc As PICTDESC, riid As GUID, ByVal fOwn As Long, ppvObj As IPicture)
Private Declare Function CreateIconFromResource Lib "user32" (ByVal presbits As Long, ByVal dwResSize As Long, ByVal fIcon As Long, ByVal dwVer As Long) As Long
Private Declare Function ExtractIconEx Lib "shell32.dll" Alias "ExtractIconExA" (ByVal lpszFile As String, ByVal nIconIndex As Long, ByVal phiconLarge As Long, ByVal phiconSmall As Long, ByVal nIcons As Long) As Long
Private Declare Function DestroyIcon Lib "user32" (ByVal hIcon As Long) As Long

Private Type PICTDESC
    cbSizeOfStruct As Long
    picType As Long
    hIcon As Long
End Type

Private Type GUID
  Data1 As Long
  Data2 As Integer
  Data3 As Integer
  Data4(7) As Byte
End Type

Private Enum eFILETYPE
    eftIcon
    eftCursor
    eftExecutable
    eftUnkown
End Enum

Private m_udtIPictureIID As GUID
'
Private m_oPicture As IPicture
'
Private m_sFilename As String
'
Private m_lIconCount As Long
'
Private m_udtIconEntries() As ICONDIRENTRY
'
Private m_lFileType As eFILETYPE
'**************************************************************************
'*                                暴露的接口                              *
'**************************************************************************
Public Property Let Filename(sFilename As String)
    m_sFilename = sFilename
    Call GetFileType
    Call GetIcons
End Property

Public Property Get Filename() As String
     Filename = m_sFilename
End Property

Public Property Get Picture(ByVal lIconIndex As Long) As IPicture
    If m_lIconCount = 0 Or m_sFilename = "" Then Exit Property
    Set m_oPicture = Nothing
    Set m_oPicture = GetPicture(lIconIndex)
    Set Picture = m_oPicture
End Property

Public Property Get IconCount() As Long
    IconCount = m_lIconCount
End Property
'**************************************************************************
'*                                暴露的接口                              *
'**************************************************************************



'**************************************************************************
'******************************** 私有函数 ********************************
'**************************************************************************
Private Function GetPicture(ByVal lIconIndex As Long) As IPicture
    Dim hIcon As Long
    lIconIndex = lIconIndex - 1
    If lIconIndex > m_lIconCount - 1 Then lIconIndex = m_lIconCount - 1
    If lIconIndex < 0 Then lIconIndex = 0
    
    hIcon = GetSingleIcon(lIconIndex)
    If hIcon <> 0 Then
        Dim oPic As IPicture, udtPict As PICTDESC
        
        With udtPict
            .cbSizeOfStruct = Len(udtPict)
            .picType = vbPicTypeIcon
            .hIcon = hIcon
        End With
        
        OleCreatePictureIndirect udtPict, m_udtIPictureIID, 1, oPic
        
        Set GetPicture = oPic
        
        DestroyIcon hIcon
    End If
End Function

Private Function GetIcons() As Boolean
    On Error GoTo Error_Handler
    Select Case m_lFileType
    Case eftIcon, eftCursor
        Dim i As Long
        Dim lFreefile As Long
        
        lFreefile = FreeFile
        Open m_sFilename For Binary As #lFreefile
        
        Dim utIconDir As ICONDIR
        Dim utIconEntry() As ICONDIRENTRY
        
        Get #lFreefile, , utIconDir
        
        If utIconDir.idReserved <> 0 Then
            Err.Raise 999999, "not a icon/cursor file", THIS_MODULE_NAME
        End If
        
        If utIconDir.idType <> 1 And utIconDir.idType <> 2 Then
            Err.Raise 999999, "not a icon/cursor file", THIS_MODULE_NAME
        End If
        
        If utIconDir.idCount <= 0 Then
            Err.Raise 999999, "not a icon/cursor file", THIS_MODULE_NAME
        End If
            
        m_lIconCount = utIconDir.idCount
        
        ReDim m_udtIconEntries(m_lIconCount - 1) As ICONDIRENTRY
        
        Get #lFreefile, , m_udtIconEntries
        
        Close #lFreefile
    
        GetIcons = True
    Case eftExecutable
        m_lIconCount = ExtractIconEx(m_sFilename, -1, 0, 0, 0)
    Case Else
    End Select
    Exit Function
Error_Handler:
    Close #lFreefile
End Function

Private Function GetSingleIcon(ByVal lIconIndex As Long) As Long
    On Error GoTo Error_Handler
    Select Case m_lFileType
    Case eftIcon, eftCursor
        Dim udtIconEntry As ICONDIRENTRY
        Dim b() As Byte
        Dim lFreefile As Long
        lFreefile = FreeFile
        
        udtIconEntry = m_udtIconEntries(lIconIndex)
        
        Open m_sFilename For Binary As #lFreefile
        
        With udtIconEntry
            Seek #lFreefile, .dwImageOffset + 1
            ReDim b(.dwBytesInRes - 1) As Byte
            Get #lFreefile, , b
        
            Close #lFreefile
            
            If UBound(b) > 0 Then
                GetSingleIcon = CreateIconFromResource(VarPtr(b(0)), .dwBytesInRes, 1, &H30000)
            End If
        End With
    Case eftExecutable
        Call ExtractIconEx(m_sFilename, lIconIndex, VarPtr(GetSingleIcon), 0, 1)
    Case eftUnkown
    End Select
    Exit Function
Error_Handler:
    Close #lFreefile
End Function

Private Sub GetFileType()
    On Error Resume Next
    Dim sExt As String
    sExt = LCase(Right$(m_sFilename, 4))
    Select Case sExt
    Case ".ico"
        m_lFileType = eftIcon
    Case ".cur"
        m_lFileType = eftCursor
    Case ".dll", ".exe"
        m_lFileType = eftExecutable
    Case Else
        m_lFileType = eftUnkown
    End Select
End Sub

Private Sub Class_Initialize()
    With m_udtIPictureIID
        .Data1 = &H7BF80980
        .Data2 = &HBF32
        .Data3 = &H101A
        .Data4(0) = &H8B
        .Data4(1) = &HBB
        .Data4(2) = &H0
        .Data4(3) = &HAA
        .Data4(4) = &H0
        .Data4(5) = &H30
        .Data4(6) = &HC
        .Data4(7) = &HAB
    End With
End Sub

Private Sub Class_Terminate()
    Set m_oPicture = Nothing
End Sub
'**************************************************************************
'******************************** 私有函数 ********************************
'**************************************************************************
