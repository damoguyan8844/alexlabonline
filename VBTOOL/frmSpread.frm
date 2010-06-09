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
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   615
      Left            =   600
      TabIndex        =   3
      Top             =   3720
      Width           =   1815
   End
   Begin VB.TextBox txtOutput 
      Height          =   375
      Left            =   600
      TabIndex        =   2
      Text            =   "C:\Spread20100331.txt"
      Top             =   1320
      Width           =   3375
   End
   Begin VB.TextBox txtPortfolioFile 
      Height          =   285
      Left            =   600
      TabIndex        =   1
      Text            =   "C:\CorpBond20100331.xml"
      Top             =   720
      Width           =   3375
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Calibrate"
      Height          =   615
      Left            =   600
      TabIndex        =   0
      Top             =   2640
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
objPF.LoadXMLFile Me.txtPortfolioFile

Dim objSecs As Securities
Set objSecs = objPF.GetAllSecurities

Dim obj As New SpreadCal
Set obj.valSec = objSecs

Dim et As ERROR_TYPE
et = obj.calibrate(#3/31/2010#, "All")

If et <> ERROR_NULL Then
    MsgBox "Error:" & CStr(et)
End If

obj.saveSpreadToFile Me.txtOutput
'obj.saveSpread 10000

End Sub

Private Sub Command2_Click()

Dim content As String
content = ReadFromFile("C:\1.xml")

Dim objXML As New CGroupXML
objXML.LoadXML content

MsgBox CStr(objXML.IsItemEnable("Test", 1, content))

End Sub
