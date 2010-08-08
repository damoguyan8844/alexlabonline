Attribute VB_Name = "mduSimpleSplitter"
Option Explicit
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

'鼠标
Public Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Public Declare Function ClipCursor Lib "user32" (lpRect As Any) As Long


'矩形结构信息
Public Declare Function GetWindowRect Lib "user32" (ByVal hwnd As Long, lpRect As RECT) As Long
Public Declare Function GetClientRect Lib "user32" (ByVal hwnd As Long, lpRect As RECT) As Long

Public Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type

'设定窗口位置、大小
Public Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long

Public Const HWND_BOTTOM = 1
Public Const HWND_NOTOPMOST = -2
Public Const HWND_TOP = 0
Public Const HWND_TOPMOST = -1
Public Const SWP_HIDEWINDOW = &H80
Public Const SWP_NOACTIVATE = &H10
Public Const SWP_NOMOVE = &H2
Public Const SWP_NOOWNERZORDER = &H200      '  Don't do owner Z ordering
Public Const SWP_NOREDRAW = &H8
Public Const SWP_NOSIZE = &H1
Public Const SWP_NOZORDER = &H4
Public Const SWP_SHOWWINDOW = &H40
'鼠标捕获
Public Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function ReleaseCapture Lib "user32" () As Long

'绘图
Public Declare Function SelectObject Lib "gdi32" (ByVal hDC As Long, ByVal hObject As Long) As Long
Public Declare Function CreatePen Lib "gdi32" (ByVal nPenStyle As Long, ByVal nWidth As Long, ByVal crColor As Long) As Long
Public Declare Function Polyline Lib "gdi32" (ByVal hDC As Long, lpPoint As POINTAPI, ByVal nCount As Long) As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Public Declare Function MoveToEx Lib "gdi32" (ByVal hDC As Long, ByVal X As Long, ByVal Y As Long, lpPoint As POINTAPI) As Long
Public Declare Function LineTo Lib "gdi32" (ByVal hDC As Long, ByVal X As Long, ByVal Y As Long) As Long
Public Declare Function ScreenToClient Lib "user32" (ByVal hwnd As Long, lpPoint As POINTAPI) As Long
Public Declare Function SetROP2 Lib "gdi32" (ByVal hDC As Long, ByVal nDrawMode As Long) As Long
Public Declare Function GetROP2 Lib "gdi32" (ByVal hDC As Long) As Long
Public Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hDC As Long) As Long
Public Declare Function GetDesktopWindow Lib "user32" () As Long

Public Type POINTAPI
        X As Long
        Y As Long
End Type

Public Type LOGBRUSH
        lbStyle As Long
        lbColor As Long
        lbHatch As Long
End Type

Public Const PS_ALTERNATE = 8
Public Const PS_COSMETIC = &H0
Public Const PS_DASH = 1                    '  -------
Public Const PS_DASHDOT = 3                 '  _._._._
Public Const PS_DASHDOTDOT = 4              '  _.._.._
Public Const PS_DOT = 2                     '  .......
Public Const PS_ENDCAP_FLAT = &H200
Public Const PS_ENDCAP_MASK = &HF00
Public Const PS_ENDCAP_ROUND = &H0
Public Const PS_ENDCAP_SQUARE = &H100
Public Const PS_GEOMETRIC = &H10000
Public Const PS_INSIDEFRAME = 6
Public Const PS_JOIN_BEVEL = &H1000
Public Const PS_JOIN_MASK = &HF000
Public Const PS_JOIN_MITER = &H2000
Public Const PS_JOIN_ROUND = &H0
Public Const PS_NULL = 5
Public Const PS_SOLID = 0
Public Const PS_STYLE_MASK = &HF
Public Const PS_TYPE_MASK = &HF0000
Public Const PS_USERSTYLE = 7

Public Const R2_BLACK = 1       '   0
Public Const R2_COPYPEN = 13    '  P
Public Const R2_LAST = 16
Public Const R2_MASKNOTPEN = 3  '  DPna
Public Const R2_MASKPEN = 9     '  DPa
Public Const R2_MASKPENNOT = 5  '  PDna
Public Const R2_MERGENOTPEN = 12        '  DPno
Public Const R2_MERGEPEN = 15   '  DPo
Public Const R2_MERGEPENNOT = 14        '  PDno
Public Const R2_NOP = 11        '  D
Public Const R2_NOT = 6 '  Dn
Public Const R2_NOTCOPYPEN = 4  '  PN
Public Const R2_NOTMASKPEN = 8  '  DPan
Public Const R2_NOTMERGEPEN = 2 '  DPon
Public Const R2_NOTXORPEN = 10  '  DPxn
Public Const R2_WHITE = 16      '   1
Public Const R2_XORPEN = 7      '  DPx




'/**********************************函 数 名  : DefaultErrorHandler*********************/
'功能描述:    默认错误处理函数
'输入参数:

'输出参数:    无
'返 回 值:    String
'调用函数:
'被调函数:
'
'修改历史:
'创建日期:    2003-03-17 15:36:24
'作    者:    陈缘
'修改方式:    新生成函数
'/**************************************************************************************/

Public Function DefaultErrorHandler(Optional sSourceModule As String) As String
    Dim s As String
    s = "ErrNO:" + CStr(Err.Number) + vbCrLf + "Description:" + Err.Description + vbCrLf + "Source:" + Err.Source + vbCrLf + "From Module:" + sSourceModule
    MsgBox s
    Debug.Print Err.Number, Err.Description, Err.Source, sSourceModule
End Function



'/**********************************函 数 名  : RaiseError*********************/
'功能描述:    生成错误消息
'输入参数:
'            lErrorNumber
'             lErrorDescription
'             lErrorSource

'输出参数:    无
'返 回 值:    String
'调用函数:
'被调函数:
'
'修改历史:
'创建日期:    2003-03-17 15:37:20
'作    者:    陈缘
'修改方式:    新生成函数
'/*****************************************************************************/

Public Function RaiseError(lErrorNumber, lErrorDescription, lErrorSource) As String
    Err.Raise Number:=lErrorNumber, Description:=lErrorDescription, Source:=lErrorSource
End Function


