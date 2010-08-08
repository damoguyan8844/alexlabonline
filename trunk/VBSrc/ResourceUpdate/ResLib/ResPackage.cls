VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CResPackage"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Member0" ,"CResEntries"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :CResPackage********************
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
Private Const THIS_MODULE_NAME As String = "CResPackage"


'局部变量，保存集合
Private m_colCol As Collection
'
Private m_sFilename As String
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

Public Property Get OneResEntries(vKey As Variant) As CResEntries
    On Error Resume Next
    Set OneResEntries = m_colCol(vKey)
End Property



Public Property Get Count() As Long
    Count = m_colCol.Count
End Property


Public Sub Remove(lResType As Long)
    m_colCol.Remove lResType
End Sub


Public Property Get NewEnum() As IUnknown
Attribute NewEnum.VB_UserMemId = -4
Attribute NewEnum.VB_MemberFlags = "40"
    Set NewEnum = m_colCol.[_NewEnum]
End Property


Private Sub Class_Initialize()
    Set m_colCol = New Collection
End Sub


Private Sub Class_Terminate()
    Set m_colCol = Nothing
End Sub

Public Function GetResEntries(ByVal lResType As Long) As CResEntries
    On Error Resume Next
    Dim sKey As String
    
    sKey = CStr(lResType)
    
    Set GetResEntries = m_colCol(sKey)
    If GetResEntries Is Nothing Then
        Dim oResEntries As New CResEntries
        oResEntries.Filename = m_sFilename
        oResEntries.ResTypeOfEntriesLng = lResType
        m_colCol.Add oResEntries, sKey
        Set GetResEntries = oResEntries
    End If
End Function


'/**********************************函 数 名  : OpAddRes*********************/
'功能描述:
'
'参    数:
'
'返 回 值: Boolean
'/**************************************************************************/
Public Function OpAddRes(oResEntry As CResEntry) As Boolean
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
            bResName = StrConv(.ResNameStr & Chr(0), vbFromUnicode)
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
    
'    If (m_lResTypeLng And &HFFFF0000) = 0 Then
'        lpResType = m_lResTypeLng
'    Else
'        'lpResType = StrPtr(m_sResTypeStr)
'        bResType = StrConv(m_sResTypeStr & Chr(0), vbFromUnicode)
'        lpResType = VarPtr(bResType(0))
'    End If
'
'    If (m_lResNameLng And &HFFFF0000) = 0 Then
'        lpResName = m_lResNameLng
'    Else
'        'lpResName = StrPtr(m_sResNameStr)
'        bResName = StrConv(m_sResNameStr & Chr(0), vbFromUnicode)
'        lpResName = VarPtr(bResName(0))
'    End If
        
    If UpdateResource(hUpdateRes, lpResType, lpResName, oResEntry.LangInt, hRes, oResEntry.ResSize) = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "资源替换错误", THIS_MODULE_NAME
    End If
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpAddRes = True
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

