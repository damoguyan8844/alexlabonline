Attribute VB_Name = "mduIconPackage"
'/**************************************************************************
'                 ��Ȩ���� (c), 2000 - 2010, ��Ե
'
' ************************   ģ �� ��   :mduIconPackage********************
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
Private Const THIS_MODULE_NAME As String = "mduIconPackage"

Public Type ICONDIR
    idReserved As Integer
    idType As Integer
    idCount As Integer
End Type

Public Type ICONDIRENTRY
    bWidth As Byte
    bHeight As Byte
    bColorCount As Byte
    bReserved As Byte
    wPlanes As Integer
    wBitCount As Integer
    dwBytesInRes As Long
    dwImageOffset As Long
End Type





