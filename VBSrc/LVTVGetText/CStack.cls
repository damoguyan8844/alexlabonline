VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CStack"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Private m_colStack As Collection
Public Function Push(v As Variant) As Boolean
    On Error Resume Next
    m_colStack.Add v
    Push = (Err.Number = 0)
End Function

Public Function Pop(v As Variant) As Boolean
    On Error Resume Next
    With m_colStack
        v = .Item(.Count)
        .Remove .Count
        Pop = (Err.Number = 0) And (.Count > 0)
    End With
End Function

Private Sub Class_Initialize()
    Set m_colStack = New Collection
End Sub

Private Sub Class_Terminate()
    Set m_colStack = Nothing
End Sub
