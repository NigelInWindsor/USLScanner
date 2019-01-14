// SerialJoystickDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SerialJoystickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerialJoystickDlg dialog
IMPLEMENT_DYNCREATE(CSerialJoystickDlg, CPropertyPage)


CSerialJoystickDlg::CSerialJoystickDlg() : CPropertyPage(CSerialJoystickDlg::IDD)
{
	//{{AFX_DATA_INIT(CSerialJoystickDlg)
	m_nJoyPtr = 0;
	m_stringElapsedTime = _T("");
	m_stringPosCounter = _T("");
	m_editJoyArray = _T("");
	m_nTAxis = 0;
	//}}AFX_DATA_INIT

}


void CSerialJoystickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialJoystickDlg)
	DDX_Control(pDX, IDC_S_AXIS_EDIT, m_editSAxis);
	DDX_Control(pDX, IDC_Z_AXIS_EDIT, m_editZAxis);
	DDX_Control(pDX, IDC_Y_AXIS_EDIT, m_editYAxis);
	DDX_Control(pDX, IDC_X_AXIS_EDIT, m_editXAxis);
	DDX_Control(pDX, IDC_EDIT_FAULTY_DATA_PKTS, m_editFaultyDataPkts);
	DDX_Control(pDX, IDC_EDIT_DEADBAND_SIZE, m_editDeadBandSize);
	DDX_Control(pDX, IDC_EDIT_DOCKED_BIT, m_editDockedBit);
	DDX_Control(pDX, IDC_EDIT_LEFT_SIDE, m_editLeftSide);
	DDX_Control(pDX, IDC_EDIT_RIGHT_SIDE, m_editRightSide);
	DDX_Control(pDX, IDC_COMBO_JOYSTICK_TYPE, m_comboJoystickSource);
	DDX_Control(pDX, IDC_EDIT_MAIN_MANIP, m_editMainManip);
	DDX_Control(pDX, IDC_EDIT_DEADMAN_1, m_editDeadMan1);
	DDX_Control(pDX, IDC_EDIT_DEADMAN_0, m_editDeadMan0);
	DDX_Control(pDX, IDC_SWITCHES_EDIT, m_editSwitches);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editSpeed);
	DDX_Control(pDX, IDC_EDIT_SCANNERFIXTURE, m_editScannerFixture);
	DDX_Control(pDX, IDC_COMBO_COMM_PORT, m_comboCommPort);
	DDX_Text(pDX, IDC_JOY_PTR_EDIT, m_nJoyPtr);
	DDX_Text(pDX, IDC_EDIT_ELAPSED_TIME, m_stringElapsedTime);
	DDX_Text(pDX, IDC_EDIT_POS_COUNTER, m_stringPosCounter);
	DDX_Text(pDX, IDC_JOY_ARRAY_EDIT, m_editJoyArray);
	DDX_Text(pDX, IDC_T_AXIS_EDIT, m_nTAxis);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_FRAME_TOOL, m_editFrameToolJoyBit);
	DDX_Control(pDX, IDC_EDIT_FIR, m_editFIR);
}


BEGIN_MESSAGE_MAP(CSerialJoystickDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CSerialJoystickDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_PORT, OnSelchangeComboCommPort)
	ON_EN_CHANGE(IDC_EDIT_SCANNERFIXTURE, OnChangeEditScannerfixture)
	ON_EN_CHANGE(IDC_EDIT_SPEED, OnChangeEditSpeed)
	ON_EN_CHANGE(IDC_EDIT_DEADMAN_0, OnChangeEditDeadman0)
	ON_EN_CHANGE(IDC_EDIT_DEADMAN_1, OnChangeEditDeadman1)
	ON_EN_CHANGE(IDC_EDIT_MAIN_MANIP, OnChangeEditMainManip)
	ON_CBN_SELCHANGE(IDC_COMBO_JOYSTICK_TYPE, OnSelchangeComboJoystickType)
	ON_EN_CHANGE(IDC_EDIT_LEFT_SIDE, OnChangeEditLeftSide)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_SIDE, OnChangeEditRightSide)
	ON_EN_CHANGE(IDC_EDIT_DOCKED_BIT, OnChangeEditDockedBit)
	ON_EN_CHANGE(IDC_EDIT_DEADBAND_SIZE, OnChangeEditDeadbandSize)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_FRAME_TOOL, &CSerialJoystickDlg::OnEnChangeEditFrameTool)
	ON_EN_CHANGE(IDC_EDIT_FIR, &CSerialJoystickDlg::OnEnChangeEditFir)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialJoystickDlg message handlers


BOOL CSerialJoystickDlg::OnInitDialog() 
{
	CString	Buff;
	CPropertyPage::OnInitDialog();
	
	for (int ii = 1; ii <= 10; ii++) {
		Buff.Format(L"Comm%d", ii);
		m_comboCommPort.AddString(Buff);
	}

	m_comboJoystickSource.AddString(_T("MSC"));
	m_comboJoystickSource.AddString(_T("SI6"));
	m_comboJoystickSource.AddString(_T("Wired serial"));
	m_comboJoystickSource.AddString(_T("Wireless serial"));
	m_comboJoystickSource.AddString(_T("Si10"));
	m_comboJoystickSource.SetCurSel(theApp.m_Tank.nJoystickSource);

	UpdateAllControls();
	SetTimer(1,100,NULL);	

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSerialJoystickDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CSerialJoystickDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%02x h"),theApp.m_Tank.nDeadManBit0);
	m_editDeadMan0.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nDeadManBit1);
	m_editDeadMan1.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nSpeedBit);
	m_editSpeed.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nScannerFixtureBit);
	m_editScannerFixture.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nMainManipBit);
	m_editMainManip.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nLeftSideBit);
	m_editLeftSide.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nRightSideBit);
	m_editRightSide.SetWindowText(Buff);
	Buff.Format(_T("%02x h"), theApp.m_Tank.nDockedBit);
	m_editDockedBit.SetWindowText(Buff);
	Buff.Format(_T("%02x h"), theApp.m_Tank.nFrameToolJoyMode);
	m_editFrameToolJoyBit.SetWindowText(Buff);

	m_comboCommPort.SetCurSel(theApp.m_LastSettings.nJoystickCommPort);
	m_comboJoystickSource.SetCurSel((int)theApp.m_Tank.nJoystickSource);

	Buff.Format(L"%d",theApp.m_Tank.nJoystickDeadBand);
	m_editDeadBandSize.SetWindowText(Buff);

	Buff.Format(L"%d", theApp.m_LastSettings.nSerialPortFIRCount);
	m_editFIR.SetWindowTextW(Buff);

}

void CSerialJoystickDlg::OnTimer(UINT nIDEvent) 
{
	static int nOldMaxPtr=-1;
	CString Buff;
	CString Line;
	WCHAR	str[200];
	int	ii;
	static int	nStart,nStop;
	static int nPosCount;
	static int nOldFaultyDataPkts = -1;
/*
	if((nStart-theApp.nStartTime) || (nStop-theApp.nStopTime)) {
		m_stringElapsedTime.Format(_T("%d"),theApp.nStopTime-theApp.nStartTime);
		nStart=theApp.nStartTime;
		nStop=theApp.nStopTime;
	}
*/
	/*
	if(nPosCount-theApp.m_ServoUser.m_Pmac.nPosCount) {
		nPosCount=theApp.m_ServoUser.m_Pmac.nPosCount;
		m_stringPosCounter.Format(_T("%d"),nPosCount);
	}
*/
	m_nJoyPtr=theApp.m_SerialJoystick.rx_ptr;

	str[0]=0;
	for(ii=0;ii<16;ii++) {
		swprintf_s(str,_T("%s %02x\r\n"),str,theApp.m_SerialJoystick.JoystickArray[ii]);
	};
	m_editJoyArray.Format(_T("%s"),str);

	Buff.Format(L"%d %d %d", theApp.m_Motors.m_nJoyValue[0], theApp.m_SerialJoystick.m_JoyMin[0], theApp.m_SerialJoystick.m_JoyMax[0]);	m_editXAxis.SetWindowText(Buff);
	Buff.Format(L"%d %d %d", theApp.m_Motors.m_nJoyValue[1], theApp.m_SerialJoystick.m_JoyMin[1], theApp.m_SerialJoystick.m_JoyMax[1]);	m_editYAxis.SetWindowText(Buff);
	Buff.Format(L"%d %d %d", theApp.m_Motors.m_nJoyValue[2], theApp.m_SerialJoystick.m_JoyMin[2], theApp.m_SerialJoystick.m_JoyMax[2]);	m_editZAxis.SetWindowText(Buff);
	Buff.Format(L"%d %d %d", theApp.m_Motors.m_nJoyValue[3], theApp.m_SerialJoystick.m_JoyMin[3], theApp.m_SerialJoystick.m_JoyMax[3]);	m_editSAxis.SetWindowText(Buff);
	m_nTAxis = theApp.m_SerialJoystick.m_nTAxis;

	Buff.Format(_T("%dd = %02xh"),theApp.m_SerialJoystick.m_nSwitches,theApp.m_SerialJoystick.m_nSwitches);
	m_editSwitches.SetWindowText(Buff);

	if(nOldMaxPtr-theApp.m_SerialJoystick.rx_ptr) {
		nOldMaxPtr=theApp.m_SerialJoystick.rx_ptr;
		Buff.Empty();

//		for(int nLine=0;nLine<20;nLine++) {
//			theApp.m_SerialJoystick.GetRxBuffer(&Line,nLine*16,16);
//			Buff += Line;
//			Buff += "\r\n";
//		}
//		m_editRxArray.SetWindowText(Buff);
	}


	if(nOldFaultyDataPkts - theApp.m_SerialJoystick.m_nFaultyDataPkts) {
		Buff.Format(L"%d",nOldFaultyDataPkts = theApp.m_SerialJoystick.m_nFaultyDataPkts);
		m_editFaultyDataPkts.SetWindowText(Buff);
	}

	
	UpdateData(FALSE);	
	CDialog::OnTimer(nIDEvent);
}


void CSerialJoystick::Start()
{
	CString Buff;

	if (theApp.m_LastSettings.nJoystickCommPort >= 0 && theApp.m_LastSettings.nJoystickCommPort <= 10) {
		Buff.Format(L"Com%d", theApp.m_LastSettings.nJoystickCommPort + 1);
		Initialize((LPCWSTR)Buff);
	} else {
		MessageBox(NULL,_T("Incorrect Serial Port Number"),_T("Error"),NULL);
	}
}


BOOL CSerialJoystick::Initialize(LPCWSTR lpszPortName)
{
  DWORD dwError;
//DWORD dwThreadID;
  DCB PortDCB;
  COMMTIMEOUTS CommTimeouts;
	m_bThreadEnabled = FALSE;

  wcscpy_s(m_PortName, lpszPortName);

  // Open the serial port.
  hPort = CreateFile (lpszPortName, // Pointer to the name of the port
                      GENERIC_READ | GENERIC_WRITE,
                                    // Access (read/write) mode
                      0,            // Share mode
                      NULL,         // Pointer to the security attribute
                      OPEN_EXISTING,// How to open the serial port
                      FILE_FLAG_OVERLAPPED,			//FILE_FLAG_OVERLAPPED,            // Port attributes
                      NULL);        // Handle to port with attribute
                                    // to copy

  // If it fails to open the port, return FALSE.
  if ( hPort == INVALID_HANDLE_VALUE ) 
  {
    // Could not open the port.
//    MessageBox (hMainWnd, TEXT("Unable to open the port"), 
//                TEXT("Error"), MB_OK);
//	char Buff[30];
//	sprintf(Buff,"Invalid Handle for port: %s"), m_PortName);
  // 	MessageBox(NULL, Buff, "Error"), MB_OK);
    dwError = GetLastError ();
    return FALSE;
  }

  PortDCB.DCBlength = sizeof (DCB);     

  // Get the default port setting information.
  GetCommState (hPort, &PortDCB);

  // Change the DCB structure settings.
  PortDCB.BaudRate = 2400;              // Current baud 
  PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
  PortDCB.fParity = TRUE;               // Enable parity checking. 
  PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
  PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
  PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
                                        // DTR flow control type 
  PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
  PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
  PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
  PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
  PortDCB.fErrorChar = FALSE;           // Disable error replacement. 
  PortDCB.fNull = FALSE;                // Disable null stripping. 
  PortDCB.fRtsControl = RTS_CONTROL_ENABLE; 
                                        // RTS flow control 
  PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                        // error.
  PortDCB.ByteSize = 8;                 // Number of bits/bytes, 4-8 
  PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
  PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 

  // Configure the port according to the specifications of the DCB 
  // structure.
  if (!SetCommState (hPort, &PortDCB))
  {
    // Could not create the read thread.
//    MessageBox (hMainWnd, TEXT("Unable to configure the serial port"), 
//                TEXT("Error"), MB_OK);

	CString Buff;
	Buff.Format(_T("Unable to configure the serial port: %s"), m_PortName);
//   	MessageBox(NULL, Buff, "Error"), MB_OK);
    dwError = GetLastError ();
    return FALSE;
  }

  // Retrieve the time-out parameters for all read and write operations
  // on the port. 
  GetCommTimeouts (hPort, &CommTimeouts);

  // Change the COMMTIMEOUTS structure settings.
  CommTimeouts.ReadIntervalTimeout = 1;  
  CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
  CommTimeouts.ReadTotalTimeoutConstant = 0;    
  CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
  CommTimeouts.WriteTotalTimeoutConstant = 1000;    

 
  // Set the time-out parameters for all read and write operations
  // on the port. 
  if (!SetCommTimeouts (hPort, &CommTimeouts))
  {
    // Could not create the read thread.
//    MessageBox (hMainWnd, TEXT("Unable to set the time-out parameters"), 
 //            
	

	CString Buff;
	Buff.Format(_T("Unable to set the time-out parameters: %s"), m_PortName);
//   	MessageBox(NULL, Buff, "Error"), MB_OK);
    dwError = GetLastError ();
    return FALSE;
  }

  // Direct the port to perform extended functions SETDTR and SETRTS.
  // SETDTR: Sends the DTR (data-terminal-ready) signal.
  // SETRTS: Sends the RTS (request-to-send) signal. 
  EscapeCommFunction (hPort, SETDTR);
  EscapeCommFunction (hPort, SETRTS);

  // Create a read thread for reading data from the communication port.

	last_read_ptr=rx_ptr=0;
	rx_buffer = new char[BUFFER_LENGTH];
	memset(rx_buffer,0,BUFFER_LENGTH);

	m_bThreadEnabled = TRUE;
	pThread = AfxBeginThread(PortOverlappedReadThread,this);

	m_nFaultyDataPkts = 0;
	InitializeMinMaxValues();

  return TRUE;
}

void CSerialJoystick::WriteByte(char Byte)
{
  DWORD dwError,
        dwNumBytesWritten;

  if (!WriteFile (hPort,              // Port handle
                  &Byte,              // Pointer to the data to write 
                  1,                  // Number of bytes to write
                  &dwNumBytesWritten, // Pointer to the number of bytes 
                                      // written
                  NULL))              // Must be NULL for Windows CE
  {
    // WriteFile failed. Report error.
    dwError = GetLastError ();
	CString String;
	String.Format(_T("%c"), Byte);
	String = L"Error Writing Caracter: " + String + L"to Serial Port";
	//MessageBox(NULL, String.GetBuffer(50), "Error"), MB_OK);
  }
}






void	CSerialJoystick::WriteStr(char *string)
{
	unsigned int	ii;

	for(ii=0;ii<strlen(string);ii++) {
		WriteByte(string[ii]);
	};
}

void CSerialJoystick::WriteStrLenght(char *string, int BytesNumber)
{
	int	ii;

	for(ii=0;ii<BytesNumber;ii++) {
		WriteByte(string[ii]);
	};



}


void CSerialJoystick::ClearBuffer()
{
	last_read_ptr = 0;
	rx_ptr = 0;
	
}

int	CSerialJoystick::Hit()
{
	return (rx_ptr - last_read_ptr);
}

bool CSerialJoystick::ReadByte(char *Byte)
{

	CTime startTime = CTime::GetCurrentTime();

	DWORD TimeStop, TimeStart, TimeElapsed;
	TimeStart =  TimeStop = 0;

	while(Hit()==0) 
	{
		TimeElapsed = TimeStop - TimeStart;
		if (TimeElapsed > 1000)
		{
			CString Buff;
			Buff.Format(_T("Problem Reading BYTE on Serial Port:  %s"),m_PortName); 
			//MessageBox(NULL, Buff, "Error"), MB_OK);
			return FALSE;
		}

	};

	*Byte=rx_buffer[last_read_ptr];
	last_read_ptr++;
	last_read_ptr%=BUFFER_LENGTH;

	return TRUE;
}

int	CSerialJoystick::ReadStr(char *string,char Eot)
{
	char Byte=0;
	int	ii=0;

	while(Byte!=Eot) 
	{
 	if (ReadByte(&Byte)==FALSE) return ii;
	string[ii]=Byte;
	ii++;

	string[ii]=0;
	}

	return ii;
}

int	CSerialJoystick::ReadStrLenght(char *string,int CaracterNumber)
{
	int ii;
	char Byte=0;
	strcpy_s(string,2, "");
	
	for (ii=0;ii<CaracterNumber;ii++)
	{
		if (ReadByte(&Byte)==FALSE) return ii;
		string[ii]=Byte;
		//string[ii]=0;
	}

	return ii;
}


void CSerialJoystick::ExtractJoyValues()
{
	unsigned char	checksum,checksum_rx;
	int	ii;

	checksum_rx=0;
	for(ii=0;ii<13;ii++) checksum_rx+=JoystickArray[ii];
	checksum_rx&=0xff;

	checksum=JoystickArray[13]|((JoystickArray[14]<<6)&0xc0);

	if((checksum==checksum_rx)&&(JoystickArray[0]==0x80)&&(JoystickArray[15]==0xff)) {
		
		int nFirMax = MinMax(&theApp.m_LastSettings.nSerialPortFIRCount, 1, 16);
		m_nFir++;
		m_nFir%= nFirMax;

		m_nXAxis[m_nFir]=((JoystickArray[2]<<6)&0xC0)+JoystickArray[1];
		m_nYAxis[m_nFir]=((JoystickArray[4]<<6)&0xC0)+JoystickArray[3];
		m_nZAxis[m_nFir]=((JoystickArray[6]<<6)&0xC0)|JoystickArray[5];
		m_nSAxis[m_nFir]=((JoystickArray[8]<<6)&0xC0)|JoystickArray[7];
		m_nTAxis=((JoystickArray[10]<<6)&0xC0)|JoystickArray[9];
		m_nSwitches=((JoystickArray[12]<<6)&0xC0)|JoystickArray[11];

		int nSum = 0;
		for (ii = 0, nSum = 0; ii < nFirMax; ii++) nSum += m_nXAxis[ii];
		theApp.m_Motors.m_nJoyValue[0] = nSum / nFirMax;
		for (ii = 0, nSum = 0; ii < nFirMax; ii++) nSum += m_nYAxis[ii];
		theApp.m_Motors.m_nJoyValue[1] = nSum / nFirMax;
		for (ii = 0, nSum = 0; ii < nFirMax; ii++) nSum += m_nZAxis[ii];
		theApp.m_Motors.m_nJoyValue[2] = nSum / nFirMax;
		for (ii = 0, nSum = 0; ii < nFirMax; ii++) nSum += m_nSAxis[ii];
		theApp.m_Motors.m_nJoyValue[3] = nSum / nFirMax;

		theApp.m_Motors.m_nJoyValue[4]=m_nTAxis;

		theApp.m_Motors.m_nSwitchJoyValue=m_nSwitches;
		if(theApp.m_Motors.m_nJoyValue[4] < 10) {
			theApp.m_Motors.m_nToggleJoyPos=0;
		}
		if((theApp.m_Motors.m_nJoyValue[4] > 10)&&(theApp.m_Motors.m_nJoyValue[4] < 254)) {
			theApp.m_Motors.m_nToggleJoyPos=1;
		}
		if(theApp.m_Motors.m_nJoyValue[4] > 245) {
			theApp.m_Motors.m_nToggleJoyPos=2;
		}

		theApp.m_Si6User.m_Si6.nCommsJoystickValue[0]=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Si6User.m_Si6.nCommsJoystickValue[1]=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Si6User.m_Si6.nCommsJoystickValue[2]=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Si6User.m_Si6.nCommsJoystickValue[3]=theApp.m_Motors.m_nJoyValue[3];
		theApp.m_Si6User.m_Si6.nCommsJoystickValue[4]=m_nTAxis;
		theApp.m_Si6User.m_Si6.nCommsSwitchValue=m_nSwitches;

		theApp.m_Si10User.m_Si10.nJoystickValue[0]=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Si10User.m_Si10.nJoystickValue[1]=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Si10User.m_Si10.nJoystickValue[2]=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Si10User.m_Si10.nJoystickValue[3]=theApp.m_Motors.m_nJoyValue[3];
		theApp.m_Si10User.m_Si10.nJoystickValue[4]=m_nTAxis;
		theApp.m_Si10User.m_Si10.nJoystickSwitchValue=m_nSwitches;

		theApp.m_Si10User.m_Si10.nJoystickMode;
		theApp.m_Si10User.m_Si10.nJoystickSource				= theApp.m_Tank.nJoystickSource;
		theApp.m_Si10User.m_Si10.nJoystickPercent				= 100;
		theApp.m_Si10User.m_Si10.nJoystickLeftSideBit			= theApp.m_Tank.nLeftSideBit;
		theApp.m_Si10User.m_Si10.nJoystickRightSideBit		= theApp.m_Tank.nRightSideBit;
		theApp.m_Si10User.m_Si10.nJoystickDeadManBit0			= theApp.m_Tank.nDeadManBit0;
		theApp.m_Si10User.m_Si10.nJoystickDeadManBit1			= theApp.m_Tank.nDeadManBit1;
		theApp.m_Si10User.m_Si10.nJoystickMainManipBit		= theApp.m_Tank.nMainManipBit;
		theApp.m_Si10User.m_Si10.nJoystickScannerFixtureBit	= theApp.m_Tank.nScannerFixtureBit;
		theApp.m_Si10User.m_Si10.nJoystickSpeedBit			= theApp.m_Tank.nSpeedBit;

	} else {
		m_nFaultyDataPkts;
	}

	if(m_JoyMin[0] > theApp.m_Motors.m_nJoyValue[0]) m_JoyMin[0] = theApp.m_Motors.m_nJoyValue[0];
	if(m_JoyMin[1] > theApp.m_Motors.m_nJoyValue[1]) m_JoyMin[1] = theApp.m_Motors.m_nJoyValue[1];
	if(m_JoyMin[2] > theApp.m_Motors.m_nJoyValue[2]) m_JoyMin[2] = theApp.m_Motors.m_nJoyValue[2];
	if(m_JoyMin[3] > theApp.m_Motors.m_nJoyValue[3]) m_JoyMin[3] = theApp.m_Motors.m_nJoyValue[3];
	if(m_JoyMax[0] < theApp.m_Motors.m_nJoyValue[0]) m_JoyMax[0] = theApp.m_Motors.m_nJoyValue[0];
	if(m_JoyMax[1] < theApp.m_Motors.m_nJoyValue[1]) m_JoyMax[1] = theApp.m_Motors.m_nJoyValue[1];
	if(m_JoyMax[2] < theApp.m_Motors.m_nJoyValue[2]) m_JoyMax[2] = theApp.m_Motors.m_nJoyValue[2];
	if(m_JoyMax[3] < theApp.m_Motors.m_nJoyValue[3]) m_JoyMax[3] = theApp.m_Motors.m_nJoyValue[3];
}

void CSerialJoystick::InitializeMinMaxValues()
{

	for(int ii=0;ii<4;ii++) {
		m_JoyMin[ii] = 255;
		m_JoyMax[ii] = 0;
	}
}

/***********************************************************************

  PortClose (HANDLE hCommPort)

***********************************************************************/
BOOL CSerialJoystick::Close ()
{
	DWORD dwError;

	if (m_bThreadEnabled==TRUE) {
		m_bThreadEnabled=FALSE;
		TRACE1("Closing port %d\n", theApp.m_LastSettings.nJoystickCommPort);
		WaitForSingleObject(&pThread->m_hThread,INFINITE);
		TRACE0("Thread closed\n");

		delete [] rx_buffer;
		rx_buffer=NULL;
		if(!CloseHandle (hPort)) {
			dwError = GetLastError();
			MessageBox(NULL,_T("Error closing handle to serial port"),_T("Serial Port"),MB_ICONERROR);
		}
	}
	return TRUE;

}






void CSerialJoystickDlg::OnSelchangeComboCommPort() 
{
	int nTemp = theApp.m_LastSettings.nJoystickCommPort;

	theApp.m_LastSettings.nJoystickCommPort = m_comboCommPort.GetCurSel();
	if(nTemp - theApp.m_LastSettings.nJoystickCommPort) {
		theApp.m_SerialJoystick.Close();
		theApp.m_SerialJoystick.Start();

	}
	
}

void CSerialJoystick::GetRxBuffer(CString *pBuff, int nStart, int nLength)
{
	char Array[100];
	int	jj,nn;
	CString Buff,Temp;

	if(rx_buffer==NULL) return;

	for(nn=0,jj=0;nn<nLength;nn++,jj++) {
		Array[jj]=rx_buffer[nStart+nn];
		if(Array[jj]==27) {
			Array[jj++]='<';
			Array[jj++]='E';
			Array[jj++]='S';
			Array[jj++]='C';
			Array[jj]='>';
		}
		if(isprint(Array[jj])==0) Array[jj]='*'; 
	}
	Array[jj++]='\t';
	Array[jj]=0;
	Buff.Format(_T("%s"),Array);

	for(nn=0;nn<nLength;nn++,jj++) {
		Temp.Format(_T("%02x "),rx_buffer[nStart+nn]);
		Buff += Temp;
	}
	pBuff->Format(_T("%s"),Buff);

}

void CSerialJoystick::ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered)
{
	if(rx_buffer==NULL) return;

	for(int nn=0;nn<(int)dwBytesTransfered;nn++)  {
		rx_buffer[rx_ptr]=pArray[nn];
		rx_ptr++;
		rx_ptr%=BUFFER_LENGTH;

		JoystickArray[nJoyPtr]=pArray[nn];
		switch(pArray[nn]) {
		default:
			nJoyPtr++;
			nJoyPtr%=(sizeof JoystickArray);
			break;
		case 0x80:	nJoyPtr=1;
			break;
		case 0xff:
			if(nJoyPtr == 15) {
				ExtractJoyValues();
			} else {
				m_nFaultyDataPkts++;
			}
			nJoyPtr=0;
			break;
		};
	}
}



/***********************************************************************

  PortReadThread (LPVOID lpvoid)

***********************************************************************/
UINT PortReadThreadNonOverLapped (LPVOID lpvoid)
{
  BYTE Byte;
  DWORD dwCommModemStatus,
        dwBytesTransferred;

  CSerialJoystick* pComs = (CSerialJoystick *) lpvoid;

  // Specify a set of events to be monitored for the port.
  SetCommMask (pComs->hPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RLSD | EV_RING);

  pComs->m_bThreadFinished=FALSE;
  while (pComs->hPort != INVALID_HANDLE_VALUE) 
  {
    // Wait for an event to occur for the port.
    WaitCommEvent (pComs->hPort, &dwCommModemStatus, 0);

    // Re-specify the set of events to be monitored for the port.
    SetCommMask (pComs->hPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RING);

    if (dwCommModemStatus & EV_RXCHAR) {
		do {
			ReadFile (pComs->hPort, &Byte, 1, &dwBytesTransferred, 0);

			if (dwBytesTransferred == 1) {
				pComs->ByteSuccessfullyRead(&Byte,dwBytesTransferred);
			}

      } while (dwBytesTransferred == 1);
    }
	GetCommModemStatus (pComs->hPort, &dwCommModemStatus);
  }
  pComs->m_bThreadFinished=TRUE;

  return 0;
}

#define READ_TIMEOUT      1      // milliseconds

UINT PortOverlappedReadThread (LPVOID lpvoid)
{
	DWORD	dwBytesTransferred;

	CSerialJoystick* pComs = (CSerialJoystick *) lpvoid;
	BOOL bWaitingOnRead = FALSE;
	OVERLAPPED osReader = {0};
	DWORD dwRes;
	BYTE	Array[100];

	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL)
		return -1;	// Error creating overlapped event; abort.
		
	SetCommMask (pComs->hPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RLSD | EV_RING);

	pComs->m_bThreadFinished=FALSE;
	while (pComs->m_bThreadEnabled == TRUE) {
		if (!bWaitingOnRead) {
			if (!ReadFile(pComs->hPort, Array, 1, &dwBytesTransferred, &osReader)) {
				if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
				 // Error in communications; report it.
				} else {
					bWaitingOnRead = TRUE;
				}
			} else {
				pComs->ByteSuccessfullyRead( Array,dwBytesTransferred);
			}
		}
		if (bWaitingOnRead) {
			dwRes = WaitForSingleObject(osReader.hEvent, READ_TIMEOUT);
			switch(dwRes)  {
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(pComs->hPort, &osReader, &dwBytesTransferred, FALSE)) {
					 // Error in communications; report it.
				} else {
					if(dwBytesTransferred>0) pComs->ByteSuccessfullyRead( Array, dwBytesTransferred);
						bWaitingOnRead = FALSE;
				}
				break;
				case WAIT_TIMEOUT:
				  // Operation isn't complete yet. fWaitingOnRead flag isn't
				  // changed since I'll loop back around, and I don't want
				  // to issue another read until the first one finishes.
				  //
				  // This is a good time to do some background work.
				break;                       
				default:
				MessageBox(NULL,_T("ERROR"),_T("ERROR"),NULL);
				  // Error in the WaitForSingleObject; abort.
				  // This indicates a problem with the OVERLAPPED structure's
				  // event handle.
				break;
			}
		}
	}

	CloseHandle(osReader.hEvent);
	pComs->m_bThreadFinished=TRUE;

	return 0;
}


BOOL CSerialJoystick::WriteABuffer(char * lpBuf, DWORD dwToWrite)
{
   OVERLAPPED osWrite = {0};
   DWORD dwWritten;
   DWORD dwRes;
   BOOL fRes;

   // Create this write operation's OVERLAPPED structure's hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (osWrite.hEvent == NULL)
      // error creating overlapped event handle
      return FALSE;

   // Issue write.
   if (!WriteFile(hPort, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
      if (GetLastError() != ERROR_IO_PENDING) { 
         // WriteFile failed, but isn't delayed. Report error and abort.
         fRes = FALSE;
      } else {
         // Write is pending.
         dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
         switch(dwRes)
         {
            // OVERLAPPED structure's event has been signaled. 
            case WAIT_OBJECT_0:
                 if (!GetOverlappedResult(hPort, &osWrite, &dwWritten, FALSE))
                       fRes = FALSE;
                 else
                  // Write operation completed successfully.
                  fRes = TRUE;
                 break;
            
            default:
                 // An error has occurred in WaitForSingleObject.
                 // This usually indicates a problem with the
                // OVERLAPPED structure's event handle.
                 fRes = FALSE;
                 break;
         }
      }
   } else {
      // WriteFile completed immediately.
      fRes = TRUE;
   }

   CloseHandle(osWrite.hEvent);
   return fRes;
}

void CSerialJoystickDlg::OnChangeEditScannerfixture() 
{
	CString Buff;

	m_editScannerFixture.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nScannerFixtureBit);
}


void CSerialJoystickDlg::OnChangeEditSpeed() 
{
	CString Buff;

	m_editSpeed.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nSpeedBit);
}



void CSerialJoystickDlg::OnChangeEditDeadman0() 
{
	CString Buff;

	m_editDeadMan0.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nDeadManBit0);
	
}

void CSerialJoystickDlg::OnChangeEditDeadman1() 
{
	CString Buff;

	m_editDeadMan1.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nDeadManBit1);
	
}

void CSerialJoystickDlg::OnChangeEditMainManip() 
{
	CString Buff;

	m_editMainManip.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nMainManipBit);
}

void CSerialJoystickDlg::OnChangeEditSideSelect() 
{
	
}

void CSerialJoystickDlg::OnSelchangeComboJoystickType() 
{
	theApp.m_Tank.nJoystickSource = (JoystickSource)m_comboJoystickSource.GetCurSel();
}

void CSerialJoystickDlg::OnChangeEditLeftSide() 
{
	CString Buff;

	m_editLeftSide.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nLeftSideBit);
}

void CSerialJoystickDlg::OnChangeEditRightSide() 
{
	CString Buff;

	m_editRightSide.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nRightSideBit);
}

void CSerialJoystickDlg::OnChangeEditDockedBit() 
{
	CString Buff;

	m_editDockedBit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nDockedBit);
}

BOOL CSerialJoystickDlg::OnSetActive() 
{
	SetTimer(1,100,NULL);	
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetAccessPrivelage();
	
	return CPropertyPage::OnSetActive();
}

BOOL CSerialJoystickDlg::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CSerialJoystickDlg::SetAccessPrivelage()
{
	bool bFlagEnable;

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_SERIAL_JOYSTICK_DIALOG] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	m_editDockedBit.EnableWindow(bFlagEnable);
	m_editLeftSide.EnableWindow(bFlagEnable);
	m_editRightSide.EnableWindow(bFlagEnable);
	m_comboJoystickSource.EnableWindow(bFlagEnable);
	m_editMainManip.EnableWindow(bFlagEnable);
	m_editDeadMan1.EnableWindow(bFlagEnable);
	m_editDeadMan0.EnableWindow(bFlagEnable);
	m_editSwitches.EnableWindow(bFlagEnable);
	m_editSpeed.EnableWindow(bFlagEnable);
	m_editScannerFixture.EnableWindow(bFlagEnable);
	m_comboCommPort.EnableWindow(bFlagEnable);
//	m_nJoyPtr.EnableWindow(bFlagEnable);
//	m_nSAxis.EnableWindow(bFlagEnable);
//	m_nTAxis.EnableWindow(bFlagEnable);
//	m_nXAxis.EnableWindow(bFlagEnable);
//	m_nYAxis.EnableWindow(bFlagEnable);
//	m_nZAxis.EnableWindow(bFlagEnable);
//	m_stringElapsedTime.EnableWindow(bFlagEnable);
//	m_stringPosCounter.EnableWindow(bFlagEnable);
//	m_editJoyArray.EnableWindow(bFlagEnable);



}

void CSerialJoystickDlg::OnChangeEditDeadbandSize() 
{
	CString Buff;

	m_editDeadBandSize.GetWindowText(Buff);
	theApp.m_Tank.nJoystickDeadBand = _ttoi(Buff);
}

void CSerialJoystickDlg::OnButtonReset() 
{
	theApp.m_SerialJoystick.InitializeMinMaxValues();
}


void CSerialJoystickDlg::OnEnChangeEditFrameTool()
{
	CString Buff;

	m_editFrameToolJoyBit.GetWindowText(Buff);
	swscanf_s(Buff, _T("%x"), &theApp.m_Tank.nFrameToolJoyMode);

	
}


void CSerialJoystickDlg::OnEnChangeEditFir()
{
	CString Buff;

	m_editFIR.GetWindowTextW(Buff);
	theApp.m_LastSettings.nSerialPortFIRCount = _ttoi(Buff);
}
