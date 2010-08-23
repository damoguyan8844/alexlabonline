VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CResEntry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "��Դ��Ŀ"
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'/**************************************************************************
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :CResEntry********************
'�� �� ��: V1.0
'��    ��: ��Ե
'��������: 2004��03��07��
'����޸�:
'��������:
'�����б�:
'�޸���ʷ:
'��    ��: 2004��03��07��
'�޸���Ա: ��Ե
'�޸�����: ����
'******************************************************************************/
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CResEntry"

'��Դ�����ַ���ʽ��
Private m_sResNameStr As String
'��Դ������ֵ��ʽ��
Private m_lResNameLng As Long
'��Դ��������
Private m_sLangStr As String
'��Դ���ԣ���ֵ��ʽ��
Private m_iLangInt As Long
'��Դ��������
Private m_sResTypeStr As String
'��Դ���ͣ���ֵ��ʽ��
Private m_lResTypeLng As Long
'��Դ��С
Private m_lResSize As Long
'��ԴID
Private m_lResID As Long
'
Private m_sFilename As String
'/**********************************�� �� ��  : Filename*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let Filename(sFileName As String)
    m_sFilename = sFileName
End Property

Public Property Get Filename() As String
     Filename = m_sFilename
End Property




'/**********************************�� �� ��  : ItemKey*********************/
'
'��������:
'
'/**************************************************************************/
Public Property Get ItemKey() As String
     ItemKey = "K:" & CStr(m_lResTypeLng) & "!" & CStr(m_lResNameLng) & "!" & CStr(m_lResID)
End Property

'/**********************************�� �� ��  : ResSize*********************/
'
'��������:��Դ��С
'
'/**************************************************************************/
Friend Property Let ResSize(lResSize As Long)
    m_lResSize = lResSize
End Property

Public Property Get ResSize() As Long
     ResSize = m_lResSize
End Property

'/**********************************�� �� ��  : ResID*********************/
'
'��������:��Դ��С
'
'/**************************************************************************/
Friend Property Let ResID(lResID As Long)
    m_lResID = lResID
End Property

Public Property Get ResID() As Long
     ResID = m_lResID
End Property



'/**********************************�� �� ��  : LangStr*********************/
'
'��������:��Դ���ԣ��ַ���ʽ��
'
'/**************************************************************************/
Public Property Get LangStr() As String
     LangStr = m_sLangStr
End Property


'/**********************************�� �� ��  : ResNameStr*********************/
'
'��������: ��Դ�����ַ���ʽ��
'
'/**************************************************************************/
Public Property Get ResNameStr() As String
     ResNameStr = m_sResNameStr
End Property


'/**********************************�� �� ��  : ResTypeStr*********************/
'
'��������:��Դ���ԣ��ַ���ʽ��
'
'/**************************************************************************/
Public Property Get ResTypeStr() As String
     ResTypeStr = m_sResTypeStr
End Property


'/**********************************�� �� ��  : ResNameLng*********************/
'
'��������: ��Դ������ֵ��ʽ��
'
'/**************************************************************************/
Friend Property Let ResNameLng(lResNameLng As Long)
    m_lResNameLng = lResNameLng
    m_sResNameStr = GetResNameStr(m_lResNameLng)
End Property

Public Property Get ResNameLng() As Long
     ResNameLng = m_lResNameLng
End Property


'/**********************************�� �� ��  : LangInt*********************/
'
'��������: ��Դ���ԣ���ֵ��ʽ��
'
'/**************************************************************************/
Friend Property Let LangInt(iLangInt As Long)
    m_iLangInt = iLangInt
    m_sLangStr = GetLangStr(LangInt)
End Property

Public Property Get LangInt() As Long
     LangInt = m_iLangInt
End Property



'/**********************************�� �� ��  : ResTypeLng*********************/
'
'��������: ��Դ���ͣ���ֵ��ʽ��
'
'/**************************************************************************/
Friend Property Let ResTypeLng(lResTypeLng As Long)
    m_lResTypeLng = lResTypeLng
    m_sResTypeStr = GetResTypeStr(m_lResTypeLng)
End Property

Public Property Get ResTypeLng() As Long
     ResTypeLng = m_lResTypeLng
End Property

'/**********************************�� �� ��  : OpGetString*********************/
'��������: LoadString...
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpGetString(sBuffer As String) As Boolean
    On Error GoTo Error_Handler
    
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    
    hMod = LoadLibrary(m_sFilename)
    
    If hMod = 0 Then
        RaiseError 99999, "�ļ�" & m_sFilename & "������", THIS_MODULE_NAME
    End If
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If
    
    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
    
    hResInfo = FindResourceEx(hMod, lpResType, lpResName, m_iLangInt)

    If hResInfo = 0 Then
        RaiseError 999999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    Dim b() As Byte
    ReDim b(m_lResSize - 1)
    CopyMemory ByVal VarPtr(b(0)), ByVal hRes, m_lResSize
    sBuffer = StringsFromTable(b)
    lRet = 1
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpGetString = (lRet <> 0)
    Exit Function
Error_Handler:
    '�Զ��������
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**********************************�� �� ��  : OpDrawIcon*********************/
'��������: LoadIcon...
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpDrawIcon(lhDestDC As Long, Optional fIsIcon As Boolean = True, Optional fIsGroup As Boolean = False) As Boolean
    On Error GoTo Error_Handler
    
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    Dim hIcon As Long
    
    hMod = LoadLibrary(m_sFilename)
    
    If hMod = 0 Then
        RaiseError 99999, "�ļ�" & m_sFilename & "������", THIS_MODULE_NAME
    End If
    
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If
    
    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
    
    hResInfo = FindResourceEx(hMod, lpResType, lpResName, m_iLangInt)

    If hResInfo = 0 Then
        RaiseError 999999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    If fIsGroup Then
        
        If fIsIcon Then
            lpResType = RT_ICON
        Else
            lpResType = RT_CURSOR
        End If
        
        lpResName = LookupIconIdFromDirectory(hRes, IIf(fIsIcon, 1, 0))
        
        hResInfo = FindResourceEx(hMod, lpResType, lpResName, m_iLangInt)
    
        If hResInfo = 0 Then
            RaiseError 999999, "��Դ���Ҵ���", THIS_MODULE_NAME
        End If
    
        hResData = LoadResource(hMod, hResInfo)
    
        If hResData = 0 Then
            RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
        End If
    
        hRes = LockResource(hResData)
    
        If hRes = 0 Then
            RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
        End If
        
    End If
    'ShowMeTheByte hRes, m_lResSize
    hIcon = CreateIconFromResource(hRes, m_lResSize, IIf(fIsIcon, 1&, 0&), &H30000)
    
    If hIcon = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    lRet = DrawIconEx(lhDestDC, 0, 0, hIcon, 0&, 0&, 0&, 0&, DI_DEFAULTSIZE Or DI_NORMAL)
    
    DestroyIcon hIcon
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpDrawIcon = (lRet <> 0)
    Exit Function
Error_Handler:
    '�Զ��������
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**************************************************************************/
'��������: Show Dialog Box...
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpShowDialog(ByVal hParent As Long) As Boolean
    On Error GoTo Error_Handler
    
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    Dim hIcon As Long
    
    hMod = LoadLibrary(m_sFilename)
    
    If hMod = 0 Then
        RaiseError 99999, "�ļ�" & m_sFilename & "������", THIS_MODULE_NAME
    End If
    
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If
    
    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
    
    hResInfo = FindResourceEx(hMod, lpResType, lpResName, m_iLangInt)

    If hResInfo = 0 Then
        RaiseError 999999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    Dim asUndefinedClasses() As String
    AnylyzDialogRes hRes, asUndefinedClasses()
    
    Call RegClasses(asUndefinedClasses())
    
    Dim hDialog As Long
    hDialog = CreateDialogIndirectParam(App.hInstance, hRes, hParent, AddressOf DialogProc, 0&)

    If hDialog <> 0 Then
        ShowWindow hDialog, vbNormalFocus
    End If
    
    Call UnRegClasses(asUndefinedClasses())
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpShowDialog = (lRet <> 0)
    Exit Function
Error_Handler:
    '�Զ��������
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**************************************************************************/
'��������: Get Version Info......
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpGetVersion(sBuffer As String) As Boolean
    On Error GoTo Error_Handler
    
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    
    hMod = LoadLibrary(m_sFilename)
    
    If hMod = 0 Then
        RaiseError vbObjectError + 9999, "�ļ�" & m_sFilename & "������", THIS_MODULE_NAME
    End If
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If
    
    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
    
    hResInfo = FindResourceEx(hMod, lpResType, lpResName, m_iLangInt)

    If hResInfo = 0 Then
        RaiseError vbObjectError + 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError vbObjectError + 9999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError vbObjectError + 9999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    Dim b() As Byte
    ReDim b(m_lResSize - 1)
    CopyMemory ByVal VarPtr(b(0)), ByVal hRes, m_lResSize
    
    sBuffer = VersionInfoFromRes(b())
    
    lRet = 1
    FreeResource hResData
    FreeLibrary hMod
    
    OpGetVersion = (lRet <> 0)
    Exit Function
Error_Handler:
    '�Զ��������
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
'/**********************************�� �� ��  : OpDrawBitmap*********************/
'��������: LoadString...
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpDrawBitmap(lhDestDC As Long, ByVal lWidth As Long, ByVal lHeight As Long) As Boolean
    On Error GoTo Error_Handler
    
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    Dim hBitmap As Long, lhDCTmp As Long
    
    hMod = LoadLibrary(m_sFilename)
    
    If hMod = 0 Then
        RaiseError 99999, "�ļ�" & m_sFilename & "������", THIS_MODULE_NAME
    End If
    
    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
    
    hBitmap = LoadBitmap(hMod, lpResName)
    
    If hBitmap = 0 Then
        RaiseError 99999, "��Դ��ȡ����", THIS_MODULE_NAME
    End If
    
    lhDCTmp = CreateCompatibleDC(lhDestDC)
    
    If lhDCTmp <> 0 Then
        Dim bi As BITMAP
        SelectObject lhDCTmp, hBitmap
        Call GetObject(hBitmap, Len(bi), bi)
        Dim ldestH As Long, ldestW As Long
        With bi
            ldestW = IIf(.bmWidth > lWidth, lWidth, .bmWidth)
            ldestH = IIf(.bmHeight > lHeight, lHeight, .bmHeight)
        End With
        lRet = StretchBlt(lhDestDC, 0, 0, ldestW, ldestH, lhDCTmp, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY)
        DeleteObject hBitmap
        DeleteObject lhDCTmp
    End If
    
    FreeLibrary hMod
    
    OpDrawBitmap = (lRet <> 0)
    Exit Function
Error_Handler:
    '�Զ��������
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
'/**********************************�� �� ��  : OpDelRes*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpDelRes() As Boolean
    On Error GoTo Error_Handler
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    
  
    Dim hUpdateRes As Long
    
    hUpdateRes = BeginUpdateResource(m_sFilename, 0&)
    If hUpdateRes = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If

    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
        
    If UpdateResource(hUpdateRes, lpResType, lpResName, m_iLangInt, 0&, 0&) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    FreeLibrary hMod
    
    OpDelRes = True
    Exit Function
Error_Handler:
    '�Զ��������
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
'/**********************************�� �� ��  : OpReplaceResUsingRawBytes*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpReplaceResUsingRawBytes(oIconEntry As CIconEntry) As Boolean
    On Error GoTo Error_Handler
    Dim lRet As Long
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    Dim bRawBytes() As Byte
    Dim hUpdateRes As Long
    
    bRawBytes = oIconEntry.GetRawBytes
    
    If SizeOfArray(bRawBytes) <= 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    hUpdateRes = BeginUpdateResource(m_sFilename, 0&)
    
    If hUpdateRes = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If

    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
        
    If UpdateResource(hUpdateRes, lpResType, lpResName, m_iLangInt, VarPtr(bRawBytes(0)), SizeOfArray(bRawBytes)) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    OpReplaceResUsingRawBytes = True
    Exit Function
Error_Handler:
    '�Զ��������
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**********************************�� �� ��  : OpReplaceRes*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpReplaceRes(oResEntry As CResEntry) As Boolean
    On Error GoTo Error_Handler
    Dim hResInfo As Long
    Dim hResData As Long
    Dim hRes As Long
    Dim lRet As Long
    Dim hMod As Long
    Dim sTmp As String
    Dim lpResType As Long, lpResName As Long
    Dim bResType() As Byte, bResName() As Byte
    
    With oResEntry
        
        If oResEntry.ResTypeLng <> m_lResTypeLng Then
            RaiseError 99999, "�ļ�" & .Filename & "������", THIS_MODULE_NAME
        End If
               
        hMod = LoadLibrary(.Filename)
        
        If hMod = 0 Then
            RaiseError 99999, "�ļ�" & .Filename & "������", THIS_MODULE_NAME
        End If
        
        If (.ResTypeLng And &HFFFF0000) = 0 Then
            lpResType = .ResTypeLng
        Else
            'lpResType = StrPtr(m_sResTypeStr)
            bResType = StrConv(.ResTypeStr & Chr(0), vbFromUnicode)
            lpResType = VarPtr(bResType(0))
        End If
        
        If (.ResNameLng And &HFFFF0000) = 0 Then
            lpResName = .ResNameLng
        Else
            'lpResName = StrPtr(m_sResNameStr)
            bResName = StrConv(ResNameStr & Chr(0), vbFromUnicode)
            lpResName = VarPtr(bResName(0))
        End If
        
        hResInfo = FindResourceEx(hMod, lpResType, lpResName, .LangInt)
    
        If hResInfo = 0 Then
            RaiseError 999999, "��Դ���Ҵ���", THIS_MODULE_NAME
        End If
    
        hResData = LoadResource(hMod, hResInfo)
    
        If hResData = 0 Then
            RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
        End If
    
        hRes = LockResource(hResData)
    
        If hRes = 0 Then
            RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
        End If
    End With
    
    Dim hUpdateRes As Long
    
    hUpdateRes = BeginUpdateResource(m_sFilename, 0&)
    If hUpdateRes = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If (m_lResTypeLng And &HFFFF0000) = 0 Then
        lpResType = m_lResTypeLng
    Else
        'lpResType = StrPtr(m_sResTypeStr)
        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
        lpResType = VarPtr(bResType(0))
    End If

    If (m_lResNameLng And &HFFFF0000) = 0 Then
        lpResName = m_lResNameLng
    Else
        'lpResName = StrPtr(m_sResNameStr)
        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
        lpResName = VarPtr(bResName(0))
    End If
        
    If UpdateResource(hUpdateRes, lpResType, lpResName, m_iLangInt, hRes, oResEntry.ResSize) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpReplaceRes = True
    Exit Function
Error_Handler:
    '�Զ��������
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


'/**********************************�� �� ��  : IsDifferent*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function IsDifferent(oResEntry As CResEntry) As Boolean
    On Error GoTo Error_Handler
    With oResEntry
        IsDifferent = True
        
        If .ResTypeStr <> m_sResTypeStr Then
            Exit Function
        End If
    
        If .ResNameStr <> m_sResNameStr Then
            Exit Function
        End If
    
        If .LangStr <> m_sLangStr Then
            Exit Function
        End If
    
        If .ResSize <> m_lResSize Then
            Exit Function
        End If
    End With
    
    IsDifferent = False
    Exit Function
Error_Handler:
    '�Զ��������
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

