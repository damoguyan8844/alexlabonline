VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   4620
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5520
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   4620
   ScaleWidth      =   5520
   StartUpPosition =   1  '所有者中心
   Begin VB.TextBox txtStatusText 
      Height          =   1905
      Left            =   510
      MultiLine       =   -1  'True
      ScrollBars      =   1  'Horizontal
      TabIndex        =   9
      Top             =   2550
      Width           =   4485
   End
   Begin VB.Frame Frame1 
      Height          =   60
      Left            =   330
      TabIndex        =   8
      Top             =   1650
      Width           =   4785
   End
   Begin VB.CommandButton cmdStop 
      Caption         =   "停止下载"
      Height          =   405
      Left            =   3690
      TabIndex        =   7
      Top             =   1110
      Width           =   1215
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "开始下载"
      Height          =   405
      Left            =   2340
      TabIndex        =   6
      Top             =   1110
      Width           =   1215
   End
   Begin VB.TextBox txtDest 
      Height          =   345
      Left            =   1080
      TabIndex        =   3
      Text            =   "c:\DZH.exe"
      Top             =   630
      Width           =   3825
   End
   Begin MSComctlLib.ProgressBar pb 
      Height          =   225
      Left            =   480
      TabIndex        =   2
      Top             =   1950
      Width           =   4545
      _ExtentX        =   8017
      _ExtentY        =   397
      _Version        =   393216
      Appearance      =   1
   End
   Begin VB.TextBox txtSrc 
      Height          =   345
      Left            =   1080
      TabIndex        =   0
      Text            =   "ftp://61.129.74.213/pub/DZH_INTERNET_V519w.EXE"
      Top             =   210
      Width           =   3825
   End
   Begin VB.Label lblProgress 
      AutoSize        =   -1  'True
      Caption         =   "进度0%"
      Height          =   180
      Left            =   2370
      TabIndex        =   5
      Top             =   2250
      Width           =   540
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "保存为:"
      Height          =   180
      Left            =   390
      TabIndex        =   4
      Top             =   690
      Width           =   630
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "URL:"
      Height          =   180
      Left            =   660
      TabIndex        =   1
      Top             =   300
      Width           =   360
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private WithEvents m_oFileDownload As CFileDownload
Attribute m_oFileDownload.VB_VarHelpID = -1

Private Sub cmdStart_Click()
    pb.Value = 0
    cmdStart.Enabled = False
    cmdStop.Enabled = True
    Me.Caption = "下载中……"
    
    If m_oFileDownload.StartDownloading(txtSrc.Text, txtDest.Text) Then
        MsgBox "下载成功！"
    Else
        MsgBox "下载失败！"
    End If
        
    cmdStart.Enabled = True
    cmdStop.Enabled = False
    Me.Caption = "空闲中"
    lblProgress.Caption = "下载进度"
End Sub

Private Sub cmdStop_Click()
    cmdStart.Enabled = True
    cmdStop.Enabled = False
    Me.Caption = "空闲中"
    lblProgress.Caption = "下载进度"
    m_oFileDownload.AbortDownloading
End Sub

Private Sub Form_Load()
    Set m_oFileDownload = New CFileDownload
    
    Me.Caption = "空闲中"
    lblProgress.Caption = "下载进度"
    cmdStart.Enabled = True
    cmdStop.Enabled = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_oFileDownload = Nothing
End Sub

Private Sub m_oFileDownload_OnProgress(ByVal lProgress As Long, ByVal lMaxProgress As Long, ByVal lStatusCode As Long, ByVal sStatusText As String)
    Dim bPercent As Long
    
    If lMaxProgress = 0 Then
        bPercent = 0
    Else
        bPercent = Int(lProgress / lMaxProgress * 100)
    End If
    
    pb.Value = bPercent
    lblProgress.Caption = "已下载" & CStr(bPercent) & "%"
    
    txtStatusText.Text = txtStatusText.Text & sStatusText
    txtStatusText.Text = txtStatusText.Text & vbCrLf
    txtStatusText.SelStart = Len(txtStatusText.Text)
End Sub
