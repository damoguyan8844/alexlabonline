Attribute VB_Name = "Module1"
Option Explicit
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function GetProp Lib "user32" Alias "GetPropA" (ByVal hwnd As Long, ByVal lpString As String) As Long
Public Declare Function SetProp Lib "user32" Alias "SetPropA" (ByVal hwnd As Long, ByVal lpString As String, ByVal hData As Long) As Long
Public Declare Function RemoveProp Lib "user32" Alias "RemovePropA" (ByVal hwnd As Long, ByVal lpString As String) As Long
Public Const GWL_WNDPROC = (-4&)
Public Const WM_NULL = 0

Public Function WinPropBag_ProcAddress(ByVal hwnd As Long, ByVal fStoreValue As Boolean, Optional ByVal lProcAddress As Long = 0, Optional ByVal fRemoveProp As Boolean = False) As Long
    If fStoreValue Then
        '保存属性
        SetProp hwnd, "MY_WINPROP_PROCADDRESS", lProcAddress
    Else
        '取出属性
        WinPropBag_ProcAddress = GetProp(hwnd, "MY_WINPROP_PROCADDRESS")
        If fRemoveProp Then
            '删除属性
            RemoveProp hwnd, "MY_WINPROP_PROCADDRESS"
        End If
    End If
    
End Function

Public Sub Subclassing(ByVal hWndTarget As Long, Optional ByVal fUnsubclassing As Boolean = False)
    If fUnsubclassing Then
        SetWindowLong hWndTarget, GWL_WNDPROC, WinPropBag_ProcAddress(hwnd:=hWndTarget, fStoreValue:=False, fRemoveProp:=True)
    Else
        WinPropBag_ProcAddress hWndTarget, True, SetWindowLong(hWndTarget, GWL_WNDPROC, AddressOf MyWindowProc)
    End If
End Sub

Public Function MyWindowProc(ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    Static sMsg As String
    If uMsg = WM_NULL Then
        If wParam And &HFFFFFF00 Then
            frmTest.lstMsg.AddItem IIf(wParam And &H80000000, "松开", "按下") & sMsg
        Else
            sMsg = "虚拟键码：&H" & Hex(wParam)
        End If
        MyWindowProc = 0
    Else
        MyWindowProc = CallWindowProc(WinPropBag_ProcAddress(hwnd, False), hwnd, uMsg, wParam, lParam)
    End If
End Function


