VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
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
   Begin MSComCtl2.DTPicker dtEDate 
      Height          =   375
      Left            =   5520
      TabIndex        =   4
      Top             =   840
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   661
      _Version        =   393216
      Format          =   21299201
      CurrentDate     =   40339
   End
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
et = obj.calibrate(dtEDate.value, "All")

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
objXML.LoadXMLFile "C:\1.xml"

Dim value As Long
MsgBox CStr(objXML.IsItemEnable("AGENCY&MUNI", 112, content, value))

End Sub
