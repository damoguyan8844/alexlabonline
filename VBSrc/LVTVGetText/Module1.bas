Attribute VB_Name = "Module1"
Option Explicit
Public Declare Function WindowFromPoint Lib "user32" (ByVal xPoint As Long, ByVal yPoint As Long) As Long
Public Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Public Declare Function GetClassName Lib "user32" Alias "GetClassNameA" (ByVal hwnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long) As Long


'Structures
Type POINTAPI
     x As Long
     y As Long
End Type

Public Type POINT
    x As Long
    y As Long
End Type

Public Type LV_FINDINFO
    flags As Long
    psz As String
    lParam As Long
    pt As POINT
    vkDirection As Long
End Type

Public Type LV_ITEM
    mask As Long
    iItem As Long
    iSubItem As Long
    state As Long
    stateMask As Long
    pszText As Long
    cchTextMax As Long
    iImage As Long
    lParam As Long
    iIndent As Long
End Type

Public Type TVITEM
    mask As Long
    hItem As Long
    state As Long
    stateMask As Long
    pszText As Long
    cchTextMax As Long
    iImage As Long
    iSelectedImage As Long
    cChildren As Long
    lParam As Long
End Type

Public Type OSVERSIONINFO
    dwOSVersionInfoSize As Long
    dwMajorVersion As Long
    dwMinorVersion As Long
    dwBuildNumber As Long
    dwPlatformId As Long
    szCSDVersion As String * 128
End Type

'public constants

Public Const LVFI_PARAM = 1
Public Const LVIF_TEXT = &H1
Public Const LVM_FIRST = &H1000
Public Const LVM_GETITEMCOUNT = LVM_FIRST + 4
Public Const LVM_FINDITEM = LVM_FIRST + 13
Public Const LVM_GETITEMTEXT = LVM_FIRST + 45
Public Const LVM_GETHEADER = LVM_FIRST + 31

Public Const HDM_FIRST = &H1200&
Public Const HDM_GETITEMCOUNT = HDM_FIRST + 0

Public Const TVIF_TEXT = 1
Public Const TVIF_HANDLE = &H10
Public Const TV_FIRST = &H1100&
Public Const TVM_GETCOUNT = TV_FIRST + 5
Public Const TVM_SELECTITEM = TV_FIRST + 11
Public Const TVM_GETITEM = TV_FIRST + 12
Public Const TVM_GETNEXTITEM = TV_FIRST + 10
Public Const TVGN_ROOT = 0
Public Const TVGN_NEXT = 1
Public Const TVGN_CHILD = 4
Public Const TVGN_CARET = 9
'API declarations
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long

Public Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hwnd As Long, lpdwProcessId As Long) As Long
Public Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Public Declare Function VirtualAlloc Lib "kernel32" (lpAddress As Any, ByVal dwSize As Long, ByVal flAllocationType As Long, ByVal flProtect As Long) As Long
Public Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Public Declare Function ReadProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Public Declare Function VirtualFree Lib "kernel32" (lpAddress As Any, ByVal dwSize As Long, ByVal dwFreeType As Long) As Long
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long
Public Declare Function FindWindow& Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String)
Public Declare Function FindWindowEx& Lib "user32" Alias "FindWindowExA" (ByVal hWndParent As Long, ByVal hWndChildAfter As Long, ByVal lpClassName As String, ByVal lpWindowName As String)
Public Declare Function GetVersionEx Lib "kernel32" Alias "GetVersionExA" (LpVersionInformation As OSVERSIONINFO) As Long

'========= Win95/98/ME Shared memory staff===============
Public Declare Function CreateFileMapping Lib "kernel32" Alias "CreateFileMappingA" (ByVal hFile As Long, ByVal lpFileMappigAttributes As Long, ByVal flProtect As Long, ByVal dwMaximumSizeHigh As Long, ByVal dwMaximumSizeLow As Long, ByVal lpName As String) As Long
Public Declare Function MapViewOfFile Lib "kernel32" (ByVal hFileMappingObject As Long, ByVal dwDesiredAccess As Long, ByVal dwFileOffsetHigh As Long, ByVal dwFileOffsetLow As Long, ByVal dwNumberOfBytesToMap As Long) As Long
Public Declare Function UnmapViewOfFile Lib "kernel32" (lpBaseAddress As Any) As Long
Public Const STANDARD_RIGHTS_REQUIRED = &HF0000
Public Const SECTION_QUERY = &H1
Public Const SECTION_MAP_WRITE = &H2
Public Const SECTION_MAP_READ = &H4
Public Const SECTION_MAP_EXECUTE = &H8
Public Const SECTION_EXTEND_SIZE = &H10
Public Const SECTION_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED Or SECTION_QUERY Or SECTION_MAP_WRITE Or SECTION_MAP_READ Or SECTION_MAP_EXECUTE Or SECTION_EXTEND_SIZE
Public Const FILE_MAP_ALL_ACCESS = SECTION_ALL_ACCESS

'============NT Shared memory staff======================
Public Const PROCESS_QUERY_INFORMATION = &H400
Public Const PROCESS_VM_OPERATION = &H8
Public Const PROCESS_VM_READ = &H10
Public Const PROCESS_VM_WRITE = &H20
Public Const PROCESS_ALL_ACCESS = 0
Public Declare Function VirtualAllocEx Lib "kernel32" (ByVal hProcess As Long, ByVal lpAddress As Long, ByVal dwSize As Long, ByVal flAllocationType As Long, ByVal flProtect As Long) As Long
Public Declare Function VirtualFreeEx Lib "kernel32" (ByVal hProcess As Long, lpAddress As Any, ByVal dwSize As Long, ByVal dwFreeType As Long) As Long

Public Const MEM_COMMIT = &H1000
Public Const MEM_RESERVE = &H2000
Public Const MEM_DECOMMIT = &H4000
Public Const MEM_RELEASE = &H8000
Public Const MEM_FREE = &H10000
Public Const MEM_PRIVATE = &H20000
Public Const MEM_MAPPED = &H40000
Public Const MEM_TOP_DOWN = &H100000

'==========Memory access public constants===========
Public Const PAGE_NOACCESS = &H1&
Public Const PAGE_READONLY = &H2&
Public Const PAGE_READWRITE = &H4&
Public Const PAGE_WRITECOPY = &H8&
Public Const PAGE_EXECUTE = &H10&
Public Const PAGE_EXECUTE_READ = &H20&
Public Const PAGE_EXECUTE_READWRITE = &H40&
Public Const PAGE_EXECUTE_WRITECOPY = &H80&
Public Const PAGE_GUARD = &H100&
Public Const PAGE_NOCACHE = &H200&



