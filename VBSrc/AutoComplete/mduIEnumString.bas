Attribute VB_Name = "mduIEnumString"
'***********************************************************************************************************************
'
'模 块 名: mduIEnumString
'
'作    者: Supergreenbean
'
'功能描述: 自定义AutoComplete对象
'
'日    期: 2006年09月
'
'关联模块: 1. CVBPEFnLocator.cls
'          2. CMyAutoCompleteSource.cls
'          3. mduAutoComplete.bas
'
'***********************************************************************************************************************
Option Explicit
'****************************************************************************************************************************************************************************************************************
'运行后本模块中的所有函数将运行在系统所创建的另外一个线程中
'****************************************************************************************************************************************************************************************************************

Public Function RealIACList_Expand(ByVal this As Long, ByVal pszExpand As Long) As Long
    RealIACList_Expand = DelegateFn(MYMSG_IACList_Expand, this, pszExpand, 0, 0)
End Function

Public Function RealIEnumString_Next(ByVal this As Long, ByVal celt As Long, ByVal rgelt As Long, ByVal pceltFetched As Long) As Long
    RealIEnumString_Next = DelegateFn(MYMSG_IEnumString_Next, this, celt, rgelt, pceltFetched)
End Function

Public Function RealIEnumString_Skip(ByVal this As Long, ByVal celt As Long) As Long
    RealIEnumString_Skip = DelegateFn(MYMSG_IEnumString_Skip, this, celt, 0, 0)
End Function

Public Function RealIEnumString_Reset(ByVal this As Long) As Long
    RealIEnumString_Reset = DelegateFn(MYMSG_IEnumString_Reset, this, 0, 0, 0)
End Function

Public Function RealIEnumString_Clone(ByVal this As Long, ByVal ppenum As Long) As Long
    RealIEnumString_Clone = DelegateFn(MYMSG_IEnumString_Clone, this, ppenum, 0, 0)
End Function

'告诉在VB主线程中对象有事情发生了
Public Function DelegateFn(ByVal Method As MYMSG_METHOD, ByVal this As Long, ByVal param1 As Long, ByVal param2 As Long, ByVal param3 As Long) As Long
    With g_tParams
        .param1 = param1
        .param2 = param2
        .param3 = param3
    End With
    
    '用阻塞式工作的SendMessage API来告诉主线程现在是什么方法被调用
    DelegateFn = SendMessage(g_hTargetWnd, ACM_ACTION, Method, ByVal this)
End Function
