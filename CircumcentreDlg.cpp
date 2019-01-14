// CircumcentreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CircumcentreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCircumcentreDlg dialog


CCircumcentreDlg::CCircumcentreDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CCircumcentreDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCircumcentreDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
}


void CCircumcentreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCircumcentreDlg)
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_SPIN_RADIUS, m_spinRadius);
	DDX_Control(pDX, IDC_SPIN_EXTRA_POINTS, m_spinExtraPoints);
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_editRadius);
	DDX_Control(pDX, IDC_EDIT_EXTRA_POINTS, m_editExtraPoints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCircumcentreDlg, CDialog)
	//{{AFX_MSG_MAP(CCircumcentreDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_EN_CHANGE(IDC_EDIT_EXTRA_POINTS, OnChangeEditExtraPoints)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, OnChangeEditRadius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RADIUS, OnDeltaposSpinRadius)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircumcentreDlg message handlers

void CCircumcentreDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CCircumcentreDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CCircumcentreDlg::IDD & MAXIMUMWINDOWSIDNUMBER] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CCircumcentreDlg::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CCircumcentreDlg::IDD & MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CCircumcentreDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;

	CRect rect = theApp.m_LastSettings.RectWindowPos[CCircumcentreDlg::IDD & MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:	m_InitLine = &PROFILE->m_TaughtLine[theApp.m_nLine];
		break;
	case SCANLINES:	m_InitLine = &PROFILE->m_ScanLine[theApp.m_nLine];
		break;
	}

	Buff.LoadString(IDS_Circumcenter_between_2_points);	m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Circumcenter_between_3_points);	m_comboMode.AddString(Buff);
	m_comboMode.SetCurSel(m_nMode = 0);


	Buff.Format(L"%.01f",m_fRadius = 10.0f);
	m_editRadius.SetWindowText(Buff);

	m_spinRadius.SetRange(0,2000);
	m_spinRadius.SetPos((int)(m_fRadius * 10.0f));

	Buff.Format(L"%d",m_nNumberPts = 1);
	m_editExtraPoints.SetWindowText(Buff);


	m_spinExtraPoints.SetRange(1,30);
	m_spinExtraPoints.SetPos(m_nNumberPts);


	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCircumcentreDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;


}

void CCircumcentreDlg::OnSelchangeComboMode() 
{
	m_nMode = m_comboMode.GetCurSel();
	
}

void CCircumcentreDlg::OnChangeEditExtraPoints() 
{
	CString Buff;
	int nTemp = m_nNumberPts;

	m_editExtraPoints.GetWindowText(Buff);
	m_nNumberPts = _ttoi(Buff);
	MinMax(&m_nNumberPts,1,30);
	if(nTemp - m_nNumberPts) {
		m_spinExtraPoints.SetPos(m_nNumberPts);
	};


}

void CCircumcentreDlg::OnChangeEditRadius() 
{
	CString Buff;
	float  fTemp = m_fRadius;

	m_editRadius.GetWindowText(Buff);
	_WTOF(Buff,m_fRadius);
	MinMax(&m_fRadius,0.1f,300.0f);
	if(fTemp - m_fRadius) {
		m_spinRadius.SetPos((int)(m_fRadius * 10.0f));
	};
}

void CCircumcentreDlg::OnDeltaposSpinRadius(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = m_fRadius;

	m_fRadius += ((float)pNMUpDown->iDelta * 0.1f);
	MinMax(&m_fRadius,0.1f,300.0f);

	if(fTemp - m_fRadius) {
		Buff.Format(L"%.01f",m_fRadius);
		m_editRadius.SetWindowText(Buff);
	}

	*pResult = 0;
}

void CCircumcentreDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp;
	CString Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_EXTRA_POINTS:
		nTemp = m_nNumberPts;
		m_nNumberPts = pSpin->GetPos()&0xffff;
		MinMax(&m_nNumberPts,1,30);
		if(m_nNumberPts - nTemp) {
			Buff.Format(L"%d",m_nNumberPts);
			m_editExtraPoints.SetWindowText(Buff);
			InvalidateParent();
		}
		break;
	};
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCircumcentreDlg::InvalidateParent()
{

	m_ProcessedLine = &m_InitLine;

	switch(m_nMode) {
	case 0:
		break;
	case 1:
		break;
	}
}
