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
'------------------------------ 类模块 VBAnyCall.cls ------------------------------
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "CVBAnyCall"

Private Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Private Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Private Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hwnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Private Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)

'-------- 变量声明 --------
Private mlParameters() As Long '参数列表
Private mlCallAddress As Long '调用的函数地址
Private mbCodeBuffer() As Byte '汇编代码字节
Private mlLastCodePosition As Long '用以跟踪最后添加的字节
Private mbCodeBytes() As Byte '用于存储代码字节
Private mfStdCall As Boolean '是否为__stdcall调用约定 True=__stdcall False=__cdecl
Private mfThroughVTable As Boolean '是否使用VTable进行跳转

'******************************* 暴露的接口 *******************************
'哑方法，在使用VTABLE方法进行调用时使用
'注意:这个方法一定要放在最前面的位置，也就是所有其他方法属性的前面!
Public Function Invoke() As Long
End Function
'调用汇编字节字符串方法
'sCodeStr :汇编字节字符串
'FuncParams():参数数组
Public Function CallCodeBytes(ByVal sCodeStr As String, ParamArray FuncParams()) As Long
   On Error Resume Next
   Dim i As Long
   ReDim mlParameters(0)
   ReDim mbCodeBuffer(0)
   mlCallAddress = 0
   
   mbCodeBytes = ByteCodeStrToBin(sCodeStr)
   
   i = UBound(mbCodeBytes)
   
   If Err.Number <> 0 Then
      Call RaiseErr("代码字节字符串转换错误")
      Exit Function
   End If
   
   mlCallAddress = VarPtr(mbCodeBytes(0))
   
   If mlCallAddress = 0 Then
      Call RaiseErr("代码入口点地址错误")
      Exit Function
   End If
   ReDim mlParameters(UBound(FuncParams) + 1)
   For i = 1 To UBound(mlParameters)
      mlParameters(i) = CLng(FuncParams(i - 1))
   Next i
   
   CallCodeBytes = ExecuteCode()
End Function


'按地址调用代码
'lFuncAddress:函数地址
'FuncParams():参数数组
Public Function CallByAddress(ByVal lFuncAddress As Long, ParamArray FuncParams()) As Long
   Dim i As Long
   ReDim mlParameters(0)
   ReDim mbCodeBuffer(0)
   mlCallAddress = 0
   mlCallAddress = lFuncAddress
   If mlCallAddress = 0 Then
      Call RaiseErr("代码入口点地址错误")
      Exit Function
   End If
   ReDim mlParameters(UBound(FuncParams) + 1)
   For i = 1 To UBound(mlParameters)
      mlParameters(i) = CLng(FuncParams(i - 1))
   Next i
   CallByAddress = ExecuteCode()
End Function

'调用DLL函数
'sDllName:Dll名称
'sFuncName:函数的名称
'FuncParams():参数数组
Public Function CallApiByName(ByVal sDllName As String, ByVal sFuncName As String, ParamArray FuncParams()) As Long
   Dim hLib As Long, i As Long
   ReDim mlParameters(0)
   ReDim mbCodeBuffer(0)
   mlCallAddress = 0
   hLib = LoadLibrary(ByVal sDllName)
   If hLib = 0 Then
      Call RaiseErr("Dll文件未找到")
      Exit Function
   End If
   mlCallAddress = GetProcAddress(hLib, ByVal sFuncName)
   If mlCallAddress = 0 Then
      Call RaiseErr("代码入口点地址错误")
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

'汇编字节字符串转换为代码字节数组
'sByteCode:汇编字节字符串
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

'是否为__stdcall调用约定 True=__stdcall False=__cdecl
Public Property Let IsStandardCall(fVal As Boolean)
    mfStdCall = fVal
End Property

Public Property Get IsStandardCall() As Boolean
    IsStandardCall = mfStdCall
End Property

'是否使用VTable进行跳转
Public Property Let ThroughVTable(fVal As Boolean)
    mfThroughVTable = fVal
End Property

Public Property Get ThroughVTable() As Boolean
    ThroughVTable = mfThroughVTable
End Property

'返回函数指针值
Public Function FuncPtr(ByVal lAddr) As Long
    FuncPtr = lAddr
End Function

'复制lAddress开始ubound(b)个字节的内容到字节数组b(),并返回字符串表示
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

'获得内存lAddress处的Long值
Public Function GetLngValue(ByVal lAddress As Long) As Long
    CopyMemory GetLngValue, ByVal lAddress, 4
End Function
'******************************* 暴露的接口 *******************************

'******************************** 私有函数 ********************************
Private Function ExecuteCode() As Long
    '--------- 使用VTable ---------
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
    
    '因为我们的Invoke方法没有参数，所以这些步骤可以不要
    '    'AddByteToCode &H58 'pop eax'将返回地址存入eax
    '    'AddByteToCode &H59 'pop ecx' / 去掉
    '    'AddByteToCode &H59 'pop ecx'|  事先
    '    'AddByteToCode &H59 'pop ecx'|  被压入
    '    'AddByteToCode &H59 'pop ecx' \ 的参数
    '    'AddByteToCode &H50 'push eax'重新压入返回地址
    For i = UBound(mlParameters) To 1 Step -1
        AddByteToCode &H68 'push parameter(i)'压入我们的参数
        AddLongToCode mlParameters(i)
    Next
    AddCallToCode mlCallAddress
    
    'VB之所以不能用__cdecl调用约定的函数就是因为VB不会自己恢复堆栈。
    If Not mfStdCall Then '如果调用约定不是__stdcall，那就自己恢复堆栈
        For i = 1 To UBound(mlParameters)
            AddByteToCode &H59 'pop ecx'恢复堆栈
        Next
    End If
    
    AddByteToCode &H51 'push ecx
    
    AddByteToCode &H8B 'mov ecx,[esp+c] <-参数1函数的地址指针
    AddByteToCode &H4C
    AddByteToCode &H24
    AddByteToCode &HC
    
    AddByteToCode &H89 'mov [ecx],eax <- 参数1=eax
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
    AddByteToCode &H58 'pop eax'将返回地址存入eax
    AddByteToCode &H59 'pop ecx' / 去掉
    AddByteToCode &H59 'pop ecx'|  事先
    AddByteToCode &H59 'pop ecx'|  被压入
    AddByteToCode &H59 'pop ecx' \ 的参数
    AddByteToCode &H50 'push eax'重新压入返回地址
    For i = UBound(mlParameters) To 1 Step -1
        AddByteToCode &H68 'push parameter(i)'压入我们的参数
        AddLongToCode mlParameters(i)
    Next
    AddCallToCode mlCallAddress
    
    'VB之所以不能用__cdecl调用约定的函数就是因为VB不会自己恢复堆栈。
    If Not mfStdCall Then '如果调用约定不是__stdcall，那就自己恢复堆栈
        For i = 1 To UBound(mlParameters)
            AddByteToCode &H59 'pop ecx'恢复堆栈
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
'******************************** 私有函数 ********************************


