Attribute VB_Name = "Module1"
Option Explicit

Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function SetClassLong Lib "user32" Alias "SetClassLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Const GWL_WNDPROC = (-4)
Public Const GCL_WNDPROC = (-24)

Public Declare Function GetScrollInfo Lib "user32" (ByVal hwnd As Long, ByVal n As Long, lpScrollInfo As SCROLLINFO) As Long
Public Declare Function ShowScrollBar Lib "user32" (ByVal hwnd As Long, ByVal wBar As Long, ByVal bShow As Long) As Long

Public Declare Function FlatSB_EnableScrollBar Lib "comctl32" (ByVal hwnd As Long, ByVal wSBflags As Long, ByVal wArrows As Long) As Long
Public Declare Function FlatSB_ShowScrollBar Lib "comctl32" (ByVal hwnd As Long, ByVal code As Long, ByVal fShow As Boolean) As Long
Public Declare Function FlatSB_GetScrollRange Lib "comctl32" (ByVal hwnd As Long, ByVal code As Long, ByVal fShow As Boolean) As Long
Public Declare Function FlatSB_SetScrollInfo Lib "comctl32" (ByVal hwnd As Long, ByVal code As Long, lpScrollInfo As SCROLLINFO, ByVal fShow As Boolean) As Long
Public Declare Function FlatSB_GetScrollInfo Lib "comctl32" (ByVal hwnd As Long, ByVal code As Long, lpScrollInfo As SCROLLINFO) As Long
Public Declare Function FlatSB_SetScrollPos Lib "comctl32" (ByVal hwnd As Long, ByVal code As Long, ByVal nPos As Long, ByVal fShow As Boolean) As Long
Public Declare Function FlatSB_SetScrollProp Lib "comctl32" (ByVal hwnd As Long, ByVal index As Long, ByVal newValue As Long, ByVal fShow As Boolean) As Long
Public Declare Function FlatSB_GetScrollProp Lib "comctl32" (ByVal hwnd As Long, ByVal index As Long, ByRef pValue As Long) As Long
Public Declare Function InitializeFlatSB Lib "comctl32" (ByVal hwnd As Long) As Long
Public Declare Function UninitializeFlatSB Lib "comctl32" (ByVal hwnd As Long) As Long

Public Const WSB_PROP_CYVSCROLL = &H1
Public Const WSB_PROP_CXHSCROLL = &H2
Public Const WSB_PROP_CYHSCROLL = &H4
Public Const WSB_PROP_CXVSCROLL = &H8
Public Const WSB_PROP_CXHTHUMB = &H10
Public Const WSB_PROP_CYVTHUMB = &H20
Public Const WSB_PROP_VBKGCOLOR = &H40
Public Const WSB_PROP_HBKGCOLOR = &H80
Public Const WSB_PROP_VSTYLE = &H100
Public Const WSB_PROP_HSTYLE = &H200
Public Const WSB_PROP_WINSTYLE = &H400
Public Const WSB_PROP_PALETTE = &H800
Public Const WSB_PROP_MASK = &HFFF

Public Const FSB_FLAT_MODE = 2
Public Const FSB_ENCARTA_MODE = 1
Public Const FSB_REGULAR_MODE = 0

Public Const SB_BOTH = 3
Public Const SB_VERT = 1
Public Const SB_HORZ = 0

Public Const SB_BOTTOM = 7
Public Const SB_CTL = 2
Public Const SB_ENDSCROLL = 8
Public Const SB_LEFT = 6
Public Const SB_LINEDOWN = 1
Public Const SB_LINELEFT = 0
Public Const SB_LINERIGHT = 1
Public Const SB_LINEUP = 0
Public Const SB_PAGEDOWN = 3
Public Const SB_PAGELEFT = 2
Public Const SB_PAGERIGHT = 3
Public Const SB_PAGEUP = 2
Public Const SB_RIGHT = 7
Public Const SB_THUMBPOSITION = 4
Public Const SB_THUMBTRACK = 5
Public Const SB_TOP = 6

Public Const SIF_RANGE = &H1
Public Const SIF_PAGE = &H2
Public Const SIF_POS = &H4
Public Const SIF_DISABLENOSCROLL = &H8
Public Const SIF_TRACKPOS = &H10
Public Const SIF_ALL = (SIF_RANGE Or SIF_PAGE Or SIF_POS Or SIF_TRACKPOS)

Public Type SCROLLINFO
    cbSize As Long
    fMask As Long
    nMin As Long
    nMax As Long
    nPage As Long
    nPos As Long
    nTrackPos As Long
End Type


Public Const WM_NCPAINT = &H85
Public Const WM_SIZE = &H5
Public Const WM_VSCROLL = &H115
Public Const WM_MOUSEWHEEL = &H20A
Public Const WHEEL_DELTA = 120
Public Const WHEEL_PAGESCROLL = 65535
Public Const WM_NCLBUTTONUP = &HA2
Public Const WM_NCLBUTTONDOWN = &HA1
Public Const WM_KEYDOWN = &H100
Public Const WM_NCCALCSIZE = &H83


Public Type WINDOWPOS
        hwnd As Long
        hWndInsertAfter As Long
        x As Long
        y As Long
        cx As Long
        cy As Long
        flags As Long
End Type

Public Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type

Public Type NCCALCSIZE_PARAMS
    rgrc(2) As RECT
    lppos As WINDOWPOS
End Type

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Public glDefWindowProc As Long


Public Const HDN_FIRST As Long = -300
Public Const HDN_TRACKA As Long = HDN_FIRST - 8
Public Const HDN_BEGINTRACKA As Long = HDN_FIRST - 6

Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long
Public Declare Function EnableWindow Lib "user32" (ByVal hwnd As Long, ByVal fEnable As Long) As Long
Public Const LVM_FIRST As Long = &H1000
Public Const LVM_GETHEADER As Long = LVM_FIRST + 31
Public Const LVM_GETITEMRECT As Long = LVM_FIRST + 14

Public Const LVIR_BOUNDS As Long = 1
Public Const LVIR_ICON As Long = 2
Public Const LVIR_LABEL As Long = 3
Public Const LVIR_SELECTBOUNDS As Long = 4
    
Public Const WM_NOTIFY = &H4E&


Public Enum WinNotifications
   NM_FIRST = -0& ' (0U- 0U) ' // generic to all controls
   NM_LAST = -99& ' (0U- 99U)
   NM_OUTOFMEMORY = (NM_FIRST - 1)
   NM_CLICK = (NM_FIRST - 2)
   NM_DBLCLK = (NM_FIRST - 3)
   NM_RETURN = (NM_FIRST - 4)
   NM_RCLICK = (NM_FIRST - 5)
   NM_RDBLCLK = (NM_FIRST - 6)
   NM_SETFOCUS = (NM_FIRST - 7)
   NM_KILLFOCUS = (NM_FIRST - 8)
   NM_CUSTOMDRAW = (NM_FIRST - 12)
   NM_HOVER = (NM_FIRST - 13)
End Enum

Public Type NMHDR
    hwndFrom As Long
    idfrom As Long
    code As Long
End Type

Public Type NMCUSTOMDRAWINFO
   hdr As NMHDR
   dwDrawStage As Long
   hdc As Long
   rc As RECT
   dwItemSpec As Long
   iItemState As Long
   lItemLParam As Long
End Type

Public Type NMLVCUSTOMDRAW
   nmcmd As NMCUSTOMDRAWINFO
   clrText As Long
   clrTextBk As Long
   iSubItem As Long
End Type


' Custom Draw Messages
'
Public Const CDDS_PREPAINT& = &H1
Public Const CDDS_POSTPAINT& = &H2
Public Const CDDS_PREERASE& = &H3
Public Const CDDS_POSTERASE& = &H4
Public Const CDDS_ITEM& = &H10000
Public Const CDDS_ITEMPREPAINT& = CDDS_ITEM Or CDDS_PREPAINT
Public Const CDDS_ITEMPOSTPAINT& = CDDS_ITEM Or CDDS_POSTPAINT
Public Const CDDS_ITEMPREERASE& = CDDS_ITEM Or CDDS_PREERASE
Public Const CDDS_ITEMPOSTERASE& = CDDS_ITEM Or CDDS_POSTERASE
Public Const CDDS_SUBITEM& = &H20000

Public Const CDRF_DODEFAULT& = &H0
Public Const CDRF_NEWFONT& = &H2
Public Const CDRF_SKIPDEFAULT& = &H4
Public Const CDRF_NOTIFYPOSTPAINT& = &H10
Public Const CDRF_NOTIFYITEMDRAW& = &H20
Public Const CDRF_NOTIFYSUBITEMDRAW = &H20      ' flags are the same, we can distinguish by context
Public Const CDRF_NOTIFYPOSTERASE& = &H40
Public Const CDRF_NOTIFYITEMERASE& = &H80
'
Public Function EnableColumnHeader(ByVal ListViewhWnd As Long, ByVal fEnable As Boolean) As Boolean
    EnableColumnHeader = EnableWindow(SendMessage(ListViewhWnd, LVM_GETHEADER, 0&, ByVal 0&), -fEnable)
End Function


Public Function GetItemRect(ByVal ListViewhWnd As Long, ByVal index As Long, utRect As RECT) As Long
    GetItemRect = SendMessage(ListViewhWnd, LVM_GETITEMRECT, index, utRect)
End Function

'主要的地方
Public Function WindowProc(ByVal hwnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    On Error Resume Next
    Static CurrentPos As Long
    Dim i As Long
    Dim lScollCode As Long
    Dim lPos As Long
    Dim iInc As Integer
    Dim ut As NCCALCSIZE_PARAMS
    
    Select Case uMsg
        Case WM_NCLBUTTONDOWN, WM_NCLBUTTONUP, WM_KEYDOWN, WM_SIZE
            WindowProc = CallWindowProc(glDefWindowProc, hwnd, uMsg, wParam, lParam)
            SyncScrollbar hwnd
        Case WM_VSCROLL, WM_MOUSEWHEEL, WHEEL_PAGESCROLL
            WindowProc = CallWindowProc(glDefWindowProc, hwnd, uMsg, wParam, lParam)

            lScollCode = LOWORD(wParam)
            lPos = HIWORD(wParam)
            Select Case lScollCode
            Case SB_THUMBTRACK
                iInc = IIf((CurrentPos - lPos) < 0, SB_LINEDOWN, SB_LINEUP)
                For i = 1 To Abs(CurrentPos - lPos)
                    SendMessage hwnd, WM_VSCROLL, MAKELONG(0, iInc), 0&
                Next
                CurrentPos = lPos
                SyncScrollbar hwnd
            End Select
        Case Else
            WindowProc = CallWindowProc(glDefWindowProc, hwnd, uMsg, wParam, lParam)
     End Select
   
End Function

'重画窗体
Sub RepaintWindow(ByVal hwnd As Long)
    SendMessage hwnd, WM_NCPAINT, 1&, 0&
End Sub

Function HIWORD(ByVal lVal As Long) As Long
    HIWORD = (lVal And &HFFFF0000) / &H10000
End Function
Function LOWORD(ByVal lVal As Long) As Long
    LOWORD = lVal And &HFFFF&
End Function

Function MAKELONG(HiBits As Integer, LoBits As Integer) As Long
    MAKELONG = CLng(HiBits) * &H10000 + LoBits
End Function

'同步系统默认滚动条和我们滚动条上滑块的位置等信息
Function SyncScrollbar(ByVal hwnd As Long) As Boolean
    Dim si As SCROLLINFO
    With si
        .cbSize = Len(si)
        .fMask = SIF_ALL
        GetScrollInfo hwnd, SB_VERT, si
        FlatSB_SetScrollInfo hwnd, SB_VERT, si, True
        ShowScrollBar hwnd, SB_VERT, False
    End With
    RepaintWindow hwnd
End Function

Function GetScrollBarWidth(ByVal hwnd As Long) As Long
    FlatSB_GetScrollProp hwnd, WSB_PROP_CXVSCROLL, GetScrollBarWidth
End Function
