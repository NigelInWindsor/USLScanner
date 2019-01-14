// CalibrationPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CalibrationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibrationPage property page

IMPLEMENT_DYNCREATE(CCalibrationPage, CPropertyPage)

CCalibrationPage::CCalibrationPage() : CPropertyPage(CCalibrationPage::IDD)
{
	//{{AFX_DATA_INIT(CCalibrationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CCalibrationPage::~CCalibrationPage()
{
}

void CCalibrationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibrationPage)
	DDX_Control(pDX, IDC_SPIN_THICKNESS_RECALIBRATION, m_spinThicknessReCalibration);
	DDX_Control(pDX, IDC_EDIT_THICK_RECAL_PERIOD, m_editThicknessReCalibrationPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalibrationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCalibrationPage)
	ON_EN_CHANGE(IDC_EDIT_THICK_RECAL_PERIOD, OnChangeEditThickRecalPeriod)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THICKNESS_RECALIBRATION, OnDeltaposSpinThicknessRecalibration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibrationPage message handlers

BOOL CCalibrationPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	m_spinThicknessReCalibration.SetRange(0,1000);
	m_spinThicknessReCalibration.SetPos(500);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalibrationPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"Days:%d Hours:%d",theApp.m_LastSettings.osThicknessReCalibrationPeriod/86400,(theApp.m_LastSettings.osThicknessReCalibrationPeriod%86400)/3600);

	m_editThicknessReCalibrationPeriod.SetWindowText(Buff);
}

void CCalibrationPage::OnChangeEditThickRecalPeriod() 
{
	CString Buff;
	int nDays,nHours;

	m_editThicknessReCalibrationPeriod.GetWindowText(Buff);

	for(int ii = 0; ii<Buff.GetLength();ii++) {
		if((isdigit(Buff.GetAt(ii))) || (Buff.GetAt(ii) == ' ')) {

	} else {
			Buff.Remove(Buff.GetAt(ii));
			ii-=1;
		}
	}

	if(Buff.GetLength()>1) {
		swscanf_s(Buff.GetBuffer(100),L"%d %d",&nDays,&nHours);

		theApp.m_LastSettings.osThicknessReCalibrationPeriod = (nDays * 86400) + (nHours * 3600);
	}
}

void CCalibrationPage::OnDeltaposSpinThicknessRecalibration(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	theApp.m_LastSettings.osThicknessReCalibrationPeriod += (pNMUpDown->iDelta * 3600);

	Buff.Format(L"Days:%d Hours:%d",theApp.m_LastSettings.osThicknessReCalibrationPeriod/86400,(theApp.m_LastSettings.osThicknessReCalibrationPeriod%86400)/3600);

	m_editThicknessReCalibrationPeriod.SetWindowText(Buff);

	*pResult = 0;
}
