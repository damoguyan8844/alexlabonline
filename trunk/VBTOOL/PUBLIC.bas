Attribute VB_Name = "PUBLIC"
Option Explicit

Public Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" ( _
    ByVal hModule As Long, _
    ByVal lpFileName As String, _
    ByVal nSize As Long) As Long
Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" ( _
    ByVal lpApplicationName As String, _
    ByVal lpKeyName As Any, _
    ByVal lpDefault As String, _
    ByVal lpReturnedString As String, _
    ByVal nSize As Long, _
    ByVal lpFileName As String) As Long

Public Declare Sub InitSheet Lib "MDOperator.dll" (ByVal shtName As String, ByVal regular As String, ByRef values As Variant)
Public Declare Sub ReleaseSheet Lib "MDOperator.dll" (ByVal shtName As String)
Public Declare Sub SetSheetAccessProc Lib "MDOperator.dll" (ByVal lppProc As Long)

Public Declare Function CheckRegular Lib "MDOperator.dll" (ByVal regular As String) As Boolean
Public Declare Function MatchSheet Lib "MDOperator.dll" (ByVal shtName As String) As Boolean
Public Declare Function CheckCell Lib "MDOperator.dll" (ByVal shtName As String, ByVal row As Long, ByVal col As Long) As Boolean

Public MKDSheets As New Collection
Public MKDCurrentSheet As Spreadsheet


Public Function SheetAccess(ByVal row As Long, ByVal col As Long, ByVal sheetName As String) As String
     
     Dim sheet As Spreadsheet
     Set sheet = GetMarketSheet(sheetName)
 
     If Not sheet Is Nothing Then

        SheetAccess = sheet.Sheets(1).Cells(row, col).value
     Else
        SheetAccess = sheetName & "not found!"
     End If
    
End Function
Public Function MDKCurrentSheetAccess(ByVal row As Long, ByVal col As Long, ByVal sheetName As String) As String
    
    If Not MKDCurrentSheet Is Nothing Then
        MDKCurrentSheetAccess = MKDCurrentSheet.Sheets(1).Cells(row, col).value
     Else
        MDKCurrentSheetAccess = "current sheet is null!"
    End If
End Function
Public Function GetMarketSheet(sheetName As String) As Spreadsheet
On Error GoTo eh:
     Dim sheet As Spreadsheet
     sheet = MKDSheets.Item(sheetName)
     Set GetMarketSheet = sheet
     Exit Function
eh:
    Set GetMarketSheet = Nothing
End Function


Public Function GetFuncPtr(ByVal Ptr As Long) As Long
  GetFuncPtr = Ptr
End Function

