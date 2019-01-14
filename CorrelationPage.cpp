// CorrelationPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CorrelationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCorrelationPage property page

IMPLEMENT_DYNCREATE(CCorrelationPage, CPropertyPage)

CCorrelationPage::CCorrelationPage(CWnd* pParent) : CPropertyPage(CCorrelationPage::IDD)
{
	//{{AFX_DATA_INIT(CCorrelationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	
}

CCorrelationPage::~CCorrelationPage()
{
}

void CCorrelationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCorrelationPage)
	DDX_Control(pDX, IDC_STATIC_SCOPE, m_staticScope);
	DDX_Control(pDX, IDC_SPIN_PRE_TRIGGER, m_spinPreTrigger);
	DDX_Control(pDX, IDC_SPIN_POST_TRIGGER, m_spinPostTrigger);
	DDX_Control(pDX, IDC_EDIT_PRE_TRIGGER, m_editPreTrigger);
	DDX_Control(pDX, IDC_EDIT_POST_TRIGGER, m_editPostTrigger);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCorrelationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCorrelationPage)
	ON_EN_CHANGE(IDC_EDIT_POST_TRIGGER, OnChangeEditPostTrigger)
	ON_EN_CHANGE(IDC_EDIT_PRE_TRIGGER, OnChangeEditPreTrigger)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCorrelationPage message handlers

BOOL CCorrelationPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UDACCEL	Accel;
	Accel.nInc=10;
	Accel.nSec=1;

	m_spinPostTrigger.SetRange(0,32000);
	m_spinPreTrigger.SetRange(0,32000);
	m_spinPostTrigger.SetAccel(1,&Accel);
	m_spinPreTrigger.SetAccel(1,&Accel);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCorrelationPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%.03fus"),(float)theApp.m_LastSettings.nCorrelationPostTrigger/1000.0f);
	m_editPostTrigger.SetWindowText(Buff);
	Buff.Format(_T("%.03fus"),(float)theApp.m_LastSettings.nCorrelationPreTrigger/1000.0f);
	m_editPreTrigger.SetWindowText(Buff);

	m_spinPostTrigger.SetPos(theApp.m_LastSettings.nCorrelationPostTrigger);
	m_spinPreTrigger.SetPos(theApp.m_LastSettings.nCorrelationPreTrigger);
}

void CCorrelationPage::OnChangeEditPostTrigger() 
{
	CString Buff;
	float fTemp;
	int nTemp;

	Buff.Format(_T(".03fus"),(float)theApp.m_LastSettings.nCorrelationPostTrigger/1000.0f);
	m_editPostTrigger.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	nTemp = (int)(fTemp * 1000.0f);
	if(theApp.m_LastSettings.nCorrelationPostTrigger - nTemp) {
		theApp.m_LastSettings.nCorrelationPostTrigger = nTemp;
		m_spinPostTrigger.SetPos(theApp.m_LastSettings.nCorrelationPostTrigger);
	}

	
}

void CCorrelationPage::OnChangeEditPreTrigger() 
{
	CString Buff;
	float fTemp;
	int nTemp;

	Buff.Format(_T(".03fus"),(float)theApp.m_LastSettings.nCorrelationPreTrigger/1000.0f);
	m_editPreTrigger.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	nTemp = (int)(fTemp * 1000.0f);
	if(theApp.m_LastSettings.nCorrelationPreTrigger - nTemp) {
		theApp.m_LastSettings.nCorrelationPreTrigger = nTemp;
		m_spinPreTrigger.SetPos(theApp.m_LastSettings.nCorrelationPreTrigger);
	}
	
}

void CCorrelationPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CString	Buff;

	int nTemp=(pSpin->GetPos()&0xffff);

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_PRE_TRIGGER:
		if(theApp.m_LastSettings.nCorrelationPreTrigger - nTemp ) {
			theApp.m_LastSettings.nCorrelationPreTrigger=nTemp;
			Buff.Format(_T("%.03fus"),(float)theApp.m_LastSettings.nCorrelationPreTrigger/1000.0f);
			m_editPreTrigger.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_POST_TRIGGER:
		if(theApp.m_LastSettings.nCorrelationPostTrigger - nTemp ) {
			theApp.m_LastSettings.nCorrelationPostTrigger=nTemp;
			Buff.Format(_T("%.03fus"),(float)theApp.m_LastSettings.nCorrelationPostTrigger/1000.0f);
			m_editPostTrigger.SetWindowText(Buff);
		}
		break;
	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCorrelationPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastVolumeToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CCorrelationPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	RenderScope(&dc);
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CCorrelationPage::RenderScope(CPaintDC *pDC)
{
//	CDC* pDC = GetDC();
	CRect rr;
	CBrush Brush(RGB(255,255,255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penGrey(PS_SOLID,1,RGB(50,50,50));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penDotGrey(PS_DOT,1,RGB(180,180,180));
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	CFont FontHorizontal;
	CFont FontVertical;
	CFont* pOldFont;
	theApp.SetFontLanguage(&lf);
	lf.lfHeight = 12;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	FontVertical.CreateFontIndirect(&lf);


	Buff = "HelloAll";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);



	m_staticScope.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top+3 + size.cy;
	rr.left-=size.cx;
	rr.right+=size.cx;
	pDC->FillRect(rr,&brushBK);

	m_staticScope.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	pDC->FillRect(&rr,&Brush);
/*
	//Find Min and Max
	double dMax = -1.0;
	double dMin = 1.0;
	double dStep = 0.01;
	int nPrecision = 1;

	if(a != NULL) {
		for(int ii=0;ii<numTaps;ii++) {
			if(a[ii]>dMax) dMax = a[ii];
			if(a[ii]<dMin) dMin = a[ii];
		}


		if((dMax - dMin) < 0.3) {
			dMax = (double)((int)(dMax*100.0) + 1) / 100.0;
			dMin = (double)((int)(dMin*100.0) - 1) / 100.0;
			dStep = 0.01;
			nPrecision = 2;
		} else {
			dMax = (double)((int)(dMax*10.0) + 1) / 10.0;
			dMin = (double)((int)(dMin*10.0) - 1) / 10.0;
			dStep = 0.1;
			nPrecision = 1;
		}
		if(dMax<=dMin) dMax = dMin + 0.1;

		//Draw graticule & text
		rr.DeflateRect(size.cx,2 * size.cy);
		rr.right += (size.cx*2/3);

		pDC->SelectObject(&penDotGrey);
		for(dY = dMin;dY<=(dMax+0.001);dY+=dStep) {
			nPnY = rr.bottom - (int)((dY-dMin) * (double)rr.Height() / (dMax - dMin));
			pDC->MoveTo(rr.left+1,nPnY);
			pDC->LineTo(rr.right+1,nPnY);
			Buff.Format(_T("%.*f"),nPrecision,dY);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(rr.left - size.cx-5,nPnY-size.cy/2,Buff);
		}

		double dMaxTime = 1.0 / (m_fMaxFrequency * 2.0) * (double)theApp.Fir.nNumTaps;
		dStep = 0.2;
		if(dMaxTime <= 1.0) dStep = 0.1;
		for(dX = 0.0;dX <= dMaxTime; dX += dStep) {
			nPnX = (int)(dX * (double)rr.Width() / dMaxTime) + rr.left;
			pDC->MoveTo(nPnX,rr.bottom-1);
			pDC->LineTo(nPnX,rr.top);
			Buff.Format(_T("%.01f"),dX);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnX-size.cx/2,rr.bottom+2,Buff);
		}

		pDC->SelectObject(&penBlack);
		pDC->MoveTo(rr.TopLeft());
		pDC->LineTo(rr.left,rr.bottom);
		pDC->LineTo(rr.BottomRight());

		Buff.LoadString(IDS_Impulse_Responce);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.top-(size.cy*3/2),Buff);

		Buff.LoadString(IDS_Time_in_us);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom+(size.cy*3/2),Buff);

		pDC->SelectObject(&FontVertical);
		Buff.LoadString(IDS_Inphase_Amplitude);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.left-size.cy*3-4,rr.CenterPoint().y+(size.cx/2),Buff);



		//Draw Trace
		rr.DeflateRect(1,1);
		pDC->SelectObject(&penRed);
		ii=0;
		nPnY = rr.bottom - (int)((a[ii++]-dMin) * (double)rr.Height() / (dMax-dMin));
		pDC->MoveTo(rr.left,nPnY);
		for(ii;ii<numTaps;ii++) {
			nPnX = MulDiv(ii,rr.Width()-1,numTaps-1) + rr.left;
			nPnY = rr.bottom - (int)((a[ii]-dMin) * (double)rr.Height() / (dMax-dMin));
			pDC->LineTo(nPnX,nPnY);
		}

	}
	*/
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

//	ReleaseDC(pDC);
}
