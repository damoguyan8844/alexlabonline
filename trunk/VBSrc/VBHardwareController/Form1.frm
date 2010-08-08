VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "硬件控制示例"
   ClientHeight    =   5115
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7980
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   5115
   ScaleWidth      =   7980
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton cmdLoadMedia 
      Caption         =   "收回"
      Height          =   345
      Left            =   2520
      TabIndex        =   10
      Top             =   4740
      Width           =   825
   End
   Begin VB.CommandButton cmdEjectMedia 
      Caption         =   "弹出"
      Height          =   345
      Left            =   1590
      TabIndex        =   9
      Top             =   4740
      Width           =   825
   End
   Begin VB.ComboBox cboDrives 
      Height          =   300
      Left            =   2310
      Style           =   2  'Dropdown List
      TabIndex        =   7
      Top             =   3930
      Width           =   1905
   End
   Begin VB.CommandButton cmdTellDriveType 
      Caption         =   "告诉我驱动器类型"
      Height          =   345
      Left            =   4290
      TabIndex        =   6
      Top             =   3900
      Width           =   1995
   End
   Begin VB.TextBox Text1 
      BackColor       =   &H8000000F&
      Height          =   315
      Left            =   1590
      Locked          =   -1  'True
      TabIndex        =   5
      Top             =   4350
      Width           =   4695
   End
   Begin VB.Frame Frame1 
      Height          =   30
      Left            =   150
      TabIndex        =   4
      Top             =   3690
      Width           =   7635
   End
   Begin VB.CommandButton cmdSetState 
      Caption         =   "禁用"
      Height          =   495
      Index           =   1
      Left            =   6480
      TabIndex        =   3
      Top             =   1320
      Width           =   1365
   End
   Begin VB.CommandButton cmdSetState 
      Caption         =   "启用"
      Height          =   495
      Index           =   0
      Left            =   6480
      TabIndex        =   2
      Top             =   690
      Width           =   1365
   End
   Begin VB.CommandButton cmdGetList 
      Caption         =   "获得硬件列表"
      Height          =   495
      Left            =   6480
      TabIndex        =   1
      Top             =   60
      Width           =   1365
   End
   Begin MSComctlLib.TreeView tvHardwareList 
      Height          =   3435
      Left            =   90
      TabIndex        =   0
      Top             =   60
      Width           =   6225
      _ExtentX        =   10980
      _ExtentY        =   6059
      _Version        =   393217
      Indentation     =   226
      LabelEdit       =   1
      LineStyle       =   1
      Style           =   7
      FullRowSelect   =   -1  'True
      Appearance      =   1
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "驱动器："
      Height          =   180
      Left            =   1560
      TabIndex        =   8
      Top             =   3990
      Width           =   720
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private WithEvents m_oHardwareController As CVBHardwareController
Attribute m_oHardwareController.VB_VarHelpID = -1

Private Sub cmdEjectMedia_Click()
    Text1.Text = CStr(m_oHardwareController.EjectMedia(cboDrives.Text))
End Sub

Private Sub cmdGetList_Click()
    tvHardwareList.Nodes.Clear
    Call m_oHardwareController.EnumDevices
End Sub


Private Sub cmdLoadMedia_Click()
    Text1.Text = CStr(m_oHardwareController.EjectMedia(cboDrives.Text, False))
End Sub

Private Sub cmdSetState_Click(Index As Integer)
    Dim v As Variant
    On Error Resume Next
    v = Split(tvHardwareList.SelectedItem.Key, "!")
    If Err.Number = 0 Then
        On Error GoTo 0
        If m_oHardwareController.SetDeviceState(CLng(v(2)), CStr(v(1)), IIf(Index = 0, True, False)) Then
            MsgBox "操作成功！"
        Else
            MsgBox "操作失败！"
        End If
    End If
End Sub

Private Sub cmdTellDriveType_Click()
    Text1.Text = m_oHardwareController.TellDriveType(cboDrives.Text)
End Sub

Private Sub Form_Load()
    Set m_oHardwareController = New CVBHardwareController
    
    Call m_oHardwareController.GetDriveNames(cboDrives)
    cboDrives.ListIndex = 0
End Sub


Private Sub AddToTV(ByVal sClass As String, ByVal sVal As String, ByVal sKey As String)
    Dim oNode As Node
    With tvHardwareList.Nodes
        On Error Resume Next
        Set oNode = .Item(sClass)
        If Err.Number <> 0 Then
            .Add , , sClass, sClass
        End If
        On Error GoTo 0
        .Add sClass, tvwChild, sKey, sVal
    End With
End Sub

Private Sub m_oHardwareController_EnumDevicesProc(ByVal lIndex As Long, ByVal sDeviceName As String, ByVal sDeviceClassName As String, ByVal sDeviceFriendlyName As String, ByVal sDeviceEnumeratorName As String, ByVal sDeviceClassGuid As String)
    AddToTV IIf(sDeviceClassName = "", "Unkown Device Class Name", sDeviceClassName), IIf(sDeviceFriendlyName = "", sDeviceName, sDeviceFriendlyName), "k" & "!" & sDeviceClassGuid & "!" & CStr(lIndex)
End Sub

