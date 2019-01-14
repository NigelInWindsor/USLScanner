// ScanLinesEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ScanLinesEditDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"
#include "PmacTrajectoryPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04
#define UPDATE_CHILDREN 0x08
/////////////////////////////////////////////////////////////////////////////
// CScanLinesEditDlg dialog


CScanLinesEditDlg::CScanLinesEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CScanLinesEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanLinesEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pFlattenDlg = NULL;
}


void CScanLinesEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanLinesEditDlg)
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScanLinesEditDlg, CDialog)
	//{{AFX_MSG_MAP(CScanLinesEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_BN_CLICKED(IDC_BUTTON_FLATTEN, OnButtonFlatten)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanLinesEditDlg message handlers

void CScanLinesEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CScanLinesEditDlg::OnClose() 
{
	CDialog::OnClose();
	DestroyWindow();
}

void CScanLinesEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CScanLinesEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScanLinesEditDlg::UpdateAllControls()
{
	
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_nLine+1);
	m_editLineNumber.SetWindowText(Buff);

	m_spinLineNumber.SetRange(0,PROFILE->m_nScanLineL-1);
	m_spinLineNumber.SetPos(theApp.m_nLine);

}



void CScanLinesEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	CString Buff;


	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_NUMBER:
		nTemp = theApp.m_nLine;
		theApp.m_nLine=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_nLine) {
			Buff.Format(_T("%d"),theApp.m_nLine+1);
			m_editLineNumber.SetWindowText(Buff);
			UpdateParent(INVALIDATE|FILLLIST|UPDATE_CHILDREN);
			UpdatePmacTrajectory();
			::SetFocus(m_editLineNumber.m_hWnd);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CScanLinesEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	CString	Buff;
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;

	if(pMsg->message==0x100) {
/*
		switch(pMsg->wParam) {
		case 13:
			
			hWnd = ::GetFocus();
			if(hWnd == m_editLineNumber.m_hWnd) {
				m_editLineNumber.GetWindowText(Buff);
				theApp.m_nLine = atoi(Buff) -1;
				if(theApp.m_nLine<0) theApp.m_nLine=0;
				m_spinLineNumber.SetPos(theApp.m_nLine);
				m_editLineNumber.SetWindowText(Buff);
				UpdateParent(FILLLIST|INVALIDATE);
				::SetFocus(m_editLineNumber.m_hWnd);
				return TRUE;
			}
			break;
		}
		*/
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CScanLinesEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);
	if(nAction & UPDATE_CHILDREN) pParent->UpdateAllControls();

}



void CScanLinesEditDlg::OnChangeEditLineNumber() 
{
	
	CString Buff;
	int nLine = theApp.m_nLine;

	m_editLineNumber.GetWindowText(Buff);
	nLine = _ttoi(Buff) - 1;
	if(nLine-theApp.m_nLine) {
		theApp.m_nLine = nLine;
		m_spinLineNumber.SetPos(theApp.m_nLine);
		UpdateParent(FILLLIST|INVALIDATE);
		::SetFocus(m_editLineNumber.m_hWnd);
	}


}

void CScanLinesEditDlg::UpdatePmacTrajectory()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPmacTrajectoryPage* pPage;

	if(pFrame->m_pPmacSheet != NULL) {
		pPage = (CPmacTrajectoryPage*)pFrame->m_pPmacSheet->GetActivePage();
		pPage->RefreshFromWayPoints();
	}

}

void CScanLinesEditDlg::OnButtonFlatten() 
{
	if (m_pFlattenDlg == NULL) {
		m_pFlattenDlg = new CFlattenDlg(this, (CWnd**)&m_pFlattenDlg);
		m_pFlattenDlg->Create(CFlattenDlg::IDD,this);
	} else {
		m_pFlattenDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}		
	
}
