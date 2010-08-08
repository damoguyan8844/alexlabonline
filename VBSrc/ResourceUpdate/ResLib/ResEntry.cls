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
Attribute VB_Description = "资源条目"
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CResEntry********************
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
Private Const THIS_MODULE_NAME As String = "CResEntry"

'资源名（字符形式）
Private m_sResNameStr As String
'资源名（数值形式）
Private m_lResNameLng As Long
'资源语言描述
Private m_sLangStr As String
'资源语言（数值形式）
Private m_iLangInt As Long
'资源类型描述
Private m_sResTypeStr As String
'资源类型（数值形式）
Private m_lResTypeLng As Long
'资源大小
Private m_lResSize As Long
'资源ID
Private m_lResID As Long
'
Private m_sFilename As String
'/**********************************属 性 名  : Filename*********************/
'
'功能描述:
'
'/**************************************************************************/
Friend Property Let Filename(sFileName As String)
    m_sFilename = sFileName
End Property

Public Property Get Filename() As String
     Filename = m_sFilename
End Property




'/**********************************属 性 名  : ItemKey*********************/
'
'功能描述:
'
'/**************************************************************************/
Public Property Get ItemKey() As String
     ItemKey = "K:" & CStr(m_lResTypeLng) & "!" & CStr(m_lResNameLng) & "!" & CStr(m_lResID)
End Property

'/**********************************属 性 名  : ResSize*********************/
'
'功能描述:资源大小
'
'/**************************************************************************/
Friend Property Let ResSize(lResSize As Long)
    m_lResSize = lResSize
End Property

Public Property Get ResSize() As Long
     ResSize = m_lResSize
End Property

'/**********************************属 性 名  : ResID*********************/
'
'功能描述:资源大小
'
'/**************************************************************************/
Friend Property Let ResID(lResID As Long)
    m_lResID = lResID
End Property

Public Property Get ResID() As Long
     ResID = m_lResID
End Property



'/**********************************属 性 名  : LangStr*********************/
'
'功能描述:资源语言（字符形式）
'
'/**************************************************************************/
Public Property Get LangStr() As String
     LangStr = m_sLangStr
End Property


'/**********************************属 性 名  : ResNameStr*********************/
'
'功能描述: 资源名（字符形式）
'
'/**************************************************************************/
Public Property Get ResNameStr() As String
     ResNameStr = m_sResNameStr
End Property


'/**********************************属 性 名  : ResTypeStr*********************/
'
'功能描述:资源语言（字符形式）
'
'/**************************************************************************/
Public Property Get ResTypeStr() As String
     ResTypeStr = m_sResTypeStr
End Property


'/**********************************属 性 名  : ResNameLng*********************/
'
'功能描述: 资源名（数值形式）
'
'/**************************************************************************/
Friend Property Let ResNameLng(lResNameLng As Long)
    m_lResNameLng = lResNameLng
    m_sResNameStr = GetResNameStr(m_lResNameLng)
End Property

Public Property Get ResNameLng() As Long
     ResNameLng = m_lResNameLng
End Property


'/**********************************属 性 名  : LangInt*********************/
'
'功能描述: 资源语言（数值形式）
'
'/**************************************************************************/
Friend Property Let LangInt(iLangInt As Long)
    m_iLangInt = iLangInt
    m_sLangStr = GetLangStr(LangInt)
End Property

Public Property Get LangInt() As Long
     LangInt = m_iLangInt
End Property



'/**********************************属 性 名  : ResTypeLng*********************/
'
'功能描述: 资源类型（数值形式）
'
'/**************************************************************************/
Friend Property Let ResTypeLng(lResTypeLng As Long)
    m_lResTypeLng = lResTypeLng
    m_sResTypeStr = GetResTypeStr(m_lResTypeLng)
End Property

Public Property Get ResTypeLng() As Long
     ResTypeLng = m_lResTypeLng
End Property

'/**********************************函 数 名  : OpGetString*********************/
'功能描述: LoadString...
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError 99999, "文件" & m_sFilename & "不存在", THIS_MODULE_NAME
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
        RaiseError 999999, "资源查找错误", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
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
    '自定义错误处理
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**********************************函 数 名  : OpDrawIcon*********************/
'功能描述: LoadIcon...
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError 99999, "文件" & m_sFilename & "不存在", THIS_MODULE_NAME
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
        RaiseError 999999, "资源查找错误", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
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
            RaiseError 999999, "资源查找错误", THIS_MODULE_NAME
        End If
    
        hResData = LoadResource(hMod, hResInfo)
    
        If hResData = 0 Then
            RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
        End If
    
        hRes = LockResource(hResData)
    
        If hRes = 0 Then
            RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
        End If
        
    End If
    'ShowMeTheByte hRes, m_lResSize
    hIcon = CreateIconFromResource(hRes, m_lResSize, IIf(fIsIcon, 1&, 0&), &H30000)
    
    If hIcon = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If
    
    lRet = DrawIconEx(lhDestDC, 0, 0, hIcon, 0&, 0&, 0&, 0&, DI_DEFAULTSIZE Or DI_NORMAL)
    
    DestroyIcon hIcon
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpDrawIcon = (lRet <> 0)
    Exit Function
Error_Handler:
    '自定义错误处理
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**************************************************************************/
'功能描述: Show Dialog Box...
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError 99999, "文件" & m_sFilename & "不存在", THIS_MODULE_NAME
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
        RaiseError 999999, "资源查找错误", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
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
    '自定义错误处理
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**************************************************************************/
'功能描述: Get Version Info......
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError vbObjectError + 9999, "文件" & m_sFilename & "不存在", THIS_MODULE_NAME
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
        RaiseError vbObjectError + 99999, "资源查找错误", THIS_MODULE_NAME
    End If

    hResData = LoadResource(hMod, hResInfo)

    If hResData = 0 Then
        RaiseError vbObjectError + 9999, "资源查找错误", THIS_MODULE_NAME
    End If

    hRes = LockResource(hResData)

    If hRes = 0 Then
        RaiseError vbObjectError + 9999, "资源查找错误", THIS_MODULE_NAME
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
    '自定义错误处理
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
'/**********************************函 数 名  : OpDrawBitmap*********************/
'功能描述: LoadString...
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError 99999, "文件" & m_sFilename & "不存在", THIS_MODULE_NAME
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
        RaiseError 99999, "资源读取错误", THIS_MODULE_NAME
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
    '自定义错误处理
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
'/**********************************函 数 名  : OpDelRes*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
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
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    FreeLibrary hMod
    
    OpDelRes = True
    Exit Function
Error_Handler:
    '自定义错误处理
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
'/**********************************函 数 名  : OpReplaceResUsingRawBytes*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Boolean
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
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If
    
    hUpdateRes = BeginUpdateResource(m_sFilename, 0&)
    
    If hUpdateRes = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
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
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    OpReplaceResUsingRawBytes = True
    Exit Function
Error_Handler:
    '自定义错误处理
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**********************************函 数 名  : OpReplaceRes*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Boolean
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
            RaiseError 99999, "文件" & .Filename & "不存在", THIS_MODULE_NAME
        End If
               
        hMod = LoadLibrary(.Filename)
        
        If hMod = 0 Then
            RaiseError 99999, "文件" & .Filename & "不存在", THIS_MODULE_NAME
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
            RaiseError 999999, "资源查找错误", THIS_MODULE_NAME
        End If
    
        hResData = LoadResource(hMod, hResInfo)
    
        If hResData = 0 Then
            RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
        End If
    
        hRes = LockResource(hResData)
    
        If hRes = 0 Then
            RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
        End If
    End With
    
    Dim hUpdateRes As Long
    
    hUpdateRes = BeginUpdateResource(m_sFilename, 0&)
    If hUpdateRes = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
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
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpReplaceRes = True
    Exit Function
Error_Handler:
    '自定义错误处理
    If hResData <> 0 Then
        FreeResource hResData
    End If
    
    If hMod <> 0 Then
        FreeLibrary hMod
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


'/**********************************函 数 名  : IsDifferent*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Boolean
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
    '自定义错误处理
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


