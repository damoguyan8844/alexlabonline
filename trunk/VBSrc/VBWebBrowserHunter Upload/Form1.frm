VERSION 5.00
Begin VB.Form frm 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "GetWebBrowser"
   ClientHeight    =   9000
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6615
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MouseIcon       =   "Form1.frx":000C
   ScaleHeight     =   9000
   ScaleWidth      =   6615
   StartUpPosition =   2  'ÆÁÄ»ÖÐÐÄ
   Begin VB.PictureBox pbTarget 
      Height          =   540
      Left            =   1260
      Picture         =   "Form1.frx":015E
      ScaleHeight     =   480
      ScaleWidth      =   480
      TabIndex        =   11
      Top             =   390
      Width           =   540
   End
   Begin VB.CommandButton Command2 
      Caption         =   "GetWebBrowser"
      Default         =   -1  'True
      Height          =   540
      Left            =   3480
      TabIndex        =   8
      Top             =   390
      Width           =   2355
   End
   Begin VB.Frame Frame1 
      Caption         =   "Frame X"
      Height          =   5985
      Left            =   90
      TabIndex        =   1
      Top             =   2910
      Width           =   6435
      Begin VB.Frame Frame2 
         Height          =   45
         Left            =   90
         TabIndex        =   12
         Top             =   1620
         Width           =   6285
      End
      Begin VB.TextBox txtSrcCode 
         Height          =   3585
         Left            =   120
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   5
         Top             =   2220
         Width           =   6195
      End
      Begin VB.TextBox txtURL 
         Height          =   345
         Left            =   510
         TabIndex        =   3
         Top             =   450
         Width           =   5715
      End
      Begin VB.CommandButton Command1 
         Caption         =   "Go to URL"
         Height          =   375
         Left            =   5010
         TabIndex        =   2
         Top             =   900
         Width           =   1215
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "Source Code:"
         Height          =   195
         Left            =   120
         TabIndex        =   6
         Top             =   1950
         Width           =   975
      End
      Begin VB.Label Label1 
         Caption         =   "URL:"
         Height          =   255
         Left            =   90
         TabIndex        =   4
         Top             =   510
         Width           =   375
      End
   End
   Begin VB.TextBox txtWnd 
      Height          =   345
      Left            =   2640
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   2250
      Width           =   1545
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "step 2: get browser object from hTargetWnd"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   630
      Left            =   3750
      TabIndex        =   10
      Top             =   1020
      Width           =   1860
      WordWrap        =   -1  'True
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "           step 1: locate a                ""Internet Explorer_Server"" window by draging this"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   585
      Left            =   330
      TabIndex        =   9
      Top             =   1020
      Width           =   2595
      WordWrap        =   -1  'True
   End
   Begin VB.Label Label3 
      AutoSize        =   -1  'True
      Caption         =   "hTargetWnd:"
      Height          =   195
      Left            =   1620
      TabIndex        =   7
      Top             =   2310
      Width           =   945
   End
End
Attribute VB_Name = "frm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function GetParent Lib "user32" (ByVal hwnd As Long) As Long

Private Declare Function SetCursor Lib "user32" (ByVal hCursor As Long) As Long
Private Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Private Declare Function WindowFromPoint Lib "user32" (ByVal xPoint As Long, ByVal yPoint As Long) As Long

Private Type POINTAPI
        X As Long
        Y As Long
End Type

Private m_hCursorOld As Long
Private m_hCursorLoaded As Long

Private m_oWBHunter As CVBWebBrowserHunter
Attribute m_oWBHunter.VB_VarHelpID = -1
Private WithEvents m_oIE As InternetExplorer
Attribute m_oIE.VB_VarHelpID = -1

Private Sub Command1_Click()
    On Error GoTo ErrHandler
    
    m_oIE.Navigate2 Trim(txtURL.Text)
    
    Exit Sub
ErrHandler:
    MsgBox "faild to communicate with remote object!"
End Sub

Private Sub Command2_Click()
    On Error GoTo ErrHandler
    
    If m_oWBHunter.GetBrowser(GetTargetWnd) Then
        Set m_oIE = m_oWBHunter.IEObject
        
        txtURL.Text = m_oIE.LocationURL
        
        Dim oDocument As HTMLDocument
        Set oDocument = m_oIE.Document
        txtSrcCode.Text = oDocument.documentElement.innerHTML
    Else
        MsgBox "failed to get webbrowser object from target window!"""
    End If
    Exit Sub
ErrHandler:
    
End Sub

Private Function GetTargetWnd() As Long
    On Error Resume Next
    GetTargetWnd = CLng(Trim(txtWnd.Text))
End Function

Private Sub Form_Load()
    Set m_oWBHunter = New CVBWebBrowserHunter
    
    m_hCursorLoaded = Me.MouseIcon
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_oWBHunter = Nothing
End Sub

Private Sub m_oIE_OnQuit()
    MsgBox "Browser Window is Closing!"
    
    txtURL.Text = ""
    txtWnd.Text = ""
End Sub

Private Sub pbTarget_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    m_hCursorOld = SetCursor(m_hCursorLoaded)
End Sub

Private Sub pbTarget_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Static pt As POINTAPI
    Static hwnd As Long
        
    Call GetCursorPos(pt)
    
    hwnd = WindowFromPoint(pt.X, pt.Y)
    
    'Internet Explorer_Server
    txtWnd.Text = CStr(GetParent(GetParent((hwnd))))
End Sub

Private Sub pbTarget_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Call SetCursor(m_hCursorOld)
End Sub
