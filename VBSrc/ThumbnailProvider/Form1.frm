VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "ThumbnailProviderע��/ע������"
   ClientHeight    =   6690
   ClientLeft      =   4770
   ClientTop       =   2550
   ClientWidth     =   5655
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6690
   ScaleWidth      =   5655
   Begin VB.Frame Frame5 
      Height          =   75
      Left            =   30
      TabIndex        =   19
      Top             =   6060
      Width           =   5595
   End
   Begin VB.Frame Frame4 
      Caption         =   "Ҫע����ļ���չ��"
      Height          =   735
      Left            =   270
      TabIndex        =   17
      Top             =   2640
      Width           =   5055
      Begin VB.TextBox txtFileExt 
         Height          =   345
         Left            =   2010
         TabIndex        =   18
         Text            =   ".myext"
         Top             =   240
         Width           =   825
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "ѡ��ThumbnailProvider�ļ���Ҫע�����"
      Height          =   2445
      Left            =   270
      TabIndex        =   12
      Top             =   90
      Width           =   5055
      Begin VB.TextBox txtFile 
         Height          =   345
         Left            =   150
         Locked          =   -1  'True
         TabIndex        =   15
         Text            =   "MyThumbnailProvier.dll"
         Top             =   360
         Width           =   4065
      End
      Begin VB.CommandButton cmdOpen 
         Caption         =   "���"
         Height          =   345
         Left            =   4290
         TabIndex        =   14
         Top             =   360
         Width           =   675
      End
      Begin VB.ListBox lstCoClasses 
         Height          =   1140
         Left            =   150
         TabIndex        =   13
         Top             =   1080
         Width           =   4755
      End
      Begin VB.Label Label3 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "�ļ��е��ࣺ"
         Height          =   180
         Left            =   150
         TabIndex        =   16
         Top             =   840
         Width           =   1080
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "����Ч��"
      Height          =   1125
      Left            =   270
      TabIndex        =   7
      Top             =   4560
      Width           =   5085
      Begin VB.OptionButton optAdornment 
         Caption         =   "���κ�Ч��"
         Height          =   285
         Index           =   0
         Left            =   420
         TabIndex        =   11
         Top             =   300
         Width           =   1275
      End
      Begin VB.OptionButton optAdornment 
         Caption         =   "ͶӰЧ��"
         Height          =   285
         Index           =   1
         Left            =   2760
         TabIndex        =   10
         Top             =   300
         Width           =   1275
      End
      Begin VB.OptionButton optAdornment 
         Caption         =   "��Ƭ�߿�Ч��"
         Height          =   285
         Index           =   2
         Left            =   2760
         TabIndex        =   9
         Top             =   630
         Width           =   1995
      End
      Begin VB.OptionButton optAdornment 
         Caption         =   "ӰƬ���ݿ�Ч��"
         Height          =   285
         Index           =   3
         Left            =   420
         TabIndex        =   8
         Top             =   630
         Width           =   2025
      End
   End
   Begin VB.CheckBox chkDisableProcessIsolation 
      Caption         =   "��Ϊ�������������"
      Height          =   465
      Left            =   270
      TabIndex        =   6
      Top             =   5670
      Width           =   2385
   End
   Begin VB.Frame Frame1 
      Caption         =   "����ͼ��"
      Height          =   975
      Left            =   270
      TabIndex        =   2
      Top             =   3480
      Width           =   5085
      Begin VB.CommandButton cmdOpenOverlay 
         Caption         =   "���"
         Height          =   345
         Left            =   4290
         TabIndex        =   4
         Top             =   360
         Width           =   675
      End
      Begin VB.TextBox txtOverlay 
         Height          =   345
         Left            =   150
         TabIndex        =   3
         Text            =   "overylay.ico"
         Top             =   360
         Width           =   4065
      End
   End
   Begin VB.CommandButton cmdUnRegister 
      Caption         =   "ע��"
      Height          =   345
      Left            =   4320
      TabIndex        =   1
      Top             =   6240
      Width           =   1035
   End
   Begin VB.CommandButton cmdRegister 
      Caption         =   "ע��"
      Height          =   345
      Left            =   3090
      TabIndex        =   0
      Top             =   6240
      Width           =   1155
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   5280
      Top             =   6360
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Label Label4 
      Caption         =   "Label4"
      Height          =   525
      Left            =   2220
      TabIndex        =   5
      Top             =   3810
      Width           =   1245
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'***********************************************************************************************************************
'
'ģ �� ��:  Form1
'
'��    ��:  Supergreenbean
'
'��������:  ע���Զ����ThumbnailProvier
'
'��    ��:  2009��03��10��
'
'��������:
'
'           1. ѡ���ṩThumbnailProvider������ļ�
'
'           2. ѡ��Ҫע�����
'
'           3. ����Ҫע����ļ���չ��
'
'           4. ѡ��������ʾ����ͼ���ͼ���ļ� ����ѡ������
'
'           5. ѡ��ϵͳ��ʾ����ͼʱ���õ�����Ч�� ����ѡ������
'
'           6. ѡ���Ƿ���Ϊ������������� ����ѡ������
'
'***********************************************************************************************************************
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "Form1"

Private Declare Function RegCreateKeyEx Lib "advapi32" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As Long, phkResult As Long, lpdwDisposition As Long) As Long
Private Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
Private Declare Function RegSetValueEx Lib "advapi32" Alias "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long
Private Declare Function SHDeleteKey Lib "shlwapi" Alias "SHDeleteKeyA" (ByVal hKey As Long, ByVal lpSubKey As String) As Long
Private Declare Function SHDeleteValue Lib "shlwapi" Alias "SHDeleteValueA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal lpValue As String) As Long

Private Const REG_SZ = 1
Private Const REG_DWORD = 4

Private Const HKEY_CLASSES_ROOT = &H80000000
Private Const HKEY_CURRENT_USER = &H80000001
Private Const HKEY_LOCAL_MACHINE = &H80000002
Private Const HKEY_USERS = &H80000003

Private Const ERROR_SUCCESS = 0
Private Const ERROR_ACCESS_DENIED = 5
Private Const ERROR_NO_MORE_ITEMS = 259

Private Declare Function GetSystemDirectory Lib "kernel32" Alias "GetSystemDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long

Private Const REGKEY_SE As String = "Software\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved"
Private Const STRING_IID_IThumbnailProvider  As String = "{E357FCCD-A995-4576-B01F-234630154E96}"
Private Const STRING_IID_IExtractImage  As String = "{BB2E617C-0920-11d1-9A0B-00C04FC2D6C1}"

Private m_sSelectedCLSID As String, m_sSelectedClassName As String

'�򿪺����Զ���ThumbnailProvier����ļ�
Private Sub cmdOpen_Click()
    On Error Resume Next
    With CommonDialog1
        .CancelError = True
        
        .ShowOpen
        
        If Err.Number = 0 Then
            
            txtFile.Text = .FileName
            
            If GetClassInformations = False Then
                MsgBox "ѡ����ļ���δ�����κο��õ��࣡"
            End If
            
            m_sSelectedCLSID = ""
        End If
    End With
End Sub

'ע��/ע�����
Private Sub CallRegProc(ByVal fReg As Boolean)
    If Len(txtFile.Text) = 0 Then Exit Sub
    
    Shell (GetSysDir & "\regsvr32.exe /s " & IIf(fReg, "", " /u ") & """" & txtFile.Text & """")
End Sub

Private Function GetSysDir() As String
    GetSysDir = Space$(255)
    GetSystemDirectory GetSysDir, 255
    GetSysDir = Trim(Replace$(GetSysDir, Chr$(0), ""))
End Function

'�������Ϣ
Private Function GetClassInformations() As Boolean
    Dim oLib As TypeLibInfo
    Set oLib = TypeLibInfoFromFile(txtFile.Text)
    
    If oLib Is Nothing Then Exit Function
    
    Dim cls As CoClassInfo
    
    lstCoClasses.Clear
     
    For Each cls In oLib.CoClasses
        lstCoClasses.AddItem oLib.Name & "." & cls.Name & "!" & cls.Guid
    Next
        
    GetClassInformations = (lstCoClasses.ListCount > 0)
End Function

'�������ע�����趨��ֵ
Private Function CreateRegKeyAndSetValue(ByVal hRoot As Long, ByVal sKeyName As String, ByVal sValueName, ByVal sData As String, Optional REGTYPE As Long = REG_SZ) As Boolean
    Dim lRet As Long
    Dim hKey As Long
    
    lRet = RegCreateKeyEx(hRoot, sKeyName, 0, vbNullString, 0, &HF003F, ByVal 0, hKey, ByVal 0)
    
    If lRet <> 0 Or hKey = 0 Then Exit Function
    
    If REGTYPE = REG_DWORD Then
        lRet = RegSetValueEx(hKey, sValueName, 0, REGTYPE, CLng(sData), 4)
    Else
        lRet = RegSetValueEx(hKey, sValueName, 0, REGTYPE, ByVal sData, Len(sData))
    End If
    
    RegCloseKey hKey
    
    CreateRegKeyAndSetValue = (lRet = 0)
End Function

'����������ͼ���ͼ���ļ�
Private Sub cmdOpenOverlay_Click()
    On Error Resume Next
    With CommonDialog1
        .CancelError = True
        
        .ShowOpen
        
        If Err.Number = 0 Then
            
            txtOverlay.Text = .FileName
        End If
    End With
End Sub

'ע������Լ���Ӧ���ļ���չ��
Private Sub cmdRegister_Click()
    If m_sSelectedCLSID = "" Then
        MsgBox "δѡ��Ҫʹ�õ��࣡"
        Exit Sub
    End If
    
    Dim sFileExt As String, sFileExtWithoutDot As String
    
    sFileExt = Trim(txtFileExt.Text)
    
    If Len(sFileExt) = 0 Then
        MsgBox "�ļ���չ������Ϊ�գ�"
        Exit Sub
    End If
    
    If Left(sFileExt, 1) <> "." Then
        MsgBox "��ʽ������.ext��"
        Exit Sub
    End If
    
    sFileExtWithoutDot = Mid$(sFileExt, 2)
    
    If CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExt, "", sFileExtWithoutDot) And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExt & "\ShellEx", "", "") And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExt & "\ShellEx\" & STRING_IID_IThumbnailProvider, "", m_sSelectedCLSID) And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExt & "\ShellEx\" & STRING_IID_IExtractImage, "", m_sSelectedCLSID) And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExt, "Treatment", GetTreatment(), REG_DWORD) And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExt, "TypeOverlay", txtOverlay.Text) And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, "CLSID\" & m_sSelectedCLSID, "DisableProcessIsolation", IIf(chkDisableProcessIsolation.Value = vbChecked, 1, 0), REG_DWORD) And _
        CreateRegKeyAndSetValue(HKEY_CLASSES_ROOT, sFileExtWithoutDot, "", sFileExtWithoutDot) And _
        CreateRegKeyAndSetValue(HKEY_LOCAL_MACHINE, REGKEY_SE, m_sSelectedCLSID, m_sSelectedClassName) Then

        Call CallRegProc(True)
                      
        MsgBox "ע��ɹ���"
    Else
        MsgBox "ע��ʧ�ܣ�"
    End If
End Sub

'�������Ч��ѡ��ֵ
Private Function GetTreatment() As Long
    GetTreatment = 0
    
    Dim i As Long
    For i = 0 To 3
        If optAdornment(i).Value Then
            GetTreatment = i
            Exit Function
        End If
    Next
End Function

'ע�������ɾ��ע��ʱд�������ע�����
Private Sub cmdUnRegister_Click()
    Dim sFileExt As String, sFileExtWithoutDot As String
    
    sFileExt = Trim(txtFileExt.Text)
    
    If Len(sFileExt) = 0 Then
        MsgBox "�ļ���չ������Ϊ�գ�"
        Exit Sub
    End If
    
    If Left(sFileExt, 1) <> "." Then
        MsgBox "��ʽ������.ext��"
        Exit Sub
    End If
    
    sFileExtWithoutDot = Mid$(sFileExt, 2)
   
    Call SHDeleteKey(HKEY_CLASSES_ROOT, sFileExt)
    Call SHDeleteKey(HKEY_CLASSES_ROOT, sFileExtWithoutDot)
    Call SHDeleteValue(HKEY_LOCAL_MACHINE, REGKEY_SE, m_sSelectedCLSID)
    Call CallRegProc(False)
    
    MsgBox "ע���ɹ���"
End Sub


'ѡ��Ҫע�����
Private Sub lstCoClasses_Click()
    With lstCoClasses
        If .ListCount = 0 Then Exit Sub
        
        If Len(.Text) = 0 Then Exit Sub
    
        m_sSelectedClassName = Left$(.Text, InStr(.Text, "!") - 1)
        m_sSelectedCLSID = Mid$(.Text, InStr(.Text, "!") + 1)
    End With
End Sub


