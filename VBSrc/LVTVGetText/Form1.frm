VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Form1 
   ClientHeight    =   4200
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   4200
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command4 
      Caption         =   "保存LV内容"
      Height          =   645
      Left            =   660
      TabIndex        =   10
      Top             =   3270
      Width           =   1335
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   270
      Top             =   3450
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton Command3 
      Caption         =   "保存TV内容"
      Height          =   645
      Left            =   2730
      TabIndex        =   9
      Top             =   3270
      Width           =   1335
   End
   Begin VB.CommandButton Command2 
      Caption         =   "获得TV文本"
      Height          =   645
      Left            =   2730
      TabIndex        =   8
      Top             =   2340
      Width           =   1335
   End
   Begin VB.TextBox Text3 
      Height          =   855
      Left            =   570
      MultiLine       =   -1  'True
      TabIndex        =   6
      Top             =   1140
      Width           =   3855
   End
   Begin VB.CommandButton Command1 
      Caption         =   "获得LV文本"
      Height          =   645
      Left            =   660
      TabIndex        =   5
      Top             =   2340
      Width           =   1335
   End
   Begin VB.TextBox Text2 
      Height          =   285
      Left            =   570
      TabIndex        =   3
      Text            =   "0"
      Top             =   600
      Width           =   915
   End
   Begin VB.CheckBox Check1 
      Caption         =   "开始获得句柄"
      Height          =   345
      Left            =   2280
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   120
      Width           =   1455
   End
   Begin VB.TextBox Text1 
      Height          =   315
      Left            =   570
      TabIndex        =   0
      Top             =   120
      Width           =   1575
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "text"
      Height          =   180
      Index           =   1
      Left            =   90
      TabIndex        =   7
      Top             =   1170
      Width           =   360
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      Caption         =   "index"
      Height          =   180
      Index           =   0
      Left            =   60
      TabIndex        =   4
      Top             =   630
      Width           =   450
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "hWnd"
      Height          =   180
      Left            =   150
      TabIndex        =   1
      Top             =   180
      Width           =   360
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim fGethWnd As Boolean
Dim mlhWnd As Long
Dim mhProcess As Long
Dim mlpTreeItemRemote As Long, mlpTextRemote As Long
Dim mszBuf() As Byte
Dim mnMaxLen As Long

Private mStackFlag As CStack
Private mStackNode As CStack
Function GetItemText(ByVal lhWnd As Long, ByVal lItemIndex As Long, Optional ByVal lSubItemIndex As Long = 0) As String
    Dim mhwnd As Long
    mhwnd = lhWnd
    Dim i As Long, s As String
    Dim dwProcessId As Long, hProcess As Long
    Dim dwBytesRead As Long, dwBytesWrite As Long
    Dim bSuccess As Long
    Call GetWindowThreadProcessId(mhwnd, dwProcessId)
    Dim lpListItemRemote As Long, lpTextRemote As Long
    Dim nMaxLen As Long
    nMaxLen = 1023
    Dim szBuf() As Byte
    ReDim szBuf(nMaxLen)
    Dim lvItemLocal As LV_ITEM
    Dim bWriteOK As Long
    
    'Dim lItemIndex As Long
    lItemIndex = lItemIndex
    Dim sItemText As String
    
    hProcess = OpenProcess(PROCESS_VM_OPERATION Or PROCESS_VM_READ Or PROCESS_VM_WRITE, 0&, dwProcessId)
    If hProcess <> 0 Then
        lpTextRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, nMaxLen + 1, MEM_COMMIT, PAGE_READWRITE)
        lpListItemRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, Len(lvItemLocal), MEM_COMMIT, PAGE_READWRITE)
        bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesWrite)
        lvItemLocal.iItem = lItemIndex
        lvItemLocal.iSubItem = lSubItemIndex
        lvItemLocal.mask = LVIF_TEXT
        lvItemLocal.cchTextMax = nMaxLen
        lvItemLocal.pszText = lpTextRemote
        dwBytesWrite = 0
        bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpListItemRemote, ByVal VarPtr(lvItemLocal), Len(lvItemLocal), dwBytesWrite)
        i = SendMessage(mhwnd, LVM_GETITEMTEXT, lItemIndex, ByVal lpListItemRemote)
        bSuccess = ReadProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesRead)
        Call VirtualFreeEx(hProcess, ByVal lpListItemRemote, 0, MEM_DECOMMIT)
        Call VirtualFreeEx(hProcess, ByVal lpTextRemote, 0, MEM_DECOMMIT)
    End If
    CloseHandle hProcess
    
    '*************** 显示结果
    i = InStrB(szBuf, ChrB(0))
    If i > 1 Then i = i - 1
    sItemText = StrConv(LeftB(szBuf, i), vbUnicode)
    '*************** 显示结果
    GetItemText = sItemText
End Function

Sub GetLVText(Optional lSubItemIndex As Long = 0)
    Dim mhwnd As Long
    mhwnd = mlhWnd
    Dim i As Long, s As String
    Dim dwProcessId As Long, hProcess As Long
    Dim dwBytesRead As Long, dwBytesWrite As Long
    Dim bSuccess As Long
    Call GetWindowThreadProcessId(mhwnd, dwProcessId)
    Dim lpListItemRemote As Long, lpTextRemote As Long
    Dim nMaxLen As Long
    nMaxLen = 1023
    Dim szBuf() As Byte
    ReDim szBuf(nMaxLen)
    Dim lvItemLocal As LV_ITEM
    Dim bWriteOK As Long
    
    Dim lItemIndex As Long
    lItemIndex = CLng(Text2.Text)
    Dim sItemText As String
    
    hProcess = OpenProcess(PROCESS_VM_OPERATION Or PROCESS_VM_READ Or PROCESS_VM_WRITE, 0&, dwProcessId)
    If hProcess <> 0 Then
        lpTextRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, nMaxLen + 1, MEM_COMMIT, PAGE_READWRITE)
        lpListItemRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, Len(lvItemLocal), MEM_COMMIT, PAGE_READWRITE)
        bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesWrite)
        lvItemLocal.iItem = lItemIndex
        lvItemLocal.iSubItem = lSubItemIndex
        lvItemLocal.mask = LVIF_TEXT
        lvItemLocal.cchTextMax = nMaxLen
        lvItemLocal.pszText = lpTextRemote
        dwBytesWrite = 0
        bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpListItemRemote, ByVal VarPtr(lvItemLocal), Len(lvItemLocal), dwBytesWrite)
        i = SendMessage(mhwnd, LVM_GETITEMTEXT, lItemIndex, ByVal lpListItemRemote)
        bSuccess = ReadProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesRead)
        Call VirtualFreeEx(hProcess, ByVal lpListItemRemote, 0, MEM_DECOMMIT)
        Call VirtualFreeEx(hProcess, ByVal lpTextRemote, 0, MEM_DECOMMIT)
    End If
    CloseHandle hProcess
    
    '*************** 显示结果
    sItemText = StrConv(LeftB(szBuf, InStrB(szBuf, ChrB(0))), vbUnicode)
    Text3.Text = sItemText
    '*************** 显示结果
End Sub

Sub GetTVText()
    Dim mhwnd As Long
    mhwnd = mlhWnd
    Dim i As Long, s As String
    Dim dwProcessId As Long, hProcess As Long
    Dim dwBytesRead As Long, dwBytesWrite As Long
    Dim bSuccess As Long
    Call GetWindowThreadProcessId(mhwnd, dwProcessId)
    Dim lpTreeItemRemote As Long, lpTextRemote As Long
    Dim nMaxLen As Long
    nMaxLen = 1023
    Dim szBuf() As Byte
    ReDim szBuf(nMaxLen)
    Dim lvItemLocal As TVITEM
    Dim bWriteOK As Long
    
    Dim lItemIndex As Long
    lItemIndex = CLng(Text2.Text)
    Dim sItemText As String
    
    hProcess = OpenProcess(PROCESS_VM_OPERATION Or PROCESS_VM_READ Or PROCESS_VM_WRITE, 0&, dwProcessId)
    If hProcess <> 0 Then
        lpTextRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, nMaxLen + 1, MEM_COMMIT, PAGE_READWRITE)
        lpTreeItemRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, Len(lvItemLocal), MEM_COMMIT, PAGE_READWRITE)
        bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesWrite)
        
        'write structure
        dwBytesWrite = 0
        lvItemLocal.hItem = SendMessage(mhwnd, TVM_GETNEXTITEM, TVGN_CARET, ByVal 0)
        lvItemLocal.mask = TVIF_TEXT + TVIF_HANDLE
        lvItemLocal.cchTextMax = nMaxLen
        lvItemLocal.pszText = lpTextRemote
        bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpTreeItemRemote, ByVal VarPtr(lvItemLocal), Len(lvItemLocal), dwBytesWrite)
        
        'get item
        i = SendMessage(mhwnd, TVM_GETITEM, 0&, ByVal lpTreeItemRemote)
        
        
        'read result
        bSuccess = ReadProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesRead)
        Call VirtualFreeEx(hProcess, ByVal lpTreeItemRemote, 0, MEM_DECOMMIT)
        Call VirtualFreeEx(hProcess, ByVal lpTextRemote, 0, MEM_DECOMMIT)
    End If
    CloseHandle hProcess
    
    '*************** 显示结果
    sItemText = StrConv(LeftB(szBuf, InStrB(szBuf, ChrB(0))), vbUnicode)
    Text3.Text = sItemText
    '*************** 显示结果
End Sub

Function GetNodeText(ByVal hNode As Long) As String
    On Error Resume Next
    Dim mhwnd As Long
    Dim i As Long, s As String
    Dim dwBytesRead As Long, dwBytesWrite As Long
    Dim bSuccess As Long
    Dim lvItemLocal As TVITEM
    Dim bWriteOK As Long
    
    mhwnd = mlhWnd
    
    ReDim mszBuf(mnMaxLen)
    bWriteOK = WriteProcessMemory(ByVal mhProcess, ByVal mlpTextRemote, mszBuf(0), mnMaxLen + 1, dwBytesWrite)
    
    'write structure
    dwBytesWrite = 0
    lvItemLocal.hItem = hNode
    lvItemLocal.mask = TVIF_TEXT + TVIF_HANDLE
    lvItemLocal.cchTextMax = mnMaxLen
    lvItemLocal.pszText = mlpTextRemote
    bWriteOK = WriteProcessMemory(ByVal mhProcess, ByVal mlpTreeItemRemote, ByVal VarPtr(lvItemLocal), Len(lvItemLocal), dwBytesWrite)
    
    'get item
    i = SendMessage(mhwnd, TVM_GETITEM, 0&, ByVal mlpTreeItemRemote)
    
    
    'read result
    bSuccess = ReadProcessMemory(ByVal mhProcess, ByVal mlpTextRemote, mszBuf(0), mnMaxLen + 1, dwBytesRead)
    
    GetNodeText = StrConv(LeftB(mszBuf, InStrB(mszBuf, ChrB(0)) - 1), vbUnicode)
End Function

'Sub WalkNode(ByVal lhWnd As Long, ByVal hNode As Long, ByVal lFileNO As Long, Optional lLevel As Long = 0)
'    Dim hChild As Long, hSibling As Long
'
'    '对每个节点干点什么吧
'    Print #lFileNO, Space(lLevel * 2) & GetNodeText(hNode)
'
'
'    '有孩子的先抱孩子
'    hChild = SendMessage(lhWnd, TVM_GETNEXTITEM, TVGN_CHILD, ByVal hNode)
'    If hChild <> 0 Then
'        WalkNode lhWnd, hChild, lFileNO, lLevel + 1
'    End If
'
'    '有兄弟的再抱兄弟
'    hSibling = SendMessage(lhWnd, TVM_GETNEXTITEM, TVGN_NEXT, ByVal hNode)
'    If hSibling <> 0 And lLevel > 0 Then
'        WalkNode lhWnd, hSibling, lFileNO, lLevel
'    End If
'End Sub

Function WalkNode1(ByVal hNode As Long, ByVal lFileNO As Long) As String
    Dim lLevel As Long
    Dim fChild As Boolean, fEnd As Boolean
    Dim hTmpNode As Long
    Dim hChild As Long, hSibling As Long
    Dim sText As String, i As Long
    
    lLevel = 0
    hTmpNode = hNode
    sText = ""
bgChild:
        fChild = False
        '对每个节点干点什么吧
        sText = sText & vbCrLf & Space(lLevel * 2) & GetNodeText(hTmpNode)
        i = i + 1
        Debug.Print i
        'Debug.Print sText
        'Print #lFileNO, sText
        
        hChild = SendMessage(mlhWnd, TVM_GETNEXTITEM, TVGN_CHILD, ByVal hTmpNode)
        If hChild <> 0 Then
            fChild = True
            mStackNode.Push hTmpNode
            mStackFlag.Push fChild
            hTmpNode = hChild
            lLevel = lLevel + 1
            GoTo bgChild
        End If
        
bgSibling:
        hSibling = SendMessage(mlhWnd, TVM_GETNEXTITEM, TVGN_NEXT, ByVal hTmpNode)
        If hSibling <> 0 Then
            If lLevel = 0 And Not fEnd Then
                mStackNode.Push hTmpNode
                mStackFlag.Push fChild
            End If
            hTmpNode = hSibling
            GoTo bgChild
        Else
            If lLevel = 0 Then
                fEnd = (lLevel = 0)
                GoTo ed
            End If
        End If
        
       
        If mStackNode.Pop(hTmpNode) And mStackFlag.Pop(fChild) Then
            If Not fChild Then
                GoTo bgChild
            Else
                If lLevel > 0 Then
                    lLevel = lLevel - 1
                End If
                GoTo bgSibling
            End If
        End If
ed:
    WalkNode1 = sText
End Function

Sub GetTVTextAll(ByVal sFilename As String)
    Dim i As Long, s As String
    Dim dwProcessId As Long, hProcess As Long
    Dim dwBytesRead As Long, dwBytesWrite As Long
    Dim bSuccess As Long
    Dim lpTreeItemRemote As Long, lpTextRemote As Long
    
    mnMaxLen = 1023
    ReDim mszBuf(mnMaxLen)
    Dim lvItemLocal As TVITEM
    If sFilename = "" Then Exit Sub
    Call GetWindowThreadProcessId(mlhWnd, dwProcessId)
    mhProcess = OpenProcess(PROCESS_VM_OPERATION Or PROCESS_VM_READ Or PROCESS_VM_WRITE, 0&, dwProcessId)
    If mhProcess <> 0 Then
        'allocate memory
        mlpTextRemote = VirtualAllocEx(ByVal mhProcess, ByVal 0&, mnMaxLen + 1, MEM_COMMIT, PAGE_READWRITE)
        mlpTreeItemRemote = VirtualAllocEx(ByVal mhProcess, ByVal 0&, Len(lvItemLocal), MEM_COMMIT, PAGE_READWRITE)

        'get content and write to file
        Dim lFreefile As Long
        Dim hRoot As Long
        lFreefile = FreeFile
        Open sFilename For Output As #lFreefile
        
        hRoot = SendMessage(mlhWnd, TVM_GETNEXTITEM, TVGN_ROOT, ByVal 0&)
        'hRoot = SendMessage(mlhWnd, TVM_GETNEXTITEM, TVGN_CARET, ByVal 0&)
        If hRoot <> 0 Then
            Set mStackNode = New CStack
            Set mStackFlag = New CStack
            Print #lFreefile, WalkNode1(hRoot, lFreefile)
            Set mStackNode = Nothing
            Set mStackFlag = Nothing
        End If
        
        Close #lFreefile
        
        'dellocate memory
        Call VirtualFreeEx(mhProcess, ByVal mlpTreeItemRemote, 0, MEM_DECOMMIT)
        Call VirtualFreeEx(mhProcess, ByVal mlpTextRemote, 0, MEM_DECOMMIT)
    End If
    CloseHandle mhProcess
End Sub

Function GetLVTextAll(ByVal sFilename As String) As String
    Dim mhwnd As Long
    mhwnd = mlhWnd
    Dim i As Long, s As String
    Dim dwProcessId As Long, hProcess As Long
    Dim dwBytesRead As Long, dwBytesWrite As Long
    Dim bSuccess As Long
    Call GetWindowThreadProcessId(mhwnd, dwProcessId)
    Dim lpListItemRemote As Long, lpTextRemote As Long
    Dim nMaxLen As Long
    nMaxLen = 1023
    Dim szBuf() As Byte
    ReDim szBuf(nMaxLen)
    Dim lvItemLocal As LV_ITEM
    Dim bWriteOK As Long
    Dim lItemIndex As Long

    Dim sItemText As String
    
    Dim lSubItemIndex As Long
    
    Dim lFreefile As Long
    Dim lListItemCount As Long, lSubItemCount As Long, k As Long, j As Long
    Dim sOutput As String
    
    lFreefile = FreeFile
    Open sFilename For Output As #lFreefile
    
    lListItemCount = SendMessage(mlhWnd, LVM_GETITEMCOUNT, 0&, ByVal 0&)
    lSubItemCount = GetColumnHeaderCount(mlhWnd) - 1
    
    hProcess = OpenProcess(PROCESS_VM_OPERATION Or PROCESS_VM_READ Or PROCESS_VM_WRITE, 0&, dwProcessId)
    If hProcess <> 0 Then
        lpTextRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, nMaxLen + 1, MEM_COMMIT, PAGE_READWRITE)
        lpListItemRemote = VirtualAllocEx(ByVal hProcess, ByVal 0&, Len(lvItemLocal), MEM_COMMIT, PAGE_READWRITE)
        
        For k = 0 To lListItemCount - 1
            lItemIndex = k
            For j = 0 To lSubItemCount
                lSubItemIndex = j
                bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesWrite)
                lvItemLocal.iItem = lItemIndex
                lvItemLocal.iSubItem = lSubItemIndex
                lvItemLocal.mask = LVIF_TEXT
                lvItemLocal.cchTextMax = nMaxLen
                lvItemLocal.pszText = lpTextRemote
                dwBytesWrite = 0
                bWriteOK = WriteProcessMemory(ByVal hProcess, ByVal lpListItemRemote, ByVal VarPtr(lvItemLocal), Len(lvItemLocal), dwBytesWrite)
                i = SendMessage(mhwnd, LVM_GETITEMTEXT, lItemIndex, ByVal lpListItemRemote)
                bSuccess = ReadProcessMemory(ByVal hProcess, ByVal lpTextRemote, szBuf(0), nMaxLen + 1, dwBytesRead)
                
                '*************** 显示结果
                i = InStrB(szBuf, ChrB(0))
                If i > 1 Then i = i - 1
                sItemText = StrConv(LeftB(szBuf, i), vbUnicode)
                '*************** 显示结果
                If j > 0 Then
                    sOutput = sOutput & vbTab & sItemText
                Else
                    sOutput = sItemText
                End If
            Next
            Print #lFreefile, sOutput
        Next
        
        Call VirtualFreeEx(hProcess, ByVal lpListItemRemote, nMaxLen + 1, MEM_DECOMMIT)
        Call VirtualFreeEx(hProcess, ByVal lpTextRemote, Len(lvItemLocal), MEM_DECOMMIT)
        CloseHandle hProcess
    End If
    
    Close #lFreefile
End Function

Private Sub Check1_Click()
    Dim pt As POINTAPI
    fGethWnd = Not fGethWnd
    Check1.Value = Abs(fGethWnd)
    Do While fGethWnd
        GetCursorPos pt
        mlhWnd = WindowFromPoint(pt.x, pt.y)
        If IsOK(mlhWnd) Then
            Text1.Text = mlhWnd
        End If
        DoEvents
    Loop
End Sub

Private Sub Command1_Click()
    Call GetLVText
End Sub

Private Sub Command2_Click()
    Call GetTVText
End Sub

Private Sub Command3_Click()
    Call GetTVTextAll(GetFile)
End Sub

Private Sub Command4_Click()
    Call GetLVTextAll(GetFile)
End Sub
Private Sub Form_Unload(Cancel As Integer)
    fGethWnd = False
End Sub

Function IsOK(ByVal val As Long) As Boolean
    Dim s As String * 255
    GetClassName val, s, 255
    IsOK = True
    'Debug.Print s
End Function

Function GetFile() As String
On Error Resume Next
With CommonDialog1
    .CancelError = True
    .Filter = "*.txt|*.txt"
    .ShowSave
    If Err.Number = 0 Then
        GetFile = .FileName
    End If
End With
End Function

Function GetColumnHeaderCount(ByVal hLVWnd As Long) As Long
    Dim hHD As Long
    hHD = SendMessage(hLVWnd, LVM_GETHEADER, 0, ByVal 0)
    If hHD = 0 Then Exit Function
    GetColumnHeaderCount = SendMessage(hHD, HDM_GETITEMCOUNT, 0, ByVal 0)
End Function
