VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CGroupXML"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Dim m_dom As DOMDocument
Dim m_error As String

Public Function LoadXML(InputXML As String) As Boolean
On Error GoTo eh:

    Set m_dom = New DOMDocument
    m_dom.LoadXML InputXML
    
    If m_dom.parseError.ErrorCode <> 0 Then
        m_error = "Failed to parse the input xml. Error:" & CStr(m_dom.parseError.ErrorCode)
        LoadXML = False
        Exit Function
    End If
    
    LoadXML = True
eh:
    If Err.Number <> 0 Then
        m_error = Err.Description
        LoadXML = False
        Err.Clear
    End If
    
End Function

Public Function IsItemEnable(groupName As String, ID As Long, ByRef name As String) As Boolean
On Error GoTo eh:
    
    name = ""
    
    If m_dom Is Nothing Then
        IsItemEnable = False
        Exit Function
    End If
    
    Dim domGroup As MSXML2.IXMLDOMNode
    Set domGroup = m_dom.selectSingleNode("/Groups/Group[@Name='" & groupName & "']")
    
    If domGroup Is Nothing Then
        IsItemEnable = False
        Exit Function
    End If
    
    Dim domItem As MSXML2.IXMLDOMNode
    Set domItem = domGroup.selectSingleNode("/Groups/Group/Item[@Key='" & CStr(ID) & "']")
    
    If domItem Is Nothing Then
        IsItemEnable = False
        Exit Function
    End If
    
    Dim enabledNode As MSXML2.IXMLDOMNode
    Set enabledNode = domItem.Attributes.getNamedItem("IsEnabled")
    
    If enabledNode Is Nothing Then
        IsItemEnable = False
        Exit Function
    End If
    
    If Trim(enabledNode.nodeValue) = "Y" Or Trim(enabledNode.nodeValue) = "y" Or Trim(enabledNode.nodeValue) = "Yes" Then
        IsItemEnable = True
    Else
        IsItemEnable = False
    End If
   
    Dim namedNode As MSXML2.IXMLDOMNode
    Set namedNode = domItem.Attributes.getNamedItem("Name")
    
    If namedNode Is Nothing Then
        IsItemEnable = False
        Exit Function
    End If
    
    name = namedNode.nodeValue
    
eh:
    If Err.Number <> 0 Then
        m_error = Err.Description
        IsItemEnable = False
        Err.Clear
    End If
End Function

Public Function ErrorMessage() As String
    ErrorMessage = m_error
End Function
