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
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :CResPackage********************
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
Private Const THIS_MODULE_NAME As String = "CResPackage"


'�ֲ����������漯��
Private m_colCol As Collection
'
Private m_sFilename As String
'/**********************************�� �� ��  : Filename*********************/
'
'��������:
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


'/**********************************�� �� ��  : OpAddRes*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Boolean
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
            bResName = StrConv(.ResNameStr & Chr(0), vbFromUnicode)
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
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    If EndUpdateResource(hUpdateRes, 0&) = 0 Then
        RaiseError 99999, "��Դ�滻����", THIS_MODULE_NAME
    End If
    
    FreeResource hResData
    FreeLibrary hMod
    
    OpAddRes = True
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
