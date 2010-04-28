VERSION 5.00
Begin VB.Form frmSpread 
   Caption         =   "Form1"
   ClientHeight    =   5700
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11895
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   5700
   ScaleWidth      =   11895
   WindowState     =   2  'Maximized
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   600
      TabIndex        =   0
      Top             =   1080
      Width           =   1815
   End
End
Attribute VB_Name = "frmSpread"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()

Dim objPF As New Portfolio
objPF.LoadXMLFile "C:\CorpBond20100331.xml"

Dim objSecs As Securities
Set objSecs = objPF.GetAllSecurities

Dim obj As New SpreadCal
Set obj.valSec = objSecs

Dim et As ERROR_TYPE
et = obj.calibrate(#3/31/2010#, "All")

If et <> ERROR_NULL Then
    MsgBox "Error:" & CStr(et)
End If

obj.saveSpreadToFile "C:\Spread20100331.txt"
obj.saveSpread 10000

End Sub

