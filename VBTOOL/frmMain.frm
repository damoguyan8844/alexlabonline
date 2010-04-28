VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   Caption         =   "TOOL(GQH) Main"
   ClientHeight    =   6180
   ClientLeft      =   165
   ClientTop       =   555
   ClientWidth     =   11880
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   6180
   ScaleWidth      =   11880
   WindowState     =   2  'Maximized
   Begin MSComDlg.CommonDialog dl 
      Left            =   9600
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Image img1 
      Height          =   2115
      Left            =   0
      Picture         =   "frmMain.frx":0000
      Stretch         =   -1  'True
      Top             =   0
      Width           =   2985
   End
   Begin VB.Menu mnupf 
      Caption         =   "Pathfile && Portfolio"
      Begin VB.Menu mnuGetInputSecsAndBuildPF 
         Caption         =   "Get inputsecs from pathfile && Build Portfolio"
      End
      Begin VB.Menu mnustrait2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuReadPFtoDB 
         Caption         =   "Read Portfolio to DB"
      End
      Begin VB.Menu mnusplit5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuDEBUG 
         Caption         =   "Debug"
      End
   End
   Begin VB.Menu mnuAddin 
      Caption         =   "Add-ins"
      Begin VB.Menu mnuSetUpXLA 
         Caption         =   "Setup Addin(xla) for Excel"
      End
      Begin VB.Menu mnuRemoveXLA 
         Caption         =   "Remove Addin(xla) about Excel"
      End
      Begin VB.Menu mnustrait1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuAbout 
         Caption         =   "About"
      End
   End
   Begin VB.Menu mnuYTS 
      Caption         =   "Yield Term Structure"
      Begin VB.Menu mnuflatyts 
         Caption         =   "Flat YTS"
      End
      Begin VB.Menu mnuspotyts 
         Caption         =   "SpotRates YTS"
      End
      Begin VB.Menu mnuparyts 
         Caption         =   "ParRates YTS"
      End
      Begin VB.Menu mnubondyts 
         Caption         =   "OnRunBonds YTS"
      End
   End
   Begin VB.Menu mnuvol 
      Caption         =   "Vol Term Structure"
      Begin VB.Menu mnuFlatvol 
         Caption         =   "Flat VolTS"
      End
      Begin VB.Menu mnusinvol 
         Caption         =   "SinPair VolTS"
      End
   End
   Begin VB.Menu mnumarket 
      Caption         =   "Market Enviroment"
      Begin VB.Menu mnupmmrates 
         Caption         =   "set PMM Rates"
      End
      Begin VB.Menu mnuindexrates 
         Caption         =   "set Index Rates"
      End
      Begin VB.Menu mnuexchangerates 
         Caption         =   "set Exchange Rates"
      End
      Begin VB.Menu mnumarketsplit 
         Caption         =   "-"
      End
      Begin VB.Menu mnusetYTS 
         Caption         =   "set Yield Term Structure"
      End
      Begin VB.Menu mnusetvolts 
         Caption         =   "set Vol Term Structure"
      End
      Begin VB.Menu mnusetCorps 
         Caption         =   "set Corp Spreads"
      End
      Begin VB.Menu mnusetCorpsEx 
         Caption         =   "set Corp Spreads EX"
      End
      Begin VB.Menu mnusplit332 
         Caption         =   "-"
      End
      Begin VB.Menu mnuaddbasketbond 
         Caption         =   "Add BasketDeliveryBond"
      End
      Begin VB.Menu mnumodelcoef 
         Caption         =   "Initialize Model Coef"
      End
   End
   Begin VB.Menu mnuOther 
      Caption         =   "Other"
      Begin VB.Menu mnumiscfun 
         Caption         =   "MISC FUN"
      End
      Begin VB.Menu mnudispweb 
         Caption         =   "Display WEB conten"
      End
      Begin VB.Menu mnutestthcapi 
         Caption         =   "Test THCAPI.dll"
      End
   End
   Begin VB.Menu mnuReport 
      Caption         =   "REPORT"
      Begin VB.Menu mnuIRR 
         Caption         =   "IRR Report"
      End
   End
   Begin VB.Menu mnuSpread 
      Caption         =   "Spread"
      Begin VB.Menu mnuCalibrateSpread 
         Caption         =   "CalibrateSpread"
      End
   End
   Begin VB.Menu mnuexit 
      Caption         =   "Exit"
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private m_analyzer As New PAnalyzer
Private m_parser As New PParser

Private Sub Form_Load()
img1.Width = Me.Width
img1.Height = Me.Height
End Sub

Private Sub Form_Resize()
img1.Width = Me.Width
img1.Height = Me.Height
End Sub

Private Sub mnuCalibrateSpread_Click()
  If Not FormExist("frmSpread") Then
        Dim frm As New frmSpread
        frm.Height = Me.Height
        frm.Width = Me.Width
        frm.Top = Me.Top
        frm.Left = Me.Left
        
        frm.Show
        frm.Caption = GetFormCaption("frmSpread")
    End If
End Sub

Private Sub mnuexit_Click()
Unload Me
End Sub

Private Function getParentPath(str As String) As String
If str <> "" Then
        Dim idx
        idx = InStrRev(str, "\")
        If idx > 0 Then
            getParentPath = Left(str, idx)
        Else
            getParentPath = str
        End If
    Else
        getParentPath = ""
    End If
End Function

Private Sub mnuIRR_Click()

Dim CRR
Set CRR = CreateObject("CreateCRReport.CreateReport")
CRR.DoReport 1

End Sub

Private Sub mnumiscfun_Click()
'        Dim str As String
'        Dim path As String
'        str = "d:"
'        path = getParentPath(str)

'Dim a As Long
'a = InStrRev("c:\etl\a.zip", "\")
'Dim b As String
'b = Left("c:\etl\a.zip", a)


'    Dim IniPath As String
'    GetModuleFileName 0, IniPath, 1024
'    IniPath = IniPath + "TNetSvr.ini"
'
'    Dim filepath As String
'    GetPrivateProfileString "TNetSvr", "ETLFolders", 1024, filepath
'
'    filepath = filepath + Content
'    Name = SaveFile(Name, filepath, filetype)
End Sub


Private Sub mnuReadPFtoDB_Click()
On Error GoTo eh:
    
    Dim dlg As MSComDlg.CommonDialog
    Set dlg = Me.dl
    dlg.filename = ""
    dlg.Flags = 0
    dlg.DefaultExt = "xls"
    dlg.Filter = "Path File & QRM File (*.xls; *.xlsx; *.csv)|*.xls;*.xlsx;*.csv|Excel Files (*.xls; *.xlsx)|*.xls;*.xlsx|CSV File (*.csv)|*.csv|XML File (*.xml)|*.xml"
    dlg.CancelError = True
    On Error GoTo Cancel:
    dlg.ShowOpen
    On Error GoTo eh:

    If dlg.filename = "" Then Exit Sub

    Dim opf As New Portfolio
    opf.Add
    Dim pfid As Long
    pfid = opf.ID
    
'    pfid = 4137410
    
'    Dim bstr As String
'    bstr = opf.GetCMRBSTR(#9/30/2009#)
'    GetCMRExcelFileForDownloadByPortolio "d:\THC\C0702\out\ETLTEMP\downloadcmrd.xlstemplate.xls", "d:\THC\C0702\out\ETLTEMP\ETLCMRXLSDetail_temp.xls", opf, #12/31/2009#
'    Dim zip As TZIPLib.Utility
'    Set zip = New TZIPLib.Utility
'    Dim content As String
'    content = zip.ReadFileContent("d:\THC\C0702\out\ETLTEMP\ETLCMRXLSDetail_temp.xls", True)

'    Dim eInputsecs As InputSecs
'    Set eInputsecs = m_parser.getInputSecsByRootPortfolio(opf)
'    eInputsecs.ExportAsFile "c:\eout3.csv", False, True
'
    pfid = m_parser.readPortfolioToDB(dlg.filename, pfid)

'    Dim tag As Long
'    tag = opf.GetPortfolioTag
    
    Dim otemp As New Portfolio
    otemp.LoadID pfid, True, True
    
    otemp.SaveXMLFile App.Path & "\pf.xml"
    
Exit Sub
Cancel:
    Exit Sub
eh:
    MsgBox Err.Description
End Sub

Private Sub mnuGetInputSecsAndBuildPF_Click()
On Error GoTo eh:
    
    Dim dlg As MSComDlg.CommonDialog
    Set dlg = Me.dl
    dlg.filename = ""
    dlg.Flags = 0
    dlg.DefaultExt = "xls"
    dlg.Filter = "Path File & QRM File (*.xls; *.xlsx; *.csv)|*.xls;*.xlsx;*.csv|Excel Files (*.xls; *.xlsx)|*.xls;*.xlsx|CSV File (*.csv)|*.csv"
    dlg.CancelError = True
    On Error GoTo Cancel:
    dlg.ShowOpen
    On Error GoTo eh:

    If dlg.filename = "" Then Exit Sub
    

    Dim osecs As InputSecs
    Set osecs = m_analyzer.getInputSecsFromFile(dlg.filename)
    
'    dim tagsecs as InputSecs
'    Dim i As Long, j As Long
'    For i = 1 To 12
'        Dim objsecs As InputSecs
'        Set objsecs = osecs.Copy
'        Dim tempsecs As InputSecs
'        Set tempsecs = objsecs.AddTerm(i)
'        If Not tempsecs Is Nothing Then
'            For j = 1 To tempsecs.Count
'                tagsecs.Add tempsecs.Item(j)
'            Next
'        End If
'    Next
'    tagsecs.ExportAsFile "c:\addterm.csv", False
    
    Dim opf As Portfolio
    Set opf = m_parser.buildPortfolio(osecs)

'    Dim dbpf As New Portfolio
'    dbpf.LoadID 4048697, True, False
'    m_parser.MergeOTSPortfolio oPf, 4048697, evaldate

Exit Sub
Cancel:
    Exit Sub
eh:
    MsgBox Err.Description
End Sub

Private Sub mnuRemoveXLA_Click()
On Error GoTo Catch

''''create excel app
    Dim xlApp As Excel.Application
    Set xlApp = Nothing
    Set xlApp = GetExcelApp(True)
    xlApp.Visible = False
    
'''''the must one for set addin ,or will faild by err:Add method of addins class failed
    xlApp.Workbooks.Add
    
    Dim oAddin As Excel.AddIn
    Set oAddin = xlApp.AddIns("ThcTreasurer")
    If Not oAddin Is Nothing Then oAddin.Installed = False
    
    xlApp.Quit
    Set xlApp = Nothing
    
    MsgBox "Uninstall Success!"

Exit Sub
Catch:
    xlApp.Quit
    Set xlApp = Nothing
    MsgBox Err.Number & " -- " & Err.Description
End Sub

Private Sub mnuSetUpXLA_Click()
On Error GoTo Catch

''''create excel app
    Dim newCreat As Boolean, bookAdd As Boolean
    Dim xlApp As Excel.Application
    Set xlApp = Nothing

    Set xlApp = CreateObject("Excel.Application")
    xlApp.Visible = False
    xlApp.Workbooks.Add
    
'''''the must one for set addin ,or will fiald by err:Add method of addins class failed
'    xlApp.Workbooks.Add
    
    Dim strPath As String
    strPath = "d:\c0702\tcom2\out\"
    
    Dim oAddin As Excel.AddIn
    Set oAddin = xlApp.AddIns.Add(strPath & "ThcTreasurer.xla", True)
    oAddin.Installed = True
    
    xlApp.Workbooks.Close
    xlApp.Quit
    Set xlApp = Nothing
    
    
    MsgBox "Setup Success!"

Exit Sub
Catch:
    xlApp.Quit
    Set xlApp = Nothing
    MsgBox Err.Number & " -- " & Err.Description
End Sub

Private Sub mnuAbout_Click()
    MsgBox "excel addin install and uninstall"
End Sub

Private Sub mnuaddbasketbond_Click()

    'instance CBOTFutureDeliveryBonds object
    Dim deliverybonds As New CBOTFutureDeliveryBonds
    'instance MarketEnvironment object
    Dim oME As New MarketEnvironment
    'instance CBOTFutureDeliveryBond object and set properties value
    Dim deliverybond As New CBOTFutureDeliveryBond
    deliverybond.ConversionFactor = 0.921
    deliverybond.Coupon = 0.0175
    deliverybond.CusipNumber = "912828HU7"
    deliverybond.Issuance = 28
    deliverybond.IssueDate = #3/31/2008#
    deliverybond.MaturityDate = #3/31/2010#
    'add this deliverybond to MarketEnvironment
    oME.AddBasketDeliverableBond CBOT_INTERESTFUTURE_TU, #3/31/2008#, deliverybond
    'set another deliverybond
    Set deliverybond = New CBOTFutureDeliveryBond
    deliverybond.ConversionFactor = 0.9559
    deliverybond.Coupon = 0.03625
    deliverybond.CusipNumber = "912828DX5"
    deliverybond.Issuance = 14
    deliverybond.IssueDate = #6/15/2005#
    deliverybond.MaturityDate = #6/15/2010#
    'add this deliverybond to MarketEnvironment
    oME.AddBasketDeliverableBond CBOT_INTERESTFUTURE_TU, #6/30/2008#, deliverybond
    'remove thoes deliverybonds from MarketEnvironment when you donnot need anymore
    oME.ClearBasketDeliverableBond


End Sub

Private Sub mnubondyts_Click()
    
    Dim oYTS As IYieldTermStructure
    'two-dimensional array for issue date, maturity, coupon rate and prices.
    Dim bonds
    ReDim bonds(0 To 12, 0 To 3)
    bonds(0, 0) = #8/28/2008# '1mo bond
    bonds(0, 1) = #8/27/2009#
    bonds(0, 2) = 0
    bonds(0, 3) = 0.1445

    bonds(1, 0) = #5/7/2009# '3mo bond
    bonds(1, 1) = #11/5/2009#
    bonds(1, 2) = 0
    bonds(1, 3) = 0.1836

    bonds(2, 0) = #8/6/2009# '6mo bond
    bonds(2, 1) = #2/4/2010#
    bonds(2, 2) = 0
    bonds(2, 3) = 0.2813

    bonds(3, 0) = #9/2/2008# '1yr bond
    bonds(3, 1) = #8/31/2010#
    bonds(3, 2) = 0.02375
    bonds(3, 3) = 101.9023

    bonds(4, 0) = #7/31/2009# '2yr bond
    bonds(4, 1) = #7/31/2011#
    bonds(4, 2) = 0.01
    bonds(4, 3) = 99.6367

    bonds(5, 0) = #7/31/2007# '3yr bond
    bonds(5, 1) = #7/31/2012#
    bonds(5, 2) = 0.04625
    bonds(5, 3) = 108.5469

    bonds(6, 0) = #7/31/2008# '4yr bond
    bonds(6, 1) = #7/31/2013#
    bonds(6, 2) = 0.03375
    bonds(6, 3) = 104.4922

    bonds(7, 0) = #7/31/2009# '5yr bond
    bonds(7, 1) = #7/31/2014#
    bonds(7, 2) = 0.02625
    bonds(7, 3) = 99.8281

    bonds(8, 0) = #5/15/2006# '7yr bond
    bonds(8, 1) = #5/15/2016#
    bonds(8, 2) = 0.05125
    bonds(8, 3) = 111.6367

    bonds(9, 0) = #5/15/2009# '10yr bond
    bonds(9, 1) = #5/15/2019#
    bonds(9, 2) = 0.03125
    bonds(9, 3) = 95.8359

    bonds(10, 0) = #2/15/1995# '15yr bond
    bonds(10, 1) = #2/15/2025#
    bonds(10, 2) = 0.07625
    bonds(10, 3) = 138.2344

    bonds(11, 0) = #2/16/1999# '20yr bond
    bonds(11, 1) = #2/15/2029#
    bonds(11, 2) = 0.0525
    bonds(11, 3) = 111.3047

    bonds(12, 0) = #5/15/2009# '30yr bond
    bonds(12, 1) = #5/15/2039#
    bonds(12, 2) = 0.0425
    bonds(12, 3) = 97.4063

    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct yield curve with reference date #6/30/2009#
    Set oYTS = oFuncMiscs.constructYieldTSbyOntheRun(bonds, #6/30/2009#)
    'return the 5-yr discount factor
    Dim dblDF5Yr As Double
    dblDF5Yr = oYTS.calDiscountFactor(5)
    
End Sub

Private Sub mnuFlatvol_Click()
    
    Dim oVolTS As IVolTermStructure
    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct a 1% flat vol curve
    Set oVolTS = oFuncMiscs.constructFlatVolTS(0.01)
    'return the volatility
    Dim dblVol5Yr As Double
    dblVol5Yr = oVolTS.volatility(5)
    
    MsgBox dblVol5Yr
    
End Sub

Private Sub mnuflatyts_Click()

    Dim oYTS As IYieldTermStructure
    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct a 6% flat yield curve with reference date #6/30/2009#
    Set oYTS = oFuncMiscs.constructFlatYieldTS(0.06, #6/30/2009#)
    'return the 5-yr discount factor
    Dim dblDF5Yr As Double
    dblDF5Yr = oYTS.calDiscountFactor(5)
    
    MsgBox dblDF5Yr
End Sub

Private Sub mnumodelcoef_Click()
    Dim oME As New MarketEnvironment
    oME.InitializeModelCoef
End Sub

Private Sub mnuparyts_Click()

    Dim oYTS As IYieldTermStructure
    'two-dimensional array for times and par rates
    '1m,  1.15923%
    '3m,  1.25158%
    '6m,  1.39018%
    '1yr, 1.66767%
    '2yr, 2.20008%
    '3yr, 2.63741%
    '4yr, 3.10033%
    '5yr ,3.41518%
    '7yr, 3.81306%
    '10yr, 4.04974%
    '15yr, 4.42256%
    '20yr, 4.77994%
    '30yr, 4.64704%
    Dim rates
    ReDim rates(0 To 12, 0 To 1) 'rates should be variant type
    rates(0, 0) = 0.08333
    rates(0, 1) = 0.0115923
    rates(1, 0) = 0.25
    rates(1, 1) = 0.0125158
    rates(2, 0) = 0.5
    rates(2, 1) = 0.0139018
    rates(3, 0) = 1
    rates(3, 1) = 0.0166767
    rates(4, 0) = 2
    rates(4, 1) = 0.0220008
    rates(5, 0) = 3
    rates(5, 1) = 0.0263741
    rates(6, 0) = 4
    rates(6, 1) = 0.0310033
    rates(7, 0) = 5
    rates(7, 1) = 0.0341518
    rates(8, 0) = 7
    rates(8, 1) = 0.0381306
    rates(9, 0) = 10
    rates(9, 1) = 0.0404974
    rates(10, 0) = 15
    rates(10, 1) = 0.0442256
    rates(11, 0) = 20
    rates(11, 1) = 0.0477994
    rates(12, 0) = 30
    rates(12, 1) = 0.0464704

    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct yield curve with reference date #6/30/2009#
    Set oYTS = oFuncMiscs.constructYieldTSbyPars(rates, #6/30/2009#)
    'return the 5-yr discount factor
    Dim dblDF5Yr As Double
    dblDF5Yr = oYTS.calDiscountFactor(5)
    
    MsgBox dblDF5Yr
End Sub

Private Sub mnuexchangerates_Click()

    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    'source currency
    Dim sc As MARKET_CURRENCY_TYPE
    sc = USD
    'target currency
    Dim tc As MARKET_CURRENCY_TYPE
    tc = CNY
    'set the exchange rates
    oME.SetExchangeRate sc, tc, #6/30/2009#, 6.84
    
End Sub

Private Sub mnupmmrates_Click()
    
    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    'instance object HistoricalValues
    Dim vals As New HistoricalValues
    Dim pmmtype As PMM_TYPE
    'pmm type
    pmmtype = PMM_30
    'value 1
    Dim val1 As New HistoricalValue
    val1.Date = #3/31/2009#
    val1.Value = 0.04
    vals.Add val1
    'value 2
    Dim val2 As New HistoricalValue
    val2.Date = #4/30/2009#
    val2.Value = 0.05
    vals.Add val2
    'value 3
    Dim val3 As New HistoricalValue
    val3.Date = #5/31/2009#
    val3.Value = 0.06
    vals.Add val3
    'add value n...
    'set the pmm rates
    oME.setPMMRates pmmtype, vals
    
End Sub

Private Sub mnuindexrates_Click()

    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    'instance object HistoricalValues
    Dim vals As New HistoricalValues
    Dim idxtype As MARKET_INDEX_TYPE
    'index type
    idxtype = INDEX_1YRTREAS
    'value 1
    Dim val1 As New HistoricalValue
    val1.Date = #3/31/2009#
    val1.Value = 0.04
    vals.Add val1
    'value 2
    Dim val2 As New HistoricalValue
    val2.Date = #4/30/2009#
    val2.Value = 0.05
    vals.Add val2
    'value 3
    Dim val3 As New HistoricalValue
    val3.Date = #5/31/2009#
    val3.Value = 0.06
    vals.Add val3
    'add value n...
    'set the index rates
    oME.setHistoricalIndexValues idxtype, vals
    
End Sub

Private Sub mnusetcorps_Click()

    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    
    'rating
    Dim creditrating As CR_CREDITRATING
    creditrating = CR_AA
    'values
    Dim spreads
    ReDim spreads(0 To 6)
    spreads(0) = 0.0024 '1yr
    spreads(1) = 0.0037 '2yr
    spreads(2) = 0.0039 '3yr
    spreads(3) = 0.0051 '5yr
    spreads(4) = 0.0064 '7yr
    spreads(5) = 0.0077 '10yr
    spreads(6) = 0.01 '30yr
    
    'set corp spreads
    oME.setCorpSpreads creditrating, spreads
    
End Sub

Private Sub mnusetCorpsEx_Click()

    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    
    'rating
    Dim creditrating As CR_CREDITRATING
    creditrating = CR_AA
    'industry type
    Dim industry As INDUSTRY_TYPE
    industry = INDUSTRY_MISCELLANEOUS
    'values
    Dim spreads
    ReDim spreads(0 To 6)
    spreads(0) = 0.0024 '1yr
    spreads(1) = 0.0037 '2yr
    spreads(2) = 0.0039 '3yr
    spreads(3) = 0.0051 '5yr
    spreads(4) = 0.0064 '7yr
    spreads(5) = 0.0077 '10yr
    spreads(6) = 0.01 '30yr
    
    'set corp spreads
    oME.setCorpSpreadsEx #6/30/2009#, creditrating, industry, spreads
    
End Sub

Private Sub mnusetvolts_Click()
    
    Dim oVolTS As IVolTermStructure
    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct a 1% flat vol curve
    Set oVolTS = oFuncMiscs.constructFlatVolTS(0.01)
    
    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    'set yts
    oME.setVolTermStructure #8/15/2009#, oVolTS
    
End Sub

Private Sub mnusetYTS_Click()

    Dim oYTS As IYieldTermStructure
    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct a 6% flat yield curve with reference date #6/30/2009#
    Set oYTS = oFuncMiscs.constructFlatYieldTS(0.06, #6/30/2009#)
        
    'instance object MarketEnvironment
    Dim oME As New MarketEnvironment
    'set yts
    oME.setYieldTermStructure #8/15/2009#, YTS_TREASURY, oYTS

End Sub

Private Sub mnusinvol_Click()

    Dim oVolTS As IVolTermStructure
    'two-dimensional array for vols
    '1m,  0.17%
    '3m,  0.19%
    '6m,  0.35%
    '1yr, 0.56%
    '2yr, 1.11%
    '3yr, 1.65%
    '4yr, 1.21%
    '5yr ,2.59%
    '7yr, 3.28%
    '10yr, 3.64%
    '15yr, 4.08%
    '20yr, 4.58%
    '30yr, 4.52%
    Dim Vols
    ReDim Vols(0 To 12, 0 To 1)
    Vols(0, 0) = 0.08333
    Vols(0, 1) = 0.0017
    Vols(1, 0) = 0.25
    Vols(1, 1) = 0.0019
    Vols(2, 0) = 0.5
    Vols(2, 1) = 0.0035
    Vols(3, 0) = 1
    Vols(3, 1) = 0.0056
    Vols(4, 0) = 2
    Vols(4, 1) = 0.0111
    Vols(5, 0) = 3
    Vols(5, 1) = 0.0165
    Vols(6, 0) = 4
    Vols(6, 1) = 0.0121
    Vols(7, 0) = 5
    Vols(7, 1) = 0.0259
    Vols(8, 0) = 7
    Vols(8, 1) = 0.0328
    Vols(9, 0) = 10
    Vols(9, 1) = 0.0364
    Vols(10, 0) = 15
    Vols(10, 1) = 0.0408
    Vols(11, 0) = 20
    Vols(11, 1) = 0.0458
    Vols(12, 0) = 30
    Vols(12, 1) = 0.0452
    
    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct vol curve
    Set oVolTS = oFuncMiscs.constructVolTSinPair(Vols)
    'return the volatility
    Dim dblVol5Yr As Double
    dblVol5Yr = oVolTS.volatility(5)
    
    MsgBox dblVol5Yr
End Sub

Private Sub mnuspotyts_Click()

    Dim oYTS As IYieldTermStructure
    
    'two-dimensional array for times and spot rates
    '1m,  0.17%
    '3m,  0.19%
    '6m,  0.35%
    '1yr, 0.56%
    '2yr, 1.11%
    '3yr, 1.65%
    '4yr, 1.21%
    '5yr ,2.59%
    '7yr, 3.28%
    '10yr, 3.64%
    '15yr, 4.08%
    '20yr, 4.58%
    '30yr, 4.52%
    Dim rates
    ReDim rates(0 To 12, 0 To 1)
    rates(0, 0) = 0.08333
    rates(0, 1) = 0.0017
    rates(1, 0) = 0.25
    rates(1, 1) = 0.0019
    rates(2, 0) = 0.5
    rates(2, 1) = 0.0035
    rates(3, 0) = 1
    rates(3, 1) = 0.0056
    rates(4, 0) = 2
    rates(4, 1) = 0.0111
    rates(5, 0) = 3
    rates(5, 1) = 0.0165
    rates(6, 0) = 4
    rates(6, 1) = 0.0121
    rates(7, 0) = 5
    rates(7, 1) = 0.0259
    rates(8, 0) = 7
    rates(8, 1) = 0.0328
    rates(9, 0) = 10
    rates(9, 1) = 0.0364
    rates(10, 0) = 15
    rates(10, 1) = 0.0408
    rates(11, 0) = 20
    rates(11, 1) = 0.0458
    rates(12, 0) = 30
    rates(12, 1) = 0.0452

    'instance object FuncMiscs
    Dim oFuncMiscs As New FuncMiscs
    'construct yield curve with reference date #6/30/2009#
    Set oYTS = oFuncMiscs.constructYieldTSbySpots(rates, #6/30/2009#)
    'return the 5-yr discount factor
    Dim dblDF5Yr As Double
    dblDF5Yr = oYTS.calDiscountFactor(5)
    
    MsgBox dblDF5Yr
End Sub

