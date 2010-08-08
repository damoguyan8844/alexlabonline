VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBBrowserHelperObject"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 ��Ȩ���� (c), 2000 - 2010, �����̶�
'
' ************************   ģ �� ��   :CVBBrowserHelperObject********************
'�� �� ��: V1.0
'��    ��: �����̶�
'��������: 2004��09��29��
'����޸�:
'��������:
'�����б�:
'�޸���ʷ:
'��    ��: 2004��09��29��
'�޸���Ա: �����̶�
'�޸�����: ����
'******************************************************************************/
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CVBBrowserHelperObject"

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Implements IObjectWithSite

Private m_oSite As IUnknownVB
Private WithEvents m_oBrowser As InternetExplorer
Attribute m_oBrowser.VB_VarHelpID = -1

Private Sub IObjectWithSite_GetSite(ByVal priid As Long, ppvObj As Long)
    m_oSite.QueryInterface priid, ppvObj
End Sub

Private Sub IObjectWithSite_SetSite(ByVal pSite As BHOLib.IUnknownVB)
    If ObjPtr(pSite) = 0 Then
        CopyMemory m_oBrowser, 0&, 4
        Exit Sub
    End If
    
    Set m_oSite = pSite
    Set m_oBrowser = pSite
End Sub

Private Sub m_oBrowser_BeforeNavigate2(ByVal pDisp As Object, URL As Variant, Flags As Variant, TargetFrameName As Variant, PostData As Variant, Headers As Variant, Cancel As Boolean)
    On Error Resume Next
    '����URL��ַ��ĩβ�ַ��ж��Ƿ�ΪҪ�����ļ�
    'FlashGet��NetAnts��Ӱ�����ʹ�������������ԭ�������ڴ�
    If IsDownloadingFile(URL) Then
        MsgBox "���������ļ� " & URL
    End If
    
    '����´�����û�й������Ͳ˵����ģ���ô����Ϊ�ǵ������ڣ��˳�
    With m_oBrowser
        If .ToolBar = False And .MenuBar = False Then
            .Quit
        End If
    End With
End Sub

Private Sub m_oBrowser_NewWindow2(ppDisp As Object, Cancel As Boolean)
    'ֱ��ȡ���κ��´��ڵĴ���
    'Cancel = True
End Sub


'����URL��ַ�ж��Ƿ�Ϊ�����ļ�
Private Function IsDownloadingFile(ByVal URL As String, Optional sFileType As String) As Boolean
    IsDownloadingFile = True
    
    If UCase(Right$(URL, 4)) = ".RAR" Then
        sFileType = ".RAR"
        Exit Function
    End If

    If UCase(Right$(URL, 4)) = ".ZIP" Then
        sFileType = ".ZIP"
        Exit Function
    End If

    If UCase(Right$(URL, 4)) = ".ISO" Then
        sFileType = ".ISO"
        Exit Function
    End If
    
    If UCase(Right$(URL, 4)) = ".WMV" Then
        sFileType = ".WMV"
        Exit Function
    End If

    IsDownloadingFile = False
End Function