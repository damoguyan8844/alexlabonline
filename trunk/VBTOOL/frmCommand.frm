VERSION 5.00
Begin VB.Form frmCommand 
   Caption         =   "Net Command"
   ClientHeight    =   4395
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10710
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   4395
   ScaleWidth      =   10710
   WindowState     =   2  'Maximized
   Begin VB.CommandButton Command7 
      Caption         =   "ResetQueueCommand"
      Height          =   495
      Left            =   8160
      TabIndex        =   11
      Top             =   3600
      Width           =   1575
   End
   Begin VB.CommandButton Command6 
      Caption         =   "CancelCommand"
      Height          =   495
      Left            =   8160
      TabIndex        =   10
      Top             =   2880
      Width           =   1575
   End
   Begin VB.CommandButton Command5 
      Caption         =   "QueueCommand"
      Height          =   495
      Left            =   8160
      TabIndex        =   9
      Top             =   2280
      Width           =   1575
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Process Command"
      Height          =   495
      Left            =   8160
      TabIndex        =   8
      Top             =   1680
      Width           =   1575
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   10000
      Left            =   240
      Top             =   1440
   End
   Begin VB.CommandButton Command3 
      Caption         =   "GetAvailablePCNest"
      Height          =   375
      Left            =   8160
      TabIndex        =   7
      Top             =   1200
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "GetQueueInput"
      Height          =   375
      Left            =   8160
      TabIndex        =   6
      Top             =   720
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "GetQueueResult"
      Height          =   375
      Left            =   8160
      TabIndex        =   5
      Top             =   240
      Width           =   1575
   End
   Begin VB.TextBox txtResult 
      Height          =   2775
      Left            =   1320
      MultiLine       =   -1  'True
      TabIndex        =   4
      Top             =   1320
      Width           =   5535
   End
   Begin VB.TextBox txtIP 
      Height          =   375
      Left            =   5160
      TabIndex        =   3
      Text            =   "192.168.0.174"
      Top             =   360
      Width           =   1695
   End
   Begin VB.TextBox txtMessageID 
      Height          =   375
      Left            =   1320
      TabIndex        =   0
      Text            =   "0"
      Top             =   360
      Width           =   1575
   End
   Begin VB.Label Label2 
      Caption         =   "MQServer:"
      Height          =   375
      Left            =   3960
      TabIndex        =   2
      Top             =   360
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "Message ID:"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   1095
   End
End
Attribute VB_Name = "frmCommand"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Declare Function ProcessCommand Lib "G:\THC\C0702\out\Plugins\tpl_web.dll" (ByVal commandText As String, ByVal inStr As String, ByVal hStopEvent As Long) As String
Private index As Long
Private Sub Command1_Click()
Dim cmdSender As New CommandSender, ssResult As Variant
    
Dim status
status = cmdSender.GetQueueResult(CLng(Me.txtMessageID), ssResult, Me.txtIP)

End Sub

Private Sub Command2_Click()

Dim cmdSender As New TActiveMQLib.CommandSender, ssResult As Variant
    
Dim status
status = cmdSender.GetQueueInput(Me.txtMessageID, ssResult, Me.txtIP)

End Sub


Private Sub Command3_Click()
    
    
Dim cmdSender As New CommandSender, ssResult As Variant
    
Dim status As Variant
status = cmdSender.GetAvailablePCList("PortfolioHelper", 3) & vbLf

If IsArray(status) Then
    Me.txtResult.Text = "Count:" & CStr(UBound(status))
    For i = 0 To UBound(status)
        Me.txtResult.Text = Me.txtResult & vbLf & CStr(status(i))
    Next
End If

End Sub

Private Sub Command4_Click()
 Dim strTemp As String
 strTemp = ReadFromFile("C:\tplTest.xml")
 strTemp = ProcessCommand("", strTemp, 0)

 'strTemp = ReadFromFile("C:\Book1Code_Out.txt")
 'Dim objZip As New TZIPLib.Utility

'objZip.DecompressStringToFile strTemp, "C:\Book1Code_Out.xls"

'strTemp = objZip.ReadFileContent("C:\Book1Code.xls", True)

'Call OutPutToFile("C:\Book1Code_Out.txt", strTemp)
End Sub

Private Sub Command5_Click()
    Dim cmdSender As New TActiveMQLib.CommandSender
    
    index = index + 1
    
    Dim res As String
    res = cmdSender.QueueCommand(Me.txtMessageID, "Test Description " & CStr(index), Me.txtResult, 0, True)
    
    MsgBox CStr(res)
    
End Sub

Private Sub Command6_Click()
Dim cmdSender As New TActiveMQLib.CommandSender, ssResult As Variant
    
Dim status
status = cmdSender.CancelQueueCommand(Me.txtMessageID)

End Sub

Private Sub Command7_Click()
Dim cmdSender As New TActiveMQLib.CommandSender, ssResult As Variant
    
Dim status
status = cmdSender.ResetQueueCommand(Me.txtMessageID, ssResult)

End Sub

Private Sub Form_Load()
index = 0
End Sub

Private Sub Timer1_Timer()
    
Dim cmdSender As New CommandSender, ssResult As Variant
    
Dim status As Variant
status = cmdSender.GetAvailablePCList("PortfolioHelper", 3)

If IsArray(status) Then
    Me.txtResult.Text = Me.txtResult & "Time:" & Time() & "      Count:" & CStr(UBound(status) + 1) & vbCrLf
    For i = 0 To UBound(status)
        Me.txtResult.Text = Me.txtResult & "--------" & CStr(status(i)) & vbCrLf
    Next
Else
    Me.txtResult.Text = Me.txtResult & "Time:" & Time() & "      Count:" & CStr(0) & vbCrLf
End If

End Sub
