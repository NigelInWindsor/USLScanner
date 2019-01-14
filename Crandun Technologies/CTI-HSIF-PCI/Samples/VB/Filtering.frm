VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "CTI-HSIF-PCI Software Library"
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
   Begin VB.CommandButton cmdStart 
      BackColor       =   &H0000FF00&
      Caption         =   "Start"
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
      Left            =   1440
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
      TabIndex        =   3
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
      TabIndex        =   2
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

Private Sub cmdStart_Click()
   Set mySensor = New CTI_HSIF_PCI
   Dim rc As Long
   Dim numSamples As Long
   
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
   MsgBox "Successfully opened the serial port.", vbInformation
      
   MsgBox "Setting sample rate.", vbInformation
   rc = mySensor.setSamplesPerSec(500)
   If (rc < 0) Then MsgBox "setSamplesPerSec returned error code " & Str(rc), vbCritical
      
   MsgBox "Setting minimum and maximum valid ranges 24 and 48 inches", vbInformation
   rc = mySensor.setMinValidRange(24)
   If (rc <> CTI_SUCCESS) Then MsgBox "setMinValidRange returned error code " & Str(rc), vbCritical
   
   rc = mySensor.setMaxValidRange(48)
   If (rc <> CTI_SUCCESS) Then MsgBox "setMaxValidRange returned error code " & Str(rc), vbCritical
   
   MsgBox "Telling library to discard samples outside of the valid range", vbInformation
   rc = mySensor.setDiscardInvalidOn()
   If (rc <> CTI_SUCCESS) Then MsgBox "setDiscardInvalidOn returned error code " & Str(rc), vbCritical
   
   MsgBox "Approx. 10 seconds of data will now be captured." & vbCrLf & _
   "Please move the sensor slowly, so that it reflects off a wide range of targets" & vbCrLf & _
   "from less than 24 inches in distance to more than 48 inches.", vbInformation
     
   rc = mySensor.setClearBuffer()   ' don't check return code - always returns CTI_SUCCESS
   
   ' Sensor is at 500 samples/second, so maximum number of samples over 10 seconds is
   ' about 5000 samples.  Some should be filtered out, so we should get less than that
   Dim startTime As Single
   startTime = Timer
   Do While Timer < startTime + 10
      DoEvents   ' Yield to other processes.
   Loop

   Dim rangeData(10000) As HSIF_DATA_PT
   Dim totSamples As Long
   Dim i As Integer

   numSamples = mySensor.getSamples(rangeData, 1, 10000)
   If (numSamples < 0) Then
         MsgBox "ERROR: getSamples for sensor returned error " + Str(numSamples), vbCritical
         rc = mySensor.setCommClosed()
         Exit Sub
   End If
      
   rc = mySensor.setCommClosed()
   If rc <> CTI_SUCCESS Then
      MsgBox "ERROR: setCommClosed returned " + Str(rc), vbCritical
   Else
      MsgBox "Closed the serial connection.", vbInformation
   End If
      
   MsgBox "Received " & Str(numSamples) & " samples.", vbInformation
   lblTotSamples.Caption = Str(numSamples)
   'List the samples returned
   For i = 1 To numSamples
      ListRange.AddItem (rangeData(i).R_X)
      ListRange.ListIndex = ListRange.ListCount - 1
      DoEvents
   Next i

End Sub



