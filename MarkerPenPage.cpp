// MarkerPenPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MarkerPenPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkerPenPage property page

IMPLEMENT_DYNCREATE(CMarkerPenPage, CPropertyPage)

CMarkerPenPage::CMarkerPenPage() : CPropertyPage(CMarkerPenPage::IDD)
{
	//{{AFX_DATA_INIT(CMarkerPenPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CMarkerPenPage::~CMarkerPenPage()
{
}

void CMarkerPenPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkerPenPage)
	DDX_Control(pDX, IDC_EDIT_JUNK, m_editJunk);
	DDX_Control(pDX, IDC_EDIT_COORD, m_editCoord);
	DDX_Control(pDX, IDC_SPIN_ON_TIME, m_spinOnTime);
	DDX_Control(pDX, IDC_EDIT_ON_TIME, m_editOnTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkerPenPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMarkerPenPage)
	ON_EN_CHANGE(IDC_EDIT_COORD, OnChangeEditCoord)
	ON_EN_CHANGE(IDC_EDIT_ON_TIME, OnChangeEditOnTime)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_JUNK, OnChangeEditJunk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkerPenPage message handlers

BOOL CMarkerPenPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_spinOnTime.SetRange(0,4000);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMarkerPenPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CMarkerPenPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CMarkerPenPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f",theApp.m_Tank.vecMarkerPen[0].x,theApp.m_Tank.vecMarkerPen[0].y,theApp.m_Tank.vecMarkerPen[0].z);
	m_editCoord.SetWindowText(Buff);

	Buff.Format(L"%d",theApp.m_Tank.nMarkerPenOnTime[0]);
	m_editOnTime.SetWindowText(Buff);
	m_spinOnTime.SetPos(theApp.m_Tank.nMarkerPenOnTime[0]);

}

void CMarkerPenPage::OnChangeEditCoord() 
{
	CString Buff;

	ExtractVector(&m_editCoord,&theApp.m_Tank.vecMarkerPen[0]);
}

void CMarkerPenPage::OnChangeEditOnTime() 
{
	CString Buff;

	m_editOnTime.GetWindowText(Buff);
	theApp.m_Tank.nMarkerPenOnTime[0] = _ttoi(Buff);
	MinMax(&theApp.m_Tank.nMarkerPenOnTime[0],0,4000);
	m_spinOnTime.SetPos(theApp.m_Tank.nMarkerPenOnTime[0]);
}

void CMarkerPenPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp;
	CString Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_ON_TIME:
		nTemp = theApp.m_Tank.nMarkerPenOnTime[0];
		theApp.m_Tank.nMarkerPenOnTime[0] = pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_Tank.nMarkerPenOnTime[0]) {
			Buff.Format(L"%d",theApp.m_Tank.nMarkerPenOnTime[0]);
			m_editOnTime.SetWindowText(Buff);
		}
		break;
	};
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


bool CMarkerPenPage::ExtractVector(CEdit *pEdit, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm,float *pfAngle)
{
	CString Buff;
	int ii,nStart;

	pEdit->GetWindowText(Buff);
	Buff.MakeUpper();

	for(ii=0;ii<Buff.GetLength();ii++) {
		if(!(isalnum(Buff.GetAt(ii)) || Buff.GetAt(ii) == '.' || Buff.GetAt(ii) == '-')) {
			Buff.Delete(ii);
			ii=-1;
		}
	}

	if((nStart=Buff.Find(L"X",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X%f",&pVec->x);
	if((nStart=Buff.Find(L"Y",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y%f",&pVec->y);
	if((nStart=Buff.Find(L"Z",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z%f",&pVec->z);

	if(pVecNorm != NULL) {
		if((nStart=Buff.Find(L"I",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"I%f",&pVecNorm->x);
		if((nStart=Buff.Find(L"J",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"J%f",&pVecNorm->y);
		if((nStart=Buff.Find(L"K",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"K%f",&pVecNorm->z);
	}
	if(pfAngle != NULL) {
		if((nStart=Buff.Find(L"U",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"U%f",pfAngle);
	}

	return true;
}

void CMarkerPenPage::OnChangeEditJunk() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
