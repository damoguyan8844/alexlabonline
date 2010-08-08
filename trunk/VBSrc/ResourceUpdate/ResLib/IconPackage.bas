Attribute VB_Name = "mduIconPackage"
'/**************************************************************************
'                 版权所有 (c), 2000 - 2010, 陈缘
'
' ************************   模 块 名   :mduIconPackage********************
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
Private Const THIS_MODULE_NAME As String = "mduIconPackage"

Public Type ICONDIR
    idReserved As Integer
    idType As Integer
    idCount As Integer
End Type

Public Type ICONDIRENTRY
    bWidth As Byte
    bHeight As Byte
    bColorCount As Byte
    bReserved As Byte
    wPlanes As Integer
    wBitCount As Integer
    dwBytesInRes As Long
    dwImageOffset As Long
End Type





