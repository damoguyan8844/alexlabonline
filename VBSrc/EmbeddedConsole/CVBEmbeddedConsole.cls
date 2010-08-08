VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBEmbeddedConsole"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function TerminateProcess Lib "kernel32" (ByVal hProcess As Long, ByVal uExitCode As Long) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hwnd As Long, lpdwProcessId As Long) As Long
Private Const PROCESS_ALL_ACCESS = &H1F0FFF

Private Declare Function GetSystemMenu Lib "user32" (ByVal hwnd As Long, ByVal bRevert As Long) As Long
Private Declare Function RemoveMenu Lib "user32" (ByVal hMenu As Long, ByVal nPosition As Long, ByVal wFlags As Long) As Long
Private Const MF_BYPOSITION = &H400
Private Const MF_REMOVE = &H1000
       
Private Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long
Private Const SM_CXBORDER As Long = 5
Private Const SM_CYBORDER As Long = 6
Private Const SM_CYCAPTION As Long = 4
Private Const SM_CYFRAME = 33
Private Const SM_CXFRAME = 32

Private Declare Function FindWindowEx Lib "user32" Alias "FindWindowExA" (ByVal hWnd1 As Long, ByVal hWnd2 As Long, ByVal lpsz1 As String, ByVal lpsz2 As String) As Long
Private Declare Function SetParent Lib "user32" (ByVal hWndChild As Long, ByVal hWndNewParent As Long) As Long

Private Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Private Const SWP_SHOWWINDOW = &H40
Private Const SWP_NOSIZE = &H1

Private Declare Function SetWindowRgn Lib "user32" (ByVal hwnd As Long, ByVal hRgn As Long, ByVal bRedraw As Boolean) As Long
Private Declare Function CreateRectRgn Lib "gdi32" (ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long

Private Declare Function GetClientRect Lib "user32" (ByVal hwnd As Long, lpRect As RECT) As Long
Private Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type


Private Const CONSOLE_CLASS_STRING = "ConsoleWindowClass"

Private m_hConsole As Long
Private m_lProcessID As Long

Public Function EmbedConsoleWindow(ByVal hTargetWnd As Long, Optional ByVal x As Long, Optional ByVal y As Long) As Boolean
    m_hConsole = CreateConsole(m_lProcessID)
    
    If m_hConsole = 0 Then Exit Function
    
    SetParent m_hConsole, hTargetWnd
    
    
    SetWindowPos m_hConsole, 0, x, y, 0, 0, SWP_SHOWWINDOW + SWP_NOSIZE
    
    Dim hRgn As Long, udtClient As RECT
    Dim lBorderWidthX As Long, lBorderWidthY As Long, lCaptionHeight As Long
    
    lBorderWidthX = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXBORDER)
    lBorderWidthY = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYBORDER)
    lCaptionHeight = GetSystemMetrics(SM_CYCAPTION)
    
    GetClientRect m_hConsole, udtClient
    With udtClient
        hRgn = CreateRectRgn(lBorderWidthX, lBorderWidthY + lCaptionHeight, .Right + lBorderWidthX, .Bottom + lBorderWidthY + lCaptionHeight)
    End With
    
    SetWindowRgn m_hConsole, hRgn, True
    
    Call KillSysMenu(m_hConsole)
End Function

Public Function TerminateConsole() As Long
    TerminateProcess OpenProcess(PROCESS_ALL_ACCESS, 0, m_lProcessID), 0
End Function

Public Property Get ConsoleWndHandle() As Long
    ConsoleWndHandle = m_hConsole
End Property

Public Property Get ConsoleProcessID() As Long
    ConsoleWndHandle = m_lProcessID
End Property

Private Sub KillSysMenu(ByVal hTargetWnd As Long)

    Dim hMenu As Long, i As Long

    hMenu = GetSystemMenu(hTargetWnd, 0)
  
    If hMenu <> 0 Then
        For i = 1 To 7
            Call RemoveMenu(hMenu, 0, MF_REMOVE Or MF_BYPOSITION)
        Next
    End If
End Sub

Private Function CreateConsole(lProcessID As Long) As Long
    Dim hConsole As Long
    Dim lPID As Long
    
    lProcessID = Shell("cmd", vbNormalFocus)
        
    Do While lProcessID <> 0
        CreateConsole = FindWindowEx(0, 0, CONSOLE_CLASS_STRING, vbNullString)
        
        GetWindowThreadProcessId CreateConsole, lPID
        
        If lPID = lProcessID Then
            Exit Do
        End If
        
        DoEvents
    Loop
End Function


Private Sub Class_Terminate()
    Call TerminateConsole
End Sub
