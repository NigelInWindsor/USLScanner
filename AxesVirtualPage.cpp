// AxesVirtualPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "AxesVirtualPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesVirtualPage property page

IMPLEMENT_DYNCREATE(CAxesVirtualPage, CPropertyPage)

CAxesVirtualPage::CAxesVirtualPage() : CPropertyPage(CAxesVirtualPage::IDD)
{
	//{{AFX_DATA_INIT(CAxesVirtualPage)
	//}}AFX_DATA_INIT
	
	FillMemory(m_nPos,sizeof m_nPos,0xcc);
	FillMemory(m_nStatus, sizeof m_nStatus,0xcc);
}

CAxesVirtualPage::~CAxesVirtualPage()
{
}

void CAxesVirtualPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxesVirtualPage)
	DDX_Control(pDX, IDC_CHECK_V_AMP_9, m_checkVAmp9);
	DDX_Control(pDX, IDC_CHECK_V_AMP_8, m_checkVAmp8);
	DDX_Control(pDX, IDC_CHECK_V_AMP_7, m_checkVAmp7);
	DDX_Control(pDX, IDC_CHECK_V_AMP_6, m_checkVAmp6);
	DDX_Control(pDX, IDC_CHECK_V_AMP_5, m_checkVAmp5);
	DDX_Control(pDX, IDC_CHECK_V_AMP_4, m_checkVAmp4);
	DDX_Control(pDX, IDC_CHECK_V_AMP_3, m_checkVAmp3);
	DDX_Control(pDX, IDC_CHECK_V_AMP_2, m_checkVAmp2);
	DDX_Control(pDX, IDC_CHECK_V_AMP_10, m_checkVAmp10);
	DDX_Control(pDX, IDC_CHECK_V_AMP_1, m_checkVAmp1);
	DDX_Control(pDX, IDC_CHECK_V_AMP_0, m_checkVAmp0);
	DDX_Control(pDX, IDC_CHECK_AMP_9, m_checkAmp9);
	DDX_Control(pDX, IDC_CHECK_AMP_8, m_checkAmp8);
	DDX_Control(pDX, IDC_CHECK_AMP_7, m_checkAmp7);
	DDX_Control(pDX, IDC_CHECK_AMP_6, m_checkAmp6);
	DDX_Control(pDX, IDC_CHECK_AMP_5, m_checkAmp5);
	DDX_Control(pDX, IDC_CHECK_AMP_4, m_checkAmp4);
	DDX_Control(pDX, IDC_CHECK_AMP_3, m_checkAmp3);
	DDX_Control(pDX, IDC_CHECK_AMP_2, m_checkAmp2);
	DDX_Control(pDX, IDC_CHECK_AMP_10, m_checkAmp10);
	DDX_Control(pDX, IDC_CHECK_AMP_1, m_checkAmp1);
	DDX_Control(pDX, IDC_CHECK_AMP_0, m_checkAmp0);
	DDX_Control(pDX, IDC_EDIT_COORDINATE_STATUS, m_editCoordinateStatus);
	DDX_Control(pDX, IDC_EDIT_PROGRAM_STATUS, m_editProgramStatus);
	DDX_Control(pDX, IDC_EDIT_COORD_SYSTEM_2, m_editCoordSystem2);
	DDX_Control(pDX, IDC_EDIT_COORD_SYSTEM_1, m_editCoordSystem1);
	DDX_Control(pDX, IDC_STATIC_AXIS_10, m_static10);
	DDX_Control(pDX, IDC_STATIC_AXIS_9, m_static9);
	DDX_Control(pDX, IDC_STATIC_AXIS_8, m_static8);
	DDX_Control(pDX, IDC_STATIC_AXIS_7, m_static7);
	DDX_Control(pDX, IDC_STATIC_AXIS_6, m_static6);
	DDX_Control(pDX, IDC_STATIC_AXIS_5, m_static5);
	DDX_Control(pDX, IDC_STATIC_AXIS_4, m_static4);
	DDX_Control(pDX, IDC_STATIC_AXIS_3, m_static3);
	DDX_Control(pDX, IDC_STATIC_AXIS_2, m_static2);
	DDX_Control(pDX, IDC_STATIC_AXIS_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_AXIS_0, m_static0);
	DDX_Control(pDX, IDC_EDIT_V_POS_10, m_editVPos10);
	DDX_Control(pDX, IDC_EDIT_V_POS_9, m_editVPos9);
	DDX_Control(pDX, IDC_EDIT_V_POS_8, m_editVPos8);
	DDX_Control(pDX, IDC_EDIT_V_POS_7, m_editVPos7);
	DDX_Control(pDX, IDC_EDIT_V_POS_6, m_editVPos6);
	DDX_Control(pDX, IDC_EDIT_V_POS_5, m_editVPos5);
	DDX_Control(pDX, IDC_EDIT_V_POS_4, m_editVPos4);
	DDX_Control(pDX, IDC_EDIT_V_POS_3, m_editVPos3);
	DDX_Control(pDX, IDC_EDIT_V_POS_2, m_editVPos2);
	DDX_Control(pDX, IDC_EDIT_V_POS_1, m_editVPos1);
	DDX_Control(pDX, IDC_EDIT_V_POS_0, m_editVPos0);
	DDX_Control(pDX, IDC_EDIT_POS_10, m_editPos10);
	DDX_Control(pDX, IDC_EDIT_POS_9, m_editPos9);
	DDX_Control(pDX, IDC_EDIT_POS_8, m_editPos8);
	DDX_Control(pDX, IDC_EDIT_POS_7, m_editPos7);
	DDX_Control(pDX, IDC_EDIT_POS_6, m_editPos6);
	DDX_Control(pDX, IDC_EDIT_POS_5, m_editPos5);
	DDX_Control(pDX, IDC_EDIT_POS_4, m_editPos4);
	DDX_Control(pDX, IDC_EDIT_POS_3, m_editPos3);
	DDX_Control(pDX, IDC_EDIT_POS_2, m_editPos2);
	DDX_Control(pDX, IDC_EDIT_POS_1, m_editPos1);
	DDX_Control(pDX, IDC_EDIT_POS_0, m_editPos0);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_10, m_checkVInPos10);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_9, m_checkVInPos9);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_8, m_checkVInPos8);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_7, m_checkVInPos7);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_6, m_checkVInPos6);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_5, m_checkVInPos5);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_4, m_checkVInPos4);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_3, m_checkVInPos3);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_2, m_checkVInPos2);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_1, m_checkVInPos1);
	DDX_Control(pDX, IDC_CHECK_V_INPOS_0, m_checkVInPos0);
	DDX_Control(pDX, IDC_CHECK_INPOS_10, m_checkInPos10);
	DDX_Control(pDX, IDC_CHECK_INPOS_9, m_checkInPos9);
	DDX_Control(pDX, IDC_CHECK_INPOS_8, m_checkInPos8);
	DDX_Control(pDX, IDC_CHECK_INPOS_7, m_checkInPos7);
	DDX_Control(pDX, IDC_CHECK_INPOS_6, m_checkInPos6);
	DDX_Control(pDX, IDC_CHECK_INPOS_5, m_checkInPos5);
	DDX_Control(pDX, IDC_CHECK_INPOS_4, m_checkInPos4);
	DDX_Control(pDX, IDC_CHECK_INPOS_3, m_checkInPos3);
	DDX_Control(pDX, IDC_CHECK_INPOS_2, m_checkInPos2);
	DDX_Control(pDX, IDC_CHECK_INPOS_1, m_checkInPos1);
	DDX_Control(pDX, IDC_CHECK_INPOS_0, m_checkInPos0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxesVirtualPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAxesVirtualPage)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesVirtualPage message handlers

BOOL CAxesVirtualPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAxesVirtualPage::OnSetActive() 
{
	FillMemory(m_nPos,sizeof m_nPos,0xcc);
	FillMemory(m_nStatus, sizeof m_nStatus,0xcc);
	SetTimer(1,200,NULL);

	theApp.m_PmacUser.m_bRetrieveVirtualAxes = true;
	theApp.m_PmacUser.SendStr(L"ENABLE PLCC 4");
	
	return CPropertyPage::OnSetActive();
}

BOOL CAxesVirtualPage::OnKillActive() 
{
	theApp.m_PmacUser.m_bRetrieveVirtualAxes = true;
	theApp.m_PmacUser.SendStr(L"DISABLE PLCC 4");
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CAxesVirtualPage::UpdateAllControls()
{
	m_static0.SetWindowText(theApp.m_Axes[0].cName);
	m_static1.SetWindowText(theApp.m_Axes[1].cName);
	m_static2.SetWindowText(theApp.m_Axes[2].cName);
	m_static3.SetWindowText(theApp.m_Axes[3].cName);
	m_static4.SetWindowText(theApp.m_Axes[4].cName);
	m_static5.SetWindowText(theApp.m_Axes[5].cName);
	m_static6.SetWindowText(theApp.m_Axes[6].cName);
	m_static7.SetWindowText(theApp.m_Axes[7].cName);
	m_static8.SetWindowText(theApp.m_Axes[8].cName);
	m_static9.SetWindowText(theApp.m_Axes[9].cName);
	m_static10.SetWindowText(theApp.m_Axes[11].cName);

}

void CAxesVirtualPage::OnTimer(UINT nIDEvent) 
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	static int nProgramNumber[2] = {0xcd,0xcd};
	static int nProgramStatus = -1;
	static int nCoordinateStatus = -1;

	InvalidateVariables(0,20,&m_editPos0,&m_editVPos0,&m_checkInPos0,&m_checkVInPos0,&m_checkAmp0,&m_checkVAmp0);
	InvalidateVariables(1,21,&m_editPos1,&m_editVPos1,&m_checkInPos1,&m_checkVInPos1,&m_checkAmp1,&m_checkVAmp1);
	InvalidateVariables(2,22,&m_editPos2,&m_editVPos2,&m_checkInPos2,&m_checkVInPos2,&m_checkAmp2,&m_checkVAmp2);
	InvalidateVariables(3,23,&m_editPos3,&m_editVPos3,&m_checkInPos3,&m_checkVInPos3,&m_checkAmp3,&m_checkVAmp3);
	InvalidateVariables(4,24,&m_editPos4,&m_editVPos4,&m_checkInPos4,&m_checkVInPos4,&m_checkAmp4,&m_checkVAmp4);
	InvalidateVariables(5,25,&m_editPos5,&m_editVPos5,&m_checkInPos5,&m_checkVInPos5,&m_checkAmp5,&m_checkVAmp5);
	InvalidateVariables(6,26,&m_editPos6,&m_editVPos6,&m_checkInPos6,&m_checkVInPos6,&m_checkAmp6,&m_checkVAmp6);
	InvalidateVariables(7,27,&m_editPos7,&m_editVPos7,&m_checkInPos7,&m_checkVInPos7,&m_checkAmp7,&m_checkVAmp7);
	InvalidateVariables(8,28,&m_editPos8,&m_editVPos8,&m_checkInPos8,&m_checkVInPos8,&m_checkAmp8,&m_checkVAmp8);
	InvalidateVariables(9,29,&m_editPos9,&m_editVPos9,&m_checkInPos9,&m_checkVInPos9,&m_checkAmp9,&m_checkVAmp9);
	InvalidateVariables(11,31,&m_editPos10,&m_editVPos10,&m_checkInPos10,&m_checkVInPos10,&m_checkAmp10,&m_checkVAmp10);

	if(nProgramNumber[0] - theApp.m_PmacUser.m_nProgramNumber[0]) {
		Buff.Format(L"%d",nProgramNumber[0] = theApp.m_PmacUser.m_nProgramNumber[0]);
		m_editCoordSystem1.SetWindowText(Buff);
	}

	if(nProgramNumber[1] - theApp.m_PmacUser.m_nProgramNumber[1]) {
		Buff.Format(L"%d",nProgramNumber[1] = theApp.m_PmacUser.m_nProgramNumber[1]);
		m_editCoordSystem2.SetWindowText(Buff);
	}

	if(nProgramStatus - theApp.m_PmacUser.m_nProgramStatus) {
		Buff.Format(L"%d",nProgramStatus = theApp.m_PmacUser.m_nProgramStatus);
		m_editProgramStatus.SetWindowText(Buff);
	}

	if(nCoordinateStatus - theApp.m_PmacUser.m_nCoordinateStatus) {
		Buff.Format(L"%d",nCoordinateStatus = theApp.m_PmacUser.m_nCoordinateStatus);
		m_editCoordinateStatus.SetWindowText(Buff);
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CAxesVirtualPage::InvalidateVariables(int nAxis, int nVAxis, CEdit *peditPos, CEdit *peditVPos, CButton *pcheckInPos, CButton *pcheckVInPos, CButton *pcheckAmp, CButton *pcheckVAmp)
{
	CString Buff;


	AxisData *pAxis = &theApp.m_Axes[nAxis];
	AxisData *pVAxis = &theApp.m_Axes[nVAxis];

	if(m_nPos[nAxis] - pAxis->nPos) {
		Buff.Format(L"%d",pAxis->nPos+pAxis->nOffset);
		peditPos->SetWindowText(Buff);
		m_nPos[nAxis] = pAxis->nPos;
	}
	if(m_nPos[nVAxis] - pVAxis->nPos) {
		Buff.Format(L"%d",pVAxis->nPos+pVAxis->nOffset);
		peditVPos->SetWindowText(Buff);
		m_nPos[nVAxis] = pVAxis->nPos;
	}
	if(m_nStatus[nAxis] - pAxis->nStatus) {
		pAxis->nStatus & PMAC_IN_POS ? pcheckInPos->SetCheck(TRUE) : pcheckInPos->SetCheck(FALSE);
	}
	if(m_nStatus[nVAxis] - pVAxis->nStatus) {
		pVAxis->nStatus & PMAC_IN_POS ? pcheckVInPos->SetCheck(TRUE) : pcheckVInPos->SetCheck(FALSE);
	}
	if(m_nStatus[nAxis] - pAxis->nStatus) {
		pAxis->nStatus & PMAC_AMPLIFIER_ENABLED ? pcheckAmp->SetCheck(TRUE) : pcheckAmp->SetCheck(FALSE);
	}
	if(m_nStatus[nVAxis] - pVAxis->nStatus) {
		pVAxis->nStatus & PMAC_AMPLIFIER_ENABLED ? pcheckVAmp->SetCheck(TRUE) : pcheckVAmp->SetCheck(FALSE);
	}
	m_nStatus[nAxis] = pAxis->nStatus;
	m_nStatus[nVAxis] = pVAxis->nStatus;
}
