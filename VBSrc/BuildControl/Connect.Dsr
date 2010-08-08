VERSION 5.00
Begin {AC0714F6-3D04-11D1-AE7D-00A0C90F26F4} Connect 
   ClientHeight    =   9945
   ClientLeft      =   1740
   ClientTop       =   1545
   ClientWidth     =   6585
   _ExtentX        =   11615
   _ExtentY        =   17542
   _Version        =   393216
   Description     =   "拦截VB编译(Compile)、连接(Link)过程，然后……"
   DisplayName     =   "Jonathan's Build Control Add-In"
   AppName         =   "Visual Basic"
   AppVer          =   "Visual Basic 6.0"
   LoadName        =   "Startup"
   LoadBehavior    =   1
   RegLocation     =   "HKEY_CURRENT_USER\Software\Microsoft\Visual Basic\6.0"
   CmdLineSupport  =   -1  'True
End
Attribute VB_Name = "Connect"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit
Private mnuBuildControl As Office.CommandBarPopup
Public WithEvents m_oHandler_BuildControl As CommandBarEvents          'command bar event handler
Attribute m_oHandler_BuildControl.VB_VarHelpID = -1

Private mnuBuildControl_About As Office.CommandBarControl
Public WithEvents m_oHandler_BuildControl_About As CommandBarEvents          'command bar event handler
Attribute m_oHandler_BuildControl_About.VB_VarHelpID = -1

Private mnuBuildControl_Install As Office.CommandBarControl
Public WithEvents m_oHandler_BuildControl_Install As CommandBarEvents          'command bar event handler
Attribute m_oHandler_BuildControl_Install.VB_VarHelpID = -1

Private mnuBuildControl_Uninstall As Office.CommandBarControl
Public WithEvents m_oHandler_BuildControl_Uninstall As CommandBarEvents          'command bar event handler
Attribute m_oHandler_BuildControl_Uninstall.VB_VarHelpID = -1

Private mnuBuildControl_AddTemplate As Office.CommandBarControl
Public WithEvents m_oHandler_BuildControl_AddTemplate As CommandBarEvents          'command bar event handler
Attribute m_oHandler_BuildControl_AddTemplate.VB_VarHelpID = -1

Private mnuBuildControl_Build As Office.CommandBarControl
Public WithEvents m_oHandler_BuildControl_Build As CommandBarEvents          'command bar event handler
Attribute m_oHandler_BuildControl_Build.VB_VarHelpID = -1

Private Const MENU_NAME_BUILD_CONTROL As String = "编译控制(&C)"

'------------------------------------------------------
'this method adds the Add-In to VB
'------------------------------------------------------
Private Sub AddinInstance_OnConnection(ByVal Application As Object, ByVal ConnectMode As AddInDesignerObjects.ext_ConnectMode, ByVal AddInInst As Object, custom() As Variant)
    On Error GoTo Error_Handler
    
    Set VBInstance = Application
    
    '添加菜单
    Call SetupMenus
    
    '安装挂钩
    Call InstallHook
    
    Exit Sub
    
Error_Handler:
    
    MsgBox Err.Description
    
End Sub

'------------------------------------------------------
'this method removes the Add-In from VB
'------------------------------------------------------
Private Sub AddinInstance_OnDisconnection(ByVal RemoveMode As AddInDesignerObjects.ext_DisconnectMode, custom() As Variant)
    On Error Resume Next
    
    '删除菜单
    Call RemoveMenus
    
    '卸载挂钩
    Call UninstallHook
End Sub

Private Sub m_oHandler_BuildControl_About_Click(ByVal CommandBarControl As Object, handled As Boolean, CancelDefault As Boolean)
    frmAbout.Show
End Sub

Private Sub m_oHandler_BuildControl_AddTemplate_Click(ByVal CommandBarControl As Object, handled As Boolean, CancelDefault As Boolean)
    If Not AddTemplateModule Then
        ShowMsg "添加模版失败！"
    End If
End Sub

Private Sub InstallHook(Optional fShow As Boolean = False)
    If g_oBuildHook.InstallHook(AddressOf BuildHookProc_Before, AddressOf BuildHookProc_After) Then
        ShowMsg "挂钩安装成功！", fShow
        mnuBuildControl_Install.Enabled = False
        mnuBuildControl_Uninstall.Enabled = True
        mnuBuildControl_Build.Enabled = True
    Else
        ShowMsg "挂钩安装失败！", fShow
    End If
End Sub

Private Sub UninstallHook(Optional fShow As Boolean = False)
    With g_oBuildHook
        If .IsInstalled Then
            If .UninstallHook Then
                ShowMsg "挂钩卸载成功！", fShow
                mnuBuildControl_Install.Enabled = True
                mnuBuildControl_Uninstall.Enabled = False
                mnuBuildControl_Build.Enabled = False
            Else
                ShowMsg "挂钩卸载失败！", fShow
            End If
        Else
            ShowMsg "挂钩未曾安装过！", fShow
        End If
    End With
End Sub

Private Sub ShowMsg(sMsg, Optional fShow As Boolean = False)
    If fShow Then MsgBox sMsg
    Debug.Print sMsg
End Sub

Private Sub m_oHandler_BuildControl_Build_Click(ByVal CommandBarControl As Object, handled As Boolean, CancelDefault As Boolean)
    On Error Resume Next
    Dim sBuildFileName As String, sTmp As String
    sBuildFileName = GetBuildFileName
    If sBuildFileName <> "" Then
        With VBInstance.ActiveVBProject
            sTmp = .BuildFileName
            
            If IsObject(.VBComponents.StartUpObject) Then
                Set g_vStartUpObject = .VBComponents.StartUpObject
            Else
                g_vStartUpObject = .VBComponents.StartUpObject
            End If
            
            .BuildFileName = sBuildFileName
            g_fBuildDLL = True
            
            Call CompileFile
            
            .BuildFileName = sTmp
        End With
    End If
End Sub

Private Sub m_oHandler_BuildControl_Click(ByVal CommandBarControl As Object, handled As Boolean, CancelDefault As Boolean)
    On Error Resume Next
    g_sBuildFileName = VBInstance.ActiveVBProject.BuildFileName
    g_sBuildFileName = Left(g_sBuildFileName, InStr(g_sBuildFileName, ".") - 1) & ".dll"
    g_sBuildFileName = Mid(g_sBuildFileName, InStrRev(g_sBuildFileName, "\") + 1)
    mnuBuildControl_Build.Caption = "生成 " & g_sBuildFileName
End Sub

Private Sub m_oHandler_BuildControl_Install_Click(ByVal CommandBarControl As Object, handled As Boolean, CancelDefault As Boolean)
    Call InstallHook(True)
End Sub

Private Sub m_oHandler_BuildControl_Uninstall_Click(ByVal CommandBarControl As Object, handled As Boolean, CancelDefault As Boolean)
    Call UninstallHook(True)
End Sub

Private Function SetupMenus() As Boolean
    On Error Resume Next
    Dim oMenuBar As Office.CommandBar
    Dim oMenuItem As Office.CommandBarButton
    Dim oMenuEntry As Office.CommandBarPopup
    Dim om As Variant
    
    Set mnuBuildControl = VBInstance.CommandBars("菜单条").Controls.Add(Type:=msoControlPopup)
    With mnuBuildControl
        .Caption = MENU_NAME_BUILD_CONTROL
        .BeginGroup = True
        Set m_oHandler_BuildControl = VBInstance.Events.CommandBarEvents(mnuBuildControl)
        
        Set mnuBuildControl_Build = .Controls.Add(msoControlButton)
        With mnuBuildControl_Build
            .BeginGroup = True
            .Caption = "生成 " & g_sBuildFileName
            Set m_oHandler_BuildControl_Build = VBInstance.Events.CommandBarEvents(mnuBuildControl_Build)
        End With
        
        Set mnuBuildControl_AddTemplate = .Controls.Add(msoControlButton)
        With mnuBuildControl_AddTemplate
            .BeginGroup = True
            .Caption = "添加VB DLL模版"
            Set m_oHandler_BuildControl_AddTemplate = VBInstance.Events.CommandBarEvents(mnuBuildControl_AddTemplate)
        End With
        
        Set mnuBuildControl_Install = .Controls.Add(msoControlButton)
        With mnuBuildControl_Install
            .BeginGroup = True
            .Caption = "安装"
            Set m_oHandler_BuildControl_Install = VBInstance.Events.CommandBarEvents(mnuBuildControl_Install)
        End With
    
        Set mnuBuildControl_Uninstall = .Controls.Add(msoControlButton)
        With mnuBuildControl_Uninstall
            .Caption = "卸载"
            Set m_oHandler_BuildControl_Uninstall = VBInstance.Events.CommandBarEvents(mnuBuildControl_Uninstall)
        End With
    
        Set mnuBuildControl_About = .Controls.Add(msoControlButton)
        With mnuBuildControl_About
            .BeginGroup = True
            .Caption = "关于..."
            Set m_oHandler_BuildControl_About = VBInstance.Events.CommandBarEvents(mnuBuildControl_About)
        End With
    
    End With
End Function

Private Function RemoveMenus() As Boolean
    On Error Resume Next
    mnuBuildControl.Delete
End Function

Private Function GetBuildFileName() As String
    GetBuildFileName = GetSaveFile(g_sBuildFileName, "*.dll|*.dll", "生成 dll")
    If GetBuildFileName = "" Then Exit Function
    If LCase(Right(GetBuildFileName, 4)) <> ".dll" Then
        GetBuildFileName = GetBuildFileName & ".dll"
    End If
End Function

Private Sub CompileFile()
    On Error Resume Next
    If GetFunctionList.Count Then
        VBInstance.ActiveVBProject.MakeCompiledFile
    Else
        MsgBox "没有找到任何可以导出的公共函数！"
    End If
End Sub
