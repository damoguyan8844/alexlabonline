Attribute VB_Name = "mduDiagram"
Public Declare Function GetFileAttributes Lib "kernel32" Alias "GetFileAttributesA" (ByVal lpFileName As String) As Long
Public Const FILE_ATTRIBUTE_DIRECTORY = &H10

Public Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Public Const HWND_TOPMOST = -1
Public Const SWP_NOMOVE = &H2
Public Const SWP_NOSIZE = &H1

Public Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function ReleaseCapture Lib "user32" () As Long

Public Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long
Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

Public Declare Function SetWindowRgn Lib "user32" (ByVal hwnd As Long, ByVal hRgn As Long, ByVal bRedraw As Boolean) As Long
Public Declare Function CreateEllipticRgn Lib "gdi32" (ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
Public Declare Function CreateEllipticRgnIndirect Lib "gdi32" (lpRect As Rect) As Long
Public Declare Function CreatePolygonRgn Lib "gdi32" (lpPoint As POINTAPI, ByVal nCount As Long, ByVal nPolyFillMode As Long) As Long
Public Declare Function CreatePolyPolygonRgn Lib "gdi32" (lpPoint As POINTAPI, lpPolyCounts As Long, ByVal nCount As Long, ByVal nPolyFillMode As Long) As Long
Public Declare Function CreateRectRgn Lib "gdi32" (ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
Public Declare Function CreateRectRgnIndirect Lib "gdi32" (lpRect As Rect) As Long
Public Declare Function CreateRoundRectRgn Lib "gdi32" (ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long, ByVal X3 As Long, ByVal Y3 As Long) As Long
Public Declare Function CombineRgn Lib "gdi32" (ByVal hDestRgn As Long, ByVal hSrcRgn1 As Long, ByVal hSrcRgn2 As Long, ByVal nCombineMode As Long) As Long
Public Declare Function EqualRgn Lib "gdi32" (ByVal hSrcRgn1 As Long, ByVal hSrcRgn2 As Long) As Long
'Public Declare Function ExtCreateRegion Lib "gdi32" (lpXform As xform, ByVal nCount As Long, lpRgnData As RgnData) As Long
Public Declare Function FillRgn Lib "gdi32" (ByVal hdc As Long, ByVal hRgn As Long, ByVal hBrush As Long) As Long
Public Declare Function FrameRgn Lib "gdi32" (ByVal hdc As Long, ByVal hRgn As Long, ByVal hBrush As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Public Declare Function GetPolyFillMode Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function GetRgnBox Lib "gdi32" (ByVal hRgn As Long, lpRect As Rect) As Long
Public Declare Function GetRegionData Lib "gdi32" Alias "GetRegionDataA" (ByVal hRgn As Long, ByVal dwCount As Long, lpRgnData As RGNDATA) As Long
Public Declare Function InvertRgn Lib "gdi32" (ByVal hdc As Long, ByVal hRgn As Long) As Long
Public Declare Function OffsetRgn Lib "gdi32" (ByVal hRgn As Long, ByVal x As Long, ByVal y As Long) As Long
Public Declare Function PaintRgn Lib "gdi32" (ByVal hdc As Long, ByVal hRgn As Long) As Long
Public Declare Function PtInRegion Lib "gdi32" (ByVal hRgn As Long, ByVal x As Long, ByVal y As Long) As Long
Public Declare Function RectInRegion Lib "gdi32" (ByVal hRgn As Long, lpRect As Rect) As Long
Public Declare Function SetPolyFillMode Lib "gdi32" (ByVal hdc As Long, ByVal nPolyFillMode As Long) As Long
Public Declare Function SetRectRgn Lib "gdi32" (ByVal hRgn As Long, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
Public Type RGNDATAHEADER
        dwSize As Long
        iType As Long
        nCount As Long
        nRgnSize As Long
        rcBound As Rect
End Type
Public Type RGNDATA
        rdh As RGNDATAHEADER
        Buffer As Byte
End Type
Public Type POINTAPI
        x As Long
        y As Long
End Type
Public Const RGN_AND = 1
Public Const RGN_COPY = 5
Public Const RGN_DIFF = 4
Public Const RGN_MAX = RGN_COPY
Public Const RGN_MIN = RGN_AND
Public Const RGN_OR = 2
Public Const RGN_XOR = 3



Public pbcolMonthlyData As New Collection
Public pbasMonth(11) As String
Public pbasngDataOperating(11, 2) As Single
Public pbasDataType(3) As String
Public pbasLVKeyName(3) As String
Public pbsDataFileName As String
Public pbsDataUnit As String
Public pblSeries As Long, pblMonth As Long
Public pblCharType As Long

Public pbavFinalData() As Variant
Public pbasLegend() As String

Public pbfEditMode As Boolean
Public pbfInit As Boolean
Public pbfIs3D As Boolean

Public Const pbcntAppName As String = "Diagram"
Public Const pbcntSectionName As String = "SavedPath"
Public Const pbcntKeyName As String = "DataFileName"
'Public Const pbcntKeyName1 As String = "Unit"
Public Const pbcntDefaultDataFileName As String = "data.dia"
Public Const pbcntDefaultDataUnit As String = "单位"

'********************************************************************
'*过程名: SavePath()
'*功能:   保存数据文件路径到注册表
'*参数:   无
'*返回值: 无
'********************************************************************

Sub SavePath()
    SaveSetting pbcntAppName, pbcntSectionName, pbcntKeyName, pbsDataFileName
'    SaveSetting pbcntAppName, pbcntSectionName, pbcntKeyName1, pbsDataUnit
End Sub

'********************************************************************
'*过程名: LoadPath()
'*功能:   从注册表读取数据文件路径
'*参数:   无
'*返回值: 无
'********************************************************************
Sub LoadPath()
    Dim s As String
    s = Trim(GetSetting(pbcntAppName, pbcntSectionName, pbcntKeyName, pbcntDefaultDataFileName))
    If InStr(s, "\") = 0 Then
        s = App.Path + "\" + s
    End If
    frmMain.txtFilepath.Text = s
    
'    pbsDataUnit = Trim(GetSetting(pbcntAppName, pbcntSectionName, pbcntKeyName1, pbcntDefaultDataUnit))
'    frmData.txtUnit.Text = pbsDataUnit
End Sub

'********************************************************************
'*函数名: SaveData
'*功能:   把数据保存到文件
'*参数:   sFilename =>数据文件名
'*        colData =>存放数据的集合
'*返回值: True  => 成功
'*        False => 失败
'********************************************************************
Function SaveData(sFilename As String, colData As Collection) As Boolean
On Error GoTo staErr
Dim lFreeFile As Long, sTmpLn As String, asTmp As Variant, alTmp(2) As Single, lp As Long, lMonthCount As Byte
Dim lChoice As Long
'If FileExist(sFilename) Then
'    lChoice = MsgBox("此文件名已存在，要覆盖吗？", vbExclamation + vbDefaultButton2 + vbYesNo, "Question")
'    If lChoice = vbNo Then
'        MsgBox "那么请换个文件名再保存", vbOKOnly + vbInformation
'        GoTo staErr
'    End If
'End If

lFreeFile = FreeFile()
Dim i As Long, j As Long
Open sFilename For Output As #lFreeFile
sTmpLn = "UNIT" + "," + pbsDataUnit
Print #lFreeFile, sTmpLn
For i = 0 To 11
    sTmpLn = Trim(pbasMonth(i))
    For j = 0 To 2
        sTmpLn = sTmpLn + "," + Trim(CStr(colData(pbasMonth(i))(j)))
    Next
    Print #lFreeFile, sTmpLn
Next
Close #lFreeFile

SaveData = True
Exit Function
staErr:
Close #lFreeFile
SaveData = False
End Function

'********************************************************************
'*函数名: LoadData
'*功能:   从数据文件中读入数据
'*参数:   sFilename =>数据文件名
'*        colData =>存放数据的集合
'*返回值: True  => 成功
'*        False => 失败
'********************************************************************
Function LoadData(sFilename As String, colData As Collection) As Boolean
On Error GoTo staErr
Dim lFreeFile As Long, sTmpLn As String, asTmp As Variant, alTmp(2) As Single, lp As Long, lMonthCount As Byte
Dim lChoice As Long


If Not FileExist(sFilename) Then
    MsgBox "请检查一下路径或文件名是否正确。", vbExclamation, "文件不存在!"
    GoTo staErr
End If

If pbfEditMode = True Then
    lChoice = MsgBox("列表中已有数据，重新导入后，原表中的数据将被覆盖。" + vbCrLf + vbCrLf + _
                "你确定要重新导入吗？", vbExclamation + vbDefaultButton2 + vbYesNo, "Question")
    If lChoice = vbNo Then GoTo staErr
End If

lFreeFile = FreeFile()
lMonthCount = 0
Set colData = New Collection
Open sFilename For Input As #lFreeFile
Line Input #lFreeFile, sTmpLn
sTmpLn = Trim(sTmpLn)
asTmp = Split(sTmpLn, ",")
If UCase(asTmp(0)) = "UNIT" Then
    pbsDataUnit = Trim(asTmp(1))
    frmData.txtUnit = pbsDataUnit
Else
    GoTo staErr
End If
Do
    lMonthCount = lMonthCount + 1
    If lMonthCount > 12 Then Exit Do
    Line Input #lFreeFile, sTmpLn
    sTmpLn = Trim(sTmpLn)
    asTmp = Split(sTmpLn, ",")
    For lp = 0 To 2
        alTmp(lp) = CSng(asTmp(lp + 1))
    Next
    'pbasMonth(lMonthCount - 1) = asTmp(0)
    colData.Add alTmp(), asTmp(0)
    
Loop While Not EOF(lFreeFile)

Close #lFreeFile

LoadData = True
Exit Function
staErr:
MsgBox "数据导入错误!", vbOKOnly + vbExclamation
Set colData = New Collection
Close #lFreeFile
LoadData = False
End Function

'********************************************************************
'*函数名: FileExist
'*功能:   判断文件是否存在
'*参数:   sPathname =>文件名
'*返回值: True  =>文件存在
'*        False =>文件不存在
'********************************************************************

Function FileExist(sPathname As String) As Boolean
On Error GoTo staErr
Dim lret As Long
lret = GetFileAttributes(Trim(sPathname))
If lret = Val(&HFFFFFFFF) Or lret = FILE_ATTRIBUTE_DIRECTORY Then GoTo staErr
FileExist = True
Exit Function
staErr:
FileExist = False
End Function

'********************************************************************
'*函数名: DelGap
'*功能:   删除所有空格
'*参数:   sIn => 待处理的字符串
'*返回值: 去掉空格后的字符串
'********************************************************************

Function DelGap(sIn As String) As String
On Error GoTo staErr
Dim s As String

Exit Function
staErr:
DelGap = ""
End Function

'********************************************************************
'*函数名: FullMonthName
'*功能:   转换月名
'*参数:   sShort => 英文月份名
'*返回值: 中文月份名
'********************************************************************
Function FullMonthName(sShort As String) As String
Dim s
Select Case UCase(sShort)
    Case "JAN"
        s = "一  月"
    Case "FEB"
        s = "二  月"
    Case "MAR"
        s = "三  月"
    Case "APR"
        s = "四  月"
    Case "MAY"
        s = "五  月"
    Case "JUN"
        s = "六  月"
    Case "JUL"
        s = "七  月"
    Case "AUG"
        s = "八  月"
    Case "SEP"
        s = "九  月"
    Case "OCT"
        s = "十  月"
    Case "NOV"
        s = "十一月"
    Case "DEC"
        s = "十二月"
    Case Else
        s = ""
End Select
FullMonthName = s
End Function

'********************************************************************
'*函数名: IsFigure
'*功能:   判断是否为数字
'*参数:   sFigure => 要判断的字符
'*返回值: True => 是数字
'         False => 不是数字
'********************************************************************
Function IsFigure(sFigure As String) As Boolean
On Error GoTo staErr
Dim l As Long
l = Asc(Left(sFigure, 1))
If Not l >= Asc("0") And l <= Asc("9") Then GoTo staErr
IsFigure = True
Exit Function
staErr:
IsFigure = False
End Function
