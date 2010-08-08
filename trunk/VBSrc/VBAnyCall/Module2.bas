Attribute VB_Name = "Module2"
Option Explicit
Public Declare Sub DD Lib "D:\calltest.dll" Alias "Dg" ()

Public Declare Sub Dg Lib "D:\我的程序\CallTest\Debug\CallTest.dll" ()
'Public Declare Sub Dg Lib "J:\User-C\CallTest\Debug\CallTest.dll" ()
Public Declare Function GetExceptionHandlerAddress Lib "D:\我的程序\CallTest\Debug\CallTest.dll" () As Long
'Public Declare Function GetExceptionHandlerAddress Lib "J:\User-C\CallTest\Debug\CallTest.dll" () As Long
Public Declare Function vbaOnError Lib "msvbvm60" Alias "__vbaOnError" (ByVal bErrMode As Byte) As Long

Public Function CallTest(a As Long) As Long
    CallTest = a + 10
End Function
