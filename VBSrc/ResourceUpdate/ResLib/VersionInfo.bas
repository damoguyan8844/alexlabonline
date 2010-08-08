Attribute VB_Name = "mduVersionInfo"
'/**************************************************************************
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :mduVersionInfo********************
'�� �� ��: V1.0
'��    ��: �����̶�
'��������: 2004��03��07��
'����޸�:
'��������:
'�����б�:
'�޸���ʷ:
'��    ��: 2004��03��07��
'�޸���Ա: �����̶�
'�޸�����: ����
'******************************************************************************/
'
Option Explicit
'��ģ������
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
    '�Զ��������

    '����Ĭ�ϴ�������
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
    '�Զ��������

    '����Ĭ�ϴ�������
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
    g_FileVersionInfoEntryNames(efviComments) = "Comments"  'ע��
    g_FileVersionInfoEntryNames(efviCompanyName) = "CompanyName"  '��˾��
    g_FileVersionInfoEntryNames(efviProductName) = "ProductName"  '��Ʒ��
    g_FileVersionInfoEntryNames(efviProductVersion) = "ProductVersion"  '��Ʒ�汾
    g_FileVersionInfoEntryNames(efviInternalName) = "InternalName"  '�ڲ�����
    g_FileVersionInfoEntryNames(efviFileDescription) = "FileDescription"  '�ļ�����
    g_FileVersionInfoEntryNames(efviFileVersion) = "FileVersion"  '�ļ��汾
    g_FileVersionInfoEntryNames(efviOriginalFilename) = "OriginalFilename"  'ԭʼ�ļ���
    g_FileVersionInfoEntryNames(efviSpecialBuild) = "SpecialBuild"  '��������
    g_FileVersionInfoEntryNames(efviPrivateBuild) = "PrivateBuild"  '˽�б����
    g_FileVersionInfoEntryNames(efviLegalCopyright) = "LegalCopyright"  '�Ϸ���Ȩ
    g_FileVersionInfoEntryNames(efviLegalTrademarks) = "LegalTrademarks"  '�Ϸ��̱�
    g_FileVersionInfoEntryNames(efviInitializedFlag) = FLAG_FVIENS_INITIALIZED   '�Ƿ��Ѿ���ʼ�����
End Sub



