// ReportHeaderFooterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ReportHeaderFooterDlg.h"
#include "ReportPrintDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportHeaderFooterDlg dialog


CReportHeaderFooterDlg::CReportHeaderFooterDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,FeatureStruct* pFeature)
	: CDialog(CReportHeaderFooterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportHeaderFooterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pFeature = pFeature;
	
}


void CReportHeaderFooterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportHeaderFooterDlg)
	DDX_Control(pDX, IDC_STATIC_HEADER_COLOUR, m_staticHeaderColour);
	DDX_Control(pDX, IDC_STATIC_FOOTER_COLOUR, m_staticFooterColour);
	DDX_Control(pDX, IDC_CHECK_HEADER, m_checkHeader);
	DDX_Control(pDX, IDC_CHECK_FOOTER, m_checkFooter);
	DDX_Control(pDX, IDC_SPIN_HEADER_HEIGHT, m_spinHeaderHeight);
	DDX_Control(pDX, IDC_SPIN_FOOTER_HEIGHT, m_spinFooterHeight);
	DDX_Control(pDX, IDC_EDIT_HEADER_HEIGHT, m_editHeaderHeight);
	DDX_Control(pDX, IDC_EDIT_FOOTER_HEIGHT, m_editFooterHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportHeaderFooterDlg, CDialog)
	//{{AFX_MSG_MAP(CReportHeaderFooterDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_FOOTER_HEIGHT, OnChangeEditFooterHeight)
	ON_EN_CHANGE(IDC_EDIT_HEADER_HEIGHT, OnChangeEditHeaderHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FOOTER_HEIGHT, OnDeltaposSpinFooterHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HEADER_HEIGHT, OnDeltaposSpinHeaderHeight)
	ON_BN_CLICKED(IDC_CHECK_HEADER, OnCheckHeader)
	ON_BN_CLICKED(IDC_CHECK_FOOTER, OnCheckFooter)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportHeaderFooterDlg message handlers

void CReportHeaderFooterDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CReportHeaderFooterDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CReportHeaderFooterDlg::IDD]=FALSE;

	
	CDialog::OnClose();
	DestroyWindow();
}

void CReportHeaderFooterDlg::OnDestroy() 
{
	

	if (CReportHeaderFooterDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CReportHeaderFooterDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CReportHeaderFooterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	
	
	m_spinFooterHeight.SetRange(0,10000);
	m_spinHeaderHeight.SetRange(0,10000);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportHeaderFooterDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	
	CReportPrintDlg *pReport = (CReportPrintDlg*)m_pParent;

	Buff.Format(L"%.01f mm",((float)m_pFeature->rrHeader.Height() * pReport->m_fPrintingAreaHeightMM) / 10000.0f);
	m_editHeaderHeight.SetWindowText(Buff);

	Buff.Format(L"%.01f mm",((float)m_pFeature->rrFooter.Height() * pReport->m_fPrintingAreaHeightMM) / 10000.0f);
	m_editFooterHeight.SetWindowText(Buff);

	m_checkHeader.SetCheck((int)m_pFeature->bHeader);
	m_checkFooter.SetCheck((int)m_pFeature->bFooter);

	m_spinHeaderHeight.SetPos(m_pFeature->rrHeader.bottom);
	m_spinFooterHeight.SetPos(m_pFeature->rrFooter.top);

}

void CReportHeaderFooterDlg::OnChangeEditFooterHeight() 
{
	float fHeight;
	CString Buff;
	CReportPrintDlg *pReport = (CReportPrintDlg*)m_pParent;
	int nTemp = m_pFeature->rrFooter.Height();

	m_editFooterHeight.GetWindowText(Buff);
	_WTOF(Buff,fHeight);
	int nHeight = (int)((fHeight * 10000.0f) / pReport->m_fPrintingAreaHeightMM);

	if(nTemp - nHeight) {
		m_pFeature->rrFooter.top = m_pFeature->rrFooter.bottom - nHeight;
		m_pParent->Invalidate(FALSE);
	}

		
}

void CReportHeaderFooterDlg::OnChangeEditHeaderHeight() 
{
	float fHeight;
	CString Buff;
	CReportPrintDlg *pReport = (CReportPrintDlg*)m_pParent;
	int nTemp = m_pFeature->rrHeader.Height();

	m_editHeaderHeight.GetWindowText(Buff);
	_WTOF(Buff,fHeight);
	int nHeight = (int)((fHeight * 10000.0f) / pReport->m_fPrintingAreaHeightMM);

	if(nTemp - nHeight) {
		m_pFeature->rrHeader.top = m_pFeature->rrHeader.bottom - nHeight;
		m_pParent->Invalidate(FALSE);
	}
}

void CReportHeaderFooterDlg::OnDeltaposSpinFooterHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CReportPrintDlg *pReport = (CReportPrintDlg*)m_pParent;

	int nStep = (int)((0.1f * 10000.0f) / pReport->m_fPrintingAreaHeightMM);
	m_pFeature->rrFooter.top -= (pNMUpDown->iDelta * nStep);
	UpdateAllControls();

	*pResult = 0;
}

void CReportHeaderFooterDlg::OnDeltaposSpinHeaderHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CReportPrintDlg *pReport = (CReportPrintDlg*)m_pParent;

	int nStep = (int)((0.1f * 10000.0f) / pReport->m_fPrintingAreaHeightMM);
	m_pFeature->rrHeader.bottom -= (pNMUpDown->iDelta * nStep);
	UpdateAllControls();
	
	*pResult = 0;
}

void CReportHeaderFooterDlg::OnCheckHeader() 
{
	m_pFeature->bHeader == false ? m_pFeature->bHeader = true : m_pFeature->bHeader = false;
	UpdateAllControls();
	m_pParent->Invalidate(FALSE);
	
}

void CReportHeaderFooterDlg::OnCheckFooter() 
{
	m_pFeature->bFooter == false ? m_pFeature->bFooter = true : m_pFeature->bFooter = false;
	UpdateAllControls();
	m_pParent->Invalidate(FALSE);
}

void CReportHeaderFooterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticHeaderColour,m_pFeature->rgbHeader);
	BlockFill(&dc,&m_staticFooterColour,m_pFeature->rgbFooter);
}

void CReportHeaderFooterDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CReportHeaderFooterDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticHeaderColour.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbHeader=dlg.GetColor();
			Invalidate(FALSE);
			m_pParent->Invalidate(FALSE);
		}
	}
	m_staticFooterColour.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbFooter=dlg.GetColor();
			Invalidate(FALSE);
			m_pParent->Invalidate(FALSE);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}
