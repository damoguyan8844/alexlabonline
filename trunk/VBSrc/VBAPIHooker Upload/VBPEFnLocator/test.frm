VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   3915
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3915
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.ListBox List1 
      Height          =   3300
      Left            =   90
      TabIndex        =   0
      Top             =   390
      Width           =   4485
   End
   Begin VB.Label Label1 
      Caption         =   "信息:"
      Height          =   285
      Left            =   120
      TabIndex        =   1
      Top             =   150
      Width           =   1305
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long
Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Private Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Private Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hwnd As Long, lpdwProcessId As Long) As Long

Private Sub Form_Load()
    Dim oPEFnLocator As CVBPEFnLocator
    Dim lOldAddress As Long
    
    Dim pid As Long
    GetWindowThreadProcessId FindWindow("Shell_TrayWnd", vbNullString), pid
    Set oPEFnLocator = New CVBPEFnLocator
    With oPEFnLocator
        '设定目标进程ID至系统任务栏
        .SetTargetTo pid
        '查找Explorer.exe模块中,从user32.dll模块里所导入的TileWindows函数的地址
        If .LocateImportFunction("Explorer.exe", "user32.dll", "TileWindows") Then
            AddMsg Hex(.ImpEntryAddress) & " " & Hex(.ImpJumpAddress) & " " & .ImpModuleName
        End If
        '查找Explorer.exe模块中,从user32.dll模块里所导出的CreateWindowExA函数的地址
        If .LocateExportFunction("user32.dll", "CreateWindowExA") Then
            AddMsg Hex(.ExpEntryAddress) & " " & Hex(.ExpJumpAddress) & " " & .ImpModuleName
        End If
    End With
    Set oPEFnLocator = Nothing
    
    
    Set oPEFnLocator = New CVBPEFnLocator
    '不指定目标进程ID,使用默认设置,即针对当前进程操作
    With oPEFnLocator
        '查找msvbvm60模块中,从user32.dll模块里所导入的CreateWindowExA函数的地址
        If .LocateImportFunction(GetModuleHandle("msvbvm60"), "user32.dll", "CreateWindowExA") Then
            AddMsg Hex(.ImpEntryAddress) & " " & Hex(.ImpJumpAddress) & " " & .ImpModuleName
            '替换所找到的CreateWindowExA函数的跳转地址
            .ImpReplace &H123456
        End If
    End With
    '默认的，对象被销毁时会自动恢复被替换的地址
    '除非 AutoRestore 属性为 False
    Set oPEFnLocator = Nothing
    
    Set oPEFnLocator = New CVBPEFnLocator
    '不指定目标进程ID,使用默认设置,即针对当前进程操作
    With oPEFnLocator
        '查找msvbvm60模块中,从user32.dll模块里所导入的CreateWindowExA函数的地址
        If .LocateImportFunction(GetModuleHandle("msvbvm60"), "user32.dll", "CreateWindowExA") Then
            AddMsg Hex(.ImpEntryAddress) & " " & Hex(.ImpJumpAddress) & " " & .ImpModuleName
            '替换所找到的CreateWindowExA函数的跳转地址
            .ImpReplace &H123456
            lOldAddress = .ImpJumpAddress
        End If
        If .LocateImportFunction(GetModuleHandle("msvbvm60"), "user32.dll", "CreateWindowExA") Then
            AddMsg Hex(.ImpEntryAddress) & " " & Hex(.ImpJumpAddress) & " " & .ImpModuleName
            '恢复CreateWindowExA函数的跳转地址
            .ImpReplace lOldAddress
        End If
        If .LocateImportFunction(GetModuleHandle("msvbvm60"), "user32.dll", "CreateWindowExA") Then
            AddMsg Hex(.ImpEntryAddress) & " " & Hex(.ImpJumpAddress) & " " & .ImpModuleName
        End If
        
        Dim hLib As Long
        hLib = LoadLibrary("shell32.dll")
        '查找shell32模块中,从user32.dll模块里所导出的序号为&HE3的函数地址
        If .LocateExportFunction(hLib, &HE3) Then
            AddMsg Hex(.ExpEntryAddress) & " " & Hex(.ExpJumpAddress) & " " & .ExpModuleName & " " & Hex(GetProcAddress(hLib, "FreeIconList"))
        End If
        '查找shell32模块中,从user32.dll模块里所导出的名称为FreeIconList的函数地址
        If oPEFnLocator.LocateExportFunction(hLib, "FreeIconList") Then
            AddMsg Hex(.ExpEntryAddress) & " " & Hex(.ExpJumpAddress) & " " & .ExpModuleName & " " & Hex(GetProcAddress(hLib, "FreeIconList"))
        End If
        FreeLibrary hLib
    End With
End Sub

Private Sub AddMsg(ByVal sMsg As String)
    With List1
        .AddItem sMsg
        .Selected(.NewIndex) = True
    End With
End Sub

