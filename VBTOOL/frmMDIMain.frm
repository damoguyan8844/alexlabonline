VERSION 5.00
Begin VB.MDIForm frmMDIMain 
   BackColor       =   &H8000000C&
   Caption         =   "Main"
   ClientHeight    =   6240
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   11745
   LinkTopic       =   "MDIForm1"
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picHolder 
      Align           =   1  'Align Top
      Height          =   2055
      Left            =   0
      ScaleHeight     =   1995
      ScaleWidth      =   11685
      TabIndex        =   0
      Top             =   0
      Width           =   11745
      Begin VB.PictureBox picWork 
         BorderStyle     =   0  'None
         Height          =   1215
         Left            =   720
         Picture         =   "frmMDIMain.frx":0000
         ScaleHeight     =   1215
         ScaleWidth      =   9135
         TabIndex        =   1
         Top             =   480
         Width           =   9135
      End
      Begin VB.Timer OTSTaskTimer 
         Interval        =   400
         Left            =   10320
         Top             =   600
      End
   End
   Begin VB.Menu mnuGQHMain 
      Caption         =   "GQHMain"
      Begin VB.Menu mnuFormMain 
         Caption         =   "FormMain"
      End
   End
   Begin VB.Menu mnuSpread 
      Caption         =   "Spread"
      Begin VB.Menu mnuCalibrateSpread 
         Caption         =   "CalibrateSpread"
      End
   End
   Begin VB.Menu mnuNetProject 
      Caption         =   "NetProject"
      Begin VB.Menu mnuNetCommand 
         Caption         =   "NetCommand"
      End
   End
   Begin VB.Menu mnuCarolinaProject 
      Caption         =   "Carolina Project"
      Begin VB.Menu mnuRealTimeTaskCalculate 
         Caption         =   "RealTimeTaskCalculate"
      End
   End
End
Attribute VB_Name = "frmMDIMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
'**********************
Private Declare Function TransparentBlt Lib "msimg32.dll" (ByVal hdcDest As Long, ByVal nXOriginDest As Long, ByVal nYOriginDest As Long, ByVal nWidthDest As Long, ByVal nHeightDest As Long, ByVal hdcSrc As Long, ByVal nXOriginSrc As Long, ByVal nYOriginSrc As Long, ByVal nWidthSrc As Long, ByVal nHeightSrc As Long, ByVal crTransparent As Long) As Long

Private Declare Function StretchBlt Lib "gdi32" (ByVal hDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal nSrcWidth As Long, ByVal nSrcHeight As Long, ByVal dwRop As Long) As Long

Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Declare Function CreateCompatibleDC Lib "gdi32" (ByVal lDC As Long) As Long
Private Declare Function DeleteDC Lib "gdi32" (ByVal lDC As Long) As Long
Private Declare Function SelectObject Lib "gdi32" (ByVal lDC As Long, ByVal hObject As Long) As Long
Private Declare Function WinExec Lib "kernel32" (ByVal lpCmdLine As String, ByVal nCmdShow As Long) As Long
Private Const SW_SHOW = 5

Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, _
                                                     ByVal bInheritHandle As Long, _
                                                     ByVal dwProcessId As Long) As Long
    
Private Declare Function GetExitCodeProcess Lib "kernel32" (ByVal hProcess As Long, lpExitCode As Long) As Long
    
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
    
Private Const PROCESS_QUERY_INFORMATION = &H400
Private Const STATUS_PENDING = &H103
    
'***************
Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type
'Private Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" (lpMutexAttributes As SECURITY_ATTRIBUTES, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
'Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

'Private Const ERROR_ALREADY_EXISTS = 183&
'Private MutexHandle As Long

Dim Pic As StdPicture, hMemDC As Long, pHeight As Long, pWidth As Long

Private Sub MDIForm_Load()
   
    picHolder.Visible = False
    picWork.Visible = False
    picWork.AutoRedraw = True
    picWork.AutoSize = True
    ' I placed this picture directly into the picWork object
    'picWork.Picture = LoadPicture("c:\winnt\internet explorer wallpaper.bmp")
    pHeight = picWork.Height
    pWidth = picWork.Width
    Set Pic = picWork.Picture
    Set picWork.Picture = Nothing
    picWork.AutoSize = False
    
    hMemDC = CreateCompatibleDC(picWork.hDC)
    SelectObject hMemDC, Pic.Handle
    
End Sub

Private Sub mnuCalibrateSpread_Click()
  If Not FormExist("frmSpread") Then
        Dim frm As New frmSpread
        frm.Show
        frm.Caption = GetFormCaption("frmSpread")
    End If
End Sub

Private Sub mnuFormMain_Click()
   If Not FormExist("frmMain") Then
        Dim frm As New frmMain
        frm.Show
        frm.Caption = GetFormCaption("frmMain")
    End If
End Sub

Private Sub MDIForm_Resize()
    If WindowState <> vbMinimized Then
        Dim w As Long, h As Long
        picWork.BackColor = BackColor
        picWork.Move 0, 0, ScaleWidth, ScaleHeight
        'x = ScaleWidth - pWidth: x = x \ Screen.TwipsPerPixelX
        'y = ScaleHeight - pHeight: y = y \ Screen.TwipsPerPixelY
        w = pWidth \ Screen.TwipsPerPixelX
        h = pHeight \ Screen.TwipsPerPixelY
        StretchBlt picWork.hDC, 0, 0, ScaleWidth \ Screen.TwipsPerPixelX, ScaleHeight \ Screen.TwipsPerPixelY, hMemDC, 0, 0, w, h, vbSrcCopy
        Set Picture = picWork.Image
        'MsgBox (ScaleWidth \ Screen.TwipsPerPixelX) & "," & (ScaleHeight \ Screen.TwipsPerPixelY)
    End If
End Sub

Private Sub MDIForm_Unload(Cancel As Integer)
    Set Pic = Nothing
    DeleteDC hMemDC
End Sub

Private Sub mnuNetCommand_Click()
    If Not FormExist("frmCommand") Then
        Dim frm As New frmCommand
        frm.Show
        frm.Caption = GetFormCaption("frmCommand")
    End If
End Sub

Private Sub mnuRealTimeTaskCalculate_Click()
    If Not FormExist("frmRealTimeTask") Then
        Dim frm As New frmRealTimeTask
        frm.Show
        frm.Caption = GetFormCaption("frmRealTimeTask")
    End If
End Sub
