VERSION 5.00
Object = "{0002E550-0000-0000-C000-000000000046}#1.1#0"; "OWC10.dll"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form frmTable 
   Caption         =   "Table Test"
   ClientHeight    =   12210
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13845
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   12210
   ScaleWidth      =   13845
   WindowState     =   2  'Maximized
   Begin VB.CommandButton Command9 
      Caption         =   "Check Cell"
      Height          =   615
      Left            =   16320
      TabIndex        =   11
      Top             =   120
      Width           =   1935
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Match Sheet"
      Height          =   615
      Left            =   13680
      TabIndex        =   10
      Top             =   120
      Width           =   2175
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Check Regular"
      Height          =   615
      Left            =   11880
      TabIndex        =   9
      Top             =   120
      Width           =   1575
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   13095
      Left            =   240
      TabIndex        =   6
      Top             =   960
      Width           =   18615
      _ExtentX        =   32835
      _ExtentY        =   23098
      _Version        =   393216
      Tab             =   1
      TabHeight       =   520
      TabCaption(0)   =   "Tab 0"
      TabPicture(0)   =   "frmTable.frx":0000
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "OTSOAS"
      Tab(0).ControlCount=   1
      TabCaption(1)   =   "Tab 1"
      TabPicture(1)   =   "frmTable.frx":001C
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "txtRegular"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).ControlCount=   1
      TabCaption(2)   =   "Tab 2"
      Tab(2).ControlEnabled=   0   'False
      Tab(2).ControlCount=   0
      Begin VB.TextBox txtRegular 
         BeginProperty Font 
            Name            =   "Fixedsys Excelsior 3.01"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   11535
         Left            =   360
         MultiLine       =   -1  'True
         ScrollBars      =   3  'Both
         TabIndex        =   8
         Text            =   "frmTable.frx":0038
         Top             =   720
         Width           =   17895
      End
      Begin OWC10.Spreadsheet OTSOAS 
         Height          =   11655
         Left            =   -74640
         OleObjectBlob   =   "frmTable.frx":07A8
         TabIndex        =   7
         Top             =   720
         Width           =   18015
      End
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Set Current Sheet"
      Height          =   615
      Left            =   9720
      TabIndex        =   5
      Top             =   120
      Width           =   1935
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Init Sheet"
      Height          =   615
      Left            =   7800
      TabIndex        =   4
      Top             =   120
      Width           =   1575
   End
   Begin VB.CommandButton Command4 
      Caption         =   "ADO Connect Excel"
      Height          =   615
      Left            =   5640
      TabIndex        =   3
      Top             =   120
      Width           =   1935
   End
   Begin VB.CommandButton Command3 
      Caption         =   "FireBalanceSheet"
      Height          =   615
      Left            =   3840
      TabIndex        =   2
      Top             =   120
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "LoadTable"
      Height          =   615
      Left            =   2160
      TabIndex        =   1
      Top             =   120
      Width           =   1455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "CreateTable"
      Height          =   615
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1575
   End
End
Attribute VB_Name = "frmTable"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private thisSheetName As String

Private Sub Command1_Click()

Dim objMgr As New ZTableLib.TableManager
objMgr.DBFile = "C:\SQLTest.db"

Dim objCol As New TableColumn
objCol.name = "Name"
objCol.Type = DATA_STRING

Dim objCols As New TableColumns
objCols.Add objCol

objMgr.AddTable "User", objCols

Dim objCol2 As New TableColumn
objCol2.name = "School"
objCol2.Type = DATA_STRING

objMgr.AddTableColumn "User", "School", DATA_STRING

objMgr.RemoveTableColumn "User", "Name"

End Sub

Private Sub Command2_Click()
Dim objMgr As New ZTableLib.TableManager
objMgr.DBFile = "C:\SQLTest.db"

Dim objCols As New TableColumns
objCols.LoadByTableName "User"

objMgr.RemoveTableColumn "User", "Name"

End Sub

Private Sub Command3_Click()
Dim objFir As New TAnalysis.CPortfolioHelper

Dim ID As Long
ID = objFir.UploadDCMFIREBalanceSheet(1, #6/30/2010#, "C:\FIRE_Balance_Sheet.xls")

End Sub

Private Sub Command4_Click()

    Dim excelFileName As String
    excelFileName = "C:\FIRE_Balance_Sheet.xls"
    
    Dim cn As ADODB.Connection
    Dim rsT As ADODB.Recordset
    Dim intTblCnt As Integer, intTblFlds As Integer
    Dim strTbl As String
    Dim rsC As ADODB.Recordset
    Dim intColCnt As Integer, intColFlds As Integer
    Dim strCol As String
    Dim t As Integer, c As Integer, f As Integer
    
    Set cn = New ADODB.Connection
    
    If Len(excelFileName) > 5 And LCase(Right(excelFileName, 5)) = ".xlsx" Then
    
        With cn
            .Provider = "Microsoft.Jet.OLEDB.4.0"
            .ConnectionString = "Data Source=" & excelFileName & _
        ";Extended Properties=Excel 12.0;"
            '.Provider = "MSDASQL"
            '.ConnectionString = "Driver={Microsoft Excel Driver (*.xls)};" & _
        "DBQ=" & App.Path & "\ExcelSrc.xls; "
            .CursorLocation = adUseClient
            .Open
        End With
        
    Else
    
        With cn
            .Provider = "Microsoft.Jet.OLEDB.4.0"
            .ConnectionString = "Data Source=" & excelFileName & _
        ";Extended Properties=Excel 8.0;"
            '.Provider = "MSDASQL"
            '.ConnectionString = "Driver={Microsoft Excel Driver (*.xls)};" & _
        "DBQ=" & App.Path & "\ExcelSrc.xls; "
            .CursorLocation = adUseClient
            .Open
        End With
        
    End If
    
    Set rsT = cn.OpenSchema(adSchemaTables)
    intTblCnt = rsT.RecordCount
    intTblFlds = rsT.Fields.Count
    
    Dim strTmp As String
    
    
    For t = 1 To intTblCnt
        
        Dim colTitles As Long
        Dim colValues As Long
  
        colTitles = -1
        colValues = -1
        
        strTbl = rsT.Fields("TABLE_NAME").value
        
        Set rsC = cn.OpenSchema(adSchemaColumns, Array(Empty, Empty, strTbl, Empty))
        intColCnt = rsC.RecordCount
        intColFlds = rsC.Fields.Count
        
        For c = 1 To intColCnt
            If Trim(rsC.Fields("COLUMN_NAME").value) <> "" Then
            
                strCol = LCase(Trim(rsC.Fields("COLUMN_NAME").value))
                
                If InStr(1, strCol, "balance sheet information") > 0 Then
                    colTitles = c
                    
                ElseIf InStr(1, strCol, "value") > 0 Then
                    
                   colValues = c
                End If
            End If
            rsC.MoveNext
        Next
        
        If colTitles > 0 And colValues > 0 Then
            
            Dim instID As Long
            Dim dtAsOf As Date
            instID = 1
            dtAsOf = #6/30/2010#
             
            Dim oFBSht As New TPORTFOLIOLib.FIREBalanceSheet, ret As Boolean, strVal As String
            ret = oFBSht.LoadByInstitutionID(instID, dtAsOf)
    
            Dim rsRows As New ADODB.Recordset
            rsRows.Open "Select * from [" & strTbl & "]", cn, adOpenStatic, adLockOptimistic, adCmdText
            
            Dim iRow As Long
            For iRow = 1 To rsRows.RecordCount
            
                strTmp = LCase(Trim(rsRows.Fields(colTitles - 1)))
                strVal = LCase(Trim(rsRows.Fields(colValues - 1)))
                Select Case strTmp
                    Case "loans":
                        If IsNumeric(strVal) Then oFBSht.Loans = CDbl(strVal)
                    Case "securities":
                        If IsNumeric(strVal) Then oFBSht.Securities = CDbl(strVal)
                    Case "total assets":
                        If IsNumeric(strVal) Then oFBSht.TotalAssets = CDbl(strVal)
                    Case "avg assets":
                        If IsNumeric(strVal) Then oFBSht.AvgAssets = CDbl(strVal)
                    Case "deposits":
                        If IsNumeric(strVal) Then oFBSht.Deposits = CDbl(strVal)
                    Case "borrowings":
                        If IsNumeric(strVal) Then oFBSht.Borrowings = CDbl(strVal)
                    Case "liabilities":
                        If IsNumeric(strVal) Then oFBSht.Liablities = CDbl(strVal)
                    Case "equity":
                        If IsNumeric(strVal) Then oFBSht.Equity = CDbl(strVal)
                    Case "unrealized loss/gain afs":
                        If IsNumeric(strVal) Then oFBSht.UnrealizedLossGainAFS = CDbl(strVal)
                    Case "loan to deposit ratio":
                        If IsNumeric(strVal) Then oFBSht.LoanToDepositRatio = CDbl(strVal)
                    Case "loan to asset ratio":
                        If IsNumeric(strVal) Then oFBSht.LoanToAssetRatio = CDbl(strVal)
                    Case "equity capital":
                        If IsNumeric(strVal) Then oFBSht.EquityCapital = CDbl(strVal)
                    Case "tier 1 capital ration":
                        If IsNumeric(strVal) Then oFBSht.Tier1CapitalRation = CDbl(strVal)
                    Case "tier 1 minimum":
                        If IsNumeric(strVal) Then oFBSht.Tier1Minimum = CDbl(strVal)
                    Case "tier 1 leverage":
                        If IsNumeric(strVal) Then oFBSht.Tier1Leverage = CDbl(strVal)
                    Case "leverage minimum":
                        If IsNumeric(strVal) Then oFBSht.LeverageMinimum = CDbl(strVal)
                    Case "at annualized income":
                        If IsNumeric(strVal) Then oFBSht.ATAnnualizedIncome = CDbl(strVal)
                    Case "annual net interest income":
                        If IsNumeric(strVal) Then oFBSht.AnnualNetInterestIncome = CDbl(strVal)
                    Case "tax rate":
                        If IsNumeric(strVal) Then oFBSht.TaxRate = CDbl(strVal)
                    Case "roa":
                        If IsNumeric(strVal) Then oFBSht.ROA = CDbl(strVal)
                    Case "roe":
                        If IsNumeric(strVal) Then oFBSht.ROE = CDbl(strVal)
                    Case "net interest margin/spread(2)":
                        If IsNumeric(strVal) Then oFBSht.NetInterestMarginSpread2 = CDbl(strVal)
                End Select
                
                rsRows.MoveNext
            Next
            
            rsRows.Close
            
            If ret Then
                oFBSht.Update
            Else
                oFBSht.Add
            End If
            'UploadDCMFIREBalanceSheet = oFBSht.PKId
        
        End If
       
  
        rsC.Close
        rsT.MoveNext
    Next
    rsT.Close
    cn.Close
    
End Sub


Private Sub Command5_Click()
  
'    Dim xlApp As Excel.Application, xlBook As Excel.Workbook, xlSht As Excel.Worksheet
'    Set xlApp = CreateObject("Excel.Application")
'    If xlApp Is Nothing Then
'        MsgBox "Create Excel.Application Error"
'        Exit Sub
'    End If
'    xlApp.Visible = False
'    'xlApp.DisplayAlerts = False
'
'    Set xlBook = xlApp.Workbooks.Open("C:\OTS OAS.xls", , True)
'    If xlBook Is Nothing Then
'        MsgBox "Open File Error"
'        xlApp.Quit
'        Set xlApp = Nothing
'        Exit Sub
'    End If
'
'    Set xlSht = xlBook.Worksheets(1)
'    Dim varContent As Variant
'    varContent = xlSht.UsedRange.Value2
'
'    xlApp.Quit
'    Set xlApp = Nothing
   Set MKDCurrentSheet = OTSOAS
   SetSheetAccessProc AddressOf MDKCurrentSheetAccess
   
    Dim regular As String
    regular = txtRegular.Text
    InitSheet thisSheetName, regular, Nothing
       
End Sub

Private Sub Command6_Click()

   'SetSheetAccessProc AddressOf SheetAccess
   Set MKDCurrentSheet = OTSOAS
   SetSheetAccessProc AddressOf MDKCurrentSheetAccess
End Sub

Private Sub Command7_Click()
    If CheckRegular(Me.txtRegular.Text) Then
        MsgBox "OK"
    Else
        MsgBox "Failed"
    End If
End Sub

Private Sub Command8_Click()
    If MatchSheet(thisSheetName) Then
        MsgBox "OK"
    Else
        MsgBox "Failed"
    End If
End Sub

Private Sub Command9_Click()
    If CheckCell(thisSheetName, OTSOAS.ActiveCell.row, OTSOAS.ActiveCell.Column) Then
        MsgBox "OK"
    Else
        MsgBox "Failed"
    End If
End Sub

Private Sub Form_Load()

    thisSheetName = "OTS OAS Sheet"
    MKDSheets.Add OTSOAS, thisSheetName
    
End Sub

Private Sub Text1_Change()

End Sub
