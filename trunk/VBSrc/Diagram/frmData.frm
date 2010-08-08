VERSION 5.00
Begin VB.Form frmData 
   Caption         =   "数据修改/添加"
   ClientHeight    =   3075
   ClientLeft      =   3690
   ClientTop       =   2280
   ClientWidth     =   6210
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   ScaleHeight     =   3075
   ScaleWidth      =   6210
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  '所有者中心
   Begin VB.Frame fra 
      Caption         =   "数据单位设置"
      Height          =   1095
      Index           =   2
      Left            =   3900
      TabIndex        =   31
      Top             =   1410
      Width           =   2265
      Begin VB.TextBox txtUnit 
         Height          =   285
         Left            =   990
         TabIndex        =   32
         Top             =   360
         Width           =   735
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "缺省的数据单位为：单位"
         Height          =   180
         Index           =   13
         Left            =   120
         TabIndex        =   34
         Top             =   750
         Width           =   1980
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         Caption         =   "单位："
         Height          =   180
         Index           =   12
         Left            =   420
         TabIndex        =   33
         Top             =   420
         Width           =   540
      End
   End
   Begin VB.Frame fra 
      Caption         =   "请选择要操作的数据类型"
      Height          =   1335
      Index           =   1
      Left            =   3900
      TabIndex        =   27
      Top             =   60
      Width           =   2265
      Begin VB.OptionButton optModify 
         Caption         =   "计划数据"
         Height          =   285
         Index           =   0
         Left            =   480
         TabIndex        =   30
         Top             =   240
         Value           =   -1  'True
         Width           =   1245
      End
      Begin VB.OptionButton optModify 
         Caption         =   "实际数据"
         Height          =   285
         Index           =   1
         Left            =   480
         TabIndex        =   29
         Top             =   615
         Width           =   1245
      End
      Begin VB.OptionButton optModify 
         Caption         =   "预测数据"
         Height          =   285
         Index           =   2
         Left            =   480
         TabIndex        =   28
         Top             =   990
         Width           =   1245
      End
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "确定(&O)"
      Height          =   375
      Left            =   3450
      TabIndex        =   26
      ToolTipText     =   "取消操作"
      Top             =   2610
      Width           =   1275
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "取消(&C)"
      Height          =   375
      Left            =   4875
      TabIndex        =   12
      ToolTipText     =   "取消操作"
      Top             =   2610
      Width           =   1275
   End
   Begin VB.Frame fra 
      Caption         =   "数据窗口"
      Height          =   2445
      Index           =   0
      Left            =   45
      TabIndex        =   13
      Top             =   60
      Width           =   3795
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   0
         Left            =   810
         TabIndex        =   0
         Text            =   "Text1"
         Top             =   315
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   1
         Left            =   810
         TabIndex        =   1
         Text            =   "Text1"
         Top             =   633
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   2
         Left            =   810
         TabIndex        =   2
         Text            =   "Text1"
         Top             =   951
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   3
         Left            =   810
         TabIndex        =   3
         Text            =   "Text1"
         Top             =   1269
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   4
         Left            =   810
         TabIndex        =   4
         Text            =   "Text1"
         Top             =   1587
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   5
         Left            =   810
         TabIndex        =   5
         Text            =   "Text1"
         Top             =   1905
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   6
         Left            =   2610
         TabIndex        =   6
         Text            =   "Text1"
         Top             =   345
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   7
         Left            =   2610
         TabIndex        =   7
         Text            =   "Text1"
         Top             =   663
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   8
         Left            =   2610
         TabIndex        =   8
         Text            =   "Text1"
         Top             =   981
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   9
         Left            =   2610
         TabIndex        =   9
         Text            =   "Text1"
         Top             =   1299
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   10
         Left            =   2610
         TabIndex        =   10
         Text            =   "Text1"
         Top             =   1617
         Width           =   975
      End
      Begin VB.TextBox txtMonthlyData 
         Height          =   270
         Index           =   11
         Left            =   2610
         TabIndex        =   11
         Text            =   "Text1"
         Top             =   1935
         Width           =   975
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "一  月："
         Height          =   180
         Index           =   0
         Left            =   150
         TabIndex        =   25
         Top             =   360
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "二  月："
         Height          =   180
         Index           =   1
         Left            =   150
         TabIndex        =   24
         Top             =   678
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "三  月："
         Height          =   180
         Index           =   2
         Left            =   150
         TabIndex        =   23
         Top             =   996
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "四  月："
         Height          =   180
         Index           =   3
         Left            =   150
         TabIndex        =   22
         Top             =   1314
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "五  月："
         Height          =   180
         Index           =   4
         Left            =   150
         TabIndex        =   21
         Top             =   1632
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "六  月："
         Height          =   180
         Index           =   5
         Left            =   150
         TabIndex        =   20
         Top             =   1950
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "七  月："
         Height          =   180
         Index           =   6
         Left            =   1935
         TabIndex        =   19
         Top             =   390
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "八  月："
         Height          =   180
         Index           =   7
         Left            =   1935
         TabIndex        =   18
         Top             =   720
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "九  月："
         Height          =   180
         Index           =   8
         Left            =   1935
         TabIndex        =   17
         Top             =   1020
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "十  月："
         Height          =   180
         Index           =   9
         Left            =   1935
         TabIndex        =   16
         Top             =   1335
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "十一月："
         Height          =   180
         Index           =   10
         Left            =   1935
         TabIndex        =   15
         Top             =   1665
         Width           =   720
      End
      Begin VB.Label lbl 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "十二月："
         Height          =   180
         Index           =   11
         Left            =   1935
         TabIndex        =   14
         Top             =   1980
         Width           =   720
      End
   End
End
Attribute VB_Name = "frmData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim iOpt As Integer
Dim fChanged As Boolean
Dim fUnitChanged As Boolean
'******************************************************
'*自定义函数
'******************************************************

'初始化文本框
Sub InitTextBox()
Dim l As Long, s As String
For l = 0 To 11
    s = Trim(CStr(pbasngDataOperating(l, iOpt)))
    txtMonthlyData(l).Text = IIf(Left(s, 1) = ".", "0" & s, s)
    txtMonthlyData(l).Enabled = frmMain.lvData.ListItems(l + 1).Checked
Next


End Sub
Sub InitTmpArray()
Dim i As Long, l As Long
If pbfEditMode = True Then
    For i = 0 To 11
        For l = 0 To 2
            pbasngDataOperating(i, l) = CSng(frmMain.lvData.ListItems(i + 1).SubItems(l + 1))
        Next
    Next
End If
InitTextBox
End Sub
'根据不同的操作修改本窗体标题
Sub OptionSelecting()
On Error Resume Next
Dim spre As String, l As Long, i As Long
If pbfEditMode = True Then
    spre = "修改"
Else
    spre = "添加"
End If

frmData.Caption = spre + pbasDataType(iOpt + 1)

InitTextBox
End Sub
'******************************************************
'*窗体函数
'******************************************************

Private Sub cmdCancel_Click()
 Me.Hide
End Sub

Private Sub cmdOK_Click()
On Error GoTo staErr
Dim l As Long, i As Long, v As Variant, a(2) As Single, sTmp As String
If fChanged Then
    If MsgBox("您真的想更改数据么?", vbYesNo + vbQuestion + vbDefaultButton2) = vbYes Then
        Set pbcolMonthlyData = New Collection
        For i = 0 To 11
            For l = 0 To 2
                a(l) = pbasngDataOperating(i, l)
                With frmMain.lvData.ListItems(i + 1)
                    If .Checked Then
                        .SubItems(l + 1) = Trim((a(l)))
                    End If
                End With
            Next
            pbcolMonthlyData.Add a, pbasMonth(i)
        Next
    Else
        Exit Sub
    End If
End If

pbfEditMode = True
pbfInit = False
Me.Hide
Exit Sub
staErr:
If Err.Number = 5 Then Resume Next
pbfEditMode = False
End Sub

Private Sub Form_Activate()
optModify(0).Value = True
InitTmpArray
fChanged = False
fUnitChanged = False
End Sub


Private Sub optModify_Click(Index As Integer)
iOpt = Index
OptionSelecting
End Sub

Private Sub txtMonthlyData_Change(Index As Integer)
fChanged = True
End Sub

Private Sub txtMonthlyData_KeyPress(Index As Integer, KeyAscii As Integer)
'文本框除数字、负号、小数点外其他一概不收
Dim i As Integer
i = KeyAscii
If (i >= 32 And i <= 44) Or i = 47 Or i >= 58 Then
    KeyAscii = 0
End If
Select Case i
Case Asc("-")
    If InStr(txtMonthlyData(Index).Text, "-") <> 0 Then KeyAscii = 0
Case Asc(".")
    If InStr(txtMonthlyData(Index).Text, ".") <> 0 Then KeyAscii = 0
End Select
End Sub

Private Sub txtMonthlyData_LostFocus(Index As Integer)
Call txtMonthlyData_Validate(Index, True)
End Sub

Private Sub txtMonthlyData_Validate(Index As Integer, Cancel As Boolean)
On Error Resume Next
Dim sTmp As String, lDotPos As Long, lMinusPos As Long, lLen As Long
sTmp = Trim(txtMonthlyData(Index).Text)
lLen = Len(sTmp)
lDotPos = InStr(sTmp, ".")
lMinusPos = InStr(sTmp, "-")

'.- , -. Abs(lDotPos - lMinusPos) = 1
'0-,0. Not IsFigure(Right(sTmp, 1))
'.0 Left(sTmp, 1) = "."
'0-0 left()
'

If (lDotPos <> 0 And lMinusPos <> 0 And Abs(lDotPos - lMinusPos) = 1) Then GoTo staAssert
If Not IsFigure(Right(sTmp, 1)) Then GoTo staAssert
If Left(sTmp, 1) = "." Then GoTo staAssert
If lMinusPos <> 0 And lMinusPos <> 1 Then GoTo staAssert

pbasngDataOperating(Index, iOpt) = CSng(txtMonthlyData(Index).Text)
Exit Sub
staAssert:
MsgBox "您输入的数字有误,请检查一下。", vbExclamation
Cancel = True
End Sub

Private Sub txtUnit_Change()
fUnitChanged = True
End Sub

Private Sub txtUnit_LostFocus()
If fUnitChanged = False Then Exit Sub
If MsgBox("将数据单位从" + pbsDataUnit + "改为" + txtUnit.Text + "吗?", vbYesNo + vbQuestion + vbDefaultButton2) = vbYes Then
    pbsDataUnit = Trim(txtUnit.Text)
Else
    txtUnit.Text = pbsDataUnit
End If

End Sub

