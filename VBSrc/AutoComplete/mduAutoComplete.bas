Attribute VB_Name = "mduAutoComplete"
'***********************************************************************************************************************
'
'ģ �� ��: mduAutoComplete
'
'��    ��: Supergreenbean
'
'��������: �Զ���AutoComplete����
'
'��    ��: 2006��09��
'
'����ģ��: 1. CVBPEFnLocator.cls
'          2. CMyAutoCompleteSource.cls
'          3. mduIEnumString.bas
'
'***********************************************************************************************************************

Option Explicit
Public Declare Function CoCreateInstance Lib "ole32.dll" (rclsid As Any, ByVal lpUnkown As Long, ByVal dwClsContext As Long, riid As Any, ppv As Any) As Long
Public Declare Function CLSIDFromString Lib "ole32.dll" (ByVal lpsz As Long, pclsid As VBGUID) As Long
Public Declare Function GetCurrentProcessId Lib "kernel32" () As Long
Public Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Public Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Public Declare Function VirtualProtectEx Lib "kernel32" (ByVal hProcess As Long, lAddress As Any, ByVal dwSize As Long, ByVal flNewProtect As Long, lpflOldProtect As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
Public Declare Function CoTaskMemAlloc Lib "ole32" (ByVal cb As Long) As Long
Public Declare Function lStrLen Lib "kernel32" Alias "lstrlenW" (ByVal lpString As Long) As Long
Public Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function GetProp Lib "user32" Alias "GetPropA" (ByVal hWnd As Long, ByVal lpString As String) As Long
Public Declare Function SetProp Lib "user32" Alias "SetPropA" (ByVal hWnd As Long, ByVal lpString As String, ByVal hData As Long) As Long
Public Declare Function RemoveProp Lib "user32" Alias "RemovePropA" (ByVal hWnd As Long, ByVal lpString As String) As Long
Public Declare Sub DebugBreak Lib "kernel32" ()
Public Const GWL_WNDPROC = (-4&)

'˵��Ŀǰ�����÷���������
Public Enum MYMSG_METHOD
    MYMSG_IACList_Expand = 1
    MYMSG_IEnumString_Next = 2
    MYMSG_IEnumString_Skip = 3
    MYMSG_IEnumString_Reset = 4
    MYMSG_IEnumString_Clone = 5
End Enum

'�Զ�����Ϣ
Public Const ACM_ACTION = &H4321

'���ڴ��ݷ��������Ľṹ��
Public Type PARAMS
    param1 As Long
    param2 As Long
    param3 As Long
End Type

'һЩ�õ��Ľӿں����GUID
Public CLSID_ACLHistory As VBGUID
Public CLSID_ACListISF As VBGUID
Public CLSID_ACLMRU As VBGUID
Public CLSID_IAutoComplete As VBGUID
Public CLSID_ACLMulti As VBGUID
Public IID_IAutoComplete As VBGUID
Public IID_IAutoComplete2 As VBGUID
Public IID_IACList As VBGUID
Public IID_IACList2 As VBGUID
Public IID_IObjMgr As VBGUID
Public IID_IUnknown As VBGUID

'���ڴ��ݷ��������Ľṹ��
Public g_tParams As PARAMS
'���ദ��Ķ���
Public g_hTargetWnd As Long

'************************************************************************************************
'�����滻 __vbaSetSystemError �������Ʒ���
'��Ҫ������д����
'************************************************************************************************
Public Sub MySetSystemError()

End Sub

'************************************************************************************************
'�� __vbaSetSystemError �����滻Ϊ�Լ��� MySetSystemError
'************************************************************************************************
Public Sub PatchSetSystemError()
    Dim oPEFnLocator As CVBPEFnLocator
    Set oPEFnLocator = New CVBPEFnLocator
    
    With oPEFnLocator
        .SetTargetTo -1
        .AutoRestore = False
        
        .LocateImportFunction GetModuleHandle(vbNullString), "msvbvm60.dll", "__vbaSetSystemError"
        .ImpReplace AddressOf MySetSystemError
    End With
    
    Set oPEFnLocator = Nothing
End Sub

Public Function MyWindowProc(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    If uMsg = ACM_ACTION Then
        With Form1.MySource
            Select Case wParam
                Case MYMSG_IACList_Expand
                    MyWindowProc = .DoIACList_Expand(g_tParams.param1)
                Case MYMSG_IEnumString_Clone
                    MyWindowProc = .DoIEnumString_Clone(g_tParams.param1)
                Case MYMSG_IEnumString_Next
                    MyWindowProc = .DoIEnumString_Next(g_tParams.param1, g_tParams.param2, g_tParams.param3)
                Case MYMSG_IEnumString_Reset
                    MyWindowProc = .DoIEnumString_Reset()
                Case MYMSG_IEnumString_Skip
                    MyWindowProc = .DoIEnumString_Skip(g_tParams.param1)
                Case Else
                    MyWindowProc = 0
            End Select
        End With
    Else
        MyWindowProc = CallWindowProc(WinPropBag_ProcAddress(hWnd, False), hWnd, uMsg, wParam, lParam)
    End If
End Function

'���ദ��������
Public Function WinPropBag_ProcAddress(ByVal hWnd As Long, ByVal fStoreValue As Boolean, Optional ByVal lProcAddress As Long = 0, Optional ByVal fRemoveProp As Boolean = False) As Long
    If fStoreValue Then
        '��������
        SetProp hWnd, "MY_WINPROP_PROCADDRESS", lProcAddress
    Else
        'ȡ������
        WinPropBag_ProcAddress = GetProp(hWnd, "MY_WINPROP_PROCADDRESS")
        If fRemoveProp Then
            'ɾ������
            RemoveProp hWnd, "MY_WINPROP_PROCADDRESS"
        End If
    End If
    
End Function

Public Sub Subclassing(ByVal hWndTarget As Long, Optional ByVal fUnsubclassing As Boolean = False)
    If fUnsubclassing Then
        SetWindowLong hWndTarget, GWL_WNDPROC, WinPropBag_ProcAddress(hWnd:=hWndTarget, fStoreValue:=False, fRemoveProp:=True)
    Else
        WinPropBag_ProcAddress hWndTarget, True, SetWindowLong(hWndTarget, GWL_WNDPROC, AddressOf MyWindowProc)
    End If
End Sub


