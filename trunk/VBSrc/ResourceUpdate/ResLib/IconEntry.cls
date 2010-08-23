VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIconEntry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :CIconEntry********************
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
Private Const THIS_MODULE_NAME As String = "CIconEntry"

'��С
Private m_lIconSize As Long
'
Private m_sFilename As String
'
Private m_sItemKey As String
'
Private m_lRawDataOffset As Long
'
Private m_lIconHeight As Long
'
Private m_lIconWidth As Long
'
Private m_sIconName As String
'
Private m_lOpDrawIcon As Boolean
'
Private m_lIconType As Long
'/**********************************�� �� ��  : IconType*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let IconType(lIconType As Long)
    m_lIconType = lIconType
End Property

Public Property Get IconType() As Long
     IconType = m_lIconType
End Property


'/**********************************�� �� ��  : IconName*********************/
'
'��������:
'
'/**************************************************************************/
Public Property Let IconName(sIconName As String)
    m_sIconName = sIconName
End Property

Public Property Get IconName() As String
     IconName = m_sIconName
End Property


'/**********************************�� �� ��  : IconWidth*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let IconWidth(lIconWidth As Long)
    m_lIconWidth = lIconWidth
End Property

Public Property Get IconWidth() As Long
     IconWidth = m_lIconWidth
End Property


'/**********************************�� �� ��  : IconHeight*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let IconHeight(lIconHeight As Long)
    m_lIconHeight = lIconHeight
End Property

Public Property Get IconHeight() As Long
     IconHeight = m_lIconHeight
End Property


'/**********************************�� �� ��  : RawDataOffset*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let RawDataOffset(lRawDataOffset As Long)
    m_lRawDataOffset = lRawDataOffset
End Property

Public Property Get RawDataOffset() As Long
     RawDataOffset = m_lRawDataOffset
End Property


'/**********************************�� �� ��  : ItemKey*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let ItemKey(sItemKey As String)
    m_sItemKey = sItemKey
End Property

Public Property Get ItemKey() As String
     ItemKey = "K:" & m_sIconName 'm_sItemKey
End Property



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



'/**********************************�� �� ��  : IconSize*********************/
'
'��������:��Դ��С
'
'/**************************************************************************/
Friend Property Let IconSize(lIconSize As Long)
    m_lIconSize = lIconSize
End Property

Public Property Get IconSize() As Long
     IconSize = m_lIconSize
End Property

'/**********************************�� �� ��  : GetRawBytes*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Byte()
'/**************************************************************************/
Public Function GetRawBytes() As Byte()
    On Error GoTo Error_Handler
    Dim b() As Byte
    Dim lFreefile As Long
    lFreefile = FreeFile
    
    Open m_sFilename For Binary As #lFreefile
    
    Seek #lFreefile, m_lRawDataOffset + 1
    ReDim b(m_lIconSize - 1) As Byte
    Get #lFreefile, , b
    
    Close #lFreefile
    GetRawBytes = b
    Exit Function
Error_Handler:
    '�Զ��������
    Close #lFreefile
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

'/**********************************�� �� ��  : OpDrawIcon*********************/
'��������:
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function OpDrawIcon(lhDestDC As Long) As Boolean
    On Error GoTo Error_Handler
    Dim hIcon As Long, lRet As Long
    Dim bRawBytes() As Byte
    
    bRawBytes = GetRawBytes()
    
    If SizeOfArray(bRawBytes) <= 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    hIcon = CreateIconFromResource(VarPtr(bRawBytes(0)), m_lIconSize, IIf(m_lIconType = 1, 1&, 0&), &H30000)
    
    If hIcon = 0 Then
        RaiseError 99999, "��Դ���Ҵ���", THIS_MODULE_NAME
    End If
    
    lRet = DrawIconEx(lhDestDC, 0, 0, hIcon, 0&, 0&, 0&, 0&, DI_DEFAULTSIZE Or DI_NORMAL)
    
    DestroyIcon hIcon
    
    Exit Function
Error_Handler:
    '�Զ��������
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function