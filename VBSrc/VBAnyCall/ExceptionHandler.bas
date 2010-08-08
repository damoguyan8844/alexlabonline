Attribute VB_Name = "mduExceptionHandler"
Option Explicit

Private Declare Function SetUnhandledExceptionFilter Lib "kernel32" (ByVal lpTopLevelExceptionFilter As Long) As Long

Private Const EXCEPTION_MAXIMUM_PARAMETERS = 15&

Private Const SIZE_OF_80387_REGISTERS = 80

Private Type FLOATING_SAVE_AREA
     ControlWord As Long
     StatusWord As Long
     TagWord As Long
     ErrorOffset As Long
     ErrorSelector As Long
     DataOffset As Long
     DataSelector As Long
     RegisterArea(SIZE_OF_80387_REGISTERS) As Byte
     Cr0NpxState As Long
End Type

Private Type CONTEXT86
    ContextFlags As Long
'These are selected by CONTEXT_DEBUG_REGISTERS
    Dr0 As Long
    Dr1 As Long
    Dr2 As Long
    Dr3 As Long
    Dr6 As Long
    Dr7 As Long
'These are selected by CONTEXT_FLOATING_POINT
    FloatSave As FLOATING_SAVE_AREA
'These are selected by CONTEXT_SEGMENTS
    SegGs As Long
    SegFs As Long
    SegEs As Long
    SegDs As Long
'These are selected by CONTEXT_INTEGER
    Edi As Long
    Esi As Long
    Ebx As Long
    Edx As Long
    Ecx As Long
    Eax As Long
'These are selected by CONTEXT_CONTROL
    Ebp As Long
    Eip As Long
    SegCs As Long
    EFlags As Long
    Esp As Long
    SegSs As Long
End Type

Private Const CONTEXT_X86 = &H10000
Private Const CONTEXT86_CONTROL = (CONTEXT_X86 Or &H1)  'SS:SP, CS:IP, FLAGS, BP
Private Const CONTEXT86_INTEGER = (CONTEXT_X86 Or &H2)  'AX, BX, CX, DX, SI, DI
Private Const CONTEXT86_SEGMENTS = (CONTEXT_X86 Or &H4)  'DS, ES, FS, GS
Private Const CONTEXT86_FLOATING_POINT = (CONTEXT_X86 Or &H8)  '387 state
Private Const CONTEXT86_DEBUG_REGISTERS = (CONTEXT_X86 Or &H10)  'DB 0-3,6,7
Private Const CONTEXT86_FULL = (CONTEXT86_CONTROL Or CONTEXT86_INTEGER Or CONTEXT86_SEGMENTS)


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

'#define EXCEPTION_EXECUTE_HANDLER       1
'#define EXCEPTION_CONTINUE_SEARCH       0
'#define EXCEPTION_CONTINUE_EXECUTION    -1

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
Debug.Print sErrMsg
    Err.Raise vbObjectError + &H123, "异常过滤函数", sErrMsg
End Function

Public Function TranslateExceptionCode(ByVal lExceptionCode As Long) As String
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


 
