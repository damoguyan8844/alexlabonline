VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBAnyCall"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'------------------------------ ��ģ�� VBAnyCall.cls ------------------------------
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CVBAnyCall"

Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Private Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Private Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)

'-------- �������� --------
Private mlParameters() As Long '�����б�
Private mlCallAddress As Long '���õĺ�����ַ
Private mbCodeBuffer() As Byte '�������ֽ�
Private mlLastCodePosition As Long '���Ը���������ӵ��ֽ�
Private mbCodeBytes() As Byte '���ڴ洢�����ֽ�
Private mfStdCall As Boolean '�Ƿ�Ϊ__stdcall����Լ�� True=__stdcall False=__cdecl
Private mfThroughVTable As Boolean '�Ƿ�ʹ��VTable������ת

'******************************* ��¶�Ľӿ� *******************************
'�Ʒ�������ʹ��VTABLE�������е���ʱʹ��
'ע��:�������һ��Ҫ������ǰ���λ�ã�Ҳ�������������������Ե�ǰ��!
Public Function Invoke() As Long
End Function
'���û���ֽ��ַ�������
'sCodeStr :����ֽ��ַ���
'FuncParams():��������
Public Function CallCodeBytes(ByVal sCodeStr As String, ParamArray FuncParams()) As Long
   On Error Resume Next
   Dim i As Long
   ReDim mlParameters(0)
   ReDim mbCodeBuffer(0)
   mlCallAddress = 0
   
   mbCodeBytes = ByteCodeStrToBin(sCodeStr)
   
   i = UBound(mbCodeBytes)
   
   If Err.Number <> 0 Then
      Call RaiseErr("�����ֽ��ַ���ת������")
      Exit Function
   End If
   
   mlCallAddress = VarPtr(mbCodeBytes(0))
   
   If mlCallAddress = 0 Then
      Call RaiseErr("������ڵ��ַ����")
      Exit Function
   End If
   ReDim mlParameters(UBound(FuncParams) + 1)
   For i = 1 To UBound(mlParameters)
      mlParameters(i) = CLng(FuncParams(i - 1))
   Next i
   
   CallCodeBytes = ExecuteCode()
End Function


'����ַ���ô���
'lFuncAddress:������ַ
'FuncParams():��������
Public Function CallByAddress(ByVal lFuncAddress As Long, ParamArray FuncParams()) As Long
   Dim i As Long
   ReDim mlParameters(0)
   ReDim mbCodeBuffer(0)
   mlCallAddress = 0
   mlCallAddress = lFuncAddress
   If mlCallAddress = 0 Then
      Call RaiseErr("������ڵ��ַ����")
      Exit Function
   End If
   ReDim mlParameters(UBound(FuncParams) + 1)
   For i = 1 To UBound(mlParameters)
      mlParameters(i) = CLng(FuncParams(i - 1))
   Next i
   CallByAddress = ExecuteCode()
End Function

'����DLL����
'sDllName:Dll����
'sFuncName:����������
'FuncParams():��������
Public Function CallApiByName(ByVal sDllName As String, ByVal sFuncName As String, ParamArray FuncParams()) As Long
   Dim hLib As Long, i As Long
   ReDim mlParameters(0)
   ReDim mbCodeBuffer(0)
   mlCallAddress = 0
   hLib = LoadLibrary(ByVal sDllName)
   If hLib = 0 Then
      Call RaiseErr("Dll�ļ�δ�ҵ�")
      Exit Function
   End If
   mlCallAddress = GetProcAddress(hLib, ByVal sFuncName)
   If mlCallAddress = 0 Then
      Call RaiseErr("������ڵ��ַ����")
      FreeLibrary hLib
      Exit Function
   End If
   ReDim mlParameters(UBound(FuncParams) + 1)
   For i = 1 To UBound(mlParameters)
      mlParameters(i) = CLng(FuncParams(i - 1))
   Next i
   CallApiByName = ExecuteCode()
   FreeLibrary hLib
End Function

'����ֽ��ַ���ת��Ϊ�����ֽ�����
'sByteCode:����ֽ��ַ���
Public Function ByteCodeStrToBin(ByVal sByteCode As String) As Byte()
    Dim s() As String
    Dim b() As Byte
    Dim i As Long
    
    s = Split(Trim(sByteCode), " ")
    
    If UBound(s) >= 0 Then
        ReDim b(UBound(s))
    End If
    
    For i = 0 To UBound(s)
        b(i) = CByte("&h" & s(i))
    Next
    ByteCodeStrToBin = b
End Function

'�Ƿ�Ϊ__stdcall����Լ�� True=__stdcall False=__cdecl
Public Property Let IsStandardCall(fVal As Boolean)
    mfStdCall = fVal
End Property

Public Property Get IsStandardCall() As Boolean
    IsStandardCall = mfStdCall
End Property

'�Ƿ�ʹ��VTable������ת
Public Property Let ThroughVTable(fVal As Boolean)
    mfThroughVTable = fVal
End Property

Public Property Get ThroughVTable() As Boolean
    ThroughVTable = mfThroughVTable
End Property

'���غ���ָ��ֵ
Public Function FuncPtr(ByVal lAddr) As Long
    FuncPtr = lAddr
End Function

'����lAddress��ʼubound(b)���ֽڵ����ݵ��ֽ�����b(),�������ַ�����ʾ
Public Function ShowMemory(ByVal lAddress As Long, b() As Byte, Optional ByVal fPrint As Boolean = True) As String
    On Error Resume Next
    Dim lLen As Long
    lLen = UBound(b) - LBound(b) + 1
    If lLen <= 0 Or Err.Number <> 0 Then
        Exit Function
    End If
    
    CopyMemory b(0), ByVal lAddress, lLen
    
    Dim i As Long
    For i = 0 To lLen - 1
        If b(i) < 16 Then
            ShowMemory = ShowMemory & "0" & Hex(b(i))
        Else
            ShowMemory = ShowMemory & Hex(b(i))
        End If
        ShowMemory = ShowMemory & " "
    Next
    
    If fPrint Then
        Debug.Print ShowMemory
    End If
End Function

'����ڴ�lAddress����Longֵ
Public Function GetLngValue(ByVal lAddress As Long) As Long
    CopyMemory GetLngValue, ByVal lAddress, 4
End Function
'******************************* ��¶�Ľӿ� *******************************

'******************************** ˽�к��� ********************************
Private Function ExecuteCode() As Long
    '--------- ʹ��VTable ---------
    Dim lpVTable As Long
    Dim lpJmp As Long
    Dim lOldFunc As Long
    Dim lNewFunc As Long
    Dim lRtn As Long
    
    If mfThroughVTable Then
        lpVTable = GetLngValue(ObjPtr(Me))
        lpJmp = lpVTable + &H1C
        lOldFunc = GetLngValue(lpJmp)
        lNewFunc = PrepareCodeForVTable
        CopyMemory ByVal lpJmp, ByVal VarPtr(lNewFunc), 4 'paste in new address
        ExecuteCode = Me.Invoke
        CopyMemory ByVal lpJmp, ByVal VarPtr(lOldFunc), 4 'restore old function address
    Else
        lNewFunc = PrepareCodeForWndProc
        ExecuteCode = CallWindowProc(lNewFunc, 0, 0, 0, 0)
    End If
End Function

Private Function PrepareCodeForVTable() As Long
    Dim i As Long, lCodeStartPosition As Long
    ReDim mbCodeBuffer(18 + 32 + 6 * UBound(mlParameters))
    lCodeStartPosition = GetAlignedlCodeStartPosition(VarPtr(mbCodeBuffer(0)))
    mlLastCodePosition = lCodeStartPosition - VarPtr(mbCodeBuffer(0))
    For i = 0 To mlLastCodePosition - 1
        mbCodeBuffer(i) = &HCC
    Next
    
    '��Ϊ���ǵ�Invoke����û�в�����������Щ������Բ�Ҫ
    '    'AddByteToCode &H58 'pop eax'�����ص�ַ����eax
    '    'AddByteToCode &H59 'pop ecx' / ȥ��
    '    'AddByteToCode &H59 'pop ecx'|  ����
    '    'AddByteToCode &H59 'pop ecx'|  ��ѹ��
    '    'AddByteToCode &H59 'pop ecx' \ �Ĳ���
    '    'AddByteToCode &H50 'push eax'����ѹ�뷵�ص�ַ
    For i = UBound(mlParameters) To 1 Step -1
        AddByteToCode &H68 'push parameter(i)'ѹ�����ǵĲ���
        AddLongToCode mlParameters(i)
    Next
    AddCallToCode mlCallAddress
    
    'VB֮���Բ�����__cdecl����Լ���ĺ���������ΪVB�����Լ��ָ���ջ��
    If Not mfStdCall Then '�������Լ������__stdcall���Ǿ��Լ��ָ���ջ
        For i = 1 To UBound(mlParameters)
            AddByteToCode &H59 'pop ecx'�ָ���ջ
        Next
    End If
    
    AddByteToCode &H51 'push ecx
    
    AddByteToCode &H8B 'mov ecx,[esp+c] <-����1�����ĵ�ַָ��
    AddByteToCode &H4C
    AddByteToCode &H24
    AddByteToCode &HC
    
    AddByteToCode &H89 'mov [ecx],eax <- ����1=eax
    AddByteToCode &H1
    
    AddByteToCode &H59 'pop ecx
        
    AddByteToCode &H33 ' xor eax,eax
    AddByteToCode &HC0 '
    
    AddByteToCode &HC2
    AddByteToCode &H8
    AddByteToCode &H0
    PrepareCodeForVTable = lCodeStartPosition
End Function

Private Function PrepareCodeForWndProc() As Long
    Dim i As Long, lCodeStartPosition As Long
    ReDim mbCodeBuffer(18 + 32 + 6 * UBound(mlParameters))
    lCodeStartPosition = GetAlignedlCodeStartPosition(VarPtr(mbCodeBuffer(0)))
    mlLastCodePosition = lCodeStartPosition - VarPtr(mbCodeBuffer(0))
    For i = 0 To mlLastCodePosition - 1
        mbCodeBuffer(i) = &HCC
    Next
    AddByteToCode &H58 'pop eax'�����ص�ַ����eax
    AddByteToCode &H59 'pop ecx' / ȥ��
    AddByteToCode &H59 'pop ecx'|  ����
    AddByteToCode &H59 'pop ecx'|  ��ѹ��
    AddByteToCode &H59 'pop ecx' \ �Ĳ���
    AddByteToCode &H50 'push eax'����ѹ�뷵�ص�ַ
    For i = UBound(mlParameters) To 1 Step -1
        AddByteToCode &H68 'push parameter(i)'ѹ�����ǵĲ���
        AddLongToCode mlParameters(i)
    Next
    AddCallToCode mlCallAddress
    
    'VB֮���Բ�����__cdecl����Լ���ĺ���������ΪVB�����Լ��ָ���ջ��
    If Not mfStdCall Then '�������Լ������__stdcall���Ǿ��Լ��ָ���ջ
        For i = 1 To UBound(mlParameters)
            AddByteToCode &H59 'pop ecx'�ָ���ջ
        Next
    End If

    AddByteToCode &HC3
    AddByteToCode &HCC
    PrepareCodeForWndProc = lCodeStartPosition
End Function

Private Sub AddCallToCode(lAddr As Long)
    AddByteToCode &HE8
    AddLongToCode lAddr - VarPtr(mbCodeBuffer(mlLastCodePosition)) - 4
End Sub

Private Sub AddLongToCode(lCode As Long)
    Dim i As Integer
    Dim b(3) As Byte
    CopyMemory b(0), lCode, 4
    For i = 0 To 3
        AddByteToCode b(i)
    Next
End Sub

Private Sub AddByteToCode(bCode As Byte)
    mbCodeBuffer(mlLastCodePosition) = bCode
    mlLastCodePosition = mlLastCodePosition + 1
End Sub

Private Function GetAlignedlCodeStartPosition(lAddr As Long) As Long
    GetAlignedlCodeStartPosition = lAddr + (15 - (lAddr - 1) Mod 16)
    If (15 - (lAddr - 1) Mod 16) = 0 Then GetAlignedlCodeStartPosition = GetAlignedlCodeStartPosition + 16
End Function

Private Sub RaiseErr(ByVal sErrMsg As String)
    Err.Raise vbObjectError + &H1321, "VBAnyCall", sErrMsg
End Sub
Private Sub Class_Initialize()
    mfStdCall = True
    mfThroughVTable = False
End Sub
'******************************** ˽�к��� ********************************

