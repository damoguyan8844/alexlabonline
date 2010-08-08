VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CMyAutoCompleteSource"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'***********************************************************************************************************************
'
'模 块 名: CMyAutoCompleteSource
'
'作    者: Supergreenbean
'
'功能描述: 自定义AutoComplete对象
'
'日    期: 2006年09月
'
'关联模块: 1. CVBPEFnLocator.cls
'          2. mduAutoComplete.bas
'          3. mduIEnumString.bas
'
'***********************************************************************************************************************
Option Explicit

'实现IACList接口，用于分层次显示
Implements IACList
'实现IEnumString接口，用于基本的候选提示
Implements IEnumString

'存放候选字符串的数组
Private m_asElements() As String
'当前的字符串元素
Private m_lCurrentElement As Long
'候选字符串元素的总数
Private m_lElementCount As Long

'用于内存操作的函数
Private Function memcpy(ByVal lpDestAddr As Long, ByVal lpSrcAddr As Long, ByVal lLen As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
    Dim lBytesReadWrite As Long
    Dim hProcess As Long
    hProcess = OpenProcess(&H1F0FFF, 0, GetCurrentProcessId)
    If hProcess Then
        If VirtualProtectEx(hProcess, ByVal lpDestAddr, lLen, &H4, lOldProtect) <> 0 Then
            WriteProcessMemory hProcess, ByVal lpDestAddr, ByVal lpSrcAddr, lLen, lBytesReadWrite
            Call VirtualProtectEx(hProcess, ByVal lpDestAddr, lLen, lOldProtect, lOldProtect)
            memcpy = (lBytesReadWrite <> 0)
        End If
    End If
    CloseHandle hProcess
    Exit Function
Error_Handler:
End Function

Private Sub ReplaceVTableEntries(ByVal lpNext As Long, ByVal lpSkip As Long, ByVal lpReset As Long, ByVal lpClone As Long, ByVal lpACListExpand As Long)
    Dim lpVTable As Long
    Dim oIEnumString As IEnumString
    Dim oACList As IACList
    
    Set oIEnumString = Me
    
    CopyMemory lpVTable, ByVal ObjPtr(oIEnumString), 4
    
    memcpy lpVTable + &HC, VarPtr(lpNext), 4
    memcpy lpVTable + &H10, VarPtr(lpSkip), 4
    memcpy lpVTable + &H14, VarPtr(lpReset), 4
    memcpy lpVTable + &H18, VarPtr(lpClone), 4
    
    Set oACList = Me
    
    CopyMemory lpVTable, ByVal ObjPtr(oACList), 4
    
    memcpy lpVTable + &HC, VarPtr(lpACListExpand), 4
End Sub

'类初始化
Private Sub Class_Initialize()
    '初始化候选字符串数组
    Call InitElements
    
    '替换IEnumString对象的VTable入口为我们自己的处理函数
    ReplaceVTableEntries AddressOf RealIEnumString_Next, AddressOf RealIEnumString_Skip, AddressOf RealIEnumString_Reset, AddressOf RealIEnumString_Clone, AddressOf RealIACList_Expand
End Sub

'初始化候选字符串元素
Private Sub InitElements()
    Dim i As Long
    
    '这里一共定义10个候选串
    m_lElementCount = 10
    
    ReDim m_asElements(m_lElementCount - 1)
    
    m_asElements(0) = "\"
    For i = 1 To m_lElementCount - 3
        m_asElements(i) = "\Level 0 E" & CStr(i - 1)
    Next
    
    m_asElements(8) = "My Source"
    m_asElements(9) = "Hello World!"
End Sub

'***********************************************************************************************************************
'默认的接口方法
'将会被替代
'***********************************************************************************************************************
Private Sub IACList_Expand(ByVal pszExpand As Long)
    
End Sub

Private Sub IEnumString_Clone(ByVal ppenum As Long)

End Sub


Private Sub IEnumString_Next(ByVal celt As Long, ByVal rgelt As Long, ByVal pceltFetched As Long)

End Sub

Private Sub IEnumString_Reset()

End Sub

Private Sub IEnumString_Skip(ByVal celt As Long)

End Sub
'***********************************************************************************************************************


'***********************************************************************************************************************
'真正处理数据的接口方法
'***********************************************************************************************************************
Public Function DoIACList_Expand(ByVal pszExpand As Long) As Long
    Dim s As String
    s = Space$(lStrLen(pszExpand))
    CopyMemory ByVal StrPtr(s), ByVal pszExpand, LenB(s)

    Dim sTmp As String
    Dim lLevel As Long
    sTmp = UCase(s)

    If InStr(sTmp, "LEVEL ") <> 0 Then
        sTmp = Mid$(sTmp, InStrRev(sTmp, "LEVEL ") + 6)
        sTmp = Left$(sTmp, InStr(sTmp, " E") - 1)
        lLevel = Val(sTmp) + 1

        sTmp = Right$(Trim(s), 2)
        If sTmp <> "\\" Then
            m_asElements(0) = s
            m_asElements(1) = s & "Level " & CStr(lLevel) & " E0"
            m_asElements(2) = s & "Level " & CStr(lLevel) & " E1"
            m_asElements(3) = s & "Level " & CStr(lLevel) & " E2"
            m_asElements(4) = s & "Level " & CStr(lLevel) & " E3"
            m_asElements(5) = s & "Level " & CStr(lLevel) & " E4"
        End If
    End If
        
    DoIACList_Expand = S_OK
    
    Debug.Print "Expand", pszExpand, sTmp
End Function

Public Function DoIEnumString_Next(ByVal celt As Long, ByVal rgelt As Long, ByVal pceltFetched As Long) As Long
    If rgelt = 0 Then
        DoIEnumString_Next = E_POINTER
        Exit Function
    End If
    
    Dim i As Long, celtFetched As Long
    Dim lpStr As Long
    
    celtFetched = 0
    For i = 0 To celt - 1
        If m_lCurrentElement = m_lElementCount Then
            Exit For
        End If
            
        lpStr = CoTaskMemAlloc((Len(m_asElements(m_lCurrentElement)) + 1) * 2)
        CopyMemory ByVal lpStr, ByVal StrPtr(m_asElements(m_lCurrentElement)), (Len(m_asElements(m_lCurrentElement)) + 1) * 2
        CopyMemory ByVal rgelt + i * 4, lpStr, 4
        
        m_lCurrentElement = m_lCurrentElement + 1
        celtFetched = celtFetched + 1
    Next
    
    If pceltFetched <> 0 Then
        CopyMemory ByVal pceltFetched, celtFetched, 4
    End If
    
    If i = celt Then
        DoIEnumString_Next = S_OK
    Else
        DoIEnumString_Next = S_FALSE
    End If
    
    Debug.Print "Next", celt, rgelt, pceltFetched
End Function


Public Function DoIEnumString_Skip(ByVal celt As Long) As Long
    If m_lCurrentElement + celt <= m_lElementCount Then
        m_lCurrentElement = m_lCurrentElement + celt
        DoIEnumString_Skip = S_OK
    Else
        m_lCurrentElement = m_lElementCount
        DoIEnumString_Skip = S_FALSE
    End If
    
    Debug.Print "Skip", celt
End Function

Public Function DoIEnumString_Reset() As Long
    m_lCurrentElement = 0
    DoIEnumString_Reset = S_OK
    
    Debug.Print "Reset"
End Function

Public Function DoIEnumString_Clone(ByVal ppenum As Long) As Long
    DoIEnumString_Clone = E_NOTIMPL
    
    Debug.Print "Clone"
End Function
'***********************************************************************************************************************

