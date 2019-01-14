// RobotComms.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotCommsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotCommsPage property page

IMPLEMENT_DYNCREATE(CRobotCommsPage, CPropertyPage)

CRobotCommsPage::CRobotCommsPage() : CPropertyPage(CRobotCommsPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotCommsPage)
	//}}AFX_DATA_INIT
	
	m_bCtrlButton = false;
	m_nNumberLines = 0;

	m_nRowsL = 0;
	m_nTopRow = 0;
	m_nDisplayRows = 2;
	m_nTxIndex = 0;

	m_nIndex = 0;
	m_pEditSpinItem = NULL;

}

CRobotCommsPage::~CRobotCommsPage()
{
}

void CRobotCommsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotCommsPage)
	DDX_Control(pDX, IDC_LIST_WATCH, m_listWatch);
	DDX_Control(pDX, IDC_SCROLL_V, m_scrollV);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_ETHERCAT_STATUS, m_editEthercatStatus);
	DDX_Control(pDX, IDC_EDIT_TX_STR, m_editInLineCommand);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_IPADDRESS_ROBOT, m_ipAddress);
	DDX_Control(pDX, IDC_CHECK_SELECT_AT_STARTUP, m_checkSelectAtStartUp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotCommsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotCommsPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_ROBOT, OnFieldchangedIpaddressRobot)
	ON_BN_CLICKED(IDC_CHECK_SELECT_AT_STARTUP, OnCheckSelectAtStartup)
	ON_EN_CHANGE(IDC_EDIT_USER, OnChangeEditUser)
	ON_EN_CHANGE(IDC_EDIT_PORT, OnChangeEditPort)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, OnChangeEditPassword)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOL, OnSelchangeComboProtocol)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, OnButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_KILL, OnButtonKill)
	ON_BN_CLICKED(IDC_RADIO_ROBOT_0, OnRadioRobot0)
	ON_BN_CLICKED(IDC_RADIO_ROBOT_1, OnRadioRobot1)
	ON_BN_CLICKED(IDC_RADIO_ROBOT_BOTH, OnRadioRobotBoth)
	ON_BN_CLICKED(IDC_BUTTON_PROG_3, OnButtonProg3)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_DC_MINUS, OnChangeEditDcMinus)
	ON_EN_CHANGE(IDC_EDIT_DC_PLUS, OnChangeEditDcPlus)
	ON_EN_CHANGE(IDC_EDIT_DC_BAND, OnChangeEditDcBand)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_JOG_TOSAFE, OnButtonJogTosafe)
	ON_BN_CLICKED(IDC_BUTTON_PMAC_RESET, OnButtonPmacReset)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_CHECK_ECHO_DATA, OnCheckEchoData)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_WATCH, OnClickListWatch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WATCH, OnDblclkListWatch)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_WATCH, OnGetdispinfoListWatch)
	ON_COMMAND(ID_WATCH_DELETE, OnWatchDelete)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WATCH, OnRclickListWatch)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COMMS_CLEARALL, OnCommsClearall)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotCommsPage message handlers


BOOL CRobotCommsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	SCROLLINFO ScrollInfo;

	m_scrollV.GetScrollInfo(&ScrollInfo);

	m_scrollV.SetScrollRange(0.0, (double)m_nRowsL, FALSE);
	m_scrollV.SetPageSize(m_nDisplayRows, false);
	m_scrollV.SetScrollPos((double)m_nTopRow, TRUE);

	CreateColumns();
	FillList();

	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotCommsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CRobotCommsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	sockaddr_in Server;
	CButton *pButton;

	Server.sin_addr.s_addr = theApp.m_FBCtrl.m_nIPAddrs;
	m_ipAddress.SetAddress(Server.sin_addr.s_addr); 


	m_checkSelectAtStartUp.SetCheck(theApp.m_FBCtrl.m_bEnableAtStartUp);

	if(theApp.m_FBCtrl.m_TerminalSocket.IsConnected() == true) {
		m_buttonConnect.SetWindowText(L"Disconnect");
	} else {
		m_buttonConnect.SetWindowText(L"Connect");
	}

	switch(	theApp.m_LastSettings.nCoordMask) {
	case 1:	pButton = (CButton* ) GetDlgItem(IDC_RADIO_ROBOT_0);
		pButton->SetCheck(1);
		break;
	case 2:	pButton = (CButton* ) GetDlgItem(IDC_RADIO_ROBOT_1);
		pButton->SetCheck(1);
		break;
	case 3:	pButton = (CButton* ) GetDlgItem(IDC_RADIO_ROBOT_BOTH);
		pButton->SetCheck(1);
		break;
	}



}

void CRobotCommsPage::OnFieldchangedIpaddressRobot(NMHDR* pNMHDR, LRESULT* pResult) 
{
	sockaddr_in Server;

	m_ipAddress.GetAddress(Server.sin_addr.s_addr); 
	theApp.m_FBCtrl.m_nIPAddrs = Server.sin_addr.s_addr;
	
	*pResult = 0;
}

void CRobotCommsPage::OnCheckSelectAtStartup() 
{
	theApp.m_FBCtrl.m_bEnableAtStartUp = m_checkSelectAtStartUp.GetCheck() & 0x01;
}

void CRobotCommsPage::OnChangeEditUser() 
{
	CString Buff;

	m_editUser.GetWindowText(Buff);
	swscanf_s(Buff,L"%s",&theApp.m_FBCtrl.m_wUser,200);
}

void CRobotCommsPage::OnChangeEditPort() 
{
	CString Buff;

	m_editPort.GetWindowText(Buff);
	theApp.m_FBCtrl.m_nPort = _ttoi(Buff);
}

void CRobotCommsPage::OnChangeEditPassword() 
{
	CString Buff;

	m_editPassword.GetWindowText(Buff); 
	theApp.m_Strings.SetAt(StringType::PPMAC_Password, Buff);
	swscanf_s(Buff,L"%s",&theApp.m_FBCtrl.m_wPassword,200);
}

void CRobotCommsPage::OnSelchangeComboProtocol() 
{
	CString Buff;

	theApp.m_FBCtrl.m_nProtocol = m_comboProtocol.GetCurSel();
}

void CRobotCommsPage::OnButtonConnect() 
{
	if(theApp.m_FBCtrl.m_TerminalSocket.IsConnected() == false) {
		theApp.m_FBCtrl.SocketConnect();
		theApp.m_FBCtrl.StartCommsThread();
	} else {
//		theApp.m_FBCtrl.SocketDisconnect();
	}
}

BOOL CRobotCommsPage::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;

	switch(pMsg->message) {
	case WM_KEYDOWN:

		switch (pMsg->wParam) {
		case 13:
			m_editInLineCommand.GetWindowText(Buff);
			theApp.m_FBCtrl.SendStr(Buff + L"\r\n", _TERMINAL);
			m_editInLineCommand.SetWindowText(L"");
			m_TxArray.Add(Buff);
			m_nTxIndex = m_TxArray.GetSize();
			return TRUE;
			break;
		case CURSOR_UP:
			if (m_TxArray.GetSize() > 0) {
				m_nTxIndex--;
				MinMax(&m_nTxIndex, 0, m_TxArray.GetSize() - 1);
				m_editInLineCommand.SetWindowText(m_TxArray.GetAt(m_nTxIndex));
				m_editInLineCommand.SetSel(0, -1);
				m_editInLineCommand.SetSel(-1);
				return TRUE;
			}
			break;
		case CURSOR_DOWN:
			if (m_TxArray.GetSize() > 0) {
				m_nTxIndex++;
				MinMax(&m_nTxIndex, 0, m_TxArray.GetSize());
				if (m_nTxIndex < m_TxArray.GetSize()) {
					m_editInLineCommand.SetWindowText(m_TxArray.GetAt(m_nTxIndex));
					m_editInLineCommand.SetSel(0, -1);
					m_editInLineCommand.SetSel(-1);
					return TRUE;
				}
				else {
					m_editInLineCommand.SetWindowText(L"");
				}
			}
			break;
		}
		break;
	}

	
	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CRobotCommsPage::OnTimer(UINT nIDEvent) 
{
	CString Buff,Temp;

	CRect rrClip, rr;
	CRgn rgn;
	int nn;

	if (GetSafeHwnd() && IsWindowVisible() == TRUE) {



		if ((nn = theApp.m_FBCtrl.getRxArraySize()) != m_nRowsL) {
			m_nRowsL = nn;
			m_nTopRow = m_nRowsL - m_nDisplayRows;

			m_scrollV.SetScrollRange(0.0, (double)m_nRowsL, FALSE);
			m_scrollV.SetPageSize(m_nDisplayRows, false);
			m_scrollV.SetScrollPos((double)m_nTopRow, TRUE);

			RenderView();
		}

	}

	if(m_nEthercatStatus != theApp.m_Ethercat.m_nBusStatus) {
		if(theApp.m_Ethercat.m_nBusStatus & ETHERCAT_BUS_ON) {
			m_editEthercatStatus.SetWindowText(L"On");
		} else {
			m_editEthercatStatus.SetWindowText(L"Off");
		}
		m_nEthercatStatus = theApp.m_Ethercat.m_nBusStatus;
	}


	m_listWatch.Invalidate(true);

	CPropertyPage::OnTimer(nIDEvent);
}


void CRobotCommsPage::OnButtonEnable() 
{
	theApp.m_FBCtrl.EnableMotors();
}

void CRobotCommsPage::OnButtonKill() 
{
	theApp.m_FBCtrl.KillMotors();
}


void CRobotCommsPage::DownLoadScanLine(int nLine)
{
	int ii,nNumberCoords;
	CString Buff;
	CCoord CpSurface;

	if(nLine&1) {
		for(ii=0,nNumberCoords=0;ii<PROFILE->m_ScanLine[m_nScanLine].GetSize();ii+=10,nNumberCoords++) {
			PROFILE->m_ScanLine[m_nScanLine].GetCoord(ii,&CpSurface);

			theApp.InvertNormScannerSpecific(&CpSurface,PORTSIDE);
			CpSurface.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,theApp.m_Robot[0].m_fBaseRotY,theApp.m_Robot[0].m_fBaseRotZ);
			CpSurface.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,theApp.m_Robot[1].m_fBaseRotY,theApp.m_Robot[1].m_fBaseRotZ);
			CpSurface.FormatQuaternion(&Buff,1,(nNumberCoords*14)+2000);
			theApp.m_FBCtrl.SendStr(Buff);
		}
		PROFILE->m_ScanLine[m_nScanLine].GetLastCoord(&CpSurface);
		theApp.InvertNormScannerSpecific(&CpSurface,PORTSIDE);
		CpSurface.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,theApp.m_Robot[0].m_fBaseRotY,theApp.m_Robot[0].m_fBaseRotZ);
		CpSurface.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,theApp.m_Robot[1].m_fBaseRotY,theApp.m_Robot[1].m_fBaseRotZ);
		CpSurface.FormatQuaternion(&Buff,1,(nNumberCoords*14)+2000);
		theApp.m_FBCtrl.SendStr(Buff);

	} else {

		for(ii=PROFILE->m_ScanLine[m_nScanLine].GetSize() - 1,nNumberCoords=0;ii>=0;ii-=10,nNumberCoords++) {
			PROFILE->m_ScanLine[m_nScanLine].GetCoord(ii,&CpSurface);
			theApp.InvertNormScannerSpecific(&CpSurface,PORTSIDE);
			CpSurface.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,theApp.m_Robot[0].m_fBaseRotY,theApp.m_Robot[0].m_fBaseRotZ);
			CpSurface.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,theApp.m_Robot[1].m_fBaseRotY,theApp.m_Robot[1].m_fBaseRotZ);
			CpSurface.FormatQuaternion(&Buff,1,(nNumberCoords*14)+2000);
			theApp.m_FBCtrl.SendStr(Buff);
		}
		PROFILE->m_ScanLine[m_nScanLine].GetCoord(0,&CpSurface);
		theApp.InvertNormScannerSpecific(&CpSurface,PORTSIDE);
		CpSurface.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,theApp.m_Robot[0].m_fBaseRotY,theApp.m_Robot[0].m_fBaseRotZ);
		CpSurface.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,theApp.m_Robot[1].m_fBaseRotY,theApp.m_Robot[1].m_fBaseRotZ);
		CpSurface.FormatQuaternion(&Buff,1,(nNumberCoords*14)+2000);
		theApp.m_FBCtrl.SendStr(Buff);
	};
	nNumberCoords++;

	Buff.Format(L"NumberCoords=%d",nNumberCoords);
	theApp.m_FBCtrl.SendStr(Buff);
	Buff.Format(L"CoordPtr=0");
	theApp.m_FBCtrl.SendStr(Buff);


	theApp.m_FBCtrl.m_n64CoordStatus[0]=0;
	theApp.m_FBCtrl.m_nProgramFinished = 0;
	theApp.m_FBCtrl.m_nCoordPtr = 0;

}

void CRobotCommsPage::OnRadioRobot0() 
{
	theApp.m_LastSettings.nCoordMask = 1;
}

void CRobotCommsPage::OnRadioRobot1() 
{
	theApp.m_LastSettings.nCoordMask = 2;
}

void CRobotCommsPage::OnRadioRobotBoth() 
{
	theApp.m_LastSettings.nCoordMask = 3;
}

void CRobotCommsPage::OnButtonProg3() 
{
	CString Buff;

	Buff.Format(L"FeedRate=%.0f",1000.0f * (float)theApp.m_LastSettings.fFeedRate / 100.0f);
	theApp.m_FBCtrl.SendStr(Buff);

	theApp.m_FBCtrl.SendStr(L"undefine all");
	theApp.m_FBCtrl.SendStr(L"delete all lookahead");

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_FBCtrl.SendStr(L"&1");
		theApp.m_FBCtrl.SendStr(L"#1->I");
		theApp.m_FBCtrl.SendStr(L"#2->I");
		theApp.m_FBCtrl.SendStr(L"#3->I");
		theApp.m_FBCtrl.SendStr(L"#4->I");
		theApp.m_FBCtrl.SendStr(L"#5->I");
		theApp.m_FBCtrl.SendStr(L"#6->I");
		theApp.m_FBCtrl.SendStr(L"#7->I");
		theApp.m_FBCtrl.SendStr(L"#8->I");
		theApp.m_FBCtrl.SendStr(L"#9->I");
		theApp.m_FBCtrl.SendStr(L"#10->I");
		theApp.m_FBCtrl.SendStr(L"#11->I");
		theApp.m_FBCtrl.SendStr(L"#12->I");
		theApp.m_FBCtrl.SendStr(L"&3 define lookahead 10000");
		theApp.m_FBCtrl.SendStr(L"Coord[1].LHDistance=4096");
		theApp.m_FBCtrl.SendStr(L"Coord[1].SegLinToPvt=1");
		theApp.m_FBCtrl.SendStr(L"Coord[1].TA=20");
		theApp.m_FBCtrl.SendStr(L"Coord[1].Td=20");
		theApp.m_FBCtrl.SendStr(L"Coord[1].TS=20");
		theApp.m_FBCtrl.SendStr(L"Coord[1].TM=5");
		theApp.m_FBCtrl.SendStr(L"B3R");
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Kinematics.m_nAxesIncludedJoystick = 0x00;
		theApp.m_FBCtrl.SendAxesIncluded(JOYSTICK_INCLUDE, true);

		theApp.m_FBCtrl.SendStr(L"&3");
		theApp.m_FBCtrl.SendStr(L"#1->I");
		theApp.m_FBCtrl.SendStr(L"#2->I");
		theApp.m_FBCtrl.SendStr(L"#3->I");
		theApp.m_FBCtrl.SendStr(L"#4->I");
		theApp.m_FBCtrl.SendStr(L"#5->I");
		theApp.m_FBCtrl.SendStr(L"#6->I");
		theApp.m_FBCtrl.SendStr(L"#7->I");
		theApp.m_FBCtrl.SendStr(L"#8->I");
		theApp.m_FBCtrl.SendStr(L"#9->I");
		theApp.m_FBCtrl.SendStr(L"#11->I");
		theApp.m_FBCtrl.SendStr(L"#12->I");
		theApp.m_FBCtrl.SendStr(L"#13->I");
		theApp.m_FBCtrl.SendStr(L"#14->I");
		theApp.m_FBCtrl.SendStr(L"#15->I");
		theApp.m_FBCtrl.SendStr(L"#16->I");
		theApp.m_FBCtrl.SendStr(L"#17->I");
		theApp.m_FBCtrl.SendStr(L"#18->I");
		theApp.m_FBCtrl.SendStr(L"#19->I");
		theApp.m_FBCtrl.SendStr(L"&3 define lookahead 10000");
		theApp.m_FBCtrl.SendStr(L"Coord[3].LHDistance=4096");
		theApp.m_FBCtrl.SendStr(L"Coord[3].SegLinToPvt=1");
		theApp.m_FBCtrl.SendStr(L"Coord[3].TA=20");
		theApp.m_FBCtrl.SendStr(L"Coord[3].Td=20");
		theApp.m_FBCtrl.SendStr(L"Coord[3].TS=20");
		theApp.m_FBCtrl.SendStr(L"Coord[3].TM=5");
		theApp.m_FBCtrl.SendStr(L"B3R");
		break;
	}
	
}

void CRobotCommsPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode) {
	case SB_LINEDOWN:
		m_nTopRow += 1;
		break;
	case SB_LINEUP:
		m_nTopRow -= 1;
		break;
	case SB_PAGEDOWN:
		m_nTopRow += m_nRowsL;
		break;
	case SB_PAGEUP:
		m_nTopRow -= m_nRowsL;
		break;
	case SB_THUMBTRACK:
		m_nTopRow = (int)m_scrollV.GetTrackPos();
		break;
	default:
		break;
	}
	if (m_nTopRow + m_nDisplayRows > theApp.m_FBCtrl.getRxArraySize()) {
		m_nTopRow = theApp.m_FBCtrl.getRxArraySize() - m_nDisplayRows;
	}
	if (m_nTopRow < 0) m_nTopRow = 0;

	m_scrollV.SetScrollRange(0.0, (double)m_nRowsL, FALSE);
	m_scrollV.SetPageSize((double)m_nDisplayRows, FALSE);
	m_scrollV.SetScrollPos((double)m_nTopRow, TRUE);

	RenderView();
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRobotCommsPage::OnChangeEditDcMinus() 
{
	CString Buff;
	int nTemp = theApp.m_FBCtrl.m_nDCRefMinus;

	m_editDCMinus.GetWindowText(Buff);
	theApp.m_FBCtrl.m_nDCRefMinus = _ttoi(Buff);
	if(nTemp - theApp.m_FBCtrl.m_nDCRefMinus) {
		m_spinDCMinus.SetPos(theApp.m_FBCtrl.m_nDCRefMinus);
		SendDCValues();
	}
}

void CRobotCommsPage::OnChangeEditDcPlus() 
{
	CString Buff;
	int nTemp = theApp.m_FBCtrl.m_nDCRefPlus;

	m_editDCPlus.GetWindowText(Buff);
	theApp.m_FBCtrl.m_nDCRefPlus = _ttoi(Buff);
	if(nTemp - theApp.m_FBCtrl.m_nDCRefPlus) {
		m_spinDCPlus.SetPos(theApp.m_FBCtrl.m_nDCRefPlus);
		SendDCValues();
	}
}

void CRobotCommsPage::OnChangeEditDcBand() 
{
	CString Buff;
	int nTemp = theApp.m_FBCtrl.m_nDCRefBand;

	m_editDCBand.GetWindowText(Buff);
	theApp.m_FBCtrl.m_nDCRefBand = _ttoi(Buff);
	if(nTemp - theApp.m_FBCtrl.m_nDCRefBand) {
		m_spinDCBand.SetPos(theApp.m_FBCtrl.m_nDCRefBand);
		SendDCValues();
	}
}

void CRobotCommsPage::SendDCValues()
{
	CString Buff;

	Buff.Format(L"ecat[0].DCRefBand=%d",theApp.m_FBCtrl.m_nDCRefBand);
	theApp.m_FBCtrl.SendStr(Buff);
	Buff.Format(L"ecat[0].DCRefPlus=%d",theApp.m_FBCtrl.m_nDCRefPlus);
	theApp.m_FBCtrl.SendStr(Buff);
	Buff.Format(L"ecat[0].DCRefMinus=%d",theApp.m_FBCtrl.m_nDCRefMinus);
	theApp.m_FBCtrl.SendStr(Buff);

}

void CRobotCommsPage::OnButtonTest() 
{
}

BOOL CRobotCommsPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

BOOL CRobotCommsPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	m_nEthercatStatus = -1;
	UpdateAllControls();
	SetTimer(1,200,NULL);
	
	return CPropertyPage::OnSetActive();
}

void CRobotCommsPage::OnButtonJogTosafe() 
{
	theApp.m_FBCtrl.SendStr(L"&1qa");
	theApp.m_FBCtrl.SendStr(L"&2qa");
	theApp.m_FBCtrl.SendStr(L"&3qa");
	theApp.m_FBCtrl.SendStr(L"undefine all");
	theApp.m_FBCtrl.SendStr(L"#1j=0#2j=0#3j=0#4j=0#5j=500000000#6j=1000000000#7j=0#8j=-400000000#9j=0");
	theApp.m_FBCtrl.SendStr(L"#11j=0#12j=0#13j=0#14j=0#15j=500000000#16j=1000000000#17j=0#18j=-400000000#19j=0");
	
}

void CRobotCommsPage::OnButtonPmacReset() 
{
	theApp.m_FBCtrl.SendStr(L"$$$");
	CWaitCursor Wait;

	Sleep(8000);

	theApp.m_FBCtrl.StartEthercat();
	Wait.Restore();
}

void CRobotCommsPage::OnButtonClear() 
{
	theApp.m_FBCtrl.m_RxArray.RemoveAll();
}

void CRobotCommsPage::OnCheckEchoData() 
{
	theApp.m_LastSettings.nFieldBusCollectStrings == 1 ? theApp.m_LastSettings.nFieldBusCollectStrings = 0 : theApp.m_LastSettings.nFieldBusCollectStrings = 1;
	m_checkEchoData.SetCheck(theApp.m_LastSettings.nFieldBusCollectStrings);
}

void CRobotCommsPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rrClip, rr;
	CRgn rgnView, rgnWatch;

	if (GetSafeHwnd() == NULL) return;

	//View
	RenderView();

}


void CRobotCommsPage::RenderView()
{
	CDC  dcMem;
	CBitmap bmp;
	CSize size;
	int		yy, nLine;
	static	bool bFlag = FALSE;
	CString Buff;
	CRect rr, rrView;


	CPaintDC* pDC = (CPaintDC*)m_staticView.GetDC();
	m_staticView.GetWindowRect(rrView);
	rrView -= rrView.TopLeft();

	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel / 8;
	if (nBytesPixel<0) nBytesPixel = 1;

	char	*pArray;
	pArray = new char[(rrView.Width() + 2) * (rrView.Height() + 2) * nBytesPixel];
	FillMemory(pArray, (rrView.Width() + 2) * (rrView.Height() + 2) * nBytesPixel, 0xff);

	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetTextColor(RGB(255, 255, 0));
	dcMem.SetBkMode(TRANSPARENT);
	Buff = "-100.0%";
	size = dcMem.GetTextExtent(Buff);
	
	m_nDisplayRows = rrView.Height() / size.cy;

	bmp.CreateCompatibleBitmap(pDC, rrView.Width(), rrView.Height());
	bmp.SetBitmapBits((DWORD)(rrView.Width()*rrView.Height()*nBytesPixel), pArray);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	dcMem.FillRect(rrView, &CBrush(RGB(0, 0, 255)));

	if (m_nTopRow < 0) m_nTopRow = 0;
	if (m_nRowsL > 0) {
		yy = 0;
		for (nLine = m_nTopRow; nLine < m_nRowsL; nLine++) {
			switch (theApp.m_FBCtrl.m_ArrayFlag[nLine]) {
			case RX_STR: dcMem.SetTextColor(RGB(200, 200, 200));
				break;
			case TX_STR: dcMem.SetTextColor(RGB(255, 255, 0));
				break;
			case ERROR_STR: dcMem.SetTextColor(RGB(255, 0, 0));
				break;
			}
			dcMem.TextOut(4, yy, theApp.m_FBCtrl.getRxArrayAt(nLine));
			yy += size.cy;
		}
	}

	pDC->BitBlt(rrView.left, rrView.top, rrView.Width(), rrView.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	dcMem.SetBkMode(OPAQUE);

	delete pArray;

	m_staticView.ReleaseDC(pDC);

}

void CRobotCommsPage::CreateColumns()
{
	int ColumnWidth[3] = { 0,100,100};
	CString ColumnName[3];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Label);
	ColumnName[2].LoadString(IDS_Value);

	CRect rr;
	m_listWatch.GetWindowRect(&rr);
	int nWidth = (rr.Width() - 4) / 2;
	ColumnWidth[1] = ColumnWidth[2] = nWidth;

	int nColumnCount = m_listWatch.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listWatch.DeleteColumn(0);
	}
	for (int ii=0;ii<3;ii++) {
		m_listWatch.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}
	m_listWatch.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CRobotCommsPage::FillList()
{
	CString Buff;

	m_listWatch.DeleteAllItems();
	for (int ii=0;ii<theApp.m_FBCtrl.m_strWatchArray.GetSize();ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listWatch.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listWatch.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<theApp.m_FBCtrl.m_strWatchArray.GetSize();ii++) 
		m_listWatch.SetItemState(ii , 0,LVIS_SELECTED);
	m_listWatch.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listWatch.SetFocus();

}


void CRobotCommsPage::OnClickListWatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	
	*pResult = 0;
}

void CRobotCommsPage::OnDblclkListWatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;


	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex=pDispInfo->item.mask;

	if(m_nIndex<0) {
		theApp.m_FBCtrl.m_strWatchArray.Add(L"");
		FillList();
		m_nIndex = theApp.m_FBCtrl.m_strWatchArray.GetSize() - 1;
	}

	if((m_nIndex>=0) && (m_nIndex<theApp.m_FBCtrl.m_strWatchArray.GetSize())) {
		m_listWatch.GetWindowRect(CtrlRect);
		m_listWatch.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:
			m_editBuff = theApp.m_FBCtrl.m_strWatchArray.GetAt(m_nIndex);
			m_pEditSpinItem->Initialize(&m_editBuff);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		}
	}	
	*pResult = 0;
}

void CRobotCommsPage::OnGetdispinfoListWatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	str[0] = 0;

	int nRow = pDispInfo->item.iItem;

	if(nRow>=0 && nRow<theApp.m_FBCtrl.m_strWatchArray.GetSize()) {

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,100,L"%s",(LPCWSTR)theApp.m_FBCtrl.m_strWatchArray.GetAt(nRow));
			break;
		case 2:
			swprintf_s(str,L"%.0f",theApp.m_FBCtrl.m_fWatchArray[nRow]);
			break;
		}
	}	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

HRESULT CRobotCommsPage::ItemChanged(WPARAM wParam, LPARAM lParam)
{
	switch ((int)wParam) {
	default:
		theApp.m_FBCtrl.m_strWatchArray.SetAt(m_nIndex, m_editBuff);
		break;
	case WM_ITEMFINISHED: FillList();
		break;
	}
	return true;
}


void CRobotCommsPage::OnWatchDelete() 
{
	if(m_nIndex>=0 && m_nIndex < theApp.m_FBCtrl.m_strWatchArray.GetSize()) {
		theApp.m_FBCtrl.m_strWatchArray.RemoveAt(m_nIndex);
	}
	FillList();
}

void CRobotCommsPage::OnRclickListWatch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint	Point;


	POSITION pos = m_listWatch.GetFirstSelectedItemPosition();
	m_nIndex=m_listWatch.GetNextSelectedItem(pos);



	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_WATCH_MENU)) {
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
	}
	
	delete menu;

	*pResult = 0;
}

void CRobotCommsPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	int nMenu = IDR_RB_ROBOT_COMMS_MENU;
	CRect rr;

	GetCursorPos(&Point);

	if (menu->LoadMenu(nMenu)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
	}
	delete menu;
	
}

void CRobotCommsPage::OnCommsClearall() 
{
	theApp.m_FBCtrl.m_RxArray.RemoveAll();
	ZeroMemory(theApp.m_FBCtrl.m_ArrayFlag, sizeof theApp.m_FBCtrl.m_ArrayFlag);
}
