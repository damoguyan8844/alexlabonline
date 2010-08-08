Attribute VB_Name = "Module1"
Option Explicit
Private Declare Function GetWindow Lib "user32" (ByVal hWnd As Long, ByVal wCmd As Long) As Long
Private Declare Function SetWindowFocus Lib "user32" Alias "SetFocus" (ByVal hWnd As Long) As Long

Private Const GW_HWNDFIRST = 0
Private Const GW_HWNDLAST = 1
Private Const GW_HWNDNEXT = 2
Private Const GW_HWNDPREV = 3

Public Sub Walk2Next(ByVal hWndMe As Long, ByVal KeyCode As Integer)
    Dim hWndFirst As Long, hWndLast As Long, hWndPrev As Long, hWndNext As Long
    Dim hWndTarget As Long
       
    '获得最顶层窗口句柄
    hWndFirst = GetWindow(hWndMe, GW_HWNDFIRST)
    '获得最底层窗口句柄
    hWndLast = GetWindow(hWndMe, GW_HWNDLAST)
    '获得前1个窗口句柄
    hWndPrev = GetWindow(hWndMe, GW_HWNDPREV)
    '获得后1个窗口句柄
    hWndNext = GetWindow(hWndMe, GW_HWNDNEXT)
       
    Select Case KeyCode
    '按了向上或向左的箭头键
    Case vbKeyUp, vbKeyLeft
        hWndTarget = IIf(hWndNext, hWndNext, hWndFirst)
    '按了向下或向右的箭头键
    Case vbKeyDown, vbKeyRight
        hWndTarget = IIf(hWndPrev, hWndPrev, hWndLast)
    End Select
    
    '将下个应该获得焦点的窗口设置焦点
    SetWindowFocus hWndTarget
End Sub
