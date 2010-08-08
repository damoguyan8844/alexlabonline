VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CSimpleSplitter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"

' ************************   文 件 名 :SimpleSplitter.cls ********************
'版 本 号: V1.0
'作    者: 超级绿豆
'日    期: 2003年03月25日
'******************************************************************************/
'
'本模块名称
Private Const THIS_MODULE_NAME As String = "CSimpleSplitter"
'
Private Declare Function LockWindowUpdate Lib "user32" (ByVal hwndLock As Long) As Long

'内存操作
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
'鼠标
Private Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Private Declare Function ClipCursor Lib "user32" (lpRect As Any) As Long
'矩形结构信息
Private Declare Function GetWindowRect Lib "user32" (ByVal hwnd As Long, lpRect As RECT) As Long
Private Declare Function GetClientRect Lib "user32" (ByVal hwnd As Long, lpRect As RECT) As Long
Private Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type
'设定窗口位置、大小
Private Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Private Const HWND_BOTTOM = 1
Private Const HWND_NOTOPMOST = -2
Private Const HWND_TOP = 0
Private Const HWND_TOPMOST = -1
Private Const SWP_HIDEWINDOW = &H80
Private Const SWP_NOACTIVATE = &H10
Private Const SWP_NOMOVE = &H2
Private Const SWP_NOOWNERZORDER = &H200      '  Don't do owner Z ordering
Private Const SWP_NOREDRAW = &H8
Private Const SWP_NOSIZE = &H1
Private Const SWP_NOZORDER = &H4
Private Const SWP_SHOWWINDOW = &H40
'鼠标捕获
Private Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long
Private Declare Function ReleaseCapture Lib "user32" () As Long
'绘图
Private Declare Function SelectObject Lib "gdi32" (ByVal hDC As Long, ByVal hObject As Long) As Long
Private Declare Function CreatePen Lib "gdi32" (ByVal nPenStyle As Long, ByVal nWidth As Long, ByVal crColor As Long) As Long
Private Declare Function Polyline Lib "gdi32" (ByVal hDC As Long, lpPoint As POINTAPI, ByVal nCount As Long) As Long
Private Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Private Declare Function MoveToEx Lib "gdi32" (ByVal hDC As Long, ByVal X As Long, ByVal Y As Long, lpPoint As POINTAPI) As Long
Private Declare Function LineTo Lib "gdi32" (ByVal hDC As Long, ByVal X As Long, ByVal Y As Long) As Long
Private Declare Function ScreenToClient Lib "user32" (ByVal hwnd As Long, lpPoint As POINTAPI) As Long
Private Declare Function SetROP2 Lib "gdi32" (ByVal hDC As Long, ByVal nDrawMode As Long) As Long
Private Declare Function GetROP2 Lib "gdi32" (ByVal hDC As Long) As Long
Private Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
Private Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hDC As Long) As Long
Private Declare Function GetDesktopWindow Lib "user32" () As Long
Private Type POINTAPI
        X As Long
        Y As Long
End Type
Private Type LOGBRUSH
        lbStyle As Long
        lbColor As Long
        lbHatch As Long
End Type
Private Const PS_ALTERNATE = 8
Private Const PS_COSMETIC = &H0
Private Const PS_DASH = 1                    '  -------
Private Const PS_DASHDOT = 3                 '  _._._._
Private Const PS_DASHDOTDOT = 4              '  _.._.._
Private Const PS_DOT = 2                     '  .......
Private Const PS_ENDCAP_FLAT = &H200
Private Const PS_ENDCAP_MASK = &HF00
Private Const PS_ENDCAP_ROUND = &H0
Private Const PS_ENDCAP_SQUARE = &H100
Private Const PS_GEOMETRIC = &H10000
Private Const PS_INSIDEFRAME = 6
Private Const PS_JOIN_BEVEL = &H1000
Private Const PS_JOIN_MASK = &HF000
Private Const PS_JOIN_MITER = &H2000
Private Const PS_JOIN_ROUND = &H0
Private Const PS_NULL = 5
Private Const PS_SOLID = 0
Private Const PS_STYLE_MASK = &HF
Private Const PS_TYPE_MASK = &HF0000
Private Const PS_USERSTYLE = 7
Private Const R2_BLACK = 1       '   0
Private Const R2_COPYPEN = 13    '  P
Private Const R2_LAST = 16
Private Const R2_MASKNOTPEN = 3  '  DPna
Private Const R2_MASKPEN = 9     '  DPa
Private Const R2_MASKPENNOT = 5  '  PDna
Private Const R2_MERGENOTPEN = 12        '  DPno
Private Const R2_MERGEPEN = 15   '  DPo
Private Const R2_MERGEPENNOT = 14        '  PDno
Private Const R2_NOP = 11        '  D
Private Const R2_NOT = 6 '  Dn
Private Const R2_NOTCOPYPEN = 4  '  PN
Private Const R2_NOTMASKPEN = 8  '  DPan
Private Const R2_NOTMERGEPEN = 2 '  DPon
Private Const R2_NOTXORPEN = 10  '  DPxn
Private Const R2_WHITE = 16      '   1
Private Const R2_XORPEN = 7      '  DPx
'
Private m_sngPercentageOfLeftPanel As Single
'
Private m_oLeftPanel As Object, m_oRightPanel As Object, m_oContainerPanel As Object
'
Private m_fIsVertical As Boolean
'分隔条宽度
Private m_lSplitterBarSize As Long, m_udtSplitterBarPos As RECT, m_udtSplitterBarPosBackup As RECT

Private m_udtCursorPos As POINTAPI

Private m_lWidth As Long, m_lHeight As Long, m_lLeftPanelWidth As Long, m_lRightPanelWidth As Long, m_lLeftPanelHeight As Long, m_lRightPanelHeight As Long

Private m_fStartDragSplitterBar As Boolean

Private m_hPen As Long, m_hPenOld As Long, m_ROP2ModeOld As Long, m_hDesktopDC As Long

'分隔条颜色
Private m_lSplitterBarColor As Long
'
Private m_lDrawSplitterBarSize As Long
'固定大小
Private m_lFixedLeftPanelSize As Long
'
Private m_lFixedRightPanelSize As Long

Public Property Let FixedRightPanelSize(ByVal lFixedRightPanelSize As Long)
    m_lFixedRightPanelSize = lFixedRightPanelSize
    m_lFixedLeftPanelSize = -1
End Property

Public Property Get FixedRightPanelSize() As Long
     FixedRightPanelSize = m_lFixedRightPanelSize
End Property


Public Property Let FixedLeftPanelSize(ByVal lData As Long)
    m_lFixedLeftPanelSize = lData
    m_lFixedRightPanelSize = -1
End Property

Public Property Get FixedLeftPanelSize() As Long
    FixedLeftPanelSize = m_lFixedLeftPanelSize
End Property

Public Property Let DrawSplitterBarSize(ByVal lDrawSplitterBarSize As Long)
    m_lDrawSplitterBarSize = lDrawSplitterBarSize
End Property

Public Property Get DrawSplitterBarSize() As Long
    DrawSplitterBarSize = m_lDrawSplitterBarSize
End Property

Public Property Let SplitterBarColor(ByVal lColor As Long)
    m_lSplitterBarColor = lColor
End Property

Public Property Get SplitterBarColor() As Long
    SplitterBarColor = m_lSplitterBarColor
End Property

Public Property Let SplitterBarSize(ByVal lSize As Long)
    m_lSplitterBarSize = lSize
End Property

Public Property Get SplitterBarSize() As Long
    SplitterBarSize = m_lSplitterBarSize
End Property

Public Property Let IsVertical(ByVal fValue As Boolean)
    m_fIsVertical = fValue
End Property

Public Property Get IsVertical() As Boolean
    IsVertical = m_fIsVertical
End Property

Public Property Let PercentageOfLeftPanel(ByVal sngPercentage As Single)
    m_sngPercentageOfLeftPanel = sngPercentage
    If m_sngPercentageOfLeftPanel > 1 Then
        m_sngPercentageOfLeftPanel = 1
    End If
    If m_sngPercentageOfLeftPanel < 0 Then
        m_sngPercentageOfLeftPanel = 0
    End If
    Call Resize
End Property

Public Property Get PercentageOfLeftPanel() As Single
    PercentageOfLeftPanel = m_sngPercentageOfLeftPanel
End Property

Public Function Attach(oLeftPanel As Object, oRightPanel As Object, oContainerPanel As Object) As Boolean
    On Error GoTo Error_Handler
    Set m_oLeftPanel = oLeftPanel
    Set m_oRightPanel = oRightPanel
    Set m_oContainerPanel = oContainerPanel
    
    Call Me.Resize
                
    Attach = True
    Exit Function
Error_Handler:
    '自定义错误处理
    
    '调用默认错误处理函数
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

Public Function Resize() As Boolean
    On Error GoTo Error_Handler
    Dim r As RECT
    If m_oContainerPanel Is Nothing Then Exit Function
    If GetClientRect(m_oContainerPanel.hwnd, r) <> 0 Then
    
        If m_fIsVertical Then
            m_lHeight = r.Bottom - r.Top
            m_lWidth = r.Right - r.Left - m_lSplitterBarSize
            
            m_lLeftPanelWidth = m_lWidth * m_sngPercentageOfLeftPanel
            m_lLeftPanelHeight = m_lHeight
            If m_lFixedLeftPanelSize > -1 And m_lFixedLeftPanelSize <= m_lWidth Then
                m_lLeftPanelWidth = m_lFixedLeftPanelSize
            End If
            
            m_lRightPanelWidth = m_lWidth * (1 - m_sngPercentageOfLeftPanel)
            If m_lFixedLeftPanelSize > -1 And m_lFixedLeftPanelSize <= m_lWidth Then
                m_lRightPanelWidth = m_lWidth - m_lLeftPanelWidth
            End If
            m_lRightPanelHeight = m_lHeight
            
            If m_lFixedRightPanelSize > -1 And m_lFixedRightPanelSize <= m_lWidth Then
                m_lRightPanelWidth = m_lFixedRightPanelSize
                m_lLeftPanelWidth = m_lWidth - m_lRightPanelHeight
            End If
            
            m_lSplitterBarSize = m_lSplitterBarSize
            
            If Not m_oLeftPanel Is Nothing Then
                SetWindowPos m_oLeftPanel.hwnd, 0&, r.Left, r.Top, m_lLeftPanelWidth, m_lHeight, 0&
            End If
            
            If Not m_oRightPanel Is Nothing Then
                SetWindowPos m_oRightPanel.hwnd, 0&, r.Left + m_lLeftPanelWidth + m_lSplitterBarSize, r.Top, m_lRightPanelWidth, m_lHeight, 0&
            End If
        Else
            m_lHeight = r.Bottom - r.Top - m_lSplitterBarSize
            m_lWidth = r.Right - r.Left
            
            m_lLeftPanelWidth = m_lWidth
            m_lLeftPanelHeight = m_lHeight * m_sngPercentageOfLeftPanel
            If m_lFixedLeftPanelSize > -1 And m_lFixedLeftPanelSize <= m_lHeight Then
                m_lLeftPanelHeight = m_lFixedLeftPanelSize
            End If
            m_lRightPanelWidth = m_lWidth
            m_lRightPanelHeight = m_lHeight * (1 - m_sngPercentageOfLeftPanel)
            If m_lFixedLeftPanelSize > -1 And m_lFixedLeftPanelSize <= m_lHeight Then
                m_lRightPanelHeight = m_lHeight - m_lLeftPanelHeight
            End If
            
            If m_lFixedRightPanelSize > -1 And m_lFixedRightPanelSize <= m_lWidth Then
                m_lRightPanelHeight = m_lFixedRightPanelSize
                m_lLeftPanelHeight = m_lHeight - m_lRightPanelHeight
            End If
            
            m_lSplitterBarSize = m_lSplitterBarSize
            
            If Not m_oLeftPanel Is Nothing Then
                SetWindowPos m_oLeftPanel.hwnd, 0&, r.Left, r.Top, m_lLeftPanelWidth, m_lLeftPanelHeight, 0&
            End If
            
            If Not m_oRightPanel Is Nothing Then
                SetWindowPos m_oRightPanel.hwnd, 0&, r.Left, r.Top + m_lLeftPanelHeight + m_lSplitterBarSize, m_lRightPanelWidth, m_lRightPanelHeight, 0&
            End If
        End If
        Call GetSplitterBarRECT
        Resize = True
    End If
    Exit Function
Error_Handler:
    '自定义错误处理
    
    '调用默认错误处理函数
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function

Private Sub Class_Initialize()
    m_sngPercentageOfLeftPanel = 0.5
    m_lSplitterBarSize = 10
    m_fIsVertical = True
    m_lSplitterBarColor = RGB(0, 0, 0)
    m_lDrawSplitterBarSize = 3
    m_lFixedLeftPanelSize = -1
    m_lFixedRightPanelSize = -1
End Sub


Public Function MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single) As Boolean
    Dim lXPosInPixel As Long, lYPosInPixel As Long
    Dim udtCurrentMousePosition As POINTAPI
    lXPosInPixel = X / Screen.TwipsPerPixelX
    lYPosInPixel = Y / Screen.TwipsPerPixelY
    If m_oContainerPanel Is Nothing Or m_oLeftPanel Is Nothing Or m_oRightPanel Is Nothing Then Exit Function
    
    If m_fStartDragSplitterBar = False Then
        If m_fIsVertical Then
            If lXPosInPixel >= m_lLeftPanelWidth And lXPosInPixel <= m_lLeftPanelWidth + m_lSplitterBarSize Then
                    Call SetCapture(m_oContainerPanel.hwnd)
                    m_oContainerPanel.MousePointer = 9
                Else
                    Call ReleaseCapture
                    m_oContainerPanel.MousePointer = 0
                End If
        Else
            If lYPosInPixel >= m_lLeftPanelHeight And lYPosInPixel <= m_lLeftPanelHeight + m_lSplitterBarSize Then
                Call SetCapture(m_oContainerPanel.hwnd)
                m_oContainerPanel.MousePointer = 7
            Else
                Call ReleaseCapture
                m_oContainerPanel.MousePointer = 0
            End If
        End If
    Else
        m_oContainerPanel.MousePointer = IIf(m_fIsVertical, 9, 7)
    End If
End Function

Public Function MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single) As Boolean
    Dim lXPosInPixel As Long, lYPosInPixel As Long
    Dim r As RECT
    
    lXPosInPixel = X / Screen.TwipsPerPixelX
    lYPosInPixel = Y / Screen.TwipsPerPixelY
    
    If m_oContainerPanel Is Nothing Or m_oLeftPanel Is Nothing Or m_oRightPanel Is Nothing Then Exit Function
    If m_fIsVertical Then
        If lXPosInPixel >= m_lLeftPanelWidth And lXPosInPixel <= m_lLeftPanelWidth + m_lSplitterBarSize Then
    Call GetDesktopDC
    Call ChangePenInSpecificDC(m_hDesktopDC, PS_SOLID, m_lDrawSplitterBarSize, m_lSplitterBarColor)
            Call SetCapture(m_oContainerPanel.hwnd)
            m_fStartDragSplitterBar = True
            LockWindowUpdate m_hDesktopDC
            
            Call GetSplitterBarRECT
            Call DrawSplitterBar
        
            Call GetWindowRect(m_oContainerPanel.hwnd, r)
            ClipCursor ByVal r
            Do While m_fStartDragSplitterBar = True
                Call GetCursorPos(m_udtCursorPos)
                With m_udtSplitterBarPos
                    If Not .Left = m_udtCursorPos.X Then
                        Call DrawSplitterBar
                        .Left = m_udtCursorPos.X
                        Call DrawSplitterBar
                    End If
                End With

                DoEvents
            Loop
            
            LockWindowUpdate 0&
    Call ReleasePenInSpecificDC(m_hDesktopDC)
    Call ReleaseDesktopDC
        End If
    Else
        If lYPosInPixel >= m_lLeftPanelHeight And lYPosInPixel <= m_lLeftPanelHeight + m_lSplitterBarSize Then
            Call SetCapture(m_oContainerPanel.hwnd)
            m_fStartDragSplitterBar = True
            Call GetSplitterBarRECT
            Call DrawSplitterBar
            
            Do While m_fStartDragSplitterBar = True
                Call GetCursorPos(m_udtCursorPos)
                With m_udtSplitterBarPos
                    If Not .Top = m_udtCursorPos.Y Then
                        Call DrawSplitterBar
                        .Top = m_udtCursorPos.Y
                        Call DrawSplitterBar
                    End If
                End With
                
                DoEvents
            Loop
        End If
    End If
    
End Function

Public Function MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single) As Boolean
    Dim sngAdjustedPercent As Single
    If m_oContainerPanel Is Nothing Or m_oLeftPanel Is Nothing Or m_oRightPanel Is Nothing Then Exit Function
    
    m_fStartDragSplitterBar = False
    Call DrawSplitterBar
    Call MouseMove(Button, Shift, X, Y)
    Call ClipCursor(ByVal 0&)
    
    CopyRECT m_udtSplitterBarPos, m_udtSplitterBarPosBackup
    Call GetSplitterBarRECT
    If m_fIsVertical = True Then
        If m_lLeftPanelWidth = 0 Then
            sngAdjustedPercent = ((m_udtCursorPos.X - m_udtSplitterBarPos.Right) + m_lLeftPanelWidth) / m_lWidth
            Me.PercentageOfLeftPanel = sngAdjustedPercent
        Else
            sngAdjustedPercent = ((m_udtCursorPos.X - m_udtSplitterBarPos.Right) + m_lLeftPanelWidth) / m_lLeftPanelWidth
            Me.PercentageOfLeftPanel = Me.PercentageOfLeftPanel * sngAdjustedPercent
        End If
    Else
        If m_lLeftPanelHeight = 0 Then
            sngAdjustedPercent = ((m_udtCursorPos.Y - m_udtSplitterBarPos.Bottom) + m_lLeftPanelHeight) / m_lHeight
            Me.PercentageOfLeftPanel = sngAdjustedPercent
        Else
            sngAdjustedPercent = ((m_udtCursorPos.Y - m_udtSplitterBarPos.Bottom) + m_lLeftPanelHeight) / m_lLeftPanelHeight
            Me.PercentageOfLeftPanel = Me.PercentageOfLeftPanel * sngAdjustedPercent
        End If
    End If
    CopyRECT m_udtSplitterBarPosBackup, m_udtSplitterBarPos
    
End Function

Private Function ChangePenInSpecificDC(hDC As Long, lPenStyle As Long, lWidth As Long, lColor As Long) As Long
    m_hPen = CreatePen(lPenStyle, lWidth, lColor)
    m_hPenOld = SelectObject(hDC, m_hPen)
    m_ROP2ModeOld = GetROP2(hDC)
    SetROP2 hDC, R2_NOTXORPEN
End Function

Private Function ReleasePenInSpecificDC(hDC As Long) As Long
    SelectObject hDC, m_hPenOld
    DeleteObject m_hPen
    SetROP2 hDC, m_ROP2ModeOld
End Function

Private Function DrawSplitterBar()
    Static p(1) As POINTAPI
    
    If m_fIsVertical = True Then
        With m_udtSplitterBarPos
            p(0).X = .Left
            p(0).Y = .Top
            p(1).X = .Left
            p(1).Y = .Bottom
        End With
    Else
        With m_udtSplitterBarPos
            p(0).X = .Left
            p(0).Y = .Top
            p(1).X = .Right
            p(1).Y = .Top
        End With
    End If
'    Call GetDesktopDC
'    Call ChangePenInSpecificDC(m_hDesktopDC, PS_SOLID, m_lDrawSplitterBarSize, m_lSplitterBarColor)
    Call Polyline(m_hDesktopDC, p(0), 2)
'    Call ReleasePenInSpecificDC(m_hDesktopDC)
'    Call ReleaseDesktopDC
End Function

Private Function GetSplitterBarRECT() As Long
    On Error Resume Next
    Dim r As RECT
    If Not m_oLeftPanel Is Nothing Then
        Call GetWindowRect(m_oLeftPanel.hwnd, r)
        If m_fIsVertical = True Then
            With m_udtSplitterBarPos
                .Left = r.Right
                .Right = .Left + m_lSplitterBarSize
                .Top = r.Top
                .Bottom = r.Bottom
            End With
        Else
            With m_udtSplitterBarPos
                .Left = r.Left
                .Right = r.Right
                .Top = r.Bottom
                .Bottom = r.Bottom + m_lSplitterBarSize
            End With
        End If
    End If
    
    If m_oLeftPanel Is Nothing And Not m_oRightPanel Is Nothing Then
        Call GetWindowRect(m_oRightPanel.hwnd, r)
        If m_fIsVertical = True Then
            With m_udtSplitterBarPos
                .Left = .Left - m_lSplitterBarSize
                .Right = r.Left
                .Top = r.Top
                .Bottom = r.Bottom
            End With
        Else
            With m_udtSplitterBarPos
                .Left = r.Left
                .Right = r.Right
                .Top = r.Bottom - m_lSplitterBarSize
                .Bottom = r.Bottom
            End With
        End If
    End If
End Function

Private Function GetDesktopDC() As Long
    m_hDesktopDC = GetDC(0&)
End Function

Private Function ReleaseDesktopDC() As Long
    ReleaseDC GetDesktopWindow, m_hDesktopDC
End Function

Private Function CopyRECT(r1 As RECT, r2 As RECT) As Boolean
    CopyMemory r2, r1, Len(r1)
End Function

