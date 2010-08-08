VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    Dim oMemAllocator As CVBMemoryAllocator
    Dim hMem As Long, hMem2 As Long
    
    Set oMemAllocator = New CVBMemoryAllocator
    
    With oMemAllocator
        MsgBox "分配并立即释放一个内存块"
        Call .Release(.Allocate(1024 * 1024& * 10&, hMem))
        
        
        MsgBox "分配一个内存块。在类销毁时该内存块会自动被释放"
        Call .Allocate(1024& * 1024& * 100, hMem2)
    End With
    
    Set oMemAllocator = Nothing
End Sub
