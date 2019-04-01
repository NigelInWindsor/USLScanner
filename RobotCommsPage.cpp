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
	DDX_Control(pDX, IDC_EDIT_ETHERCAT_STATUS, m_editEthercatStatus);
	DDX_Control(pDX, IDC_EDIT_TX_STR, m_editInLineCommand);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_IPADDRESS_ROBOT, m_ipAddress);
	DDX_Control(pDX, IDC_CHECK_SELECT_AT_STARTUP, m_checkSelectAtStartUp);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_MESSAGES, m_listMessages);
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
	ON_COMMAND(ID_COMMS_CLEARALL, OnCommsClearall)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_MESSAGES, &CRobotCommsPage::OnLvnGetdispinfoListMessages)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MESSAGES, &CRobotCommsPage::OnNMRClickListMessages)
	ON_COMMAND(ID_COMMS_COPY, &CRobotCommsPage::OnCommsCopy)
	ON_COMMAND(ID_EDIT_COPY, &CRobotCommsPage::OnEditCopy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotCommsPage message handlers


BOOL CRobotCommsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

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

	switch (pMsg->message) {
	case WM_KEYUP:
		m_bCtrlButton = false;
		break;
	case WM_KEYDOWN:
		switch (pMsg->wParam) {
		case 17: m_bCtrlButton = true;//Ctrl button down
			break;
		case 'c':
		case 'C':
			if (m_bCtrlButton == true) {
				OnEditCopy();
				return true;
			};
			break;
		case 'v':
		case 'V':
			if (m_bCtrlButton == true) {
				OnEditPaste();
				return true;
			};
			break;
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



void CRobotCommsPage::OnButtonEnable() 
{
	theApp.m_FBCtrl.EnableMotors();
}

void CRobotCommsPage::OnButtonKill() 
{
	theApp.m_FBCtrl.KillMotors();
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


void CRobotCommsPage::OnButtonClear() 
{
	theApp.m_FBCtrl.m_RxArray.RemoveAll();
}



void CRobotCommsPage::OnCommsClearall() 
{
	theApp.m_FBCtrl.m_RxArray.RemoveAll();
	ZeroMemory(theApp.m_FBCtrl.m_ArrayFlag, sizeof theApp.m_FBCtrl.m_ArrayFlag);
}

void CRobotCommsPage::CreateColumns()
{
	int ColumnWidth[2] = { 0, 120 };
	CString ColumnName[2] = { L"#", L"Enable" };
	CRect rrList;

	CDC* pDC = m_listMessages.GetDC();

	int nColumnCount = (&m_listMessages.GetHeaderCtrl())->GetItemCount();
	for (int ii = 0; ii < nColumnCount; ii++) m_listMessages.DeleteColumn(0);
	for (int ii = 0; ii < 2; ii++) {
		m_listMessages.InsertColumn(ii, ColumnName[ii], LVCFMT_LEFT, ColumnWidth[ii]);
	}
	m_listMessages.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listMessages.ReleaseDC(pDC);
	m_listMessages.GetWindowRect(rrList);
	ScreenToClient(rrList);
	m_listMessages.SetColumnWidth(1, rrList.Width() * 10);
	m_listMessages.SetBkColor(RGB(0, 0, 255));

}

void CRobotCommsPage::FillList()
{
	CString Buff;

	m_listMessages.DeleteAllItems();
	m_listMessages.setCellRowColumns(1000, 2);

}

void CRobotCommsPage::OnTimer(UINT_PTR nIDEvent)
{
	CString Buff;
	int nRxSize, nListSize;

	if (GetSafeHwnd()) {
		if ((nRxSize = theApp.m_FBCtrl.getRxArraySize()) != (nListSize = m_listMessages.GetItemCount())) {
			if (nRxSize > 200 && nRxSize < nListSize) {
				int nCount = nListSize - nRxSize;
				for (int ii = 0; ii < nCount; ii++) {
					m_listMessages.DeleteItem(0);
				}
			}
			else {
				for (int ii = nListSize; ii < nRxSize; ii++) {
					Buff.Format(_T("%d"), ii + 1);
					m_listMessages.InsertItem(ii, Buff.GetBuffer(255), ii);
				};
			}
			m_listMessages.EnsureVisible(nRxSize - 1, FALSE);
		}
	}

	if (m_nEthercatStatus != theApp.m_Ethercat.m_nBusStatus) {
		if (theApp.m_Ethercat.m_nBusStatus & ETHERCAT_BUS_ON) {
			m_editEthercatStatus.SetWindowText(L"On");
		}
		else {
			m_editEthercatStatus.SetWindowText(L"Off");
		}
		m_nEthercatStatus = theApp.m_Ethercat.m_nBusStatus;
	}

	CPropertyPage::OnTimer(nIDEvent);
}




void CRobotCommsPage::OnLvnGetdispinfoListMessages(NMHDR *pNMHDR, LRESULT *pResult)
{
	//	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[200];

	str[0] = 0;
	pDispInfo->item.pszText = str;
	int nRow = pDispInfo->item.iItem;

	if (nRow < theApp.m_FBCtrl.getRxArraySize()) {
		switch (pDispInfo->item.iSubItem) {
		case 0:	swprintf_s(str, 200, _T("%d"), pDispInfo->item.iItem + 1);
			break;
		case 1:	swprintf_s(str, 200, _T("%s"), (LPCWSTR)theApp.m_FBCtrl.getRxArrayAt(nRow).GetBuffer());
			break;
		}
		if (theApp.m_FBCtrl.GetRxFlagAt(nRow) == ERROR_STR) {
			m_listMessages.setCellColor(nRow, 1, RGB(255, 0, 0), RGB(0, 0, 255));
		}
		else {
			m_listMessages.setCellColor(nRow, 1, RGB(200, 200, 200), RGB(0, 0, 255));
		}
	}

	*pResult = 0;
}



void CRobotCommsPage::OnNMRClickListMessages(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	int nMenu = IDR_RB_ROBOT_COMMS_MENU;
	CRect rr;

	GetCursorPos(&Point);

	if (menu->LoadMenu(nMenu)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);
	}
	delete menu;

	*pResult = 0;
}


void CRobotCommsPage::OnCommsCopy()
{
	HGLOBAL hClipboardData;
	WCHAR * pchData;
	CString Buff;

	Buff.Empty();
	POSITION pos = m_listMessages.GetFirstSelectedItemPosition();
	while (pos) {
		int nIndex = m_listMessages.GetNextSelectedItem(pos);
		Buff += theApp.m_FBCtrl.getRxArrayAt(nIndex);
		Buff += "\r\n";
	}

	if (OpenClipboard())
	{
		EmptyClipboard();
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, (Buff.GetLength() + 1) * sizeof WCHAR);
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		for (int ii = 0; ii < Buff.GetLength(); ii++) {
			pchData[ii] = Buff.GetAt(ii);
		}
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_UNICODETEXT, hClipboardData);
		CloseClipboard();
	}
}


void CRobotCommsPage::OnEditCopy()
{
	HGLOBAL hClipboardData;
	WCHAR * pchData;
	CString Buff;

	Buff.Empty();
	POSITION pos = m_listMessages.GetFirstSelectedItemPosition();
	while (pos) {
		int nIndex = m_listMessages.GetNextSelectedItem(pos);
		Buff += theApp.m_FBCtrl.getRxArrayAt(nIndex);
		Buff += "\r\n";
	}

	if (OpenClipboard())
	{
		EmptyClipboard();
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, (Buff.GetLength() + 1) * sizeof WCHAR);
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		for (int ii = 0; ii < Buff.GetLength(); ii++) {
			pchData[ii] = Buff.GetAt(ii);
		}
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_UNICODETEXT, hClipboardData);
		CloseClipboard();
	}
}


void CRobotCommsPage::OnEditPaste()
{
	CString Buff;

	if(OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		WCHAR * buffer = (WCHAR*)GlobalLock(hData);
		Buff.Format(L"%s", buffer);

		GlobalUnlock(hData);
		CloseClipboard();

		theApp.m_FBCtrl.SendStr(Buff, _TERMINAL);
	}
}