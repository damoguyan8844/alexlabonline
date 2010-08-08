VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   3285
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5625
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3285
   ScaleWidth      =   5625
   StartUpPosition =   2  '屏幕中心
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   4740
      Top             =   1410
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton Command2 
      Caption         =   "打开"
      Height          =   375
      Left            =   4380
      TabIndex        =   5
      Top             =   180
      Width           =   1125
   End
   Begin VB.TextBox txtFile 
      Height          =   345
      Left            =   990
      Locked          =   -1  'True
      TabIndex        =   4
      Top             =   180
      Width           =   3315
   End
   Begin VB.ComboBox cboIconIndex 
      Height          =   300
      Left            =   990
      TabIndex        =   2
      Text            =   "Combo1"
      Top             =   660
      Width           =   3315
   End
   Begin VB.CommandButton Command1 
      Caption         =   "设定图标"
      Height          =   375
      Left            =   4380
      TabIndex        =   0
      Top             =   630
      Width           =   1125
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "预览"
      Height          =   180
      Index           =   3
      Left            =   120
      TabIndex        =   8
      Top             =   1830
      Width           =   360
   End
   Begin VB.Image Image1 
      BorderStyle     =   1  'Fixed Single
      Height          =   1065
      Left            =   990
      Stretch         =   -1  'True
      Top             =   1830
      Width           =   1155
   End
   Begin VB.Label Label2 
      Caption         =   "0个"
      Height          =   405
      Left            =   1020
      TabIndex        =   7
      Top             =   1260
      Width           =   2085
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "图标总数"
      Height          =   180
      Index           =   2
      Left            =   90
      TabIndex        =   6
      Top             =   1260
      Width           =   720
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "当前文件名"
      Height          =   180
      Index           =   1
      Left            =   60
      TabIndex        =   3
      Top             =   210
      Width           =   900
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "图标编号"
      Height          =   180
      Index           =   0
      Left            =   90
      TabIndex        =   1
      Top             =   720
      Width           =   720
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim m_oAnyIcon As CVBAnyIcon

Private Sub Command1_Click()
    On Error Resume Next
    Set Me.Icon = m_oAnyIcon.Picture(Val(cboIconIndex.Text))
    Set Image1.Picture = Me.Icon
End Sub

Private Sub Command2_Click()
    On Error Resume Next
    Dim s As String
    Dim i As Long
    With m_oAnyIcon
        s = GetFileName
        If s <> "" Then
            .Filename = s
            txtFile.Text = s
            i = .IconCount
            Label2.Caption = CStr(i) & "个"
            With cboIconIndex
                .Clear
                For i = 1 To i
                    .AddItem i
                Next
                .ListIndex = 0
                Command1.Enabled = IIf(cboIconIndex.ListCount = 0, False, True)
            End With
        End If
    End With
End Sub

Private Sub Form_Load()
    Set m_oAnyIcon = New CVBAnyIcon
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_oAnyIcon = Nothing
End Sub

Function GetFileName() As String
On Error Resume Next
    With CommonDialog1
        .Filter = "*.exe|*.exe|*.dll|*.dll|*.ico|*.ico|*.cur|*.cur"
        .ShowOpen
        GetFileName = .Filename
    End With
End Function

