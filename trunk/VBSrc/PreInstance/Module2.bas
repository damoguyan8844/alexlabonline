Attribute VB_Name = "Module2"
Option Explicit
Private Declare Function GetWindowThreadProcessId Lib "User32" (ByVal hWnd As Long, lpdwProcessId As Long) As Long
Private Declare Function AttachThreadInput Lib "User32" (ByVal idAttach As Long, ByVal idAttachTo As Long, ByVal fAttach As Long) As Long
Private Declare Function GetForegroundWindow Lib "User32" () As Long
Private Declare Function SetForegroundWindow Lib "User32" (ByVal hWnd As Long) As Long
Private Declare Function IsIconic Lib "User32" (ByVal hWnd As Long) As Long
Private Declare Function ShowWindow Lib "User32" (ByVal hWnd As Long, ByVal nCmdShow As Long) As Long

Private Const SW_SHOW = 5
Private Const SW_RESTORE = 9


Public Function ForceForegroundWindow(ByVal hWnd As Long) As Boolean
   Dim ThreadID1 As Long
   Dim ThreadID2 As Long
   Dim nRet As Long

   If hWnd = GetForegroundWindow() Then
      ForceForegroundWindow = True
   Else
      ThreadID1 = GetWindowThreadProcessId(GetForegroundWindow, ByVal 0&)
      ThreadID2 = GetWindowThreadProcessId(hWnd, ByVal 0&)
      If ThreadID1 <> ThreadID2 Then
         Call AttachThreadInput(ThreadID1, ThreadID2, True)
         nRet = SetForegroundWindow(hWnd)
         Call AttachThreadInput(ThreadID1, ThreadID2, False)
      Else
         nRet = SetForegroundWindow(hWnd)
      End If
      If IsIconic(hWnd) Then
         Call ShowWindow(hWnd, SW_RESTORE)
      Else
         Call ShowWindow(hWnd, SW_SHOW)
      End If
      ForceForegroundWindow = CBool(nRet)
   End If
End Function

