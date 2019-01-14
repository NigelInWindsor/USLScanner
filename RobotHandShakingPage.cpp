// RobotHandShakingPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotHandShakingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotHandShakingPage property page

IMPLEMENT_DYNCREATE(CRobotHandShakingPage, CPropertyPage)

CRobotHandShakingPage::CRobotHandShakingPage() : CPropertyPage(CRobotHandShakingPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotHandShakingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nTableMode = 0;
}

CRobotHandShakingPage::~CRobotHandShakingPage()
{
}

void CRobotHandShakingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotHandShakingPage)
	DDX_Control(pDX, IDC_EDIT_COORDSYSTEM3, m_editCoordSystem3);
	DDX_Control(pDX, IDC_EDIT_SCANLINE, m_editScanLine);
	DDX_Control(pDX, IDC_LIST_GENERAL, m_listGeneral);
	DDX_Control(pDX, IDC_EDIT_UT_COLLECTION_PTR, m_editUTCollectionPtr);
	DDX_Control(pDX, IDC_EDIT_INPUT_REGSITER, m_editInputRegister);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_TO_PORT, m_editOutputToPort);
	DDX_Control(pDX, IDC_EDIT_SYNC_COUNT, m_editInSyncCount);
	DDX_Control(pDX, IDC_COMBO_OUT_SYNCH_PULSE, m_comboOutSynchPulseBit);
	DDX_Control(pDX, IDC_COMBO_OUT_COLLECT_DATA_BIT, m_comboOutCollectDataBit);
	DDX_Control(pDX, IDC_COMBO_IN_SYNCH_PULSE, m_comboInSynchPulseBit);
	DDX_Control(pDX, IDC_COMBO_IN_COLLECT_DATA_BIT, m_comboInCollectDataBit);
	DDX_Control(pDX, IDC_COMBO_INPUT_PORT, m_comboInputPort);
	DDX_Control(pDX, IDC_COMBO_OUTPUT_PORT, m_comboOutputPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotHandShakingPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotHandShakingPage)
	ON_CBN_SELCHANGE(IDC_COMBO_IN_COLLECT_DATA_BIT, OnSelchangeComboInCollectDataBit)
	ON_CBN_SELCHANGE(IDC_COMBO_IN_SYNCH_PULSE, OnSelchangeComboInSynchPulse)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_PORT, OnSelchangeComboInputPort)
	ON_CBN_SELCHANGE(IDC_COMBO_OUT_COLLECT_DATA_BIT, OnSelchangeComboOutCollectDataBit)
	ON_CBN_SELCHANGE(IDC_COMBO_OUT_SYNCH_PULSE, OnSelchangeComboOutSynchPulse)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_PORT, OnSelchangeComboOutputPort)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_TO_PORT, OnChangeEditOutputToPort)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_GENERAL, OnGetdispinfoListGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotHandShakingPage message handlers

BOOL CRobotHandShakingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_comboInputPort.AddString(L"SI10 GPIO");
	m_comboOutputPort.AddString(L"SI10 GPIO");

	for(int ii=0;ii<8;ii++) {
		Buff.Format(L"%d",ii);
		m_comboOutCollectDataBit.AddString(Buff);
		m_comboOutSynchPulseBit.AddString(Buff);
		m_comboInCollectDataBit.AddString(Buff);
		m_comboInSynchPulseBit.AddString(Buff);
	}

	CreateColumns();
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRobotHandShakingPage::OnSetActive() 
{
	
	
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();
	SetTimer(1,250,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CRobotHandShakingPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CRobotHandShakingPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	m_comboInputPort.SetCurSel(theApp.m_Tank.nRobotOutputPort = 0);
	m_comboOutputPort.SetCurSel(theApp.m_Tank.nRobotInputPort = 0);

	m_comboOutCollectDataBit.SetCurSel(theApp.m_Tank.nRobotOutCollectDataBit);
	m_comboOutSynchPulseBit.SetCurSel(theApp.m_Tank.nRobotOutSynchPulseBit);
	m_comboInCollectDataBit.SetCurSel(theApp.m_Tank.nRobotInCollectDataBit);
	m_comboInSynchPulseBit.SetCurSel(theApp.m_Tank.nRobotInSynchPulseBit);

	FillList();
}

void CRobotHandShakingPage::OnSelchangeComboInCollectDataBit() 
{
	

	theApp.m_Tank.nRobotInCollectDataBit = 1 << m_comboInCollectDataBit.GetCurSel();
}

void CRobotHandShakingPage::OnSelchangeComboInSynchPulse() 
{
	
	int nMask = 1;
	theApp.m_Tank.nRobotInSynchPulseBit = m_comboInSynchPulseBit.GetCurSel();
	nMask <<= theApp.m_Tank.nRobotInSynchPulseBit;
	theApp.m_Si10User.m_Si10.nGPIOIrqMask = nMask;
}

void CRobotHandShakingPage::OnSelchangeComboInputPort() 
{
	
}

void CRobotHandShakingPage::OnSelchangeComboOutCollectDataBit() 
{
	
	theApp.m_Tank.nRobotOutCollectDataBit = m_comboOutCollectDataBit.GetCurSel();
}

void CRobotHandShakingPage::OnSelchangeComboOutSynchPulse() 
{
	
	theApp.m_Tank.nRobotOutSynchPulseBit = m_comboOutSynchPulseBit.GetCurSel();
}

void CRobotHandShakingPage::OnSelchangeComboOutputPort() 
{
	
}

void CRobotHandShakingPage::OnTimer(UINT nIDEvent) 
{
	
	CString Buff;

	Buff.Format(L"%d",theApp.m_Si10User.m_Si10.nGPIOIrqCount);
	m_editInSyncCount.SetWindowText(Buff);


	Buff.Format(L"%04x",theApp.m_Si10User.GetFieldBusRegister());
//	Buff.Format(L"%04x",theApp.m_Si10User.GetInputPort());
	m_editInputRegister.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_UtUser.GetCollectPtr());
	m_editUTCollectionPtr.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_FBCtrl.m_nScanLine);
	m_editScanLine.SetWindowText(Buff);

	Buff.Format(L"$%016I64x",theApp.m_FBCtrl.m_n64CoordStatus[2]);
	m_editCoordSystem3.SetWindowText(Buff);

	
	CPropertyPage::OnTimer(nIDEvent);
}

void CRobotHandShakingPage::OnChangeEditOutputToPort() 
{
	
	CString Buff;

	m_editOutputToPort.GetWindowText(Buff);

	int nValue = _ttoi(Buff);
	theApp.m_Si10User.WriteGeneralOuput(nValue & 0xff);
}


void CRobotHandShakingPage::CreateColumns()
{
	
	int ColumnWidth[10] = { 30,50,50,50,50,50,50};
	CString ColumnName[10] = {L"#",L"X",L"Y",L"Z",L"i",L"j",L"k"};
	int	ii;

	for (ii=0;ii<7;ii++) {
		m_listGeneral.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listGeneral.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CRobotHandShakingPage::FillList()
{
	
	int	ii;
	CString Buff;

	m_listGeneral.DeleteAllItems();
	switch(m_nTableMode) {
	case 0:
		for (ii=0;ii<theApp.m_FBCtrl.m_vDataGather.GetSize();ii++) {
			Buff.Format(_T("%d"),ii);
			m_listGeneral.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
		break;
	case 1:
		for (ii=0;ii<theApp.m_UtUser.CollectGetSize(theApp.m_Thread.m_nDir ^ 1);ii++) {
			Buff.Format(_T("%d"),ii);
			m_listGeneral.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
		break;
	}
}

void CRobotHandShakingPage::OnGetdispinfoListGeneral(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nIndex;
	CUSLVertex vert;
	TimeSlotDataCollectionStruct TSCollect;

	str[0]=0;

	switch(m_nTableMode) {
	case 0:
		if(theApp.m_FBCtrl.m_vDataGather.GetSize()<=0) return;
		nIndex = pDispInfo->item.iItem;
		if(nIndex>=theApp.m_FBCtrl.m_vDataGather.GetSize()) return;

		theApp.m_FBCtrl.m_vDataGather.GetVertex(nIndex, &vert);

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:	swprintf_s(str,L"%.01f",vert.pt.x);
			break;
		case 2:	swprintf_s(str,L"%.01f",vert.pt.y);
			break;
		case 3:	swprintf_s(str,L"%.01f",vert.pt.z);
			break;
		case 4:	swprintf_s(str,L"%.04f",vert.norm.x);
			break;
		case 5:	swprintf_s(str,L"%.04f",vert.norm.y);
			break;
		case 6:	swprintf_s(str,L"%.04f",vert.norm.z);
			break;
		}
		break;
	case 1:
		if(theApp.m_UtUser.CollectGetSize(theApp.m_Thread.m_nDir ^ 1)<=0) return;
		nIndex = pDispInfo->item.iItem;
		if(nIndex>=theApp.m_UtUser.CollectGetSize(theApp.m_Thread.m_nDir ^ 1)) return;

		theApp.m_UtUser.GetCollect(theApp.m_Thread.m_nDir ^ 1,nIndex,&TSCollect);

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:	swprintf_s(str,L"%d",TSCollect.nFieldBusCount);
			break;
		case 2:	
			break;
		case 3:	
			break;
		}
		break;
	};

	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}
