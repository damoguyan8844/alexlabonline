VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   BackColor       =   &H00FFFFFF&
   ClientHeight    =   7800
   ClientLeft      =   60
   ClientTop       =   60
   ClientWidth     =   10485
   ControlBox      =   0   'False
   BeginProperty Font 
      Name            =   "宋体"
      Size            =   10.5
      Charset         =   134
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   ScaleHeight     =   520
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   699
   StartUpPosition =   2  '屏幕中心
   Begin VB.PictureBox pb 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1500
      Index           =   3
      Left            =   7320
      ScaleHeight     =   1500
      ScaleWidth      =   1500
      TabIndex        =   13
      Top             =   5670
      Width           =   1500
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "导入数据"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   4
         Left            =   420
         TabIndex        =   20
         Top             =   660
         Width           =   720
      End
   End
   Begin VB.PictureBox pbs 
      BorderStyle     =   0  'None
      Height          =   675
      Left            =   2970
      Picture         =   "frmMain.frx":0442
      ScaleHeight     =   675
      ScaleWidth      =   3585
      TabIndex        =   22
      ToolTipText     =   "您可以点这里拖动窗口"
      Top             =   1410
      Width           =   3585
   End
   Begin VB.PictureBox pb 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1500
      Index           =   5
      Left            =   4440
      ScaleHeight     =   1500
      ScaleWidth      =   1500
      TabIndex        =   15
      Top             =   6090
      Width           =   1500
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "退出程序"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   6
         Left            =   420
         TabIndex        =   17
         Top             =   630
         Width           =   720
      End
   End
   Begin VB.PictureBox pb 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1500
      Index           =   4
      Left            =   5910
      ScaleHeight     =   1500
      ScaleWidth      =   1500
      TabIndex        =   14
      Top             =   6000
      Width           =   1500
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "保存数据"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   5
         Left            =   420
         TabIndex        =   16
         Top             =   660
         Width           =   720
      End
   End
   Begin VB.PictureBox pb 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1500
      Index           =   2
      Left            =   8130
      ScaleHeight     =   1500
      ScaleWidth      =   1500
      TabIndex        =   12
      Top             =   4290
      Width           =   1500
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "清除列表"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   3
         Left            =   420
         TabIndex        =   21
         Top             =   660
         Width           =   720
      End
   End
   Begin VB.PictureBox pb 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1500
      Index           =   1
      Left            =   8400
      ScaleHeight     =   1500
      ScaleWidth      =   1500
      TabIndex        =   11
      Top             =   2820
      Width           =   1500
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "修改数据"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   2
         Left            =   420
         TabIndex        =   18
         Top             =   660
         Width           =   720
      End
   End
   Begin VB.PictureBox pb 
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1500
      Index           =   0
      Left            =   8040
      ScaleHeight     =   1500
      ScaleWidth      =   1500
      TabIndex        =   10
      Top             =   1350
      Width           =   1500
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "生成图表"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   1
         Left            =   420
         TabIndex        =   19
         Top             =   660
         Width           =   720
      End
   End
   Begin VB.Timer tmr 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   6870
      Top             =   6960
   End
   Begin VB.CommandButton cmdModify 
      BackColor       =   &H00FFFFFF&
      Caption         =   "修改数据"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1065
      Left            =   1380
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   9
      ToolTipText     =   "修改当前数据文件中的数据"
      Top             =   2460
      UseMaskColor    =   -1  'True
      Visible         =   0   'False
      Width           =   1125
   End
   Begin VB.CommandButton cmdLoad 
      BackColor       =   &H00FFFFFF&
      Caption         =   "导入数据"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1065
      Left            =   4995
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   8
      ToolTipText     =   "从文件中读入数据"
      Top             =   2460
      UseMaskColor    =   -1  'True
      Visible         =   0   'False
      Width           =   1125
   End
   Begin VB.CommandButton cmdSave 
      BackColor       =   &H00FFFFFF&
      Caption         =   "保存数据"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1065
      Left            =   2580
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   7
      ToolTipText     =   "把数据保存到文件"
      Top             =   2460
      UseMaskColor    =   -1  'True
      Visible         =   0   'False
      Width           =   1125
   End
   Begin VB.CommandButton cmdExit 
      BackColor       =   &H00FFFFFF&
      Caption         =   "退出程序"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1065
      Left            =   3795
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   6
      ToolTipText     =   "把数据保存到文件"
      Top             =   2460
      UseMaskColor    =   -1  'True
      Visible         =   0   'False
      Width           =   1125
   End
   Begin VB.CommandButton cmdClear 
      BackColor       =   &H00FFFFFF&
      Caption         =   "清除列表"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1065
      Left            =   165
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   5
      ToolTipText     =   "从文件中读入数据"
      Top             =   2460
      UseMaskColor    =   -1  'True
      Visible         =   0   'False
      Width           =   1125
   End
   Begin VB.CommandButton cmdGenerateDiagram 
      BackColor       =   &H00FFFFFF&
      Caption         =   "生成图表"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1065
      Left            =   6210
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   4
      ToolTipText     =   "把数据保存到文件"
      Top             =   2460
      UseMaskColor    =   -1  'True
      Visible         =   0   'False
      Width           =   1125
   End
   Begin MSComDlg.CommonDialog cdlg 
      Left            =   300
      Top             =   3990
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Frame fra 
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   0
      Left            =   1560
      TabIndex        =   1
      Top             =   360
      Visible         =   0   'False
      Width           =   1125
      Begin VB.TextBox txtFilepath 
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   1770
         TabIndex        =   2
         Top             =   330
         Width           =   4320
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "当前数据文件路径："
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   180
         Index           =   0
         Left            =   180
         TabIndex        =   3
         Top             =   390
         Width           =   1620
      End
   End
   Begin MSComctlLib.ListView lvData 
      Height          =   3135
      Left            =   1890
      TabIndex        =   0
      ToolTipText     =   "请在要操作的数据前打勾"
      Top             =   2280
      Width           =   5850
      _ExtentX        =   10319
      _ExtentY        =   5530
      View            =   3
      Arrange         =   2
      LabelEdit       =   1
      LabelWrap       =   0   'False
      HideSelection   =   0   'False
      HideColumnHeaders=   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      NumItems        =   0
      Picture         =   "frmMain.frx":2CEE
   End
   Begin VB.Image imgbtn 
      Height          =   1500
      Index           =   2
      Left            =   60
      Picture         =   "frmMain.frx":A61E
      Top             =   870
      Visible         =   0   'False
      Width           =   1500
   End
   Begin VB.Image imgbtn 
      Height          =   1500
      Index           =   1
      Left            =   1020
      Picture         =   "frmMain.frx":CB66
      Top             =   900
      Visible         =   0   'False
      Width           =   1500
   End
   Begin VB.Image imgbtn 
      Height          =   1500
      Index           =   0
      Left            =   2100
      Picture         =   "frmMain.frx":F119
      Top             =   870
      Visible         =   0   'False
      Width           =   1500
   End
   Begin VB.Image img 
      Height          =   480
      Index           =   5
      Left            =   1026
      Picture         =   "frmMain.frx":110D0
      Top             =   300
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image img 
      Height          =   480
      Index           =   4
      Left            =   1752
      Picture         =   "frmMain.frx":11512
      Top             =   300
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image img 
      Height          =   480
      Index           =   3
      Left            =   3204
      Picture         =   "frmMain.frx":11954
      Top             =   300
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image img 
      Height          =   480
      Index           =   2
      Left            =   3930
      Picture         =   "frmMain.frx":11D96
      Top             =   300
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image img 
      Height          =   480
      Index           =   1
      Left            =   2478
      Picture         =   "frmMain.frx":121D8
      Top             =   300
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image img 
      Height          =   480
      Index           =   0
      Left            =   300
      Picture         =   "frmMain.frx":1261A
      Top             =   300
      Visible         =   0   'False
      Width           =   480
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Const lOffsetX  As Long = 10
Const lOffsetY As Long = 7
Dim lX As Long, lY As Long
Dim fMouseStat As Boolean
Dim lLastID As Integer
'******************************************************
'*自定义函数
'******************************************************


'初始化
Sub InitProc()
LoadPath
pbsDataFileName = Trim(txtFilepath.Text)

pbasDataType(0) = "月    份"
pbasDataType(1) = "计划数据"
pbasDataType(2) = "实际数据"
pbasDataType(3) = "预测数据"

pbasLVKeyName(0) = "keyMonth"
pbasLVKeyName(1) = "keyPlan"
pbasLVKeyName(2) = "keyReal"
pbasLVKeyName(3) = "keyExpect"

pbasMonth(0) = "Jan"
pbasMonth(1) = "Feb"
pbasMonth(2) = "Mar"
pbasMonth(3) = "Apr"
pbasMonth(4) = "May"
pbasMonth(5) = "Jun"
pbasMonth(6) = "Jul"
pbasMonth(7) = "Aug"
pbasMonth(8) = "Sep"
pbasMonth(9) = "Oct"
pbasMonth(10) = "Nov"
pbasMonth(11) = "Dec"

pb(0).ToolTipText = "当输入完数据后，就可以点这里生成图表"
pb(1).ToolTipText = "添加、修改数据及设置数据单位"
pb(2).ToolTipText = "清除列表中的数据"
pb(3).ToolTipText = "从文件中导入数据"
pb(4).ToolTipText = "把数据保存到文件中"
pb(5).ToolTipText = "结束掉一切"

Dim l As Long
With lvData
    For l = 0 To 3
        .ColumnHeaders.Add , pbasLVKeyName(l), pbasDataType(l)
    Next
    
    .HideColumnHeaders = True
    .Checkboxes = True
    .FlatScrollBar = True
    .FullRowSelect = True
    .GridLines = True
    '.HotTracking = True
    .HideSelection = True
    .MultiSelect = True
    .FlatScrollBar = True
    .PictureAlignment = lvwTile
    
    
    LVinit
End With

Dim v As Variant
For Each v In pb
     v.Picture = imgbtn(0).Picture
     v.Tag = "0"
Next

pbsDataUnit = pbcntDefaultDataUnit
frmData.txtUnit = pbsDataUnit
End Sub
'初始化列表框:填入月份名
Sub LVinit()
Dim l As Long
lvData.ListItems.Clear
For l = 0 To 11

    lvData.ListItems.Add , pbasMonth(l), FullMonthName(pbasMonth(l))
    lvData.ListItems(l + 1).Checked = True
    lvData.ListItems(l + 1).SubItems(1) = 0
    lvData.ListItems(l + 1).SubItems(2) = 0
    lvData.ListItems(l + 1).SubItems(3) = 0
Next

pbfInit = True

CheckButtonStat
End Sub
'将集合中的填充到列表
Sub CollectionToList()
On Error Resume Next
Dim v As Variant, li As ListItem, l As Long
For l = 0 To 11
    Set li = lvData.ListItems(l + 1)
    v = pbcolMonthlyData(pbasMonth(l))
    li.SubItems(1) = v(0)
    li.SubItems(2) = v(1)
    li.SubItems(3) = v(2)
    li.Checked = True
Next
End Sub

'将列表中的数据填充到集合
Sub ListToCollection()
On Error Resume Next
Dim v As Variant, li As ListItem, a(2) As Single, l As Long, i As Long
For l = 0 To 11
    Set li = lvData.ListItems(pbasMonth(l))
    For i = 0 To 2
        pbcolMonthlyData(pbasMonth(l))(i) = CSng(li.SubItems(i + 1))
    Next
Next
End Sub

'获得导入数据文件名
Function GetOpenFile(ByRef s As String) As Boolean
On Error GoTo staErr

With frmMain.cdlg
    .CancelError = True
    .InitDir = App.Path
    .Flags = cdlOFNExplorer Or _
                cdlOFNLongNames Or _
                    cdlOFNHideReadOnly
    .Filter = "数据文件(*.dia)|*.dia"
    .ShowOpen
    s = Trim(.FileName)
End With
    
GetOpenFile = True
Exit Function
staErr:
GetOpenFile = False
End Function
'获得保存文件名
Function GetSaveFile(ByRef s As String) As Boolean
On Error GoTo staErr

With frmMain.cdlg
    .CancelError = True
    .InitDir = App.Path
    .Flags = cdlOFNExplorer Or _
                cdlOFNLongNames Or _
                    cdlOFNHideReadOnly Or _
                        cdlOFNOverwritePrompt
    .Filter = "数据文件(*.dia)|*.dia"
    .FileName = s
    .ShowSave
    s = Trim(.FileName)
End With
    
GetSaveFile = True
Exit Function
staErr:
GetSaveFile = False
End Function

Function CheckButtonStat()
On Error Resume Next
If pbfInit = True Then
    pbfEditMode = False
    cmdSave.Enabled = False
    cmdModify.Caption = "添加数据"
    cmdModify.Tag = "1"
Else
    pbfEditMode = True
    cmdSave.Enabled = True
    cmdModify.Caption = "修改数据"
    cmdModify.Tag = "0"
End If
SetLabelText
End Function
Sub SetLabelText()
lbl(1) = cmdGenerateDiagram.Caption
lbl(2) = cmdModify.Caption
lbl(3) = cmdClear.Caption
lbl(4) = cmdLoad.Caption
lbl(5) = cmdSave.Caption
lbl(6) = cmdExit.Caption
End Sub
Sub SetRegion()
Dim hFinal As Long, hBar As Long, hCentre As Long, hBall(5) As Long, l As Long, lr As Long, lOld As Long

lOld = frmMain.ScaleMode
frmMain.ScaleMode = vbPixels

hFinal = CreateRectRgn(0, 0, Me.ScaleWidth, Me.ScaleHeight)

With pbs
    hBar = CreateRectRgn(.Left + lOffsetX - 6, .Top + lOffsetY + 1 - 4, .Left + .Width + 4, .Top + .Height + 4)
    'hBar = CreateRoundRectRgn(.Left + lOffsetX, .Top + lOffsetY + 1, .Left + .Width, .Top + .Height, 20, 20)
    'hBar = CreateEllipticRgn(.Left + lOffsetX - 5, .Top + lOffsetY + 1 - 5, .Left + .Width + 5, .Top + .Height + 5)
End With

With lvData
    hCentre = CreateRectRgn(.Left + lOffsetX - 6, .Top + lOffsetY + 1 - 4, .Left + .Width + 4, .Top + .Height + 4)
    'hCentre = CreateRoundRectRgn(.Left + lOffsetX, .Top + lOffsetY + 1, .Left + .Width, .Top + .Height - 15, 10, 10)
End With


For l = 0 To 5
    With pb(l)
        lr = l * 2.5 - 1
        hBall(l) = CreateEllipticRgn(.Left + lOffsetX + lr, .Top + lOffsetY + lr, .Left + .Width - lr, .Top + .Height - lr)
    End With
Next

Call CombineRgn(hFinal, hBar, hCentre, RGN_OR)
Call CombineRgn(hFinal, hFinal, hBall(0), RGN_OR)

'Call CombineRgn(hFinal, hCentre, hBall(0), RGN_OR)

Call CombineRgn(hFinal, hFinal, hBall(1), RGN_OR)
Call CombineRgn(hFinal, hFinal, hBall(2), RGN_OR)
Call CombineRgn(hFinal, hFinal, hBall(3), RGN_OR)
Call CombineRgn(hFinal, hFinal, hBall(4), RGN_OR)
Call CombineRgn(hFinal, hFinal, hBall(5), RGN_OR)


Call SetWindowRgn(Me.hwnd, hFinal, True)

frmMain.ScaleMode = lOld
End Sub
'******************************************************
'*窗体函数
'******************************************************


Private Sub cmdClear_Click()
If pbfInit = False Then
    If MsgBox("您确定要清除列表框吗?", vbYesNo + vbDefaultButton2 + vbQuestion) = vbYes Then
        LVinit
        pbfEditMode = False
    End If
End If
End Sub

Private Sub cmdExit_Click()
Unload Me
End Sub

Private Sub cmdGenerateDiagram_Click()

frmWizard.Show vbModal
End Sub

Private Sub cmdLoad_Click()
Dim sPath As String
sPath = pbsDataFileName
If GetOpenFile(sPath) Then
    If LoadData(sPath, pbcolMonthlyData) Then
        pbsDataFileName = sPath
        lvData.Enabled = True
        CollectionToList
        pbfInit = False
        MsgBox "数据已成功从文件 " + """" + pbsDataFileName + """" + " 导入。", vbOKOnly + vbInformation, "数据导入"
    End If
End If
CheckButtonStat
End Sub

Private Sub cmdModify_Click()
frmData.Show vbModal
CheckButtonStat
End Sub

Private Sub cmdSave_Click()
Dim sPath As String
ListToCollection
sPath = pbsDataFileName

If GetSaveFile(sPath) Then
    If SaveData(sPath, pbcolMonthlyData) Then
        MsgBox "数据已保存到 " + sPath, vbOKOnly + vbInformation
    Else
        MsgBox "数据未保存到 " + sPath, vbOKOnly + vbInformation
    End If
End If
End Sub




Private Sub Form_Load()
If App.PrevInstance = True Then End

InitProc

SetRegion


End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim v As Variant
For Each v In pb
    If v.Tag = "1" Then v.Picture = imgbtn(0).Picture
Next
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
If MsgBox("您真的想退出么？", vbYesNo + vbDefaultButton2 + vbQuestion) = vbYes Then
    SavePath
    Set pbcolMonthlyData = Nothing
    Unload frmData
    Unload frmWizard
    Unload frmDiagram
Else
    Cancel = True
End If
End Sub

Private Sub img_Click(Index As Integer)
Select Case Index
Case 0
    cmdGenerateDiagram_Click
Case 1
    cmdModify_Click
Case 2
    cmdClear_Click
Case 3
    cmdLoad_Click
Case 4
    cmdSave_Click
Case 5
    cmdExit_Click
Case 6
End Select
End Sub

Private Sub img_MouseMove(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
img(Index).Picture = imgbtn(1).Picture
End Sub

Private Sub lbl_Click(Index As Integer)
img_Click Index - 1
End Sub

Private Sub lbl_MouseDown(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
pb_MouseDown Index - 1, Button, Shift, x, y
End Sub

Private Sub lbl_MouseMove(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
pb_MouseMove Index - 1, Button, Shift, x, y
End Sub

Private Sub lbl_MouseUp(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
pb_MouseUp Index - 1, Button, Shift, x, y

End Sub

Private Sub lvData_DblClick()
lvData.SelectedItem.Checked = Not lvData.SelectedItem.Checked

End Sub


Private Sub lvData_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim s As String, li As ListItem
With pb(lLastID)
    If .Tag = "1" Then
        .Picture = imgbtn(0).Picture
        .Tag = "0"
    End If
End With

With lvData
    Set li = .HitTest(x, y)
    s = "月份：" + li.Text + _
            "  计划数据：" + li.SubItems(1) + pbsDataUnit + _
                "  实际数据：" + li.SubItems(2) + pbsDataUnit + _
                    "  预测数据：" + li.SubItems(3) + pbsDataUnit
    .ToolTipText = s
End With
End Sub


Private Sub pb_Click(Index As Integer)
img_Click Index
End Sub

Private Sub pb_MouseDown(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
pb(Index).Picture = imgbtn(2).Picture
End Sub

Private Sub pb_MouseMove(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
Static i As Integer
pb(Index).Picture = imgbtn(1).Picture
pb(Index).Tag = "1"
If i <> Index Then
    pb(i).Picture = imgbtn(0).Picture
    pb(i).Tag = "0"
End If
i = Index
lLastID = i
End Sub

Private Sub pb_MouseUp(Index As Integer, Button As Integer, Shift As Integer, x As Single, y As Single)
pb(Index).Picture = imgbtn(1).Picture
End Sub

Private Sub pbs_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single)
SetCapture pbs.hwnd
fMouseStat = True
lX = x
lY = y

End Sub


Private Sub pbs_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
Dim l As Long
If fMouseStat Then
    With frmMain
        l = .ScaleMode
        .ScaleMode = vbTwips
        .Move .Left + (x - lX), .Top + (y - lY)
        lX = x
        lY = y
        .ScaleMode = l
    End With
End If
ReleaseCapture
fMouseStat = False

End Sub

Private Sub tmr_Timer()
On Error Resume Next
If pbfInit = True Then
    pbfEditMode = False
    cmdSave.Enabled = False
    cmdModify.Caption = "添加数据(&A)"
    cmdModify.Tag = "1"
Else
    pbfEditMode = True
    cmdSave.Enabled = True
    cmdModify.Caption = "修改数据(&M)"
    cmdModify.Tag = "0"
End If
End Sub

Private Sub txtFilepath_Change()
pbsDataFileName = Trim(txtFilepath.Text)
End Sub
