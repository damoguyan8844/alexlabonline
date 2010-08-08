VERSION 5.00
Begin VB.Form frmWizard 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "图表生成向导"
   ClientHeight    =   4545
   ClientLeft      =   45
   ClientTop       =   270
   ClientWidth     =   7410
   Icon            =   "frmWizard.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4545
   ScaleWidth      =   7410
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  '所有者中心
   Begin VB.CommandButton cmdCancel 
      Caption         =   "取消(&C)"
      Height          =   375
      Left            =   2820
      TabIndex        =   4
      Top             =   4050
      Width           =   1215
   End
   Begin VB.CommandButton cmdNext 
      Caption         =   "下一步(&N)"
      Default         =   -1  'True
      Height          =   375
      Left            =   5820
      TabIndex        =   2
      Top             =   4050
      Width           =   1215
   End
   Begin VB.CommandButton cmdBack 
      Caption         =   "上一步(&B)"
      Height          =   375
      Left            =   4485
      TabIndex        =   1
      Top             =   4050
      Width           =   1215
   End
   Begin VB.CommandButton cmdFinish 
      Caption         =   "完成(&F)"
      Height          =   375
      Left            =   5820
      TabIndex        =   3
      Top             =   4050
      Width           =   1215
   End
   Begin VB.PictureBox pbStep 
      BorderStyle     =   0  'None
      Height          =   3735
      Index           =   2
      Left            =   0
      ScaleHeight     =   249
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   495
      TabIndex        =   27
      Top             =   0
      Width           =   7425
      Begin VB.Frame fra 
         ClipControls    =   0   'False
         Height          =   1155
         Index           =   3
         Left            =   2910
         TabIndex        =   35
         Top             =   2220
         Width           =   3705
         Begin VB.OptionButton optChartType 
            Caption         =   "饼图"
            Height          =   285
            Index           =   5
            Left            =   2670
            TabIndex        =   43
            Tag             =   "14"
            Top             =   660
            Width           =   1005
         End
         Begin VB.OptionButton optChartType 
            Caption         =   "组合图"
            Height          =   285
            Index           =   4
            Left            =   1410
            TabIndex        =   42
            Tag             =   "9"
            Top             =   660
            Width           =   1005
         End
         Begin VB.OptionButton optChartType 
            Caption         =   "阶梯图"
            Height          =   285
            Index           =   3
            Left            =   150
            TabIndex        =   41
            Tag             =   "7"
            Top             =   660
            Width           =   1005
         End
         Begin VB.OptionButton optChartType 
            Caption         =   "面积图"
            Height          =   285
            Index           =   2
            Left            =   2670
            TabIndex        =   40
            Tag             =   "5"
            Top             =   330
            Width           =   1005
         End
         Begin VB.OptionButton optChartType 
            Caption         =   "直方图"
            Height          =   285
            Index           =   1
            Left            =   1410
            TabIndex        =   39
            Tag             =   "1"
            Top             =   330
            Width           =   1005
         End
         Begin VB.OptionButton optChartType 
            Caption         =   "折线图"
            Height          =   285
            Index           =   0
            Left            =   150
            TabIndex        =   38
            Tag             =   "3"
            Top             =   330
            Value           =   -1  'True
            Width           =   1005
         End
      End
      Begin VB.Frame fra 
         ClipControls    =   0   'False
         Height          =   525
         Index           =   2
         Left            =   3450
         TabIndex        =   34
         Top             =   1650
         Width           =   2625
         Begin VB.OptionButton optView 
            Caption         =   "三维视图"
            Height          =   285
            Index           =   1
            Left            =   1395
            TabIndex        =   37
            Tag             =   "3d"
            Top             =   180
            Width           =   1125
         End
         Begin VB.OptionButton optView 
            Caption         =   "二维视图"
            Height          =   285
            Index           =   0
            Left            =   240
            TabIndex        =   36
            Tag             =   "2d"
            Top             =   180
            Value           =   -1  'True
            Width           =   1125
         End
      End
      Begin VB.Image imgStep 
         BorderStyle     =   1  'Fixed Single
         Height          =   3450
         Index           =   2
         Left            =   150
         Picture         =   "frmWizard.frx":0442
         Stretch         =   -1  'True
         Top             =   210
         Width           =   1800
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "第三步："
         BeginProperty Font 
            Name            =   "楷体_GB2312"
            Size            =   18
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   5
         Left            =   2160
         TabIndex        =   29
         Top             =   360
         Width           =   1515
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "请选择要生成的图表类型"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   21.75
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         Height          =   435
         Index           =   4
         Left            =   2310
         TabIndex        =   28
         Top             =   960
         Width           =   4785
      End
   End
   Begin VB.PictureBox pbStep 
      BorderStyle     =   0  'None
      Height          =   3735
      Index           =   0
      Left            =   0
      ScaleHeight     =   249
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   495
      TabIndex        =   0
      Top             =   0
      Width           =   7425
      Begin VB.Frame fra 
         Height          =   735
         Index           =   1
         Left            =   2130
         TabIndex        =   6
         Top             =   2070
         Width           =   4995
         Begin VB.CheckBox chkModify 
            Caption         =   "计划数据"
            Height          =   315
            Index           =   0
            Left            =   120
            TabIndex        =   48
            Top             =   270
            Width           =   1065
         End
         Begin VB.CheckBox chkModify 
            Caption         =   "实际数据"
            Height          =   315
            Index           =   1
            Left            =   1350
            TabIndex        =   47
            Top             =   270
            Width           =   1065
         End
         Begin VB.CheckBox chkModify 
            Caption         =   "预测数据"
            Height          =   315
            Index           =   2
            Left            =   2580
            TabIndex        =   46
            Top             =   270
            Width           =   1065
         End
         Begin VB.CheckBox chkModify 
            Caption         =   "全部数据"
            Height          =   315
            Index           =   3
            Left            =   3810
            Style           =   1  'Graphical
            TabIndex        =   45
            Top             =   270
            Width           =   1065
         End
         Begin VB.OptionButton optModify 
            Caption         =   "全部数据"
            Height          =   285
            Index           =   3
            Left            =   3750
            TabIndex        =   32
            Top             =   1200
            Width           =   1245
         End
         Begin VB.OptionButton optModify 
            Caption         =   "预测数据"
            Height          =   285
            Index           =   2
            Left            =   2535
            TabIndex        =   9
            Top             =   1200
            Visible         =   0   'False
            Width           =   1065
         End
         Begin VB.OptionButton optModify 
            Caption         =   "实际数据"
            Height          =   285
            Index           =   1
            Left            =   1305
            TabIndex        =   8
            Top             =   1200
            Visible         =   0   'False
            Width           =   1245
         End
         Begin VB.OptionButton optModify 
            Caption         =   "计划数据"
            Height          =   285
            Index           =   0
            Left            =   90
            TabIndex        =   7
            Top             =   1200
            Value           =   -1  'True
            Visible         =   0   'False
            Width           =   1245
         End
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "请选择要抽取的数据类型"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   21.75
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00000000&
         Height          =   435
         Index           =   1
         Left            =   2310
         TabIndex        =   10
         Top             =   960
         Width           =   4905
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "第一步："
         BeginProperty Font 
            Name            =   "楷体_GB2312"
            Size            =   18
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00000000&
         Height          =   360
         Index           =   0
         Left            =   2160
         TabIndex        =   5
         Top             =   360
         Width           =   1515
      End
      Begin VB.Image imgStep 
         BorderStyle     =   1  'Fixed Single
         Height          =   3450
         Index           =   0
         Left            =   180
         Picture         =   "frmWizard.frx":902B
         Stretch         =   -1  'True
         Top             =   210
         Width           =   1800
      End
   End
   Begin VB.PictureBox pbStep 
      BorderStyle     =   0  'None
      Height          =   3735
      Index           =   1
      Left            =   0
      ScaleHeight     =   249
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   495
      TabIndex        =   11
      Top             =   0
      Width           =   7425
      Begin VB.Frame fra 
         Height          =   1125
         Index           =   0
         Left            =   2130
         TabIndex        =   12
         Top             =   2070
         Width           =   5115
         Begin VB.CheckBox chkMonth 
            Caption         =   "全选"
            Height          =   495
            Index           =   12
            Left            =   4500
            Style           =   1  'Graphical
            TabIndex        =   33
            Top             =   360
            Width           =   345
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "十二月"
            Height          =   225
            Index           =   11
            Left            =   3420
            TabIndex        =   26
            Tag             =   "12"
            Top             =   780
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "十一月"
            Height          =   225
            Index           =   10
            Left            =   3420
            TabIndex        =   25
            Tag             =   "11"
            Top             =   480
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "十  月"
            Height          =   225
            Index           =   9
            Left            =   3420
            TabIndex        =   24
            Tag             =   "10"
            Top             =   195
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "九  月"
            Height          =   225
            Index           =   8
            Left            =   2340
            TabIndex        =   23
            Tag             =   "9"
            Top             =   798
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "八  月"
            Height          =   225
            Index           =   7
            Left            =   2340
            TabIndex        =   22
            Tag             =   "8"
            Top             =   504
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "七  月"
            Height          =   225
            Index           =   6
            Left            =   2340
            TabIndex        =   21
            Tag             =   "7"
            Top             =   210
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "六  月"
            Height          =   225
            Index           =   5
            Left            =   1260
            TabIndex        =   20
            Tag             =   "6"
            Top             =   810
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "五  月"
            Height          =   225
            Index           =   4
            Left            =   1260
            TabIndex        =   19
            Tag             =   "5"
            Top             =   510
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "四  月"
            Height          =   225
            Index           =   3
            Left            =   1260
            TabIndex        =   18
            Tag             =   "4"
            Top             =   225
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "三  月"
            Height          =   225
            Index           =   2
            Left            =   180
            TabIndex        =   17
            Tag             =   "3"
            Top             =   795
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "二  月"
            Height          =   225
            Index           =   1
            Left            =   180
            TabIndex        =   16
            Tag             =   "2"
            Top             =   510
            Width           =   855
         End
         Begin VB.CheckBox chkMonth 
            Caption         =   "一  月"
            Height          =   225
            Index           =   0
            Left            =   180
            TabIndex        =   15
            Tag             =   "1"
            Top             =   210
            Width           =   855
         End
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "第二步："
         BeginProperty Font 
            Name            =   "楷体_GB2312"
            Size            =   18
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   3
         Left            =   2160
         TabIndex        =   14
         Top             =   360
         Width           =   1515
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "请选择要抽取的月份"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   21.75
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         Height          =   435
         Index           =   2
         Left            =   2310
         TabIndex        =   13
         Top             =   960
         Width           =   3915
      End
      Begin VB.Image imgStep 
         BorderStyle     =   1  'Fixed Single
         Height          =   3450
         Index           =   1
         Left            =   150
         Picture         =   "frmWizard.frx":D53F
         Stretch         =   -1  'True
         Top             =   210
         Width           =   1800
      End
   End
   Begin VB.PictureBox pbStep 
      BorderStyle     =   0  'None
      Height          =   3735
      Index           =   3
      Left            =   0
      ScaleHeight     =   249
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   495
      TabIndex        =   30
      Top             =   0
      Width           =   7425
      Begin VB.TextBox txtSummary 
         BackColor       =   &H00C0C0C0&
         Height          =   1965
         Left            =   2790
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         TabIndex        =   44
         Text            =   "frmWizard.frx":161FD
         Top             =   1050
         Width           =   3945
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "总结："
         BeginProperty Font 
            Name            =   "楷体_GB2312"
            Size            =   18
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   -1  'True
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Index           =   6
         Left            =   2160
         TabIndex        =   31
         Top             =   360
         Width           =   1140
      End
      Begin VB.Image imgStep 
         BorderStyle     =   1  'Fixed Single
         Height          =   3450
         Index           =   3
         Left            =   150
         Picture         =   "frmWizard.frx":16203
         Stretch         =   -1  'True
         Top             =   210
         Width           =   1800
      End
   End
   Begin VB.Line ln 
      BorderColor     =   &H80000010&
      Index           =   1
      X1              =   165
      X2              =   7070
      Y1              =   3840
      Y2              =   3840
   End
   Begin VB.Line ln 
      BorderColor     =   &H80000014&
      Index           =   0
      X1              =   165
      X2              =   7070
      Y1              =   3870
      Y2              =   3870
   End
End
Attribute VB_Name = "frmWizard"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim sFinalSummary As String
Dim lStepRec As Long
Const cntlFirstStep As Long = 0
Const cntlLastStep As Long = 3

'******************************************************
'*自定义函数
'******************************************************
Function AllChecked() As Boolean
Dim l As Long, i As Long

For l = 0 To 11
    If chkMonth(l).Value = vbChecked Then i = i + 1
Next

AllChecked = IIf(i >= 12, True, False)
End Function

Function AllChecked1() As Boolean
Dim l As Long, i As Long

For l = 0 To 2
    If chkModify(l).Value = vbChecked Then i = i + 1
Next

AllChecked1 = IIf(i >= 3, True, False)
End Function

Sub ChkInit()
Dim l As Long, i As Long
i = 1
For l = 1 To 12
    chkMonth(l - 1).Value = IIf(frmMain.lvData.ListItems(l).Checked, vbChecked, vbUnchecked)
    If chkMonth(l - 1).Value = vbChecked Then
        i = i + 1
    End If
Next

chkMonth(12).Value = IIf(i >= 12, vbChecked, vbUnchecked)
    
For l = 1 To 3
    chkModify(l).Value = vbChecked
Next
End Sub
Sub CheckStep()
cmdNext.ZOrder 0
Select Case lStepRec
    Case cntlFirstStep
        cmdBack.Enabled = False
        cmdFinish.Enabled = False
        cmdNext.Enabled = True
    Case cntlLastStep
        cmdNext.Enabled = False
        cmdBack.Enabled = True
        cmdFinish.Enabled = True
        cmdFinish.SetFocus
        cmdFinish.ZOrder 0
    Case Else
        cmdNext.Enabled = True
        cmdBack.Enabled = True
        cmdFinish.Enabled = False
End Select


End Sub
Sub CheckOpt()

Exit Sub
If optView(0).Value = False Then
    optChartType(5).Enabled = False
Else
    optChartType(5).Enabled = True
End If

End Sub

Sub MakeSummary() '月份+数据类型+视图类型+图表类型
Dim lPlus As Long, l As Long
Dim v As Variant
sFinalSummary = "您选择了 "

If AllChecked Then
    sFinalSummary = sFinalSummary + "全年的"
Else
    For Each v In chkMonth
        If v.Value = vbChecked Then
            sFinalSummary = sFinalSummary + v.Caption + "、"
        End If
        If v.Index = 11 Then
            If Right(sFinalSummary, 1) = "、" Then sFinalSummary = Left(sFinalSummary, Len(sFinalSummary) - 1) + "的"
        End If
    Next
End If

If AllChecked1 Then
    sFinalSummary = sFinalSummary + "全部数据"
Else
    For Each v In chkModify
        If v.Value = vbChecked Then
            If v.Index = 3 Then
                sFinalSummary = sFinalSummary + v.Caption
            Else
                sFinalSummary = sFinalSummary + v.Caption + "、"
            End If
        End If
    Next
    sFinalSummary = Left(sFinalSummary, Len(sFinalSummary) - 1)
End If

''If optModify(3).Value = True Then
''    sFinalSummary = sFinalSummary + "所有数据"
''Else
'For Each v In optModify
'    If v.Value = True Then
'        sFinalSummary = sFinalSummary + v.Caption
'        Exit For
'    End If
'Next
''End If


For Each v In optView
    If v.Value = True Then
        sFinalSummary = sFinalSummary + " ,本程序将据此生成" + v.Caption
        Exit For
    End If
Next

If optView(0).Value = True Then
    lPlus = 0
Else
    lPlus = 1
End If

For Each v In optChartType
    If v.Value = True Then
        sFinalSummary = sFinalSummary + v.Caption
        pblCharType = Val(v.Tag) - lPlus
        Exit For
    End If
Next

txtSummary.Text = sFinalSummary
End Sub


'******************************************************
'*窗体函数
'******************************************************
Private Sub chkModify_Click(Index As Integer)
Dim l As Long, lv As Long
With chkModify(Index)
    If Index = 3 Then
        If chkModify(3).Value = vbChecked Then
            lv = vbChecked
            For l = 0 To 3
                chkModify(l).Value = lv
            Next
            chkModify(3).Value = vbUnchecked
        End If
    End If
    If .Value = vbChecked Then
        .Tag = "1"
    Else
        .Tag = "0"
    End If
End With

End Sub
Private Sub chkMonth_Click(Index As Integer)
Dim l As Long, lv As Long
With chkMonth(Index)
    If Index = 12 Then
        If chkMonth(12).Value = vbChecked Then
            lv = vbChecked
            For l = 0 To 11
                chkMonth(l).Value = lv
            Next
            chkMonth(12).Value = vbUnchecked
        End If
    End If
    If .Value = vbUnchecked Then chkMonth(12).Value = vbUnchecked
    If .Value = vbChecked Then
        .Tag = "1"
    Else
        .Tag = "0"
    End If
    
End With
End Sub


Private Sub cmdBack_Click()
On Error Resume Next
lStepRec = lStepRec - 1
pbStep(lStepRec).ZOrder 0
If lStepRec < cntlFirstStep Then lStepRec = cntlFirstStep
CheckStep
CheckOpt
End Sub

Private Sub cmdCancel_Click()
Me.Hide
End Sub

Private Sub cmdFinish_Click()
Me.Hide
frmDiagram.Show vbModal
End Sub

Private Sub cmdNext_Click()
On Error Resume Next
lStepRec = lStepRec + 1
pbStep(lStepRec).ZOrder 0
If lStepRec > cntlLastStep Then lStepRec = cntlLastStep
CheckStep
CheckOpt

If lStepRec = cntlLastStep Then
    MakeSummary
End If
End Sub

Private Sub Form_Activate()
lStepRec = cntlFirstStep
pbStep(cntlFirstStep).ZOrder 0
CheckStep
ChkInit

End Sub

Private Sub optChartType_Click(Index As Integer)
CheckOpt
End Sub

Private Sub optView_Click(Index As Integer)
CheckOpt
If Index = 1 Then
    pbfIs3D = True
Else
    pbfIs3D = False
End If
End Sub
