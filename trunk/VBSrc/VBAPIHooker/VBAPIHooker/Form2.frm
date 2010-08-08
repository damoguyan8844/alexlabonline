VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "API钩子示例 - 右击导入函数表框后使用菜单"
   ClientHeight    =   9510
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9720
   Icon            =   "Form2.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   9510
   ScaleWidth      =   9720
   StartUpPosition =   2  '屏幕中心
   Begin MSComctlLib.ImageList iml 
      Left            =   4260
      Top             =   120
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   2
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form2.frx":08CA
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form2.frx":11A4
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.Frame Frame2 
      Caption         =   "调用信息"
      Height          =   2055
      Left            =   90
      TabIndex        =   10
      Top             =   7350
      Width           =   9525
      Begin VB.ListBox List1 
         Height          =   1320
         Left            =   4860
         TabIndex        =   13
         Top             =   570
         Width           =   4545
      End
      Begin VB.TextBox Text1 
         Height          =   1305
         Left            =   180
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   11
         Top             =   570
         Width           =   4545
      End
      Begin VB.Label Label1 
         Caption         =   "调用信息："
         Height          =   285
         Index           =   0
         Left            =   4860
         TabIndex        =   14
         Top             =   330
         Width           =   1365
      End
      Begin VB.Label Label1 
         Caption         =   "堆栈内容："
         Height          =   285
         Index           =   1
         Left            =   180
         TabIndex        =   12
         Top             =   330
         Width           =   1365
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "模块信息"
      Height          =   4935
      Left            =   90
      TabIndex        =   5
      Top             =   2280
      Width           =   9525
      Begin MSComctlLib.ListView lvExportList 
         Height          =   1455
         Left            =   90
         TabIndex        =   6
         Top             =   570
         Width           =   9315
         _ExtentX        =   16431
         _ExtentY        =   2566
         LabelEdit       =   1
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         FullRowSelect   =   -1  'True
         GridLines       =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   0
      End
      Begin MSComctlLib.ListView lvImportList 
         Height          =   2505
         Left            =   90
         TabIndex        =   7
         Top             =   2340
         Width           =   9315
         _ExtentX        =   16431
         _ExtentY        =   4419
         LabelEdit       =   1
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         FullRowSelect   =   -1  'True
         GridLines       =   -1  'True
         _Version        =   393217
         Icons           =   "iml"
         SmallIcons      =   "iml"
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   0
      End
      Begin VB.Label lblExportCount 
         AutoSize        =   -1  'True
         Height          =   180
         Left            =   120
         TabIndex        =   9
         Top             =   360
         Width           =   90
      End
      Begin VB.Label lblImportCount 
         AutoSize        =   -1  'True
         Height          =   180
         Left            =   90
         TabIndex        =   8
         Top             =   2100
         Width           =   90
      End
   End
   Begin VB.CommandButton cmdRefresh 
      Appearance      =   0  'Flat
      Height          =   405
      Left            =   2970
      Picture         =   "Form2.frx":1A7E
      Style           =   1  'Graphical
      TabIndex        =   4
      ToolTipText     =   "刷新进程列表"
      Top             =   0
      Width           =   375
   End
   Begin MSComctlLib.ListView lvProcList 
      Height          =   1785
      Left            =   180
      TabIndex        =   0
      Top             =   390
      Width           =   3195
      _ExtentX        =   5636
      _ExtentY        =   3149
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      FullRowSelect   =   -1  'True
      GridLines       =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   0
   End
   Begin MSComctlLib.ListView lvModList 
      Height          =   1785
      Left            =   3510
      TabIndex        =   1
      Top             =   390
      Width           =   6015
      _ExtentX        =   10610
      _ExtentY        =   3149
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      FullRowSelect   =   -1  'True
      GridLines       =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   0
   End
   Begin VB.Label lblModCount 
      AutoSize        =   -1  'True
      Height          =   180
      Left            =   3540
      TabIndex        =   3
      Top             =   120
      Width           =   90
   End
   Begin VB.Label lblProcCount 
      AutoSize        =   -1  'True
      Height          =   180
      Left            =   210
      TabIndex        =   2
      Top             =   120
      Width           =   90
   End
   Begin VB.Menu mnuPopout 
      Caption         =   "Popout"
      Visible         =   0   'False
      Begin VB.Menu mnuHook 
         Caption         =   "安装钩子"
      End
      Begin VB.Menu mnuUnhook 
         Caption         =   "卸载钩子"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function GetCurrentProcess Lib "kernel32" () As Long

Private Declare Function OpenProcessToken Lib "advapi32" (ByVal ProcessHandle As Long, ByVal DesiredAccess As Long, TokenHandle As Long) As Long
     
Private Declare Function LookupPrivilegeValue Lib "advapi32" Alias "LookupPrivilegeValueA" (ByVal lpSystemName As String, ByVal lpName As String, lpLuid As LUID) As Long
     
Private Declare Function AdjustTokenPrivileges Lib "advapi32" (ByVal TokenHandle As Long, ByVal DisableAllPrivileges As Long, NewState As TOKEN_PRIVILEGES, ByVal BufferLength As Long, PreviousState As TOKEN_PRIVILEGES, ReturnLength As Long) As Long

Private Type LUID
    UsedPart As Long
    IgnoredForNowHigh32BitPart As Long
End Type

Private Type LUID_AND_ATTRIBUTES
    TheLuid As LUID
    Attributes As Long
End Type

Private Type TOKEN_PRIVILEGES
    PrivilegeCount As Long
    TheLuid As LUID
    Attributes As Long
End Type

Private Const TOKEN_ADJUST_PRIVILEGES = &H20
Private Const TOKEN_QUERY = &H8

Private Const SE_PRIVILEGE_ENABLED = &H2

Private Declare Function CreateToolhelp32Snapshot Lib "kernel32" (ByVal dwFlags As Long, ByVal th32ProcessID As Long) As Long
Private Const TH32CS_SNAPPROCESS = &H2
Private Const TH32CS_SNAPMODULE = &H8

Private Declare Function Module32First Lib "kernel32" (ByVal hSnapShot As Long, lpme As MODULEENTRY32) As Long
Private Declare Function Module32Next Lib "kernel32" (ByVal hSnapShot As Long, lpme As MODULEENTRY32) As Long

Private Declare Function Process32First Lib "kernel32" (ByVal hSnapShot As Long, lppe As PROCESSENTRY32) As Long
Private Declare Function Process32Next Lib "kernel32" (ByVal hSnapShot As Long, lppe As PROCESSENTRY32) As Long

Private Type MODULEENTRY32
    dwSize As Long
    th32ModuleID As Long
    th32ProcessID As Long
    GlblcntUsage As Long
    ProccntUsage As Long
    modBaseAddr As Long
    modBaseSize As Long
    hModule As Long
    szModule  As String * 256
    szExePath As String * 260
End Type

Private Type PROCESSENTRY32
    dwSize As Long
    cntUsage As Long
    th32ProcessID As Long
    th32DefaultHeapID As Long
    th32ModuleID As Long
    cntThreads As Long
    th32ParentProcessID As Long
    pcPriClassBase As Long
    dwFlags  As Long
    szExePath As String * 260
End Type

Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private Const HOOKED_COLOR As Long = &HFF

Private m_atModEntries() As MODULEENTRY32
Private m_atProcEntries() As PROCESSENTRY32
Private WithEvents m_oApiHooker  As CVBAPIHooker
Attribute m_oApiHooker.VB_VarHelpID = -1

Private Function GetModEntries(ByVal lProcessID As Long, atModEntries() As MODULEENTRY32) As Boolean
    On Error GoTo Error_Handler
    Dim hSnapShot As Long
    Dim i As Long
    
    Erase atModEntries
    
    If lProcessID = 0 Or lProcessID = 4 Then
        Exit Function
    End If
    
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, lProcessID)
    
    If hSnapShot <> -1 Then
        i = 0
        ReDim atModEntries(i)
        atModEntries(i).dwSize = LenB(atModEntries(i))
        If Module32First(hSnapShot, atModEntries(i)) <> 0 Then
            Do
                i = i + 1
                ReDim Preserve atModEntries(i)
                atModEntries(i).dwSize = LenB(atModEntries(i))
                If Module32Next(hSnapShot, atModEntries(i)) = 0 Then
                    ReDim Preserve atModEntries(i - 1)
                    Exit Do
                End If
            Loop
        End If
        CloseHandle hSnapShot
        GetModEntries = True
    End If
    Exit Function
Error_Handler:
    If hSnapShot <> -1 Then CloseHandle (hSnapShot)
End Function

Private Function GetProcEntries(atProcEntries() As PROCESSENTRY32) As Boolean
    On Error GoTo Error_Handler
    Dim hSnapShot As Long
    Dim i As Long
    
    Erase atProcEntries
    
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)
    
    If hSnapShot <> -1 Then
        i = 0
        ReDim atProcEntries(i)
        atProcEntries(i).dwSize = LenB(atProcEntries(i))
        If Process32First(hSnapShot, atProcEntries(i)) <> 0 Then
            Do
                i = i + 1
                ReDim Preserve atProcEntries(i)
                atProcEntries(i).dwSize = LenB(atProcEntries(i))
                If Process32Next(hSnapShot, atProcEntries(i)) = 0 Then
                    ReDim Preserve atProcEntries(i - 1)
                    Exit Do
                End If
            Loop
        End If
        CloseHandle hSnapShot
        GetProcEntries = True
    End If
    Exit Function
Error_Handler:
    If hSnapShot <> -1 Then CloseHandle (hSnapShot)
End Function

Private Function FillModList(atModEntries() As MODULEENTRY32) As Boolean
    Dim i As Long
    Dim itm As ListItem
    
    lblModCount.Caption = "共有0个模块"
    With lvModList
        .Visible = False

        If .ColumnHeaders.Count = 0 Then
            .ColumnHeaders.Add , "name", "模块名"
            .ColumnHeaders.Add(, "path", "模块路径名").Width = 5000
            .ColumnHeaders.Add , "handle", "模块句柄"
            .ColumnHeaders.Add , "size", "模块大小"
            .View = lvwReport
        End If
        
        For i = LBound(atModEntries) To UBound(atModEntries)
            Set itm = .ListItems.Add(, "K" & CStr(atModEntries(i).hModule), UCase$(atModEntries(i).szModule))
            
            itm.SubItems(1) = UCase$(atModEntries(i).szExePath)
            itm.SubItems(2) = Right$("00000000" & Hex(atModEntries(i).hModule), 8)
            itm.SubItems(3) = Right$("00000000" & Hex(atModEntries(i).modBaseSize), 8)
        Next
                
        .Visible = True
        lblModCount.Caption = "共有" & .ListItems.Count & "个模块"
        If .ListItems.Count > 0 Then
            lvModList_ItemClick .ListItems(1)
        End If
    End With
End Function

Private Function FillProcList(atProcEntries() As PROCESSENTRY32) As Boolean
    Dim i As Long
    Dim itm As ListItem
    
    lblProcCount.Caption = "共有0个进程"
    With lvProcList
        .Visible = False
        
        If .ColumnHeaders.Count = 0 Then
            .ColumnHeaders.Add , "pid", "进程ID"
            .ColumnHeaders.Add(, "name", "进程名").Width = 10000
            .View = lvwReport
        End If
        .ListItems.Clear
        
        For i = LBound(atProcEntries) To UBound(atProcEntries)
            Set itm = .ListItems.Add(, "K" & CStr(atProcEntries(i).th32ProcessID), Right$("00000000" & Hex(atProcEntries(i).th32ProcessID), 8))
            
            itm.SubItems(1) = UCase$(atProcEntries(i).szExePath)
        Next
        
        .Visible = True
        lblProcCount.Caption = "共有" & .ListItems.Count & "个进程"
        
        If .ListItems.Count > 0 Then
            lvProcList_ItemClick .ListItems(1)
        End If
    End With
End Function

Private Function FillExportList(avExports() As Variant) As Boolean
    On Error Resume Next
    Dim i As Long
    Dim itm As ListItem
    
    With lvExportList
        .Visible = False
        
        If .ColumnHeaders.Count = 0 Then
            .ColumnHeaders.Add , "ordinal", "函数导出序号"
            .ColumnHeaders.Add(, "name", "函数名").Width = 4000
            .ColumnHeaders.Add , "entryaddr", "函数导出表RVA"
            .ColumnHeaders.Add , "funaddr", "函数入口RVA"
            .View = lvwReport
        End If
        .ListItems.Clear
        
        For i = LBound(avExports) To UBound(avExports)
            Set itm = .ListItems.Add(, "K" & CStr(avExports(i, 0)), Right$("0000" & Hex(avExports(i, 1)), 4))
            
            itm.SubItems(1) = IIf(IsEmpty(avExports(i, 2)), "-", avExports(i, 2))
            itm.SubItems(2) = Right$("00000000" & Hex(avExports(i, 3)), 8)
            itm.SubItems(3) = Right$("00000000" & Hex(avExports(i, 4)), 8)
            
            If avExports(i, 4) = 0 Then
                itm.SubItems(1) = "无效函数"
            End If
        Next
        
        .Visible = True
        lblExportCount.Caption = "共有" & .ListItems.Count & "个导出函数"
    End With
End Function

Private Function FillImportList(avImports() As Variant) As Boolean
    On Error Resume Next
    Dim i As Long
    Dim itm As ListItem
    
    With lvImportList
        .Visible = False
        
        If .ColumnHeaders.Count = 0 Then
            .ColumnHeaders.Add , "index", "函数序号"
            .ColumnHeaders.Add(, "name", "函数名").Width = 4000
            .ColumnHeaders.Add(, "libname", "库名").Width = 2000
            .ColumnHeaders.Add , "entryaddr", "函数导出表RVA"
            .ColumnHeaders.Add , "funaddr", "函数入口RVA"
            .View = lvwReport
        End If
        .ListItems.Clear
        
        For i = LBound(avImports) To UBound(avImports)
            Set itm = .ListItems.Add(, "K" & CStr(avImports(i, 0)), Right$("0000" & Hex(avImports(i, 0)), 4))
            
            itm.SubItems(1) = avImports(i, 2)
            itm.SubItems(2) = avImports(i, 1)
            itm.SubItems(3) = Right$("00000000" & Hex(avImports(i, 3)), 8)
            itm.SubItems(4) = Right$("00000000" & Hex(avImports(i, 4)), 8)
        Next
        
        .Visible = True
        lblImportCount.Caption = "共有个" & .ListItems.Count & "导入函数"
    End With
End Function

Private Sub cmdRefresh_Click()
    If GetProcEntries(m_atProcEntries) Then
        FillProcList m_atProcEntries()
    End If
End Sub

Private Sub Form_Load()
    Call AquireDebugToken
    Call cmdRefresh_Click
    Set m_oApiHooker = New CVBAPIHooker
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set m_oApiHooker = Nothing
End Sub

Private Sub lvImportList_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single)
    Dim itm As ListItem
    If Button = vbRightButton Then
        Set itm = lvImportList.HitTest(x, y)
        
        If itm Is Nothing Then Exit Sub
        
        mnuHook.Enabled = (itm.Tag = "")
        mnuUnhook.Enabled = Not mnuHook.Enabled
        
        PopupMenu mnuPopout
    End If
End Sub

Private Sub lvModList_ItemClick(ByVal Item As MSComctlLib.ListItem)
    lvExportList.ListItems.Clear
    lvExportList.Sorted = False
    
    lvImportList.ListItems.Clear
    lvImportList.Sorted = False
    
    lblExportCount.Caption = "共有0个导出函数"
    lblImportCount.Caption = "共有0个导入函数"
    
    Dim oPEFnLocator As CVBPEFnLocator
    Dim avExports() As Variant
    Dim avImports() As Variant
    
    Set oPEFnLocator = New CVBPEFnLocator
    If oPEFnLocator.SetTargetTo(CLng("&H" & lvProcList.SelectedItem.Text)) Then
        If oPEFnLocator.GetExports(CLng("&h" & Item.SubItems(2)), avExports()) Then
            FillExportList avExports()
        End If
        
        If oPEFnLocator.GetImports(CLng("&h" & Item.SubItems(2)), avImports()) Then
            FillImportList avImports()
        End If
    End If
    Set oPEFnLocator = Nothing
    
    m_oApiHooker.SetTargetTo
End Sub

Private Sub lvProcList_ColumnClick(ByVal ColumnHeader As MSComctlLib.ColumnHeader)
    With lvProcList
        .SortOrder = IIf(.SortOrder, 0, 1)
        .SortKey = ColumnHeader.SubItemIndex
        .Sorted = True
    End With
End Sub

Private Sub lvModList_ColumnClick(ByVal ColumnHeader As MSComctlLib.ColumnHeader)
    With lvModList
        .SortOrder = IIf(.SortOrder, 0, 1)
        .SortKey = ColumnHeader.SubItemIndex
        .Sorted = True
    End With
End Sub

Private Sub lvExportList_ColumnClick(ByVal ColumnHeader As MSComctlLib.ColumnHeader)
    With lvExportList
        .SortOrder = IIf(.SortOrder, 0, 1)
        .SortKey = ColumnHeader.SubItemIndex
        .Sorted = True
    End With
End Sub

Private Sub lvImportList_ColumnClick(ByVal ColumnHeader As MSComctlLib.ColumnHeader)
    With lvImportList
        .SortOrder = IIf(.SortOrder, 0, 1)
        .SortKey = ColumnHeader.SubItemIndex
        .Sorted = True
    End With
End Sub

Private Sub lvProcList_ItemClick(ByVal Item As MSComctlLib.ListItem)
    lvModList.ListItems.Clear
    lvModList.Sorted = False
    lblModCount.Caption = "共有0个模块"
    
    If GetModEntries(CLng("&H" & Item.Text), m_atModEntries) Then
        FillModList m_atModEntries()
    End If
End Sub

Private Function AquireDebugToken() As Boolean
    Dim hToken As Long
    Dim tTP As TOKEN_PRIVILEGES
    Dim tTPPrev As TOKEN_PRIVILEGES
    Dim tLUID As LUID
    
    If OpenProcessToken(GetCurrentProcess, TOKEN_ADJUST_PRIVILEGES Or TOKEN_QUERY, hToken) Then
        If LookupPrivilegeValue("", "SeDebugPrivilege", tLUID) <> 0 Then
            With tTP
                .PrivilegeCount = 1
                .TheLuid = tLUID
                .Attributes = SE_PRIVILEGE_ENABLED
            End With
            
            If AdjustTokenPrivileges(hToken, 0, tTP, Len(tTP), tTPPrev, Len(tTPPrev)) <> 0 Then
                AquireDebugToken = True
            End If
        End If
        CloseHandle hToken
    End If
End Function

Private Sub m_oApiHooker_BeforeAction(ByVal sAPIName As String, ByVal lpArgs As Long, fIntercept As Boolean, lArgCount As Long, ByVal lReturnAddress As Long)
    AddMsg "即将调用函数 " & sAPIName & " ,参数堆栈地址为：&H" & Hex(lpArgs)
    
    '显示堆栈内容
    Dim abDumpData() As Byte
    Dim alArgs() As Long
    
    m_oApiHooker.DumpStack abDumpData(), 128
    m_oApiHooker.GetArguments alArgs(), 16
    
    AddTxt "单字节方式：" & vbCrLf
    AddTxt Array2Str(abDumpData(), 1, 8) & vbCrLf & vbCrLf
    AddTxt "四字节方式：" & vbCrLf
    AddTxt Array2Str(alArgs(), 4, 4) & vbCrLf & vbCrLf
    AddTxt "函数返回地址：" & vbCrLf
    AddTxt Right$("00000000" & Hex(lReturnAddress), 8) & vbCrLf & vbCrLf
    
    fIntercept = False
End Sub

Private Sub m_oAPIHooker_AfterAction(ByVal sAPIName As String, ByVal fIntercepted As Boolean, lRetValue As Long)
    AddMsg "函数 " & sAPIName & IIf(fIntercepted, "被阻止", " 调用完毕,返回参数为：&H" & Hex(lRetValue))
End Sub

Private Sub mnuHook_Click()
    On Error GoTo Error_Handler
    Static pid As Long
    Dim targetpid As Long
    Dim hMod As Long
    Dim sLibname As String
    Dim vFnName As String
    
    targetpid = CLng("&H" & lvProcList.SelectedItem.Text)
    
    If targetpid <> pid Then
        pid = targetpid
        If Not m_oApiHooker.SetTargetTo(pid) Then
            Exit Sub
        End If
    End If
    
    hMod = CLng("&H" & lvModList.SelectedItem.SubItems(2))
    
    With lvImportList.SelectedItem
        sLibname = .SubItems(2)
        vFnName = .SubItems(1)
        If IsNumeric(vFnName) Then
            vFnName = CLng(vFnName)
        Else
            vFnName = CStr(vFnName)
        End If
        .Tag = m_oApiHooker.Hook(hMod, sLibname, vFnName)
        If .Tag <> -1 Then
            .SmallIcon = 2
            With .ListSubItems(1)
                .ForeColor = HOOKED_COLOR
                .Bold = True
            End With
            MsgBox "API钩子安装成功！"
        Else
            .Tag = ""
            MsgBox "API钩子安装失败！"
        End If
    End With
    lvImportList.Refresh
    
    Exit Sub
Error_Handler:
End Sub


Private Sub AddMsg(ByVal sMsg As String)
    With List1
        .AddItem sMsg
        .Selected(.NewIndex) = True
    End With
End Sub

Private Sub AddTxt(ByVal sTxt As String)
    With Text1
        .Text = .Text & sTxt
        .SelStart = Len(.Text)
    End With
End Sub

Private Function Array2Str(v As Variant, ByVal lDataLen As Long, Optional lNewLinePos As Long = 4) As String
    If IsArray(v) Then
        Dim i As Long
        Array2Str = ""
        For i = LBound(v) To UBound(v)
            Array2Str = Array2Str & " " & IIf((i Mod lNewLinePos = 0) And i <> 0, vbCrLf, "") & Right$(String(lDataLen * 2, "0") & Hex(v(i)), lDataLen * 2)
        Next
        Array2Str = Trim(Array2Str)
    End If
End Function

Private Sub mnuUnhook_Click()
    With lvImportList.SelectedItem
        .SmallIcon = 0
        With .ListSubItems(1)
            .ForeColor = 0
            .Bold = False
        End With
        If .Tag > -1 Then
            If m_oApiHooker.Unhook(.Tag) Then
                .Tag = ""
                MsgBox "钩子卸载成功！"
            Else
                MsgBox "钩子卸载失败！"
            End If
        End If
    End With
    lvImportList.Refresh
End Sub
