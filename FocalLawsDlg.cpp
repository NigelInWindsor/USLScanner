// FocalLawsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FocalLawsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFocalLawsDlg dialog


CFocalLawsDlg::CFocalLawsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CFocalLawsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFocalLawsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDlg = pDlg;
	m_bDlgCreated = false;
	m_fFocalLength = 40.0f;
}


void CFocalLawsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFocalLawsDlg)
	DDX_Control(pDX, IDC_SPIN_PITCH, m_spinPitch);
	DDX_Control(pDX, IDC_SPIN_NUMBER_ELEMENTS, m_spinNumberElements);
	DDX_Control(pDX, IDC_EDIT_PITCH, m_editPitch);
	DDX_Control(pDX, IDC_EDIT_NUMBER_ELEMENTS, m_editNumberElements);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFocalLawsDlg, CDialog)
	//{{AFX_MSG_MAP(CFocalLawsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_PITCH, OnChangeEditPitch)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_ELEMENTS, OnChangeEditNumberElements)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFocalLawsDlg message handlers

void CFocalLawsDlg::PostNcDestroy() 
{
	
	CDialog::PostNcDestroy();
	delete this;
}

void CFocalLawsDlg::OnClose()
{
	APP App= (APP) AfxGetApp();
	App->m_LastSettings.bWindowOpenOnLastExit[CFocalLawsDlg::IDD]=FALSE;

	CDialog::OnClose();
	DestroyWindow();
}

void CFocalLawsDlg::OnDestroy()
{
	APP App= (APP) AfxGetApp();

	if (CFocalLawsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(App->m_LastSettings.RectWindowPos[CFocalLawsDlg::IDD]);
	}

	CDialog::OnDestroy();

	*m_pDlg = NULL;
}

BOOL CFocalLawsDlg::OnInitDialog()
{
	APP App= (APP) AfxGetApp();

	CDialog::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	CRect rect = App->m_LastSettings.RectWindowPos[CFocalLawsDlg::IDD];
	if (App->m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		if(rect.Width()>1024) rect.right=rect.left+1024;
		if(rect.Height()>512) rect.bottom=rect.top+512;
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	};

	m_StaticPosView.SetNewPos();

	m_bDlgCreated = true;
	UpdateAllControls();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFocalLawsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();

	if(m_bDlgCreated == true) Invalidate(FALSE);

}

void CFocalLawsDlg::UpdateAllControls()
{
	if(m_bDlgCreated != true) return;
	APP App= (APP) AfxGetApp();
	CString Buff;

	Buff.Format(_T("%.01fmm"),App->m_UtUser.m_Global.LSA.fFocalLawPitchMM);
	m_editPitch.SetWindowText(Buff);
	m_spinPitch.SetRange(0,100);
	m_spinPitch.SetPos((int)(App->m_UtUser.m_Global.LSA.fFocalLawPitchMM * 10.0f));

	m_spinNumberElements.SetRange(1,16);
	m_spinNumberElements.SetPos(App->m_UtUser.m_Global.LSA.nElements);
	Buff.Format(_T("%d"),App->m_UtUser.m_Global.LSA.nElements);
	m_editNumberElements.SetWindowText(Buff);

}

void CFocalLawsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_NUMBER_ELEMENTS:
		App->m_UtUser.m_Global.LSA.nElements=(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%d"),App->m_UtUser.m_Global.LSA.nElements);
		m_editNumberElements.SetWindowText(Buff);
		break;
	case IDC_SPIN_PITCH:
		fTemp = App->m_UtUser.m_Global.LSA.fFocalLawPitchMM;
		App->m_UtUser.m_Global.LSA.fFocalLawPitchMM = (float)(pSpin->GetPos()&0xffff) / 10.0f;
		if(fTemp - App->m_UtUser.m_Global.LSA.fFocalLawPitchMM) {
			Buff.Format(_T("%.01fmm"),App->m_UtUser.m_Global.LSA.fFocalLawPitchMM);
			m_editPitch.SetWindowText(Buff);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFocalLawsDlg::OnChangeEditPitch() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	float fTemp = App->m_UtUser.m_Global.LSA.fFocalLawPitchMM;

	m_editPitch.GetWindowText(Buff);
	_WTOF(Buff,App->m_UtUser.m_Global.LSA.fFocalLawPitchMM);
	if(fTemp - App->m_UtUser.m_Global.LSA.fFocalLawPitchMM) {
		m_spinPitch.SetPos((int)(App->m_UtUser.m_Global.LSA.fFocalLawPitchMM * 10.0f));
	}
}

void CFocalLawsDlg::OnChangeEditNumberElements() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editNumberElements.GetWindowText(Buff);
	App->m_UtUser.m_Global.LSA.nElements = _ttoi(Buff);
	
	if(App->m_UtUser.m_Global.LSA.nElements<1){
		App->m_UtUser.m_Global.LSA.nElements=1;
		m_editNumberElements.SetWindowText(_T("1"));
	}else if(App->m_UtUser.m_Global.LSA.nElements>16){
		App->m_UtUser.m_Global.LSA.nElements=16;
		m_editNumberElements.SetWindowText(_T("16"));
	}

	m_spinNumberElements.SetPos(App->m_UtUser.m_Global.LSA.nElements);
	
	Invalidate(true);
}

void CFocalLawsDlg::OnPaint() 
{
	APP App = (APP) AfxGetApp();
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CBitmap bmp;
	CRect rr;
	CFont Font;
	
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	CRect rrView = rr;
	rrView.OffsetRect(-rrView.left,-rrView.top);
	rrView.left += 20;
	rrView.right -= 20;
	rrView.top += 30;
	rrView.bottom -= 20;

	COLORREF* pArray = new COLORREF[rr.Width() * rr.Height()];
	FillMemory(pArray,rr.Width() * rr.Height() * 4, 0xff);

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pArray);

	dcMem.CreateCompatibleDC(&dc);

	App->m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&App->m_LastSettings.lfViewGraticule);
	dcMem.SetBkMode(TRANSPARENT);

	CFont *pOldFont = dcMem.SelectObject(&Font);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	RenderElements(&dcMem,&rrView);
	RenderFocalPlane(&dcMem,&rrView);
	RenderFoalPoints(&dcMem,&rrView);

	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


	delete pArray;
}


void CFocalLawsDlg::RenderElements(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();
	CString Buff;


	pDC->MoveTo(rr.left,rr.top);
	pDC->LineTo(rr.right,rr.top);


	int nMaxWidth = App->m_UtUser.m_Global.LSA.nElements * 30;
	for(int ii=0;ii<App->m_UtUser.m_Global.LSA.nElements;ii++) {

		m_el[ii].pt.x = rr.CenterPoint().x;
		if(App->m_UtUser.m_Global.LSA.nElements>1)
			m_el[ii].pt.x = MulDiv(ii,nMaxWidth,App->m_UtUser.m_Global.LSA.nElements-1) + (rr.CenterPoint().x - (nMaxWidth / 2));
		m_el[ii].pt.y = rr.top;

		pDC->MoveTo(m_el[ii].pt.x-4,m_el[ii].pt.y-4);
		pDC->LineTo(m_el[ii].pt.x+5,m_el[ii].pt.y+5);
		pDC->MoveTo(m_el[ii].pt.x-4,m_el[ii].pt.y+4);
		pDC->LineTo(m_el[ii].pt.x+5,m_el[ii].pt.y-5);

		m_el[ii].fX = (((float)(App->m_UtUser.m_Global.LSA.nElements-1) / 2.0f) - (float)ii) * App->m_UtUser.m_Global.LSA.fFocalLawPitchMM;
		m_el[ii].fY = m_fFocalLength;
		
		m_el[ii].fH = (float)hypot(m_el[ii].fX,m_el[ii].fY);

		m_el[ii].fTime = m_el[ii].fH / 1.480f;

		Buff.Format(_T("%.03f"),m_el[ii].fTime);
		CSize size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_el[ii].pt.x-size.cx/2,m_el[ii].pt.y-16,Buff);

	}

}

void CFocalLawsDlg::RenderFocalPlane(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();

	int nPnY = (int)(m_fFocalLength * 20.0f);
	
	pDC->MoveTo(rr.left,nPnY);
	pDC->LineTo(rr.right,nPnY);

}

void CFocalLawsDlg::RenderFoalPoints(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();
	int nTime;
	float fRayLength,fY;

	for(int ii=0;ii<App->m_UtUser.m_Global.LSA.nElements;ii++) {

		nTime = (int)(m_el[ii].fTime * 1000.0f);
		nTime = (nTime / 10) * 10;

		fRayLength = ((float)nTime / 1000.0f) * 1.480f;
		fY = sqrtf((fRayLength * fRayLength) - (m_el[ii].fX * m_el[ii].fX));

		int nPnY = (int)(fY * 20.0f);
		pDC->MoveTo(rr.CenterPoint().x-4,nPnY-4);
		pDC->LineTo(rr.CenterPoint().x+5,nPnY+5);
		pDC->MoveTo(rr.CenterPoint().x-4,nPnY+4);
		pDC->LineTo(rr.CenterPoint().x+5,nPnY-5);

	}
}
