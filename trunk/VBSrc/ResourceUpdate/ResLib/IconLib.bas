Attribute VB_Name = "mduIconLib"
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 陈缘
'
' ************************   模 块 名   :mduIconLib********************
'版 本 号: V1.0
'作    者: 陈缘
'生成日期: 2004年03月10日
'最近修改:
'功能描述:
'函数列表:
'修改历史:
'日    期: 2004年03月10日
'修改人员: 陈缘
'修改内容: 生成
'******************************************************************************/
'
Option Explicit
'本模块名称
Private Const THIS_MODULE_NAME As String = "mduIconLib"

Dim m_oIconPackage As CIconPackage

Public Function CreateIconListWithObject(oIconPackage As CIconPackage) As Boolean
    On Error GoTo Error_Handler
    Dim lFreefile As Long
    Set m_oIconPackage = oIconPackage
    
    lFreefile = FreeFile
    Open m_oIconPackage.Filename For Binary As #lFreefile
    
    Dim utIconDir As ICONDIR
    Dim utIconEntry() As ICONDIRENTRY
    
    Get #lFreefile, , utIconDir
    
    If utIconDir.idReserved <> 0 Then
        RaiseError 999999, "not a icon/cursor file", THIS_MODULE_NAME
    End If
    
    If utIconDir.idType <> 1 And utIconDir.idType <> 2 Then
        RaiseError 999999, "not a icon/cursor file", THIS_MODULE_NAME
    End If
    
    If utIconDir.idCount <= 0 Then
        RaiseError 999999, "not a icon/cursor file", THIS_MODULE_NAME
    End If
        
    Dim oIconEntry As CIconEntry, i As Long
    ReDim utIconEntry(utIconDir.idCount - 1) As ICONDIRENTRY
    
    Get #lFreefile, , utIconEntry
    
    For i = 0 To UBound(utIconEntry)
        Set oIconEntry = m_oIconPackage.IconEntries.Add()
        With oIconEntry
            .IconSize = utIconEntry(i).dwBytesInRes
            .RawDataOffset = utIconEntry(i).dwImageOffset
            .IconHeight = utIconEntry(i).bHeight
            .IconWidth = utIconEntry(i).bWidth
            .IconType = utIconDir.idType
        End With
    Next
    
    Close #lFreefile
    CreateIconListWithObject = True
    Exit Function
Error_Handler:
    '自定义错误处理
    Close #lFreefile
    
    '调用默认错误处理函数
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


Public Function SizeOfArray(b As Variant) As Long
    If IsArray(b) Then
        SizeOfArray = UBound(b) - LBound(b) + 1
    End If
End Function
