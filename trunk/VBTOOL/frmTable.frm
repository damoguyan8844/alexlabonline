VERSION 5.00
Begin VB.Form frmTable 
   Caption         =   "Table Test"
   ClientHeight    =   7095
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11070
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   7095
   ScaleWidth      =   11070
   WindowState     =   2  'Maximized
   Begin VB.CommandButton Command4 
      Caption         =   "ADO Connect Excel"
      Height          =   615
      Left            =   6840
      TabIndex        =   3
      Top             =   1440
      Width           =   2295
   End
   Begin VB.CommandButton Command3 
      Caption         =   "FireBalanceSheet"
      Height          =   735
      Left            =   6840
      TabIndex        =   2
      Top             =   240
      Width           =   2295
   End
   Begin VB.CommandButton Command2 
      Caption         =   "LoadTable"
      Height          =   615
      Left            =   720
      TabIndex        =   1
      Top             =   1320
      Width           =   2535
   End
   Begin VB.CommandButton Command1 
      Caption         =   "CreateTable"
      Height          =   615
      Left            =   720
      TabIndex        =   0
      Top             =   360
      Width           =   2535
   End
End
Attribute VB_Name = "frmTable"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()

Dim objMgr As New TTABLELib.TableManager
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
Dim objMgr As New TTABLELib.TableManager
objMgr.DBFile = "C:\SQLTest.db"

Dim objCols As New TableColumns
objCols.LoadByTableName "User"

objMgr.RemoveTableColumn "User", "Name"

End Sub

Private Sub Command3_Click()
Dim objFir As New TAnalysis.CPortfolioHelper

Dim id As Long
id = objFir.UploadDCMFIREBalanceSheet(1, #6/30/2010#, "C:\FIRE_Balance_Sheet.xls")

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
