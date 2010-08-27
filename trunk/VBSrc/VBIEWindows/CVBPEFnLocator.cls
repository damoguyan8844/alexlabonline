VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CVBPEFnLocator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'***********************************************************************************************************************
'
'ģ �� ��: CVBPEFnLocator
'
'��    ��: Supergreenbean
'
'��������: ��ȡ���滻ָ���������ض�ģ�������/���������ַ
'
'��    ��: 2005��03��02��
'
'ʹ��˵��:
'         1. �趨Ŀ�����
'               ��:
'                   ���趨Ŀ�����Ϊ����ID����&H1234�Ľ���
'                   oPEFnLocator.SetTargetTo &H1234
'
'                   ע:�����ִ���ⲽ,��Ĭ�ϰ�Ŀ�������Ϊ��ǰ����
'
'         2. ��λ�ض�ģ���е��ض�����/�������
'
'           a)����ģ������λ����/�������
'               ��:
'                   �ٶ�Shell32.dll��ģ����Ϊ&H773A0000,
'
'                   ����ζ�λshell32.dll��,��user32.dll���������ShowCaret�����ĵ�ַ
'                       oPEFnLocator.LocateImportFunction &H773A0000,"user32.dll","ShowCaret"
'
'                   ����ζ�λshell32.dll�е�������Control_RunDLL�ĵ�ַ
'                       oPEFnLocator.LocateExportFunction &H773A0000,"Control_RunDLL"
'
'           b)����ģ�����ƶ�λ����/�������
'               ��:
'                   ����ζ�λshell32.dll��,��user32.dll���������ShowCaret�����ĵ�ַ
'                       oPEFnLocator.LocateImportFunction "shell32.dll","user32.dll","ShowCaret"
'
'                   ����ζ�λshell32.dll�е�������Control_RunDLL�ĵ�ַ
'                       oPEFnLocator.LocateExportFunction "shell32.dll","Control_RunDLL"
'
'         3. �滻��ת��ַ
'               ��:
'                   ������滻�Ѷ�λ�ĺ�����ת��ַΪ&H123456
'                       oPEFnLocator.ImpReplace &H123456
'         4. �ָ���ת��ַ
'               ��:
'                   ����λָ��Ѷ�λ�ĺ���ԭ��ת��ַ
'                       oPEFnLocator.ImpRestore
'         5. ��õ��������б�
'               ��:
'                   Dim avExports() As Variant
'                   oPEFnLocator.GetExports hMod, avExports()
'         5. ��õ��뺯���б�
'               ��:
'                   Dim avImports() As Variant
'                   oPEFnLocator.GetImports hMod, avImports()
'***********************************************************************************************************************
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CVBPEFnLocator"

'=======================================================================================================================
'����ΪWindows API�������
'=======================================================================================================================
Private Declare Function GetCurrentProcessId Lib "kernel32" () As Long
Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private Const PROCESS_ALL_ACCESS = &H1F0FFF

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)

Private Declare Function VirtualProtectEx Lib "kernel32" (ByVal hProcess As Long, lAddress As Any, ByVal dwSize As Long, ByVal flNewProtect As Long, lpflOldProtect As Long) As Long
Private Const PAGE_READWRITE As Long = &H4

Private Declare Function ReadProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Private Declare Function WriteProcessMemory Lib "kernel32" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long

Private Declare Function CreateToolhelp32Snapshot Lib "kernel32" (ByVal dwFlags As Long, ByVal th32ProcessID As Long) As Long
Private Const TH32CS_SNAPMODULE = &H8

Private Declare Function Module32First Lib "kernel32" (ByVal hSnapShot As Long, lpme As MODULEENTRY32) As Long
Private Declare Function Module32Next Lib "kernel32" (ByVal hSnapShot As Long, lpme As MODULEENTRY32) As Long
Private Type MODULEENTRY32
    dwSize As Long
    th32ModuleID As Long
    th32ProcessID As Long
    GlblcntUsage As Long
    ProccntUsage As Long
    modBaseAddr As Long
    modBaseSize As Long
    hModule As Long
    szModule  As String * 256
    szExePath As String * 260
End Type

Private Declare Function GetFullPathName Lib "kernel32" Alias "GetFullPathNameA" (ByVal lpFileName As String, ByVal nBufferLength As Long, ByVal lpBuffer As String, lpFilePart As Long) As Long
'=======================================================================================================================
'����ΪWindows API�������
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ���ڲ�˽�б���
'=======================================================================================================================

'Ŀ�꺯���ں���������е���ڵ�ַ
Private m_lImpEntryAddress As Long
'Ŀ�꺯���ں���������е���ת��ַ
Private m_lImpJumpAddress As Long
'Ŀ��ģ���ļ��������룩
Private m_sImpModuleName As String
'Ŀ�꺯���ں���������е���ڵ�ַ
Private m_lExpEntryAddress As Long
'Ŀ�꺯���ں���������е���ת��ַ
Private m_lExpJumpAddress As Long
'Ŀ��ģ���ļ����������
Private m_sExpModuleName As String
'������ڵ�6���ֽڵĴ���
Private m_abExpEntryCodes(5) As Byte

'�����������ת��ַ�Ƿ��滻��
Private m_fImpReplaced As Boolean
'��������Ƿ��滻��
Private m_fExpReplaced As Boolean
'�Ƿ����౻����ʱ�Զ��ָ����滻��ַ
Private m_fAutoRestore As Boolean

'Ŀ����̵Ľ���ID
Private m_lProcessID As Long
'Ŀ����̵Ľ��̾��
Private m_lhProcess As Long
'=======================================================================================================================
'����Ϊ���ڲ�˽�б���
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ������
'=======================================================================================================================
'�Ƿ��Զ��ָ����滻��ַ
Public Property Let AutoRestore(fAutoRestore As Boolean)
    m_fAutoRestore = fAutoRestore
End Property

Public Property Get AutoRestore() As Boolean
     AutoRestore = m_fAutoRestore
End Property

'�Ƿ��滻�˵����ַ
Public Property Get ImpReplaced() As Boolean
     ImpReplaced = m_fImpReplaced
End Property

'�Ƿ��滻�˵�������/��ָ����ַ
Public Property Get ExpReplaced() As Boolean
     ExpReplaced = m_fExpReplaced
End Property


'Ŀ��ģ���ļ���
Public Property Let ImpModuleName(sImpModuleName As String)
    m_sImpModuleName = sImpModuleName
End Property

Public Property Get ImpModuleName() As String
     ImpModuleName = m_sImpModuleName
End Property

'Ŀ�꺯���ں���������е���ת��ַ
Public Property Let ImpJumpAddress(lImpJumpAddress As Long)
    m_lImpJumpAddress = lImpJumpAddress
End Property

Public Property Get ImpJumpAddress() As Long
     ImpJumpAddress = m_lImpJumpAddress
End Property


'Ŀ�꺯���ں���������е���ڵ�ַ
Public Property Let ImpEntryAddress(lImpEntryAddress As Long)
    m_lImpEntryAddress = lImpEntryAddress
End Property

Public Property Get ImpEntryAddress() As Long
     ImpEntryAddress = m_lImpEntryAddress
End Property

'Ŀ��ģ���ļ���
Public Property Let ExpModuleName(sExpModuleName As String)
    m_sExpModuleName = sExpModuleName
End Property

Public Property Get ExpModuleName() As String
     ExpModuleName = m_sExpModuleName
End Property

'Ŀ�꺯���ں���������е���ת��ַ
Public Property Let ExpJumpAddress(lExpJumpAddress As Long)
    m_lExpJumpAddress = lExpJumpAddress
End Property

Public Property Get ExpJumpAddress() As Long
     ExpJumpAddress = m_lExpJumpAddress
End Property


'Ŀ�꺯���ں���������е���ڵ�ַ
Public Property Let ExpEntryAddress(lExpEntryAddress As Long)
    m_lExpEntryAddress = lExpEntryAddress
End Property

Public Property Get ExpEntryAddress() As Long
     ExpEntryAddress = m_lExpEntryAddress
End Property
'=======================================================================================================================
'����Ϊ������
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ�๫�к���
'=======================================================================================================================
'�滻������ת��ַ
Public Function ImpReplace(ByVal lAddress As Long, Optional ByVal lpTargetAddress As Long = 0) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
    
    If m_fImpReplaced Then Exit Function
    
    lpTargetAddress = IIf(lpTargetAddress = 0, m_lImpEntryAddress, lpTargetAddress)
    
    If lpTargetAddress = 0 Then Exit Function
    
    ImpReplace = memcpy2remote(lpTargetAddress, VarPtr(lAddress), 4)
    
    m_fImpReplaced = ImpReplace
    Exit Function
Error_Handler:
End Function

'�ָ�ԭ������ת��ַ
Public Function ImpRestore() As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
        
    If Not m_fImpReplaced Then Exit Function
    
    If m_lImpJumpAddress = 0 Then Exit Function
        
    ImpRestore = memcpy2remote(m_lImpEntryAddress, VarPtr(m_lImpJumpAddress), 4)
    
    m_fImpReplaced = False
    Exit Function
Error_Handler:
End Function

'�滻������ת��ַ
Public Function ExpReplace(ByVal lpAddress As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
            
    If m_fExpReplaced Then Exit Function
    
    If lpAddress = 0 Or m_lExpJumpAddress = 0 Then Exit Function
    
    If memcpy2local(VarPtr(m_abExpEntryCodes(0)), m_lExpJumpAddress, 6) Then
    
        ExpReplace = memcpy2remote(m_lExpJumpAddress, VarPtr(&H25FF), 2)
        ExpReplace = memcpy2remote(m_lExpJumpAddress + 2, VarPtr(lpAddress), 4)
        
        m_fExpReplaced = ExpReplace
    End If
    Exit Function
Error_Handler:
End Function

'�ָ�ԭ������ת��ַ
Public Function ExpRestore() As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
        
    If Not m_fExpReplaced Then Exit Function
    
    If m_lExpJumpAddress = 0 Then Exit Function
        
    ExpRestore = memcpy2remote(m_lExpJumpAddress, VarPtr(m_abExpEntryCodes(0)), 6)
    
    m_fExpReplaced = False
    Exit Function
Error_Handler:
End Function

'�趨���뺯����ַ
Public Function SetImportFunctionAddress(ByVal lAddress As Long) As Boolean
    m_sImpModuleName = "UserDefinedTargetImportAddress"
    
    m_lImpEntryAddress = lAddress
    
    SetImportFunctionAddress = memcpy2local(VarPtr(m_lImpJumpAddress), m_lImpEntryAddress, 4)
End Function

' ��λ���뺯��
Public Function LocateImportFunction(ByVal vModuleHandleOrName As Variant, ByVal sLibname As String, ByVal vFunction As Variant) As Boolean
    On Error GoTo Error_Handler
    Dim hModule As Long
    
    hModule = GetModuleHandle(vModuleHandleOrName, m_lProcessID)
    If hModule = 0 Then Exit Function
    
    If TypeName(vModuleHandleOrName) = "String" Then
        m_sImpModuleName = vModuleHandleOrName
    Else
        m_sImpModuleName = GetModuleFileName(hModule, m_lProcessID)
    End If
    If m_sImpModuleName = "" Then Exit Function
    
    m_lImpEntryAddress = 0
    m_lImpJumpAddress = 0
    
    Dim lpIMAGE_IMPORT_DESCRIPTOR As Long
    Dim lpLibName As Long
    Dim lpIMAGE_IMPORT_BY_NAME As Long
    Dim lpFunction As Long, lpTemp As Long, lpCounter As Long
        
    lpIMAGE_IMPORT_DESCRIPTOR = hModule + MemValue(hModule + MemValue(hModule + &H3C) + &H80)
    Do
        If MemValue(lpIMAGE_IMPORT_DESCRIPTOR) = 0 Then Exit Do
        
        lpLibName = hModule + MemValue(lpIMAGE_IMPORT_DESCRIPTOR + &HC)
        
        If UCase$(sLibname) = UCase$(RemoteStrFromPtr(lpLibName)) Then
            lpIMAGE_IMPORT_BY_NAME = hModule + MemValue(lpIMAGE_IMPORT_DESCRIPTOR)
            lpCounter = 0
            Do
                lpFunction = MemValue(lpIMAGE_IMPORT_BY_NAME + lpCounter * 4)
                If lpFunction = 0 Then Exit Do
                
                lpTemp = lpFunction + 2
                  
                If TypeName(vFunction) <> "String" Then
                    If lpTemp And &H80000000 Then  '�������
                        If (lpTemp And &H7FFFFFFF) = CLng(vFunction) Then Exit Do
                    End If
                Else
                    If (lpTemp And &H80000000) = 0 Then '�ַ����
                        If UCase$(RemoteStrFromPtr(hModule + lpTemp)) = UCase$(vFunction) Then Exit Do
                    End If
                End If
                lpCounter = lpCounter + 1
            Loop
            If lpFunction <> 0 Then
                lpIMAGE_IMPORT_BY_NAME = hModule + MemValue(lpIMAGE_IMPORT_DESCRIPTOR + &H10)
                m_lImpEntryAddress = lpIMAGE_IMPORT_BY_NAME + lpCounter * 4
                m_lImpJumpAddress = MemValue(m_lImpEntryAddress)
            End If
        End If
        lpIMAGE_IMPORT_DESCRIPTOR = lpIMAGE_IMPORT_DESCRIPTOR + &H14
    Loop
    LocateImportFunction = m_lImpEntryAddress <> 0
    Exit Function
Error_Handler:
    
End Function

'������뺯���б�
'Dim avImports() As Variant
'
'avImports(i,0)=���
'avImports(i,1)=�������
'avImports(i,2)=���뺯���������
'avImports(i,3)=�����RVA
'avImports(i,4)=���뺯�����RVA
Public Function GetImports(ByVal vModuleHandleOrName As Variant, avImports() As Variant) As Boolean
    On Error GoTo Error_Handler
    Dim hModule As Long
    
    hModule = GetModuleHandle(vModuleHandleOrName, m_lProcessID)
    If hModule = 0 Then Exit Function
    
    If TypeName(vModuleHandleOrName) = "String" Then
        m_sImpModuleName = vModuleHandleOrName
    Else
        m_sImpModuleName = GetModuleFileName(hModule, m_lProcessID)
    End If
    If m_sImpModuleName = "" Then Exit Function
    
    Dim lpIMAGE_IMPORT_DESCRIPTOR As Long
    Dim lpLibName As Long
    Dim lpIMAGE_IMPORT_BY_NAME As Long
    Dim lpFunction As Long, lpTemp As Long, lTemp2 As Long, lpCounter As Long
        
    Dim i As Long, sLibname As String
    Dim avTmp() As Variant
    lpCounter = 0
    
    lpIMAGE_IMPORT_DESCRIPTOR = hModule + MemValue(hModule + MemValue(hModule + &H3C) + &H80)
    Do
        If MemValue(lpIMAGE_IMPORT_DESCRIPTOR) = 0 Then Exit Do
        
        lpLibName = hModule + MemValue(lpIMAGE_IMPORT_DESCRIPTOR + &HC)
        
        sLibname = RemoteStrFromPtr(lpLibName)
            
        lpIMAGE_IMPORT_BY_NAME = hModule + MemValue(lpIMAGE_IMPORT_DESCRIPTOR)
        i = lpCounter
        Do
            ReDim Preserve avTmp(lpCounter * 5 + 4)
            avTmp(lpCounter * 5) = lpCounter + 1
            avTmp(lpCounter * 5 + 1) = sLibname
            
            lpFunction = MemValue(lpIMAGE_IMPORT_BY_NAME + (lpCounter - i) * 4)
            If lpFunction = 0 Then
                ReDim Preserve avTmp(lpCounter * 5 - 1)
                'lpCounter = lpCounter - 1
                Exit Do
            End If
            
            lpTemp = lpFunction + 2
              
            If lpTemp And &H80000000 Then  '�������
                avTmp(lpCounter * 5 + 2) = (lpTemp And &H7FFFFFFF)
            Else
                avTmp(lpCounter * 5 + 2) = RemoteStrFromPtr(hModule + lpTemp)
            End If
            
            If lpFunction <> 0 Then
                lTemp2 = hModule + MemValue(lpIMAGE_IMPORT_DESCRIPTOR + &H10)
                avTmp(lpCounter * 5 + 3) = lTemp2 + (lpCounter - i) * 4
                avTmp(lpCounter * 5 + 4) = MemValue(lTemp2 + (lpCounter - i) * 4)
            End If
            
            lpCounter = lpCounter + 1
        Loop
        
        
        lpIMAGE_IMPORT_DESCRIPTOR = lpIMAGE_IMPORT_DESCRIPTOR + &H14
    Loop
    
    ReDim avImports(lpCounter - 1, 4)
    
    For i = 0 To lpCounter - 1
        avImports(i, 0) = avTmp(i * 5 + 0)
        avImports(i, 1) = avTmp(i * 5 + 1)
        avImports(i, 2) = avTmp(i * 5 + 2)
        avImports(i, 3) = avTmp(i * 5 + 3)
        avImports(i, 4) = avTmp(i * 5 + 4)
    Next
    
    GetImports = True
    Exit Function
Error_Handler:
    
End Function
'�趨�������
Public Function SetExportFunctionAddress(ByVal lAddress As Long) As Boolean
    m_sExpModuleName = "UserDefinedTargetAddress"
    m_lExpEntryAddress = 0
    m_lExpJumpAddress = lAddress
    
    SetExportFunctionAddress = True
End Function

'��λ�������
Public Function LocateExportFunction(ByVal vModuleHandleOrName As Variant, ByVal vFunction As Variant) As Boolean
    On Error GoTo Error_Handler
    Dim hModule As Long
    
    hModule = GetModuleHandle(vModuleHandleOrName, m_lProcessID)
    If hModule = 0 Then Exit Function
    
    If TypeName(vModuleHandleOrName) = "String" Then
        m_sExpModuleName = vModuleHandleOrName
    Else
        m_sExpModuleName = GetModuleFileName(hModule, m_lProcessID)
    End If
    If m_sExpModuleName = "" Then Exit Function
    
    m_lExpEntryAddress = 0
    m_lExpJumpAddress = 0
    
    Dim lpEXPORT_TABLE As Long
    Dim lNumberOfNames As Long, lNumberOfFunctions As Long, lBase As Long
    Dim lpNamesTable As Long, lpFunctionsTable As Long, lpOrdinalsTable As Long
    Dim lpIMAGE_ExpORT_BY_NAME As Long
    Dim lpFunction As Long, lNameOrdinal As Long
        
    lpEXPORT_TABLE = hModule + MemValue(hModule + MemValue(hModule + &H3C) + &H78)
    lBase = MemValue(lpEXPORT_TABLE + &H10)
    lNumberOfFunctions = MemValue(lpEXPORT_TABLE + &H14)
    lNumberOfNames = MemValue(lpEXPORT_TABLE + &H18)
    lpFunctionsTable = hModule + MemValue(lpEXPORT_TABLE + &H1C)
    lpNamesTable = hModule + MemValue(lpEXPORT_TABLE + &H20)
    lpOrdinalsTable = hModule + MemValue(lpEXPORT_TABLE + &H24)
    
    If TypeName(vFunction) = "String" Then
        Do While lNumberOfNames > 0
            lNumberOfNames = lNumberOfNames - 1
            lpFunction = hModule + MemValue(lpNamesTable + lNumberOfNames * 4)
            lNameOrdinal = MemValue((lpOrdinalsTable + lNumberOfNames * 2), True)
            If UCase$(RemoteStrFromPtr(lpFunction)) = UCase$(vFunction) Then
                If lNameOrdinal >= lNumberOfFunctions Then Exit Do
                m_lExpEntryAddress = lpFunctionsTable + 4 * lNameOrdinal
                m_lExpJumpAddress = hModule + MemValue(m_lExpEntryAddress)
                Exit Do
            End If
        Loop
    Else
        If vFunction - lBase < lNumberOfFunctions Then
            m_lExpEntryAddress = lpFunctionsTable + 4 * (vFunction - lBase)
            m_lExpJumpAddress = hModule + MemValue(m_lExpEntryAddress)
        End If
    End If
    LocateExportFunction = m_lExpEntryAddress <> 0
    Exit Function
Error_Handler:
       
End Function

'�����������б�
'Dim avExports() As Variant
'
'avExports(i,0)=���
'avExports(i,1)=�����������
'avExports(i,2)=������
'avExports(i,3)=����������RVA
'avExports(i,4)=�������RVA
Public Function GetExports(ByVal vModuleHandleOrName As Variant, avExports() As Variant) As Boolean
    On Error GoTo Error_Handler
    Dim hModule As Long
    
    hModule = GetModuleHandle(vModuleHandleOrName, m_lProcessID)
    If hModule = 0 Then Exit Function
    
    If TypeName(vModuleHandleOrName) = "String" Then
        m_sExpModuleName = vModuleHandleOrName
    Else
        m_sExpModuleName = GetModuleFileName(hModule, m_lProcessID)
    End If
    If m_sExpModuleName = "" Then Exit Function
    
    Dim lpEXPORT_TABLE As Long
    Dim lNumberOfNames As Long, lNumberOfFunctions As Long, lBase As Long
    Dim lpNamesTable As Long, lpFunctionsTable As Long, lpOrdinalsTable As Long
    Dim lpIMAGE_ExpORT_BY_NAME As Long
    Dim lpFunction As Long, lNameOrdinal As Long
        
    lpEXPORT_TABLE = hModule + MemValue(hModule + MemValue(hModule + &H3C) + &H78)
    lBase = MemValue(lpEXPORT_TABLE + &H10)
    lNumberOfFunctions = MemValue(lpEXPORT_TABLE + &H14)
    lNumberOfNames = MemValue(lpEXPORT_TABLE + &H18)
    lpFunctionsTable = hModule + MemValue(lpEXPORT_TABLE + &H1C)
    lpNamesTable = hModule + MemValue(lpEXPORT_TABLE + &H20)
    lpOrdinalsTable = hModule + MemValue(lpEXPORT_TABLE + &H24)
    
    
    ReDim avExports(lNumberOfFunctions - 1, 4) As Variant
    Dim i As Long
    
    For i = 0 To lNumberOfFunctions - 1
        avExports(i, 0) = i
        avExports(i, 1) = i + lBase
        avExports(i, 3) = lpFunctionsTable + 4 * i
        avExports(i, 4) = MemValue(lpFunctionsTable + 4 * i) + hModule
        'avExports(i, 3) = lpFunctionsTable + 4 * i  - hModule
        'avExports(i, 4) = MemValue(lpFunctionsTable + 4 * i)
    Next
    
    Do While lNumberOfNames > 0
        lNumberOfNames = lNumberOfNames - 1
        lpFunction = hModule + MemValue(lpNamesTable + lNumberOfNames * 4)
        lNameOrdinal = MemValue((lpOrdinalsTable + lNumberOfNames * 2), True)
            
        If lNameOrdinal >= lNumberOfFunctions Then Exit Do
        
        avExports(lNameOrdinal, 2) = RemoteStrFromPtr(lpFunction)
    Loop
    
    GetExports = True
    Exit Function
Error_Handler:
       
End Function

'��ʼ������
Public Function SetTargetTo(Optional ByVal pid As Long = -1) As Boolean
    m_lProcessID = IIf(pid = -1, GetCurrentProcessId, pid)
    m_lhProcess = OpenProcess(PROCESS_ALL_ACCESS, 0&, m_lProcessID)
    SetTargetTo = (m_lhProcess <> 0)
End Function

'�����������ڴ����ݸ��Ƶ��������ڴ�
Public Function Pmemcpy2local(ByVal lpDestAddr As Long, ByVal lpSrcAddr As Long, ByVal lLen As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
    Dim lBytesReadWrite As Long
    
    If m_lhProcess Then
        If VirtualProtectEx(m_lhProcess, ByVal lpSrcAddr, lLen, PAGE_READWRITE, lOldProtect) <> 0 Then
            ReadProcessMemory m_lhProcess, ByVal lpSrcAddr, ByVal lpDestAddr, lLen, lBytesReadWrite
            Call VirtualProtectEx(m_lhProcess, ByVal lpSrcAddr, lLen, lOldProtect, lOldProtect)
            Pmemcpy2local = (lBytesReadWrite <> 0)
        End If
    End If
    Exit Function
Error_Handler:
End Function

'�ѱ����ڴ����ݸ��Ƶ������������ڴ�
Public Function Pmemcpy2remote(ByVal lpDestAddr As Long, ByVal lpSrcAddr As Long, ByVal lLen As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
    Dim lBytesReadWrite As Long
    
    If m_lhProcess Then
        If VirtualProtectEx(m_lhProcess, ByVal lpDestAddr, lLen, PAGE_READWRITE, lOldProtect) <> 0 Then
            WriteProcessMemory m_lhProcess, ByVal lpDestAddr, ByVal lpSrcAddr, lLen, lBytesReadWrite
            Call VirtualProtectEx(m_lhProcess, ByVal lpDestAddr, lLen, lOldProtect, lOldProtect)
            Pmemcpy2remote = (lBytesReadWrite <> 0)
        End If
    End If
    Exit Function
Error_Handler:
End Function
'=======================================================================================================================
'����Ϊ�๫������
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ�ڲ�����
'=======================================================================================================================
'��ָ���ȡ�ַ���
Private Function RemoteStrFromPtr(ByVal lpString As Long) As String
    On Error GoTo Error_Handler
    Dim b(254) As Byte, b2() As Byte, lStrLen As Long, lpBuffer As Long
    Dim lPosOfZero As Long
    
    Dim lOldProtect As Long
    Dim lBytesReadWrite As Long
    Dim lAddress As Long
    
    lStrLen = 0
    
    lpBuffer = VarPtr(b(0))
    
    If m_lhProcess Then
            ReDim Preserve b2(254)
            Do
                lAddress = lpString + lStrLen * 255
                If VirtualProtectEx(m_lhProcess, ByVal lAddress, 255, PAGE_READWRITE, lOldProtect) <> 0 Then
                    ReadProcessMemory m_lhProcess, ByVal lAddress, ByVal lpBuffer, 255, lBytesReadWrite
                    Call VirtualProtectEx(m_lhProcess, ByVal lAddress, 255, lOldProtect, lOldProtect)
                    CopyMemory b2(lStrLen * 255), ByVal lpBuffer, 255
                Else
                    Exit Do
                End If
                
                lPosOfZero = InStrB(b, ChrB(0))
                If lPosOfZero <> 0 Then Exit Do
                lStrLen = lStrLen + 1
                ReDim Preserve b2((lStrLen + 1) * 255 - 1)
            Loop
            
            lStrLen = 255 * lStrLen + lPosOfZero - 1
            
            If lStrLen >= 1 Then
                ReDim Preserve b2(lStrLen - 1)
                RemoteStrFromPtr = StrConv(b2, vbUnicode)
            End If
    End If
    Exit Function
Error_Handler:
End Function

'�����������ڴ����ݸ��Ƶ��������ڴ�
Private Function memcpy2local(ByVal lpDestAddr As Long, ByVal lpSrcAddr As Long, ByVal lLen As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
    Dim lBytesReadWrite As Long
    
    If m_lhProcess Then
        If VirtualProtectEx(m_lhProcess, ByVal lpSrcAddr, lLen, PAGE_READWRITE, lOldProtect) <> 0 Then
            ReadProcessMemory m_lhProcess, ByVal lpSrcAddr, ByVal lpDestAddr, lLen, lBytesReadWrite
            Call VirtualProtectEx(m_lhProcess, ByVal lpSrcAddr, lLen, lOldProtect, lOldProtect)
            memcpy2local = (lBytesReadWrite <> 0)
        End If
    End If
    Exit Function
Error_Handler:
End Function

'�ѱ����ڴ����ݸ��Ƶ������������ڴ�
Private Function memcpy2remote(ByVal lpDestAddr As Long, ByVal lpSrcAddr As Long, ByVal lLen As Long) As Boolean
    On Error GoTo Error_Handler
    Dim lOldProtect As Long
    Dim lBytesReadWrite As Long
    
    If m_lhProcess Then
        If VirtualProtectEx(m_lhProcess, ByVal lpDestAddr, lLen, PAGE_READWRITE, lOldProtect) <> 0 Then
            WriteProcessMemory m_lhProcess, ByVal lpDestAddr, ByVal lpSrcAddr, lLen, lBytesReadWrite
            Call VirtualProtectEx(m_lhProcess, ByVal lpDestAddr, lLen, lOldProtect, lOldProtect)
            memcpy2remote = (lBytesReadWrite <> 0)
        End If
    End If
    Exit Function
Error_Handler:
End Function

'��ȡָ���ڴ�4�ֽڻ�2�ֽ�����
Private Function MemValue(ByVal lAddress As Long, Optional ByVal fWORD As Boolean = False) As Long
    memcpy2local VarPtr(MemValue), lAddress, IIf(fWORD, 2, 4)
End Function

'����ģ������ȡģ������
Private Function GetModuleFileName(ByVal hModule As Long, ByVal pid As Long) As String
    On Error GoTo Error_Handler
    Dim hSnapShot As Long, t As MODULEENTRY32
    Dim lStrLen As Long
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid)
    t.dwSize = LenB(t)
    If hSnapShot <> -1 Then
        If Module32First(hSnapShot, t) Then
            Do
                If t.hModule = hModule Then Exit Do
            Loop While Module32Next(hSnapShot, t)
            GetModuleFileName = t.szExePath
            lStrLen = InStr(GetModuleFileName, Chr(0)) - 1
            If lStrLen > 0 Then
                GetModuleFileName = GetFileName(Left$(GetModuleFileName, lStrLen))
            End If
        End If
        CloseHandle hSnapShot
    End If
    Exit Function
Error_Handler:
End Function

'����ģ�����ƻ�ȡģ����
Private Function GetModuleHandle(ByVal vModuleHandleOrName As Variant, ByVal pid As Long) As Long
    On Error GoTo Error_Handler
    Dim hSnapShot As Long, t As MODULEENTRY32
    Dim lStrLen As Long, s As String, sModuleName As String
    
    If TypeName(vModuleHandleOrName) <> "String" Then
        GetModuleHandle = vModuleHandleOrName
        Exit Function
    Else
        sModuleName = vModuleHandleOrName
    End If
    
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid)
    t.dwSize = LenB(t)
    If hSnapShot <> -1 Then
        If Module32First(hSnapShot, t) Then
            Do
                s = t.szExePath
                lStrLen = InStr(s, Chr(0)) - 1
                If lStrLen > 0 Then
                    s = Left$(s, lStrLen)
                End If
                
                If UCase$(GetFileName(s)) = UCase$(sModuleName) Then
                    GetModuleHandle = t.hModule
                    Exit Do
                End If
            Loop While Module32Next(hSnapShot, t)
            
        End If
        CloseHandle hSnapShot
    End If
    Exit Function
Error_Handler:
End Function

'���ļ�·���ַ����з�����ļ���
Private Function GetFileName(ByVal sFilePath As String) As String
    On Error GoTo Error_Handler
    Dim lPosOfBackSlash As Long
    Dim lStrLen As Long
    lPosOfBackSlash = InStrRev(sFilePath, "\")
    lStrLen = Len(sFilePath)
    
    If lStrLen > lPosOfBackSlash Then
        GetFileName = Right$(sFilePath, lStrLen - lPosOfBackSlash)
    End If
    Exit Function
Error_Handler:
End Function
'=======================================================================================================================
'����Ϊ�ڲ�����
'=======================================================================================================================

'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

'=======================================================================================================================
'����Ϊ���ʼ�������ٹ���
'=======================================================================================================================
'���ʼ��
Private Sub Class_Initialize()
    m_lImpEntryAddress = 0
    m_lImpJumpAddress = 0
    m_sImpModuleName = ""
    m_lExpEntryAddress = 0
    m_lExpJumpAddress = 0
    m_sExpModuleName = ""
    
    m_fImpReplaced = False
    m_fExpReplaced = False
    m_fAutoRestore = True
    
    'Ĭ���趨Ŀ�����Ϊ��ǰ����
    Call SetTargetTo(-1)
End Sub

'������
Private Sub Class_Terminate()
    If m_fAutoRestore Then
        '�ָ����滻��ַ
        Call ImpRestore
        Call ExpRestore
    End If
    
    '�رմ򿪵Ľ��̾��
    CloseHandle m_lhProcess
End Sub
'=======================================================================================================================
'����Ϊ���ʼ�������ٹ���
'=======================================================================================================================

