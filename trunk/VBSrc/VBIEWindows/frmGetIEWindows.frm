VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmGetIEWindows 
   Caption         =   "Get IE Core Windows"
   ClientHeight    =   8730
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7980
   LinkTopic       =   "Form1"
   ScaleHeight     =   8730
   ScaleWidth      =   7980
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton cmdGetWebBrowser 
      Caption         =   "GetWebBrowser"
      Height          =   540
      Left            =   2940
      TabIndex        =   9
      Top             =   3330
      Width           =   2355
   End
   Begin VB.Frame Frame1 
      Caption         =   "Frame X"
      Height          =   4695
      Left            =   150
      TabIndex        =   2
      Top             =   3990
      Width           =   7665
      Begin VB.CommandButton cmdGotoURL 
         Caption         =   "Go to URL"
         Height          =   375
         Left            =   6300
         TabIndex        =   6
         ToolTipText     =   "type a url above and click, guess what will happen to the selected ie window?"
         Top             =   930
         Width           =   1215
      End
      Begin VB.TextBox txtURL 
         Height          =   345
         Left            =   510
         TabIndex        =   5
         Top             =   450
         Width           =   7035
      End
      Begin VB.TextBox txtSrcCode 
         Height          =   2325
         Left            =   120
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   4
         Top             =   2220
         Width           =   7395
      End
      Begin VB.Frame Frame2 
         Height          =   45
         Left            =   90
         TabIndex        =   3
         Top             =   1620
         Width           =   7485
      End
      Begin VB.Label Label1 
         Caption         =   "URL:"
         Height          =   255
         Left            =   90
         TabIndex        =   8
         Top             =   510
         Width           =   375
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "Source Code:"
         Height          =   195
         Left            =   120
         TabIndex        =   7
         Top             =   1950
         Width           =   975
      End
   End
   Begin MSComctlLib.ListView ListView1 
      Height          =   2235
      Left            =   150
      TabIndex        =   1
      Top             =   960
      Width           =   7725
      _ExtentX        =   13626
      _ExtentY        =   3942
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   0
   End
   Begin VB.CommandButton cmdGetWindows 
      Caption         =   "Get IE Windows"
      Default         =   -1  'True
      Height          =   585
      Left            =   3030
      TabIndex        =   0
      Top             =   180
      Width           =   1875
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "step 2"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Index           =   1
      Left            =   2190
      TabIndex        =   11
      Top             =   3210
      Width           =   705
      WordWrap        =   -1  'True
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      AutoSize        =   -1  'True
      Caption         =   "step 1"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Index           =   0
      Left            =   2340
      TabIndex        =   10
      Top             =   90
      Width           =   645
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmGetIEWindows"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Long, ByVal Length As Long)
Private Declare Function EnumWindows Lib "user32.dll" (ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Private Declare Function EnumChildWindows Lib "user32.dll" (ByVal hWndParent As Long, ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Private Declare Function GetClassName Lib "user32.dll" Alias "GetClassNameA" (ByVal hwnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long) As Long
Private Declare Function GetWindowModuleFileName Lib "user32.dll" Alias "GetWindowModuleFileNameA" (ByVal hwnd As Long, ByVal pszFileName As String, ByVal cchFileNameMax As Long) As Long
Private Declare Function GetWindowText Lib "user32.dll" Alias "GetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
Private Declare Function GetWindowTextLength Lib "user32.dll" Alias "GetWindowTextLengthA" (ByVal hwnd As Long) As Long

Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long
Private Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

Private Const WM_USER = &H400
Private Const WM_GETSHELLBROWSER = WM_USER + 7

Private Declare Function GetAncestor Lib "user32.dll" (ByVal hwnd As Long, ByVal gaFlags As Long) As Long
Private Const GA_ROOT As Long = 2

Private Type HOSTWINDOWINFO
    hwnd As Long
    WindowText As String
    WindowModuleFileName As String
End Type

Private Type IEWINDOWINFO
    HOSTWINDOWINFO As HOSTWINDOWINFO
    WindowText As String
    hwnd As Long
End Type

Private m_oWBHunter As CVBWebBrowserHunter
Private WithEvents m_oIE As InternetExplorer
Attribute m_oIE.VB_VarHelpID = -1

 
Private Function GetHostWindowInfo(ByVal hTargetWnd As Long) As IEWINDOWINFO
    Dim tInfo As IEWINDOWINFO
    Dim lLen As Long
    Dim txt As String, cls As String
    
    GetWindowTextAndClass hTargetWnd, txt, cls
    
    tInfo.hwnd = hTargetWnd
    tInfo.WindowText = txt

    With tInfo.HOSTWINDOWINFO
        .hwnd = GetAncestor(hTargetWnd, GA_ROOT)
        .WindowModuleFileName = Space$(1024)
        lLen = GetWindowModuleFileName(.hwnd, .WindowModuleFileName, 1024)
        .WindowModuleFileName = Left$(.WindowModuleFileName, lLen)
        GetWindowTextAndClass .hwnd, .WindowText, ""
    End With
    
    GetHostWindowInfo = tInfo
End Function

'取得窗口标题以及类名
Private Function GetWindowTextAndClass(ByVal hwnd As Long, sWindowText As String, sWindowClass As String) As Boolean
    Dim lTextLen As Long, lClassNameLen As Long
    lTextLen = GetWindowTextLength(hwnd)
    sWindowText = Space$(lTextLen + 1)
    lTextLen = GetWindowText(hwnd, sWindowText, lTextLen + 1)
    sWindowText = Left$(sWindowText, lTextLen)
    sWindowClass = Space$(256)
    lClassNameLen = GetClassName(hwnd, sWindowClass, 256)
    sWindowClass = Left$(sWindowClass, lClassNameLen)
    
    GetWindowTextAndClass = True
End Function

Private Sub cmdGetWebBrowser_Click()
    On Error GoTo ErrHandler
    
    Dim hTargetWnd As Long
    If ListView1.SelectedItem Is Nothing Then
        MsgBox "please select a window from the listview above first!"
        Exit Sub
    End If
    
    hTargetWnd = CLng(ListView1.SelectedItem.SubItems(2))
    
    If m_oWBHunter.GetBrowser(hTargetWnd) Then
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
    Debug.Print Err.Description
End Sub

Private Sub cmdGetWindows_Click()
    Dim oIEWindows As New CVBIEWindows
    oIEWindows.DoSearch
    
    ListView1.ListItems.Clear
    
    Dim tInfo As IEWINDOWINFO
    Dim hwnd As Variant
    For Each hwnd In oIEWindows.ShellWindows
        With ListView1
            tInfo = GetHostWindowInfo(hwnd)
            With .ListItems.Add(, , tInfo.HOSTWINDOWINFO.WindowText)
                .ListSubItems.Add , , tInfo.HOSTWINDOWINFO.WindowModuleFileName
                .ListSubItems.Add , , tInfo.hwnd
            End With
        End With
    Next
End Sub

Private Sub cmdGotoURL_Click()
    On Error GoTo ErrHandler
    
    m_oIE.Navigate2 Trim(txtURL.Text)
    
    Exit Sub
ErrHandler:
    MsgBox "faild to communicate with remote object!"
End Sub

Private Sub Form_Load()
    ListView1.View = lvwReport
    ListView1.HideSelection = False
    With ListView1.ColumnHeaders
        .Add(, , "Host Window Text").Width = 5000
        .Add(, , "Host App Path").Width = 5000
        .Add , , "IE Window Handle"
    End With
    
    Set m_oWBHunter = New CVBWebBrowserHunter
End Sub

Private Sub m_oIE_DocumentComplete(ByVal pDisp As Object, URL As Variant)
        MsgBox "Document Complete!" & vbCrLf & "URL:" & URL
End Sub

