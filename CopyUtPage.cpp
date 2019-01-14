// CopyUtPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CopyUtPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyUtPage property page

IMPLEMENT_DYNCREATE(CCopyUtPage, CPropertyPage)

CCopyUtPage::CCopyUtPage() : CPropertyPage(CCopyUtPage::IDD)
{
	//{{AFX_DATA_INIT(CCopyUtPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CCopyUtPage::~CCopyUtPage()
{
}

void CCopyUtPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyUtPage)
	DDX_Control(pDX, IDC_LIST_UT_VARIABLES, m_listUtVariables);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CCopyUtPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCopyUtPage)
	ON_LBN_SELCHANGE(IDC_LIST_UT_VARIABLES, OnSelchangeListUtVariables)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_UT_VARIABLES, OnCheckChangeListUtVariables)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyUtPage message handlers

BOOL CCopyUtPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	bool bFlag;
	
	m_listUtVariables.ResetContent();
	m_listUtVariables.SetCheckStyle(BS_AUTOCHECKBOX);

	Buff.LoadString(IDS_Preamp_Gain);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Preamp_Mode);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Damping);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulse_Width);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulse_Voltage);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Linear_Gain);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Log_Gain);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Output_Mode);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Rectifier_Mode);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Type);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Range);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Center);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Cut_Off);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Mode);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Trigger);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Delay);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Threshold);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Table);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Scope_Delay);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Scope_Width);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Mode);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Delay);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Width);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Threshold);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Dead_Zone);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Peak_Width);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Amplitude_Collect);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_TOF_Collect);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_TOF_Mode);							m_listUtVariables.AddString(Buff);

	theApp.m_LastSettings.n64CopyUtVariable & _UT_PREAMP_GAIN ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(0,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PREAMP_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(1,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAMPING ? bFlag = true : bFlag = false;			m_listUtVariables.SetCheck(2,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSE_WIDTH ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(3,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_PULSE_VOLTAGE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(4,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_LINEAR_GAIN ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(5,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_LOG_GAIN ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(6,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_OUTPUT_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(7,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_RECTIFY_MODE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(8,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_TYPE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(9,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_RANGE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(10,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_CENTER ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(11,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_FILTER_CUT_OFF ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(12,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(13,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_TRIGGER ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(14,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_DELAY ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(15,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_THRESHOLD ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(16,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_DAC_TABLE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(17,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_SCOPE_DELAY ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(18,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_SCOPE_WIDTH ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(19,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(20,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_DELAY ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(21,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_WIDTH ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(22,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_THRESHOLD ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(23,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_FAR_DEAD_ZONE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(24,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_FAR_PEAK_WIDTH ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(25,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_AMPLITUDE_COLLECT ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(26,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_TOF_COLLECT ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(27,bFlag);
	theApp.m_LastSettings.n64CopyUtVariable & _UT_TOF_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(28,bFlag);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCopyUtPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastCustomizeTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CCopyUtPage::OnSelchangeListUtVariables() 
{

}

void CCopyUtPage::OnCheckChangeListUtVariables() 
{
	theApp.m_LastSettings.n64CopyUtVariable = 0;
	if(m_listUtVariables.GetCheck(0)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_PREAMP_GAIN;	
	if(m_listUtVariables.GetCheck(1)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_PREAMP_MODE;	
	if(m_listUtVariables.GetCheck(2)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_DAMPING;	
	if(m_listUtVariables.GetCheck(3)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_PULSE_WIDTH;	
	if(m_listUtVariables.GetCheck(4)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_PULSE_VOLTAGE;	
	if(m_listUtVariables.GetCheck(5)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_LINEAR_GAIN;	
	if(m_listUtVariables.GetCheck(6)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_LOG_GAIN;	
	if(m_listUtVariables.GetCheck(7)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_OUTPUT_MODE;	
	if(m_listUtVariables.GetCheck(8)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_RECTIFY_MODE;	
	if(m_listUtVariables.GetCheck(9)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_TYPE;	
	if(m_listUtVariables.GetCheck(10)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_RANGE;	
	if(m_listUtVariables.GetCheck(11)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_CENTER;	
	if(m_listUtVariables.GetCheck(12)) theApp.m_LastSettings.n64CopyUtVariable |= _UT_FILTER_CUT_OFF;
	if(m_listUtVariables.GetCheck(13)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_DAC_MODE;
	if(m_listUtVariables.GetCheck(14)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_DAC_TRIGGER;
	if(m_listUtVariables.GetCheck(15)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_DAC_DELAY;
	if(m_listUtVariables.GetCheck(16)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_DAC_THRESHOLD;
	if(m_listUtVariables.GetCheck(17)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_DAC_TABLE;
	if(m_listUtVariables.GetCheck(18)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_SCOPE_DELAY;
	if(m_listUtVariables.GetCheck(19)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_SCOPE_WIDTH;
	if(m_listUtVariables.GetCheck(20)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_GATE_MODE;
	if(m_listUtVariables.GetCheck(21)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_GATE_DELAY;
	if(m_listUtVariables.GetCheck(22)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_GATE_WIDTH;
	if(m_listUtVariables.GetCheck(23)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_GATE_THRESHOLD;
	if(m_listUtVariables.GetCheck(24)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_GATE_FAR_DEAD_ZONE;
	if(m_listUtVariables.GetCheck(25)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_GATE_FAR_PEAK_WIDTH;
	if(m_listUtVariables.GetCheck(26)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_AMPLITUDE_COLLECT;
	if(m_listUtVariables.GetCheck(27)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_TOF_COLLECT;
	if(m_listUtVariables.GetCheck(28)) theApp.m_LastSettings.n64CopyUtVariable |=	_UT_TOF_MODE;


}
