// EnvelopeEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EnvelopeEditDlg.h"
#include "3DViewEditDlg.h"
#include "EnvelopeToolsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeEditDlg dialog


CEnvelopeEditDlg::CEnvelopeEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop)
	: CDialog(CEnvelopeEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnvelopeEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pEnvelopeRectangularDlg = NULL;
}


void CEnvelopeEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvelopeEditDlg)
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_EDIT_LABEL, m_editLabel);
	DDX_Control(pDX, IDC_SPIN_LINE, m_spinLine);
	DDX_Control(pDX, IDC_EDIT_LINE, m_editLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvelopeEditDlg, CDialog)
	//{{AFX_MSG_MAP(CEnvelopeEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RECTILINEAR, OnButtonRectilinear)
	ON_BN_CLICKED(ID_TOOLS_ENVELOPE, OnToolsEnvelope)
	ON_EN_CHANGE(IDC_EDIT_LINE, OnChangeEditLine)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_LABEL, OnChangeEditLabel)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeEditDlg message handlers

void CEnvelopeEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CEnvelopeEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CEnvelopeEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CEnvelopeEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);
	
	m_spinLine.SetRange(0,200);
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnvelopeEditDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_nLine + 1);
	m_editLine.SetWindowText(Buff);

	m_spinLine.SetPos(theApp.m_nLine);

	if(PROFILE->m_EnvelopeLine) {
		if(theApp.m_nLine < PROFILE->m_nEnvelopeLineL) {
			Buff = PROFILE->m_EnvelopeLine[theApp.m_nLine].GetString();
		}
	}
	m_editLabel.SetWindowText(Buff);

	SendMessage(WM_PAINT);
}

void CEnvelopeEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	pParent->InitializeEditLine();
	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);

}



void CEnvelopeEditDlg::OnButtonRectilinear() 
{
	if (m_pEnvelopeRectangularDlg == NULL) {
		m_pEnvelopeRectangularDlg = new CEnvelopeRectangularDlg(this, (CWnd**)&m_pEnvelopeRectangularDlg);
		m_pEnvelopeRectangularDlg->Create(CEnvelopeRectangularDlg::IDD,this);
	} else {
		m_pEnvelopeRectangularDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}

void CEnvelopeEditDlg::OnToolsEnvelope() 
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


//	pParent->OnToolsEnvelope();
	CEnvelopeToolsDlg dlg(pParent);

	if(dlg.DoModal()==IDOK) {
		UpdateParent(FILLLIST|PAINT|INVALIDATE);
	}

	
}

void CEnvelopeEditDlg::OnChangeEditLine() 
{
	CString Buff;
	int nTemp = theApp.m_nLine;
	
	m_editLine.GetWindowText(Buff);
	theApp.m_nLine = _ttoi(Buff)-1;

	if(theApp.m_nLine >= PROFILE->m_nEnvelopeLineL) theApp.m_nLine = PROFILE->m_nEnvelopeLineL-1; 
	if(theApp.m_nLine<0) theApp.m_nLine = 0;

	if(nTemp - theApp.m_nLine) {
		UpdateAllControls();
		UpdateParent(FILLLIST);
	}
}

void CEnvelopeEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	CString Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE:
		nTemp = theApp.m_nLine;
		theApp.m_nLine=pSpin->GetPos()&0xffff;
		if(theApp.m_nLine<0) theApp.m_nLine = 0;
		if(theApp.m_nLine>=PROFILE->m_nEnvelopeLineL) theApp.m_nLine=PROFILE->m_nEnvelopeLineL - 1;
		if(nTemp - theApp.m_nLine) {

			if(PROFILE->m_EnvelopeLine) {
				if(theApp.m_nLine < PROFILE->m_nEnvelopeLineL) {
					Buff = PROFILE->m_EnvelopeLine[theApp.m_nLine].GetString();
				}
			}
			m_editLabel.SetWindowText(Buff);

			Buff.Format(_T("%d"),theApp.m_nLine+1);
			m_editLine.SetWindowText(Buff);
			UpdateParent(INVALIDATE|FILLLIST);

			Invalidate(false);
			::SetFocus(m_editLine.m_hWnd);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEnvelopeEditDlg::OnChangeEditLabel() 
{
	CString Buff;

	m_editLabel.GetWindowText(Buff);

	if(PROFILE->m_EnvelopeLine) {
		if(theApp.m_nLine < PROFILE->m_nEnvelopeLineL) {
			PROFILE->m_EnvelopeLine[theApp.m_nLine].SetString(Buff);
		}
	}
}

void CEnvelopeEditDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	if(PROFILE->m_nEnvelopeLineL<=0) {
		MessageBox(L"Current envelope has no coordinates\nColour can not be assigned",L"Error",MB_ICONERROR);
	} else {

		m_staticColor.GetWindowRect(&rr);
		ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			if(dlg.DoModal()==IDOK) {
				PROFILE->m_EnvelopeLine[theApp.m_nLine].m_rgb=dlg.GetColor();
				Invalidate(FALSE);
				UpdateParent(INVALIDATE);
			}
		}

	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CEnvelopeEditDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(PROFILE->m_nEnvelopeLineL>0) {
		BlockFill(&dc,&m_staticColor,PROFILE->m_EnvelopeLine[theApp.m_nLine].m_rgb);
	};
	// Do not call CDialog::OnPaint() for painting messages
}

void CEnvelopeEditDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

BOOL CEnvelopeEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:

			return TRUE;
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
