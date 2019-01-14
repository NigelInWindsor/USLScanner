// GlobalUtPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "GlobalUtPage.h"
#include "UltrasonicsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlobalUtPage property page

IMPLEMENT_DYNCREATE(CGlobalUtPage, CPropertyPage)

CGlobalUtPage::CGlobalUtPage(CPropertySheet* pSheet) : CPropertyPage(CGlobalUtPage::IDD)
{
	//{{AFX_DATA_INIT(CGlobalUtPage)
	//}}AFX_DATA_INIT

	m_pSheet = pSheet;
	

}

CGlobalUtPage::~CGlobalUtPage()
{
}

void CGlobalUtPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlobalUtPage)
	DDX_Control(pDX, IDC_COMBO_TIMESLOTS, m_comboTimeSlots);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_2, m_comboChannel2);
	DDX_Control(pDX, IDC_COMBO_HT_SOURCE, m_comboHtSource);
	DDX_Control(pDX, IDC_COMBO_TX, m_comboTx);
	DDX_Control(pDX, IDC_COMBO_RX, m_comboRx);
	DDX_Control(pDX, IDC_PROGRESS_AMPLITUDE, m_progressAmplitude);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ANALOGUE, m_checkUseAnaloguePort);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_comboProcess);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGlobalUtPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGlobalUtPage)
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESS, OnSelchangeComboProcess)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ANALOGUE, OnCheckCollectAnalogue)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_RX, OnSelchangeComboRx)
	ON_CBN_SELCHANGE(IDC_COMBO_TX, OnSelchangeComboTx)
	ON_CBN_SELCHANGE(IDC_COMBO_HT_SOURCE, OnSelchangeComboHtSource)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL_2, OnSelchangeComboChannel2)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMESLOTS, OnSelchangeComboTimeslots)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalUtPage message handlers


BOOL CGlobalUtPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboTimeSlots.AddString(_T("1"));
	m_comboTimeSlots.AddString(_T("2"));
	m_comboTimeSlots.AddString(_T("3"));
	m_comboTimeSlots.AddString(_T("4"));
	m_comboTimeSlots.AddString(_T("5"));
	m_comboTimeSlots.AddString(_T("6"));
	m_comboTimeSlots.AddString(_T("7"));
	m_comboTimeSlots.AddString(_T("8"));
	m_comboTimeSlots.SetCurSel(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots-1);

	
	m_progressAmplitude.SetRange(0,100);
	m_progressAmplitude.SetStep(1);



	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGlobalUtPage::OnSelchangeComboProcess() 
{
	CString	Buff;

	int nn=m_comboProcess.GetCurSel();
	m_comboProcess.GetLBText(nn,Buff);

	if(wcscmp(Buff,_T("Gpd 1"))==0) theApp.m_LastSettings.nProcessCard=USL_ATP_GPD0;
	if(wcscmp(Buff,_T("Gpd 2"))==0) theApp.m_LastSettings.nProcessCard=USL_ATP_GPD1;
	if(wcscmp(Buff,_T("Tims 1"))==0) theApp.m_LastSettings.nProcessCard=USL_ATP_TIMS0;
	if(wcscmp(Buff,_T("Tims 2"))==0) theApp.m_LastSettings.nProcessCard=USL_ATP_TIMS1;
	if(wcscmp(Buff,_T("ADC"))==0) theApp.m_LastSettings.nProcessCard=USL_ADC100;

	theApp.m_UtUser.RfAndClockSource();
	
}


void CGlobalUtPage::OnCheckCollectAnalogue() 
{
}

void CGlobalUtPage::OnTimer(UINT nIDEvent) 
{
	CString	Buff;
	static	int	nOldAmplitude=0;

	int nAmplitude=theApp.m_Si6User.ReadJoystick(3);
	if(nAmplitude-nOldAmplitude) {
		m_progressAmplitude.SetPos(MulDiv(nAmplitude/2,100,127));
		nOldAmplitude=nAmplitude;
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CGlobalUtPage::OnSelchangeComboRx() 
{
}

void CGlobalUtPage::OnSelchangeComboTx() 
{
}

void CGlobalUtPage::OnSelchangeComboHtSource() 
{
}

void CGlobalUtPage::OnSelchangeComboChannel2() 
{
	
}

void CGlobalUtPage::OnSelchangeComboTimeslots() 
{
	int nTemp = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;
	CUltrasonicsSheet* pSheet = (CUltrasonicsSheet*)m_pSheet;
	
	theApp.m_UtUser.m_Global.Mux.nMaxTimeslots=m_comboTimeSlots.GetCurSel() + 1;
	if(nTemp-theApp.m_UtUser.m_Global.Mux.nMaxTimeslots) {
		theApp.m_UtUser.ChangeMaxTimeSlots(theApp.m_nJobNumber);
		pSheet->SetMaxTimeSlots();
	}
	UpdateRelatedPages();

}

void CGlobalUtPage::UpdateRelatedPages()
{
	QuerySiblings(0,0);
}

BOOL CGlobalUtPage::OnSetActive() 
{

	UpdateAllControls();
	return CPropertyPage::OnSetActive();
}

void CGlobalUtPage::UpdateAllControls()
{

}
