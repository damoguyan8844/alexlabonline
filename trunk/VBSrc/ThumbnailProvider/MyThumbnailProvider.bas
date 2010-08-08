Attribute VB_Name = "modMyThumbnailProvider"
'***********************************************************************************************************************
'
'模 块 名: modMyThumbnailProvier
'
'作    者: Supergreenbean
'
'功能描述: 根据给定的文本内容创建位图
'
'日    期: 2009年03月10日
'
'使用说明:
'
'       1. CreateThumbnail(显示于缩略图中的文本内容)
'
'***********************************************************************************************************************
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "modMyThumbnailProvier"

Public Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hdc As Long) As Long
Public Declare Function CreateCompatibleBitmap Lib "gdi32" (ByVal hdc As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Public Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Public Declare Function TextOut Lib "gdi32" Alias "TextOutA" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal lpString As String, ByVal nCount As Long) As Long
Public Declare Function CreatePen Lib "gdi32" (ByVal nPenStyle As Long, ByVal nWidth As Long, ByVal crColor As Long) As Long
Public Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function DrawText Lib "user32" Alias "DrawTextA" (ByVal hdc As Long, ByVal lpStr As String, ByVal nCount As Long, lpRect As RECT, ByVal wFormat As Long) As Long
Public Declare Function SetTextColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
Public Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long
Public Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long
Public Declare Function SetBkMode Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long
Public Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Public Declare Function StretchBlt Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal nSrcWidth As Long, ByVal nSrcHeight As Long, ByVal dwRop As Long) As Long
Public Const SRCCOPY = &HCC0020 ' (DWORD) dest = source

Public Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type

Public Const DT_CENTER = &H1
Public Const DT_VCENTER = &H4
Public Const DT_WORDBREAK = &H10
Public Const DT_CALCRECT = &H400
Public Const DT_END_ELLIPSIS = &H8000

Public Const TRANSPARENT = 1

'创建缩略图
Public Function CreateThumbnail(ByVal s As String) As Long
    Dim hBmp As Long, hOldBmp As Long, hTmpDC As Long
    Dim rc As RECT
    Dim hBrush As Long
    
    hTmpDC = CreateCompatibleDC(0)
    
    DrawText hTmpDC, s, Len(s), rc, DT_CALCRECT
    
    hBmp = CreateCompatibleBitmap(hTmpDC, rc.Right + 1, rc.Bottom + 1)
    hOldBmp = SelectObject(hTmpDC, hBmp)
    
    With rc
        .Left = 1
        .Top = 1
        .Right = .Right - 1
        .Bottom = .Bottom - 1
    End With
    
    hBrush = CreateSolidBrush(RGB(255, 255, 255))
    FillRect hTmpDC, rc, hBrush
    DeleteObject hBrush
    
    SetBkMode hTmpDC, TRANSPARENT
    
    DrawText hTmpDC, s, Len(s), rc, DT_CENTER Or DT_VCENTER
    
    CreateThumbnail = hBmp
    
    SelectObject hTmpDC, hOldBmp
    DeleteObject hTmpDC
End Function

Public Function CreateThumbnailWithSize(ByVal s As String, cx As Long, cy As Long, Optional clrDepth As Long = 0) As Long
    Dim hBmp As Long, hOldBmp As Long, hBmp2 As Long, hOldBmp2 As Long, hTmpDC As Long, hTmpDC2 As Long
    Dim rc As RECT
    Dim hBrush As Long
    
    hTmpDC = CreateCompatibleDC(0)
    hTmpDC2 = CreateCompatibleDC(0)
    
    DrawText hTmpDC, s, Len(s), rc, DT_CALCRECT
    
    hBmp = CreateCompatibleBitmap(hTmpDC, rc.Right + 1, rc.Bottom + 1)
    hOldBmp = SelectObject(hTmpDC, hBmp)
    hBmp2 = CreateCompatibleBitmap(hTmpDC2, cx + 1, cy + 1)
    hOldBmp2 = SelectObject(hTmpDC2, hBmp2)
    
    With rc
        .Left = 1
        .Top = 1
        .Right = .Right - 1
        .Bottom = .Bottom - 1
    End With
    
    hBrush = CreateSolidBrush(RGB(255, 255, 255))
    FillRect hTmpDC, rc, hBrush
    DeleteObject hBrush
    
    SetBkMode hTmpDC, TRANSPARENT
    
    DrawText hTmpDC, s, Len(s), rc, DT_CENTER Or DT_VCENTER
    
    StretchBlt hTmpDC2, 0, 0, cx, cy, hTmpDC, 0, 0, rc.Right, rc.Bottom, SRCCOPY
    
    CreateThumbnailWithSize = hBmp2
    
    SelectObject hTmpDC, hOldBmp
    SelectObject hTmpDC2, hOldBmp2
    DeleteObject hBmp
    DeleteObject hTmpDC
End Function

