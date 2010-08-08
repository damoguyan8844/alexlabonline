VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "frmTargetWindow"
   ClientHeight    =   2985
   ClientLeft      =   60
   ClientTop       =   630
   ClientWidth     =   5160
   LinkTopic       =   "Form1"
   ScaleHeight     =   2985
   ScaleWidth      =   5160
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   1035
      Left            =   1110
      TabIndex        =   0
      Top             =   660
      Width           =   2475
   End
   Begin VB.Menu r 
      Caption         =   "r"
      Begin VB.Menu b 
         Caption         =   "b"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub b_Click()
    Call Command1_Click
End Sub

Private Sub Command1_Click()
    MsgBox "hello~~"
End Sub

Private Sub Form_Initialize()
    Call InitExceptionHandler
    
    '在窗体创建前安装挂钩
    If g_oWCHooker.InstallHooker(AddressOf WndHookProc_BeforeCreation, AddressOf WndHookProc_AfterCreation) Then
    End If
End Sub

Private Sub Form_Load()
    '创建完毕，卸载挂钩
    With g_oWCHooker
        If .IsInstalled Then
            .UnInstallHooker
        End If
    End With
    
    
    Me.Caption = "子窗体2"
    Me.WindowState = vbMaximized
End Sub

