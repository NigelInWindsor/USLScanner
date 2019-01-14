// LeAlignPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LeAlignPage.h"
#include "ViewBladeThickneessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeAlignPage property page

IMPLEMENT_DYNCREATE(CLeAlignPage, CPropertyPage)

CLeAlignPage::CLeAlignPage(CWnd* pParent) : CPropertyPage(CLeAlignPage::IDD)
{
	//{{AFX_DATA_INIT(CLeAlignPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pParent = pParent;
}

CLeAlignPage::~CLeAlignPage()
{
}

void CLeAlignPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeAlignPage)
	DDX_Control(pDX, IDC_COMBO_LE_ALGORITHM, m_comboLEAlgorithm);
	DDX_Control(pDX, IDC_SPIN_Y_OFFSET, m_spinYOffset);
	DDX_Control(pDX, IDC_EDIT_Y_OFFSET, m_editYOffset);
	DDX_Control(pDX, IDC_EDIT_LE_DROP, m_editLeDrop);
	DDX_Control(pDX, IDC_LIST_ALIGN_WHICH, m_listAlignWhich);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeAlignPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLeAlignPage)
	ON_BN_CLICKED(IDC_BUTTON_DO_IT, OnButtonDoIt)
	ON_EN_CHANGE(IDC_EDIT_LE_DROP, OnChangeEditLeDrop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_OFFSET, OnDeltaposSpinYOffset)
	ON_EN_CHANGE(IDC_EDIT_Y_OFFSET, OnChangeEditYOffset)
	ON_CBN_SELCHANGE(IDC_COMBO_LE_ALGORITHM, OnSelchangeComboLeAlgorithm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeAlignPage message handlers

BOOL CLeAlignPage::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();
	
	m_listAlignWhich.AddString(_T("This line"));
	m_listAlignWhich.AddString(_T("This scan"));
	m_listAlignWhich.AddString(_T("All scans"));
	m_listAlignWhich.SetCurSel(2);
	
	Buff.Format(_T("%.0f%%"),theApp.m_LastSettings.fLePercentageDrop);
	m_editLeDrop.SetWindowText(Buff);

	m_comboLEAlgorithm.AddString(L"> 2* LE Nominal");
	m_comboLEAlgorithm.SetCurSel( theApp.m_LastSettings.nLEAlgorithm );

	m_spinYOffset.SetRange(0,100);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CLeAlignPage::OnChangeEditLeDrop() 
{
	CString Buff;

	m_editLeDrop.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fLePercentageDrop);
}

void CLeAlignPage::OnButtonDoIt() 
{
	CViewBladeThickneessDlg* pParent = (CViewBladeThickneessDlg*) m_pParent;
	CData* pData;
	int nLine,nScanType;

	pData = &pParent->m_Data;
	
	switch(m_listAlignWhich.GetCurSel()) {
	case 0:
		pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].AlignLeadingEdge();
		break;
	case 1:
		for(nLine=0;nLine<pData->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
			pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].AlignLeadingEdge();
		}
		break;
	case 2:
		for(nScanType=0;nScanType<2;nScanType++) {
			for(nLine=0;nLine<pData->m_nLineBladeL[nScanType];nLine++) {
				pData->m_LineBlade[nScanType][nLine].AlignLeadingEdge();
			}
		}
		break;
	}

	pParent->Invalidate(FALSE);

}

BOOL CLeAlignPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CLeAlignPage::OnDeltaposSpinYOffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fTemp = theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide];
	CString Buff;

	theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] += ((float)pNMUpDown->iDelta * 0.1f);
	if(theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] < -5.0f) theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] = -5.0f;
	if(theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] >  5.0f) theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] =  5.0f;

	if(fTemp - theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide]) {
		Buff.Format(L"%.1f mm",theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide]);
		m_editYOffset.SetWindowText(Buff);
	}
	*pResult = 0;
}

void CLeAlignPage::OnChangeEditYOffset() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide];
	
	m_editYOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide]);

	if(fTemp - theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide]) {
		Buff.Format(L"%.1f mm",theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide]);
		m_editYOffset.SetWindowText(Buff);

		m_spinYOffset.SetPos((int)(theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] * 10.0f) + 50);
	}
}

void CLeAlignPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"%.1f mm",theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide]);
	m_editYOffset.SetWindowText(Buff);

	m_spinYOffset.SetPos((int)(theApp.m_LastSettings.fBladeYOffset[theApp.m_Thread.m_nBladeSide] * 10.0f) + 50);
}

void CLeAlignPage::OnSelchangeComboLeAlgorithm() 
{
	theApp.m_LastSettings.nLEAlgorithm = m_comboLEAlgorithm.GetCurSel();
}
