Attribute VB_Name = "Module3"
Option Explicit


Public Declare Function SetClassName Lib "D:\我的程序\CallTest\Debug\CallTest.dll" (ByVal hWnd1 As Long) As Long
Public Declare Function SetDebugToken Lib "D:\我的程序\CallTest\Debug\CallTest.dll" () As Long

Function SetClsName(ByVal lhWnd As Long) As Boolean
    Dim lAddr As Long
    Dim s As String * 25
    lAddr = SetClassName(lhWnd)
    'CopyMemory ByVal s, ByVal lAddr, Len(s)
    Debug.Print s
End Function



