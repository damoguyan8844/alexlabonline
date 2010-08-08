VERSION 5.00
Object = "{0ECD9B60-23AA-11D0-B351-00A0C9055D8E}#6.0#0"; "MSHFLXGD.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6435
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10020
   LinkTopic       =   "Form1"
   ScaleHeight     =   6435
   ScaleWidth      =   10020
   StartUpPosition =   3  '窗口缺省
   Begin MSHierarchicalFlexGridLib.MSHFlexGrid MSHFlexGrid1 
      Height          =   5295
      Left            =   4080
      TabIndex        =   1
      Top             =   360
      Width           =   5415
      _ExtentX        =   9551
      _ExtentY        =   9340
      _Version        =   393216
      _NumberOfBands  =   1
      _Band(0).Cols   =   2
   End
   Begin VB.TextBox MfgMonth 
      Height          =   375
      Left            =   600
      TabIndex        =   0
      Top             =   360
      Width           =   2775
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
'**************************************************************************************************************
'--------------------------------------------------------------------------------------------------------------
'--------------------------------------------------------------------------------------------------------------
'开始对lhWnd窗口进行消息捕获
Sub BeginMouseWheelCapture(ByVal lhWnd As Long)
    g_lDefWindowProc = SetWindowLong(lhWnd, GWL_WNDPROC, AddressOf WindowProc)
End Sub

'结束对lhWnd窗口的消息捕获
Sub EndMouseWheelCapture(ByVal lhWnd As Long)
    g_lDefWindowProc = SetWindowLong(lhWnd, GWL_WNDPROC, g_lDefWindowProc)
End Sub
'--------------------------------------------------------------------------------------------------------------
'--------------------------------------------------------------------------------------------------------------


'--------------------------------------------------------------------------------------------------------------
'--------------------------------------------------------------------------------------------------------------
Private Sub MfgMonth_GotFocus()
    Call BeginMouseWheelCapture(MfgMonth.hwnd)
End Sub

Private Sub MfgMonth_LostFocus()
    Call EndMouseWheelCapture(MfgMonth.hwnd)
End Sub
'--------------------------------------------------------------------------------------------------------------
'--------------------------------------------------------------------------------------------------------------


'--------------------------------------------------------------------------------------------------------------
'--------------------------------------------------------------------------------------------------------------
Private Sub MSHFlexGrid1_GotFocus()
    Call BeginMouseWheelCapture(MSHFlexGrid1.hwnd)
End Sub

Private Sub MSHFlexGrid1_LostFocus()
    Call EndMouseWheelCapture(MSHFlexGrid1.hwnd)
End Sub
'--------------------------------------------------------------------------------------------------------------
'--------------------------------------------------------------------------------------------------------------
'**************************************************************************************************************

Private Sub Form_Load()
    Dim i As Long
    With MSHFlexGrid1
        .Cols = 3
        .Rows = 50
        For i = 1 To 49
            .TextMatrix(i, 1) = i
        Next
    End With
End Sub


