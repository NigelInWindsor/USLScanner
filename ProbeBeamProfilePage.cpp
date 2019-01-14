// ProbeBeamProfilePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeBeamProfilePage.h"
#include "ProbeCharacterisationSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeBeamProfilePage property page

IMPLEMENT_DYNCREATE(CProbeBeamProfilePage, CResizablePage)

CProbeBeamProfilePage::CProbeBeamProfilePage(CProbe* pProbe,CProbeBeamProfile* pBeamProfile) : CResizablePage(CProbeBeamProfilePage::IDD)
{
	//{{AFX_DATA_INIT(CProbeBeamProfilePage)
	//}}AFX_DATA_INIT
	m_pProbe = pProbe;
	m_pBeamProfile = pBeamProfile;
	
	m_rr = NULL;


}

CProbeBeamProfilePage::~CProbeBeamProfilePage()
{
	SAFE_DELETE_ARRAY( m_rr );
}

void CProbeBeamProfilePage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeBeamProfilePage)
	DDX_Control(pDX, IDC_CHECK_FOCAL_LENGTH, m_checkFocalLength);
	DDX_Control(pDX, IDC_CHECK_SPOT_SIZE_FAR, m_checkSpotSizeFarZone);
	DDX_Control(pDX, IDC_CHECK_NEAR_ZONE, m_checkNearZone);
	DDX_Control(pDX, IDC_CHECK_SPOT_SIZE, m_checkSpotSize);
	DDX_Control(pDX, IDC_CHECK_AT_ATTENUATION, m_checkAtAttenuation);
	DDX_Control(pDX, IDC_EDIT_AT_DB_DROP, m_editAtAttenuation);
	DDX_Control(pDX, IDC_SPIN_DB_DROP, m_spinAtAttenuation);
	DDX_Control(pDX, IDC_STATIC_AXIAL_PLOT, m_staticAxialPlot);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_STATIC_AXIAL_PLOT_GRAPH, m_staticAxialGraph);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeBeamProfilePage, CResizablePage)
	//{{AFX_MSG_MAP(CProbeBeamProfilePage)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BEAM_CLEARALL, OnBeamClearall)
	ON_EN_CHANGE(IDC_EDIT_AT_DB_DROP, OnChangeEditAtDbDrop)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_AT_ATTENUATION, OnCheckAtAttenuation)
	ON_BN_CLICKED(IDC_CHECK_SPOT_SIZE, OnCheckSpotSize)
	ON_BN_CLICKED(IDC_CHECK_NEAR_ZONE, OnCheckNearZone)
	ON_BN_CLICKED(IDC_CHECK_SPOT_SIZE_FAR, OnCheckSpotSizeFar)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_COMMAND(ID_BEAM_GOTOHERE, OnBeamGotohere)
	ON_COMMAND(ID_BEAM_UPDATESCANCOORDINATES, OnBeamUpdatescancoordinates)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeBeamProfilePage message handlers

BOOL CProbeBeamProfilePage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CStatic* pStatic;

//	AddAnchor(IDC_STATIC_VIEW, CSize(0,0), CSize(100,100));
//	AddAnchor(IDC_STATIC_AXIAL_PLOT, CSize(0,0), CSize(100,100));

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosAxialPlot.Initialize(this, m_staticAxialPlot, TRUE, TRUE, TRUE, FALSE);
	m_StaticPosAxialGraph.Initialize(this, m_staticAxialGraph, TRUE, TRUE, TRUE, FALSE);


//	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC0)) pStatic->SetWindowText(m_pBeamProfile->m_Fields.GetAt(0));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC1)) pStatic->SetWindowText(m_pBeamProfile->m_Fields.GetAt(1));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC2)) pStatic->SetWindowText(m_pBeamProfile->m_Fields.GetAt(2));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC3)) pStatic->SetWindowText(m_pBeamProfile->m_Fields.GetAt(3));

	m_StaticPosView.SetNewPos();
	m_StaticPosAxialPlot.SetNewPos();
	m_StaticPosAxialGraph.SetNewPos();

	m_spinAtAttenuation.SetRange(0,300);

	m_bLbDown=false;
	m_bRbDown=false;
	m_fAxialX[0] = 0.5f;
	m_fAxialX[1] = 0.5f;

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProbeBeamProfilePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	CEdit* pEdit;

//	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT0)) pEdit->SetWindowText(m_pBeamProfile->m_strValues.GetAt(0));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT1)) pEdit->SetWindowText(m_pBeamProfile->m_strValues.GetAt(1));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT2)) pEdit->SetWindowText(m_pBeamProfile->m_strValues.GetAt(2));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT3)) pEdit->SetWindowText(m_pBeamProfile->m_strValues.GetAt(3));

	Buff.Format(_T("%.0f dB"),theApp.m_LastSettings.fProbeAtAttenuation);
	m_editAtAttenuation.SetWindowText(Buff);
	m_spinAtAttenuation.SetPos((int)fabs(theApp.m_LastSettings.fProbeAtAttenuation));

	m_checkAtAttenuation.SetCheck(theApp.m_LastSettings.bProbeDisplayAttenuation);
	m_checkSpotSize.SetCheck(theApp.m_LastSettings.bProbeDisplaySpotSize);
	m_checkNearZone.SetCheck(theApp.m_LastSettings.bProbeDisplayNearZone);


}

void CProbeBeamProfilePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	m_staticAxialPlot.GetWindowRect(rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);


	RenderCScans(&dc);
	RenderBScan(&dc);
	RenderAxialGraph(&dc);
	
}

void CProbeBeamProfilePage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		m_StaticPosView.SetNewPos();
		m_StaticPosAxialPlot.SetNewPos();
		m_StaticPosAxialGraph.SetNewPos();
		CDC* pDC = GetDC();
		RenderCScans((CPaintDC*)pDC);
		RenderBScan((CPaintDC*)pDC);
//		Invalidate(FALSE);
		ReleaseDC(pDC);
	}
	
}


void CProbeBeamProfilePage::RenderCScans(CPaintDC* pDC)
{
	CRect rrView;
	CDC  dcMem;
	CBitmap bmp;
	int	nOffset,nPtr,nColor,ii,nX,nY,nPtr1;
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;
	int nRow,nColumn,nScan,nArraySize;
	CString Buff;
	int m_nImageOrientation = 0;
	int nPeak,nMaxColor,nMinColor;
	CButton* pButton;
	CPen* pOldPen;
	CRect rr;

	m_staticView.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(2,2);

	if(m_pBeamProfile->m_nCScansL<=0) {
		pDC->FillRect(rrView,&CBrush(RGB(255,255,255)));
		return;
	}


	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 14;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);


	char *pArrayView = new char[(rrView.Width()+2) * (rrView.Height()+2) * nBytesPixel];
	char *pDest;



	unsigned char	cColor[256][4];
	for(ii=0;ii<256;ii++) {
		cColor[ii][0]= unsigned char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>16)&0xff);
		cColor[ii][1]= unsigned char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>8)&0xff);
		cColor[ii][2]= unsigned char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>0)&0xff);
	}

	nPeak = m_pBeamProfile->CalculateStatsFromCScans(theApp.m_LastSettings.fProbeAtAttenuation);

	if(theApp.m_LastSettings.bProbeDisplayAttenuation) {
		nMaxColor = (int)(pow(10,theApp.m_LastSettings.fProbeAtAttenuation / 20.0f) * nPeak) + 128;
		nMinColor = (int)(pow(10,(theApp.m_LastSettings.fProbeAtAttenuation-1.0f) / 20.0f) * nPeak) + 128;
		for(ii=nMinColor;ii<=nMaxColor;ii++) {
			cColor[ii][0]=255;
			cColor[ii][1]=0;
			cColor[ii][2]=0;
		}
	}

	int nRowsL,nColumnsL;
	nRowsL = nColumnsL = (int)sqrt(m_pBeamProfile->m_nCScansL);
	if((nRowsL * nColumnsL) < m_pBeamProfile->m_nCScansL) {
		nColumnsL++;
		nRowsL++;
	}
	CProbeScanData *pScan;
	SAFE_DELETE_ARRAY( m_rr );
	m_rr = new CRect[m_pBeamProfile->m_nCScansL];

	for(nRow=0,nScan=0;nRow<nRowsL;nRow++) {
		for(nColumn=0;(nColumn<nColumnsL) && (nScan<m_pBeamProfile->m_nCScansL) ;nColumn++,nScan++) {
			m_rr[nScan].left = MulDiv(nColumn,rrView.Width(),nColumnsL);
			m_rr[nScan].right = MulDiv(nColumn+1,rrView.Width(),nColumnsL);
			m_rr[nScan].top = MulDiv(nRow,rrView.Height(),nRowsL);
			m_rr[nScan].bottom = MulDiv(nRow+1,rrView.Height(),nRowsL);
			
			pScan = &m_pBeamProfile->m_pCScans[nScan];
			nArraySize = pScan->m_nSamplesLine * pScan->m_nNumberLines;

			if(nArraySize) {
				for(nY=0;nY<m_rr[nScan].Height();nY++) {
					if(m_nImageOrientation == 0) {
						nOffset = MulDiv(nY,pScan->m_nNumberLines-1,m_rr[nScan].Height()-1) * pScan->m_nSamplesLine;
					} else {
						nOffset = MulDiv(m_rr[nScan].Height()-nY-1,pScan->m_nNumberLines-1,m_rr[nScan].Height()-1) * pScan->m_nSamplesLine;
					}
					pDest = pArrayView + (((m_rr[nScan].top + nY) * rrView.Width()) + m_rr[nScan].left) * nBytesPixel;
					for(nX=0;nX<m_rr[nScan].Width();nX++) {
						nPtr = MulDiv(nX,pScan->m_nSamplesLine-1,m_rr[nScan].Width()-1) + nOffset;
						nPtr1=nPtr + pScan->m_nSamplesLine;
						MinMax(&nPtr1,0,nArraySize-1);
						if((nPtr>=0) && (nPtr<nArraySize)) {
							nColor = (pScan->m_cAmpBuffer[nPtr] + pScan->m_cAmpBuffer[nPtr1]) / 2 + 128;
							*pDest++=cColor[nColor][0];
							*pDest++=cColor[nColor][1];
							*pDest++=cColor[nColor][2];
							if(nBytesPixel>3) pDest++;
						} else {
							TRACE0("Error");
							ASSERT(FALSE);
						}
					}
				}
			}
		}
	}


	bmp.CreateCompatibleBitmap(pDC,rrView.Width(),rrView.Height());
	bmp.SetBitmapBits((DWORD)(rrView.Width()*rrView.Height()*4),pArrayView);
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(RGB(255,128,129));
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(&Font);
	CPen penGreenyBlue(PS_SOLID,1,RGB(0,128,128));
	pOldPen = dcMem.SelectObject(&penGreenyBlue);


	for(nScan = 0; nScan < m_pBeamProfile->m_nCScansL; nScan++) {
		pScan = &m_pBeamProfile->m_pCScans[nScan];

		if(theApp.m_LastSettings.bProbeDisplayWaterPath) {
			Buff.Format(_T("Wp:%.01fmm"),pScan->m_CpCentre.fWp);
			dcMem.TextOut(m_rr[nScan].left,m_rr[nScan].top,Buff);
		}

		if(theApp.m_LastSettings.bProbeDisplaySpotSize) {
			Buff.Format(_T("D:%.01fmm W:%.01fmm H:%.01fmm WP:%01.fmm"),pScan->m_fDiameterSpot,pScan->m_fWidthSpot,pScan->m_fHeightSpot,pScan->m_CpCentre.fWp[0]);
			dcMem.TextOut(m_rr[nScan].left,m_rr[nScan].top,Buff);
			Buff.Format(_T("Sym(WH):%.02fmm"),pScan->m_fSymmetrySpot);
			CSize size = dcMem.GetTextExtent(Buff);
			dcMem.TextOut(m_rr[nScan].left,m_rr[nScan].top+size.cy,Buff);

			nX = MulDiv(pScan->m_ptCentre.x,m_rr[nScan].Width()-1,pScan->m_nSamplesLine) + m_rr[nScan].left;
			nY = MulDiv(pScan->m_ptCentre.y,m_rr[nScan].Height()-1,pScan->m_nNumberLines) + m_rr[nScan].top;
			dcMem.MoveTo(nX-3,nY);
			dcMem.LineTo(nX+4,nY);
			dcMem.MoveTo(nX,nY-3);
			dcMem.LineTo(nX,nY+4);

			rr.left = MulDiv(pScan->m_rrArc.left,m_rr[nScan].Width()-1,pScan->m_nSamplesLine) + m_rr[nScan].left;
			rr.right = MulDiv(pScan->m_rrArc.right,m_rr[nScan].Width()-1,pScan->m_nSamplesLine) + m_rr[nScan].left;
			rr.top = MulDiv(pScan->m_rrArc.top,m_rr[nScan].Height()-1,pScan->m_nNumberLines) + m_rr[nScan].top;
			rr.bottom = MulDiv(pScan->m_rrArc.bottom,m_rr[nScan].Height()-1,pScan->m_nNumberLines) + m_rr[nScan].top;
			dcMem.Arc(rr,rr.TopLeft(),rr.TopLeft());
		}
	}


	pDC->BitBlt(rrView.left,rrView.top,rrView.Width(),rrView.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldBitmap);
	dcMem.SelectObject(pOldFont);

	delete pArrayView;


	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_SPOT_SIZE)) {
		Buff.Format(_T("Spot size from C scans: %.02f mm"),m_pBeamProfile->m_fSpotSizeFromCScans);
		pButton->SetWindowText(Buff);
	}


}

void CProbeBeamProfilePage::RenderBScan(CPaintDC* pDC)
{
	CRect rrView;
	CDC  dcMem;
	CBitmap bmp;
	int	nOffset,nPtr,nColor,ii,nX,nY;
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;
	int nArraySize,nPeak,nMinColor,nMaxColor;
	CString Buff;
	CProbeScanData *pScan;
	CPoint pt;
	CButton* pButton;

	m_staticAxialPlot.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(2,2);


	if(m_pBeamProfile->m_nBScansL<=0) {
		pDC->FillRect(rrView,&CBrush(RGB(255,255,255)));
		return;
	}


	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = theApp.m_Scope.m_nTextSize;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);

	ASSERT(pDC);

	char *pArrayView = new char[(rrView.Width()+2) * (rrView.Height()+2) * nBytesPixel];
	char *pDest;



	unsigned char	cColor[256][4];
	for(ii=0;ii<256;ii++) {
		cColor[ii][0]= unsigned char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>16)&0xff);
		cColor[ii][1]= unsigned char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>8)&0xff);
		cColor[ii][2]= unsigned char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>0)&0xff);
	}

	nPeak = m_pBeamProfile->CalculateStatsFromBScans(theApp.m_LastSettings.fProbeAtAttenuation);

	if(theApp.m_LastSettings.bProbeDisplayAttenuation) {
		nMaxColor = (int)(pow(10,theApp.m_LastSettings.fProbeAtAttenuation / 20.0f) * nPeak) + 128;
		nMinColor = (int)(pow(10,(theApp.m_LastSettings.fProbeAtAttenuation-2.0f) / 20.0f) * nPeak) + 128;
		for(ii=nMinColor;ii<=nMaxColor;ii++) {
			cColor[ii][0]=255;
			cColor[ii][1]=0;
			cColor[ii][2]=0;
		}
	}


	pScan = &m_pBeamProfile->m_pBScans[0];
	nArraySize = pScan->m_nSamplesLine * pScan->m_nNumberLines;

	for(nY=0;nY<rrView.Height();nY++) {
		nOffset = MulDiv(nY,pScan->m_nNumberLines-1,rrView.Height()-1) * pScan->m_nSamplesLine;
		pDest = pArrayView + (nY * rrView.Width()) * nBytesPixel;
		for(nX=0;nX<rrView.Width();nX++) {
			nPtr = MulDiv(nX,pScan->m_nSamplesLine-1,rrView.Width()-1) + nOffset;
			if((nPtr>=0) && (nPtr<nArraySize)) {
				nColor = pScan->m_cAmpBuffer[nPtr] + 128;
				*pDest++=cColor[nColor][0];
				*pDest++=cColor[nColor][1];
				*pDest++=cColor[nColor][2];
				if(nBytesPixel>3) pDest++;
			} else {
//				TRACE0("Error");
//				ASSERT(FALSE);
			}
		}
	}


	bmp.CreateCompatibleBitmap(pDC,rrView.Width(),rrView.Height());
	bmp.SetBitmapBits((DWORD)(rrView.Width()*rrView.Height()*4),pArrayView);
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(RGB(255,128,129));
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(&Font);
	CPen penGreenyBlue(PS_SOLID,1,RGB(0,128,128));
	CPen* pOldPen = dcMem.SelectObject(&penGreenyBlue);


	if(theApp.m_LastSettings.bProbeDisplaySpotSize) {

		pt.x = MulDiv(pScan->m_ptEllipseTop.x,rrView.Width()-1,pScan->m_nSamplesLine);
		pt.y = MulDiv(pScan->m_ptEllipseTop.y,rrView.Height()-1,pScan->m_nNumberLines);
		dcMem.MoveTo(pt.x,pt.y-3);
		dcMem.LineTo(pt.x,pt.y+4);
		dcMem.MoveTo(pt.x-3,pt.y);
		dcMem.LineTo(pt.x+4,pt.y);
		pt.x = MulDiv(pScan->m_ptEllipseLeft.x,rrView.Width()-1,pScan->m_nSamplesLine);
		pt.y = MulDiv(pScan->m_ptEllipseLeft.y,rrView.Height()-1,pScan->m_nNumberLines);
		dcMem.MoveTo(pt.x,pt.y-3);
		dcMem.LineTo(pt.x,pt.y+4);
		dcMem.MoveTo(pt.x-3,pt.y);
		dcMem.LineTo(pt.x+4,pt.y);
		pt.x = MulDiv(pScan->m_ptEllipseRight.x,rrView.Width()-1,pScan->m_nSamplesLine);
		pt.y = MulDiv(pScan->m_ptEllipseRight.y,rrView.Height()-1,pScan->m_nNumberLines);
		dcMem.MoveTo(pt.x,pt.y-3);
		dcMem.LineTo(pt.x,pt.y+4);
		dcMem.MoveTo(pt.x-3,pt.y);
		dcMem.LineTo(pt.x+4,pt.y);
		pt.x = MulDiv(pScan->m_ptEllipseBottom.x,rrView.Width()-1,pScan->m_nSamplesLine);
		pt.y = MulDiv(pScan->m_ptEllipseBottom.y,rrView.Height()-1,pScan->m_nNumberLines);
		dcMem.MoveTo(pt.x,pt.y-3);
		dcMem.LineTo(pt.x,pt.y+4);
		dcMem.MoveTo(pt.x-3,pt.y);
		dcMem.LineTo(pt.x+4,pt.y);


	}
	if(theApp.m_LastSettings.bProbeDisplayNearZone) {
		pt.y = MulDiv(pScan->m_ptCentre.y,rrView.Height()-1,pScan->m_nNumberLines);
		dcMem.MoveTo(0,pt.y);
		dcMem.LineTo(rrView.Width()-1,pt.y);
	}

	pDC->BitBlt(rrView.left,rrView.top,rrView.Width(),rrView.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldBitmap);
	dcMem.SelectObject(pOldFont);

	delete pArrayView;

	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_NEAR_ZONE)) {
		Buff.Format(_T("Near Zone from B scan : %.02f mm"),pScan->m_ptCentre.y * pScan->m_fSlowIncrement + m_pProbe->m_Cp[0].fWp[0]);
		pButton->SetWindowText(Buff);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_SPOT_SIZE_FAR)) {
		Buff.Format(_T("Spot Size far Zone : %.02f mm"),pScan->m_fDiameterSpot);
		pButton->SetWindowText(Buff);
	}
	if(pButton = (CButton*)GetDlgItem(IDC_CHECK_FOCAL_LENGTH)) {
		Buff.Format(_T("Focal Length : %.02f mm"),pScan->m_fFocalLength);
		pButton->SetWindowText(Buff);
	}

	DrawBScanGraticule(pDC,pScan);

}


BOOL CProbeBeamProfilePage::OnSetActive() 
{
	((CProbeCharacterisationSheet*)GetParent())->SetToolBarCheckedState();	//SetToolBarCheckedState();;
	((CProbeCharacterisationSheet*)GetParent())->m_pCurrentPage = (CPropertyPage*)this;
	
	return CResizablePage::OnSetActive();
}



void CProbeBeamProfilePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CPoint pt = point;
	CRect rr;

	ScreenToClient(&point);
	m_ptClient = point;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB__BEAM_CSCAN_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		};
	}

	delete menu;

	
}

void CProbeBeamProfilePage::OnBeamClearall() 
{
	m_pBeamProfile->DeleteAllCScans();
	Invalidate(FALSE);
}

int CProbeBeamProfilePage::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

void CProbeBeamProfilePage::OnChangeEditAtDbDrop() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fProbeAtAttenuation;

	m_editAtAttenuation.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fProbeAtAttenuation);
	if(fTemp - theApp.m_LastSettings.fProbeAtAttenuation) {
		m_spinAtAttenuation.SetPos((int)fabs(theApp.m_LastSettings.fProbeAtAttenuation));
	}
}

void CProbeBeamProfilePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_DB_DROP:
		fTemp = theApp.m_LastSettings.fProbeAtAttenuation;
		theApp.m_LastSettings.fProbeAtAttenuation = (float)(pSpin->GetPos()&0xffff) * -1.0f;
		if(fTemp - theApp.m_LastSettings.fProbeAtAttenuation) {
			UpdateAllControls();
			Invalidate(FALSE);
		}
		break;
	}	
	
	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CProbeBeamProfilePage::OnCheckAtAttenuation() 
{
	theApp.m_LastSettings.bProbeDisplayAttenuation = m_checkAtAttenuation.GetCheck();
	Invalidate(FALSE);
}

void CProbeBeamProfilePage::OnCheckSpotSize() 
{
	theApp.m_LastSettings.bProbeDisplaySpotSize = m_checkSpotSize.GetCheck();
	Invalidate(FALSE);
}

void CProbeBeamProfilePage::OnCheckNearZone() 
{
	theApp.m_LastSettings.bProbeDisplayNearZone = m_checkNearZone.GetCheck();
	Invalidate(FALSE);
}

void CProbeBeamProfilePage::OnCheckSpotSizeFar() 
{
	// TODO: Add your control notification handler code here
	
}

void CProbeBeamProfilePage::DrawBScanGraticule(CPaintDC *pDC,CProbeScanData *pScan)
{
	CRect rr,rrBigger,rrBeamProfile,rrCScan;
	int nY;
	CString Buff;

	CBrush brushBK(GetSysColor(COLOR_BTNFACE));
	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 10;
	theApp.SetFontLanguage(&lf);
	lf.lfEscapement=900;
	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);

	CSize size = pDC->GetTextExtent(_T("-100.0"));

	m_staticAxialPlot.GetWindowRect(&rrBeamProfile);
	ScreenToClient(&rrBeamProfile);

	m_staticView.GetWindowRect(&rrCScan);
	ScreenToClient(&rrCScan);

//Right hand border
	rr = rrBeamProfile;
	rr.DeflateRect(2,2);
	rr.left = rr.right+2;
	rr.right += (size.cx+4);
	rrBigger = rr;
	rrBigger.top -= size.cx;
	rrBigger.bottom += size.cx;
	pDC->FillRect(&rrBigger,&brushBK);
//left hand border
	rrBigger.left = rrCScan.right;
	rrBigger.right = rrBeamProfile.left;
	rrBigger.top = rrBeamProfile.top-size.cx/2;
	rrBigger.bottom = rrBeamProfile.bottom+size.cx;
	pDC->FillRect(&rrBigger,&brushBK);
//top border
	rrBigger.left = rrBeamProfile.left-20;
	rrBigger.right = rrBeamProfile.right + 20;
	rrBigger.top = rrBeamProfile.top-size.cy;
	rrBigger.bottom = rrBeamProfile.top;
	pDC->FillRect(&rrBigger,&brushBK);
//bottom border
	rrBigger.left = rrBeamProfile.left;
	rrBigger.right = rrBeamProfile.right + 20;
	rrBigger.top = rrBeamProfile.bottom;
	rrBigger.bottom = rrBeamProfile.bottom + size.cy*2;
	pDC->FillRect(&rrBigger,&brushBK);
//Top Edge of c scan
	rrBigger.left = rrCScan.left;
	rrBigger.right = rrBeamProfile.left;
	rrBigger.top = rrCScan.top-size.cy;
	rrBigger.bottom = rrCScan.top;
	pDC->FillRect(&rrBigger,&brushBK);
	
	pDC->SetBkMode(TRANSPARENT);

	CPoint pt;
	rr = rrBeamProfile;
	rr.DeflateRect(2,2);

	pt.y = rr.top;
	pt.x = (int)(m_fAxialX[0] * (float)rr.Width()) + rr.left;
	pDC->MoveTo(pt);
	pt.y = rr.bottom;
	pt.x = (int)(m_fAxialX[1] * (float)rr.Width()) + rr.left;
	pDC->LineTo(pt);

	rr = rrBeamProfile;
	rr.left=rr.right;
	rr.right+=20;
	rr.DeflateRect(2,2);


	float fStart = pScan->m_Cp[0].fWp[0];
	float fFinish = pScan->m_Cp[0].Length(pScan->m_Cp[1],0) + fStart;

	if((fStart<0.0f) || (fStart>=1000.0f)) return;
	if((fFinish<0.0f) || (fFinish>=1000.0f)) return;
	if(fStart<=fFinish) return;

	float fY = (float)((((int)fStart / 5) + 1) * 5);

	for(fY;fY<=fFinish;fY+=5.0f) {
		nY=(int)((fY-fStart)* (float)(rr.Height()-1) /  (fFinish-fStart)) + rr.top;
		pDC->MoveTo(rr.left,nY);
		pDC->LineTo(rr.left+3,nY);
		Buff.Format(_T("%.01f"),fY);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.left+3,nY+size.cx/2,Buff);
	}

	for(int ii=0;ii<m_pBeamProfile->m_nCScansL;ii++) {
		fY = m_pBeamProfile->m_pCScans[ii].m_CpCentre.fWp[0];
		nY=(int)((fY-fStart)* (float)(rr.Height()-1) /  (fFinish-fStart)) + rr.top;
		pDC->MoveTo(rrBeamProfile.left,nY);
		pDC->LineTo(rrBeamProfile.left-3,nY);
		Buff.Format(_T("%d"),ii+1);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rrBeamProfile.left-1-size.cy,nY+size.cx/2,Buff);
	}



	pDC->SelectObject(pOldFont);
}

void CProbeBeamProfilePage::OnChangeEdit1() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pBeamProfile->m_strValues.SetAt(1,Buff);
	m_pProbe->m_bModified = TRUE;
}

void CProbeBeamProfilePage::OnChangeEdit2() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pBeamProfile->m_strValues.SetAt(2,Buff);
	m_pProbe->m_bModified = TRUE;
}

void CProbeBeamProfilePage::OnChangeEdit3() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	m_pBeamProfile->m_strValues.SetAt(3,Buff);
	m_pProbe->m_bModified = TRUE;
}
/*
m_pBeamProfile->m_nCScansL) {
		nColumnsL++;
		nRowsL++;
	}
	CProbeScanData *pScan;
	SAFE_DELETE_ARRAY( m_rr );
	m_rr = new CRect[m_pBeamProfile->m_nCScansL];

	for(nRow=0,nScan=0;nRow<nRowsL;nRow++) {
		for(nColumn=0;(nColumn<nColumnsL) && (nScan<m_pBeamProfile->m_nCScansL) ;nColumn++,nScan++) {
			m_rr[nScan].left = MulDiv(nColumn,rrView.Width(),nColumnsL);
			m_rr[nScan].right = MulDiv(nColumn+1,rrView.Width(),nColumnsL);
			m_rr[nScan].top = MulDiv(nRow,rrView.Height(),nRowsL);
			m_rr[nScan].b
			*/

void CProbeBeamProfilePage::OnBeamGotohere() 
{
	CCoord Cp;
	CRect rrView;

	m_staticView.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(2,2);

	CPoint pt = m_ptClient;
	pt.x -= rrView.left;
	pt.y -= rrView.top;

	theApp.m_Motors.GetHeadPos(&Cp);
	for(int nScan=0;nScan<m_pBeamProfile->m_nCScansL;nScan++) {
		if(m_rr[nScan].PtInRect(pt)) {
			CProbeScanData *pScan = &m_pBeamProfile->m_pCScans[nScan];

			Cp.Side0.fX = ((float)(pt.x-m_rr[nScan].left) * pScan->m_fFastScanSize / (float)m_rr[nScan].Width()) + (pScan->m_Cp[0].Side0.fX - pScan->m_fFastScanSize/2.0f) ;
			Cp.Side0.fZ = ((float)(pt.y-m_rr[nScan].top) *  pScan->m_fSlowScanSize / (float)m_rr[nScan].Height()) + (pScan->m_Cp[0].Side0.fZ - pScan->m_fSlowScanSize/2.0f) ;
			Cp.Side0.fY = pScan->m_CpCentre.Side0.fY;
	
			theApp.m_Thread.m_bEnableJoystick=FALSE;
			theApp.m_Thread.StopCurrentThreadAction();

			theApp.m_Thread.m_CpMoveTo = Cp;
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

			if(theApp.m_LastSettings.bProbeTrackgate == TRUE) {
				int nDelay = pScan->m_Cp[0].nDelay[0];
				theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0] = nDelay;
				theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide],0);
			}

		}
	}
	
}

void CProbeBeamProfilePage::OnBeamUpdatescancoordinates() 
{
	CCoord Cp;
	CRect rrView;
	CProbeCharacterisationSheet* pParent = (CProbeCharacterisationSheet*)GetParent();
	
	m_staticView.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(2,2);

	CPoint pt = m_ptClient;
	pt.x -= rrView.left;
	pt.y -= rrView.top;

	theApp.m_Motors.GetHeadPos(&Cp);
	for(int nScan=0;nScan<m_pBeamProfile->m_nCScansL;nScan++) {
		if(m_rr[nScan].PtInRect(pt)) {
			CProbeScanData *pScan = &m_pBeamProfile->m_pCScans[nScan];

			Cp.Side0.fX = ((float)(pt.x-m_rr[nScan].left) * pScan->m_fFastScanSize / (float)m_rr[nScan].Width()) + (pScan->m_Cp[0].Side0.fX - pScan->m_fFastScanSize/2.0f) ;
			Cp.Side0.fZ = ((float)(pt.y-m_rr[nScan].top) *  pScan->m_fSlowScanSize / (float)m_rr[nScan].Height()) + (pScan->m_Cp[0].Side0.fZ - pScan->m_fSlowScanSize/2.0f) ;
			Cp.Side0.fY = pScan->m_CpCentre.Side0.fY;
	
			m_pProbe->m_Cp[1].Side0.fX = m_pProbe->m_Cp[0].Side0.fX = Cp.Side0.fX;
			m_pProbe->m_Cp[1].Side0.fZ = m_pProbe->m_Cp[0].Side0.fZ = Cp.Side0.fZ;

			if(pParent->m_pProbeToolsSheet)	pParent->m_pProbeToolsSheet->UpdateAllPages();

		}
	}
}

void CProbeBeamProfilePage::RenderAxialGraph(CPaintDC* pDC)
{
	CRect rrView;
	CDC  dcMem;
	CBitmap bmp;
	int	nPtr;
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;
	CString Buff;
	int m_nImageOrientation = 0;
	CPen* pOldPen;
	CRect rr;

	m_staticAxialGraph.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(2,2);
	rr = rrView;
	rr.OffsetRect(-rr.left,-rr.top);

	if(m_pBeamProfile->m_nCScansL<=0) {
		pDC->FillRect(rrView,&CBrush(RGB(255,255,255)));
		return;
	}


	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 14;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);

	char *pArrayView = new char[(rrView.Width()+2) * (rrView.Height()+2) * nBytesPixel];

	bmp.CreateCompatibleBitmap(pDC,rrView.Width(),rrView.Height());
	bmp.SetBitmapBits((DWORD)(rrView.Width()*rrView.Height()*4),pArrayView);
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(RGB(255,128,129));
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(&Font);
	CPen penGreenyBlue(PS_SOLID,1,RGB(0,128,128));
	pOldPen = dcMem.SelectObject(&penGreenyBlue);

	CBrush brushBk(RGB(255,255,255));
	CBrush* pOldBrush = dcMem.SelectObject(&brushBk);


	dcMem.Rectangle(rr);

	CProbeScanData *pScan;
	pScan = &m_pBeamProfile->m_pBScans[0];

	int nSample,nAmp;
	CPoint pt;
	float fTemp;
	if(pScan==NULL) return;
	for(int nLine=0;nLine < pScan->m_nNumberLines;nLine++) {
		fTemp =((m_fAxialX[1] - m_fAxialX[0]) * (float)nLine / (float)(pScan->m_nNumberLines - 1)) + m_fAxialX[0];
		nSample = (int)((float)pScan->m_nSamplesLine * fTemp);
		nPtr = (pScan->m_nSamplesLine * nLine) + nSample;
		nAmp = pScan->m_cAmpBuffer[nPtr];

		pt.x = MulDiv(nAmp,rr.Width(),127) + rr.left;
		pt.y = MulDiv(nLine,rr.Height(),pScan->m_nNumberLines - 1) + rr.top;

		if(nLine==0) {
			dcMem.MoveTo(pt);
		} else {
			dcMem.LineTo(pt);
		}
	}

	pDC->BitBlt(rrView.left,rrView.top,rrView.Width(),rrView.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldBitmap);
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBrush);

	delete pArrayView;




}

void CProbeBeamProfilePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;

	m_staticAxialPlot.GetWindowRect(rr);
	ScreenToClient(rr);

	if(rr.PtInRect(point)) {

		m_bLbDown = true;
		if(point.y < rr.CenterPoint().y) {
			m_nMoveEnd = 0;
		} else {
			m_nMoveEnd = 1;
		}
	}
	
	CResizablePage::OnLButtonDown(nFlags, point);
}

void CProbeBeamProfilePage::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_bLbDown = false;
	CResizablePage::OnLButtonUp(nFlags, point);
}

void CProbeBeamProfilePage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rr;
	if(m_bLbDown != true) return;

	m_staticAxialPlot.GetWindowRect(rr);
	ScreenToClient(rr);

	if(rr.PtInRect(point)) {

		m_fAxialX[m_nMoveEnd] = (float)(point.x - rr.left) / (float)rr.Width();
		Invalidate(false);
	}
	
	CResizablePage::OnMouseMove(nFlags, point);
}
