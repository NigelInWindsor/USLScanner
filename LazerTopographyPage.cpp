// LazerTopographyPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LazerTopographyPage.h"
#include "ProfileSheet.h"
#include "rangehs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CLazerTopographyPage, CPropertyPage)


CLazerTopographyPage::CLazerTopographyPage() : CPropertyPage(CLazerTopographyPage::IDD)
{
	//{{AFX_DATA_INIT(CLazerTopographyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDlgCreated = false;
	m_pEditSpinItem = NULL;
	m_pArray = NULL;
	m_nView=0;
	m_bLeftButtonDown = false;


	m_pLaserViewDlg=NULL;
	m_pLaserScanDlg=NULL;

	fTempJitterMin = 10000;
	fTempJitterMax = -1;

}

CLazerTopographyPage::~CLazerTopographyPage()
{
	if(m_pLaserViewDlg) delete m_pLaserViewDlg;
	if(m_pLaserScanDlg) delete m_pLaserScanDlg;
	SAFE_DELETE( m_pArray );
}

void CLazerTopographyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLazerTopographyPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLazerTopographyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLazerTopographyPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_COMMAND(ID_BUTTON_LASER_VIEW,LaserToolBarView)
	ON_COMMAND(ID_BUTTON_LASER_SCAN,LaserToolBarScan)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_BUTTON_3D_TOOLS, OnButton3dTools)
	ON_COMMAND(ID_BUTTON_LASER_360_VIEW, OnButtonLaser360View)
	ON_COMMAND(ID_BUTTON_LASER_WINDOW_VIEW, OnButtonLaserWindowView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LASER_ADDTAUGHTPOINT, OnLaserAddtaughtpoint)
	ON_COMMAND(ID_LASER_NEWTAUGHTLINE, OnLaserNewtaughtline)
	ON_COMMAND(ID_BUTTON_LASER_OFF_ON, OnButtonLaserOffOn)
	ON_COMMAND(ID_LASER_STYLE_LINEAR, OnLaserStyleLinear)
	ON_COMMAND(ID_LASER_STYLE_SPLINE, OnLaserStyleSpline)
	ON_COMMAND(ID_LASER_DELETE, OnLaserDelete)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLazerTopographyPage message handlers

int CLazerTopographyPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndLaserBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndLaserBar.LoadToolBar(IDR_PROFILE_LASER_TOPO_TOOLBAR)) {
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
	m_wndRebar.AddBar(&m_wndLaserBar);
	
	return 0;
}

void CLazerTopographyPage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::PostNcDestroy();
}

void CLazerTopographyPage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnClose();
}

void CLazerTopographyPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();

	CloseLazer();
	
}

BOOL CLazerTopographyPage::OnInitDialog() 
{
	APP App = (APP) AfxGetApp();
	CPropertyPage::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosView.SetNewPos();

	App->m_pLazerTopographyPage = this;

	SetToolBarCheckedState();
	m_bDlgCreated = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLazerTopographyPage::OnSetActive() 
{
	m_bDlgCreated = true;
	
	CRect rr;
	m_wndRebar.GetWindowRect(rr);

	m_pLaserViewDlg = new CLaserViewDlg(this,(CWnd**)&m_pLaserViewDlg,rr.left,rr.bottom);
	m_pLaserViewDlg->Create(CLaserViewDlg::IDD, this);

//	m_pLaserScanDlg = new CLaserScanDlg(this,(CWnd**)&m_pLaserScanDlg,rr.left,rr.bottom);
//	m_pLaserScanDlg->Create(CLaserScanDlg::IDD, this);
	SetTimer(1,500,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CLazerTopographyPage::OnKillActive() 
{
	m_bDlgCreated = false;
	
//	CloseLazer();
	CloseAllChildren();
	KillTimer(1);

	return CPropertyPage::OnKillActive();
}

void CLazerTopographyPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();
	Invalidate(FALSE);	
}



void CLazerTopographyPage::CloseLazer()
{
	APP App = (APP) AfxGetApp();
	App->m_pLazerTopographyPage = NULL;
}



void CLazerTopographyPage::OnPaint() 
{
	
	if(m_bDlgCreated != true) return;
	CPaintDC dc(this); // device context for painting
	APP App = (APP) AfxGetApp();
	CRect Rect = m_StaticPosView.m_Rect;
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


	switch(m_nView){
	default:
	case 0:
		DrawPolarPlot((CPaintDC*)&dcMem,rect);
		break;
	case 1:
		DrawWindowPlot((CPaintDC*)&dcMem,rect);
		break;
	case 2:
		DrawComponentPlot((CPaintDC*)&dcMem,rect);
		DrawMarker((CPaintDC*)&dcMem,rect);
		break;
	}//switch


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	
	CCoord Cp;
	App->m_Motors.GetHeadPos(&Cp);
	if((fabs(m_Cp.Side0.fX - Cp.Side0.fX) > 2.0f) || (fabs(m_Cp.Side0.fZ - Cp.Side0.fZ) > 2.0f)) {
		App->m_LaserUser.ResetDataSets();
		m_Cp = Cp;
	}
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CLazerTopographyPage::RenderScreen()
{
	if(m_bDlgCreated != true) return;

	CDC* pDC = GetDC();

	APP App = (APP) AfxGetApp();
	CRect Rect = m_StaticPosView.m_Rect;
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

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);


	switch(m_nView){
	default:
	case 0:
		DrawPolarPlot((CPaintDC*)&dcMem,rect);
		break;
	case 1:
		DrawWindowPlot((CPaintDC*)&dcMem,rect);
		break;
	case 2:
		DrawComponentPlot((CPaintDC*)&dcMem,rect);
		DrawMarker((CPaintDC*)&dcMem,rect);
		break;
	}//switch


	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	
	CCoord Cp;
	App->m_Motors.GetHeadPos(&Cp);
	if((fabs(m_Cp.Side0.fX - Cp.Side0.fX) > 2.0f) || (fabs(m_Cp.Side0.fZ - Cp.Side0.fZ) > 2.0f)) {
		App->m_LaserUser.ResetDataSets();
		m_Cp = Cp;
	}


	ReleaseDC(pDC);

}


void CLazerTopographyPage::DrawPolarPlot(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();
	CFont HorizontalFont;
	CFont VerticalFont;
	CPen penGrey(PS_SOLID,1,RGB(100,100,100));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CString Buff = L"999.9";
	CSize size = pDC->GetTextExtent(Buff);
	m_nRadius = rr.Height()/2;
	if(m_nRadius > rr.Width()/2) m_nRadius = rr.Width()/2;
	m_nRadius -= (size.cy * 2);
	CPoint pt = rr.CenterPoint();
	CRect rrCircle(pt.x-m_nRadius,pt.y-m_nRadius,pt.x+m_nRadius,pt.y+m_nRadius);
	float fX,fY,fLastfX,fLastfY;



	CPen* pOldPen = pDC->SelectObject(&penGrey);
	pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->Ellipse(&rrCircle);

	pDC->MoveTo(pt.x-m_nRadius-10,pt.y);
	pDC->LineTo(pt.x+m_nRadius+10,pt.y);
	pDC->MoveTo(pt.x,pt.y-m_nRadius-10);
	pDC->LineTo(pt.x,pt.y+m_nRadius+10);

	App->m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&App->m_LastSettings.lfViewGraticule);
	App->m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&App->m_LastSettings.lfViewGraticule);

	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);

	Buff = "90.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x-size.cx/2,pt.y+m_nRadius+2,Buff);

	Buff = "270.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x-size.cx/2,pt.y-m_nRadius-2-size.cy,Buff);

	pDC->SelectObject(&VerticalFont);

	Buff = "180.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x-m_nRadius-size.cx,pt.y+size.cx/2,Buff);

	Buff = "0.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x+m_nRadius+2,pt.y+size.cx/2,Buff);

	Buff.Format(_T("%d"),App->m_nLazerLoopCount);
	pDC->TextOut(pt.x,pt.y,Buff);

	pDC->SelectObject(pOldFont);

	int nX,nY;
	float fAngle;
	std::map<float,float>::iterator iter;

	fLastfX=0.0f;
	fLastfY=0.0f;
	int nStarted=0;
	for(int ii=0;ii<NSR;ii++) {
		fAngle = 2.0f * PIf * (float)ii / 4095;	
		float fRange=App->m_LaserUser.m_fRange[ii];
		
		fX = cosf(fAngle) * fRange;
		fY = sinf(fAngle) * fRange;
		nX = pt.x - (int)(fX * (float)m_nRadius / App->m_LastSettings.fMaxDistance);
		nY = pt.y + (int)(fY * (float)m_nRadius / App->m_LastSettings.fMaxDistance);

		if((nX - pt.x) && (nY - pt.y)) {
//			if(nStarted == 0) {
			if(hypot(fX-fLastfX,fY-fLastfY) < 250.0f) {
				if(nStarted==0) pDC->MoveTo(nX,nY);
				pDC->LineTo(nX,nY);
				nStarted = 1;
			} else {
				pDC->MoveTo(nX,nY);
			}
		}
		fLastfX = fX;
		fLastfY = fY;
	}


	pDC->SelectObject(pOldPen);

	CRect rrWindow;
	rrWindow.left = pt.x - (int)(App->m_LastSettings.fLaserMinY * (float)m_nRadius / App->m_LastSettings.fMaxDistance);
	rrWindow.right = pt.x - (int)(App->m_LastSettings.fLaserMaxY * (float)m_nRadius / App->m_LastSettings.fMaxDistance);
	rrWindow.bottom = pt.y + (int)(App->m_LastSettings.fLaserMinZ * (float)m_nRadius / App->m_LastSettings.fMaxDistance);
	rrWindow.top = pt.y + (int)(App->m_LastSettings.fLaserMaxZ * (float)m_nRadius / App->m_LastSettings.fMaxDistance);

	pDC->MoveTo(rrWindow.BottomRight());
	pDC->LineTo(rrWindow.right,rrWindow.top);
	pDC->LineTo(rrWindow.TopLeft());
	pDC->LineTo(rrWindow.left,rrWindow.bottom);
	pDC->LineTo(rrWindow.BottomRight());

	Buff.Format(_T("Angle %0.2f X: %.01fmm Y:%.01fmm Z:%.01fmm"),m_fMarkerAngle*RAD_TO_DEG,
		m_Cp.Side0.fX - App->m_LastSettings.vLaserTranslation.x,
		m_fX-App->m_LastSettings.vLaserTranslation.y,
		m_Cp.Side0.fZ + m_fY - App->m_LastSettings.vLaserTranslation.z);

	pDC->TextOut(0,0,Buff);

}


int m_nGrid[1000][2000];


void CLazerTopographyPage::DrawWindowPlot(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();
	CPolyCoord Line;
	CPoint *pPt = NULL;
	CCoord Cp,CpHead;
	float fY,fZ,fAngle,fRange,fStep,fPos;
	int ii,nX,nY,nW,nH,nMax,ss;
	CPoint pt;
	CString Buff;
	int nYStep = 2;
	int nZStep = 4;
	int nWidth = (int)(App->m_LastSettings.fLaserMaxY - App->m_LastSettings.fLaserMinY) / nYStep;
	int nHeight = (int)(App->m_LastSettings.fLaserMaxZ - App->m_LastSettings.fLaserMinZ) / nZStep;

	if(nWidth>=1000) nWidth = 999;
	if(nHeight>=2000) nHeight = 1999;

	ZeroMemory(m_nGrid,sizeof m_nGrid);

	//Render raw data
	for(ii=0;ii<NSR;ii++) {
		fAngle = 2.0f * PIf * (float)ii / 4095;	
		fRange=App->m_LaserUser.m_fRange[ii];
		
		fY = cosf(fAngle) * fRange;
		fZ = sinf(fAngle) * fRange;

		if((App->m_LastSettings.fLaserMinY < fY) && (fY < App->m_LastSettings.fLaserMaxY) && (App->m_LastSettings.fLaserMinZ < fZ) && (fZ < App->m_LastSettings.fLaserMaxZ)) {

			nX = (int)((float)rr.Width() * (fY - App->m_LastSettings.fLaserMinY) / (App->m_LastSettings.fLaserMaxY - App->m_LastSettings.fLaserMinY));
			nY = (int)((float)rr.Height() * (fZ - App->m_LastSettings.fLaserMinZ) / (App->m_LastSettings.fLaserMaxZ - App->m_LastSettings.fLaserMinZ));

			pt.x = rr.right - nX;
			pt.y = rr.top + nY;

			pDC->SetPixel(pt,RGB(0,0,0));

			nW = (int)(fY-App->m_LastSettings.fLaserMinY) / nYStep;
			nH = (int)(fZ-App->m_LastSettings.fLaserMinZ) / nZStep;
			if(nW<0) nW=0;
			if(nW>=nWidth) nW = nWidth-1;
			if(nH<0) nH=0;
			if(nH>=nHeight) nH = nHeight-1;
			m_nGrid[nW][nH]++;
		}
	}

	//render filtered Data
	for(nH=0;nH<nHeight;nH++) {
		nY = 0;
		nMax=0;
		for(nW=0;nW<nWidth;nW++) {

			if(m_nGrid[nW][nH]>nMax) {
				nMax = m_nGrid[nW][nH];
				nY=nW;
			}
		}

		pt.x = rr.right - MulDiv(nY,rr.Width(),nWidth);
		pt.y = rr.top + MulDiv(nH,rr.Height(),nHeight);

		if(nH==0) {
			pDC->MoveTo(pt);
		} else {
			if((pt.x < rr.right) && (pt.x > rr.left)) {
//				pDC->LineTo(pt);
				pDC->SetPixel(pt,RGB(0,0,0));
			}
		}
	}

	//render current line
	if(PROFILE->m_TaughtLine != NULL) {
		if(App->m_nLine<PROFILE->m_nTaughtLineL) {
			CPen penRed(PS_SOLID,1,RGB(255,0,0));
			CPen *pOldPen = pDC->SelectObject(&penRed);
			Line = &PROFILE->m_TaughtLine[App->m_nLine];

			App->m_Motors.GetHeadPos(&CpHead);

			pPt = new CPoint[Line.m_nCoordL];
			for(ss=0;ss<Line.m_nCoordL;ss++) {
				Line.GetCoord(ss,&Cp);
				fY = Cp.Side0.fY + App->m_LastSettings.vLaserTranslation.y;
				fZ = Cp.Side0.fZ + App->m_LastSettings.vLaserTranslation.z - CpHead.Side0.fZ;
				nX = (int)((float)rr.Width() * (fY - App->m_LastSettings.fLaserMinY) / (App->m_LastSettings.fLaserMaxY - App->m_LastSettings.fLaserMinY));
				nY = (int)((float)rr.Height() * ((fZ * -1.0f) - App->m_LastSettings.fLaserMinZ) / (App->m_LastSettings.fLaserMaxZ - App->m_LastSettings.fLaserMinZ));

				pPt[ss].x = rr.right - nX;
				pPt[ss].y = rr.top + nY;
				if(ss==App->m_nIndex) {
					DrawCross(pDC,pPt[ss],4,COLORREF(RGB(0,0,255)));
				} else {
					DrawCross(pDC,pPt[ss],4,COLORREF(RGB(255,0,0)));
				}
			};

			switch(Line.m_nStyle) {
			case LINEAR_LINE:
				pDC->Polyline(pPt,Line.m_nCoordL);
				break;
			case SPLINE_LINE:
				Line.m_bModified=TRUE;
				Line.SetPrimaryAxis(30);
				Line.PreProcessLine();
				fStep = Line.m_fLinearLength / 200.0f;
				if(fStep<=0.0f) fStep=1.0;
				SAFE_DELETE_ARRAY( pPt );
				pPt = new CPoint[802];
				for(fPos=0,ss=0;fPos<=Line.m_fLinearLength;fPos+=fStep,ss++) {
					Line.CalculateCoord(fPos,Cp);
					fY = Cp.Side0.fY + App->m_LastSettings.vLaserTranslation.y;
					fZ = Cp.Side0.fZ + App->m_LastSettings.vLaserTranslation.z - CpHead.Side0.fZ;
					nX = (int)((float)rr.Width() * (fY - App->m_LastSettings.fLaserMinY) / (App->m_LastSettings.fLaserMaxY - App->m_LastSettings.fLaserMinY));
					nY = (int)((float)rr.Height() * (fZ - App->m_LastSettings.fLaserMinZ) / (App->m_LastSettings.fLaserMaxZ - App->m_LastSettings.fLaserMinZ));

					pPt[ss].x = rr.right - nX;
					pPt[ss].y = rr.bottom - nY;
				}
				pDC->Polyline(pPt,ss);
				break;
			}
			pDC->SelectObject(pOldPen);
		}
	}


	Buff.Format(_T("Angle %0.2f X: %.01fmm Y:%.01fmm Z:%.01fmm"),m_fMarkerAngle*RAD_TO_DEG,
		m_Cp.Side0.fX - App->m_LastSettings.vLaserTranslation.x,
		m_fX-App->m_LastSettings.vLaserTranslation.y,
		m_Cp.Side0.fZ + m_fY - App->m_LastSettings.vLaserTranslation.z);
	pDC->TextOut(0,0,Buff);

	SAFE_DELETE_ARRAY( pPt );

}

void CLazerTopographyPage::DrawMarker(CDC *pDC, CRect rr)
{


	APP App = (APP) AfxGetApp();
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	float fAngle,fRange;
	CPoint Pt;
	CPoint CentrePoint = rr.CenterPoint();
	CString Buff;
	CFont HorizontalFont;

	App->m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&App->m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);

	Buff.Format(_T("%0.5f"),m_fMarkerAngle*RAD_TO_DEG);
	pDC->TextOut(0,0,Buff);


	App->m_LaserUser.m_CriticalSection.Lock();
	std::map<float,float>::iterator iter;

	iter = App->m_LaserUser.m_nRangeDataMap.upper_bound(m_fMarkerAngle*RAD_TO_DEG);
	
	fRange = iter->second;
	fAngle = iter->first;


	if(fRange>0) {
		Pt = WorldToClientPolar(fAngle*DEG_TO_RAD,fRange);
		CPen* pOldPen = pDC->SelectObject(&penBlue);
		pDC->MoveTo(0,CentrePoint.y);
		pDC->LineTo(Pt.x,Pt.y);
		Buff.Format(_T("%0.5f, %0.1f"),fAngle,fRange); //require trig to get distance in Y?
		pDC->TextOut(Pt.x,Pt.y,Buff);
	}else{
		int gg=99;
	}

	App->m_LaserUser.m_CriticalSection.Unlock();
}

CPoint CLazerTopographyPage::WorldToClientPolar(float fAngle, float fRange)
{
	APP App = (APP) AfxGetApp();
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.right -= rr.left;
	rr.left=0;
	rr.bottom -= rr.top;
	rr.top=0;
	CPoint retPt, CentrePoint = rr.CenterPoint();
	float fX,fY;


	fX = fRange*cosf(fAngle);
	fY = fRange*sinf(fAngle);


	float xRatio = (rr.Width())/(App->m_LastSettings.fMaxDistance-App->m_LastSettings.fMinDistance);
	retPt.x = (int)((fX*xRatio)-(App->m_LastSettings.fMinDistance*xRatio));


	float fMaxHeight=App->m_LastSettings.fMaxDistance*tanf((App->m_LastSettings.fAngleRange/2)*DEG_TO_RAD);

	if(App->m_LastSettings.bLaserFlipDisplay){
		retPt.y = CentrePoint.y - (int)((fY/fMaxHeight)*(float)(rr.Height()/2));
	}else{
		retPt.y = CentrePoint.y + (int)((fY/fMaxHeight)*(float)(rr.Height()/2));
	}
	

	return retPt;



}

void CLazerTopographyPage::DrawComponentPlot(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();
	CFont HorizontalFont;
	CFont VerticalFont;
	CPen penGrey(PS_SOLID,1,RGB(100,100,100));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPoint pt = rr.CenterPoint();
	CString Buff;
	CSize size;
	float fLastYPos=-1;



	CPen* pOldPen = pDC->SelectObject(&penGrey);
	pDC->SelectStockObject(HOLLOW_BRUSH);

	pDC->MoveTo(rr.left+10,rr.bottom-20);
	pDC->LineTo(rr.left+10,rr.bottom-10);
	pDC->LineTo(rr.right-10,rr.bottom-10);
	pDC->LineTo(rr.right-10,rr.bottom-20);

	App->m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&App->m_LastSettings.lfViewGraticule);
	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);

	Buff.Format(_T("%.0fmm"),App->m_LastSettings.fMinDistance); //Doesn't work TODO
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.left+12,rr.bottom-size.cy-10,Buff);


	Buff.Format(_T("%.0fmm"),App->m_LastSettings.fMaxDistance);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.right-size.cx-12,rr.bottom-size.cy-10,Buff);

	pDC->SelectObject(pOldFont);

	int kk=0;
	bool bFirstPass=true;
	float fAngle;
	float fRange;
	float fStartAngle = 360 - (App->m_LastSettings.fAngleRange/2);
	float fFinishAngle = (App->m_LastSettings.fAngleRange/2);
	float fMaxX=App->m_LastSettings.fMaxDistance;;
	float fMinX=App->m_LastSettings.fMinDistance;
	float fMaxHeight=fMaxX*tanf((App->m_LastSettings.fAngleRange/2)*DEG_TO_RAD);

	std::map<float,float>::iterator iter;

	App->m_LaserUser.m_CriticalSection.Lock();
	for (iter = App->m_LaserUser.m_nRangeDataMap.begin(); iter != App->m_LaserUser.m_nRangeDataMap.end(); iter++ ) {
		fAngle = iter->first * DEG_TO_RAD;		
		//TRACE1("%f\n",fAngle);
 		if( (fAngle>0) && (fAngle<(360*DEG_TO_RAD)) ) {
			if((fAngle>(DEG_TO_RAD*fStartAngle))||(fAngle<(DEG_TO_RAD*fFinishAngle))) {
				
				fRange = iter->second;

				if((fRange>fMinX)&&(fRange<fMaxX)){
				
				/*	if((fAngle<0.005) && (fAngle>-0.005)) {
						if(fTempJitterMax<fRange)
							fTempJitterMax=fRange;
						if(fTempJitterMin>fRange)
							fTempJitterMin=fRange;
					}
				*/

					CPoint pt = WorldToClientPolar(fAngle,fRange);

					if(bFirstPass) {
						pDC->MoveTo(pt.x,pt.y);
					} else {
						if(abs(fLastYPos-pt.y)>10) {
							pDC->MoveTo(pt.x,pt.y);
						}
						pDC->LineTo(pt.x,pt.y);
					}

					fLastYPos = (float)pt.y;
					bFirstPass=false;
				}
			}
		}
	}
	App->m_LaserUser.m_CriticalSection.Unlock();
	pDC->SelectObject(pOldPen);

}

void CLazerTopographyPage::Move(int x, int y)
{
	OnMove(x, y);
}

void CLazerTopographyPage::OnMove(int x, int y) 
{
	CPropertyPage::OnMove(x, y);
	
	if(m_bDlgCreated != true) return;
//	CResizablePage::OnMove(x, y);

	CRect rr,rrReBar;
	GetClientRect(rr);
	ClientToScreen(rr);
	m_wndRebar.GetWindowRect(rrReBar);

	if(m_pLaserViewDlg) m_pLaserViewDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);	
	if(m_pLaserScanDlg) m_pLaserScanDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);	
}


void CLazerTopographyPage::CloseAllChildren()
{
	if(m_pLaserViewDlg) m_pLaserViewDlg->SendMessage(WM_CLOSE);
	if(m_pLaserScanDlg) m_pLaserScanDlg->SendMessage(WM_CLOSE);
}

void CLazerTopographyPage::UpdateAllControls()
{
	if(m_pLaserViewDlg) m_pLaserViewDlg->UpdateAllControls();

}


void CLazerTopographyPage::LaserToolBarView()
{
	CRect rr;
	m_wndRebar.GetWindowRect(rr);

	CloseAllChildren();
	m_pLaserViewDlg = new CLaserViewDlg(this,(CWnd**)&m_pLaserViewDlg,rr.left,rr.bottom);
	m_pLaserViewDlg->Create(CLaserViewDlg::IDD, this);

}

void CLazerTopographyPage::LaserToolBarScan()
{
	CRect rr;
	m_wndRebar.GetWindowRect(rr);

	CloseAllChildren();
	m_pLaserScanDlg = new CLaserScanDlg(this,(CWnd**)&m_pLaserScanDlg,rr.left,rr.bottom);
	m_pLaserScanDlg->Create(CLaserScanDlg::IDD, this);


}

void CLazerTopographyPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	APP App = (APP) AfxGetApp();
	CCoord Cp,CpHead;
	CRect rr;
	m_bLeftButtonDown = true;
	m_fMarkerAngle = ClientToAngle(point);
	int nIgnoreAxis;

	float temp = m_fMarkerAngle * RAD_TO_DEG;

	switch(m_nView) {
	case 0:
		break;
	case 1:
		m_staticView.GetWindowRect(rr);
		ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			App->m_Motors.GetHeadPos(&CpHead);

			ClientToPos(point,&m_fX,&m_fY);

			Cp.Zero();

			Cp.Side0.fX = CpHead.Side0.fX - App->m_LastSettings.vLaserTranslation.x;
			Cp.Side0.fY = m_fX - App->m_LastSettings.vLaserTranslation.y;
			Cp.Side0.fZ = CpHead.Side0.fZ + m_fY - App->m_LastSettings.vLaserTranslation.z;

			PROFILE->FindNearestCoordinate(TAUGHTLINES,Cp,&App->m_nLine,&App->m_nIndex,nIgnoreAxis='X');
			Invalidate(false);
		};
		break;
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}


float CLazerTopographyPage::ClientToAngle(CPoint pt)
{
	APP App = (APP) AfxGetApp();
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pt.y -= rr.top;
	pt.x -= rr.left;
	rr.right -= rr.left;
	rr.left=0;
	rr.bottom -= rr.top;
	rr.top=0;
	CPoint ptCentre = rr.CenterPoint();
	float fAngle;

	float fMaxHeight=App->m_LastSettings.fMaxDistance*tanf((App->m_LastSettings.fAngleRange/2)*DEG_TO_RAD);

	float fPixelsPerMM = (float)rr.Width() / (App->m_LastSettings.fMaxDistance-App->m_LastSettings.fMinDistance);

	rr.left -= (int)(fPixelsPerMM*App->m_LastSettings.fMinDistance);

	float XPosRatio = (float)(pt.x-rr.left) / (float)rr.Width();
	
	fMaxHeight /= XPosRatio;

	float fHeight = ((float)(ptCentre.y-pt.y) / (float)(rr.Height()/2)) * fMaxHeight;

	float fRange = App->m_LastSettings.fMaxDistance;
	
	if(App->m_LastSettings.bLaserFlipDisplay){
		fAngle = atan2f(fHeight,fRange);
	}else{
		fAngle = -atan2f(fHeight,fRange);
	}

	if(fAngle<0) 
		fAngle += (2.0f*PIf);

	return fAngle;

}

void CLazerTopographyPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bLeftButtonDown) {
		m_fMarkerAngle = ClientToAngle(point);
		ClientToPos(point,&m_fX,&m_fY);
	}
	
	CPropertyPage::OnMouseMove(nFlags, point);
}

void CLazerTopographyPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLeftButtonDown = false;
	
	CPropertyPage::OnLButtonUp(nFlags, point);
}


void CLazerTopographyPage::ClientToPos(CPoint pt, float *fX, float *fY)
{
	APP App = (APP) AfxGetApp();
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	CPoint pt0 = rr.CenterPoint();

	switch(m_nView) {
	case 0:
		*fX = (float)(pt.x - pt0.x) * App->m_LastSettings.fMaxDistance / (float)m_nRadius * -1.0f;
		*fY = (float)(pt.y - pt0.y) * App->m_LastSettings.fMaxDistance / (float)m_nRadius * -1.0f;
		break;
	case 1:
		*fX = ((float)(rr.right - pt.x) * (App->m_LastSettings.fLaserMaxY - App->m_LastSettings.fLaserMinY) / (float)rr.Width()) +  App->m_LastSettings.fLaserMinY;
		*fY = (((float)(pt.y - rr.top ) * (App->m_LastSettings.fLaserMaxZ - App->m_LastSettings.fLaserMinZ) / (float)rr.Height()) +  App->m_LastSettings.fLaserMinZ) * -1.0f;
		break;
	}
}

void CLazerTopographyPage::OnButton3dTools() 
{
	CProfileSheet *pSheet = (CProfileSheet*)GetParent();
	pSheet->ToolsDrawingoptions();
	
}

void CLazerTopographyPage::OnButtonLaser360View() 
{
	m_nView = 0;
	Invalidate(false);
	SetToolBarCheckedState();
	
}

void CLazerTopographyPage::OnButtonLaserWindowView() 
{
	m_nView = 1;
	Invalidate(false);
	SetToolBarCheckedState();
}

void CLazerTopographyPage::SetToolBarCheckedState()
{
	APP App = (APP)AfxGetApp();
	int nIndex;

	return;
	if((nIndex=m_wndLaserBar.CommandToIndex(ID_BUTTON_LASER_360_VIEW))>=0)
		m_nView==0 ? m_wndLaserBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndLaserBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndLaserBar.CommandToIndex(ID_BUTTON_LASER_WINDOW_VIEW))>=0)
		m_nView==1 ? m_wndLaserBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndLaserBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndLaserBar.CommandToIndex(ID_BUTTON_LASER_OFF_ON))>=0)
		App->m_LastSettings.bLaserOffOn==true ? m_wndLaserBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndLaserBar.SetButtonStyle(nIndex,0);
}

void CLazerTopographyPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	APP App = (APP) AfxGetApp();
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint pt = point;
	CRect Rect = m_StaticPosView.m_Rect;
	ScreenToClient(&point);
	m_ptClient = point;

	if(Rect.PtInRect(point)) {
		menu->LoadMenu(IDR_RB_LASER_MENU);
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
	}

	delete menu;
	
}

void CLazerTopographyPage::OnLaserAddtaughtpoint() 
{
	APP App = (APP) AfxGetApp();
	CCoord Cp, CpHead;

	if(App->m_nLine>=PROFILE->m_nTaughtLineL) App->m_nLine=PROFILE->m_nTaughtLineL-1;
	if(App->m_nLine<0) App->m_nLine=0;

	App->m_Motors.GetHeadPos(&CpHead);

	ClientToPos(m_ptClient,&m_fX,&m_fY);

	Cp.Zero();

	Cp.Side1.fX = Cp.Side0.fX = CpHead.Side0.fX - App->m_LastSettings.vLaserTranslation.x;
	Cp.Side1.fY = Cp.Side0.fY = m_fX - App->m_LastSettings.vLaserTranslation.y;
	Cp.Side1.fZ = Cp.Side0.fZ = CpHead.Side0.fZ + m_fY - App->m_LastSettings.vLaserTranslation.z;
	Cp.Side1.fJ = Cp.Side0.fJ = -1.0f;

	if(PROFILE->m_TaughtLine == NULL) {
		PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=1];
		PROFILE->m_TaughtLine[App->m_nLine=0].AddCoord(Cp);
	} else {
		PROFILE->m_TaughtLine[App->m_nLine].AddCoord(Cp);
	}
	PROFILE->m_TaughtLine[App->m_nLine].Sort(2);

	Invalidate(false);
}

void CLazerTopographyPage::OnLaserDelete() 
{
	APP App = (APP) AfxGetApp();

	if(PROFILE->m_TaughtLine) PROFILE->m_TaughtLine[App->m_nLine].DeleteCoord(App->m_nIndex);
	Invalidate(false);
	
}

void CLazerTopographyPage::OnLaserNewtaughtline() 
{
	APP App = (APP) AfxGetApp();
	
	PROFILE->AddLine(TAUGHTLINES);
	App->m_nLine = PROFILE->m_nTaughtLineL - 1;
	OnLaserAddtaughtpoint();
	UpdateAllControls();

}

void CLazerTopographyPage::OnButtonLaserOffOn() 
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.bLaserOffOn == false ? App->m_LastSettings.bLaserOffOn = true : App->m_LastSettings.bLaserOffOn = false;
	SetToolBarCheckedState();

	if(App->m_LastSettings.bLaserOffOn == false) {
//		App->m_LaserUser.LaserOff();
//		App->m_LaserUser.setMotorRPM(1,0,1);
		App->m_Si10User.CloseLaserDoor();
	} else {
//		App->m_LaserUser.LaserOn();
//		App->m_LaserUser.setMotorRPM(1,App->m_LastSettings.nLaserMotorRPM,1);
		App->m_Si10User.OpenLaserDoor();
	}
}

BOOL CLazerTopographyPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_3D_TOOLS:  strTipText.LoadString(IDS_Tools);
		break;
	case ID_BUTTON_LASER_360_VIEW:  strTipText.LoadString(IDS_360_View);
		break;
	case ID_BUTTON_LASER_WINDOW_VIEW:  strTipText.LoadString(IDS_Window_View);
		break;
	case ID_BUTTON_LASER_OFF_ON:  strTipText.LoadString(IDS_Laser_On_Off);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CLazerTopographyPage::OnLaserStyleLinear() 
{
	APP App = (APP) AfxGetApp();

	PROFILE->m_TaughtLine[App->m_nLine].m_nStyle = 0;
	Invalidate(false);
	
}

void CLazerTopographyPage::OnLaserStyleSpline() 
{
	APP App = (APP) AfxGetApp();

	PROFILE->m_TaughtLine[App->m_nLine].m_nStyle = 1;
	Invalidate(false);
	
}

void CLazerTopographyPage::DrawCross(CDC *pDC, CPoint pt, int nSize, COLORREF rgb)
{
	CPen pen(PS_SOLID,1,rgb);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(pt.x-nSize, pt.y-nSize);
	pDC->LineTo(pt.x+nSize+1, pt.y+nSize+1);
	pDC->MoveTo(pt.x-nSize, pt.y+nSize);
	pDC->LineTo(pt.x+nSize+1, pt.y-nSize-1);

	pDC->SelectObject(pOldPen);
}


void CLazerTopographyPage::OnTimer(UINT nIDEvent) 
{

	Invalidate(false);
	CPropertyPage::OnTimer(nIDEvent);
}
