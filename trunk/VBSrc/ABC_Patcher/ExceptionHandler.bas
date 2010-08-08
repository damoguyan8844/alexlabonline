Attribute VB_Name = "mduExceptionHandler"
Option Explicit

Private Declare Function SetUnhandledExceptionFilter Lib "kernel32" (ByVal lpTopLevelExceptionFilter As Long) As Long

Private Const EXCEPTION_MAXIMUM_PARAMETERS = 15&

Private Type CONTEXT
    FltF0 As Double
    FltF1 As Double
    FltF2 As Double
    FltF3 As Double
    FltF4 As Double
    FltF5 As Double
    FltF6 As Double
    FltF7 As Double
    FltF8 As Double
    FltF9 As Double
    FltF10 As Double
    FltF11 As Double
    FltF12 As Double
    FltF13 As Double
    FltF14 As Double
    FltF15 As Double
    FltF16 As Double
    FltF17 As Double
    FltF18 As Double
    FltF19 As Double
    FltF20 As Double
    FltF21 As Double
    FltF22 As Double
    FltF23 As Double
    FltF24 As Double
    FltF25 As Double
    FltF26 As Double
    FltF27 As Double
    FltF28 As Double
    FltF29 As Double
    FltF30 As Double
    FltF31 As Double
    IntV0 As Double
    IntT0 As Double
    IntT1 As Double
    IntT2 As Double
    IntT3 As Double
    IntT4 As Double
    IntT5 As Double
    IntT6 As Double
    IntT7 As Double
    IntS0 As Double
    IntS1 As Double
    IntS2 As Double
    IntS3 As Double
    IntS4 As Double
    IntS5 As Double
    IntFp As Double
    IntA0 As Double
    IntA1 As Double
    IntA2 As Double
    IntA3 As Double
    IntA4 As Double
    IntA5 As Double
    IntT8 As Double
    IntT9 As Double
    IntT10 As Double
    IntT11 As Double
    IntRa As Double
    IntT12 As Double
    IntAt As Double
    IntGp As Double
    IntSp As Double
    IntZero As Double
    Fpcr As Double
    SoftFpcr As Double
    Fir As Double
    Psr As Long
    ContextFlags As Long
    Fill(4) As Long
End Type

Private Type EXCEPTION_RECORD
    ExceptionCode As Long
    ExceptionFlags As Long
    pExceptionRecord As Long
    ExceptionAddress As Long
    NumberParameters As Long
    ExceptionInformation(EXCEPTION_MAXIMUM_PARAMETERS - 1) As Long
End Type

Private Type EXCEPTION_POINTERS
    pExceptionRecord As Long
    ContextRecord As Long
End Type

Private Declare Function FormatMessage Lib "kernel32" Alias "FormatMessageA" (ByVal dwFlags As Long, lpSource As Any, ByVal dwMessageId As Long, ByVal dwLanguageId As Long, ByVal lpBuffer As String, ByVal nSize As Long, Arguments As Long) As Long

Private Const FORMAT_MESSAGE_ALLOCATE_BUFFER = &H100
Private Const FORMAT_MESSAGE_ARGUMENT_ARRAY = &H2000
Private Const FORMAT_MESSAGE_FROM_HMODULE = &H800
Private Const FORMAT_MESSAGE_FROM_STRING = &H400
Private Const FORMAT_MESSAGE_FROM_SYSTEM = &H1000
Private Const FORMAT_MESSAGE_IGNORE_INSERTS = &H200
Private Const FORMAT_MESSAGE_MAX_WIDTH_MASK = &HFF

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Public Function InitExceptionHandler() As Long
    InitExceptionHandler = SetUnhandledExceptionFilter(AddressOf MyExceptionFilter)
End Function

Public Function MyExceptionFilter(lpExceptionPointers As EXCEPTION_POINTERS) As Long
    Dim i As Long
    Dim utExceptionRecord As EXCEPTION_RECORD
    Dim sErrMsg As String
    MyExceptionFilter = 0&
    CopyMemory ByVal VarPtr(utExceptionRecord), ByVal lpExceptionPointers.pExceptionRecord, Len(utExceptionRecord)
    Do
        i = i + 1
        If i > 100 Then Exit Do
        With utExceptionRecord
            sErrMsg = TranslateExceptionCode(.ExceptionCode)
            
            If sErrMsg = TranslateExceptionCode(&HC0000005) Then
                sErrMsg = sErrMsg & " - 位于 &H" & Hex(.ExceptionAddress) & " 的代码试图向地址 &H" & _
                            Hex(.ExceptionInformation(1)) & " " & _
                            IIf(.ExceptionInformation(0) = 0, "读取", "写入") & "数据"
            End If
            
            If .pExceptionRecord = 0 Then Exit Do
            
            CopyMemory ByVal VarPtr(utExceptionRecord), ByVal .pExceptionRecord, Len(utExceptionRecord)
            
        End With
        sErrMsg = sErrMsg & vbCrLf
    Loop

    Err.Raise vbObjectError + &H123, "异常过滤函数", sErrMsg
End Function

Private Function TranslateExceptionCode(ByVal lExceptionCode As Long) As String
    Select Case lExceptionCode
    Case &HC0000005
        TranslateExceptionCode = "EXCEPTION_ACCESS_VIOLATION"
    Case &HC000008C
        TranslateExceptionCode = "EXCEPTION_ARRAY_BOUNDS_EXCEEDEDEX"
    Case &H80000003
        TranslateExceptionCode = "EXCEPTION_BREAKPOINT"
    Case &H80000002
        TranslateExceptionCode = "EXCEPTION_DATATYPE_MISALIGNMENT"
    Case &HC000008D
        TranslateExceptionCode = "EXCEPTION_FLOAT_DENORMAL_OPERANDEXCE"
    Case &HC000008E
        TranslateExceptionCode = "EXCEPTION_FLOAT_DIVIDE_BY_ZERO"
    Case &HC000008F
        TranslateExceptionCode = "EXCEPTION_FLOAT_INEXACT_RESULT"
    Case &HC0000090
        TranslateExceptionCode = "EXCEPTION_INVALID_OPERATION"
    Case &HC0000091
        TranslateExceptionCode = "EXCEPTION_FLOAT_OVERFLOW"
    Case &HC0000092
        TranslateExceptionCode = "EXCEPTION_FLOAT_STACK_CHECK"
    Case &HC0000093
        TranslateExceptionCode = "EXCEPTION_FLOAT_UNDERFLOW"
    Case &H80000001
        TranslateExceptionCode = "EXCEPTION_GUARD_PAGE"
    Case &HC000001D
        TranslateExceptionCode = "EXCEPTION_ILLEGAL_INSTRUCTION"
    Case &HC0000006
        TranslateExceptionCode = "EXCEPTION_IN_PAGE_ERROR"
    Case &HC0000094
        TranslateExceptionCode = "EXCEPTION_INT_DIVIDE_BY_ZERO"
    Case &HC0000095
        TranslateExceptionCode = "EXCEPTION_INT_OVERFLOW"
    Case &HC0000026
        TranslateExceptionCode = "EXCEPTION_INVALID_DISPOSITION"
    Case &HC0000008
        TranslateExceptionCode = "EXCEPTION_INVALID_HANDLE"
    Case &HC0000025
        TranslateExceptionCode = "EXCEPTION_NONCONTINUABLE_EXCEPTION"
    Case &HC0000096
        TranslateExceptionCode = "EXCEPTION_PRIV_INSTRUCTION"
    Case &HC0000004
        TranslateExceptionCode = "EXCEPTION_SINGLE_STEP"
    Case &HC00000FD
        TranslateExceptionCode = "EXCEPTION_STACK_OVERFLOW"
    Case Else
        TranslateExceptionCode = "EXCEPTION_UNKOWN_CODE"
    End Select
End Function

Public Function GetLastDllErrDescription(Optional vErrNumber As Variant = Empty) As String
    Dim lErrNumber As Long
    
    lErrNumber = Err.LastDllError
    
    If Not IsEmpty(vErrNumber) Then
        If IsNumeric(vErrNumber) Then
            lErrNumber = CLng(vErrNumber)
        End If
    End If
    
    Dim sDesc As String * 512, lLen As Long
    lLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM + FORMAT_MESSAGE_IGNORE_INSERTS, ByVal 0&, lErrNumber, 0&, sDesc, Len(sDesc), ByVal 0&)
    GetLastDllErrDescription = Left$(sDesc, lLen)
End Function


Public Function ShowMemory(ByVal lAddress As Long, ByVal lLen As Long, Optional ByVal fPrint As Boolean = True) As String
    Dim b() As Byte
    ReDim b(lLen - 1)
    CopyMemory ByVal VarPtr(b(0)), ByVal lAddress, lLen
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

Function sb()
    Dim f As Double
    Dim b(7) As Byte
    Dim i As Long
    f = 1.1
    CopyMemory b(0), ByVal VarPtr(f), 8
    
    Dim s As String
    For i = 7 To 0 Step -1
        s = s & Right$("00" & Hex(b(i)), 2)
    Next
    Debug.Print s
End Function

Function StrCopy(sDest As String, sSrc As String) As Boolean
    On Error GoTo Error_Handler
    sDest = String(Len(sSrc), " ")
    CopyMemory ByVal StrPtr(sDest), ByVal StrPtr(sSrc), LenB(sSrc)
    Exit Function
Error_Handler:
    '自定义错误处理

    '调用默认错误处理函数
    'Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function
