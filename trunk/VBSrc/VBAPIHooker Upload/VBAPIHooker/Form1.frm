VERSION 5.00
Begin VB.Form frm 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "API Hook ʾ��"
   ClientHeight    =   7620
   ClientLeft      =   150
   ClientTop       =   435
   ClientWidth     =   8985
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7620
   ScaleWidth      =   8985
   StartUpPosition =   2  '��Ļ����
   Begin VB.Frame Frame2 
      Caption         =   "�����������ز���"
      Height          =   1155
      Left            =   480
      TabIndex        =   7
      Top             =   6330
      Width           =   8055
      Begin VB.CommandButton Command3 
         Caption         =   "�����հ׼��±�����"
         Height          =   615
         Left            =   960
         TabIndex        =   11
         Top             =   330
         Width           =   2595
      End
      Begin VB.CommandButton Command4 
         Caption         =   "�������ؼ��±�"
         Height          =   615
         Left            =   4590
         TabIndex        =   8
         Top             =   330
         Width           =   2595
      End
      Begin VB.Label Label5 
         Caption         =   "��2��"
         Height          =   255
         Left            =   4080
         TabIndex        =   13
         Top             =   540
         Width           =   465
      End
      Begin VB.Label Label4 
         AutoSize        =   -1  'True
         Caption         =   "��1��"
         Height          =   180
         Left            =   450
         TabIndex        =   12
         Top             =   540
         Width           =   450
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "���������ز��ԣ���������ԣ�"
      Height          =   1155
      Left            =   480
      TabIndex        =   4
      Top             =   5070
      Width           =   8055
      Begin VB.CommandButton Command2 
         Caption         =   "���Ե�����Ϣ��"
         Height          =   615
         Left            =   4590
         TabIndex        =   6
         Top             =   330
         Width           =   2595
      End
      Begin VB.CommandButton Command1 
         Caption         =   "���ر����� MsgBox(MessageBoxIndirectA)"
         Height          =   615
         Left            =   960
         TabIndex        =   5
         Top             =   330
         Width           =   2595
      End
      Begin VB.Label Label3 
         Caption         =   "��2��"
         Height          =   255
         Left            =   4080
         TabIndex        =   10
         Top             =   540
         Width           =   465
      End
      Begin VB.Label Label2 
         AutoSize        =   -1  'True
         Caption         =   "��1��"
         Height          =   180
         Left            =   450
         TabIndex        =   9
         Top             =   540
         Width           =   450
      End
   End
   Begin VB.ListBox List1 
      Height          =   1860
      Left            =   480
      TabIndex        =   1
      Top             =   3120
      Width           =   8055
   End
   Begin VB.TextBox Text1 
      Height          =   2205
      Left            =   480
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   390
      Width           =   8025
   End
   Begin VB.Label Label1 
      Caption         =   "��ջ���ݣ�"
      Height          =   285
      Index           =   1
      Left            =   480
      TabIndex        =   3
      Top             =   120
      Width           =   1365
   End
   Begin VB.Label Label1 
      Caption         =   "������Ϣ��"
      Height          =   285
      Index           =   0
      Left            =   480
      TabIndex        =   2
      Top             =   2820
      Width           =   1365
   End
End
Attribute VB_Name = "frm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long
Private Declare Function FindWindow Lib "user32" Alias "FindWindowA" (ByVal lpClassName As String, ByVal lpWindowName As String) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hWnd As Long, lpdwProcessId As Long) As Long

'����������
Private WithEvents m_oApiHooker As CVBAPIHooker
Attribute m_oApiHooker.VB_VarHelpID = -1
Private WithEvents m_oAPIHooker2 As CVBAPIHooker
Attribute m_oAPIHooker2.VB_VarHelpID = -1

Private Sub Command1_Click()
    Static lHookIndex As Long
    If Command1.Caption = "��ֹ����" Then
        Command1.Caption = "���ر����� MessageBoxIndirectA"
        
        m_oAPIHooker2.Unhook lHookIndex
        
        Set m_oAPIHooker2 = Nothing
    Else
        '���ر�����
        Set m_oAPIHooker2 = New CVBAPIHooker
    
        With m_oAPIHooker2
            .SetTargetTo -1
            
            If GetModuleHandle("vba6.dll") = 0 Then
                Command1.Caption = "��ֹ����"
                
                lHookIndex = .HookImport("msvbvm60.dll", "user32.dll", "MessageBoxIndirectA")
            Else
                MsgBox "��Ϊδ֪ԭ����IDE���޷�����BeforeAction��AfterAction�¼����������Ҫ���Ա������������ԡ�"
            End If
        End With
    End If
End Sub

Private Sub Command2_Click()
    Dim lRetVal As Long
    lRetVal = MsgBox("���ԣ�", vbExclamation + vbYesNoCancel + vbDefaultButton3)
    AddMsg "�����""" & IIf(lRetVal = vbYes, "��", IIf(lRetVal = vbNo, "��", "ȡ��")) & """"
End Sub

Private Sub Command3_Click()
    Shell "notepad.exe"
End Sub

Private Sub Command4_Click()
    Static lHookIndex As Long, lHookIndex2 As Long
    If Command4.Caption = "��ֹ����" Then
        Command4.Caption = "��������"
        
        m_oApiHooker.Unhook lHookIndex
        m_oApiHooker.Unhook lHookIndex2
    
        Set m_oApiHooker = Nothing
    Else
        Command4.Caption = "��ֹ����"
        
        '���ؼ��±�����
        Set m_oApiHooker = New CVBAPIHooker
    
        With m_oApiHooker
            Dim pid
            GetWindowThreadProcessId FindWindow(vbNullString, "�ޱ��� - ���±�"), pid
            .SetTargetTo pid
            lHookIndex = .HookImport("notepad.exe", "comdlg32.dll", "GetOpenFileNameW")
            lHookIndex2 = .HookImport("notepad.exe", "comdlg32.dll", "ChooseFontW")
        End With
    End If
End Sub

Private Sub AddMsg(ByVal sMsg As String)
    With List1
        .AddItem sMsg
        .Selected(.NewIndex) = True
    End With
End Sub

Private Sub AddTxt(ByVal sTxt As String)
    With Text1
        .Text = .Text & sTxt
        .SelStart = Len(.Text)
    End With
End Sub

Private Function Array2Str(v As Variant, ByVal lDataLen As Long, Optional lNewLinePos As Long = 4) As String
    If IsArray(v) Then
        Dim i As Long
        Array2Str = ""
        For i = LBound(v) To UBound(v)
            Array2Str = Array2Str & " " & IIf((i Mod lNewLinePos = 0) And i <> 0, vbCrLf, "") & Right$(String(lDataLen * 2, "0") & Hex(v(i)), lDataLen * 2)
        Next
        Array2Str = Trim(Array2Str)
    End If
End Function

Private Sub m_oApiHooker_BeforeAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal lpArgs As Long, fIntercept As Boolean, lArgCount As Long, ByVal lReturnAddress As Long)
    AddMsg "�������ú��� " & sAPIName & " ,������ջ��ַΪ��&H" & Hex(lpArgs)
    
    Select Case sAPIName
    Case "GetOpenFileNameW"
        '��ʾ��ջ����
        Dim abDumpData() As Byte
        Dim alArgs() As Long
        
        m_oApiHooker.DumpStack abDumpData(), 128
        m_oApiHooker.GetArguments alArgs(), 16
        
        AddTxt "���ֽڷ�ʽ��" & vbCrLf
        AddTxt Array2Str(abDumpData(), 1, 8) & vbCrLf & vbCrLf
        AddTxt "���ֽڷ�ʽ��" & vbCrLf
        AddTxt Array2Str(alArgs(), 4, 4) & vbCrLf & vbCrLf
        fIntercept = False
    Case "ChooseFontW"
        '��ֹAPI
        fIntercept = True
        lArgCount = 1
    End Select
End Sub

Private Sub m_oApiHooker_AfterAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal fIntercepted As Boolean, lRetValue As Long)
    AddMsg "���� " & sAPIName & IIf(fIntercepted, "����ֹ", " �������,���ز���Ϊ��&H" & Hex(lRetValue))
    
    Select Case sAPIName
    Case "GetOpenFileNameW"
    Case "ChooseFontW"
    End Select
    
    '�޸ĺ�������ֵ
    'lRetValue = 1
End Sub

Private Sub m_oAPIHooker2_BeforeAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal lpArgs As Long, fIntercept As Boolean, lArgCount As Long, ByVal lReturnAddress As Long)
    AddMsg "�������ú��� " & sAPIName & " ,������ջ��ַΪ��&H" & Hex(lpArgs)
    
    '��ʾ��ջ����
    Dim abDumpData() As Byte
    Dim alArgs() As Long
    
    m_oAPIHooker2.DumpStack abDumpData(), 128
    m_oAPIHooker2.GetArguments alArgs(), 16
    
    AddTxt "���ֽڷ�ʽ��" & vbCrLf
    AddTxt Array2Str(abDumpData(), 1, 8) & vbCrLf & vbCrLf
    AddTxt "���ֽڷ�ʽ��" & vbCrLf
    AddTxt Array2Str(alArgs(), 4, 4) & vbCrLf & vbCrLf
     
    fIntercept = False
    lArgCount = 1
End Sub

Private Sub m_oAPIHooker2_AfterAction(ByVal lHookID As Long, ByVal lHookType As Long, ByVal sAPIName As String, ByVal fIntercepted As Boolean, lRetValue As Long)
    AddMsg "���� " & sAPIName & IIf(fIntercepted, "����ֹ", " �������,���ز���Ϊ��&H" & Hex(lRetValue))
    
    '�޸ĺ�������ֵ
    lRetValue = vbYes
End Sub
