Attribute VB_Name = "mduResLib"
'/**************************************************************************
'                 版权所有 (c), 2004 - 2xxx, 绿豆工作室
'
' ************************   模 块 名   :mduResLaLaLa********************
'版 本 号: V1.0
'作    者: 陈缘
'生成日期: 2004年03月07日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月07日
'修改人员: 陈缘
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "mduResLaLaLa"
Public Declare Function lstrlen Lib "kernel32" Alias "lstrlenA" (ByVal lpString As Long) As Long
Public Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyA" (ByVal lpString1 As Long, ByVal lpString2 As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
Public Declare Function lstrlenW Lib "kernel32" (ByVal lpString As Long) As Long

Public Declare Function BeginUpdateResource Lib "kernel32" Alias "BeginUpdateResourceA" (ByVal pFileName As String, ByVal bDeleteExistingResources As Long) As Long
Public Declare Function EndUpdateResource Lib "kernel32" Alias "EndUpdateResourceA" (ByVal hUpdate As Long, ByVal fDiscard As Long) As Long
Public Declare Function FindResource Lib "kernel32" Alias "FindResourceA" (ByVal hInstance As Long, ByVal lpName As String, ByVal lpType As String) As Long
Public Declare Function FindResourceEx Lib "kernel32" Alias "FindResourceExA" (ByVal hModule As Long, ByVal lpType As Long, ByVal lpName As Long, ByVal wLanguage As Integer) As Long
Public Declare Function LockResource Lib "kernel32" (ByVal hResData As Long) As Long
Public Declare Function LoadResource Lib "kernel32" (ByVal hInstance As Long, ByVal hResInfo As Long) As Long
Public Declare Function UpdateResource Lib "kernel32" Alias "UpdateResourceA" (ByVal hUpdate As Long, ByVal lpType As Long, ByVal lpName As Long, ByVal wLanguage As Integer, ByVal lpData As Long, ByVal cbData As Long) As Long
Public Declare Function SizeofResource Lib "kernel32" (ByVal hInstance As Long, ByVal hResInfo As Long) As Long
Public Declare Function EnumResourceLanguages Lib "kernel32" Alias "EnumResourceLanguagesA" (ByVal hModule As Long, ByVal lpType As Long, ByVal lpName As Long, ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Public Declare Function EnumResourceNames Lib "kernel32" Alias "EnumResourceNamesA" (ByVal hModule As Long, ByVal lpType As Long, ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Public Declare Function EnumResourceTypes Lib "kernel32" Alias "EnumResourceTypesA" (ByVal hModule As Long, ByVal lpEnumFunc As Long, ByVal lParam As Long) As Long
Public Declare Function LoadIcon Lib "user32" Alias "LoadIconA" (ByVal hInstance As Long, ByVal lpIconName As String) As Long
Public Declare Function LoadMenu Lib "user32" Alias "LoadMenuA" (ByVal hInstance As Long, ByVal lpString As String) As Long
Public Declare Function LoadCursor Lib "user32" Alias "LoadCursorA" (ByVal hInstance As Long, ByVal lpCursorName As String) As Long
Public Declare Function LoadBitmap Lib "user32" Alias "LoadBitmapA" (ByVal hInstance As Long, ByVal lpBitmapName As Long) As Long
Public Declare Function LoadAccelerators Lib "user32" Alias "LoadAcceleratorsA" (ByVal hInstance As Long, ByVal lpTableName As String) As Long
Public Declare Function LoadString Lib "user32" Alias "LoadStringA" (ByVal hInstance As Long, ByVal wID As Long, ByVal lpBuffer As String, ByVal nBufferMax As Long) As Long
Public Declare Function LoadStringLng Lib "user32" Alias "LoadStringA" (ByVal hInstance As Long, ByVal wID As Long, ByVal lpBuffer As Long, ByVal nBufferMax As Long) As Long
Public Declare Function FreeResource Lib "kernel32" (ByVal hResData As Long) As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Public Declare Function DestroyCursor Lib "user32" (ByVal hCursor As Long) As Long
Public Declare Function DestroyIcon Lib "user32" (ByVal hIcon As Long) As Long
Public Declare Function DestroyMenu Lib "user32" (ByVal hMenu As Long) As Long
Public Declare Function DestroyAcceleratorTable Lib "user32" (ByVal haccel As Long) As Long
Public Declare Function LoadImage Lib "user32" Alias "LoadImageA" (ByVal hInst As Long, ByVal lpsz As Long, ByVal un1 As Long, ByVal n1 As Long, ByVal n2 As Long, ByVal un2 As Long) As Long
Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Public Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long

Public Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long

Public Declare Function CreateDialogIndirectParam Lib "user32" Alias "CreateDialogIndirectParamA" (ByVal hInstance As Long, ByVal lpTemplate As Long, ByVal hWndParent As Long, ByVal lpDialogFunc As Long, ByVal dwInitParam As Long) As Long
Public Declare Function EndDialog Lib "user32" (ByVal hDlg As Long, ByVal nResult As Long) As Long
Public Declare Function ShowWindow Lib "user32" (ByVal hwnd As Long, ByVal nCmdShow As Long) As Long

Public Const DS_ABSALIGN           As Long = &H1
Public Const DS_SYSMODAL           As Long = &H2
Public Const DS_LOCALEDIT          As Long = &H20  '/* Edit items get Local storage. */
Public Const DS_SETFONT            As Long = &H40  '/* User specified font for Dlg controls */
Public Const DS_MODALFRAME         As Long = &H80  '/* Can be combined with WS_CAPTION  */
Public Const DS_NOIDLEMSG          As Long = &H100 '/* WM_ENTERIDLE message will not be sent */
Public Const DS_SETFOREGROUND      As Long = &H200 '/* not in win3.1 */

Public Const DS_3DLOOK             As Long = &H4
Public Const DS_FIXEDSYS           As Long = &H8
Public Const DS_NOFAILCREATE       As Long = &H10
Public Const DS_CONTROL            As Long = &H400
Public Const DS_CENTER             As Long = &H800
Public Const DS_CENTERMOUSE        As Long = &H1000
Public Const DS_CONTEXTHELP        As Long = &H2000


'typedef struct {
'  WORD      dlgVer;0
'  WORD      signature;2
'  DWORD     helpID;4
'  DWORD     exStyle;8
'  DWORD     style;12
'  WORD      cDlgItems;16
'  short     x;18
'  short     y;20
'  short     cx;22
'  short     cy;24
'  sz_Or_Ord menu;26
'  sz_Or_Ord windowClass;
'  WCHAR     title[titleLen];
'// The following members exist only if the style member is
'// set to DS_SETFONT or DS_SHELLFONT.
'  WORD     pointsize;
'  WORD     weight;
'  BYTE     italic;
'  BYTE     charset;
'  WCHAR    typeface[stringLen];
'} DLGTEMPLATEEX;

Public Type DLGTEMPLATEEX
    wDlgVer As Integer
    wSignature As Integer
    dwHelpID As Long
    dwExStyle As Long
    dwStyle As Long
    cDlgItems As Integer
    x As Integer
    y As Integer
    cx As Integer
    cy As Integer
End Type

Public Type DLGTEMPLATEEX_EX
    wDlgVer As Integer
    wSignature As Integer
    dwHelpID As Long
    dwExStyle As Long
    dwStyle As Long
    cDlgItems As Integer
    x As Integer
    y As Integer
    cx As Integer
    cy As Integer
    menu() As Byte
    windowClass() As Byte
    title() As Byte
    ponitsize As Integer
    weight As Integer
    italic As Byte
    charset As Byte
    typeface() As Byte
End Type

'typedef struct {
'  DWORD  helpID;0
'  DWORD  exStyle;4
'  DWORD  style;8
'  short  x;12
'  short  y;14
'  short  cx;16
'  short  cy;18
'  DWORD   id;20
'  sz_Or_Ord windowClass;22
'  sz_Or_Ord title;
'  WORD   extraCount;
'} DLGITEMTEMPLATEEX;

Public Type DLGITEMTEMPLATEEX
    dwHelpID As Long
    dwExStyle As Long
    dwStyle As Long
    x As Integer
    y As Integer
    cx As Integer
    cy As Integer
    id As Long
End Type

Public Type DLGITEMTEMPLATEEX_EX
    dwHelpID As Long
    dwExStyle As Long
    dwStyle As Long
    x As Integer
    y As Integer
    cx As Integer
    cy As Integer
    id As Integer
    windowClass() As Byte
    title() As Byte
    extraCount As Integer
End Type

Public Type DLGITEMTEMPLATE
        style As Long
        dwExtendedStyle As Long
        x As Integer
        y As Integer
        cx As Integer
        cy As Integer
        id As Integer
End Type

Public Type DLGTEMPLATE
        style As Long
        dwExtendedStyle As Long
        cdit As Integer
        x As Integer
        y As Integer
        cx As Integer
        cy As Integer
End Type

Public Const WM_INITDIALOG = &H110
Public Const WM_COMMAND = &H111

Public Declare Function CreateIconFromResource Lib "user32" (ByVal presbits As Long, ByVal dwResSize As Long, ByVal fIcon As Long, ByVal dwVer As Long) As Long
Public Declare Function LookupIconIdFromDirectory Lib "user32" (ByVal presbits As Long, ByVal fIcon As Long) As Long
Public Declare Function LookupIconIdFromDirectoryEx Lib "user32" (ByVal presbits As Long, ByVal fIcon As Long, ByVal cxDesired As Long, ByVal cyDesired As Long, ByVal Flags As Long) As Long
Public Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Public Declare Function DrawIconEx Lib "user32" (ByVal hdc As Long, ByVal xLeft As Long, ByVal yTop As Long, ByVal hIcon As Long, ByVal cxWidth As Long, ByVal cyWidth As Long, ByVal istepIfAniCur As Long, ByVal hbrFlickerFreeDraw As Long, ByVal diFlags As Long) As Long
Public Declare Function DrawIcon Lib "user32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal hIcon As Long) As Long
Public Declare Function GetBitmapDimensionEx Lib "gdi32" (ByVal hBitmap As Long, lpDimension As SIZEL) As Long
Public Declare Function StretchBlt Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal nSrcWidth As Long, ByVal nSrcHeight As Long, ByVal dwRop As Long) As Long
Public Declare Function SetBitmapDimensionEx Lib "gdi32" (ByVal hbm As Long, ByVal nX As Long, ByVal nY As Long, lpSize As SIZEL) As Long
Public Declare Function GetObject Lib "gdi32" Alias "GetObjectA" (ByVal hObject As Long, ByVal nCount As Long, lpObject As Any) As Long

Public Declare Function GetLocaleInfo Lib "kernel32" Alias "GetLocaleInfoA" (ByVal Locale As Long, ByVal LCType As Long, ByVal lpLCData As String, ByVal cchData As Long) As Long

Public Declare Function VerQueryValue Lib "version.dll" Alias "VerQueryValueA" (ByVal pBlock As Long, ByVal lpSubBlock As Long, ByVal lplpBuffer As Long, puLen As Long) As Long
Public Type VS_FIXEDFILEINFO
        dwSignature As Long
        dwStrucVersion As Long         '  e.g. 0x00000042 = "0.42"
        dwFileVersionMS As Long        '  e.g. 0x00030075 = "3.75"
        dwFileVersionLS As Long        '  e.g. 0x00000031 = "0.31"
        dwProductVersionMS As Long     '  e.g. 0x00030010 = "3.10"
        dwProductVersionLS As Long     '  e.g. 0x00000031 = "0.31"
        dwFileFlagsMask As Long        '  = 0x3F for version "0.42"
        dwFileFlags As Long            '  e.g. VFF_DEBUG Or VFF_PRERELEASE
        dwFileOS As Long               '  e.g. VOS_DOS_WINDOWS16
        dwFileType As Long             '  e.g. VFT_DRIVER
        dwFileSubtype As Long          '  e.g. VFT2_DRV_KEYBOARD
        dwFileDateMS As Long           '  e.g. 0
        dwFileDateLS As Long           '  e.g. 0
End Type

Public Type VarStruct
    wLength As Integer
    wValueLength As Integer
    wType As Integer
    szKey As Long
    Padding As Long
    Value As Long
End Type

Public Type SIZEL
        cx As Long
        cy As Long
End Type

Public Type BITMAP '14 bytes
        bmType As Long
        bmWidth As Long
        bmHeight As Long
        bmWidthBytes As Long
        bmPlanes As Integer
        bmBitsPixel As Integer
        bmBits As Long
End Type

Public Const LOCALE_IDEFAULTLANGUAGE = &H9
Public Const LOCALE_IDEFAULTCOUNTRY = &HA
Public Const LOCALE_ILANGUAGE = &H1
Public Const LOCALE_SCOUNTRY = &H6
Public Const LOCALE_SENGCOUNTRY = &H1002
Public Const LOCALE_SENGLANGUAGE = &H1001
Public Const LOCALE_SLANGUAGE = &H2
Public Const LOCALE_SNATIVECTRYNAME = &H8
Public Const LOCALE_SNATIVELANGNAME = &H4

Public Const IMAGE_BITMAP = 0&
Public Const IMAGE_ICON = 1&
Public Const IMAGE_CURSOR = 2&
Public Const IMAGE_ENHMETAFILE = 3&

Public Const DI_MASK = &H1
Public Const DI_IMAGE = &H2
Public Const DI_NORMAL = &H3
Public Const DI_COMPAT = &H4
Public Const DI_DEFAULTSIZE = &H8

Public Const SRCAND = &H8800C6
Public Const SRCCOPY = &HCC0020
Public Const SRCERASE = &H440328
Public Const SRCINVERT = &H660046
Public Const SRCPAINT = &HEE0086

Public Const LR_DEFAULTCOLOR = &H0
Public Const LR_MONOCHROME = &H1
Public Const LR_COLOR = &H2
Public Const LR_COPYRETURNORG = &H4
Public Const LR_COPYDELETEORG = &H8
Public Const LR_LOADFROMFILE = &H10
Public Const LR_LOADTRANSPARENT = &H20
Public Const LR_DEFAULTSIZE = &H40
Public Const LR_VGACOLOR = &H80
Public Const LR_LOADMAP3DCOLORS = &H1000
Public Const LR_CREATEDIBSECTION = &H2000
Public Const LR_COPYFROMRESOURCE = &H4000
Public Const LR_SHARED = &H8000

Public Const RT_CURSOR = 1&
Public Const RT_BITMAP = 2&
Public Const RT_ICON = 3&
Public Const RT_MENU = 4&
Public Const RT_DIALOG = 5&
Public Const RT_STRING = 6&
Public Const RT_FONTDIR = 7&
Public Const RT_FONT = 8&
Public Const RT_ACCELERATOR = 9&
Public Const RT_RCDATA = 10&
Public Const RT_MESSAGETABLE = 11&
Public Const DIFFERENCE = 11&
Public Const RT_GROUP_CURSOR = RT_CURSOR + DIFFERENCE
Public Const RT_GROUP_ICON = RT_ICON + DIFFERENCE
Public Const RT_VERSION = 16&
Public Const RT_DLGINCLUDE = 17&
Public Const RT_PLUGPLAY = 19&
Public Const RT_VXD = 20&
Public Const RT_ANICURSOR = 21&
Public Const RT_ANIICON = 22&
Public Const RT_HTML = 23&
 
Private Declare Function RegisterClass Lib "user32" Alias "RegisterClassA" (Class As WNDCLASS) As Long
Private Declare Function UnregisterClass Lib "user32" Alias "UnregisterClassA" (ByVal lpClassName As String, ByVal hInstance As Long) As Long
Private Declare Function GetClassInfo Lib "user32" Alias "GetClassInfoA" (ByVal hInstance As Long, ByVal lpClassName As String, lpWndClass As WNDCLASS) As Long
Private Type WNDCLASS
    style As Long
    lpfnwndproc As Long
    cbClsextra As Long
    cbWndExtra2 As Long
    hInstance As Long
    hIcon As Long
    hCursor As Long
    hbrBackground As Long
    lpszMenuName As String
    lpszClassName As String
End Type

Dim m_oResPackage As CResPackage

Public Function EnumTypesFunc(ByVal hModule As Long, ByVal lpType As Long, ByVal lParam As Long) As Boolean
    
    Call EnumResourceNames(hModule, lpType, AddressOf EnumNamesFunc, 0&)
    EnumTypesFunc = True
End Function
 
Public Function EnumNamesFunc(ByVal hModule As Long, ByVal lpType As Long, ByVal lpName As Long, ByVal lParam As Long) As Boolean
    Call EnumResourceLanguages(hModule, lpType, lpName, AddressOf EnumLangsFunc, 0&)
    
    EnumNamesFunc = True
End Function
 
Public Function EnumLangsFunc(ByVal hModule As Long, ByVal lpType As Long, ByVal lpName As Long, ByVal wLang As Integer, ByVal lParam As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lhRes As Long
    Dim oResEntries As CResEntries
    Dim oResEntry As CResEntry
    EnumLangsFunc = False
    
    lhRes = FindResourceEx(hModule, lpType, lpName, wLang)
    
    If lhRes = 0 Then
        RaiseError 99999, "资源查找错误", THIS_MODULE_NAME
    End If
    
    Set oResEntries = m_oResPackage.GetResEntries(lpType)
    
    If oResEntries Is Nothing Then
        RaiseError 99999, "无法获得资源条目集合对象", THIS_MODULE_NAME
    End If
    
    Set oResEntry = oResEntries.Add(lhRes)
    
    If oResEntry Is Nothing Then
        RaiseError 99999, "无法获得资源条目对象", THIS_MODULE_NAME
    End If
    
    With oResEntry
        .LangInt = wLang
        .ResNameLng = lpName
        .ResSize = SizeofResource(hModule, lhRes)
        .ResID = lhRes
    End With
    
    EnumLangsFunc = True
    Exit Function
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

Public Function CreateResListWithObject(oResPackage As CResPackage) As Boolean
    On Error GoTo Error_Handler
    Dim lhMod As Long
    lhMod = LoadLibrary(oResPackage.Filename)
    
    If lhMod = 0 Then
        RaiseError 99999, "文件不存在", THIS_MODULE_NAME
    End If
    
    Set m_oResPackage = oResPackage
    
    Call EnumResourceTypes(lhMod, AddressOf EnumTypesFunc, 0&)
    
    Call FreeLibrary(lhMod)
    
    CreateResListWithObject = True
    Exit Function
Error_Handler:
    '自定义错误处理
    If lhMod <> 0 Then
        Call FreeLibrary(lhMod)
    End If
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

Public Function DialogProc(ByVal hwndDlg As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    On Error GoTo EH
    Dim NotidyCode      As Long
    Dim ItemID          As Long
    Dim Index           As Integer
    Dim Str1            As String
    Dim hCombo          As Long
    Dim TempStr         As String * 255
            
    If uMsg = WM_COMMAND Then
        NotidyCode = wParam \ 65536
        ItemID = wParam And 65535
        
        EndDialog hwndDlg, 0
        DialogProc = 1
        Exit Function
    End If
    
    If uMsg = WM_INITDIALOG Then
        DialogProc = 1
        Exit Function
    End If
    DialogProc = 0
    Exit Function
EH:
    Debug.Print Err.Description
    Resume Next
End Function
Public Function AnylyzDialogRes(ByVal hDialogRes As Long, asUndefinedClasses() As String) As Boolean
    On Error GoTo EH
    If hDialogRes = 0 Then Exit Function
    Dim lLen As Long
    Dim i As Long
    Dim udtTmp As DLGTEMPLATEEX
    CopyMemory ByVal VarPtr(udtTmp), ByVal hDialogRes, Len(udtTmp)
    If udtTmp.wDlgVer = 1 And udtTmp.wSignature = &HFFFF Then
        'ex style
        Dim udtTemplate As DLGTEMPLATEEX
        Dim udtTemplateEx As DLGTEMPLATEEX_EX
        Dim udtItem As DLGITEMTEMPLATEEX
        Dim udtItemEx() As DLGITEMTEMPLATEEX_EX
        CopyMemory ByVal VarPtr(udtTemplateEx), ByVal hDialogRes, Len(udtTemplate)
        
        With udtTemplateEx
            Dim Sytels() As String
            Call GetStyleList(.dwStyle, Sytels())
            
            Dim lpAddress As Long
            'udtTemplateEx.menu
            lpAddress = hDialogRes + Len(udtTemplate)
            Select Case ValueOf(lpAddress)
            Case &H0
                ReDim .menu(1)
                lpAddress = lpAddress + 2
            Case &HFFFF
                ReDim .menu(3)
                CopyMemory ByVal VarPtr(.menu(0)), ByVal lpAddress, 4
                lpAddress = lpAddress + 4
            Case Else
                lLen = lstrlenW(lpAddress) * 2
                ReDim .menu(lLen - 1)
                CopyMemory ByVal VarPtr(.menu(0)), ByVal lpAddress, lLen
                lpAddress = lpAddress + lLen + 2
            End Select
            
            'udtTemplateEx.windowClass
            Select Case ValueOf(lpAddress)
            Case &H0
                ReDim .windowClass(1)
                lpAddress = lpAddress + 2
            Case &HFFFF
                ReDim .windowClass(3)
                CopyMemory ByVal VarPtr(.windowClass(0)), ByVal lpAddress, 4
                lpAddress = lpAddress + 4
            Case Else
                lLen = lstrlenW(lpAddress) * 2
                ReDim .windowClass(lLen - 1)
                CopyMemory ByVal VarPtr(.windowClass(0)), ByVal lpAddress, lLen
                lpAddress = lpAddress + lLen + 2
            End Select
                    
            'udtTemplateEx.title
            Select Case ValueOf(lpAddress)
            Case &H0
                ReDim .title(1)
                lpAddress = lpAddress + 2
            Case Else
                lLen = lstrlenW(lpAddress) * 2
                ReDim .title(lLen - 1)
                CopyMemory ByVal VarPtr(.title(0)), ByVal lpAddress, lLen
                lpAddress = lpAddress + lLen + 2
            End Select
                                
            If (.dwStyle And DS_SETFONT) <> 0 Then
                CopyMemory ByVal VarPtr(.ponitsize), ByVal lpAddress, 2
                lpAddress = lpAddress + 2
                CopyMemory ByVal VarPtr(.weight), ByVal lpAddress, 2
                lpAddress = lpAddress + 2
                CopyMemory ByVal VarPtr(.italic), ByVal lpAddress, 1
                lpAddress = lpAddress + 1
                CopyMemory ByVal VarPtr(.charset), ByVal lpAddress, 1
                lpAddress = lpAddress + 1
                
                'udtTemplateEx.typeface
                lLen = lstrlenW(lpAddress) * 2
                ReDim .typeface(lLen - 1)
                CopyMemory ByVal VarPtr(.typeface(0)), ByVal lpAddress, lLen
                lpAddress = lpAddress + lLen + 2
            End If
            ReDim udtItemEx(.cDlgItems - 1)
        End With
        
        Dim lUndefinedClassCount As Long
        lUndefinedClassCount = -1
        For i = 0 To udtTemplateEx.cDlgItems - 1
            CopyMemory ByVal VarPtr(udtItemEx(0)), ByVal lpAddress, Len(udtItem)
            lpAddress = lpAddress + Len(udtItem)
            
            With udtItemEx(i)
                'udtItemEx.windowClass
                Select Case ValueOf(lpAddress)
                Case &HFFFF
                    ReDim .windowClass(3)
                    CopyMemory ByVal VarPtr(.windowClass(0)), ByVal lpAddress, 4
                    lpAddress = lpAddress + 4
                Case Else
                    lLen = lstrlenW(lpAddress) * 2
                    If lLen = 0 Then
                        ReDim .windowClass(1)
                    Else
                        ReDim .windowClass(lLen - 1)
                        CopyMemory ByVal VarPtr(.windowClass(0)), ByVal lpAddress, lLen
                    End If
                    lpAddress = lpAddress + lLen + 2
                    
                    lUndefinedClassCount = lUndefinedClassCount + 1
                    ReDim Preserve asUndefinedClasses(lUndefinedClassCount)
                    asUndefinedClasses(lUndefinedClassCount) = .windowClass
                End Select
                
                'udtItemEx.title
                Select Case ValueOf(lpAddress)
                Case &HFFFF
                    ReDim .title(3)
                    CopyMemory ByVal VarPtr(.windowClass(0)), ByVal lpAddress, 4
                    lpAddress = lpAddress + 4
                Case Else
                    lLen = lstrlenW(lpAddress) * 2
                    If lLen = 0 Then
                        ReDim .title(1)
                    Else
                        ReDim .title(lLen - 1)
                        CopyMemory ByVal VarPtr(.title(0)), ByVal lpAddress, lLen
                    End If
                    lpAddress = lpAddress + lLen + 2
                End Select
                
                CopyMemory ByVal VarPtr(.extraCount), ByVal lpAddress, 2
                lpAddress = AlignAddress(lpAddress + 2 + .extraCount)
                
            End With
        Next
    Else
        'normal style
        Dim udtTemplateN As DLGTEMPLATE
        Dim udtItemN() As DLGITEMTEMPLATE
        CopyMemory ByVal VarPtr(udtTemplateN), ByVal hDialogRes, Len(udtTemplateN)
        With udtTemplateN
            ReDim udtItemN(.cdit - 1)
            CopyMemory ByVal VarPtr(udtItemN(0)), hDialogRes + Len(udtTemplateN), Len(udtItemN(0)) * .cdit
        End With
    End If
    Exit Function
EH:
    Debug.Print Err.Description
    Resume Next
End Function
Public Function AlignAddress(ByVal lpAddress As Long) As Long
    AlignAddress = ((lpAddress + 3) \ 4) * 4
End Function
Public Function GetStyleList(ByVal lStyle As Long, sList() As String) As Boolean
    Dim i As Long
    
    If (lStyle And DS_3DLOOK) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_3DLOOK"
        i = i + 1
    End If

    If (lStyle And DS_ABSALIGN) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_ABSALIGN"
        i = i + 1
    End If

    If (lStyle And DS_CENTER) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_CENTER"
        i = i + 1
    End If

    If (lStyle And DS_CENTERMOUSE) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_CENTERMOUSE"
        i = i + 1
    End If

    If (lStyle And DS_CONTEXTHELP) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_CONTEXTHELP"
        i = i + 1
    End If

    If (lStyle And DS_CONTROL) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_CONTROL"
        i = i + 1
    End If

    If (lStyle And DS_FIXEDSYS) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_FIXEDSYS"
        i = i + 1
    End If

    If (lStyle And DS_LOCALEDIT) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_LOCALEDIT"
        i = i + 1
    End If

    If (lStyle And DS_MODALFRAME) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_MODALFRAME"
        i = i + 1
    End If

    If (lStyle And DS_NOFAILCREATE) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_NOFAILCREATE"
        i = i + 1
    End If

    If (lStyle And DS_NOIDLEMSG) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_NOIDLEMSG"
        i = i + 1
    End If

    If (lStyle And DS_SETFONT) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_SETFONT"
        i = i + 1
    End If

    If (lStyle And DS_SETFOREGROUND) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_SETFOREGROUND"
        i = i + 1
    End If

    If (lStyle And DS_SYSMODAL) <> 0 Then
        ReDim Preserve sList(i)
        sList(i) = "DS_SYSMODAL"
        i = i + 1
    End If

    GetStyleList = True
End Function

Public Function ValueOf(ByVal lpAddress As Long) As Integer
    CopyMemory ValueOf, ByVal lpAddress, 2
End Function

Public Function ShowVal(s As String) As Boolean
    Dim i As Long
    Dim sVal As String
    Dim sT As String
    sT = ""
    Debug.Print s
    For i = 1 To Len(s)
        sT = sT & Mid(s, i, 1) & "(" & Hex(Asc(Mid(s, i, 1))) & ")"
    Next
    Debug.Print sT
End Function

Public Function ShowMeTheByte(hRes As Long, lSize As Long) As Boolean
    Dim b() As Byte
    ReDim b(lSize - 1)
    CopyMemory ByVal VarPtr(b(0)), ByVal hRes, lSize
    Dim i As Long
    Debug.Print lSize, UBound(b) - LBound(b) + 1
    For i = 0 To UBound(b)
        Debug.Print Right("00" & Hex(b(i)), 2); IIf((i + 1) Mod 2 = 0, " ", "");
    Next
    Debug.Print
End Function

Public Function RegClasses(asClasses() As String) As Boolean
    On Error GoTo Error_Handler
    Dim udt As WNDCLASS
    Dim i As Long
    If GetClassInfo(App.hInstance, "Static", udt) Then
        For i = LBound(asClasses) To UBound(asClasses)
            udt.lpszClassName = asClasses(i)
            RegisterClass udt
        Next
    End If
    RegClasses = True
    Exit Function
Error_Handler:
    Call UnRegClasses(asClasses())
End Function

Public Sub UnRegClasses(asClasses() As String)
    On Error Resume Next
    Dim i As Long
    For i = LBound(asClasses) To UBound(asClasses)
        UnregisterClass asClasses(i), App.hInstance
    Next
End Sub
