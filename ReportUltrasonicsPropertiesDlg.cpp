// ReportUltrasonicsPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ReportUltrasonicsPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportUltrasonicsPropertiesDlg dialog


CReportUltrasonicsPropertiesDlg::CReportUltrasonicsPropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature,CData *pData)
	: CDialog(CReportUltrasonicsPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportUltrasonicsPropertiesDlg)
	//}}AFX_DATA_INIT
	m_pFeature = pFeature;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pData = pData;
	

}


void CReportUltrasonicsPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportUltrasonicsPropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	DDX_Control(pDX, IDC_STATIC_TITLE_BK, m_staticTitleBk);
	DDX_Control(pDX, IDC_STATIC_TITLE_TEXT, m_staticTitleText);
	DDX_Control(pDX, IDC_CHECK_TITLE_BAR, m_checkTitleBar);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_editTitle);
	DDX_Control(pDX, IDC_LIST_VARIABLES, m_listUtVariables);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportUltrasonicsPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CReportUltrasonicsPropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_TITLE, OnChangeEditTitle)
	ON_BN_CLICKED(IDC_CHECK_TITLE_BAR, OnCheckTitleBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, OnSelchangeComboLanguage)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_VARIABLES, OnCheckChangeListUtVariables)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportUltrasonicsPropertiesDlg message handlers

void CReportUltrasonicsPropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CReportUltrasonicsPropertiesDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CReportUltrasonicsPropertiesDlg::IDD]=FALSE;

	
	CDialog::OnClose();
	DestroyWindow();
}

void CReportUltrasonicsPropertiesDlg::OnDestroy() 
{
	

	if (CReportUltrasonicsPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CReportUltrasonicsPropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CReportUltrasonicsPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	
	bool bFlag;
	
	m_listUtVariables.ResetContent();
	m_listUtVariables.SetCheckStyle(BS_AUTOCHECKBOX);

	Buff.LoadString(IDS_Preamp_Gain);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Tx_Rx);								m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Damping);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulse_Width);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Pulse_Voltage);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gain);								m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Output_Mode);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Type);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Range);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Center);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Filter_Cut_Off);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_DAC_Mode);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Mode);							m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Delay);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Width);						m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Threshold);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_Gate_Dead_Zone);					m_listUtVariables.AddString(Buff);
	Buff.LoadString(IDS_TOF_Mode);							m_listUtVariables.AddString(Buff);

	m_pFeature->n64UtVariables & FEATURE_UT_PREAMP_GAIN ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(0,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_TX_RX ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(1,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_DAMPING ? bFlag = true : bFlag = false;			m_listUtVariables.SetCheck(2,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_PULSE_WIDTH ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(3,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_PULSE_VOLTAGE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(4,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_GAIN ? bFlag = true : bFlag = false;			m_listUtVariables.SetCheck(5,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_OUTPUT_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(6,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_FILTER_TYPE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(7,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_FILTER_RANGE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(8,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_FILTER_CENTER ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(9,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_FILTER_CUT_OFF ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(10,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_DAC_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(11,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_GATE_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(12,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_GATE_DELAY ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(13,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_GATE_WIDTH ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(14,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_GATE_THRESHOLD ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(15,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_GATE_FAR_DEAD_ZONE ? bFlag = true : bFlag = false;	m_listUtVariables.SetCheck(16,bFlag);
	m_pFeature->n64UtVariables & FEATURE_UT_TOF_MODE ? bFlag = true : bFlag = false;		m_listUtVariables.SetCheck(17,bFlag);

	Buff.LoadString(IDS_English);	m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Display);	m_comboLanguage.AddString(Buff);

	
	UpdateAllControls();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportUltrasonicsPropertiesDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_pFeature->unFlags & FEATURE_TITLEBAR ? m_checkTitleBar.SetCheck(true) : m_checkTitleBar.SetCheck(false);

	m_editTitle.SetWindowText( *m_pFeature->pCaption );

	m_comboLanguage.SetCurSel(m_pFeature->nLanguage);

}

void CReportUltrasonicsPropertiesDlg::OnChangeEditTitle() 
{
	CString Buff;

	m_editTitle.GetWindowText(Buff);
	if(*m_pFeature->pCaption != Buff) {
		*m_pFeature->pCaption = Buff;
		UpdateParent();
	}
}

void CReportUltrasonicsPropertiesDlg::OnCheckTitleBar() 
{
	if(m_checkTitleBar.GetCheck() == TRUE) {
		m_pFeature->unFlags |= FEATURE_TITLEBAR;
	} else {
		m_pFeature->unFlags &= ~FEATURE_TITLEBAR;
	}
	UpdateParent();
}

void CReportUltrasonicsPropertiesDlg::OnCheckChangeListUtVariables() 
{
	

	m_pFeature->n64UtVariables = 0;
	if(m_listUtVariables.GetCheck(0)) m_pFeature->n64UtVariables |= FEATURE_UT_PREAMP_GAIN;	
	if(m_listUtVariables.GetCheck(1)) m_pFeature->n64UtVariables |= FEATURE_UT_TX_RX;	
	if(m_listUtVariables.GetCheck(2)) m_pFeature->n64UtVariables |= FEATURE_UT_DAMPING;	
	if(m_listUtVariables.GetCheck(3)) m_pFeature->n64UtVariables |= FEATURE_UT_PULSE_WIDTH;	
	if(m_listUtVariables.GetCheck(4)) m_pFeature->n64UtVariables |= FEATURE_UT_PULSE_VOLTAGE;	
	if(m_listUtVariables.GetCheck(5)) m_pFeature->n64UtVariables |= FEATURE_UT_GAIN;	
	if(m_listUtVariables.GetCheck(6)) m_pFeature->n64UtVariables |= FEATURE_UT_OUTPUT_MODE;	
	if(m_listUtVariables.GetCheck(7)) m_pFeature->n64UtVariables |= FEATURE_UT_FILTER_TYPE;	
	if(m_listUtVariables.GetCheck(8)) m_pFeature->n64UtVariables |= FEATURE_UT_FILTER_RANGE;	
	if(m_listUtVariables.GetCheck(9)) m_pFeature->n64UtVariables |= FEATURE_UT_FILTER_CENTER;	
	if(m_listUtVariables.GetCheck(10)) m_pFeature->n64UtVariables |= FEATURE_UT_FILTER_CUT_OFF;
	if(m_listUtVariables.GetCheck(11)) m_pFeature->n64UtVariables |=	FEATURE_UT_DAC_MODE;
	if(m_listUtVariables.GetCheck(12)) m_pFeature->n64UtVariables |=	FEATURE_UT_GATE_MODE;
	if(m_listUtVariables.GetCheck(13)) m_pFeature->n64UtVariables |=	FEATURE_UT_GATE_DELAY;
	if(m_listUtVariables.GetCheck(14)) m_pFeature->n64UtVariables |=	FEATURE_UT_GATE_WIDTH;
	if(m_listUtVariables.GetCheck(15)) m_pFeature->n64UtVariables |=	FEATURE_UT_GATE_THRESHOLD;
	if(m_listUtVariables.GetCheck(16)) m_pFeature->n64UtVariables |=	FEATURE_UT_GATE_FAR_DEAD_ZONE;
	if(m_listUtVariables.GetCheck(17)) m_pFeature->n64UtVariables |=	FEATURE_UT_TOF_MODE;

	theApp.m_LastSettings.n64FeatureUtVariables = m_pFeature->n64UtVariables;;

}

void CReportUltrasonicsPropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CColorDialog dlg;

	m_staticTitleText.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbTitle=dlg.GetColor();
			Invalidate(FALSE);
			UpdateParent();
		}
	}

	m_staticTitleBk.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbBkTitle=dlg.GetColor();
			Invalidate(FALSE);
			UpdateParent();
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CReportUltrasonicsPropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	BlockFill(&dc,&m_staticTitleText,m_pFeature->rgbTitle);
	BlockFill(&dc,&m_staticTitleBk,m_pFeature->rgbBkTitle);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CReportUltrasonicsPropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CReportUltrasonicsPropertiesDlg::UpdateParent()
{

	m_pParent->Invalidate(FALSE);


}

void CReportUltrasonicsPropertiesDlg::OnSelchangeComboLanguage() 
{
	m_pFeature->nLanguage = m_comboLanguage.GetCurSel();
	m_pParent->Invalidate(FALSE);

}
