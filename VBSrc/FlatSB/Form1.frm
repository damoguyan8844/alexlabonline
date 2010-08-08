VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3975
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3975
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin MSComctlLib.ListView ListView1 
      Height          =   2835
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   4125
      _ExtentX        =   7276
      _ExtentY        =   5001
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      Checkboxes      =   -1  'True
      GridLines       =   -1  'True
      HotTracking     =   -1  'True
      HoverSelection  =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   2
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "a"
         Object.Width           =   2540
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   1
         Text            =   "b"
         Object.Width           =   2540
      EndProperty
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    Dim lBarWidth As Long
    Dim hwnd As Long
    Dim i As Long
    Dim oItem As ListItem
    
    
    For i = 1 To 100
        Set oItem = ListView1.ListItems.Add(, , i)
    Next
    '----------------------------------------------------------------------------------------
    '好戏开场……
    '----------------------------------------------------------------------------------------
    
    
    lBarWidth = 10 '超级无敌大滚动条......
    hwnd = ListView1.hwnd
    
    '子类处理
    glDefWindowProc = SetWindowLong(hwnd, GWL_WNDPROC, AddressOf WindowProc)

    Call InitializeFlatSB(hwnd) '初始化平面滚动条
    FlatSB_SetScrollProp hwnd, WSB_PROP_VSTYLE, FSB_FLAT_MODE, True '设定样式
    FlatSB_SetScrollProp hwnd, WSB_PROP_CXVSCROLL, lBarWidth, True '设定宽度
    FlatSB_ShowScrollBar hwnd, SB_VERT, True  '显示滚动条
    
    EnableColumnHeader hwnd, False
    EnableColumnHeader hwnd, True
End Sub

Private Sub Form_Resize()
    ListView1.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
End Sub

Private Sub Form_Unload(Cancel As Integer)
    
   If glDefWindowProc Then
        '结束子类处理
        SetWindowLong ListView1.hwnd, GWL_WNDPROC, glDefWindowProc
        glDefWindowProc = 0
   End If

   Call UninitializeFlatSB(ListView1.hwnd) '还原滚动条
End Sub

