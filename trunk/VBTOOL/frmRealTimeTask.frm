VERSION 5.00
Begin VB.Form frmRealTimeTask 
   Caption         =   "Real Time Task Calculate"
   ClientHeight    =   5205
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11655
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   5205
   ScaleWidth      =   11655
   WindowState     =   2  'Maximized
   Begin VB.Frame fmDatabaseSettings 
      Caption         =   "Database Settings"
      Height          =   975
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   11415
      Begin VB.TextBox txtIP 
         Height          =   375
         Left            =   600
         TabIndex        =   8
         Text            =   "192.168.0.177"
         Top             =   360
         Width           =   1695
      End
      Begin VB.TextBox txtDatabase 
         Height          =   375
         Left            =   3360
         TabIndex        =   7
         Text            =   "C0702"
         Top             =   360
         Width           =   1215
      End
      Begin VB.TextBox txtLogin 
         Height          =   375
         Left            =   5880
         TabIndex        =   6
         Text            =   "sa"
         Top             =   360
         Width           =   1215
      End
      Begin VB.TextBox txtPassword 
         Height          =   375
         IMEMode         =   3  'DISABLE
         Left            =   8160
         PasswordChar    =   "*"
         TabIndex        =   5
         Text            =   "bj2008*"
         Top             =   360
         Width           =   975
      End
      Begin VB.CommandButton cmdConn 
         Caption         =   "Test"
         Height          =   375
         Left            =   9600
         TabIndex        =   4
         Top             =   360
         Width           =   1695
      End
      Begin VB.Label lblIP 
         Caption         =   "IP:"
         Height          =   375
         Left            =   240
         TabIndex        =   12
         Top             =   360
         Width           =   375
      End
      Begin VB.Label lblDatabase 
         Caption         =   "Database:"
         Height          =   375
         Left            =   2400
         TabIndex        =   11
         Top             =   360
         Width           =   855
      End
      Begin VB.Label lblLogin 
         Caption         =   "Login Name:"
         Height          =   375
         Left            =   4920
         TabIndex        =   10
         Top             =   360
         Width           =   1095
      End
      Begin VB.Label lblPassword 
         Caption         =   "Password:"
         Height          =   375
         Left            =   7320
         TabIndex        =   9
         Top             =   360
         Width           =   975
      End
   End
   Begin VB.CheckBox chkCalibrateOAS 
      Caption         =   "Is Calibrate OAS"
      Height          =   495
      Left            =   2040
      TabIndex        =   2
      Top             =   2280
      Width           =   1695
   End
   Begin VB.TextBox txtTaskID 
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Text            =   "0"
      Top             =   2280
      Width           =   1455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   855
      Left            =   240
      TabIndex        =   0
      Top             =   3000
      Width           =   2535
   End
End
Attribute VB_Name = "frmRealTimeTask"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public m_Sql As String
Public m_Conn As ADODB.Connection
Public m_rs As ADODB.Recordset

Private Declare Sub InitTheRealTimeCalc Lib "irrcalc.dll" (ByVal theDate As Date)
Private Declare Function SaveRltimeSecurityResults Lib "irrcalc.dll" (ByVal TaskId As Long, ByRef IDs As Variant, ByRef Prices As Variant, ByRef YTMs As Variant, ByRef oass As Variant) As Long
Private Declare Function SetPortfolioXml Lib "irrcalc.dll" (ByVal xml As String) As Boolean
Private Declare Sub SetInputPortfolio Lib "irrcalc.dll" (ByVal obj As Portfolio)
Private Declare Function Calculate Lib "irrcalc.dll" (ByVal newthread As Boolean, thrd As Long) As Long
Private Declare Function InitRltimeSecurity Lib "irrcalc.dll" (ByVal eDate As Date, ByVal pfXML As String) As Boolean
Private Declare Sub EnableCalcOAS Lib "irrcalc.dll" (ByVal yes As Boolean)


Private Sub cmdConn_Click()
    
    On Error GoTo eh
    
    Dim IsConn As Boolean
    Dim DBIP As String, DBLogin As String, DBPassword As String, DBName As String, ConnStr As String
    
    IsConn = False
    DBIP = txtIP.Text '"192.168.0.60"  '
    DBLogin = txtLogin.Text '"sa" '
    DBPassword = txtPassword.Text '"sa"  '
    DBName = txtDatabase.Text ' "ThcData" '
    
    ConnStr = "Provider=Sqloledb;User ID=" & DBLogin & "; Password=" & DBPassword & _
        "; Initial Catalog = " & DBName & "; Data Source=" & DBIP & ";"
    Set m_Conn = New ADODB.Connection
    m_Conn.ConnectionTimeout = 60
    m_Conn.Open ConnStr
    m_Conn.CommandTimeout = 1200
    IsConn = True
    
    MsgBox "Connection is OK"
    
    Exit Sub
eh:
    MsgBox "Connection Error: " & Err.Description
End Sub

Private Sub Command1_Click()
On Error GoTo eh:

    m_Sql = "SELECT * FROM RealTimeTask WHERE ID=" & CStr(Me.txtTaskID)
    
    Set m_rs = New ADODB.Recordset
    m_rs.Open m_Sql, m_Conn, adOpenKeyset
    
    If m_rs.EOF Then
        MsgBox "NO Task Found with specified ID"
        Exit Sub
    End If
    
    Dim s As String
    s = m_rs("pfXML")
    
    InitRltimeSecurity Date, s
    
    If Me.chkCalibrateOAS.Value = 1 Then
        EnableCalcOAS (True)
    Else
        EnableCalcOAS (False)
    End If
    
    
    Dim t As Long
    Dim res As Long
    res = Calculate(False, t)
    
    MsgBox "res:" & CStr(res)
    
    Dim id As Variant, pr As Variant, ytm As Variant, oass As Variant
    Dim tid As Long
    
    SaveRltimeSecurityResults tid, id, pr, ytm, oass
    
    'MsgBox id
    
    Exit Sub
eh:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub Form_Load()

End Sub

Private Sub Form_Unload(Cancel As Integer)

If Not m_Conn Is Nothing Then
    If m_Conn.State = adStateOpen Then m_Conn.Close
    Set m_Conn = Nothing
End If

End Sub
