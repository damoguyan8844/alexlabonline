Attribute VB_Name = "mduIEnumString"
'***********************************************************************************************************************
'
'ģ �� ��: mduIEnumString
'
'��    ��: Supergreenbean
'
'��������: �Զ���AutoComplete����
'
'��    ��: 2006��09��
'
'����ģ��: 1. CVBPEFnLocator.cls
'          2. CMyAutoCompleteSource.cls
'          3. mduAutoComplete.bas
'
'***********************************************************************************************************************
Option Explicit
'****************************************************************************************************************************************************************************************************************
'���к�ģ���е����к�����������ϵͳ������������һ���߳���
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

'������VB���߳��ж��������鷢����
Public Function DelegateFn(ByVal Method As MYMSG_METHOD, ByVal this As Long, ByVal param1 As Long, ByVal param2 As Long, ByVal param3 As Long) As Long
    With g_tParams
        .param1 = param1
        .param2 = param2
        .param3 = param3
    End With
    
    '������ʽ������SendMessage API���������߳�������ʲô����������
    DelegateFn = SendMessage(g_hTargetWnd, ACM_ACTION, Method, ByVal this)
End Function
