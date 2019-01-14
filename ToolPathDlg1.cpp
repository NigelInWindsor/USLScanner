// ToolPathDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ToolPathDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolPathDlg dialog


CToolPathDlg::CToolPathDlg(CWnd* pParent /*=NULL*/, CWnd** pDlg)
	: CDialog(CToolPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolPathDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pArray = NULL;
	m_nSample = 0;
	m_pDlg = pDlg;
}



BEGIN_MESSAGE_MAP(CToolPathDlg, CDialog)
	//{{AFX_MSG_MAP(CToolPathDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_MIN_MAX, OnButtonMinMax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolPathDlg message handlers

void CToolPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolPathDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP

}


void CToolPathDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CToolPathDlg::OnClose() 
{

	CDialog::OnClose();
	DestroyWindow();
}

void CToolPathDlg::OnDestroy() 
{
	
	SAFE_DELETE( m_pArray );
	
	//Store Window Position
	if (IDD_TOOLPATH_DIALOG< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_TOOLPATH_DIALOG]);

	*m_pDlg = NULL;	
	
	CDialog::OnDestroy();
}

int CToolPathDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_TOOLPATH_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL CToolPathDlg::OnInitDialog() 
{
		
	
	CDialog::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	if (IDD_TOOLPATH_DIALOG < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_TOOLPATH_DIALOG];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosView.SetNewPos();

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolPathDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = true;
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CToolPathDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = false;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CToolPathDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(	m_bLButtonDown == true) {
		CRect rr;
		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);

		point.x -= rr.left;
		if(m_rrGrat.PtInRect(point)) {
			if(m_LocalToolPath[CURRENT].m_nCoordL>0) {
				m_nSample = MulDiv((point.x - m_rrGrat.left) , m_LocalToolPath[CURRENT].m_nCoordL-1 , m_rrGrat.Width());
				Invalidate(FALSE);
			}
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CToolPathDlg::OnPaint() 
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
	DrawMinMax((CPaintDC*)&dcMem,rect);


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
}

void CToolPathDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
		//Store Pos and Size of the Dialog
	CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_TOOLPATH_DIALOG];
	if (IDD_TOOLPATH_DIALOG < MAXIMUMWINDOWSIDNUMBER && GetSafeHwnd()) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_TOOLPATH_DIALOG]);
	}

	m_StaticPosView.SetNewPos(FALSE);
	Invalidate(FALSE);
	
}

void CToolPathDlg::DrawPlots(CDC *pDC, CRect rr)
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

	if(m_LocalToolPath[CURRENT].m_nCoordL > 0) {

		fMaxAngle = 500.0f;
		pDC->SelectObject(&penRed);
		for(xx=0;xx<m_LocalToolPath[CURRENT].m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[CURRENT].m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[CURRENT].m_pCp[xx].Side0.fXt*fHalfHeight) / fMaxAngle);
			if(xx==0) {
				pDC->MoveTo(nX,nY);
			} else {
				pDC->LineTo(nX,nY);
			}
		}
		for(xx=0;xx<m_LocalToolPath[CURRENT].m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[CURRENT].m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[CURRENT].m_pCp[xx].Side0.fXt*fHalfHeight) / fMaxAngle);
			Cross(pDC,nX,nY,6);
		}

		fMaxAngle = 90.0f;
		pDC->SelectObject(&penGreen);
		for(xx=0;xx<m_LocalToolPath[CURRENT].m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[CURRENT].m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[CURRENT].m_pCp[xx].Side0.fYt*fHalfHeight) / fMaxAngle);
			if(xx==0) {
				pDC->MoveTo(nX,nY);
			} else {
				pDC->LineTo(nX,nY);
			}
		}
		for(xx=0;xx<m_LocalToolPath[CURRENT].m_nCoordL;xx++) {
			nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[CURRENT].m_nCoordL-1);
			nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[CURRENT].m_pCp[xx].Side0.fYt*fHalfHeight) / fMaxAngle);
			Cross(pDC,nX,nY,6);
		}

		Buff.Format(_T("%.0fmm"),m_LocalToolPath[CURRENT].m_pCp[0].Side0.fX);
		pDC->TextOut(m_rrGrat.left,m_rrGrat.bottom+2,Buff);

		Buff.Format(_T("%.0fmm"),m_LocalToolPath[CURRENT].m_pCp[m_LocalToolPath[CURRENT].m_nCoordL-1].Side0.fX);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.right-size.cx,m_rrGrat.bottom+2,Buff);

		Buff.Format(_T("Coord = %d   i = %.03f  j= %.03f"),m_nSample,m_LocalToolPath[CURRENT].m_pCp[m_nSample].Side0.fI,m_LocalToolPath[CURRENT].m_pCp[m_nSample].Side0.fI);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.CenterPoint().x-size.cx/2,m_rrGrat.bottom+2,Buff);

		Buff.Format(_T("Coord = %d   Xt = %.01f  Yt= %.01f"),m_nSample,m_LocalToolPath[CURRENT].m_pCp[m_nSample].Side0.fXt,m_LocalToolPath[CURRENT].m_pCp[m_nSample].Side0.fYt);
		pDC->TextOut(m_rrGrat.CenterPoint().x-size.cx/2,m_rrGrat.bottom+2+size.cy,Buff);
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}


void CToolPathDlg::DrawMinMax(CDC *pDC, CRect rr)
{
	
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen(PS_SOLID,1,RGB(0,255,0));
	int	xx,nX,nY;

	CPen* pOldPen = pDC->SelectObject(&penGrey);

	float fMaxAngle = 500.0f;
	float fHalfHeight = (float)(m_rrGrat.Height() / 2);

	for(int nType = 1; nType<3; nType++) {

		if(m_LocalToolPath[nType].m_nCoordL > 0) {

			fMaxAngle = 500.0f;
			pDC->SelectObject(&penRed);
			for(xx=0;xx<m_LocalToolPath[nType].m_nCoordL;xx++) {
				nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[nType].m_nCoordL-1);
				nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[nType].m_pCp[xx].Side0.fXt*fHalfHeight) / fMaxAngle);
				if(xx==0) {
					pDC->MoveTo(nX,nY);
				} else {
					pDC->LineTo(nX,nY);
				}
			}
			for(xx=0;xx<m_LocalToolPath[nType].m_nCoordL;xx++) {
				nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[nType].m_nCoordL-1);
				nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[nType].m_pCp[xx].Side0.fXt*fHalfHeight) / fMaxAngle);
				Cross(pDC,nX,nY,3);
			}
		}
	}

	for(int nType = 3; nType<5; nType++) {

		if(m_LocalToolPath[nType].m_nCoordL > 0) {
			fMaxAngle = 90.0f;
			pDC->SelectObject(&penGreen);
			for(xx=0;xx<m_LocalToolPath[nType].m_nCoordL;xx++) {
				nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[nType].m_nCoordL-1);
				nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[nType].m_pCp[xx].Side0.fYt*fHalfHeight) / fMaxAngle);
				if(xx==0) {
					pDC->MoveTo(nX,nY);
				} else {
					pDC->LineTo(nX,nY);
				}
			}
			for(xx=0;xx<m_LocalToolPath[nType].m_nCoordL;xx++) {
				nX = m_rrGrat.left + MulDiv(xx,m_rrGrat.Width(),m_LocalToolPath[nType].m_nCoordL-1);
				nY = m_rrGrat.CenterPoint().y - (int)((m_LocalToolPath[nType].m_pCp[xx].Side0.fYt*fHalfHeight) / fMaxAngle);
				Cross(pDC,nX,nY,3);
			}
		}
	}


	pDC->SelectObject(pOldPen);

}

void CToolPathDlg::Cross(CDC *pDC, int nX, int nY,int nSize)
{
	pDC->MoveTo(nX-nSize,nY-nSize);
	pDC->LineTo(nX+nSize+1,nY+nSize+1);
	pDC->MoveTo(nX-nSize,nY+nSize);
	pDC->LineTo(nX+nSize+1,nY-nSize-1);
}


void CToolPathDlg::CalculateFromScanLine(int nScanLine)
{
	

	m_LocalToolPath[CURRENT].Zero();
	PROFILE->CalculateToolPath(nScanLine,m_LocalToolPath[CURRENT]);

	if(m_hWnd)
		Invalidate(FALSE);

}



void CToolPathDlg::OnButtonMinMax() 
{
	
	float fXt[2],fYt[2];
	int nn,nLine;
	int nXtMin, nXtMax, nYtMin, nYtMax;

	nXtMin = nXtMax = nYtMin = nYtMax = 0;

	if(m_LocalToolPath[MIN_XT].m_nCoordL == 0) {
		fXt[0]=fYt[0]=10000.0f;
		fXt[1]=fYt[1]=-10000.0f;

		for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {

			m_LocalToolPath[CURRENT].Zero();
			PROFILE->CalculateToolPath(nLine,m_LocalToolPath[CURRENT]);

			for(nn=0;nn<m_LocalToolPath[CURRENT].m_nCoordL;nn++) {
				if(fXt[0] > m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fXt) {
					fXt[0] = m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fXt;
					nXtMin = nLine;
				}
				if(fXt[1] < m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fXt) {
					fXt[1] = m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fXt;
					nXtMax = nLine;
				}
				if(fYt[0] > m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fYt) {
					fYt[0] = m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fYt;
					nYtMin = nLine;
				}
				if(fYt[1] < m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fYt) {
					fYt[1] = m_LocalToolPath[CURRENT].m_pCp[nn].Side0.fYt;
					nYtMax = nLine;
				}

			}

		}
		m_LocalToolPath[MIN_XT].Zero();
		PROFILE->CalculateToolPath(nXtMin,m_LocalToolPath[MIN_XT]);
		m_LocalToolPath[MAX_XT].Zero();
		PROFILE->CalculateToolPath(nXtMax,m_LocalToolPath[MAX_XT]);
		m_LocalToolPath[MIN_YT].Zero();
		PROFILE->CalculateToolPath(nYtMin,m_LocalToolPath[MIN_YT]);
		m_LocalToolPath[MAX_YT].Zero();
		PROFILE->CalculateToolPath(nYtMax,m_LocalToolPath[MAX_YT]);
		
		m_LocalToolPath[CURRENT].Zero();
	} else {
		m_LocalToolPath[MIN_XT].Zero();
		m_LocalToolPath[MAX_XT].Zero();
		m_LocalToolPath[MIN_YT].Zero();
		m_LocalToolPath[MAX_YT].Zero();
	}

	Invalidate(FALSE);

	
}
