VERSION 5.00
Begin VB.Form frmCommand 
   Caption         =   "Net Command"
   ClientHeight    =   4395
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10710
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   4395
   ScaleWidth      =   10710
   WindowState     =   2  'Maximized
   Begin VB.TextBox Text1 
      Height          =   2775
      Left            =   1320
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   1320
      Width           =   5535
   End
   Begin VB.TextBox txtIP 
      Height          =   375
      Left            =   5160
      TabIndex        =   3
      Text            =   "192.168.0.174"
      Top             =   360
      Width           =   1695
   End
   Begin VB.TextBox txtMessageID 
      Height          =   375
      Left            =   1320
      TabIndex        =   0
      Text            =   "0"
      Top             =   360
      Width           =   1575
   End
   Begin VB.Label Label2 
      Caption         =   "MQServer:"
      Height          =   375
      Left            =   3960
      TabIndex        =   2
      Top             =   360
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "Message ID:"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   1095
   End
End
Attribute VB_Name = "frmCommand"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
