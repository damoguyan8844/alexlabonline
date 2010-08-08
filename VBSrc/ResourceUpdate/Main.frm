VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   5445
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8280
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   5445
   ScaleWidth      =   8280
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton cmdDelRes 
      Caption         =   "<-删除<-"
      Height          =   375
      Left            =   3570
      TabIndex        =   9
      Top             =   4920
      Width           =   1230
   End
   Begin VB.CommandButton cmdAddRes 
      Caption         =   "<-添加<-"
      Height          =   375
      Left            =   3570
      TabIndex        =   8
      Top             =   4410
      Width           =   1230
   End
   Begin VB.CommandButton cmdReplace 
      Caption         =   "<-替换<-"
      Height          =   375
      Left            =   3570
      TabIndex        =   7
      Top             =   3900
      Width           =   1230
   End
   Begin VB.CommandButton cmdOpenFile 
      Caption         =   "打开->(&R)"
      Height          =   375
      Index           =   1
      Left            =   4230
      TabIndex        =   6
      Top             =   2490
      Width           =   1230
   End
   Begin VB.CommandButton cmdView 
      Caption         =   "查看(&V)"
      Height          =   375
      Index           =   1
      Left            =   4230
      TabIndex        =   4
      Top             =   2070
      Visible         =   0   'False
      Width           =   1230
   End
   Begin VB.CommandButton cmdView 
      Caption         =   "查看(&V)"
      Height          =   375
      Index           =   0
      Left            =   2820
      TabIndex        =   3
      Top             =   2070
      Visible         =   0   'False
      Width           =   1230
   End
   Begin VB.PictureBox pbPreview 
      AutoRedraw      =   -1  'True
      Height          =   1980
      Left            =   3030
      ScaleHeight     =   1920
      ScaleWidth      =   2145
      TabIndex        =   2
      Top             =   30
      Width           =   2205
   End
   Begin MSComctlLib.TreeView tvRes 
      Height          =   5370
      Index           =   0
      Left            =   60
      TabIndex        =   1
      Top             =   15
      Width           =   2715
      _ExtentX        =   4789
      _ExtentY        =   9472
      _Version        =   393217
      HideSelection   =   0   'False
      Indentation     =   617
      LabelEdit       =   1
      LineStyle       =   1
      Style           =   7
      Appearance      =   1
      OLEDropMode     =   1
   End
   Begin VB.CommandButton cmdOpenFile 
      Caption         =   "<-打开(&L)"
      Height          =   375
      Index           =   0
      Left            =   2820
      TabIndex        =   0
      Top             =   2490
      Width           =   1230
   End
   Begin MSComctlLib.TreeView tvRes 
      Height          =   5370
      Index           =   1
      Left            =   5490
      TabIndex        =   5
      Top             =   0
      Width           =   2715
      _ExtentX        =   4789
      _ExtentY        =   9472
      _Version        =   393217
      HideSelection   =   0   'False
      Indentation     =   617
      LabelEdit       =   1
      LineStyle       =   1
      Style           =   7
      Appearance      =   1
      OLEDropMode     =   1
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 超级绿豆
'
' ************************   模 块 名   :frmMain********************
'版 本 号: V1.0
'作    者: 超级绿豆
'生成日期: 2004年03月09日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月09日
'修改人员: 超级绿豆
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "frmMain"



Private Sub cmdAddRes_Click()
    On Error GoTo Error_Handler
    Dim oResPackageDest As CResPackage
    Dim oResEntrySrc As CResEntry
    Dim fResult As Boolean
    
    Set oResEntrySrc = GetResEntryFromNode(tvRes(1), g_oResLib(1))
    
    If oResEntrySrc Is Nothing Then
        MsgBox "moo..."
        Exit Sub
    End If
    Set oResPackageDest = g_oResLib(0).ResPackage
    fResult = oResPackageDest.OpAddRes(oResEntrySrc)
    
    If Not fResult Then
        MsgBox "moo..."
    Else
        MsgBox "ok"
        LoadResFile oResPackageDest.FileName, 0
        
    End If
    Exit Sub
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Sub

Private Sub cmdDelRes_Click()
    Dim oResEntry As CResEntry
    Set oResEntry = GetResEntryFromNode(tvRes(0), g_oResLib(0))
    
    If oResEntry Is Nothing Then
        Exit Sub
    End If
    
    If Not oResEntry.OpDelRes Then
        MsgBox "moo..."
    Else
        MsgBox "ok"
        LoadResFile oResEntry.FileName, 0
    End If
End Sub

Private Sub cmdOpenFile_Click(Index As Integer)
    Dim sFileName As String
    sFileName = GetOpenFile(Me, "*.*|*.*")
    If Len(sFileName) = 0 Then Exit Sub
    
    If Index = 1 Then
        If LCase(Right$(sFileName, 4)) = ".ico" Then
            LoadIconFile sFileName
            g_fIsIconMode = True
            Exit Sub
        End If
        g_fIsIconMode = False
    End If
    
    LoadResFile sFileName, Index
End Sub

Private Sub cmdReplace_Click()
    On Error GoTo Error_Handler
    Dim oResEntrySrc As CResEntry
    Dim oResEntryDest As CResEntry
    Dim fResult As Boolean
    Set oResEntryDest = GetResEntryFromNode(tvRes(0), g_oResLib(0))
    
    If oResEntryDest Is Nothing Then
        MsgBox "moo..."
        Exit Sub
    End If
    
    
    If g_fIsIconMode Then
        Dim oIconEntry As CIconEntry
        Set oIconEntry = g_oIconLib.IconPackage.IconEntries.IconEntry(tvRes(1).SelectedItem.Key)
        If Not oIconEntry Is Nothing Then
            fResult = oResEntryDest.OpReplaceResUsingRawBytes(oIconEntry)
        End If
    Else
        Set oResEntrySrc = GetResEntryFromNode(tvRes(1), g_oResLib(1))
        
        If oResEntrySrc Is Nothing Or oResEntryDest Is Nothing Then
            MsgBox "moo..."
            Exit Sub
        End If
        fResult = oResEntryDest.OpReplaceRes(oResEntrySrc)
    End If
        
    If Not fResult Then
        MsgBox "moo..."
    Else
        MsgBox "ok"
        LoadResFile oResEntryDest.FileName, 0
        
    End If
    Exit Sub
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Sub

Private Sub cmdView_Click(Index As Integer)
    If g_fIsIconMode And Index = 1 Then
        Dim oIconEntry As CIconEntry
        Set oIconEntry = g_oIconLib.IconPackage.IconEntries.IconEntry(tvRes(1).SelectedItem.Key)
        If Not oIconEntry Is Nothing Then
            pbPreview.Cls
            oIconEntry.OpDrawIcon pbPreview.hDC
        End If
        Exit Sub
    End If
    
    Dim sStr As String
    Dim oResEntry As CResEntry
    Set oResEntry = GetResEntryFromNode(tvRes(Index), g_oResLib(Index))
    If oResEntry Is Nothing Then
        Exit Sub
    End If
    
    pbPreview.Cls
    
    Select Case oResEntry.ResTypeLng
    Case RT_ICON, RT_ANIICON
        If Not oResEntry.OpDrawIcon(pbPreview.hDC) Then
            MsgBox "moo..."
        End If
    Case RT_GROUP_ICON
        If Not oResEntry.OpDrawIcon(pbPreview.hDC, True, True) Then
            MsgBox "moo..."
        End If
    Case RT_BITMAP
        pbPreview.ScaleMode = vbPixels
        If Not oResEntry.OpDrawBitmap(pbPreview.hDC, pbPreview.ScaleWidth, pbPreview.ScaleHeight) Then
            MsgBox "moo..."
        End If
    Case RT_CURSOR, RT_ANICURSOR
        If Not oResEntry.OpDrawIcon(pbPreview.hDC, False) Then
            MsgBox "moo..."
        End If
    Case RT_GROUP_CURSOR
        If Not oResEntry.OpDrawIcon(pbPreview.hDC, False, True) Then
            MsgBox "moo..."
        End If
    Case RT_STRING
        If oResEntry.OpGetString(sStr) Then
            pbPreview.Print sStr
        Else
            MsgBox "moo..."
        End If
    Case RT_DIALOG
        If oResEntry.OpShowDialog(frmMain.hWnd) Then
        End If
    Case RT_VERSION
        If oResEntry.OpGetVersion(sStr) Then
            pbPreview.Print sStr
        Else
            MsgBox "moo..."
        End If
    End Select
End Sub


Private Sub Form_Load()
    Call InitExceptionHandler
    
    Set g_oResLib(0) = New CResLib
    Set g_oResLib(1) = New CResLib
    
    LoadResFile "D:\temp\dest.exe", 0
    LoadResFile "D:\temp\src.exe", 1
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set g_oResLib(0) = Nothing
    Set g_oResLib(1) = Nothing
End Sub


Private Sub tvRes_NodeClick(Index As Integer, ByVal Node As MSComctlLib.Node)
    Call cmdView_Click(Index)
End Sub


'**********************************************************************************************************************************************************
'**********************************************************************************************************************************************************
'**********************************************************************************************************************************************************
'**********************************************************************************************************************************************************
'**********************************************************************************************************************************************************
'**********************************************************************************************************************************************************
Function AddToTreeView(tvObj As TreeView, oResLib As CResLib) As Boolean
    Dim oResEntries As CResEntries
    Dim oResEntry As CResEntry
    Dim oNode As Node, oNodeChild As Node
    Dim oNodes As Nodes
    
    If oResLib.ResPackage Is Nothing Then Exit Function
    
    tvObj.Visible = False
    
    Set oNodes = tvObj.Nodes
    
    oNodes.Clear
    
    For Each oResEntries In oResLib.ResPackage
        Set oNode = oNodes.Add(Key:=oResEntries.ItemKey, Text:=oResEntries.ResTypeOfEntries)
        For Each oResEntry In oResEntries
            With oResEntry
                Set oNodeChild = oNodes.Add(Relative:=oNode, Relationship:=tvwChild, Key:=.ItemKey, Text:=.ResNameStr & " " & .LangStr & " :" & CStr(.ResSize))
            End With
        Next
        'oNode.Expanded = True
    Next
    
    tvObj.Visible = True
End Function

Function AddToTreeViewIcon(tvObj As TreeView, oIconLib As CIconLib) As Boolean
    Dim oIconEntry As CIconEntry
    Dim oNode As Node
    Dim oNodes As Nodes
    
    If oIconLib.IconPackage Is Nothing Then Exit Function
    
    tvObj.Visible = False
    
    Set oNodes = tvObj.Nodes
    
    oNodes.Clear
    
    Set oNode = oNodes.Add(Key:=oIconLib.FileName, Text:=oIconLib.FileName)
    
    If oIconLib.IconPackage.IconEntries.Count > 0 Then
        For Each oIconEntry In oIconLib.IconPackage.IconEntries
            With oIconEntry
                Call oNodes.Add(Relative:=oNode, Relationship:=tvwChild, Key:=.ItemKey, Text:=.IconName & "(" & CStr(.IconWidth) & "x" & CStr(.IconHeight) & ":" & CStr(.IconSize) & ")")
            End With
        Next
    Else
        Call oNodes.Add(Relative:=oNode, Relationship:=tvwChild, Key:="K:Nothing", Text:="(无有效图标)")
    End If
    oNode.Expanded = True
    tvObj.Visible = True
End Function
Function GetResEntryFromNode(tvObj As TreeView, oResLib As CResLib) As CResEntry
    Dim oResEntries As CResEntries
    Dim oResEntry As CResEntry
    Dim oNode As Node
    Dim sKey1 As String, sKey2 As String
    
    Set GetResEntryFromNode = Nothing
    
    Set oNode = tvObj.SelectedItem
    If oNode.Parent Is Nothing Then
        Exit Function
    End If
    
    sKey1 = oNode.Key
    
    Set oNode = oNode.Parent
    
    sKey2 = oNode.Key
    
    Set oResEntries = oResLib.ResPackage.OneResEntries(KeyOfResEntries(sKey2))
    
    If oResEntries Is Nothing Then
        Exit Function
    End If
    
    Set oResEntry = oResEntries(KeyOfResEntry(sKey1))
    
    If oResEntry Is Nothing Then
        Exit Function
    End If
    
    Set GetResEntryFromNode = oResEntry
End Function

Function KeyOfResEntries(sKey As String) As String
    On Error Resume Next
    KeyOfResEntries = Replace(sKey, "K:", "")
End Function

Function KeyOfResEntry(sKey As String) As String
    On Error Resume Next
    KeyOfResEntry = Mid(sKey, InStrRev(sKey, "!") + 1)
End Function

Private Sub tvRes_OLEDragDrop(Index As Integer, Data As MSComctlLib.DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Data.GetFormat(15) Then
        LoadResFile Data.Files(1), Index
    End If
End Sub

Function LoadResFile(ByVal sResFile As String, ByVal Index As Integer) As Boolean

    If Not FileExists(sResFile) Then
        Exit Function
    End If
        
    Set g_oResLib(Index) = Nothing
    Set g_oResLib(Index) = New CResLib
    
    g_oResLib(Index).FileName = sResFile
    If g_oResLib(Index).CreateResList Then
        AddToTreeView tvRes(Index), g_oResLib(Index)
    End If
End Function

Function LoadIconFile(ByVal sIconFile As String) As Boolean

    If Not FileExists(sIconFile) Then
        Exit Function
    End If
        
    Set g_oIconLib = Nothing
    Set g_oIconLib = New CIconLib
    
    g_oIconLib.FileName = sIconFile
    If g_oIconLib.CreateIconList Then
        AddToTreeViewIcon tvRes(1), g_oIconLib
    End If
End Function


