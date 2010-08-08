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
'ģ �� ��: CVBIEWindows
'
'��    ��: Supergreenbean
'
'��������: ��������IE���ĵĴ���
'
'��    ��: 2007��03��02��
'
'ʹ��˵��:
'       1.��������
'           ��:
'               ����������
'               oIEWindows.DoSearch
'       2.�������д���
'           ��:
'               ���������д���
'                Dim hWnd As Variant
'                For Each hWnd In oIEWindows.ShellWindows
'                   Debug.Pring hWnd
'                Next
'***********************************************************************************************************************
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CVBIEWindows"

'=======================================================================================================================
'����Ϊ����¼�
'=======================================================================================================================
'=======================================================================================================================
'����Ϊ����¼�
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����ΪWindows API�������
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
'����ΪWindows API�������
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ���ڲ�˽�б���
'=======================================================================================================================
'���п��Է���IShellBrowser�ӿڵĴ��ھ������
Private m_colWindows As Collection

'���ڻص����̴���
Private m_abEnumWindowsProc() As Byte
Private m_abEnumChildProc() As Byte

'IE��������
Private Const IE_WINDOW_CLASSNAME = "Shell Embedding"
'=======================================================================================================================
'����Ϊ���ڲ�˽�б���
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ������
'=======================================================================================================================
'*********************************************
'*********************************************
'ö�ٺ������̡������̱���������з���������ǰ��
Public Sub EnumWindowsProc(ByVal hwnd As Long, ByVal lParam As Long, rtnValue As Long)
    Call ParseWindow(hwnd)
    Call EnumChildWindows(hwnd, VarPtr(m_abEnumChildProc(0)), 0)
    rtnValue = 1
End Sub

'ö�ٺ������̡������̱���������з���������ǰ�棬EnumWindowsProc���̵ĺ���
Public Sub EnumChildProc(ByVal hwnd As Long, ByVal lParam As Long, rtnValue As Long)
    Call ParseWindow(hwnd)
    rtnValue = 1
End Sub

'������ЧIE���ڵļ���
Public Property Get ShellWindows() As Collection
    Set ShellWindows = m_colWindows
End Property
'*********************************************
'*********************************************

'=======================================================================================================================
'����Ϊ������
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ�๫�к���
'=======================================================================================================================
'''''''''���������ܷ���IShellBrowser�ӿڵĴ���
'��������IE���ĵĴ���
Public Sub DoSearch()
    Set m_colWindows = New Collection
    
    Call EnumWindows(VarPtr(m_abEnumWindowsProc(0)), 0)
End Sub
'=======================================================================================================================
'����Ϊ�๫������
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ�ڲ�����
'=======================================================================================================================
'����Ŀ�괰���Ƿ���IShellBrowserָ��
'ͨ���˺����õ��Ĵ���Ҫ��������ĺ������ܵõ��Ĵ���������,����Ҳ������ʹĿ�괰�ڱ����Ļ���,�Ƽ�ʹ������ĺ���
'Private Function ParseWindow(ByVal hTargetWnd As Long) As Long
'    On Error Resume Next
'    If SendMessage(hTargetWnd, WM_GETSHELLBROWSER, 0, ByVal 0) <> 0 Then
'        Dim txt As String, cls As String
'        Call GetWindowTextAndClass(hTargetWnd, txt, cls)
'        m_colWindows.Add hTargetWnd, CStr(hTargetWnd)
'    End If
'End Function

'����Ŀ�괰���Ƿ�ΪIE�ں˴���
Private Function ParseWindow(ByVal hTargetWnd As Long) As Long
    On Error Resume Next
    Dim txt As String, cls As String
    Call GetWindowTextAndClass(hTargetWnd, txt, cls)
    If cls = IE_WINDOW_CLASSNAME Then
        m_colWindows.Add hTargetWnd, CStr(hTargetWnd)
    End If
End Function

'ȡ�ô��ڱ����Լ�����
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

'׼������
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

'16�����ַ�����Ϊ�ֽ�����
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

'�����ͱ�Ϊ16�����ַ���
Private Function LongToByteCodeStr(ByVal lValue As Long) As String
    Dim s As String
    s = Right$("00000000" & Hex(lValue), 8)
    LongToByteCodeStr = Mid$(s, 1, 2) & " " & LongToByteCodeStr
    LongToByteCodeStr = Mid$(s, 3, 2) & " " & LongToByteCodeStr
    LongToByteCodeStr = Mid$(s, 5, 2) & " " & LongToByteCodeStr
    LongToByteCodeStr = Mid$(s, 7, 2) & " " & LongToByteCodeStr
End Function
'=======================================================================================================================
'����Ϊ�ڲ�����
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ���ʼ�������ٹ���
'=======================================================================================================================
'���ʼ��
Private Sub Class_Initialize()
    Call PrepareCode
End Sub

'������
Private Sub Class_Terminate()
End Sub
'=======================================================================================================================
'����Ϊ���ʼ�������ٹ���
'=======================================================================================================================

