// FaroTrackerPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FaroTrackerPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaroTrackerPage property page

WCHAR m_IPAddress[256];
//Tracker* gpTracker;
//CBlocking gBlocking;
//CWorkerThread gWorkerThread;
//CListeners gListeners;
//RUNMODE gRunMode;


IMPLEMENT_DYNCREATE(CFaroTrackerPage, CPropertyPage)

CFaroTrackerPage::CFaroTrackerPage() : CPropertyPage(CFaroTrackerPage::IDD)
{
	//{{AFX_DATA_INIT(CFaroTrackerPage)
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;
	theApp.m_nElementIndex = 0;
	ZeroMemory(theApp.m_nListOrderArray, sizeof theApp.m_nListOrderArray);
	m_nOrder = 0;
	m_bCtrlButton = false;
	m_bShiftButton = false;
	m_FileName.Empty();
	m_pFaroElementExpansionDlg = NULL;

	RetrieveTracker();

}

CFaroTrackerPage::~CFaroTrackerPage()
{
	SAFE_DELETE( m_pFaroElementExpansionDlg );
	SaveTracker();
}

void CFaroTrackerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFaroTrackerPage)
	DDX_Control(pDX, IDC_COMBO_REMOTE_PAGE_DOWN, m_comboPageDown);
	DDX_Control(pDX, IDC_COMBO_REMOTE_PAGE_UP, m_comboPageUp);
	DDX_Control(pDX, IDC_EDIT_CLOUD_LENGTH, m_editCloudLength);
	DDX_Control(pDX, IDC_IPADDRESS_FARO, m_ipAddress);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
	DDX_Control(pDX, IDC_EDIT_ELEMENT_STATS, m_editElementStats);
	DDX_Control(pDX, IDC_LIST_ELEMENTS, m_listElements);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	DDX_Control(pDX, IDC_EDIT_COORDINATE, m_editCoordinate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFaroTrackerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFaroTrackerPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ELEMENTS, OnRclickListElements)
	ON_COMMAND(ID_BUTTON_CONNECT, OnButtonConnect)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_MEASURE, OnButtonMeasure)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_COMMAND(ID_BUTTON_ADD, OnButtonAdd)
	ON_LBN_SELCHANGE(IDC_LIST_ELEMENTS, OnSelchangeListElements)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_COMMAND(ID_FAROCOORDS_DELETE, OnFarocoordsDelete)
	ON_COMMAND(ID_FAROCOORDS_LINE, OnFarocoordsLine)
	ON_COMMAND(ID_FAROCOORDS_ARC, OnFarocoordsArc)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_COMMAND(ID_BUTTON_START_UP, OnButtonStartUp)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ELEMENTS, OnGetdispinfoListElements)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ELEMENTS, OnClickListElements)
	ON_COMMAND(ID_FAROCOORDS_ADD000, OnFarocoordsAdd000)
	ON_COMMAND(ID_FAROELEMENT_DELETE, OnFaroelementDelete)
	ON_COMMAND(ID_FAROCOORDS_LOOKATTHISPOINT, OnFarocoordsLookatthispoint)
	ON_COMMAND(ID_FAROCOORDS_FRAME_ORIGIN, OnFarocoordsFrameOrigin)
	ON_COMMAND(ID_FAROCOORDS_FRAME_XVECTOR, OnFarocoordsFrameXvector)
	ON_COMMAND(ID_FAROCOORDS_FRAME_YVECTOR, OnFarocoordsFrameYvector)
	ON_COMMAND(ID_FAROCOORDS_TOOLTIP_ORIGIN, OnFarocoordsTooltipOrigin)
	ON_COMMAND(ID_BUTTON_WORLD_COORDINATES, OnButtonWorldCoordinates)
	ON_COMMAND(ID_BUTTON_TOOL_COORDINATES, OnButtonToolCoordinates)
	ON_COMMAND(ID_BUTTON_FRAME_COORDINATES, OnButtonFrameCoordinates)
	ON_COMMAND(ID_BUTTON_DELETE, OnButtonDelete)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_BUTTON_POLAR, OnButtonPolar)
	ON_COMMAND(ID_BUTTON_RECTANGULAR, OnButtonRectangular)
	ON_COMMAND(ID_FAROELEMENT_CIRCUMCENTERTOLIST, OnFaroelementCircumcentertolist)
	ON_COMMAND(ID_FAROCOORDS_TOOLTIP_PLANE, OnFarocoordsTooltipPlane)
	ON_COMMAND(ID_FAROELEMENT_EXPAND, OnFaroelementExpand)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_FARO, OnFieldchangedIpaddress1)
	ON_COMMAND(ID_FAROCOORDS_FRAME_HORIZONTALPLANE, OnFarocoordsFrameHorizontalplane)
	ON_COMMAND(ID_FAROCOORDS_FRAME_VERTICALPLANE, OnFarocoordsFrameVerticalplane)
	ON_COMMAND(ID_FAROCOORDS_FRAME_ZVECTOR, OnFarocoordsFrameZvector)
	ON_COMMAND(ID_FAROCOORDS_ANGLE, OnFarocoordsAngle)
	ON_EN_CHANGE(IDC_EDIT_CLOUD_LENGTH, OnChangeEditCloudLength)
	ON_COMMAND(ID_BUTTON_EARTH_LEVEL, OnButtonEarthLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_REMOTE_PAGE_UP, OnSelchangeComboRemotePageUp)
	ON_CBN_SELCHANGE(IDC_COMBO_REMOTE_PAGE_DOWN, OnSelchangeComboRemotePageDown)
	ON_COMMAND(ID_FAROCOORDS_TRANSFER_TAUGHTPOINTSNEWLINE,OnFarocoordsTransferTaughtPointsNewLine)
	ON_COMMAND(ID_FAROCOORDS_TRANSFER_REFERENCEPOINTS,OnFaroCoordsTransferReferencePoints)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaroTrackerPage message handlers
int CFaroTrackerPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_FARO_TRACKER_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL CFaroTrackerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

/*	Buff.LoadString(IDS_Page_Up);	m_comboRemoteAddButton.AddString(Buff);
	Buff.LoadString(IDS_Page_Down);	m_comboRemoteAddButton.AddString(Buff);

  	Buff.LoadString(IDS_Page_Up);	m_comboCloudButton.AddString(Buff);
	Buff.LoadString(IDS_Page_Down);	m_comboCloudButton.AddString(Buff);
*/	
	
	Buff.LoadString(IDS_No_action);				m_comboPageUp.AddString(Buff);
	Buff.LoadString(IDS_ADD_POINT);				m_comboPageUp.AddString(Buff);
	Buff.LoadString(IDS_ADD_CLOUD_POINTS);		m_comboPageUp.AddString(Buff);
	Buff.LoadString(IDS_Move_Points_to_New_Taught_Line);		m_comboPageUp.AddString(Buff);

	Buff.LoadString(IDS_No_action);				m_comboPageDown.AddString(Buff);
	Buff.LoadString(IDS_ADD_POINT);				m_comboPageDown.AddString(Buff);
	Buff.LoadString(IDS_ADD_CLOUD_POINTS);		m_comboPageDown.AddString(Buff);
	Buff.LoadString(IDS_Move_Points_to_New_Taught_Line);		m_comboPageDown.AddString(Buff);


	
	CreateColumns();
	FillList();
	FillElementList();

	//128.128.128.100
	if (theApp.m_LastSettings.nFaroTrackerIPAddrs == 0) theApp.m_LastSettings.nFaroTrackerIPAddrs = 0x80808064;

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CFaroTrackerPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CFaroTrackerPage::OnButtonConnect() 
{
	if(theApp.m_pFaroTracker == NULL) {
		if (CheckAllFilesExist() == false) {
			CString text = L"\nPlease check that the JRE folder and the jar files are in\nthe current working directory and restart the application.";
			AfxMessageBox(text, MB_ICONSTOP);
			return;
		}

		BeginWaitCursor();

		try
		{
			switch (theApp.m_LastSettings.nFaroTrackerType = 1) {
			case 0: theApp.m_pFaroTracker = new Tracker((FAROLPTSTR)L"Tracker4xxx");
				break;
			case 1: //theApp.m_pFaroTracker = new Tracker(L"TrackerKeystone"); //?
				theApp.m_pFaroTracker = new Tracker((FAROLPTSTR)L"TrackerKeystone"); //?
				break;
			case 2: theApp.m_pFaroTracker = new Tracker((FAROLPTSTR)L"TrackerKeystoneSim");
				break;
			}
		
		}
		catch(TrackerException* e)
		{
			WCHAR* msg = (WCHAR*)e->getText(); 
			AfxMessageBox(msg);
			//?? trackerDeleteString((FAROLPTSTR)msg);  
			delete e;
			return;
		}
		catch(SMXException* e)
		{
			WCHAR msg[1024];
			e->getMsg(msg, 1024);
			CString text = L"\nPlease download and install Java\nEnsure USLScanner is digitally signed and running in Administrator mode";
			wcscat_s(msg, text.GetBuffer(0));
			AfxMessageBox(msg, MB_ICONSTOP);
			delete e;
			return;
		}

		sockaddr_in Server;
		Server.sin_addr.s_addr = theApp.m_LastSettings.nFaroTrackerIPAddrs;
		swprintf_s(m_IPAddress, L"%d.%d.%d.%d",Server.sin_addr.S_un.S_un_b.s_b4,Server.sin_addr.S_un.S_un_b.s_b3,Server.sin_addr.S_un.S_un_b.s_b2,Server.sin_addr.S_un.S_un_b.s_b1);

		theApp.m_FaroWorkerThread.CreateThread();
		theApp.m_FaroWorkerThread.OnConnect(m_IPAddress);
		
		EndWaitCursor();
	} else {
		theApp.m_FaroWorkerThread.OnDisconnect();
		SAFE_DELETE( theApp.m_pFaroTracker );

	}

}

void CFaroTrackerPage::OnButtonStartUp() 
{
	theApp.m_FaroWorkerThread.ExecuteCommandSequence();
}

BOOL CFaroTrackerPage::OnSetActive() 
{
	SetTimer(1,300,NULL);
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CFaroTrackerPage::OnKillActive() 
{
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}


void CFaroTrackerPage::OnButtonMeasure() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(theApp.m_FaroWorkerThread.m_bCollectingBackGround != TRUE) {
		theApp.m_FaroWorkerThread.OnStartBackgndMeasurements();
		pFrame->SendMessage(UI_VIEW_LASER_TRACKER_COORDINATES);
	} else {
		theApp.m_FaroWorkerThread.OnStopBackgndMeasurements();
	}
}

void CFaroTrackerPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	sockaddr_in Server;

	Server.sin_addr.s_addr = theApp.m_LastSettings.nFaroTrackerIPAddrs;
	m_ipAddress.SetAddress(Server.sin_addr.s_addr); 

	m_comboPageDown.SetCurSel(theApp.m_LastSettings.nFaroRemotePageDown);
	m_comboPageUp.SetCurSel(theApp.m_LastSettings.nFaroRemotePageUp);

	Buff.Format(L"%.1f mm",theApp.m_LastSettings.fFaroCloudLength);
	m_editCloudLength.SetWindowText(Buff);


	SetToolBarCheckedState();
	SetAccessPrivelages();
}

void CFaroTrackerPage::SetAccessPrivelages()
{
	bool bFlag = false;

	if(theApp.m_pFaroTracker != NULL) {
		if(theApp.m_FaroWorkerThread.m_bConnected == TRUE ) {
			bFlag = true;
		}
	}



}

void CFaroTrackerPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	D3DXVECTOR3 vPos,vAZD;
	int nStatus;


	Buff.Format(L"%s",theApp.m_FaroWorkerThread.m_strMessage);
	m_editMessage.SetWindowText(Buff);

//	if(theApp.m_FaroWorkerThread.m_bNewValueAvailable == true) {

		theApp.m_FaroWorkerThread.m_bNewValueAvailable = false;

		theApp.m_FaroWorkerThread.GetAZDCoordinate(&vAZD);
		nStatus = theApp.m_FaroWorkerThread.GetRawXYZCoordinate(&vPos);
		
		Buff.Format(L"Azimuth \t%.03f\r\nZenith \t%.03f\r\nDistance \t%.03f\r\nX\t%.03f\r\nY\t%.03f\r\nZ\t%.03f\r\nLevel\r\nRX\t%.04f\x0b0\r\nRY\t%.04f\x0b0\r\nRZ\t%.04f\x0b0",
					vAZD.x,vAZD.y,vAZD.z,vPos.x,vPos.y,vPos.z,
					theApp.m_FaroWorkerThread.m_dLevelRX*RAD_TO_DEG,theApp.m_FaroWorkerThread.m_dLevelRY*RAD_TO_DEG,theApp.m_FaroWorkerThread.m_dLevelRZ*RAD_TO_DEG);
		m_editCoordinate.SetWindowText(Buff);
//	}

	SetToolBarCheckedState();

	CPropertyPage::OnTimer(nIDEvent);
}


void CFaroTrackerPage::SetToolBarCheckedState()
{
	int	nIndex,nFlags;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_CONNECT))>=0)
		theApp.m_FaroWorkerThread.m_bConnected==TRUE ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	nFlags = 0;
	if(theApp.m_FaroWorkerThread.m_bConnected==FALSE) {
		nFlags = TBBS_DISABLED;
	}
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_START_UP))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlags);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_WARM_UP))>=0)
		theApp.m_FaroWorkerThread.m_nInitializationStatus & 0x01 ?  m_wndToolBar.SetButtonStyle(nIndex,nFlags|TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,nFlags);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_MOTORS))>=0) 
		theApp.m_FaroWorkerThread.m_nInitializationStatus & 0x02 ?  m_wndToolBar.SetButtonStyle(nIndex,nFlags|TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,nFlags);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ENCODER))>=0) 
		theApp.m_FaroWorkerThread.m_nInitializationStatus & 0x04 ?  m_wndToolBar.SetButtonStyle(nIndex,nFlags|TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,nFlags);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_INTENSITY))>=0) 
		theApp.m_FaroWorkerThread.m_nInitializationStatus & 0x08 ?  m_wndToolBar.SetButtonStyle(nIndex,nFlags|TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,nFlags);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ADM))>=0) 
		theApp.m_FaroWorkerThread.m_nInitializationStatus & 0x10 ?  m_wndToolBar.SetButtonStyle(nIndex,nFlags|TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,nFlags);

	theApp.m_FaroWorkerThread.m_nInitializationStatus & 0x3f ? nFlags : nFlags = TBBS_DISABLED;
	
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_MEASURE))>=0) 
		theApp.m_FaroWorkerThread.m_bCollectingBackGround == TRUE ? m_wndToolBar.SetButtonStyle(nIndex,nFlags|TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,nFlags);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_EARTH_LEVEL))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlags);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ADD))>=0) m_wndToolBar.SetButtonStyle(nIndex,nFlags);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_WORLD_COORDINATES))>=0) 
		theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_FRAME_COORDINATES))>=0) 
		theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode == 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_TOOL_COORDINATES))>=0) 
		theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode == 2 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RECTANGULAR))>=0) 
		theApp.m_LastSettings.nFaroDisplayRectangularPolar == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POLAR))>=0) 
		theApp.m_LastSettings.nFaroDisplayRectangularPolar == 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

}

BOOL CFaroTrackerPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_CONNECT: strTipText.LoadString(IDS_Connect);
	   break;
	case ID_BUTTON_MEASURE: strTipText.LoadString(IDS_Measure);
	   break;
	case ID_BUTTON_EARTH_LEVEL: strTipText.LoadString(IDS_Earth_Level);
	   break;
	case ID_BUTTON_ADD: strTipText.LoadString(IDS_Add);
	   break;
	case ID_BUTTON_WORLD_COORDINATES: strTipText.LoadString(IDS_World_coordinates);
	   break;
	case ID_BUTTON_FRAME_COORDINATES: strTipText.LoadString(IDS_Frame_coordinates);
	   break;
	case ID_BUTTON_TOOL_COORDINATES: strTipText.LoadString(IDS_Tool_coordinates);
	   break;
	case ID_BUTTON_RECTANGULAR: strTipText.LoadString(IDS_Rectangular_coordinates);
	   break;
	case ID_BUTTON_POLAR: strTipText.LoadString(IDS_Polar_coordinates);
	   break;
	case ID_FILE_OPEN: strTipText.LoadString(IDS_Open);
	   break;
	case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save_As);
	   break;
	}
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CFaroTrackerPage::CreateColumns()
{
	int ColumnWidth[6] = { 26,26,50,50,50,20};
	CString ColumnName[6];
	CRect rr;

	m_listCoords.GetWindowRect(&rr);

	ColumnName[0]= "#";
	ColumnName[1]= "^";
	ColumnName[2]= "X";
	ColumnName[3]= "Y";
	ColumnName[4]= "Z";
	ColumnName[5]= "Z";
	ColumnWidth[2] = (rr.Width() - (ColumnWidth[0]+ColumnWidth[1]+ColumnWidth[5])-4) / 3;
	ColumnWidth[3] = (rr.Width() - (ColumnWidth[0]+ColumnWidth[1]+ColumnWidth[5])-4) / 3;
	ColumnWidth[4] = (rr.Width() - (ColumnWidth[0]+ColumnWidth[1]+ColumnWidth[5])-4) / 3;

	int nColumnCount = m_listCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}
	for (int ii=0;ii<5;ii++) {
		m_listCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_RIGHT,ColumnWidth[ii]);
	}
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CFaroTrackerPage::FillList()
{
	CString Buff;

	m_listCoords.DeleteAllItems();
	for (int ii=0;ii<PROFILE->m_vCloud.GetSize();ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	for(int ii=0;ii<PROFILE->m_vCloud.GetSize();ii++) 
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoords.SetItemState(PROFILE->m_vCloud.GetSize()-1 , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoords.EnsureVisible(PROFILE->m_vCloud.GetSize()-1,FALSE);
	m_listCoords.SetFocus();
	Update3DView();

}

void CFaroTrackerPage::FillElementList()
{
	CString Buff, Temp;

	m_listElements.DeleteAllItems();
	for(int nIndex=0;nIndex<theApp.m_FaroWorkerThread.m_nElementL;nIndex++) {

		switch(theApp.m_FaroWorkerThread.m_Element[nIndex].m_Type) {
		case VA_LINE:	Buff.LoadString(IDS_Line);
			break;
		case VA_ARC:	Buff.LoadString(IDS_Arc);
			break;
		case VA_ANGLE:	Buff.LoadString(IDS_Angle);
			break;
		case VA_CIRCLE:	Buff.LoadString(IDS_Circle);
			break;
		case VA_X_VECTOR:	Buff.LoadString(IDS_X_Vector);
			break;
		case VA_Y_VECTOR:	Buff.LoadString(IDS_Y_Vector);
			break;
		case VA_HORIZ_PLANE:	Buff.LoadString(IDS_Horizontal_Plane);
			break;
		}
		m_listElements.InsertItem(nIndex, Buff.GetBuffer(255), nIndex);
	}
	Update3DView();


}

void CFaroTrackerPage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn = pDispInfo->item.iItem;
	D3DXVECTOR3 vPt;

	if(PROFILE->m_vCloud.GetSize()<=0) return;
	PROFILE->m_vCloud.GetPt(nn, &vPt);
	switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
	default:
		break;
	case 1:	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt, &vPt);
		break;
	case 2:	theApp.m_FaroWorkerThread.GetToolCoordinate(&vPt, &vPt);
		break;
	}


	switch(pDispInfo->item.iSubItem) {
	case 0: swprintf_s(str,L"%d",nn+1);
		break;
	case 1:
		if(theApp.m_nListOrderArray[nn]) {
			swprintf_s(str,L"%d",theApp.m_nListOrderArray[nn]);
		} else {
			str[0]=0;
		}
		break;
	case 2:
		swprintf_s(str,L"%.03f",vPt.x);
		break;
	case 3:
		swprintf_s(str,L"%.03f",vPt.y);
		break;
	case 4:
		swprintf_s(str,L"%.03f",vPt.z);
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CFaroTrackerPage::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	m_nIndex=m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_FARO_COORD_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

bool CFaroTrackerPage::AddCloudPoint()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	if(theApp.m_FaroWorkerThread.AddCloudCoordinate() == true) {
		FillList();

		pFrame->SendMessage(UI_TRACKER_COORDINATE_ADDED);

		return true;
	}
	return false;

}


void CFaroTrackerPage::OnButtonAdd() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	theApp.m_FaroWorkerThread.AddCoordinate();
	FillList();

	pFrame->SendMessage(UI_TRACKER_COORDINATE_ADDED);

}

void CFaroTrackerPage::RetrieveTracker()
{
	CFileException e;
	CUSLFile File;
	CString FilePath,Buff;

	FilePath = theApp.m_LastSettingsPath + L"\\Tracker.set";

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
	} else {
		RetrieveTracker(&File);
		File.Close();
	}

}

void CFaroTrackerPage::RetrieveTracker(CUSLFile *pFile)
{
	int	ii,nVertexOffset,nFrameOffset,nElementOffset,nToolOffset;
	float	fVersion;

	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner Tracker"};
	char str[60];
	int nStartOffset = (int)pFile->GetPosition();

	memset(str,0,sizeof str);
	pFile->Read(str,strlen(Identifier));
	if(strcmp(str,Identifier)) {
		MessageBox(L"Incompatible Tank file type",L"Error",MB_ICONERROR);
	}
	pFile->Read(&fVersion,sizeof fVersion);

	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Read(&nVertexOffset,sizeof nVertexOffset);
	pFile->Read(&nFrameOffset,sizeof nFrameOffset);
	pFile->Read(&nElementOffset,sizeof nElementOffset);
	pFile->Read(&nToolOffset,sizeof nToolOffset);

	if(nVertexOffset) {
		pFile->Seek(nVertexOffset,CFile::begin);
		PROFILE->m_vCloud.Retrieve(pFile);
	}

	if(nFrameOffset) {
		pFile->Seek(nFrameOffset,CFile::begin);
		pFile->Read(&theApp.m_FaroWorkerThread.m_vFrameRotate,	sizeof theApp.m_FaroWorkerThread.m_vFrameRotate);
		pFile->Read(&theApp.m_FaroWorkerThread.m_vFrameNorm,		sizeof theApp.m_FaroWorkerThread.m_vFrameNorm);
		pFile->Read(&theApp.m_FaroWorkerThread.m_vFrameOrigin,	sizeof theApp.m_FaroWorkerThread.m_vFrameOrigin);
	}

	if(nToolOffset) {
		pFile->Seek(nToolOffset,CFile::begin);
		pFile->Read(&theApp.m_FaroWorkerThread.m_vToolRotate,		sizeof theApp.m_FaroWorkerThread.m_vToolRotate);
		pFile->Read(&theApp.m_FaroWorkerThread.m_vToolNorm,		sizeof theApp.m_FaroWorkerThread.m_vToolNorm);
		pFile->Read(&theApp.m_FaroWorkerThread.m_vToolOrigin,		sizeof theApp.m_FaroWorkerThread.m_vToolOrigin);
	}
	
	if(nElementOffset) {
		pFile->Seek(nElementOffset,CFile::begin);
		pFile->Read(&theApp.m_FaroWorkerThread.m_nElementL,sizeof theApp.m_FaroWorkerThread.m_nElementL);
		for(ii=0;ii<theApp.m_FaroWorkerThread.m_nElementL;ii++) {
			theApp.m_FaroWorkerThread.m_Element[ii].Retrieve(pFile);
		};
	}
}

void CFaroTrackerPage::SaveTracker()
{
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff ;
	FilePath = theApp.m_LastSettingsPath + L"\\Tracker.set";

	::CreateDirectory(theApp.m_LastSettingsPath,NULL);

	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(Buff,_T("Warning"), MB_ICONERROR);
	} else {
		SaveTracker(&File);
		File.Close();
	}

}

void CFaroTrackerPage::SaveTracker(CUSLFile *pFile)
{
	int	ii,nVertexOffset,nFrameOffset,nElementOffset,nToolOffset;
	int nOffsetArray[0x100];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);
	char Identifier[60]={"Ultrasonic Sciences Ltd USLScanner Tracker"};
	int nStartOffset = (int)pFile->GetPosition();

	//Write the identifier
	pFile->Write(Identifier,strlen(Identifier));
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(nVertexOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	PROFILE->m_vCloud.Save(pFile);

	pFile->Seek(nFrameOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&theApp.m_FaroWorkerThread.m_vFrameRotate,	sizeof theApp.m_FaroWorkerThread.m_vFrameRotate);
	pFile->Write(&theApp.m_FaroWorkerThread.m_vFrameNorm,		sizeof theApp.m_FaroWorkerThread.m_vFrameNorm);
	pFile->Write(&theApp.m_FaroWorkerThread.m_vFrameOrigin,	sizeof theApp.m_FaroWorkerThread.m_vFrameOrigin);

	pFile->Seek(nToolOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&theApp.m_FaroWorkerThread.m_vToolRotate,	sizeof theApp.m_FaroWorkerThread.m_vToolRotate);
	pFile->Write(&theApp.m_FaroWorkerThread.m_vToolNorm,		sizeof theApp.m_FaroWorkerThread.m_vToolNorm);
	pFile->Write(&theApp.m_FaroWorkerThread.m_vToolOrigin,	sizeof theApp.m_FaroWorkerThread.m_vToolOrigin);

	pFile->Seek(nElementOffset=(int)pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&theApp.m_FaroWorkerThread.m_nElementL,sizeof theApp.m_FaroWorkerThread.m_nElementL);
	for(ii=0;ii<theApp.m_FaroWorkerThread.m_nElementL;ii++) {
		theApp.m_FaroWorkerThread.m_Element[ii].Save(pFile);
	};


	int nFinishOffset = (int)pFile->GetPosition();

	//Write the offsets
	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Write(nOffsetArray,0x100);
	pFile->Seek(0x100+nStartOffset,CFile::begin);
	pFile->Write(&nVertexOffset,sizeof nVertexOffset);
	pFile->Write(&nFrameOffset,sizeof nFrameOffset);
	pFile->Write(&nElementOffset,sizeof nElementOffset);
	pFile->Write(&nToolOffset,sizeof nToolOffset);

	pFile->Seek(nFinishOffset,CFile::begin);

}


void CFaroTrackerPage::OnSelchangeListElements() 
{
}


void CFaroTrackerPage::OnFarocoordsDelete() 
{
	int nStartIndex,nFinishIndex;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	nStartIndex=(int)pos;
	nFinishIndex=nStartIndex=m_listCoords.GetNextSelectedItem(pos);
	while(pos) {
		nFinishIndex=m_listCoords.GetNextSelectedItem(pos);
	}

	PROFILE->m_vCloud.Delete(nStartIndex,nFinishIndex);
	FillList();
	
}

void CFaroTrackerPage::OnFarocoordsLine() 
{
	FillNewElement(VA_LINE);
}

void CFaroTrackerPage::OnFarocoordsArc() 
{
	FillNewElement(VA_ARC);
}

void CFaroTrackerPage::OnFarocoordsAngle() 
{
	FillNewElement(VA_ANGLE);
}


void CFaroTrackerPage::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	int nIndex=pDispInfo->item.mask;
	if(m_bShiftButton == false && m_bCtrlButton == false) {
		ZeroMemory(theApp.m_nListOrderArray,sizeof theApp.m_nListOrderArray);
		m_nOrder = 0;
		theApp.m_nListOrderArray[nIndex] = ++m_nOrder;
	}

	if(m_bCtrlButton == true) {
		theApp.m_nListOrderArray[nIndex] = ++m_nOrder;
	}

	if(m_bShiftButton == true) {

		POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
		while(pos) {
			nIndex=m_listCoords.GetNextSelectedItem(pos);
			if(theApp.m_nListOrderArray[nIndex] == 0) {
				theApp.m_nListOrderArray[nIndex] = ++m_nOrder;
			}
		}

	}
	m_listCoords.Invalidate();
	Update3DView();

	*pResult = 0;
}

BOOL CFaroTrackerPage::PreTranslateMessage(MSG* pMsg) 
{
	HANDLE hWnd;
	POSITION pos;
	int nIndex;

	switch(pMsg->message) {
	case WM_KEYUP:
		if(m_bCtrlButton == false && m_bShiftButton == false) {
			hWnd = ::GetFocus();
			if(hWnd == m_listCoords.m_hWnd) {
				ZeroMemory(theApp.m_nListOrderArray,sizeof theApp.m_nListOrderArray);
				m_nOrder = 0;
				pos = m_listCoords.GetFirstSelectedItemPosition();
				nIndex=m_listCoords.GetNextSelectedItem(pos);
				theApp.m_nListOrderArray[nIndex] = ++m_nOrder;
				Update3DView();
				m_listCoords.Invalidate(false);
			}
		}
		m_bCtrlButton = false;
		m_bShiftButton = false;
		switch(pMsg->wParam) {
			case 0x21: // Page up
			switch(theApp.m_LastSettings.nFaroRemotePageUp){
			case 2:
				AddCloudPoint();
				break;
			}
			break;
		case 0x22: // page down
			switch(theApp.m_LastSettings.nFaroRemotePageDown){
			case 2:
				AddCloudPoint();
				break;
			}
			break;
		}
		break;
	case WM_KEYDOWN:
		switch(pMsg->wParam) {
		case 16: m_bShiftButton=true;
			break;
		case 17: m_bCtrlButton=true;
			break;
		case 0x21: // Page up
			switch(theApp.m_LastSettings.nFaroRemotePageUp){
				//0=None
			case 1:
				OnButtonAdd();
				break;
			case 2:
				AddCloudPoint();
				break;
			case 3:
				MoveAllCloudPointsToNewTaughtLine(true);
				break;
			}
		/*	if(theApp.m_LastSettings.nFaroRemoteAddButton == 0) {
				OnButtonAdd();
				::SetFocus(m_listElements.m_hWnd);
				return TRUE;
			};
			if(theApp.m_LastSettings.nFaroRemoteCloudButton == 0) {
				AddCloudPoint();
				return TRUE;
			};
			*/
			break;
		case 0x22: // page down
			switch(theApp.m_LastSettings.nFaroRemotePageDown){
				//0=None
			case 1:
				OnButtonAdd();
				break;
			case 2:
				AddCloudPoint();
				break;
			case 3:
				MoveAllCloudPointsToNewTaughtLine(true);
				break;
			}
			/*
			if(theApp.m_LastSettings.nFaroRemoteAddButton == 1) {
				OnButtonAdd();
				::SetFocus(m_listElements.m_hWnd);
				return TRUE;
			};
			if(theApp.m_LastSettings.nFaroRemoteCloudButton == 1) {
				AddCloudPoint();
				return TRUE;
			};
			*/
			break;
		}
		break;
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}


bool CFaroTrackerPage::FillNewElement(VertexArrayType Type)
{
	CVertexArray Element;
	int nTotalRequired;
	int nTotal = 0;
	D3DXVECTOR3 vPt;
	CString Buff,WarningMsg;

	switch(Element.m_Type = Type) {
	case VA_LINE: nTotalRequired = 2;
		WarningMsg = "Line";
		break;
	case VA_ARC: nTotalRequired = 3;
		WarningMsg = "Arc";
		break;
	case VA_ANGLE: nTotalRequired = 3;
		WarningMsg = "Angle";
		break;
	case VA_CIRCLE: nTotalRequired = 3;
		WarningMsg = "Circle";
		break;
	case VA_X_VECTOR: nTotalRequired = 2;
		WarningMsg = "X-Vector";
		break;
	case VA_Y_VECTOR: nTotalRequired = 2;
		WarningMsg = "Y-Vector";
		break;
	case VA_Z_VECTOR: nTotalRequired = 2;
		WarningMsg = "Z-Vector";
		break;
	case VA_HORIZ_PLANE: nTotalRequired = 3;
		WarningMsg = "XY-Plane";
		break;
	}

	for(int nPts=0;nPts<m_nOrder;nPts++) {
		for(int ii=0;ii<PROFILE->m_vCloud.GetSize();ii++) {
			if(theApp.m_nListOrderArray[ii] == (nPts+1)) { 
				PROFILE->m_vCloud.GetPt(ii, &vPt);
				Element.Add(&vPt);
				nTotal++;
			}
		}
	}

	if(nTotal >= nTotalRequired) {
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL] = &Element;
		theApp.m_FaroWorkerThread.m_nElementL++;
		FillElementList();
		return true;
	} else {
		Buff.Format(L"An %s must have %d coordinates",WarningMsg,nTotalRequired);
		MessageBox(Buff, L"Error", MB_ICONERROR);
		return false;
	}
}

void CFaroTrackerPage::OnRclickListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	m_nIndex=m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_FARO_ELEMENT_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}


void CFaroTrackerPage::OnGetdispinfoListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn = pDispInfo->item.iItem;
	D3DXVECTOR3 vPt;

	if(PROFILE->m_vCloud.GetSize()<=0) return;
	PROFILE->m_vCloud.GetPt(nn, &vPt);
	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt, &vPt);


	switch(pDispInfo->item.iSubItem) {
	case 0: swprintf_s(str,L"%d",nn+1);
		break;
	case 1:
		if(theApp.m_nListOrderArray[nn]) {
			swprintf_s(str,L"%d",theApp.m_nListOrderArray[nn]);
		} else {
			str[0]=0;
		}
		break;
	case 2:
		swprintf_s(str,L"%.03f",vPt.x);
		break;
	case 3:
		swprintf_s(str,L"%.03f",vPt.y);
		break;
	case 4:
		swprintf_s(str,L"%.03f",vPt.z);
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CFaroTrackerPage::OnClickListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CString Buff,Temp;
	float fRadius,fAngle0,fAngle1,fLength,fNormAngle;
	D3DXVECTOR3 vCenter,vNorm,vLine,vPt[2];
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	int ii;

	theApp.m_nElementIndex=pDispInfo->item.mask;

	Buff.Empty();
	switch(theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].m_Type) {
	case VA_LINE:	Buff.LoadString(IDS_Line);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].CalculateLength(&fLength);
		Buff.Format(L"Length: %.03f",fLength);

		for(ii=0;ii<2;ii++) {
			theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].GetPt(ii,&vPt[ii]);

			switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
			default:
				break;
			case 1:	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt[ii], &vPt[ii]);
				break;
			case 2:	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt[ii], &vPt[ii]);
				break;
			}
		}
		vLine = vPt[1] - vPt[0];

		Temp.Format(L"\r\nX: %.02f\r\nY: %.02f\r\nZ: %.02f",vLine.x,vLine.y,vLine.z);
		Buff += Temp;
		break;
	case VA_ARC:	Buff.LoadString(IDS_Arc);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].CalculateCircumCenter(&fRadius, &vCenter, &fAngle0, &fAngle1,&vNorm);
		
		Buff.LoadString(IDS_Arc);
		Temp.Format(L"\r\nRadius: %.03f",fRadius);
		Buff += Temp;
		theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vCenter, &vCenter);
		Temp.Format(L"\r\nX:%.03f Y:%.03f Z:%.03f",vCenter.x, vCenter.y, vCenter.z);
		Buff += Temp;
		Temp.Format(L"\r\nAngle 1<->2: %.03f",fAngle0 * RAD_TO_DEG);
		Buff += Temp;
		Temp.Format(L"\r\nAngle 2<->3: %.03f",fAngle1 * RAD_TO_DEG);
		Buff += Temp;
		fNormAngle = acosf(D3DXVec3Dot(&vNorm, &vUp));
		Temp.Format(L"\r\nAngle Normal: %.03f",fNormAngle * RAD_TO_DEG);
		Buff += Temp;

		break;
	case VA_ANGLE:	Buff.LoadString(IDS_Angle);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].CalculateAngle(&fAngle0);
		Temp.Format(L"\r\nAngle: %.03f",fAngle0 * RAD_TO_DEG);
		Buff += Temp;
		break;
	case VA_CIRCLE:	Buff.LoadString(IDS_Circle);
		break;
	case VA_X_VECTOR:	Buff.LoadString(IDS_X_Vector);
		break;
	case VA_Y_VECTOR:	Buff.LoadString(IDS_Y_Vector);
		break;
	case VA_HORIZ_PLANE:	Buff.LoadString(IDS_XY_Plane);
		break;
	}
	
	m_editElementStats.SetWindowText(Buff);
	Update3DView();
	UpdateExpandedElement();

	*pResult = 0;
}



void CFaroTrackerPage::OnFarocoordsAdd000() 
{
	PROFILE->m_vCloud.Add(&theApp.m_FaroWorkerThread.m_vFrameOrigin);
	FillList();
	
}

void CFaroTrackerPage::OnFaroelementDelete() 
{
	int nStartIndex,nFinishIndex;

	POSITION pos = m_listElements.GetFirstSelectedItemPosition();
	nStartIndex=(int)pos;
	nFinishIndex=nStartIndex=m_listElements.GetNextSelectedItem(pos);
	while(pos) {
		nFinishIndex=m_listElements.GetNextSelectedItem(pos);
	}

	theApp.m_FaroWorkerThread.DeleteElements(nStartIndex, nFinishIndex);
	FillElementList();
}


void CFaroTrackerPage::Update3DView()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->SendMessage(UI_UPDATE_3D_VIEW_DLG);
}

void CFaroTrackerPage::UpdateProfileDlg()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);
}



void CFaroTrackerPage::OnFarocoordsLookatthispoint() 
{
	int nIndex;
	D3DXVECTOR3 vPt;
	CCoord Cp;


	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	nIndex=m_listCoords.GetNextSelectedItem(pos);

	PROFILE->m_vCloud.GetPt(nIndex, &vPt);
	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt, &vPt);

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CProfileSheet* pSheet = pFrame->m_pProfileSheet;
	
	if(pSheet->m_p3DViewPage) {
		Cp.Side0.pt = vPt;
		Cp.Side0.norm = D3DXVECTOR4(0.0f, 0.0f, 1.0f,0.0f);

		pSheet->m_p3DViewPage->LookAtPoint(Cp,false);
		Update3DView();
	}

}





void CFaroTrackerPage::OnFarocoordsTooltipOrigin() 
{
	D3DXVECTOR3 vPt,vFrameOrigin;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	int nIndex = m_listCoords.GetNextSelectedItem(pos);

	PROFILE->m_vCloud.GetPt(nIndex, &vPt);
	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt,&vPt);
	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vFrameOrigin,&theApp.m_FaroWorkerThread.m_vFrameOrigin);
	theApp.m_FaroWorkerThread.m_vToolOrigin = vPt - vFrameOrigin;

	m_listCoords.Invalidate();
	
}

void CFaroTrackerPage::OnButtonWorldCoordinates() 
{
	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode = 0;
	m_listCoords.Invalidate();
	SetToolBarCheckedState();
	UpdateExpandedElement();
}

void CFaroTrackerPage::OnButtonFrameCoordinates() 
{
	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode = 1;
	m_listCoords.Invalidate();
	SetToolBarCheckedState();
	UpdateExpandedElement();
}

void CFaroTrackerPage::OnButtonToolCoordinates() 
{
	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode = 2;
	m_listCoords.Invalidate();
	SetToolBarCheckedState();
	UpdateExpandedElement();
}

void CFaroTrackerPage::OnButtonDelete() 
{
	CEdit *pEdit = (CEdit*)GetFocus();

	if(pEdit == GetDlgItem(IDC_LIST_COORDS)) {
		OnFarocoordsDelete();
	} 
	if(pEdit == GetDlgItem(IDC_LIST_ELEMENTS)) {
		OnFaroelementDelete();
	} 
	
}

void CFaroTrackerPage::OnFileSaveas() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_FileName;
	if((nPos = m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".tra"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Tracker (*.tra)|*.tra|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_TrackerPathName;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_TrackerPathName = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			SaveTracker(&File);
			File.Close();
		}
	}		
	
}

void CFaroTrackerPage::OnFileOpen() 
{
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".tra"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Tracker (*.tra)|*.tra|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_TrackerPathName;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_TrackerPathName = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			RetrieveTracker(&File);
			File.Close();
			FillList();
			FillElementList();
		}
	}
	
}




void CFaroTrackerPage::OnButtonPolar() 
{
	theApp.m_LastSettings.nFaroDisplayRectangularPolar = 1;
}

void CFaroTrackerPage::OnButtonRectangular() 
{
	theApp.m_LastSettings.nFaroDisplayRectangularPolar = 0;
}


void CFaroTrackerPage::OnFaroelementCircumcentertolist() 
{
	float fRadius,fAngle0,fAngle1;
	D3DXVECTOR3 vCenter;

	switch(theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].m_Type) {
	case VA_ARC:
		theApp.m_FaroWorkerThread.m_Element[theApp.m_nElementIndex].CalculateCircumCenter(&fRadius, &vCenter, &fAngle0, &fAngle1);
		PROFILE->m_vCloud.Add(&vCenter);
		break;
	}

	m_listCoords.Invalidate();
	FillList();

}



void CFaroTrackerPage::OnFarocoordsTooltipPlane() 
{
	int nTotal=0;
	D3DXVECTOR3 vPt[3],vOA,vOB,vNorm;

	if(FillNewElement(VA_HORIZ_PLANE)) {
		theApp.m_FaroWorkerThread.m_vToolNorm = D3DXVECTOR3(0.0f,0.0f,1.0f);
		theApp.m_FaroWorkerThread.GetToolCoordinate(&vPt[0], &theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].m_pVp[0].pt);
		theApp.m_FaroWorkerThread.GetToolCoordinate(&vPt[1], &theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].m_pVp[1].pt);
		theApp.m_FaroWorkerThread.GetToolCoordinate(&vPt[2], &theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].m_pVp[2].pt);

		vOA = vPt[0] - vPt[1];
		vOB = vPt[2] - vPt[1];

		D3DXVec3Cross(&vNorm, &vOA, &vOB);
		D3DXVec3Normalize( &vNorm, &vNorm );

		theApp.m_FaroWorkerThread.m_vToolNorm = vNorm;
		m_listCoords.Invalidate();
	}
}

void CFaroTrackerPage::OnFaroelementExpand() 
{
	if (m_pFaroElementExpansionDlg == NULL) {
		m_pFaroElementExpansionDlg = new CFaroElementExpansionDlg(this,(CWnd**)&m_pFaroElementExpansionDlg,theApp.m_nElementIndex);
		m_pFaroElementExpansionDlg->Create(CFaroElementExpansionDlg::IDD,this);
		SetFocus();
	} else {
		m_pFaroElementExpansionDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}

void CFaroTrackerPage::UpdateExpandedElement()
{
	if(m_pFaroElementExpansionDlg != NULL) {
		m_pFaroElementExpansionDlg->m_nElementIndex = theApp.m_nElementIndex;
		m_pFaroElementExpansionDlg->UpdateAllControls();
	};
}

void CFaroTrackerPage::OnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	sockaddr_in Server;

	m_ipAddress.GetAddress(Server.sin_addr.s_addr); 
	theApp.m_LastSettings.nFaroTrackerIPAddrs = Server.sin_addr.s_addr;
	
	*pResult = 0;
}


void CFaroTrackerPage::OnFarocoordsFrameOrigin() 
{
	D3DXVECTOR3 vPt;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	int nIndex = m_listCoords.GetNextSelectedItem(pos);

	PROFILE->m_vCloud.GetPt(nIndex, &vPt);
	theApp.m_FaroWorkerThread.m_vFrameOrigin = vPt;

	m_listCoords.Invalidate();

}

void CFaroTrackerPage::OnFarocoordsFrameXvector() 
{
	D3DXVECTOR3 vPt[2];

	if(FillNewElement(VA_X_VECTOR)) {
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(0,&vPt[0]);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(1,&vPt[1]);
		theApp.m_FaroWorkerThread.m_vFrameRotate.z = 0.0f;
		theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt[0], &vPt[0]);
		theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt[1], &vPt[1]);
		
		theApp.m_FaroWorkerThread.m_vFrameRotate.z = 	-atan2f(vPt[1].y-vPt[0].y, vPt[1].x-vPt[0].x);

		m_listCoords.Invalidate();
	}
	
}

void CFaroTrackerPage::OnFarocoordsFrameYvector() 
{
	D3DXVECTOR3 vPt[2];

	if(FillNewElement(VA_Y_VECTOR)) {
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(0,&vPt[0]);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(1,&vPt[1]);
		theApp.m_FaroWorkerThread.m_vFrameRotate.z = 0.0f;
		theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt[0], &vPt[0]);
		theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt[1], &vPt[1]);
		
		theApp.m_FaroWorkerThread.m_vFrameRotate.z = PIf/2.0f	- atan2f(vPt[1].y-vPt[0].y, vPt[1].x-vPt[0].x);

		m_listCoords.Invalidate();
	}
}

void CFaroTrackerPage::OnFarocoordsFrameHorizontalplane() 
{
	int nTotal=0;
	D3DXVECTOR3 vPt[3],vOA,vOB,vNorm;

	if(FillNewElement(VA_HORIZ_PLANE)) {

		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(0,&vPt[0]);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(1,&vPt[1]);
		theApp.m_FaroWorkerThread.m_Element[theApp.m_FaroWorkerThread.m_nElementL-1].GetPt(2,&vPt[2]);

		vOA = vPt[0] - vPt[1];
		vOB = vPt[2] - vPt[1];

		D3DXVec3Normalize( &vOA, &vOA );
		D3DXVec3Normalize( &vOB, &vOB );
		D3DXVec3Cross(&vNorm, &vOA, &vOB);
		D3DXVec3Normalize( &vNorm, &vNorm );
		if(vNorm.z<0.0f) {
			vNorm.x*=-1.0f;
			vNorm.y*=-1.0f;
			vNorm.z*=-1.0f;
		}

		theApp.m_FaroWorkerThread.m_vFrameNorm = vNorm;
		m_listCoords.Invalidate();
	}
}

void CFaroTrackerPage::OnFarocoordsFrameVerticalplane() 
{
	// TODO: Add your command handler code here
	
}

void CFaroTrackerPage::OnFarocoordsFrameZvector() 
{
	// TODO: Add your command handler code here
	
}



void CFaroTrackerPage::OnChangeEditCloudLength() 
{
	CString Buff;

	m_editCloudLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fFaroCloudLength);
}


void CFaroTrackerPage::OnButtonEarthLevel() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(theApp.m_FaroWorkerThread.m_bCollectingBackGround == TRUE) {
		theApp.m_FaroWorkerThread.OnStopBackgndMeasurements();
	}
	theApp.m_FaroWorkerThread.StartLevelMeasure();
}

void CFaroTrackerPage::OnSelchangeComboRemotePageUp() 
{
	theApp.m_LastSettings.nFaroRemotePageUp = m_comboPageUp.GetCurSel();
	
}

void CFaroTrackerPage::OnSelchangeComboRemotePageDown() 
{
	theApp.m_LastSettings.nFaroRemotePageDown = m_comboPageDown.GetCurSel();
}

void CFaroTrackerPage::OnFarocoordsTransferTaughtPointsNewLine()
{
	MoveAllCloudPointsToNewTaughtLine(false);
}


void CFaroTrackerPage::MoveAllCloudPointsToNewTaughtLine(bool bDeleteCloudPoints)
{
	CPolyCoord CopyLine;

	CUSLVertex vPt,vCloud;
	CopyLine.Zero();

	for(int ii=0;ii<PROFILE->m_vCloud.GetSize();ii++) {
		PROFILE->m_vCloud.GetVertex(ii,&vCloud);
		
		switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
		default:
			vPt=vCloud;
			break;
		case 1:	theApp.m_FaroWorkerThread.GetFrameCoordinate(vPt,vCloud);
			break;
		case 2:	theApp.m_FaroWorkerThread.GetToolCoordinate(vPt, vCloud);
			break;
		}

		CopyLine.AddVertex(vPt);
	}

	PROFILE->AddLine(TAUGHTLINES,&CopyLine,PROFILE->m_nSlowAxis);



	//Remove Cloud points
	if(bDeleteCloudPoints) {
		PROFILE->m_vCloud.Zero();
		FillList();
	}

	
	Update3DView();
//	UpdateProfileDlg(); //This will update 2D but changes focus so buttons dont work
	Invalidate();	
	
}

void CFaroTrackerPage::OnFaroCoordsTransferReferencePoints()
{


}


bool CFaroTrackerPage::CheckAllFilesExist()
{
	bool bRet = true;
	CString Buff, Missing, TotalMissing;
	CString Files[10];

	Files[0] = theApp.m_WorkingDirectory + L"\\Tracker.dll";
	Files[1] = theApp.m_WorkingDirectory + L"\\TrackerUnicode.dll";
	Files[2] = theApp.m_WorkingDirectory + L"\\Apps4xxx.jar";
	Files[3] = theApp.m_WorkingDirectory + L"\\AppsKeystoneSim.jar";
	Files[4] = theApp.m_WorkingDirectory + L"\\Ftp.jar";
	Files[5] = theApp.m_WorkingDirectory + L"\\Tracker.jar";
	Files[6] = theApp.m_WorkingDirectory + L"\\Utility.jar";

	TotalMissing.Empty();

	Buff = theApp.m_WorkingDirectory + L"\\jre";
	if(CheckDirExists(Buff) == false) {
		Missing.Format(L"Dir: %s is missing. Please reinstall\r\n", Buff);
		TotalMissing += Missing;
		bRet = false;
	}

	for(int ii=0;ii<7;ii++) {
		if(CheckFileExists(Files[ii]) == false) {
			Missing.Format(L"File: %s is missing.\r\n", Files[ii]);
			TotalMissing += Missing;
			bRet = false;
		}
	}

	if(bRet == false) {
		MessageBox(TotalMissing, L"Error", MB_ICONERROR);
	}

	return bRet;
}


bool CFaroTrackerPage::CheckDirExists(CString Dir)
{
	DWORD ftyp = GetFileAttributes(Dir.GetBuffer(256));

	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

bool CFaroTrackerPage::CheckFileExists(CString FileName)
{
	CUSLFile File;
	CFileException e;
	CString Buff;


	if(!File.Open(FileName, CFile::typeBinary|CFile::modeRead, &e)) {
		File.DecodeFileException(&e,&Buff,FileName);
		return false;
	} else {
		File.Close();
		return true;
	}
}
