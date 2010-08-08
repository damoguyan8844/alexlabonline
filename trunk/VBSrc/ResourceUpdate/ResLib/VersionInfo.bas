Attribute VB_Name = "mduVersionInfo"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :mduVersionInfo********************
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
Private Const THIS_MODULE_NAME As String = "mduVersionInfo"

Public Declare Function GetFileVersionInfoSize Lib "version.dll" Alias "GetFileVersionInfoSizeA" (ByVal lptstrFilename As String, lpdwHandle As Long) As Long
Public Declare Function GetFileVersionInfo Lib "version.dll" Alias "GetFileVersionInfoA" (ByVal lptstrFilename As String, ByVal dwHandle As Long, ByVal dwLen As Long, lpData As Any) As Long
Public Declare Function VerQueryValue Lib "version.dll" Alias "VerQueryValueA" (ByVal pBlock As Long, ByVal lpSubBlock As String, lplpBuffer As Long, puLen As Long) As Long

Public g_FileVersionInfoEntryNames(12) As String

Public Const FLAG_FVIENS_INITIALIZED As String = "999"

Public Enum eFileVersionInfoEntryNames
    efviComments = 0
    efviInternalName
    efviProductName
    efviCompanyName
    efviLegalCopyright
    efviProductVersion
    efviFileDescription
    efviLegalTrademarks
    efviPrivateBuild
    efviFileVersion
    efviOriginalFilename
    efviSpecialBuild
    efviInitializedFlag
End Enum

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)


Public Function GetVersionInfoFromResVerBytes(bVersionBlock() As Byte, ByVal lEntryName As eFileVersionInfoEntryNames) As String
    On Error GoTo Error_Handler
    Dim i As Long
    Dim lVersionSize As Long
    Dim pBlock() As Byte, SubBlock As String
    Dim lpTranslate As Long, bTranslate() As Byte
    Dim lSizeOflpTranslate As Long
    Dim lplpBuffer() As Byte, puLen As Long, lpBuffer As Long
    
    'lVersionSize = GetFileVersionInfoSize(sFileName, 0&)
    'If lVersionSize <= 0 Then Exit Function
    
    Call InitFileVersionInfoNames
    
    'ReDim pBlock(lVersionSize - 1)
    'Call GetFileVersionInfo(sFileName, 0&, lVersionSize, pBlock(0))
    pBlock = bVersionBlock
    
    VerQueryValue VarPtr(pBlock(0)), "\\VarFileInfo\\Translation", lpTranslate, lSizeOflpTranslate
    ReDim bTranslate(lSizeOflpTranslate - 1)
    CopyMemory bTranslate(0), ByVal lpTranslate, lSizeOflpTranslate
    
    For i = 1 To lSizeOflpTranslate / (UBound(bTranslate) + 1)
        SubBlock = "\\StringFileInfo\\"
        SubBlock = SubBlock & Byte2Hex(bTranslate(), 0, 1, True)
        SubBlock = SubBlock & Byte2Hex(bTranslate(), 2, 3, True)
        SubBlock = SubBlock & "\\" & g_FileVersionInfoEntryNames(lEntryName)
    
        VerQueryValue VarPtr(pBlock(0)), SubBlock, lpBuffer, puLen
        If lpBuffer <> 0 And puLen <> 0 Then
            ReDim lplpBuffer(puLen - 1)
            CopyMemory lplpBuffer(0), ByVal lpBuffer, puLen
            ReDim Preserve lplpBuffer(InStrB(lplpBuffer, ChrB(0)) - 2)
            GetVersionInfoFromResVerBytes = StrConv(lplpBuffer, vbUnicode)
        End If
    Next

    Exit Function
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


Public Function GetVersionInfo(ByVal sFileName As String, ByVal lEntryName As eFileVersionInfoEntryNames) As String
    On Error GoTo Error_Handler
    Dim i As Long
    Dim lVersionSize As Long
    Dim pBlock() As Byte, SubBlock As String
    Dim lpTranslate As Long, bTranslate() As Byte
    Dim lSizeOflpTranslate As Long
    Dim lplpBuffer() As Byte, puLen As Long, lpBuffer As Long
    
    lVersionSize = GetFileVersionInfoSize(sFileName, 0&)
    If lVersionSize <= 0 Then Exit Function
    
    Call InitFileVersionInfoNames
    
    ReDim pBlock(lVersionSize - 1)
    Call GetFileVersionInfo(sFileName, 0&, lVersionSize, pBlock(0))
    
    VerQueryValue VarPtr(pBlock(0)), "\\VarFileInfo\\Translation", lpTranslate, lSizeOflpTranslate
    ReDim bTranslate(lSizeOflpTranslate - 1)
    CopyMemory bTranslate(0), ByVal lpTranslate, lSizeOflpTranslate
    
    For i = 1 To lSizeOflpTranslate / (UBound(bTranslate) + 1)
        SubBlock = "\\StringFileInfo\\"
        SubBlock = SubBlock & Byte2Hex(bTranslate(), 0, 1, True)
        SubBlock = SubBlock & Byte2Hex(bTranslate(), 2, 3, True)
        SubBlock = SubBlock & "\\" & g_FileVersionInfoEntryNames(lEntryName)
    
        VerQueryValue VarPtr(pBlock(0)), SubBlock, lpBuffer, puLen
        If lpBuffer <> 0 And puLen <> 0 Then
            ReDim lplpBuffer(puLen - 1)
            CopyMemory lplpBuffer(0), ByVal lpBuffer, puLen
            ReDim Preserve lplpBuffer(InStrB(lplpBuffer, ChrB(0)) - 2)
            GetVersionInfo = StrConv(lplpBuffer, vbUnicode)
        End If
    Next

    Exit Function
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

Private Function Byte2Hex(bArray() As Byte, Optional ByVal lStart As Long = 0, Optional ByVal lEnd As Long = -1, Optional fReversed As Boolean = False) As String
    Dim i As Long
    lStart = IIf(lStart < 0, 0, lStart)
    lEnd = IIf(lEnd < 0, UBound(bArray), lEnd)
    
    If fReversed Then
        For i = lEnd To lStart Step -1
            Byte2Hex = Byte2Hex & Right$("00" & Hex(bArray(i)), 2)
        Next
    Else
        For i = lStart To lEnd
            Byte2Hex = Byte2Hex & Right$("00" & Hex(bArray(i)), 2)
        Next
    End If
End Function

Public Sub InitFileVersionInfoNames()
    If g_FileVersionInfoEntryNames(12) = FLAG_FVIENS_INITIALIZED Then Exit Sub
    g_FileVersionInfoEntryNames(efviComments) = "Comments"  '注释
    g_FileVersionInfoEntryNames(efviCompanyName) = "CompanyName"  '公司名
    g_FileVersionInfoEntryNames(efviProductName) = "ProductName"  '产品名
    g_FileVersionInfoEntryNames(efviProductVersion) = "ProductVersion"  '产品版本
    g_FileVersionInfoEntryNames(efviInternalName) = "InternalName"  '内部名称
    g_FileVersionInfoEntryNames(efviFileDescription) = "FileDescription"  '文件描述
    g_FileVersionInfoEntryNames(efviFileVersion) = "FileVersion"  '文件版本
    g_FileVersionInfoEntryNames(efviOriginalFilename) = "OriginalFilename"  '原始文件名
    g_FileVersionInfoEntryNames(efviSpecialBuild) = "SpecialBuild"  '特殊编译号
    g_FileVersionInfoEntryNames(efviPrivateBuild) = "PrivateBuild"  '私有编译号
    g_FileVersionInfoEntryNames(efviLegalCopyright) = "LegalCopyright"  '合法版权
    g_FileVersionInfoEntryNames(efviLegalTrademarks) = "LegalTrademarks"  '合法商标
    g_FileVersionInfoEntryNames(efviInitializedFlag) = FLAG_FVIENS_INITIALIZED   '是否已经初始化标记
End Sub



