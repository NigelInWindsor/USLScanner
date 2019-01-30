// MultipleBScansDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MultipleBScansDlg.h"
#include "math.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultipleBScansDlg dialog
#define	TOOL_RULER		1
#define	ZOOM			2


CMultipleBScansDlg::CMultipleBScansDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,LPCTSTR lpszFileName,int nMode)
	: CDialog(CMultipleBScansDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultipleBScansDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_FileName = lpszFileName;
	
	m_nMode = nMode;
	m_bZoom=FALSE;
	if(nMode == REPLAY) {
		m_Data.Retrieve(m_FileName);
	} else {
//		m_Data.OpenForWriting(m_FileName);
	}
	m_nBscan=0;
	m_nContextOption = 0;
	m_nMaxVertical = 1000;
	m_nMinVertical = 0;
	m_pScopeDlg=NULL;

}


void CMultipleBScansDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultipleBScansDlg)
	DDX_Control(pDX, IDC_CHECK_ENVELOPE, m_checkEnvelope);
	DDX_Control(pDX, IDC_SPIN_VELOCITY, m_spinVelocity);
	DDX_Control(pDX, IDC_EDIT_VELOCITY, m_editVelocity);
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
	DDX_Control(pDX, IDC_COMBO_RCTIFY, m_comboRectify);
	DDX_Control(pDX, IDC_SPIN_BSCAN_NUMBER, m_spinBscanNumber);
	DDX_Control(pDX, IDC_EDIT_BSCAN_NUMBER, m_editBscanNumber);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultipleBScansDlg, CDialog)
	//{{AFX_MSG_MAP(CMultipleBScansDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_BSCAN_NUMBER, OnChangeEditBscanNumber)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BSCAN_RULER, OnBscanRuler)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BSCAN_COLOUR, OnBscanColour)
	ON_CBN_SELCHANGE(IDC_COMBO_RCTIFY, OnSelchangeComboRctify)
	ON_COMMAND(ID_BSCAN_ZOOM, OnBscanZoom)
	ON_EN_CHANGE(IDC_EDIT_PROMPT, OnChangeEditPrompt)
	ON_EN_CHANGE(IDC_EDIT_VELOCITY, OnChangeEditVelocity)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_COMMAND(ID_VIEW_RULERCOLOUR, OnViewRulercolour)
	ON_COMMAND(ID_VIEW_OSCILLOSCOPE, OnViewOscilloscope)
	ON_BN_CLICKED(IDC_CHECK_ENVELOPE, OnCheckEnvelope)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultipleBScansDlg message handlers

void CMultipleBScansDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CMultipleBScansDlg::OnClose() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff;
	ASSERT(pFrame);

//	pFrame->AbortScan(); //TODO


	theApp.m_LastSettings.bWindowOpenOnLastExit[CMultipleBScansDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}

void CMultipleBScansDlg::OnDestroy() 
{
	if (CMultipleBScansDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CMultipleBScansDlg::IDD]);
	}

	CDialog::OnDestroy();
	*m_pDlg = NULL;			
}

BOOL CMultipleBScansDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect WindowsRect,rr; 
	GetClientRect(WindowsRect);


//	m_staticView.GetWindowRect(rr);
//	ScreenToClient(rr);
//	rr.right=WindowsRect.right-10;
//	rr.bottom=WindowsRect.bottom-10;
//	m_staticView.SetWindowPos(NULL, rr.left,rr.top, rr.Width(), rr.Height(),SWP_HIDEWINDOW|SWP_NOREDRAW);

	m_staticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	
	if (CMultipleBScansDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CMultipleBScansDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}
	m_staticPosView.SetNewPos();
	m_staticScroll.SetNewPos();

	m_spinBscanNumber.SetRange(0,m_Data.m_nNumberBscans-1);
	m_comboRectify.AddString(_T("FullWave"));
	m_comboRectify.AddString(_T("+HalfWave"));
	m_comboRectify.AddString(_T("-HalfWave"));

	m_spinVelocity.SetRange(1000,7000);

	CalculateScrollBar();

	UpdateAllControls();

	UpdateWindowName();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMultipleBScansDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	//Store Pos and Size of the Dialog
	CRect rect = theApp.m_LastSettings.RectWindowPos[CMultipleBScansDlg::IDD];
	if (CMultipleBScansDlg::IDD < MAXIMUMWINDOWSIDNUMBER && GetSafeHwnd()) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CMultipleBScansDlg::IDD]);
	}

	m_staticPosView.SetNewPos();   
	m_staticScroll.SetNewPos();   

	if (GetSafeHwnd()) {
	
		CRect rect;
		GetClientRect(rect);


		Invalidate(TRUE);
	}
	
}

void CMultipleBScansDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rr;
	CBrush Brush(RGB(255,255,255));
	CRgn rgn;
	CRect rect;
	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 17;
	theApp.SetFontLanguage(&lf);		//Times

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = dc.SelectObject(&Font);
	dc.SetBkMode(TRANSPARENT);
	

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

//	dc.GetClipBox(&rect);
//	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
//	dc.SelectClipRgn(&rgn);

	DrawHorizontalGraticule(&dc,rr,0);
	DrawVerticalGraticule(&dc,rr,0);
	DrawBscan(&dc,rr,0);
	DrawRuler(&dc,rr,0);

//	rgn.SetRectRgn(&rect);
//	dc.SelectClipRgn(&rgn);
	dc.SelectObject(pOldFont);

}


void CMultipleBScansDlg::DrawBscan(CDC *pDC, CRect rr, int nDevice)
{
	CDC  dcMem;
	CBitmap bmp;
	unsigned char	*pArray;
	static int offset=0;
	CFont font;
	char	cColor[256][3];
	unsigned char	cTemp;
	int ii,jj;
	COLORREF Black = RGB(0,0,0);
	COLORREF White = RGB(255,255,255);

	if(m_Data.m_pBscanArray==NULL) return;

	int nOldBscan = m_nBscan;
	if(m_nBscan<0) m_nBscan=0;
	if(m_nBscan>=m_Data.m_nNumberBscans) m_nBscan=m_Data.m_nNumberBscans;

	if(m_nBscan - nOldBscan)
		UpdateAllControls();

	//Convert Palette
	for(ii=0,jj=128;ii<128;ii++,jj++) {
		if((theApp.m_LastSettings.nBscanRectifyMode==0)||(theApp.m_LastSettings.nBscanRectifyMode==1)){
			cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[0][jj]>>16)&0xff);
			cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[0][jj]>>8)&0xff);
			cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[0][jj]>>0)&0xff);
		} else {
			cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[0][128]>>16)&0xff);
			cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[0][128]>>8)&0xff);
			cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[0][128]>>0)&0xff);
		}
	}
	for(ii=0,jj=255;ii<128;ii++,jj--) {
		if((theApp.m_LastSettings.nBscanRectifyMode==0)||(theApp.m_LastSettings.nBscanRectifyMode==2)){
			cColor[ii+128][0]=char((theApp.m_Palette.m_rgbAmplitude[0][jj]>>16)&0xff);
			cColor[ii+128][1]=char((theApp.m_Palette.m_rgbAmplitude[0][jj]>>8)&0xff);
			cColor[ii+128][2]=char((theApp.m_Palette.m_rgbAmplitude[0][jj]>>0)&0xff);
		} else {
			cColor[ii+128][0]=char((theApp.m_Palette.m_rgbAmplitude[0][128]>>16)&0xff);
			cColor[ii+128][1]=char((theApp.m_Palette.m_rgbAmplitude[0][128]>>8)&0xff);
			cColor[ii+128][2]=char((theApp.m_Palette.m_rgbAmplitude[0][128]>>0)&0xff);
		}
	}

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;
	if(nBytesPixel<3) {
		MessageBox(_T("Please reset the screen to at least 24 bit color mode"),_T("Error"),NULL);
	}
		
	if((nDevice==PRINTER)&&(theApp.m_LastSettings.bBWReverse==TRUE)) {
		for(ii=0;ii<256;ii++) {
			if((theApp.m_Palette.m_rgbAmplitude[0][ii]!=Black) && (theApp.m_Palette.m_rgbAmplitude[0][ii]!=White)) {
				cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>16)&0xff);
				cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>8)&0xff);
				cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>0)&0xff);
			}
			if(theApp.m_Palette.m_rgbAmplitude[0][ii]==Black) {
				cColor[ii][0]=(char)0xff;
				cColor[ii][1]=(char)0xff;
				cColor[ii][2]=(char)0xff;
			}
			if(theApp.m_Palette.m_rgbAmplitude[0][ii]==White) {
				cColor[ii][0]=0;
				cColor[ii][1]=0;
				cColor[ii][2]=0;
			}
		}
	}
/*
	if(rr.Width() %4) {
		rr.right=(rr.right-rr.left)/4+rr.left;
		rr.right=(rr.right-rr.left)*4+rr.left;
	}
	*/
	pArray = new unsigned char[(rr.Width()+2) * (rr.Height()+2) * nBytesPixel];
//	memset(pArray,0,rr.Width()*sizeof(COLORREF)*rr.Height());


//	m_Data.BuildBscanArray(m_nBscan);

	int nDest=0;
	int nSrc=0;
	int nPtr=0;

	m_nLine0 = MulDiv(m_nMinVertical,m_Data.m_nNumberLines,1000);
	m_nLine1 = MulDiv(m_nMaxVertical,m_Data.m_nNumberLines,1000);

	for(int yy=0;yy<rr.Height()-2;yy++) { //scanline

		nDest = yy * rr.Width() * nBytesPixel;
		nSrc = (MulDiv(yy,m_nLine1-m_nLine0,rr.Height()-1)+m_nLine0) * m_Data.m_nAscanLength;

/*		if(theApp.m_LastSettings.bEnvelopeOnOff) {
			EnvDetector.CalculateEnvelopes(&m_Data.m_pBscanArray[nSrc],m_Data.m_nAscanLength);

			for(nEnvNum=0;nEnvNum<EnvDetector.m_nNumEnvelopes;nEnvNum++) {
				EnvDetector.m_pointResults[nEnvNum].x += nSrc;
			}
		}
		nEnvNum=0;
*/
		for(int xx=0;xx<rr.Width()-2;xx++) { //pixel in scan line
			try{
			nPtr = MulDiv(xx,m_Data.m_nAscanLength,rr.Width()-1) + nSrc;
			cTemp = m_Data.m_pBscanArray[nPtr];
			
	/*		if( (nPtr>(EnvDetector.m_pointResults[nEnvNum].x-2)) && (nPtr<(EnvDetector.m_pointResults[nEnvNum].x+2)) && theApp.m_LastSettings.bEnvelopeOnOff) {
				pArray[nDest++]=50;
				pArray[nDest++]=50;
				pArray[nDest++]=255;
				nEnvNum++;
			}else{
	*/			pArray[nDest++]=cColor[cTemp][0];
				pArray[nDest++]=cColor[cTemp][1];
				pArray[nDest++]=cColor[cTemp][2];
	//		}
			if(nBytesPixel>3) nDest++;
			}catch(...){}


		}


	}

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;


}

void CMultipleBScansDlg::OnChangeEditBscanNumber() 
{
	CString Buff;

	m_editBscanNumber.GetWindowText(Buff);
	m_nBscan = _ttoi(Buff) - 1;

	m_Data.BuildBscanArray(m_nBscan);
	
	Invalidate(FALSE);

}


void CMultipleBScansDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),m_nBscan+1);
	m_editBscanNumber.SetWindowText(Buff);

	m_spinBscanNumber.SetPos(m_nBscan);

	m_comboRectify.SetCurSel(theApp.m_LastSettings.nBscanRectifyMode%3);

	Buff.Format(_T("%d"),m_Data.m_nMaterialVelocity);
	m_editVelocity.SetWindowText(Buff);
	m_spinVelocity.SetPos(m_Data.m_nMaterialVelocity);
}

void CMultipleBScansDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp,nPage;

	if((nSBCode!=4) && (nSBCode!=8) && (m_bZoom==TRUE)) {
		nPage = m_nMaxVertical - m_nMinVertical;
		nPage=(int)((theApp.m_LastSettings.fBscanZoomLength * 1000.0) / m_Data.m_fScanLength);
		if(nPage>1000) nPage=1000;

		nTemp = m_nMinVertical;
		switch(nSBCode) {
		case SB_LINEDOWN:
			m_nMinVertical+=(nPage/10);
			break;
		case SB_LINEUP:
			 m_nMinVertical-=(nPage/10);
			break;
		case SB_PAGEDOWN:
			m_nMinVertical+=nPage;
			break;
		case SB_PAGEUP:
			m_nMinVertical-=nPage;
			break;
		case SB_THUMBTRACK:
			m_nMinVertical = nPos;
			break;
		}
		if(nTemp - m_nMinVertical) {
			m_nMaxVertical = m_nMinVertical + nPage;
			if(m_nMinVertical < 0) m_nMinVertical = 0;
			if(m_nMaxVertical>1000) {
				m_nMaxVertical=1000;
				m_nMinVertical = m_nMaxVertical - nPage;
				if(m_nMinVertical < 0) m_nMinVertical = 0;
			}
			SetScrollPos(SB_VERT,m_nMinVertical,TRUE);
			Invalidate(FALSE);
		}
	} else {
		if(pSpin!=NULL) {
			switch(pSpin->GetDlgCtrlID()) {
			case IDC_SPIN_BSCAN_NUMBER:
				nTemp=m_nBscan;
				m_nBscan=(pSpin->GetPos()&0xffff);
				if(nTemp-m_nBscan) {
					Buff.Format(_T("%d"),m_nBscan+1);
					m_editBscanNumber.SetWindowText(Buff);
					Invalidate(FALSE);
				}
				break;
			case IDC_SPIN_VELOCITY:
				nTemp = (pSpin->GetPos() & 0xffff);
				if(nTemp - m_Data.m_nMaterialVelocity) {
					m_Data.ChangeVelocityDepth(nTemp);
					theApp.m_LastSettings.nMaterialVelocity = nTemp;
					Buff.Format(_T("%d"),m_Data.m_nMaterialVelocity);
					m_editVelocity.SetWindowText(Buff);
					Invalidate(FALSE);
				}
				break;
			}
		}
	}


	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMultipleBScansDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
/*
	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SCROLLBAR_BSCAN_NUMBER:
		nTemp=m_nBscan;
		m_nBscan=(pScrollBar->GetScrollPos()&0xffff);
		if(nTemp-m_nBscan) {
			Buff.Format("%d",m_nBscan+1);
			m_editBscanNumber.SetWindowText(Buff);
			m_scrollBscanNumber.SetScrollPos(m_nBscan);
			Invalidate(FALSE);
		}
		break;
	}
*/	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMultipleBScansDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	m_ptClient = point;

	if(m_nContextOption==0) {
		if (menu->LoadMenu(IDR_RB_MULTIPLE_BSCAN_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
			delete menu;
		}		
	} else {
		m_nContextOption=0;
	}
}



void CMultipleBScansDlg::DrawHorizontalGraticule(CDC *pDC, CRect rr, int nDisplay)
{
	int	nXp;
	CString Buff;
	CSize size;
	float fStep;
	CRect rectBk;
	COLORREF OldBkColor(RGB(210,210,210));

	rectBk = rr;
	rectBk.bottom = rectBk.top;
	rectBk.top -=18;
	CPen penGrey(PS_SOLID,1,OldBkColor);
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CBrush brushGrey(GetSysColor(COLOR_BTNFACE));
	CPen* pOldPen = pDC->SelectObject(&penGrey);
	CBrush* pOldBrush = pDC->SelectObject(&brushGrey);

	pDC->Rectangle(&rectBk);

	//Horizontal
	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 17;
	lf.lfEscapement = 0;
	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&penBlack);

	fStep = 5.0;
	if(m_Data.m_fDepth <=20.0) fStep = 1.0;
	rr.top+=2;

	for(float fXp=0.0;fXp<m_Data.m_fDepth;fXp+=fStep) {
		nXp = (int)((fXp * (float)rr.Width()) / m_Data.m_fDepth) + rr.left;
		pDC->MoveTo(nXp,rr.top);
		pDC->LineTo(nXp,rr.top-4);
		Buff.Format(_T("%.01f"),fXp);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nXp-size.cx/2,rr.top-4-size.cy,Buff);
	}
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

void CMultipleBScansDlg::DrawVerticalGraticule(CDC *pDC, CRect rr, int nDisplay)
{
	int	nYp;
	CString Buff;
	CSize size;
	CRect rectBk;
	COLORREF OldBkColor(RGB(210,210,210));
	float fStep;

	rr.right += 2;
	rectBk = rr;
	rectBk.right += 20;
	rectBk.left = rr.right;
	rectBk.top -= 17;
	rectBk.bottom += 17;
	CPen penGrey(PS_SOLID,1,OldBkColor);
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CBrush brushGrey(GetSysColor(COLOR_BTNFACE));
	CPen* pOldPen = pDC->SelectObject(&penGrey);
	CBrush* pOldBrush = pDC->SelectObject(&brushGrey);

	pDC->Rectangle(&rectBk);

	//Horizontal
	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 17;
	lf.lfEscapement = 900;
	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&penBlack);

	float	fScanLength = m_Data.m_fScanLength * (float)(m_nMaxVertical-m_nMinVertical) / 1000.0f;
	float   fStart = m_Data.m_fScanLength * (float)m_nMinVertical / 1000.0f;
	if(fScanLength>300.0f) {
		fStep=100.0f;
	} else {
		fStep=10.0f;
	}
	for(float fYp=0.0;fYp<fScanLength;fYp+=fStep) {
		nYp = (int)((fYp * (float)rr.Height()) / fScanLength) + rr.top;
		pDC->MoveTo(rr.right,nYp);
		if(fmod(fYp,50.0)==0) {
			pDC->LineTo(rr.right+7,nYp);
		} else {
			pDC->LineTo(rr.right+4,nYp);
		}
		if(fmod(fYp,100.0)==0) {
			Buff.Format(_T("%.0f"),fYp + fStart + m_Data.m_CpStart.dX0p);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(rr.right+4,nYp+size.cx/2,Buff);
		}
	}
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

}

void CMultipleBScansDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

	if(rr.PtInRect(point)) {
		switch(m_nContextOption) {
		case TOOL_RULER:
			point.x-=rr.left;
			point.y-=rr.top;
			m_ptRulerStart=m_ptRulerFinish=point;
			break;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}



void CMultipleBScansDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

	switch(m_nContextOption) {
	case TOOL_RULER:
		if(rr.PtInRect(point)) {
			point.x-=rr.left;
			point.y-=rr.top;
			m_ptRulerFinish=point;
			Invalidate(FALSE);
		}
		break;
	}

	if((m_pScopeDlg) && (rr.PtInRect(point))) {
		char Trace[6000];
		int nSrc = (MulDiv(point.y-rr.top,m_nLine1-m_nLine0,rr.Height()-1)+m_nLine0) * m_Data.m_nAscanLength;
		try{
			memcpy(Trace,&m_Data.m_pBscanArray[nSrc],m_Data.m_nAscanLength);
//			m_pScopeDlg->RefreshPlayBackTrace(Trace,m_Data.m_nAscanLength);  //TODO

			point.x-=rr.left;
			int nPos = (int)((double)point.x/(double)rr.Width())*m_Data.m_nAscanLength;
//			m_pScopeDlg->DrawVerticalLine(nPos,m_Data.m_nAscanLength); //TODO

		}catch(...){
			//m_pBscanArray dissappeaed?
			CDialog::OnMouseMove(nFlags, point);
			return;
		}	
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CMultipleBScansDlg::DrawRuler(CDC *pDC, CRect rr, int nDevice)
{
	if(m_nContextOption != TOOL_RULER) return;

	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penWhite(PS_SOLID,20,RGB(255,255,255));
	CPen penRuler(PS_SOLID,2,theApp.m_LastSettings.rgbRulerLine);
	CRect	rectRuler;
	CString Buff;
	CSize size;
	int nSize = 10;

	rectRuler.left = m_ptRulerStart.x+rr.left;
	rectRuler.top = m_ptRulerStart.y+rr.top;
	rectRuler.right = m_ptRulerFinish.x+rr.left;
	rectRuler.bottom = m_ptRulerFinish.y+rr.top;

	CPen* pOldPen = pDC->SelectObject(&penRuler);
	pDC->MoveTo(rectRuler.left,rectRuler.top);
	pDC->LineTo(rectRuler.right,rectRuler.top);
	pDC->LineTo(rectRuler.right,rectRuler.bottom);
	pDC->LineTo(rectRuler.left,rectRuler.bottom);
	pDC->LineTo(rectRuler.left,rectRuler.top);
/*
	pDC->SelectObject(&penWhite);
	pDC->MoveTo(rectRuler.left-nSize,rectRuler.top-nSize);
	pDC->LineTo(rectRuler.right+nSize,rectRuler.top-nSize);
	pDC->LineTo(rectRuler.right+nSize,rectRuler.bottom+nSize);
	pDC->LineTo(rectRuler.left-nSize,rectRuler.bottom+nSize);
	pDC->LineTo(rectRuler.left-nSize,rectRuler.top-nSize);

	
	
	if(rectRuler.Width() > 0) {
		memset( (void*)&lf, 0, sizeof(lf));
		lf.lfHeight = 17;
		lf.lfEscapement = 0;
		theApp.SetFontLanguage(&lf);

		CFont fontH;
		fontH.CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(&fontH);
		pDC->SetBkMode(TRANSPARENT);

		pDC->SelectObject(&penBlack);
		float fDepth = ((float)rectRuler.Width() * m_Data.m_fDepth) / (float)rr.Width();
		for(fXp=0.0,ii=0;fXp<fDepth;fXp+=0.1f,ii++) {
			nXp = (int)((fXp * (float)rectRuler.Width()) / fDepth) + rectRuler.left;
			pDC->MoveTo(nXp,rectRuler.top);
			if((ii%10)==0) {
				pDC->LineTo(nXp,rectRuler.top-5);
				Buff.Format("%.01f",fXp);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(nXp-size.cx/2,rectRuler.top-2-size.cy,Buff);
			}
			if((ii%5)==0) {
				pDC->LineTo(nXp,rectRuler.top-5);
			}
			pDC->LineTo(nXp,rectRuler.top-3);
		}
		Buff.Format("%.02fmm ",fDepth);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut((rectRuler.left+rectRuler.right)/2-size.cx/2,rectRuler.bottom,Buff);

		pDC->SelectObject(pOldFont);
	}

	if(rectRuler.Height() > 0) {
		memset( (void*)&lf, 0, sizeof(lf));
		lf.lfHeight = 17;
		lf.lfEscapement = 900;
		theApp.SetFontLanguage(&lf);

		CFont fontV;
		fontV.CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(&fontV);
		pDC->SetBkMode(TRANSPARENT);

		pDC->SelectObject(&penBlack);
		float fScanLength = m_Data.m_fScanLength * (float)(m_nMaxVertical-m_nMinVertical) / 1000.0f;
		fScanLength = ((float)rectRuler.Height() * fScanLength) / (float)rr.Height();

		for(fYp=0.0,ii=0;fYp<fScanLength;fYp+=10.0f,ii++) {
			nYp = (int)((fYp * (float)rectRuler.Height()) / fScanLength) + rectRuler.top;
			pDC->MoveTo(rectRuler.right,nYp);
			if((ii%10)==0) {
				pDC->LineTo(rectRuler.right+5,nYp);
				Buff.Format("%.01f",fYp);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(rectRuler.right+4,nYp+size.cx/2,Buff);
			}
			if((ii%5)==0) {
				pDC->LineTo(rectRuler.right+5,nYp);
			}
				pDC->LineTo(rectRuler.right+3,nYp);
		}
		Buff.Format("%.01fmm",fScanLength);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rectRuler.left-size.cy,(rectRuler.top+rectRuler.bottom)/2+size.cx/2,Buff);

		pDC->SelectObject(pOldFont);
	}
	
*/	
	
	pDC->SelectObject(pOldPen);

	int nStart;
	int nFinish;
	if(rectRuler.top<rectRuler.bottom) {
		nStart=rectRuler.top;
		nFinish=rectRuler.bottom;
	} else {
		nStart=rectRuler.bottom;
		nFinish=rectRuler.top;
	}

	int nLine = MulDiv(nStart-rr.top,m_nLine1-m_nLine0,rr.Height()) + m_nLine0;
	float fStart = m_Data.m_fScanLength * (float)nLine / (float)m_Data.m_nNumberLines + (float)m_Data.m_CpStart.dX0p;
	nLine = MulDiv(nFinish-rr.top,m_nLine1-m_nLine0,rr.Height()) + m_nLine0;
	float fFinish = m_Data.m_fScanLength * (float)nLine / (float)m_Data.m_nNumberLines + (float)m_Data.m_CpStart.dX0p;

	float fDepth = ((float)rectRuler.Width() * m_Data.m_fDepth) / (float)rr.Width();
	float fScanLength = m_Data.m_fScanLength * (float)(m_nMaxVertical-m_nMinVertical) / 1000.0f;
	fScanLength = ((float)rectRuler.Height() * fScanLength) / (float)rr.Height();

	Buff.Format(_T("%.01fmm %.01fmm (%.02fmm * %.01fmm)"),fStart,fFinish,fabs(fDepth),fabs(fScanLength));
	m_editPrompt.SetWindowText(Buff);

}

void CMultipleBScansDlg::OnBscanRuler() 
{
	CRect	rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

	ScreenToClient(&m_ptClient);
	m_ptClient.x-=rr.left;
	m_ptClient.y-=rr.top;
	m_ptRulerStart.x=m_ptClient.x;
	m_ptRulerFinish.x=m_ptClient.x;
	m_ptRulerStart.y=m_ptClient.y;
	m_ptRulerFinish.y=m_ptClient.y;

	m_nContextOption = TOOL_RULER;
}

void CMultipleBScansDlg::OnBscanColour() 
{
	CMFCColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbRulerLine=dlg.GetColor();
		Invalidate(FALSE);
	};
	
}

void CMultipleBScansDlg::OnSelchangeComboRctify() 
{
	theApp.m_LastSettings.nBscanRectifyMode = m_comboRectify.GetCurSel();
	Invalidate(FALSE);
}


void CMultipleBScansDlg::OnScrollHasMovedUslbarctrlVscroll(long LowPosition, long HighPosition, long Width) 
{
//	m_nMinVertical = m_scrollVertical.GetLowPosition();
//	m_nMaxVertical = m_scrollVertical.GetHighPosition();
	Invalidate(FALSE);
	
}

void CMultipleBScansDlg::CalculateScrollBar()
{
	SCROLLINFO	info;
//	m_scrollVertical.SetLowPosition(m_nMinVertical);
//	m_scrollVertical.SetHighPosition(m_nMaxVertical);

	SetScrollRange(SB_VERT,0,1000,FALSE);
	GetScrollInfo(SB_VERT,&info,SIF_ALL);
	info.nPage = m_nMaxVertical - m_nMinVertical;
	SetScrollInfo(SB_VERT,&info,TRUE);


}

void CMultipleBScansDlg::OnBscanZoom() 
{
	CString Buff;

	if(m_bZoom==FALSE) {
		if(theApp.m_LastSettings.fBscanZoomLength<10.0) theApp.m_LastSettings.fBscanZoomLength=10.0;
		if(theApp.m_LastSettings.fBscanZoomLength>3000.0) theApp.m_LastSettings.fBscanZoomLength=3000.0;

		Buff.Format(_T("%.01fmm"),theApp.m_LastSettings.fBscanZoomLength);
		m_editPrompt.SetWindowText(Buff);

		m_nMinVertical=0;
		m_nMaxVertical=(int)((theApp.m_LastSettings.fBscanZoomLength * 1000.0) / m_Data.m_fScanLength);
		if(m_nMaxVertical>1000) m_nMaxVertical=1000;
		m_bZoom=TRUE;
	} else {
		m_nMinVertical=0;
		m_nMaxVertical=1000;
		m_bZoom=FALSE;
	}
	CalculateScrollBar();

	Invalidate(FALSE);

}

void CMultipleBScansDlg::OnChangeEditPrompt() 
{
	CString Buff;

	if(m_nContextOption!=TOOL_RULER) {
		m_editPrompt.GetWindowText(Buff);
		_WTOF(Buff,theApp.m_LastSettings.fBscanZoomLength);
	}

}

void CMultipleBScansDlg::UpdateWindowName()
{
	CString Buff;

	Buff.Format(_T("%s"),m_Data.m_FileName);
	SetWindowText(Buff);

}

void CMultipleBScansDlg::UpdateBscanNumber()
{

}

void CMultipleBScansDlg::OnChangeEditVelocity() 
{
	CString Buff;
	int	nVelocity;

	m_editVelocity.GetWindowText(Buff);
	nVelocity = _ttoi(Buff);
	if(m_Data.m_nMaterialVelocity - nVelocity) {
		m_Data.ChangeVelocityDepth(nVelocity);
		theApp.m_LastSettings.nMaterialVelocity = nVelocity;
		m_spinVelocity.SetPos(m_Data.m_nMaterialVelocity);
		Invalidate(FALSE);
	}

}

void CMultipleBScansDlg::OnFileSaveas() 
{
	CString	FilePath,Buff;
	CFile ActionFile;
	CFileException ExceptionInstance;

	CFileDialog File(TRUE,_T(".vol"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Volume (*.vol)|*.vol|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {

		FilePath.Format(_T("%s"),File.GetPathName());
		if (ActionFile.Open(FilePath, CFile::typeBinary|CFile::modeRead, &ExceptionInstance)) { 
			ActionFile.Close();
			Buff = L"File already exists!:\r\n " + FilePath + L"\r\n Overwrite";
			if (MessageBox(Buff, _T("Overwite"), MB_YESNO) != IDYES) {
				return;
			}
		}

		m_Data.Rename(FilePath);
		UpdateWindowName();
		return;
	}
	return;
	
}

void CMultipleBScansDlg::OnViewZoom() 
{
	OnBscanZoom();
	
}

void CMultipleBScansDlg::OnViewRulercolour() 
{
	OnBscanColour();
}

void CMultipleBScansDlg::OnViewOscilloscope() 
{
	if (m_pScopeDlg == NULL) {
		m_pScopeDlg = new CScopeDlg(this,(CWnd**)&m_pScopeDlg);
		m_pScopeDlg->Create(CScopeDlg::IDD, this);	
		theApp.m_LastSettings.bWindowOpenOnLastExit[CScopeDlg::IDD]=TRUE;
		m_pScopeDlg->SetRfInputType(RFTYPELINEAR);
	} else {
		m_pScopeDlg->SendMessage(WM_CLOSE);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CScopeDlg::IDD]=FALSE;
	}
		
}

void CMultipleBScansDlg::OnCheckEnvelope() 
{
/*
	theApp.m_LastSettings.bEnvelopeOnOff=m_checkEnvelope.GetCheck()&1;
	Invalidate(FALSE);
*/	
}
