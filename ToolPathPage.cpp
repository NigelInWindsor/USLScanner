// ToolPathPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ToolPathPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolPathPage property page

IMPLEMENT_DYNCREATE(CToolPathPage, CResizablePage)

CToolPathPage::CToolPathPage() : CResizablePage(CToolPathPage::IDD)
{
	//{{AFX_DATA_INIT(CToolPathPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pArray = NULL;
	m_nSample = 0;
}

CToolPathPage::~CToolPathPage()
{
	SAFE_DELETE( m_pArray );
}

void CToolPathPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolPathPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolPathPage, CResizablePage)
	//{{AFX_MSG_MAP(CToolPathPage)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolPathPage message handlers

BOOL CToolPathPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	AddAnchor(IDC_STATIC_VIEW, CSize(0,0), CSize(100,100));
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolPathPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		Invalidate(FALSE);
	}
	
}

void CToolPathPage::FetchToolPath()
{
	

	m_Line.Zero();
	m_Line = &PROFILE->m_ToolPath;
	if(m_hWnd)
		Invalidate(FALSE);
}

void CToolPathPage::OnPaint() 
{
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	
	CRect		rr,rect;
	CDC			dcMem;
	CBitmap		bmp;
	static	int	nWidth;
	static	int	nHeight;
	static int nLoop=0;
	CString Buff;
	
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	if((m_pArray==NULL) || (rr.Width()-nWidth) || (rr.Height()-nHeight)) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		nWidth=rr.Width();
		nHeight=rr.Height();
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width()-2;
	rect.bottom = rr.Height()-2;

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);


	DrawPlots((CPaintDC*)&dcMem,rect);


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CToolPathPage::DrawPlots(CDC *pDC, CRect rr)
{
	
	CFont HorizontalFont;
	CFont VerticalFont;
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen(PS_SOLID,1,RGB(0,255,0));
	int	xx,nX,nY;

	m_rrGrat = rr;

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	CString Buff = L"999.9";
	CSize size = pDC->GetTextExtent(Buff);

	m_rrGrat.DeflateRect(size.cx,size.cy*2);
	float fHalfHeight = (float)(m_rrGrat.Height() / 2);

	CPen* pOldPen = pDC->SelectObject(&penGrey);

	pDC->MoveTo(m_rrGrat.TopLeft());
	pDC->LineTo(m_rrGrat.right,m_rrGrat.top);
	pDC->LineTo(m_rrGrat.BottomRight());
	pDC->LineTo(m_rrGrat.left,m_rrGrat.bottom);
	pDC->LineTo(m_rrGrat.TopLeft());

	float fMaxAngle = 500.0f;

	//Xt Angle
	for(float fAngle = 0.0; fAngle <= fMaxAngle; fAngle += 45.0f) {
		nY = (int)((fAngle*fHalfHeight) / fMaxAngle);
		Buff.Format(_T("%.0f"),fAngle);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.left - size.cx,m_rrGrat.CenterPoint().y-size.cy/2-nY,Buff);

		Buff.Format(_T("%.0f"),-fAngle);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.left - size.cx,m_rrGrat.CenterPoint().y-size.cy/2+nY,Buff);
	}		

	//YtAngle;
	fMaxAngle=90.0f;
	for(float fAngle = 0.0; fAngle <= fMaxAngle; fAngle += 10.0f) {
		nY = (int)((fAngle*fHalfHeight) / fMaxAngle);
		Buff.Format(_T("%.0f"),fAngle);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.right,m_rrGrat.CenterPoint().y-size.cy/2-nY,Buff);

		Buff.Format(_T("%.0f"),-fAngle);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.right,m_rrGrat.CenterPoint().y-size.cy/2+nY,Buff);
	}		

	if(m_Line.m_nCoordL > 0) {

		fMaxAngle = 500.0f;
		pDC->SelectObject(&penRed);
		for(xx=0;xx<m_Line.m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_Line.m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_Line.m_pCp[xx].Side0.fXt*fHalfHeight) / fMaxAngle);
			if(xx==0) {
				pDC->MoveTo(nX,nY);
			} else {
				pDC->LineTo(nX,nY);
			}
		}
		for(xx=0;xx<m_Line.m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_Line.m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_Line.m_pCp[xx].Side0.fXt*fHalfHeight) / fMaxAngle);
			Cross(pDC,nX,nY,6);
		}

		fMaxAngle = 90.0f;
		pDC->SelectObject(&penGreen);
		for(xx=0;xx<m_Line.m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_Line.m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_Line.m_pCp[xx].Side0.fYt*fHalfHeight) / fMaxAngle);
			if(xx==0) {
				pDC->MoveTo(nX,nY);
			} else {
				pDC->LineTo(nX,nY);
			}
		}
		for(xx=0;xx<m_Line.m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_Line.m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_Line.m_pCp[xx].Side0.fYt*fHalfHeight) / fMaxAngle);
			Cross(pDC,nX,nY,6);
		}

		Buff.Format(_T("%.0fmm"),m_Line.m_pCp[0].Side0.fX);
		pDC->TextOut(m_rrGrat.left,m_rrGrat.bottom+2,Buff);

		Buff.Format(_T("%.0fmm"),m_Line.m_pCp[m_Line.m_nCoordL-1].Side0.fX);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.right-size.cx,m_rrGrat.bottom+2,Buff);

		Buff.Format(_T("Coord = %d   i = %.03f  j= %.03f"),m_nSample,m_Line.m_pCp[m_nSample].Side0.fI,m_Line.m_pCp[m_nSample].Side0.fI);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.CenterPoint().x-size.cx/2,m_rrGrat.bottom+2,Buff);

		Buff.Format(_T("Coord = %d   Xt = %.01f  Yt= %.01f"),m_nSample,m_Line.m_pCp[m_nSample].Side0.fXt,m_Line.m_pCp[m_nSample].Side0.fYt);
		pDC->TextOut(m_rrGrat.CenterPoint().x-size.cx/2,m_rrGrat.bottom+2+size.cy,Buff);
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}

void CToolPathPage::Cross(CDC *pDC, int nX, int nY,int nSize)
{
	pDC->MoveTo(nX-nSize,nY-nSize);
	pDC->LineTo(nX+nSize+1,nY+nSize+1);
	pDC->MoveTo(nX-nSize,nY+nSize);
	pDC->LineTo(nX+nSize+1,nY-nSize-1);
}

void CToolPathPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	m_bLButtonDown = true;
	
	CResizablePage::OnLButtonDown(nFlags, point);
}

void CToolPathPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(	m_bLButtonDown == true) {
		CRect rr;
		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);

		point.x -= rr.left;
		if(m_rrGrat.PtInRect(point)) {
			if(m_Line.m_nCoordL>0) {
				m_nSample = MulDiv((point.x - m_rrGrat.left) , m_Line.m_nCoordL-1 , m_rrGrat.Width());
				Invalidate(FALSE);
			}
		}
	}

	CResizablePage::OnMouseMove(nFlags, point);
}

void CToolPathPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = false;
	
	CResizablePage::OnLButtonUp(nFlags, point);
}
