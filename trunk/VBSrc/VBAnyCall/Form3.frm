VERSION 5.00
Begin VB.Form Form3 
   Caption         =   "Form3"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form3"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   525
      Left            =   1500
      TabIndex        =   0
      Top             =   1350
      Width           =   1575
   End
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    'һ��Ҫ����on error resume next���,�����������ֹ
    On Error Resume Next
    Dim m_oAnyCall As New CVBAnyCall
    Dim i As Long
    With m_oAnyCall
        .ThroughVTable = True
        i = 123
        .IsStandardCall = True
        Debug.Print .CallApiByName("D:\�ҵĳ���\CallTest\Debug\CallTest.dll", "CallTestS", VarPtr(i))
        Debug.Print i
        .IsStandardCall = False
        Debug.Print .CallApiByName("D:\�ҵĳ���\CallTest\Debug\CallTest.dll", "CallTestC", VarPtr(i))
        Debug.Print i
        .IsStandardCall = True
        Call .CallApiByName("D:\�ҵĳ���\CallTest\Debug\CallTest.dll", "CallTestSub", VarPtr(i))
        Debug.Print i
    End With
    Set m_oAnyCall = Nothing
End Sub

