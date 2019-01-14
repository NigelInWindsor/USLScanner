// EthercatPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "EthercatPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEthercatPage property page

IMPLEMENT_DYNCREATE(CEthercatPage, CPropertyPage)

CEthercatPage::CEthercatPage() : CPropertyPage(CEthercatPage::IDD)
{
	//{{AFX_DATA_INIT(CEthercatPage)
	//}}AFX_DATA_INIT
	
}

CEthercatPage::~CEthercatPage()
{
}

void CEthercatPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEthercatPage)
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_EDIT_POS_DIFF, m_editPositionDifference);
	DDX_Control(pDX, IDC_EDIT_TIME_DIFF, m_editTimeDiff);
	DDX_Control(pDX, IDC_EDIT_TIME1, m_editTime1);
	DDX_Control(pDX, IDC_EDIT_TIME0, m_editTime0);
	DDX_Control(pDX, IDC_EDIT_POSITION2, m_editPosition1);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editPosition0);
	DDX_Control(pDX, IDC_EDIT_ETHERCAT_COUNT, m_editEthercatCount);
	DDX_Control(pDX, IDC_EDIT_RX, m_editRx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEthercatPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEthercatPage)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEthercatPage message handlers

BOOL CEthercatPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboMode.AddString(L"Irq SI10");
	m_comboMode.AddString(L"Event 0 ms");
	m_comboMode.AddString(L"Event 1 ms");
	m_comboMode.AddString(L"Event 2 ms");
	m_comboMode.AddString(L"Event 5 ms");
	m_comboMode.AddString(L"Polled");

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEthercatPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();
	
	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CEthercatPage::OnKillActive() 
{

	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CEthercatPage::UpdateAllControls()
{
	m_comboMode.SetCurSel( theApp.m_LastSettings.nEthercatCommunicationMode );
}

void CEthercatPage::OnTimer(UINT nIDEvent) 
{
	CString Buff,Temp;
	unsigned char Array[84];
	int	ii;
	int nNumberAxes;
	int *pnPos,*pnPos0,*pnPos1;

	switch(theApp.m_Tank.nScannerDescription) {
	default: nNumberAxes = 20;
		break;
	case DUAL_ROBOT_9_PLUS_9: nNumberAxes = 20;
		break;
	}
	int nLength = (nNumberAxes + 1) * (sizeof (int));	//12 axes + 1 control

	if(theApp.m_LastSettings.nEthercatCommunicationMode == 5) {		//Polled
		if(theApp.m_Ethercat.IORead(Array,sizeof Array) == true) {
			CopyMemory(theApp.m_Ethercat.m_RxArray, Array, sizeof Array);

			CopyMemory(&theApp.m_Ethercat.m_EthercatIO[0],&theApp.m_Ethercat.m_EthercatIO[1],sizeof theApp.m_Ethercat.m_EthercatIO[0]);
			CopyMemory(&theApp.m_Ethercat.m_EthercatIO[1],&theApp.m_Ethercat.m_RxArray,nLength);
		}
	}

	Buff.Format(L"%d",theApp.m_Ethercat.m_nCallBackCount);
	m_editEthercatCount.SetWindowText(Buff);
		
	Buff.Empty();
	for(ii=0;ii<100;ii++) {
		if(((ii%4) == 0) && (ii!=0)) {
			Buff += "\r\n";
		};
		Temp.Format(L"%02x ",theApp.m_Ethercat.m_RxArray[ii]);
		Buff += Temp;
	}
	m_editRx.SetWindowText(Buff);

	Buff.Format(L"%f",theApp.m_Ethercat.m_EthercatIO[0].dHRTime);
	m_editTime0.SetWindowText(Buff);

	pnPos = (int *)&theApp.m_Ethercat.m_EthercatIO[0].cArray;
	Buff.Format(L"%08x\r\n",theApp.m_Ethercat.m_EthercatIO[0].nCntrl);
	for(ii=0;ii<nNumberAxes;ii++) {
		Temp.Format(L"%d\r\n",pnPos[ii]);
		Buff+=Temp;
	};
	m_editPosition0.SetWindowText(Buff);

	Buff.Format(L"%f",theApp.m_Ethercat.m_EthercatIO[1].dHRTime);
	m_editTime1.SetWindowText(Buff);

	pnPos = (int *)&theApp.m_Ethercat.m_EthercatIO[1].cArray;
	Buff.Format(L"%08x\r\n",theApp.m_Ethercat.m_EthercatIO[1].nCntrl);
	for(ii=0;ii<nNumberAxes;ii++) {
		Temp.Format(L"%d\r\n",pnPos[ii]);
		Buff+=Temp;
	};
	m_editPosition1.SetWindowText(Buff);

	Buff.Format(L"%f",theApp.m_Ethercat.m_EthercatIO[1].dHRTime - theApp.m_Ethercat.m_EthercatIO[0].dHRTime);
	m_editTimeDiff.SetWindowText(Buff);

	pnPos0 = (int *)&theApp.m_Ethercat.m_EthercatIO[0].cArray;
	pnPos1 = (int *)&theApp.m_Ethercat.m_EthercatIO[1].cArray;
	Buff.Format(L"%08x\r\n",theApp.m_Ethercat.m_EthercatIO[1].nCntrl);
	for(ii=0;ii<nNumberAxes;ii++) {
		Temp.Format(L"%d\r\n",pnPos1[ii] - pnPos0[ii]);
		Buff+=Temp;
	};
	m_editPositionDifference.SetWindowText(Buff);


	CPropertyPage::OnTimer(nIDEvent);
}

void CEthercatPage::OnSelchangeComboMode() 
{
	theApp.m_LastSettings.nEthercatCommunicationMode = m_comboMode.GetCurSel();
}

void CEthercatPage::OnButtonStart() 
{
	theApp.m_Ethercat.Initialize(&m_editMessage);
	theApp.m_Ethercat.StartFieldBus();
}

void CEthercatPage::OnButtonClose() 
{
	theApp.m_Ethercat.Close(&m_editMessage);
}
