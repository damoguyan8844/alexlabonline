Attribute VB_Name = "Module1"
Option Explicit
Public Declare Function GlobalAddAtom Lib "kernel32" Alias "GlobalAddAtomA" (ByVal lpString As String) As Integer
Public Declare Function GlobalDeleteAtom Lib "kernel32" (ByVal nAtom As Integer) As Integer
Public Declare Function GlobalFindAtom Lib "kernel32" Alias "GlobalFindAtomA" (ByVal lpString As String) As Integer

Public Const MYAPP_SIGNAL_STRING = "x9832ks8x912ka0zxlk1230zxasdk321xz"
Public g_iAtom  As Integer
Sub Main()
    If App.PrevInstance Then
        If Not SetSignal Then
            'say sth...
            MsgBox "moo..."
        End If
        End
    End If
    Form1.Show
End Sub

Public Function SetSignal() As Boolean
    g_iAtom = GlobalAddAtom(MYAPP_SIGNAL_STRING)
    SetSignal = (g_iAtom <> 0)
End Function

Public Function GetSignal() As Boolean
    g_iAtom = GlobalFindAtom(MYAPP_SIGNAL_STRING)
    If g_iAtom <> 0 Then
        GetSignal = True
        GlobalDeleteAtom g_iAtom
    Else
        GetSignal = False
    End If
End Function
