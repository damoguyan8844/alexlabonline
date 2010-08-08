VERSION 5.00
Begin VB.Form frmTest 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "KBHOOKTESTWINDOW"
   ClientHeight    =   6705
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4710
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   6705
   ScaleWidth      =   4710
   StartUpPosition =   3  '窗口缺省
   Begin VB.ListBox lstMsg 
      Height          =   3300
      Left            =   90
      TabIndex        =   3
      Top             =   2910
      Width           =   4515
   End
   Begin VB.CommandButton cmdUnhook 
      Caption         =   "Unhook"
      Height          =   495
      Left            =   1140
      TabIndex        =   2
      Top             =   2010
      Width           =   2175
   End
   Begin VB.CommandButton cmdHook 
      Caption         =   "Hook"
      Height          =   495
      Left            =   1140
      TabIndex        =   1
      Top             =   1320
      Width           =   2175
   End
   Begin VB.TextBox txtTargetTID 
      Height          =   285
      Left            =   2190
      TabIndex        =   0
      Text            =   "0"
      Top             =   300
      Width           =   975
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "注意：目标线程ID为0时，安装的将是全局钩子"
      ForeColor       =   &H000000FF&
      Height          =   180
      Left            =   540
      TabIndex        =   5
      Top             =   810
      Width           =   3690
   End
   Begin VB.Label Label1 
      Caption         =   "目标线程ID:"
      Height          =   195
      Index           =   0
      Left            =   1140
      TabIndex        =   4
      Top             =   330
      Width           =   1215
   End
End
Attribute VB_Name = "frmTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function InstallHook Lib "kbhook.dll" (ByVal lThreadID As Long) As Long
Private Declare Function UninstallHook Lib "kbhook.dll" () As Long

Private Sub cmdHook_Click()
    On Error Resume Next
    Dim tid As Long
    tid = CLng(Trim(txtTargetTID.Text))
    
    If InstallHook(tid) <> 0 Then
        MsgBox "钩子安装成功！"
    Else
        MsgBox "钩子安装失败！"
    End If
End Sub

Private Sub cmdUnhook_Click()
    Call UninstallHook
    MsgBox "卸载钩子完毕！"
End Sub

Private Sub Form_Load()
    Subclassing Me.hwnd
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Subclassing Me.hwnd, True
End Sub
