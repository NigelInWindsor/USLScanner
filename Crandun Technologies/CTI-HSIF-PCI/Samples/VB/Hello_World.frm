VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "CTI-HSIF Software Library"
   ClientHeight    =   4635
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4635
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdStop 
      BackColor       =   &H008080FF&
      Caption         =   "Stop Sampling"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   2640
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   120
      Width           =   1695
   End
   Begin VB.CommandButton cmdStart 
      BackColor       =   &H0000FF00&
      Caption         =   "Start Sampling"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   240
      Style           =   1  'Graphical
      TabIndex        =   1
      Top             =   120
      Width           =   1815
   End
   Begin VB.ListBox ListRange 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "#,##0.00"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   3000
      Left            =   120
      TabIndex        =   0
      Top             =   1200
      Width           =   4455
   End
   Begin VB.Label lblTotSamples 
      BackColor       =   &H0000FFFF&
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2400
      TabIndex        =   4
      Top             =   840
      Width           =   1695
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      BackColor       =   &H0000FFFF&
      Caption         =   "Samples taken: "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   480
      TabIndex        =   3
      Top             =   840
      Width           =   1935
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Base 1
Option Explicit
Private mySensor As CTI_HSIF_PCI
Private bExitFlag As Boolean
Private Sub cmdStart_Click()
   Set mySensor = New CTI_HSIF_PCI
   Dim rc As Long
     
   rc = mySensor.setDriverOpen()
   If rc <> CTI_SUCCESS Then
      MsgBox "ERROR: setDriverOpen returned " + Str(rc), vbCritical
      Exit Sub
   End If
      
   rc = mySensor.setCalibrationFile("C:\LOOKUPHS")
   If rc <> CTI_SUCCESS Then
      MsgBox "ERROR: setCalibrationFile returned " + Str(rc), vbCritical
      Exit Sub
   End If
   
   rc = mySensor.setCommOpen("COM1", 9600)
   If rc <> CTI_SUCCESS Then
      MsgBox "ERROR: setCommOpen returned " + Str(rc), vbCritical
      Exit Sub
   End If
   MsgBox "Successfully opened the serial port. Starting sampling.", vbInformation
   
   Dim rangeData(2000) As HSIF_DATA_PT
   Dim totSamples As Long
   Dim i As Integer
   
   bExitFlag = False
      
   rc = mySensor.setClearBuffer()      ' Always returns CTI_SUCCESS
      
   totSamples = 0
   Do
      rc = mySensor.getSamples(rangeData, 1, 10000)
      If rc < 0 Then
         MsgBox "ERROR: getSamples returned " + Str(rc), vbCritical
         bExitFlag = True
      Else
         For i = 1 To rc
            ListRange.AddItem (rangeData(i).R_X)
            ListRange.ListIndex = ListRange.ListCount - 1
            totSamples = totSamples + 1
            lblTotSamples.Caption = Str(totSamples)
            DoEvents
            If (bExitFlag = True) Then Exit Do
         Next i
      End If
   Loop While (bExitFlag = False)

   rc = mySensor.setCommClosed()
   If rc <> CTI_SUCCESS Then
      MsgBox "ERROR: setCommClosed returned " + Str(rc), vbCritical
   Else
      MsgBox "Closed the serial connection.", vbInformation
   End If
      
End Sub

Private Sub cmdStop_Click()
   bExitFlag = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
   bExitFlag = True
End Sub

