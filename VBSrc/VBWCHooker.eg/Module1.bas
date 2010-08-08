Attribute VB_Name = "Module1"
Option Explicit
'更改类名
Private Declare Function GlobalFindAtom Lib "kernel32" Alias "GlobalFindAtomA" (ByVal lpString As String) As Integer
Private Declare Function FindAtom Lib "kernel32" Alias "FindAtomA" (ByVal lpString As String) As Integer

Private Declare Function RegisterClass Lib "user32" Alias "RegisterClassA" (Class As WNDCLASS) As Long
Private Declare Function UnregisterClass Lib "user32" Alias "UnregisterClassA" (ByVal lpClassName As String, ByVal hInstance As Long) As Long
Private Declare Function GetClassInfoLong2 Lib "user32" Alias "GetClassInfoA" (ByVal hInstance As Long, ByVal lpClassName As String, lpWndClass As WNDCLASS2) As Long
Private Declare Function GetClassInfo Lib "user32" Alias "GetClassInfoA" (ByVal hInstance As Long, ByVal lpClassName As String, lpWndClass As WNDCLASS) As Long
Private Declare Function GetClassInfoLong Lib "user32" Alias "GetClassInfoA" (ByVal hInstance As Long, ByVal hAtom As Long, lpWndClass As WNDCLASS2) As Long

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Private Type WNDCLASS2
    style As Long
    lpfnwndproc As Long
    cbClsextra As Long
    cbWndExtra2 As Long
    hInstance As Long
    hIcon As Long
    hCursor As Long
    hbrBackground As Long
    lpszMenuName As Long
    lpszClassName As Long
End Type
Private Type WNDCLASS
    style As Long
    lpfnwndproc As Long
    cbClsextra As Long
    cbWndExtra2 As Long
    hInstance As Long
    hIcon As Long
    hCursor As Long
    hbrBackground As Long
    lpszMenuName As Long
    lpszClassName As String
End Type
Public Const MY_CLASS_NAME As String = "MyVBFormOK"

'MDI窗口
Private Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Private Declare Function DefMDIChildProc Lib "user32" Alias "DefMDIChildProcA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Private Const WM_CLOSE = &H10

Private Const WM_CHILDACTIVATE = &H22
Private Const WM_GETMINMAXINFO = &H24
Private Const WM_MENUCHAR = &H120
Private Const WM_MOVE = &H3
Private Const WM_SETFOCUS = &H7
Private Const WM_SIZE = &H5
Private Const WM_SYSCOMMAND = &H112

Private Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyA" (ByVal lpString1 As String, ByVal lpString2 As Long) As Long
Private Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Private Const GWL_WNDPROC = (-4)
Private Declare Function FindWindowEx Lib "user32" Alias "FindWindowExA" (ByVal hWnd1 As Long, ByVal hWnd2 As Long, ByVal lpsz1 As String, ByVal lpsz2 As String) As Long
Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String, ByVal cch As Long) As Long

Const WS_EX_MDICHILD = &H40

'测试用
Public Declare Sub Dg Lib "D:\我的程序\CallTest\Debug\CallTest.dll" ()
Public Declare Sub DD Lib "D:\CallTest.dll" Alias "Dg" ()

'声明钩子类
Public g_oWCHooker As New CVBWindowCreationHooker

'窗口创建信息结构
Public Type tCWSTRUCT
    dwExStyle As Long
    lpClassName As Long
    lpWindowName As Long
    dwStyle As Long
    x As Long
    y As Long
    nWidth As Long
    nHeight As Long
    hWndParent As Long
    hMenu As Long
    hInstance As Long
    lpParam As Long
End Type

Private Const WS_EX_TOOLWINDOW = &H80&
Private Const WS_EX_TOPMOST = &H8&

Public g_lDefaultWindowProc As Long

'要设为MDI子窗口的目标窗体的标题，在这里Form1的标题为frm
Const TARGET_WINDOW_CAPTION As String = "frmTargetWindow"

Public Function UnRegClass() As Boolean
    UnRegClass = (UnregisterClass(MY_CLASS_NAME, App.hInstance) <> 0)
End Function

Public Sub WndHookProc_BeforeCreation(lpCWStruct As tCWSTRUCT)
    With g_oWCHooker
        With lpCWStruct
            '如果不目标窗口则退出此过程
            If InStr(StrFromPtr(.lpWindowName), TARGET_WINDOW_CAPTION) = 0 Then Exit Sub
            
            '----------------- 更改窗体类名 -----------------
            
            Dim wc As WNDCLASS
            Dim wc2 As WNDCLASS2
            Dim lClassAtom  As Long
            
            
            If GetClassInfoLong(App.hInstance, (.lpClassName And &HFFFF&), wc2) Then
                If GetClassInfo(App.hInstance, MY_CLASS_NAME, wc) Then
                    Call UnRegClass
                End If
                
                With wc2
                    wc.cbClsextra = .cbClsextra
                    wc.cbWndExtra2 = .cbWndExtra2
                    wc.hbrBackground = .hbrBackground
                    wc.hCursor = .hCursor
                    wc.hIcon = .hIcon
                    wc.hInstance = .hInstance
                    wc.lpfnwndproc = .lpfnwndproc
                    wc.lpszMenuName = .lpszMenuName
                    wc.style = .style
                    wc.lpszClassName = MY_CLASS_NAME
                End With
                
                lClassAtom = RegisterClass(wc)
                
                lClassAtom = (lClassAtom And &HFFFF&)
                
                If lClassAtom Then
                    .lpClassName = lClassAtom
                End If
            End If
            
            
            '----------------- 将普通窗体更改为MDI子窗体 -----------------
        
            '设定窗口的父窗口及相应的风格
            Dim hMDIForm As Long
            Dim hMDIClient As Long
            '获得MDI父窗体句柄
            hMDIForm = MDIForm1.hwnd
            '查找MDIClient窗口
            hMDIClient = FindWindowEx(hMDIForm, 0&, "MDIClient", "")
            '将当前窗体的父窗体设为MDIClient
            .hWndParent = hMDIClient
            '设定窗口的风格为MDI子窗体的风格
            .dwExStyle = .dwExStyle Or WS_EX_MDICHILD
        End With
    End With
End Sub

Public Sub WndHookProc_AfterCreation(ByVal hNewWnd As Long)
    '如果不目标窗口则退出此过程
    If InStr(WndText(hNewWnd), TARGET_WINDOW_CAPTION) = 0 Then Exit Sub
    
    '通过更换窗口的子类处理过程,实现MDI子窗体的行为
    g_lDefaultWindowProc = SetWindowLong(hNewWnd, GWL_WNDPROC, AddressOf WindowProc)
End Sub

'我们的窗体过程
Public Function WindowProc(ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    Select Case wMsg
    '这些消息是MDI子窗体必须处理的
    Case WM_CHILDACTIVATE, WM_GETMINMAXINFO, WM_MENUCHAR, WM_MOVE, WM_SETFOCUS, WM_SYSCOMMAND, WM_SIZE
        WindowProc = DefMDIChildProc(hwnd, wMsg, wParam, lParam)
    '这个消息得两个过程一起处理，否则你的菜单就……
    Case WM_CLOSE
        WindowProc = DefMDIChildProc(hwnd, wMsg, wParam, lParam)
        WindowProc = CallWindowProc(g_lDefaultWindowProc, hwnd, wMsg, wParam, lParam)
        '其他消息传回给原来的窗口过程
    Case Else
        WindowProc = CallWindowProc(g_lDefaultWindowProc, hwnd, wMsg, wParam, lParam)
    End Select
End Function

'获得目标窗口的标题
Public Function WndText(ByVal hWndTarget As Long) As String
    On Error Resume Next
    Dim l As Long
    WndText = String(255, Chr(0))
    l = GetWindowText(hWndTarget, WndText, Len(WndText))
    WndText = Left(WndText, l)
End Function

'从指针获得ANSI字符串
Public Function StrFromPtr(ByVal lpString As Long) As String
    StrFromPtr = String(255, Chr(0))
    lstrcpy StrFromPtr, lpString
    StrFromPtr = Left(StrFromPtr, InStr(StrFromPtr, Chr(0)) - 1)
End Function

