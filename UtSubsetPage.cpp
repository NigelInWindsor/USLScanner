// UtSubsetPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "UtSubsetPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUtSubsetPage property page

IMPLEMENT_DYNCREATE(CUtSubsetPage, CPropertyPage)

CUtSubsetPage::CUtSubsetPage() : CPropertyPage(CUtSubsetPage::IDD)
{
	//{{AFX_DATA_INIT(CUtSubsetPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	

}

CUtSubsetPage::~CUtSubsetPage()
{
}

void CUtSubsetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUtSubsetPage)
	DDX_Control(pDX, IDC_LIST_UT_VARIABLES, m_listVariables);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUtSubsetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUtSubsetPage)
	ON_LBN_SELCHANGE(IDC_LIST_UT_VARIABLES, OnSelchangeListUtVariables)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_UT_FILLALL, OnUtFillall)
	ON_COMMAND(ID_UT_CLEARALL, OnUtClearall)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_UT_VARIABLES, OnCheckChangeListUtVariables)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUtSubsetPage message handlers

BOOL CUtSubsetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	
	int ii=0;
	
	m_listVariables.ResetContent();
	m_listVariables.SetCheckStyle(BS_AUTOCHECKBOX);

	Buff.LoadString(IDS_Preamp_Gain);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Preamp_Mode);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Damping);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulse_Width);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulse_Voltage);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_PRF);								m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulser);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Output_Mode);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Transmitter);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Receiver);							m_listVariables.AddString(Buff);

	Buff.LoadString(IDS_Linear_Gain);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Log_Gain);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Rectifier_Mode);					m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Type);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Range);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Center);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Cut_Off);					m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Mode);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Trigger);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Delay);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Threshold);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Table);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Scope_Delay);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Scope_Width);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Mode);							m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Delay);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Width);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Threshold);					m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Dead_Zone);					m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Peak_Width);					m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_Amplitude_Collect);					m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_TOF_Collect);						m_listVariables.AddString(Buff);
	Buff.LoadString(IDS_TOF_Mode);							m_listVariables.AddString(Buff);

	

	UpdateAllControls();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUtSubsetPage::UpdateAllControls()
{
	
	if (GetSafeHwnd() == NULL) return;
	bool bFlag;

	int ii = 0;
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PREAMP_GAIN ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PREAMP_MODE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAMPING ? bFlag = true : bFlag = false;			m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSE_WIDTH ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSE_VOLTAGE ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PRF ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_PULSER_OFF_ON ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_OUTPUT_MODE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TX ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_RX ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);

	theApp.m_LastSettings.n64SubsetUtVariable & _UT_LINEAR_GAIN ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_LOG_GAIN ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_RECTIFY_MODE ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_TYPE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_RANGE ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_CENTER ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_FILTER_CUT_OFF ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_MODE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_TRIGGER ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_DELAY ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_THRESHOLD ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_DAC_TABLE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_SCOPE_DELAY ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_SCOPE_WIDTH ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_MODE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_DELAY ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_WIDTH ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_THRESHOLD ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_FAR_DEAD_ZONE ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_GATE_FAR_PEAK_WIDTH ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_AMPLITUDE_COLLECT ? bFlag = true : bFlag = false;	m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TOF_COLLECT ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);
	theApp.m_LastSettings.n64SubsetUtVariable & _UT_TOF_MODE ? bFlag = true : bFlag = false;		m_listVariables.SetCheck(ii++,bFlag);


}


BOOL CUtSubsetPage::OnSetActive() 
{
	
	theApp.m_LastSettings.nLastCustomizeTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	
	return CPropertyPage::OnSetActive();
}

BOOL CUtSubsetPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CUtSubsetPage::OnSelchangeListUtVariables() 
{
	// TODO: Add your control notification handler code here
	
}

void CUtSubsetPage::OnCheckChangeListUtVariables() 
{
	
	int ii=0;

	theApp.m_LastSettings.n64SubsetUtVariable = 0;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PREAMP_GAIN;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PREAMP_MODE;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_DAMPING;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PULSE_WIDTH;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PULSE_VOLTAGE;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PRF;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_PULSER_OFF_ON;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_OUTPUT_MODE;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_TX;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_RX;
	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_LINEAR_GAIN;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_LOG_GAIN;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_RECTIFY_MODE;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_TYPE;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_RANGE;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_CENTER;	
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |= _UT_FILTER_CUT_OFF;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_DAC_MODE;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_DAC_TRIGGER;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_DAC_DELAY;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_DAC_THRESHOLD;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_DAC_TABLE;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_SCOPE_DELAY;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_SCOPE_WIDTH;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_GATE_MODE;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_GATE_DELAY;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_GATE_WIDTH;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_GATE_THRESHOLD;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_GATE_FAR_DEAD_ZONE;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_GATE_FAR_PEAK_WIDTH;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_AMPLITUDE_COLLECT;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_TOF_COLLECT;
	if(m_listVariables.GetCheck(ii++)) theApp.m_LastSettings.n64SubsetUtVariable |=	_UT_TOF_MODE;


	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->RedrawUltrasonicsSheet();
}



BOOL CUtSubsetPage::PreTranslateMessage(MSG* pMsg) 
{
	CRect rr;

	switch(pMsg->message) {
	case 0x204://RBUTTONDOWN
		m_listVariables.GetWindowRect(&rr);
		if(rr.PtInRect(pMsg->pt)) {
			OnContextMenu(this, pMsg->pt);
			return true;
		};
		break;
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CUtSubsetPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;

	if (menu->LoadMenu(IDR_RB_UT_SUBSET)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
	}

	delete menu;
	
}

void CUtSubsetPage::OnUtFillall() 
{
	

	theApp.m_LastSettings.n64SubsetUtVariable = 0xffffffffffff;
	UpdateAllControls();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->RedrawUltrasonicsSheet();
	
}

void CUtSubsetPage::OnUtClearall() 
{
	

	theApp.m_LastSettings.n64SubsetUtVariable = 0;
	UpdateAllControls();
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->RedrawUltrasonicsSheet();
}


