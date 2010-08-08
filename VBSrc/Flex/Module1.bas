Attribute VB_Name = "Module2"
Option Explicit
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long

Public Const GWL_WNDPROC = (-4)

Public Const SB_LINEDOWN = 1
Public Const SB_LINEUP = 0

Public Const WM_VSCROLL = &H115
Public Const WM_MOUSEWHEEL = &H20A

Public g_lDefWindowProc As Long

Public Function WindowProc(ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    On Error Resume Next
    
    '如果为滚轮消息则发送滚动消息
    If uMsg = WM_MOUSEWHEEL Then
        Call ScrollFlex(IIf(wParam > 0, SB_LINEUP, SB_LINEDOWN))
    End If
    
    WindowProc = CallWindowProc(g_lDefWindowProc, hwnd, uMsg, wParam, lParam)
End Function

'滚动MSHFlex控件,参数lDirection指明滚动方向
Public Sub ScrollFlex(ByVal lDirection As Long)
    SendMessage Form1.MSHFlexGrid1.hwnd, WM_VSCROLL, lDirection, ByVal 0
End Sub
