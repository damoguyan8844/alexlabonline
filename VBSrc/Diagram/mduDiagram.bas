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
Public Const pbcntDefaultDataUnit As String = "��λ"

'********************************************************************
'*������: SavePath()
'*����:   ���������ļ�·����ע���
'*����:   ��
'*����ֵ: ��
'********************************************************************

Sub SavePath()
    SaveSetting pbcntAppName, pbcntSectionName, pbcntKeyName, pbsDataFileName
'    SaveSetting pbcntAppName, pbcntSectionName, pbcntKeyName1, pbsDataUnit
End Sub

'********************************************************************
'*������: LoadPath()
'*����:   ��ע����ȡ�����ļ�·��
'*����:   ��
'*����ֵ: ��
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
'*������: SaveData
'*����:   �����ݱ��浽�ļ�
'*����:   sFilename =>�����ļ���
'*        colData =>������ݵļ���
'*����ֵ: True  => �ɹ�
'*        False => ʧ��
'********************************************************************
Function SaveData(sFilename As String, colData As Collection) As Boolean
On Error GoTo staErr
Dim lFreeFile As Long, sTmpLn As String, asTmp As Variant, alTmp(2) As Single, lp As Long, lMonthCount As Byte
Dim lChoice As Long
'If FileExist(sFilename) Then
'    lChoice = MsgBox("���ļ����Ѵ��ڣ�Ҫ������", vbExclamation + vbDefaultButton2 + vbYesNo, "Question")
'    If lChoice = vbNo Then
'        MsgBox "��ô�뻻���ļ����ٱ���", vbOKOnly + vbInformation
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
'*������: LoadData
'*����:   �������ļ��ж�������
'*����:   sFilename =>�����ļ���
'*        colData =>������ݵļ���
'*����ֵ: True  => �ɹ�
'*        False => ʧ��
'********************************************************************
Function LoadData(sFilename As String, colData As Collection) As Boolean
On Error GoTo staErr
Dim lFreeFile As Long, sTmpLn As String, asTmp As Variant, alTmp(2) As Single, lp As Long, lMonthCount As Byte
Dim lChoice As Long


If Not FileExist(sFilename) Then
    MsgBox "����һ��·�����ļ����Ƿ���ȷ��", vbExclamation, "�ļ�������!"
    GoTo staErr
End If

If pbfEditMode = True Then
    lChoice = MsgBox("�б����������ݣ����µ����ԭ���е����ݽ������ǡ�" + vbCrLf + vbCrLf + _
                "��ȷ��Ҫ���µ�����", vbExclamation + vbDefaultButton2 + vbYesNo, "Question")
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
MsgBox "���ݵ������!", vbOKOnly + vbExclamation
Set colData = New Collection
Close #lFreeFile
LoadData = False
End Function

'********************************************************************
'*������: FileExist
'*����:   �ж��ļ��Ƿ����
'*����:   sPathname =>�ļ���
'*����ֵ: True  =>�ļ�����
'*        False =>�ļ�������
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
'*������: DelGap
'*����:   ɾ�����пո�
'*����:   sIn => ��������ַ���
'*����ֵ: ȥ���ո����ַ���
'********************************************************************

Function DelGap(sIn As String) As String
On Error GoTo staErr
Dim s As String

Exit Function
staErr:
DelGap = ""
End Function

'********************************************************************
'*������: FullMonthName
'*����:   ת������
'*����:   sShort => Ӣ���·���
'*����ֵ: �����·���
'********************************************************************
Function FullMonthName(sShort As String) As String
Dim s
Select Case UCase(sShort)
    Case "JAN"
        s = "һ  ��"
    Case "FEB"
        s = "��  ��"
    Case "MAR"
        s = "��  ��"
    Case "APR"
        s = "��  ��"
    Case "MAY"
        s = "��  ��"
    Case "JUN"
        s = "��  ��"
    Case "JUL"
        s = "��  ��"
    Case "AUG"
        s = "��  ��"
    Case "SEP"
        s = "��  ��"
    Case "OCT"
        s = "ʮ  ��"
    Case "NOV"
        s = "ʮһ��"
    Case "DEC"
        s = "ʮ����"
    Case Else
        s = ""
End Select
FullMonthName = s
End Function

'********************************************************************
'*������: IsFigure
'*����:   �ж��Ƿ�Ϊ����
'*����:   sFigure => Ҫ�жϵ��ַ�
'*����ֵ: True => ������
'         False => ��������
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
