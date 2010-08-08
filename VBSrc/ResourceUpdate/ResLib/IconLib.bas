Attribute VB_Name = "mduIconLib"
'/**************************************************************************
'                 ��Ȩ���� (c), 2000 - 2010, ��Ե
'
' ************************   ģ �� ��   :mduIconLib********************
'�� �� ��: V1.0
'��    ��: ��Ե
'��������: 2004��03��10��
'����޸�:
'��������:
'�����б�:
'�޸���ʷ:
'��    ��: 2004��03��10��
'�޸���Ա: ��Ե
'�޸�����: ����
'******************************************************************************/
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "mduIconLib"

Dim m_oIconPackage As CIconPackage

Public Function CreateIconListWithObject(oIconPackage As CIconPackage) As Boolean
    On Error GoTo Error_Handler
    Dim lFreefile As Long
    Set m_oIconPackage = oIconPackage
    
    lFreefile = FreeFile
    Open m_oIconPackage.Filename For Binary As #lFreefile
    
    Dim utIconDir As ICONDIR
    Dim utIconEntry() As ICONDIRENTRY
    
    Get #lFreefile, , utIconDir
    
    If utIconDir.idReserved <> 0 Then
        RaiseError 999999, "not a icon/cursor file", THIS_MODULE_NAME
    End If
    
    If utIconDir.idType <> 1 And utIconDir.idType <> 2 Then
        RaiseError 999999, "not a icon/cursor file", THIS_MODULE_NAME
    End If
    
    If utIconDir.idCount <= 0 Then
        RaiseError 999999, "not a icon/cursor file", THIS_MODULE_NAME
    End If
        
    Dim oIconEntry As CIconEntry, i As Long
    ReDim utIconEntry(utIconDir.idCount - 1) As ICONDIRENTRY
    
    Get #lFreefile, , utIconEntry
    
    For i = 0 To UBound(utIconEntry)
        Set oIconEntry = m_oIconPackage.IconEntries.Add()
        With oIconEntry
            .IconSize = utIconEntry(i).dwBytesInRes
            .RawDataOffset = utIconEntry(i).dwImageOffset
            .IconHeight = utIconEntry(i).bHeight
            .IconWidth = utIconEntry(i).bWidth
            .IconType = utIconDir.idType
        End With
    Next
    
    Close #lFreefile
    CreateIconListWithObject = True
    Exit Function
Error_Handler:
    '�Զ��������
    Close #lFreefile
    
    '����Ĭ�ϴ�������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


Public Function SizeOfArray(b As Variant) As Long
    If IsArray(b) Then
        SizeOfArray = UBound(b) - LBound(b) + 1
    End If
End Function
