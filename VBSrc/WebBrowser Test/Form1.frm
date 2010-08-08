VERSION 5.00
Object = "{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}#1.1#0"; "shdocvw.dll"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5145
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7185
   LinkTopic       =   "Form1"
   ScaleHeight     =   5145
   ScaleWidth      =   7185
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox Text1 
      Height          =   330
      Left            =   45
      OLEDropMode     =   1  'Manual
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   4290
      Width           =   7065
   End
   Begin SHDocVwCtl.WebBrowser WebBrowser1 
      Height          =   4095
      Left            =   60
      TabIndex        =   0
      Top             =   0
      Width           =   7035
      ExtentX         =   12409
      ExtentY         =   7223
      ViewMode        =   0
      Offline         =   0
      Silent          =   0
      RegisterAsBrowser=   0
      RegisterAsDropTarget=   1
      AutoArrange     =   0   'False
      NoClientEdge    =   0   'False
      AlignLeft       =   0   'False
      NoWebView       =   0   'False
      HideFileNames   =   0   'False
      SingleClick     =   0   'False
      SingleSelection =   0   'False
      NoFolders       =   0   'False
      Transparent     =   0   'False
      ViewID          =   "{0057D0E0-3573-11CF-AE69-08002B2E1262}"
      Location        =   "http:///"
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'----------------- 看这里 -----------------
'----------------- 看这里 -----------------
'----------------- 看这里 -----------------
'----------------- 看这里 -----------------
'其他地方是其他东西
Dim oDoc As HTMLDocument
Dim oWeb As WebBrowser_V1

Private Sub WebBrowser1_NavigateComplete2(ByVal pDisp As Object, URL As Variant)
    Dim oTable As HTMLTable
    Set oWeb = pDisp
    Set oDoc = oWeb.Document
    Set oTable = oDoc.documentElement.All("Table3")
    If Not oTable Is Nothing Then
        oTable.outerHTML = "supergreenbean(超级绿豆)~~~~哈哈哈哈哈"
    End If
End Sub
'----------------- 看这里 -----------------
'----------------- 看这里 -----------------
'----------------- 看这里 -----------------
'----------------- 看这里 -----------------

Private Sub Form_Load()
WebBrowser1.Navigate "http://expert.csdn.net/expert/forum.asp"
End Sub

Private Sub Form_Resize()
    WebBrowser1.Width = Me.ScaleWidth
End Sub

Private Sub Text1_OLEDragDrop(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Data.GetFormat(vbCFText) Then
        Text1.Text = Data.GetData(vbCFText)
    ElseIf Data.GetFormat(vbCFFiles) Then
        Text1.Text = Data.Files(1)
    End If
End Sub

Private Sub WebBrowser1_DocumentComplete(ByVal pDisp As Object, URL As Variant)
    Dim oEvent As New CEventHandler
    oEvent.SetHandler Me, "GetIt" '设定处理事件要调用的方法
    WebBrowser1.Document.body.onmouseup = oEvent '绑定事件对象
    
End Sub

Public Sub GetIt()
    Dim oDoc As Object
    Dim oTxtRgn As Object
    Dim sSelectedHTML As String
    Set oDoc = WebBrowser1.Document '获得文档对象
    Set oTxtRgn = oDoc.selection.createRange '获得文档当前正选择的区域对象
    sSelectedHTML = oTxtRgn.htmlText '选择区域文本赋值
    Text1.Text = sSelectedHTML '文本框显示抓取得HTML源码
    

End Sub

Public Function Check() As Boolean
Dim vDoc, vTag
Dim i As Integer
Set vDoc = WebBrowser1.Document
For i = 0 To vDoc.All.length - 1
If UCase(vDoc.All(i).tagName) = "INPUT" Then
Set vTag = vDoc.All(i)
If vTag.Type = "button" Then
Check = True
Exit Function
End If
End If
Next i
Check = False
End Function


