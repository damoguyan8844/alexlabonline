VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBIEWindows"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'***********************************************************************************************************************
'
'模 块 名: CVBIEWindows
'
'作    者: Supergreenbean
'
'功能描述: 搜索所有IE核心的窗口
'
'日    期: 2007年03月02日
'
'使用说明:
'       1.搜索窗口
'           例:
'               ※搜索窗口
'               oIEWindows.DoSearch
'       2.遍历所有窗口
'           例:
'               ※遍历所有窗口
'                Dim hWnd As Variant
'                For Each hWnd In oIEWindows.ShellWindows
'                   Debug.Pring hWnd
'                Next
'***********************************************************************************************************************
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBIEWindows"

'=======================================================================================================================
'以下为类的事件
'=======================================================================================================================
'=======================================================================================================================
'以下为类的事件
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'以下为Windows API相关声明
'=======================================================================================================================
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Long, ByVal Length As Long)
Private Declare Function EnumWindows Lib "user32.dll" (ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Private Declare Function EnumChildWindows Lib "user32.dll" (ByVal hWndParent As Long, ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Private Declare Function GetClassName Lib "user32.dll" Alias "GetClassNameA" (ByVal hwnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long) As Long
Private Declare Function GetWindowText Lib "user32.dll" Alias "GetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
Private Declare Function GetWindowTextLength Lib "user32.dll" Alias "GetWindowTextLengthA" (ByVal hwnd As Long) As Long

Private Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long
Private Declare Function PostMessage Lib "user32" Alias "PostMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long

Private Const WM_USER = &H400
Private Const WM_GETSHELLBROWSER = WM_USER + 7
'=======================================================================================================================
'以上为Windows API相关声明
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'以下为类内部私有变量
'=======================================================================================================================
'所有可以返回IShellBrowser接口的窗口句柄集合
Private m_colWindows As Collection

'窗口回调过程代码
Private m_abEnumWindowsProc() As Byte
Private m_abEnumChildProc() As Byte

'IE窗口类名
Private Const IE_WINDOW_CLASSNAME = "Shell Embedding"
'=======================================================================================================================
'以上为类内部私有变量
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'以下为类属性
'=======================================================================================================================
'*********************************************
'*********************************************
'枚举函数过程。本过程必须放在所有方法和属性前面
Public Sub EnumWindowsProc(ByVal hwnd As Long, ByVal lParam As Long, rtnValue As Long)
    Call ParseWindow(hwnd)
    Call EnumChildWindows(hwnd, VarPtr(m_abEnumChildProc(0)), 0)
    rtnValue = 1
End Sub

'枚举函数过程。本过程必须放在所有方法和属性前面，EnumWindowsProc过程的后面
Public Sub EnumChildProc(ByVal hwnd As Long, ByVal lParam As Long, rtnValue As Long)
    Call ParseWindow(hwnd)
    rtnValue = 1
End Sub

'所有有效IE窗口的集合
Public Property Get ShellWindows() As Collection
    Set ShellWindows = m_colWindows
End Property
'*********************************************
'*********************************************

'=======================================================================================================================
'以上为类属性
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'以下为类公有函数
'=======================================================================================================================
'''''''''查找所有能返回IShellBrowser接口的窗口
'查找所有IE核心的窗口
Public Sub DoSearch()
    Set m_colWindows = New Collection
    
    Call EnumWindows(VarPtr(m_abEnumWindowsProc(0)), 0)
End Sub
'=======================================================================================================================
'以上为类公共函数
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'以下为内部函数
'=======================================================================================================================
'测试目标窗口是否有IShellBrowser指针
'通过此函数得到的窗口要数比下面的函数所能得到的窗口数更多,但是也增加了使目标窗口崩溃的机会,推荐使用下面的函数
'Private Function ParseWindow(ByVal hTargetWnd As Long) As Long
'    On Error Resume Next
'    If SendMessage(hTargetWnd, WM_GETSHELLBROWSER, 0, ByVal 0) <> 0 Then
'        Dim txt As String, cls As String
'        Call GetWindowTextAndClass(hTargetWnd, txt, cls)
'        m_colWindows.Add hTargetWnd, CStr(hTargetWnd)
'    End If
'End Function

'测试目标窗口是否为IE内核窗口
Private Function ParseWindow(ByVal hTargetWnd As Long) As Long
    On Error Resume Next
    Dim txt As String, cls As String
    Call GetWindowTextAndClass(hTargetWnd, txt, cls)
    If cls = IE_WINDOW_CLASSNAME Then
        m_colWindows.Add hTargetWnd, CStr(hTargetWnd)
    End If
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

'准备代码
Private Function PrepareCode() As Boolean
    On Error GoTo Error_Handler
    
    Dim lpThis As Long
    Dim lpVTable As Long
    lpThis = ObjPtr(Me)
    CopyMemory lpVTable, ByVal lpThis, 4
    
    Dim sCodes As String
    sCodes = ""
    'int 3
    'sCodes = "CC "
    '
    'PUSH EBP
    'MOV EBP,ESP
    sCodes = sCodes & "55 8B EC "
    
    'LEA ECX,DWORD PTR SS:[EBP-4]
    'PUSH ECX
    'PUSH ECX
    'LEA ECX,DWORD PTR SS:[EBP+C]
    'PUSH DWORD PTR DS:[ECX]
    'LEA ECX,DWORD PTR SS:[EBP+8]
    'PUSH DWORD PTR DS:[ECX]
    sCodes = sCodes & "8D 4D FC 51 51 8D 4D 0C FF 31 8D 4D 08 FF 31 "
    'push lpThis
    sCodes = sCodes & "68 " & LongToByteCodeStr(lpThis)
    'call WndProc
    sCodes = sCodes & "FF 15 " & LongToByteCodeStr(lpVTable + &H1C)
    '8B 04 24
    'MOV EAX, [ESP]
    '8B E5
    'MOV ESP,EBP
    '5D
    'POP EBP
    sCodes = sCodes & "8B 04 24 8B E5 5D "
    'RET 8
    sCodes = sCodes & "C2 08 00"
    
    m_abEnumWindowsProc = ByteCodeStrToBin(sCodes)
    m_abEnumChildProc = ByteCodeStrToBin(sCodes)
    
    PrepareCode = True
    Exit Function
Error_Handler:
End Function

'16进制字符串变为字节数组
Private Function ByteCodeStrToBin(ByVal sByteCode As String) As Byte()
    Dim s() As String
    Dim b() As Byte
    Dim i As Long
    
    s = Split(Trim(sByteCode), " ")
    
    If UBound(s) >= 0 Then
        ReDim b(UBound(s))
    End If
    
    For i = 0 To UBound(s)
        b(i) = CByte("&h" & s(i))
    Next
    ByteCodeStrToBin = b
End Function

'长整型变为16进制字符串
Private Function LongToByteCodeStr(ByVal lValue As Long) As String
    Dim s As String
    s = Right$("00000000" & Hex(lValue), 8)
    LongToByteCodeStr = Mid$(s, 1, 2) & " " & LongToByteCodeStr
    LongToByteCodeStr = Mid$(s, 3, 2) & " " & LongToByteCodeStr
    LongToByteCodeStr = Mid$(s, 5, 2) & " " & LongToByteCodeStr
    LongToByteCodeStr = Mid$(s, 7, 2) & " " & LongToByteCodeStr
End Function
'=======================================================================================================================
'以上为内部函数
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'以下为类初始化与销毁过程
'=======================================================================================================================
'类初始化
Private Sub Class_Initialize()
    Call PrepareCode
End Sub

'类销毁
Private Sub Class_Terminate()
End Sub
'=======================================================================================================================
'以上为类初始化与销毁过程
'=======================================================================================================================


