// ThicknessPalettePage1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ThicknessPalettePage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThicknessPalettePage property page

IMPLEMENT_DYNCREATE(CThicknessPalettePage, CResizablePage)

CThicknessPalettePage::CThicknessPalettePage() : CResizablePage(CThicknessPalettePage::IDD)
{
	//{{AFX_DATA_INIT(CThicknessPalettePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nWhich = 0;
	m_pFont=NULL;

}

CThicknessPalettePage::~CThicknessPalettePage()
{
	SAFE_DELETE( m_pFont );
}

void CThicknessPalettePage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThicknessPalettePage)
	DDX_Control(pDX, IDC_STATIC_4, m_static4);
	DDX_Control(pDX, IDC_STATIC_3, m_static3);
	DDX_Control(pDX, IDC_STATIC_2, m_static2);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_0, m_static0);
	DDX_Control(pDX, IDC_SPIN_NUMBER_OF_ENTRIES, m_spinNumberOfEntries);
	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_ENTRIES, m_editNumberOfEntries);
	DDX_Control(pDX, IDC_SPIN_RANGE, m_spinRange);
	DDX_Control(pDX, IDC_EDIT_RANGE, m_editRange);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_EDIT_MIN, m_editMin);
	DDX_Control(pDX, IDC_SPIN_STEPSIZE, m_spinStepSize);
	DDX_Control(pDX, IDC_SPIN_MIN, m_spinMin);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE, m_editStepSize);
	DDX_Control(pDX, IDC_STATIC_BAR_GRAPH, m_staticBarGraph);
	DDX_Control(pDX, IDC_STATIC_ENTRIES, m_staticEntries);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThicknessPalettePage, CResizablePage)
	//{{AFX_MSG_MAP(CThicknessPalettePage)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE, OnChangeEditStepsize)
	ON_EN_CHANGE(IDC_EDIT_MIN, OnChangeEditMin)
	ON_EN_CHANGE(IDC_EDIT_RANGE, OnChangeEditRange)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_OF_ENTRIES, OnChangeEditNumberOfEntries)
	ON_WM_HELPINFO()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RANGE, OnDeltaposSpinRange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEPSIZE, OnDeltaposSpinStepsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MIN, OnDeltaposSpinMin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThicknessPalettePage message handlers

BOOL CThicknessPalettePage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	GetClientRect(m_rrClient);

	AddAnchor(&m_comboMode);

	AddAnchor(&m_editMin);
	AddAnchor(&m_editStepSize);
	AddAnchor(&m_editRange);
	AddAnchor(&m_editNumberOfEntries);

	AddAnchor(&m_spinNumberOfEntries);
	AddAnchor(&m_spinStepSize);
	AddAnchor(&m_spinMin);
	AddAnchor(&m_spinRange);

	AddAnchor(&m_staticBarGraph);
	AddAnchor(&m_staticEntries);

	AddAnchor(&m_static0);
	AddAnchor(&m_static1);
	AddAnchor(&m_static2);
	AddAnchor(&m_static3);
	AddAnchor(&m_static4);

	m_comboMode.AddString(_T("Linear"));
	m_comboMode.AddString(_T("Block"));

	m_spinStepSize.SetRange(0,6000);
	m_spinMin.SetRange(0,6000);
	m_spinRange.SetRange(0,6000);
	m_spinNumberOfEntries.SetRange(1,100);

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);

	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CThicknessPalettePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CThicknessPalettePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	CString	Buff;
	

	m_comboMode.SetCurSel(theApp.m_Palette.m_nModeThickness[m_nWhich]);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,theApp.m_Palette.m_dMinThickness[m_nWhich]*theApp.m_fUnits,theApp.m_Units);
	m_editMin.SetWindowText(Buff);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,theApp.m_Palette.m_dStepSize[m_nWhich]*theApp.m_fUnits,theApp.m_Units);
	m_editStepSize.SetWindowText(Buff);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,theApp.m_Palette.m_dThicknessRange[m_nWhich]*theApp.m_fUnits,theApp.m_Units);
	m_editRange.SetWindowText(Buff);

	Buff.Format(_T("%d"),theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]);
	m_editNumberOfEntries.SetWindowText(Buff);

	SetAccessPrivelage();
}

void CThicknessPalettePage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CThicknessPalettePage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr,r;
	LOGFONT	lf;
	CString Buff;

	if(GetSafeHwnd() && m_comboMode.GetSafeHwnd()) {
		
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);

		m_comboMode.SetFont(m_pFont);

		m_editMin.SetFont(m_pFont);
		m_editStepSize.SetFont(m_pFont);
		m_editRange.SetFont(m_pFont);
		m_editNumberOfEntries.SetFont(m_pFont);

		m_spinNumberOfEntries.SetFont(m_pFont);
		m_spinStepSize.SetFont(m_pFont);
		m_spinMin.SetFont(m_pFont);
		m_spinRange.SetFont(m_pFont);

		m_staticBarGraph.SetFont(m_pFont);
		m_staticEntries.SetFont(m_pFont);

		m_static0.SetFont(m_pFont);
		m_static1.SetFont(m_pFont);
		m_static2.SetFont(m_pFont);
		m_static3.SetFont(m_pFont);
		m_static4.SetFont(m_pFont);

		Invalidate(FALSE);
		UpdateAllControls();
	}

}

void CThicknessPalettePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_NUMBER_OF_ENTRIES:
		nTemp = theApp.m_Palette.m_nThicknessEntriesl[m_nWhich];
		theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]) {
			Buff.Format(_T("%d"),theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]);
			m_editNumberOfEntries.SetWindowText(Buff);
			Invalidate(FALSE);
		}
		break;
	};		

	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CThicknessPalettePage::OnPaint() 
{
	CRect	rr;
	CPaintDC dc(this); // device context for painting
	
	RefreshEntries(&dc);

	m_staticBarGraph.GetWindowRect(rr);
	ScreenToClient(rr);
	RefreshBarGraph(&dc,rr);

}

void CThicknessPalettePage::RefreshEntries(CPaintDC *pDC)
{
	int	ii,yy,y1;
	CRect rr,rectBox;
	CBrush* pOldBrush;
	CBrush* pBrush;

	m_staticEntries.GetWindowRect(rr);
	ScreenToClient(rr);

	for(ii=0;ii<theApp.m_Palette.m_nThicknessEntriesl[m_nWhich];ii++) {
		yy=MulDiv(ii,rr.Height(),theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]) + rr.top;
		y1=MulDiv(ii+1,rr.Height(),theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]) + rr.top;
		pBrush = new CBrush;
		if(theApp.m_Palette.m_Thickness[m_nWhich][ii].bSet==TRUE) {
			pBrush->CreateSolidBrush(theApp.m_Palette.m_Thickness[m_nWhich][ii].rgbValue);
		} else {
			pBrush->CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		};
		pOldBrush=pDC->SelectObject(pBrush);

		rectBox.SetRect(rr.left,yy,rr.right,y1);
		pDC->Rectangle(rectBox);

		pDC->SelectObject(pOldBrush);
		delete pBrush;
	}
}

void CThicknessPalettePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMFCColorDialog dlg;
	int	ii;
	CRect rr;
	m_staticEntries.GetWindowRect(rr);
	ScreenToClient(rr);

	if(rr.PtInRect(point)) {
		ii = MulDiv(point.y-rr.top,theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]-1,rr.Height());
		if(theApp.m_Palette.m_Thickness[m_nWhich][ii].bSet==TRUE) {
			theApp.m_Palette.m_Thickness[m_nWhich][ii].bSet=FALSE;
		} else {
			if(dlg.DoModal()==IDOK) {
				theApp.m_Palette.m_Thickness[m_nWhich][ii].bSet=TRUE;
				theApp.m_Palette.m_Thickness[m_nWhich][ii].rgbValue=dlg.GetColor();
			};
		};
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		SendMessage(WM_PAINT);
	};
	CResizablePage::OnLButtonDown(nFlags, point);
}

void CThicknessPalettePage::OnSelchangeComboMode() 
{
	theApp.m_Palette.m_nModeThickness[m_nWhich]=m_comboMode.GetCurSel();
	theApp.m_Palette.CalculatePalette();
	Invalidate(FALSE);
	SendMessage(WM_PAINT);
}



void CThicknessPalettePage::RefreshBarGraph(CPaintDC *pDC,CRect rr)
{
	CRect	bar;
	CBrush* pOldBrush;
	CBrush* pBrush;
	CPen*	pOldPen;
	int		xx,yy,nColor,nPny;
	static	bool bFlag=FALSE;
	CString Buff;
	double	dStep;
	int	nRange=(int)(theApp.m_Palette.m_dThicknessRange[0] * 100.0);
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	bar = rr;
	rr.DeflateRect(2,2,2,2);
	bar.top+=10;
	bar.bottom-=10;
	bar.left+=4;
	bar.right=bar.left+8;

	pBrush = new CBrush;
	pBrush->CreateSolidBrush(RGB(255,255,255));
	pOldBrush=pDC->SelectObject(pBrush);

	if(bFlag==FALSE) {
		pDC->Rectangle(rr);
	//	bFlag=TRUE;
	};

	CDC  dcMem;
	CBitmap bmp;
	char	*pArray,*pBuffer;
	pArray = new char[(bar.Width()+2) * (bar.Height()+2) * 4];

	for(yy=0;yy<bar.Height();yy++) {
		pBuffer=&pArray[((bar.Height()-1-yy)*bar.Width()) * nBytesPixel];

		nColor=MulDiv(yy,nRange,bar.Height())+theApp.m_Palette.m_nMinThicknessNumber[0];
		if(nColor>theApp.m_Palette.m_nMaxThicknessNumber[0]) nColor=theApp.m_Palette.m_nMaxThicknessNumber[0];
		for(xx=0;xx<bar.Width();xx++) {
			*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>16)&0xff);
			*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>8)&0xff);
			*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>0)&0xff);
			if(nBytesPixel>3) pBuffer++;
		};
	};

	bmp.CreateCompatibleBitmap(pDC,bar.Width(),bar.Height());
	bmp.SetBitmapBits((DWORD)(bar.Width()*bar.Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pDC->BitBlt(bar.left,bar.top,bar.Width(),bar.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;
/*

	for(gg=0;gg<10;gg++) {
		for(yy=gg;yy<=bar.Height();yy+=10) {
			nColor=MulDiv(yy,nRange,bar.Height())+theApp.m_Palette.m_nMinThicknessNumber;
			if(nColor>theApp.m_Palette.m_nMaxThicknessNumber) nColor=theApp.m_Palette.m_nMaxThicknessNumber;
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,theApp.m_Palette.m_rgbThickness[nColor]);
			pOldPen=pDC->SelectObject(pPen);
			pDC->MoveTo(bar.left,bar.bottom-yy);
			pDC->LineTo(bar.right,bar.bottom-yy);
			pDC->SelectObject(pOldPen);
			delete pPen;
		};
	};

*/

	CSize size;
	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 12;
	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);

	CPen BlackPen(PS_SOLID,0,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&BlackPen);
	if(theApp.m_Palette.m_dStepSize[0]<=0.0) theApp.m_Palette.m_dStepSize[0] = theApp.m_Palette.m_dThicknessRange[0] / 10.0;
	for(dStep=0;dStep<=theApp.m_Palette.m_dThicknessRange[0];dStep+=theApp.m_Palette.m_dStepSize[0]) {
		nPny=bar.bottom-(int)((dStep*(double)bar.Height())/theApp.m_Palette.m_dThicknessRange[0]);
		pDC->MoveTo(bar.right+2,nPny);
		pDC->LineTo(bar.right+3,nPny);
		Buff.Format(_T("%.02f"),(dStep+theApp.m_Palette.m_dMinThickness[0]) * theApp.m_fUnits);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(bar.right+3,nPny-size.cy/2,Buff);
	};
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	delete pBrush;

}




void CThicknessPalettePage::UpdateImages()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);

	for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
//		if(pFrame->m_pViewDlg[nn]!=NULL) {
//			pFrame->m_pViewDlg[nn]->Invalidate(TRUE);
//		};
	};

}


void CThicknessPalettePage::OnChangeEditNumberOfEntries() 
{
	CString Buff;
	int nTemp=theApp.m_Palette.m_nThicknessEntriesl[m_nWhich];

	m_editNumberOfEntries.GetWindowText(Buff);
	theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]=_ttoi(Buff);
	if(nTemp-theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]) {
		m_spinNumberOfEntries.SetPos(theApp.m_Palette.m_nThicknessEntriesl[m_nWhich]);
	}
}

BOOL CThicknessPalettePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPaletteTab = CThicknessPalettePage::IDD;
	UpdateAllControls();

	return CResizablePage::OnSetActive();
}

BOOL CThicknessPalettePage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("PaletteDlg.html"),NULL);
	
	return CResizablePage::OnHelpInfo(pHelpInfo);
}

void CThicknessPalettePage::OnChangeEditRange() 
{
	double dTemp;
	CString	Buff;

	m_editRange.GetWindowText(Buff);
	_WTOF(Buff,dTemp);
	dTemp /= theApp.m_fUnits;
	if(theApp.m_Palette.m_dThicknessRange[0] - dTemp) {
		theApp.m_Palette.m_dThicknessRange[0] = dTemp;
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		UpdateImages();
	}
}

void CThicknessPalettePage::OnDeltaposSpinRange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_Palette.m_dThicknessRange[m_nWhich] += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		theApp.m_Palette.m_dThicknessRange[m_nWhich] += ((float)pNMUpDown->iDelta * 0.254f);
	}
	if(theApp.m_Palette.m_dThicknessRange[m_nWhich]<=0.0) theApp.m_Palette.m_dThicknessRange[m_nWhich]=0.1;
	theApp.m_Palette.CalculatePalette();
	UpdateAllControls();
	Invalidate(FALSE);
	UpdateImages();
	
	*pResult = 0;
}

void CThicknessPalettePage::OnChangeEditStepsize() 
{
	CString	Buff;
	double dTemp;

	m_editStepSize.GetWindowText(Buff);
	_WTOF(Buff,dTemp);
	dTemp /= theApp.m_fUnits;
	if(theApp.m_Palette.m_dStepSize[m_nWhich] - dTemp) {
		theApp.m_Palette.m_dStepSize[m_nWhich] = dTemp;
		if(theApp.m_Palette.m_dStepSize[m_nWhich]<=0.0) theApp.m_Palette.m_dStepSize[m_nWhich]=0.1;
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		UpdateImages();
	}
}

void CThicknessPalettePage::OnDeltaposSpinStepsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_Palette.m_dStepSize[m_nWhich] += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		theApp.m_Palette.m_dStepSize[m_nWhich] += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	if(theApp.m_Palette.m_dStepSize[m_nWhich]<=0.0) theApp.m_Palette.m_dStepSize[m_nWhich]=0.1;
	UpdateAllControls();
	Invalidate(FALSE);
	UpdateImages();
	
	*pResult = 0;
}

void CThicknessPalettePage::OnChangeEditMin() 
{
	double dTemp;
	CString	Buff;

	m_editMin.GetWindowText(Buff);
	_WTOF(Buff,dTemp);
	dTemp /= theApp.m_fUnits;
	if(theApp.m_Palette.m_dMinThickness[m_nWhich] - dTemp) {
		theApp.m_Palette.m_dMinThickness[m_nWhich] = dTemp;
		if(theApp.m_Palette.m_dMinThickness[m_nWhich]<0.0) theApp.m_Palette.m_dMinThickness[m_nWhich]=0.0;
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		UpdateImages();
	}
}

void CThicknessPalettePage::OnDeltaposSpinMin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_Palette.m_dMinThickness[m_nWhich] += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		theApp.m_Palette.m_dMinThickness[m_nWhich] += ((float)pNMUpDown->iDelta * 0.254f);
	}
	if(theApp.m_Palette.m_dMinThickness[m_nWhich]<0.0) theApp.m_Palette.m_dMinThickness[m_nWhich]=0.0;

	UpdateAllControls();
	Invalidate(FALSE);
	UpdateImages();
	
	*pResult = 0;
}

void CThicknessPalettePage::SetAccessPrivelage()
{
	bool bFlag = false;
	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_PALETTE_THICK_DIALOG1]) {
		bFlag = true;
	}

	m_spinNumberOfEntries.EnableWindow(bFlag);
	m_editNumberOfEntries.EnableWindow(bFlag);
	m_spinRange.EnableWindow(bFlag);
	m_editRange.EnableWindow(bFlag);
	m_comboMode.EnableWindow(bFlag);
	m_editMin.EnableWindow(bFlag);
	m_spinStepSize.EnableWindow(bFlag);
	m_spinMin.EnableWindow(bFlag);
	m_editStepSize.EnableWindow(bFlag);
	m_staticBarGraph.EnableWindow(bFlag);
	m_staticEntries.EnableWindow(bFlag);

}
