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
       
    '�����㴰�ھ��
    hWndFirst = GetWindow(hWndMe, GW_HWNDFIRST)
    '�����ײ㴰�ھ��
    hWndLast = GetWindow(hWndMe, GW_HWNDLAST)
    '���ǰ1�����ھ��
    hWndPrev = GetWindow(hWndMe, GW_HWNDPREV)
    '��ú�1�����ھ��
    hWndNext = GetWindow(hWndMe, GW_HWNDNEXT)
       
    Select Case KeyCode
    '�������ϻ�����ļ�ͷ��
    Case vbKeyUp, vbKeyLeft
        hWndTarget = IIf(hWndNext, hWndNext, hWndFirst)
    '�������»����ҵļ�ͷ��
    Case vbKeyDown, vbKeyRight
        hWndTarget = IIf(hWndPrev, hWndPrev, hWndLast)
    End Select
    
    '���¸�Ӧ�û�ý���Ĵ������ý���
    SetWindowFocus hWndTarget
End Sub
