VERSION 5.00
Object = "{65E121D4-0C60-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCHRT20.OCX"
Begin VB.Form frmDiagram 
   Caption         =   "图表"
   ClientHeight    =   6105
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6960
   Icon            =   "frmDiagram.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6105
   ScaleWidth      =   6960
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  '所有者中心
   Begin VB.Frame fraOp 
      BorderStyle     =   0  'None
      Height          =   1455
      Left            =   90
      TabIndex        =   1
      Top             =   4200
      Width           =   6915
      Begin VB.CommandButton cmdPrint 
         Caption         =   "打印"
         Height          =   375
         Left            =   5460
         TabIndex        =   10
         Top             =   750
         Width           =   1275
      End
      Begin VB.CommandButton cmdBack 
         Caption         =   "返回"
         Height          =   375
         Left            =   5460
         TabIndex        =   9
         Top             =   330
         Width           =   1275
      End
      Begin VB.Frame fraInfo 
         Caption         =   "当前被选择数据点信息"
         Height          =   1275
         Left            =   2190
         TabIndex        =   2
         Top             =   60
         Width           =   3075
         Begin VB.TextBox txtInfo 
            BorderStyle     =   0  'None
            Height          =   255
            Index           =   0
            Left            =   1140
            Locked          =   -1  'True
            TabIndex        =   5
            Top             =   330
            Width           =   1635
         End
         Begin VB.TextBox txtInfo 
            BorderStyle     =   0  'None
            Height          =   255
            Index           =   1
            Left            =   1140
            Locked          =   -1  'True
            TabIndex        =   4
            Top             =   585
            Width           =   1635
         End
         Begin VB.TextBox txtInfo 
            BorderStyle     =   0  'None
            Height          =   255
            Index           =   2
            Left            =   1140
            Locked          =   -1  'True
            TabIndex        =   3
            Top             =   840
            Width           =   1635
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            Caption         =   "数据类型:"
            Height          =   180
            Index           =   0
            Left            =   300
            TabIndex        =   8
            Top             =   360
            Width           =   810
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            Caption         =   "月份:"
            Height          =   180
            Index           =   1
            Left            =   300
            TabIndex        =   7
            Top             =   630
            Width           =   450
         End
         Begin VB.Label lbl 
            AutoSize        =   -1  'True
            Caption         =   "数值:"
            Height          =   180
            Index           =   2
            Left            =   300
            TabIndex        =   6
            Top             =   900
            Width           =   450
         End
      End
   End
   Begin MSChart20Lib.MSChart msc 
      Height          =   3705
      Left            =   0
      OleObjectBlob   =   "frmDiagram.frx":0442
      TabIndex        =   0
      TabStop         =   0   'False
      ToolTipText     =   "当显示的是三维图表时，您可以按住Ctrl键旋转本图"
      Top             =   30
      Width           =   6915
   End
End
Attribute VB_Name = "frmDiagram"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim sMonth(1 To 12) As String
Dim lFinalData() As Single
'******************************************************
'*自定义函数
'******************************************************
Sub GetBound()
On Error Resume Next
Dim v As Variant
pblSeries = 0
pblMonth = 0

'For Each v In frmWizard.optModify
'    If v.Value = True Then pblSeries = pblSeries + 1
'Next

For Each v In frmWizard.chkModify
    If v.Value = vbChecked Then pblSeries = pblSeries + 1
Next

For Each v In frmWizard.chkMonth
    If v.Value = vbChecked Then
        pblMonth = pblMonth + 1
        sMonth(pblMonth) = FullMonthName(pbasMonth(v.Index))
    End If
Next

ReDim pbavFinalData(1 To pblMonth, 1 To pblSeries + 1)
ReDim lFinalData(1 To pblSeries + 1, 1 To pblMonth)
ReDim pbasLegend(1 To pblMonth) As String

End Sub

Sub GatherData()
On Error Resume Next
Dim l As Long, i As Long, j As Long, v As Variant
'For l = 1 To pblMonth
'    For i = 2 To pblSeries + 1
'        pbavFinalData(l, i) = l * Rnd
'    Next
'Next
For l = 1 To pblMonth
    pbavFinalData(l, 1) = sMonth(l)
Next


l = 0
j = 0
For Each v In frmWizard.chkMonth
    If v.Value = vbChecked Then
        l = l + 1
        For i = 0 To 2
            If frmWizard.chkModify(i).Value = vbChecked Then
                j = j + 1
                If j > pblSeries Then j = 1
                pbavFinalData(l, j + 1) = pbcolMonthlyData(pbasMonth(v.Index))(i)
            End If
        Next
    End If
Next
msc.ChartData = pbavFinalData

j = 0
For i = 0 To 2
    If frmWizard.chkModify(i).Value = vbChecked Then
        j = j + 1
        If j > pblSeries Then j = 1
        msc.Plot.SeriesCollection(j).LegendText = frmWizard.chkModify(i).Caption
    End If
Next



End Sub

Private Sub cmdBack_Click()
Me.Hide
End Sub

Private Sub cmdPrint_Click()
    msc.EditCopy
    Printer.PaintPicture Clipboard.GetData, 0, 0
End Sub

'******************************************************
'*窗体函数
'******************************************************

Private Sub Form_Activate()

GetBound
GatherData

Me.WindowState = 2

With msc
    .chartType = pblCharType
    .TitleText = "XX公司数据图表"
    .ShowLegend = True
    .AllowSeriesSelection = False
    .Backdrop.Fill.Style = VtFillStyleBrush
    .Backdrop.Frame.Style = VtFrameStyleThickOuter
End With

With msc.Plot
    .Axis(VtChAxisIdX).AxisTitle = "月份"
    .Axis(VtChAxisIdY).AxisTitle = "数据单位(" + pbsDataUnit + ")"
    .Axis(VtChAxisIdZ).AxisTitle = "数据类型"
    If pbfIs3D Then
        .Axis(VtChAxisIdX).AxisTitle.VtFont.Name = "黑体"
        .Axis(VtChAxisIdX).AxisTitle.VtFont.Size = 24
        .Axis(VtChAxisIdX).AxisTitle.VtFont.VtColor.Red = 255
        
        .Axis(VtChAxisIdY).AxisTitle.VtFont.Name = "黑体"
        .Axis(VtChAxisIdY).AxisTitle.VtFont.Size = 24
        .Axis(VtChAxisIdY).AxisTitle.VtFont.VtColor.Red = 255
        
        .Axis(VtChAxisIdZ).AxisTitle.VtFont.Name = "黑体"
        .Axis(VtChAxisIdZ).AxisTitle.VtFont.Size = 24
        .Axis(VtChAxisIdZ).AxisTitle.VtFont.VtColor.Red = 255
        
    Else
        .Axis(VtChAxisIdX).AxisTitle.VtFont.Name = "黑体"
        .Axis(VtChAxisIdX).AxisTitle.VtFont.Size = 12
        .Axis(VtChAxisIdX).AxisTitle.VtFont.VtColor.Red = 255
        
        .Axis(VtChAxisIdY).AxisTitle.VtFont.Name = "黑体"
        .Axis(VtChAxisIdY).AxisTitle.VtFont.Size = 12
        .Axis(VtChAxisIdY).AxisTitle.VtFont.VtColor.Red = 255
        
        .Axis(VtChAxisIdZ).AxisTitle.VtFont.Name = "黑体"
        .Axis(VtChAxisIdZ).AxisTitle.VtFont.Size = 12
        .Axis(VtChAxisIdZ).AxisTitle.VtFont.VtColor.Red = 255
    End If
End With

End Sub

Private Sub Form_Resize()
On Error Resume Next
msc.Move -10, -10, Me.ScaleWidth + 40, Me.ScaleHeight - fraOp.Height
fraOp.Move 0, msc.Top + msc.Height, msc.Width
fraInfo.Move (fraOp.Width - fraInfo.Width) / 2, (fraOp.Height - fraInfo.Height) / 2
cmdBack.Move fraInfo.Left + fraInfo.Width + 100, (fraInfo.Height - cmdBack.Height) / 2 + 100
End Sub

Private Sub msc_PointSelected(Series As Integer, DataPoint As Integer, MouseFlags As Integer, Cancel As Integer)
If pbfEditMode = True Then

    txtInfo(0) = msc.Plot.SeriesCollection(Series).LegendText
    txtInfo(1) = FullMonthName(pbasMonth(DataPoint - 1))
    txtInfo(2) = CStr(msc.ChartData(DataPoint, Series)) + pbsDataUnit
Else
    txtInfo(0) = msc.Plot.SeriesCollection(Series).LegendText
    txtInfo(1) = FullMonthName(pbasMonth(DataPoint - 1))
    txtInfo(2) = CStr(0) + pbsDataUnit
    
End If
End Sub
