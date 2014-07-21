VERSION 5.00
Begin VB.Form main 
   Caption         =   "Readmouse"
   ClientHeight    =   6765
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7620
   LinkTopic       =   "Form1"
   ScaleHeight     =   6765
   ScaleWidth      =   7620
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox scanmode 
      Caption         =   "Scan mode"
      Height          =   375
      Left            =   4560
      TabIndex        =   5
      Top             =   120
      Width           =   1455
   End
   Begin VB.CommandButton gobutton 
      Caption         =   "Go"
      Height          =   255
      Left            =   2400
      TabIndex        =   3
      Top             =   120
      Width           =   615
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   50
      Left            =   3960
      Top             =   120
   End
   Begin VB.PictureBox pic 
      Height          =   6135
      Left            =   120
      ScaleHeight     =   16
      ScaleMode       =   0  'User
      ScaleWidth      =   16
      TabIndex        =   2
      Top             =   600
      Width           =   7455
   End
   Begin VB.ComboBox portsel 
      Height          =   315
      ItemData        =   "main.frx":0000
      Left            =   1080
      List            =   "main.frx":000A
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label dxdy 
      Caption         =   "dxdy"
      Height          =   255
      Left            =   3240
      TabIndex        =   4
      Top             =   120
      Width           =   615
   End
   Begin VB.Label Label1 
      Caption         =   "Port:"
      Height          =   255
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   855
   End
End
Attribute VB_Name = "main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim px, py As Integer
'20=recv
'80=clk
'8=dat

Private Sub Form_Load()
portsel.ListIndex = 0
End Sub

Private Sub spisend(a As Integer, d As Integer)
Dim p, n, x As Integer
p = Val(portsel.Text)
a = a Or &H80
For x = 7 To 0 Step -1
    If (a And (2 ^ x)) <> 0 Then n = 8 Else n = 0
    Out p, n
    Out p, n + &H80
Next
For x = 7 To 0 Step -1
    If (d And (2 ^ x)) <> 0 Then n = 8 Else n = 0
    Out p, n
    Out p, n + &H80
Next
End Sub

Private Function spirecv(a As Integer)
Dim p, n, d, x As Integer
Dim t As Single
p = Val(portsel.Text)
a = a And &H7F
For x = 7 To 0 Step -1
    If (a And (2 ^ x)) <> 0 Then n = 8 Else n = 0
    Out p, n
    Out p, n + &H80
Next
Out p, n + &H80
Out p, n + &H80
n = 8: d = 0
For x = 7 To 0 Step -1
    Out p, n
    Out p, n + &H80
   ' t! = Timer + 0.0001: While t! > Timer: DoEvents: Wend
    d = d * 2
    If (Inp(p + 1) And &H20) <> 0 Then d = d + 1
Next
spirecv = d
End Function

Private Sub gobutton_Click()
If Timer1.Enabled Then
    Timer1.Enabled = False
    gobutton.Caption = "Go"
'    spisend 0, &H80
Else
    gobutton.Caption = "Stop"
    spisend 0, &H1
    Timer1.Enabled = True
End If

End Sub

Private Sub scanmode_Click()
If scanmode.Value = 1 Then
    pic.ScaleMode = 3
    px = pic.ScaleWidth / 2
    py = pic.ScaleHeight / 2
Else
    pic.ScaleHeight = 18
    pic.ScaleWidth = 18
    px = 0
    py = 0
End If
pic.Cls
End Sub

Private Sub adjdxdy()
Dim dx, dy As Integer
dy = spirecv(2): If dy > 127 Then dy = dy - 256
dx = spirecv(3): If dx > 127 Then dx = dx - 256
px = px + (dx * 1.1)
py = py - (dy * 1.1)
If py < 0 Then py = 0
If px < 0 Then px = 0
If py > 1000 Then py = 1000
If px > 1000 Then px = 1000
End Sub

Private Sub Timer1_Timer()
Dim x, y, p, tout As Integer
spisend 8, 0
For x = 0 To 17
    If scanmode.Value = 1 Then adjdxdy
    For y = 17 To 0 Step -1
        tout = 10
        Do
            p = spirecv(8)
            tout = tout - 1
        Loop While ((p And &H40) = 0) And (tout > 0)
        p = (p And 63) * 4
        pic.Line (x + px, y + py)-(x + px + 1, y + py + 1), RGB(p, p, p), BF
    Next
Next
End Sub
