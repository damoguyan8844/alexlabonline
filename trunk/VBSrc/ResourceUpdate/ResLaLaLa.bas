Attribute VB_Name = "mduResLaLaLa"
'/**************************************************************************
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :mduResLaLaLa********************
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
Private Const THIS_MODULE_NAME As String = "mduResLaLaLa"

Public g_oResLib(0 To 1) As CResLib
Public g_oIconLib As CIconLib
Public g_fIsIconMode As Boolean

Public Const RT_CURSOR = 1&
Public Const RT_BITMAP = 2&
Public Const RT_ICON = 3&
Public Const RT_MENU = 4&
Public Const RT_DIALOG = 5&
Public Const RT_STRING = 6&
Public Const RT_FONTDIR = 7&
Public Const RT_FONT = 8&
Public Const RT_ACCELERATOR = 9&
Public Const RT_RCDATA = 10&
Public Const RT_MESSAGETABLE = 11&
Public Const DIFFERENCE = 11&
Public Const RT_GROUP_CURSOR = RT_CURSOR + DIFFERENCE
Public Const RT_GROUP_ICON = RT_ICON + DIFFERENCE
Public Const RT_VERSION = 16&
Public Const RT_DLGINCLUDE = 17&
Public Const RT_PLUGPLAY = 19&
Public Const RT_VXD = 20&
Public Const RT_ANICURSOR = 21&
Public Const RT_ANIICON = 22&
Public Const RT_HTML = 23&

Function FileExists(sFileName As String) As Boolean
    On Error Resume Next
    FileExists = (FileLen(sFileName) <> 0)
End Function
