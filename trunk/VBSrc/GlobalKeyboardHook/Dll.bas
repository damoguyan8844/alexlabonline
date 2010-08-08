Attribute VB_Name = "DLL"
Option Explicit

'Dll常数
Public Const DLL_PROCESS_ATTACH As Long = 1
Public Const DLL_THREAD_ATTACH As Long = 2
Public Const DLL_PROCESS_DETACH As Long = 0
Public Const DLL_THREAD_DETACH As Long = 3

'dll的实例句柄
Public g_hInstance As Long
'钩子的句柄
Public g_hHook As Long

'用于存放补丁代码的结构
Public Type MAGIC_CODES
    magic_code0 As Currency
    magic_code1 As Currency
    magic_code2 As Currency
    magic_code3 As Currency
    magic_code4 As Currency
    magic_code5 As Currency
    magic_code6 As Currency
    magic_code7 As Currency
    magic_code8 As Currency
    magic_code9 As Currency
    magic_code10 As Currency
    magic_code11 As Currency
    magic_code12 As Currency
    magic_code13 As Currency
    magic_code14 As Currency
    magic_code15 As Currency
    magic_code16 As Currency
    magic_code17 As Currency
    magic_code18 As Currency
    magic_code19 As Currency
    magic_code20 As Currency
    magic_code21 As Currency
    magic_code22 As Currency
    magic_code23 As Currency
    magic_code24 As Currency
    magic_code25 As Currency
    magic_code26 As Currency
    magic_code27 As Currency
    magic_code28 As Currency
    magic_code29 As Currency
    magic_code30 As Currency
    magic_code31 As Currency
    magic_code32 As Currency
    magic_code33 As Currency
    magic_code34 As Currency
    magic_code35 As Currency
    magic_code36 As Currency
    magic_code37 As Currency
    magic_code38 As Currency
    magic_code39 As Currency
    magic_code40 As Currency
    magic_code41 As Currency
    magic_code42 As Currency
End Type

'为了存在而存在的过程
Private Sub DummyFuncionLib()
    Call VirtualProtect(0, 0, 0, 0)
End Sub

'假入口点，同时也是__vbaSetSystemError函数的替代者
Private Sub Main()
End Sub

'最关键的过程
Private Sub DoPatch()
    Dim mc As MAGIC_CODES
    With mc
        '调试用代码
        'int3 nop nop nop nop nop nop nop
        '.magic_code0 = -802975918502651.0644@
        
        '非调试用
        'nop nop nop nop nop nop nop nop
        .magic_code0 = -802975918502651.0704@
        
        .magic_code1 = 622040008903809.7237@
        .magic_code2 = 2168831800.4567@
        .magic_code3 = -605627189390182.1952@
        .magic_code4 = -410728286014535.9417@
        .magic_code5 = 502798750898251.1685@
        .magic_code6 = -372999204216032.4364@
        .magic_code7 = -827798135450524.8205@
        .magic_code8 = 330798298674547.8277@
        .magic_code9 = 793866097004852.0192@
        .magic_code10 = -53886902838295.8744@
        .magic_code11 = 2628655880.6015@
        .magic_code12 = -637595467560638.7968@
        .magic_code13 = 3939799933124.608@
        .magic_code14 = 66535032.5641@
        .magic_code15 = 501110827696265.5371@
        .magic_code16 = 54985288.582@
        .magic_code17 = 702247908688761.7795@
        .magic_code18 = -68312644782790.3605@
        .magic_code19 = 9335951978.2772@
        .magic_code20 = 578748446467995.9296@
        .magic_code21 = -425055349760355.2973@
        .magic_code22 = 356374425853313.3139@
        .magic_code23 = 277126188320481.4583@
        .magic_code24 = 1235852261667.3539@
        .magic_code25 = -900059665159297.4336@
        .magic_code26 = -252114107887497.6061@
        .magic_code27 = -842953393026347.0963@
        .magic_code28 = -149490929198009.3184@
        .magic_code29 = -150736500635517.8552@
        .magic_code30 = -459724283347085.6799@
        .magic_code31 = -453752972934401.3159@
        .magic_code32 = -437047414471871.3398@
        .magic_code33 = -889589831.3589@
        .magic_code34 = 841287142247312.6911@
        .magic_code35 = 305928172851989.9953@
        .magic_code36 = 31815573375221.4667@
        .magic_code37 = 619676175635965.6554@
        .magic_code38 = 90002062006572.6712@
        .magic_code39 = -21944536.5504@
        .magic_code40 = -857137313911176.5879@
        .magic_code41 = 553169067076638.3384@
        .magic_code42 = 8916027.9435@
    End With


    Call CallWindowProc(VarPtr(mc), g_hInstance, AddressOf Main, 0, 0)
End Sub

'Dll入口点函数
Public Function DllMain(ByVal hinstDLL As Long, ByVal fdwReason As Long, ByVal lpvReserved As Long) As Long
    g_hInstance = hinstDLL
    
    Select Case fdwReason
    Case DLL_PROCESS_ATTACH
        '调用补丁过程，使我们能在接下去使用任何API
        Call DoPatch
    Case DLL_PROCESS_DETACH
    Case DLL_THREAD_ATTACH
    Case DLL_THREAD_DETACH
    End Select
    
    DllMain = 1
End Function

'键盘钩子过程
Public Function KeyboardProc(ByVal nCode As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    If nCode >= 0 Then
        SendMsg wParam
        SendMsg lParam
    End If
    KeyboardProc = CallNextHookEx(g_hHook, nCode, wParam, ByVal lParam)
End Function

'安装钩子
Public Function InstallHook(ByVal tid As Long) As Long
    g_hHook = SetWindowsHookEx(WH_KEYBOARD, AddressOf KeyboardProc, g_hInstance, tid)
    InstallHook = g_hHook
End Function

'卸载钩子
Public Function UninstallHook() As Long
    UninstallHook = UnhookWindowsHookEx(g_hHook)
End Function

'发送消息到接受窗口
Public Sub SendMsg(ByVal lData As Long)
    SendMessage FindWindow(vbNullString, "KBHOOKTESTWINDOW"), WM_NULL, ByVal lData, ByVal 0
End Sub
