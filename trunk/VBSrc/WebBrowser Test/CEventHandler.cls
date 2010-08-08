VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CEventHandler"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Dim oObject As Object
Dim sMethod As String
Dim bInstantiated As Boolean

Private Sub Class_Initialize()
 bInstantiated = False
End Sub

Public Sub SetHandler(oInObject As Object, sInMethod As String)

    Set oObject = oInObject
    sMethod = sInMethod
    bInstantiated = True
  
End Sub

Public Sub My_Default_Method()
Attribute My_Default_Method.VB_UserMemId = 0

    If bInstantiated Then
        CallByName oObject, sMethod, VbMethod
    End If
  
End Sub

