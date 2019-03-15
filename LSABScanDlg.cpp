// LSABScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "MainFrm.h"
#include "LSABScanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	LSA_INVERT_BSCAN		0x0001
#define	LSA_POSITIVE_HW_BSCAN	0x0002
#define	LSA_NEGATIVE_HW_BSCAN	0x0004
#define	LSA_SMOOTH_BSCAN		0x0008
#define	LSA_HIGHLIGHT_LINE		0x0010
#define	LSA_DISPLAY_GATES		0x0020
#define	LSA_DISPLAY_TOF			0x0040
#define	LSA_DISPLAY_SCALE		0x0080
#define	LSA_DISPLAY_DELTA_WP	0x0100
#define	LSA_DISPLAY_FRAME_RATE	0x0200

/////////////////////////////////////////////////////////////////////////////
// CLSABScanDlg dialog

extern int CompareUnsignedChar(const void *Cp0, const void *Cp1);
extern UINT RenderBScan(LPVOID pParam);

CLSABScanDlg::CLSABScanDlg(CWnd* pParent /*=NULL*/, CWnd** pDlg)
	: CDialog(CLSABScanDlg::IDD, pParent)
{
	m_pDlg = pDlg;

	//{{AFX_DATA_INIT(CLSABScanDlg)
	//}}AFX_DATA_INIT
	m_bDisplayTOF = false;
	m_pRGBArray = NULL;

}



void CLSABScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLSABScanDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_STATIC_SCALE, m_staticScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLSABScanDlg, CDialog)
	//{{AFX_MSG_MAP(CLSABScanDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_LSA_HIGHLIGHT, OnButtonLsaHighlight)
	ON_COMMAND(ID_BUTTON_LSA_INVERT, OnButtonLsaInvert)
	ON_COMMAND(ID_BUTTON_LSA_FULLWAVE, OnButtonLsaFullwave)
	ON_COMMAND(ID_BUTTON_LSA_POSITIVE_HW, OnButtonLsaPositiveHw)
	ON_COMMAND(ID_BUTTON_LSA_NEGATIVE_HW, OnButtonLsaNegativeHw)
	ON_COMMAND(ID_BUTTON_LSA_GATES, OnButtonLsaGates)
	ON_COMMAND(ID_BUTTON_LSA_SMOOTH, OnButtonLsaSmooth)
	ON_COMMAND(ID_BUTTON_LSA_TOF, OnButtonLsaTof)
	ON_COMMAND(ID_BUTTON_LSA_SCALE, OnButtonLsaScale)
	ON_COMMAND(ID_BUTTON_DELTA_WP, OnButtonDeltaWp)
	ON_COMMAND(ID_BUTTON_FRAME_RATE, OnButtonFrameRate)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
int CLSABScanDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_LSA_BSCAN_TOOLBAR)) {
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

// CLSABScanDlg message handlers
void CLSABScanDlg::PostNcDestroy() 
{
	SAFE_DELETE(m_pRGBArray);
	
	CDialog::PostNcDestroy();
	delete this;
}

void CLSABScanDlg::OnClose()
{
	theApp.SuspendThread(this);

	theApp.m_LastSettings.bWindowOpenOnLastExit[CLSABScanDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=FALSE;
	CDialog::OnClose();
	DestroyWindow();
}

void CLSABScanDlg::OnDestroy()
{
	theApp.SuspendThread(this);
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CLSABScanDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);

	SAFE_DELETE(m_pRGBArray);


	CDialog::OnDestroy();

	*m_pDlg = NULL;
}

BOOL CLSABScanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosScale.Initialize(this, m_staticScale, TRUE, FALSE, TRUE, TRUE);

	CRect rect = theApp.m_LastSettings.RectWindowPos[CLSABScanDlg::IDD % MAXIMUMWINDOWSIDNUMBER];
	theApp.ConfineToScreen(&rect);
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	};

	m_StaticPosView.SetNewPos();
	m_StaticPosScale.SetNewPos();

	int nScanPitch = theApp.m_PhasedArray[PORTSIDE].getIntTxFocalLawPitch();
	int nNumberFocalLaws = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();
	theApp.m_LSA.ApplyFocalLaws(nNumberFocalLaws, theApp.m_PhasedArray[PORTSIDE].getTxAperture(), theApp.m_PhasedArray[PORTSIDE].getTxFirstElement(), false, 0, nScanPitch, theApp.m_PhasedArray[PORTSIDE].getElementPitch());
	theApp.m_PhasedArray[PORTSIDE].setNumberFocalLaws(nNumberFocalLaws);
//	theApp.m_PhasedArray[PORTSIDE].CalculateFocalLawVertices();

	m_nRectification=0;

	CalculatePalette();

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	theApp.StartThread(L"LSA B-Scan Dlg", RenderBScan, this, 100, THREAD_PRIORITY_BELOW_NORMAL);

	SetScalePosition();
	Invalidate(TRUE);

	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT RenderBScan(LPVOID pParam)
{
	CLSABScanDlg* pParent = (CLSABScanDlg*)pParam;

	if (WaitForSingleObject(pParent->m_hSemaphore, 10) == WAIT_OBJECT_0) {

		if (pParent->GetSafeHwnd() && pParent->IsWindowVisible() == TRUE) {
			pParent->DrawBScan();
		}

		ReleaseSemaphore(pParent->m_hSemaphore, 1, NULL);
	}

	return THREAD_CONTINUE;
}


void CLSABScanDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
}


void CLSABScanDlg::DrawBScan()
{
	if (GetSafeHwnd() == NULL) return;
	if(theApp.m_LSA.m_nActualLawsL<=0) theApp.m_LSA.m_nActualLawsL =1;

	CRect rr;
	CDC* pDC = GetDC();
	signed char	*pData = theApp.m_LSA.m_AscanData;
	CBitmap bmp;
	CDC  dcMem;
	int ii, nFL,yy,nDeltaY;
	int nTS,nX,nY0,nY1,nSrc,nPnY;
	int nAscanLength = theApp.m_UtUser.m_TS[0].Adc.nAcqLength;
	int	nHighY0 = 0,nHighY1 = 1;
	CSize size;
	CString Buff;

	if (pDC == NULL) return;

	CalculatePalette();

	if((nAscanLength>MAX_ASCAN_LENGTH) || (nAscanLength<0))
		nAscanLength=0;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);


	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;
	if(nBytesPixel<3) {
//		MessageBox("Please reset the screen to at least 24 bit color mode","Error",NULL);
	}
		
	if (m_rectArray.Width() != rr.Width() || m_rectArray.Height() != rr.Height()) {
		SAFE_DELETE(m_pRGBArray);
		m_pRGBArray = new COLORREF[(rr.Width() + 2) * (rr.Height() + 2)];
		m_rectArray = rr;
	}
	int nArraySize = rr.Width() * rr.Height();

	int m_nNumberLines = theApp.m_LSA.m_nActualLawsL;
	int m_nSamplesLine = nAscanLength;
	int n3DFilter = 0;
	int nColor;

	signed char *pSrc = theApp.m_LSA.m_AscanData;

	int nDest=0, LineNum=0,  nOldPtr=0, nPtr=0;
	long loffset=0;
	unsigned char cTemp;
	int	nTemp1,nTemp0;
	int nHighLightLine = 0;
	if(theApp.m_LastSettings.nLSAMask & LSA_HIGHLIGHT_LINE) {
		if(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) {
			nHighLightLine = theApp.m_LSA.m_nActualLawsL-theApp.m_LSA.m_nScopeViewLaw-1;
		} else {
			nHighLightLine = theApp.m_LSA.m_nScopeViewLaw;
		}
	}


	nDeltaY = MulDiv(1, m_rectArray.Height()-1,theApp.m_LSA.m_nActualLawsL);
	int* pnPtr = new int[m_rectArray.Width()];
	for (int xx = 0; xx < m_rectArray.Width(); xx++) {
		pnPtr[xx] = MulDiv(xx, nAscanLength, m_rectArray.Width() - 1);
	}
	
	if(theApp.m_LastSettings.nLSAMask & LSA_SMOOTH_BSCAN) {
		nY0=0;
		for(nFL=0;nFL<theApp.m_LSA.m_nActualLawsL;nFL++) {

			nY1 = MulDiv(nFL+1, m_rectArray.Height()-(nDeltaY/2),theApp.m_LSA.m_nActualLawsL);
			if(nFL<(theApp.m_LSA.m_nActualLawsL-1)) {
				nY1 += (nDeltaY/2);
			} else {
				nY1= m_rectArray.Height()-1;
			}
			nSrc = nFL * MAX_ASCAN_LENGTH;
			if(nHighLightLine == nFL) {
				nHighY0 = nY0;
				nHighY1 = nY1;
			};
			int nDenominator = nY1 - nY0;
			for(yy=0;yy<nDenominator;yy++) {
				if(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) {
					nDest = ((m_rectArray.Height()-1)-(yy+nY0)) * m_rectArray.Width();
				} else {
					nDest= (yy+nY0) * m_rectArray.Width();
				}
				if (nDest + m_rectArray.Width() < nArraySize) {
					DWORD *pDest = (DWORD*)&m_pRGBArray[nDest];
					for (int xx = 0; xx < m_rectArray.Width(); xx++) {
						nPtr = pnPtr[xx] + nSrc;
						if (nFL < (theApp.m_LSA.m_nActualLawsL - 1)) {
							nTemp0 = pSrc[nPtr];
							nTemp1 = pSrc[nPtr + MAX_ASCAN_LENGTH];
							nColor = MulDiv(yy, (nTemp1 - nTemp0), nDenominator) + nTemp0;
						}
						else {
							nColor = pSrc[nPtr];
						}
						DWORD *pColor = (DWORD*)&m_cColor[abs(nColor)][0];
						*pDest++ = *pColor;
					}
				}
			}
			nY0 = nY1;
		}
	} else {
		nY0=0;
		for(nFL=0;nFL<theApp.m_LSA.m_nActualLawsL;nFL++) {

			nY1 = MulDiv(nFL+1, m_rectArray.Height()-1,theApp.m_LSA.m_nActualLawsL);
			nSrc = nFL * MAX_ASCAN_LENGTH;
			if(nHighLightLine == nFL) {
				nHighY0 = nY0;
				nHighY1 = nY1;
			};

			for(yy=nY0;yy<nY1;yy++) {
				if(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) {
					nDest = ((m_rectArray.Height()-1)-yy) * m_rectArray.Width();
				} else {
					nDest= yy * m_rectArray.Width();
				}
				if (nDest + m_rectArray.Width() < nArraySize) {
					DWORD *pDest = (DWORD*)&m_pRGBArray[nDest];
					for (int xx = 0; xx < m_rectArray.Width(); xx++) {
						nPtr = pnPtr[xx] + nSrc;
						cTemp = pSrc[nPtr];
						DWORD *pColor = (DWORD*)&m_cColor[cTemp][0];
						*pDest++ = *pColor;
					}
				}
			}
			nY0 = nY1;
		}
	}
	delete pnPtr;

	bmp.CreateCompatibleBitmap(pDC, m_rectArray.Width(), m_rectArray.Height());
	bmp.SetBitmapBits((DWORD)(m_rectArray.Width()*m_rectArray.Height()*nBytesPixel), m_pRGBArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen* pOldPen = dcMem.SelectObject(&pen);


	if(theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_GATES) DrawGates(&dcMem,rr);
	if(theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_DELTA_WP) DrawDeltaWp(&dcMem,rr);
	if(theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_FRAME_RATE) DrawFrameRate(&dcMem,rr);

	if(theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_TOF) {
		CPoint* pPt = new CPoint[rr.Height()];

		for(yy=0;yy<rr.Height();yy++) {
			if(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) {
				pPt[yy].y = rr.Height() - yy - 1;
			} else {
				pPt[yy].y = yy;
			}

			nTS = MulDiv(yy,theApp.m_LSA.m_nActualLawsL-1,rr.Height());
			nX = MulDiv(theApp.m_UtUser.m_TS[nTS].Dsp.TimsStartPos[0],rr.Width(),nAscanLength);
			
			pPt[yy].x = nX;
		}
		dcMem.Polyline(pPt,rr.Height());
		for(yy=0;yy<rr.Height();yy++) {
			nTS = MulDiv(yy,theApp.m_LSA.m_nActualLawsL-1,rr.Height());
			nX = MulDiv(theApp.m_UtUser.m_TS[nTS].Dsp.TimsStopPos[0],rr.Width(),nAscanLength);
			pPt[yy].x = nX;
		}
		dcMem.Polyline(pPt,rr.Height());
		delete pPt;
	}

	if(theApp.m_LastSettings.nLSAMask & LSA_HIGHLIGHT_LINE) {
		CPen penGrey(PS_SOLID,1,RGB(200,200,200));
		dcMem.SelectObject(&penGrey);
		dcMem.MoveTo(0,nHighY0);
		dcMem.LineTo(rr.Width()-1,nHighY0);
		dcMem.MoveTo(0,nHighY1);
		dcMem.LineTo(rr.Width()-1,nHighY1);
		Buff.Format(L"%d",theApp.m_LSA.m_nScopeViewLaw+1);
		size = dcMem.GetTextExtent(Buff);
		dcMem.SetTextColor(RGB(200,200,200));
		dcMem.SetBkMode(TRANSPARENT);
		dcMem.TextOut(0,(nHighY0+nHighY1)/2-(size.cy/2),Buff);
	}

	
	dcMem.SelectObject(pOldPen);

	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	//Scale
	m_staticScale.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.Width() > 1) {

		CBrush BrushWhite(RGB(255,255,255));
		pDC->SelectObject(BrushWhite);
		pDC->Rectangle(rr.left,rr.top,rr.right,rr.bottom);

		LOGFONT lf;
		memset( (void*)&lf, 0, sizeof(lf));
		lf.lfHeight = 12;
		theApp.SetFontLanguage(&lf);

		CFont Font;
		Font.CreateFontIndirect(&lf);
		CFont* pOldFont = pDC->SelectObject(&Font);
		pDC->SetBkMode(TRANSPARENT);

		CPen BlackPen(PS_SOLID,0,RGB(0,0,0));
		pDC->SelectObject(&BlackPen);
		Buff = L"1";
		size = pDC->GetTextExtent(Buff);


		ii = size.cy / (int)((double)rr.Height()/(double)theApp.m_LSA.m_nActualLawsL);
		for( ; ii<theApp.m_LSA.m_nActualLawsL; ii++) {
			if(!(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN)) {
				Buff.Format(_T("%d-"),ii+1);
			} else {
				Buff.Format(_T("%d-"),theApp.m_LSA.m_nActualLawsL - ii);
			}
			size = pDC->GetTextExtent(Buff);
			if(ii==theApp.m_LSA.m_nScopeViewLaw) {
				pDC->SetTextColor(RGB(255,0,0));
			} else {
				pDC->SetTextColor(RGB(128,128,128));
			}
			nPnY = rr.top + MulDiv(ii,rr.Height()-1-nDeltaY,theApp.m_LSA.m_nActualLawsL-1) + (nDeltaY/2) - (size.cy/2);
			if((nPnY + size.cy) < rr.bottom) pDC->TextOut(rr.left+3,nPnY,Buff);

			ii+= size.cy / (int)((double)rr.Height()/(double)theApp.m_LSA.m_nActualLawsL);
		}
	}
	ReleaseDC(pDC);
}


void CLSABScanDlg::CalculatePalette()
{
	int ii, jj;

	for (ii = 0, jj = 128; ii<128; ii++, jj += 1) {
		if (m_nRectification == 0 || m_nRectification == 1) {
			m_cColor[ii][0] = char((theApp.m_Palette.m_rgbAmplitude[0][jj] >> 16) & 0xff);
			m_cColor[ii][1] = char((theApp.m_Palette.m_rgbAmplitude[0][jj] >> 8) & 0xff);
			m_cColor[ii][2] = char((theApp.m_Palette.m_rgbAmplitude[0][jj] >> 0) & 0xff);
			m_cColor[ii][3] = 0;
		}
		else {
			m_cColor[ii][0] = char((theApp.m_Palette.m_rgbAmplitude[0][128] >> 16) & 0xff);
			m_cColor[ii][1] = char((theApp.m_Palette.m_rgbAmplitude[0][128] >> 8) & 0xff);
			m_cColor[ii][2] = char((theApp.m_Palette.m_rgbAmplitude[0][128] >> 0) & 0xff);
			m_cColor[ii][3] = 0;
		}
	}


	//Unsigned -ve 128-255
	for (ii = 0, jj = 255; ii<128; ii++, jj -= 1) {
		if (m_nRectification == 0 || m_nRectification == 2) {
			m_cColor[ii + 128][0] = char((theApp.m_Palette.m_rgbAmplitude[0][jj] >> 16) & 0xff);
			m_cColor[ii + 128][1] = char((theApp.m_Palette.m_rgbAmplitude[0][jj] >> 8) & 0xff);
			m_cColor[ii + 128][2] = char((theApp.m_Palette.m_rgbAmplitude[0][jj] >> 0) & 0xff);
			m_cColor[ii + 128][3] = 0;
		}
		else {
			m_cColor[ii + 128][0] = char((theApp.m_Palette.m_rgbAmplitude[0][128] >> 16) & 0xff);
			m_cColor[ii + 128][1] = char((theApp.m_Palette.m_rgbAmplitude[0][128] >> 8) & 0xff);
			m_cColor[ii + 128][2] = char((theApp.m_Palette.m_rgbAmplitude[0][128] >> 0) & 0xff);
			m_cColor[ii + 128][3] = 0;
		}
	}
}

void CLSABScanDlg::DrawGates(CDC* pDC,CRect rr)
{
	
	int	gg,nInterfacePos;
	int nTimeSlot = 0;

	if((theApp.m_LSA.m_bDevicePresent) && (theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30] == LSA)) {
		nTimeSlot = theApp.m_LSA.m_nScopeViewLaw;	
	}

	ADC200Data* pAdc = NULL;
	GatesData* pGates = NULL;
	DSP200Data* pDsp = NULL;
	CPen* pPen;
	CPen *pOldPen;

	CPoint	*pPtStart = new CPoint[theApp.m_LSA.m_nActualLawsL+1];
	CPoint	*pPtFinish = new CPoint[theApp.m_LSA.m_nActualLawsL+1];


	for(gg=0;gg<8;gg++) {
		for(int nLaw = 0;nLaw<theApp.m_LSA.m_nActualLawsL;nLaw++) {

			pAdc = &theApp.m_UtUser.m_TS[nLaw].Adc;
			pGates = &theApp.m_UtUser.m_TS[nLaw].Gate;
			pDsp = &theApp.m_UtUser.m_TS[nLaw].Dsp;

			if(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) {
				pPtStart[nLaw].y = pPtFinish[nLaw].y = MulDiv(theApp.m_LSA.m_nActualLawsL - nLaw - 1,rr.Height(),theApp.m_LSA.m_nActualLawsL-1);
			} else {
				pPtStart[nLaw].y = pPtFinish[nLaw].y = MulDiv(nLaw,rr.Height(),theApp.m_LSA.m_nActualLawsL-1);
			}

			switch(pGates->nGateMode[gg]) {
			case 0:
				break;
			case 1:
				pPtStart[nLaw].x=MulDiv(pGates->nNsDelay[gg]-pAdc->nDelay,rr.Width(),pAdc->nWidth);
				pPtFinish[nLaw].x=MulDiv(pGates->nNsWidth[gg],rr.Width(),pAdc->nWidth) + pPtStart[nLaw].x;
				break;
			case 2:
			case 3:
				nInterfacePos=(int)((float)pGates->nTimeSample[0]*pAdc->fSamplePeriod);
				pPtStart[nLaw].x=MulDiv(pGates->nNsDelay[gg]+nInterfacePos,rr.Width(),pAdc->nWidth);
				pPtFinish[nLaw].x=MulDiv((int)((float)pGates->nSampleWidth[gg]*pAdc->fSamplePeriod),rr.Width(),pAdc->nWidth) + pPtStart[nLaw].x;
				break;
			};
		}
		if((pPtStart[0].x<rr.Width())&&(pPtFinish[0].x>=0)&&(pPtFinish[0].x<rr.Width())&&pGates->nGateMode[gg]) {
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGateColor[gg]);
			pOldPen=pDC->SelectObject(pPen);

			pDC->Polyline(pPtStart,theApp.m_LSA.m_nActualLawsL);
			pDC->Polyline(pPtFinish,theApp.m_LSA.m_nActualLawsL);

			pDC->SelectObject(pOldPen);
			delete pPen;
		};
	}
	delete pPtStart;
	delete pPtFinish;
}

void CLSABScanDlg::DrawFrameRate(CDC *pDC, CRect rr)
{
	CString Buff;
	int nFocalLawCount;

	if (theApp.m_AOSPhasedArray.IsConnected() == true) {
		Buff.Format(L"Frame rate = %d per second", theApp.m_AOSPhasedArray.getFrameRate());
	}
	else {
		if ((nFocalLawCount = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws()) > 0) {
			Buff.Format(L"Frame rate = %d per second", theApp.m_nRealTotalPRF / theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws());
		}
	}

	CSize size = pDC->GetTextExtent(Buff);
	pDC->SetTextColor(RGB(200, 200, 200));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rr.Width() - size.cx - 3, rr.Height() - size.cy, Buff);
}

void CLSABScanDlg::DrawDeltaWp(CDC *pDC, CRect rr)
{
	
	CString Buff;

	float fPath0=theApp.m_UtUser.GetGateWaterPath(0);
	float fPath1=theApp.m_UtUser.GetGateWaterPath(theApp.m_LSA.m_nActualLawsL - 1);

	Buff.Format(L"Wp diff' %d/%d = %.01fmm",1, theApp.m_LSA.m_nActualLawsL, fabsf(fPath0 - fPath1));
	CSize size = pDC->GetTextExtent(Buff);
	pDC->SetTextColor(RGB(200,200,200));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rr.Width()-size.cx-3,0,Buff);

}

void CLSABScanDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd()) {
		if (WaitForSingleObject(m_hSemaphore, 10) == WAIT_OBJECT_0) {
			m_StaticPosView.SetNewPos();
			m_StaticPosScale.SetNewPos();
			ReleaseSemaphore(m_hSemaphore, 1, NULL);
		}
	}
}

void CLSABScanDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	int nSlot;
	int nSweepWidth = theApp.m_LSA.m_nActualLawsL;

	if(rr.PtInRect(point)) {
		if(theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) {
			nSlot = MulDiv(rr.Height()-(point.y-rr.top),nSweepWidth-1,rr.Height());
		} else {
			nSlot = MulDiv(point.y-rr.top,nSweepWidth-1,rr.Height());
		}
		if(nSlot<0) nSlot=0;
		if(nSlot>=nSweepWidth) nSlot = nSweepWidth-1;
		theApp.m_LSA.m_nScopeViewLaw = nSlot;
		theApp.m_UtUser.m_Global.nTimeSlot = nSlot;
		pFrame->SendMessage(UI_FOCAL_LAW_CHANGED, _UPDATE_PHASED_ARRAY_SHEET | _UPDATE_ULTRASONICS_SHEET, NULL);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}



BOOL CLSABScanDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_BUTTON_LSA_HIGHLIGHT: strTipText.LoadString(IDS_Highlight);
	   break;
   case ID_BUTTON_LSA_INVERT: strTipText.LoadString(IDS_Invert);
	   break;
   case ID_BUTTON_LSA_FULLWAVE: strTipText.LoadString(IDS_Fullwave);
	   break;
   case ID_BUTTON_LSA_POSITIVE_HW: strTipText.LoadString(IDS_Positive_HW);
	   break;
   case ID_BUTTON_LSA_NEGATIVE_HW: strTipText.LoadString(IDS_Negative_HW);
	   break;
   case ID_BUTTON_LSA_GATES: strTipText.LoadString(IDS_Amplitude_gates);
	   break;
   case ID_BUTTON_LSA_TOF: strTipText.LoadString(IDS_Time_of_flight);
	   break;
   case ID_BUTTON_LSA_SMOOTH: strTipText.LoadString(IDS_Smooth);
	   break;
   case ID_BUTTON_LSA_SCALE: strTipText.LoadString(IDS_Scale);
	   break;
   case ID_BUTTON_DELTA_WP: strTipText.LoadString(IDS_Delta_water_path);
	   break;
   case ID_BUTTON_FRAME_RATE: strTipText.LoadString(IDS_Frame_rate);
	   break;
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}



void CLSABScanDlg::SetToolBarCheckedState()
{
	
	int	nIndex;

	m_nRectification = 0;
	if(theApp.m_LastSettings.nLSAMask & LSA_POSITIVE_HW_BSCAN) m_nRectification= 1;
	if(theApp.m_LastSettings.nLSAMask & LSA_NEGATIVE_HW_BSCAN) m_nRectification= 2;


	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_INVERT))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_HIGHLIGHT))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_HIGHLIGHT_LINE ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_FULLWAVE))>=0)
		((theApp.m_LastSettings.nLSAMask & (LSA_POSITIVE_HW_BSCAN | LSA_NEGATIVE_HW_BSCAN)) ==0) ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_POSITIVE_HW))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_POSITIVE_HW_BSCAN ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_NEGATIVE_HW))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_NEGATIVE_HW_BSCAN ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_GATES))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_GATES ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_TOF))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_TOF ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_SMOOTH))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_SMOOTH_BSCAN ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LSA_SCALE))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_SCALE ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_DELTA_WP))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_DELTA_WP ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_FRAME_RATE))>=0)
		theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_FRAME_RATE ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

}

void CLSABScanDlg::OnButtonLsaFullwave() 
{
	theApp.m_LastSettings.nLSAMask &= ~(LSA_POSITIVE_HW_BSCAN | LSA_NEGATIVE_HW_BSCAN);
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonLsaPositiveHw() 
{
	theApp.m_LastSettings.nLSAMask &= ~LSA_NEGATIVE_HW_BSCAN; 
	theApp.m_LastSettings.nLSAMask |= LSA_POSITIVE_HW_BSCAN;
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonLsaNegativeHw() 
{
	theApp.m_LastSettings.nLSAMask &= ~LSA_POSITIVE_HW_BSCAN;
	theApp.m_LastSettings.nLSAMask |= LSA_NEGATIVE_HW_BSCAN;
	SetToolBarCheckedState();
}


void CLSABScanDlg::OnButtonLsaSmooth() 
{
	theApp.m_LastSettings.nLSAMask & LSA_SMOOTH_BSCAN ? theApp.m_LastSettings.nLSAMask &= ~LSA_SMOOTH_BSCAN : theApp.m_LastSettings.nLSAMask |= LSA_SMOOTH_BSCAN;
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonLsaInvert() 
{
	
	
	theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN ? theApp.m_LastSettings.nLSAMask &= ~LSA_INVERT_BSCAN : theApp.m_LastSettings.nLSAMask |= LSA_INVERT_BSCAN;
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonLsaHighlight() 
{
	theApp.m_LastSettings.nLSAMask & LSA_HIGHLIGHT_LINE ? theApp.m_LastSettings.nLSAMask &= ~LSA_HIGHLIGHT_LINE : theApp.m_LastSettings.nLSAMask |= LSA_HIGHLIGHT_LINE;
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonLsaGates() 
{
	theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_GATES ? theApp.m_LastSettings.nLSAMask &= ~LSA_DISPLAY_GATES : theApp.m_LastSettings.nLSAMask |= LSA_DISPLAY_GATES;
	SetToolBarCheckedState();
	
}

void CLSABScanDlg::OnButtonLsaTof() 
{
	theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_TOF ? theApp.m_LastSettings.nLSAMask &= ~LSA_DISPLAY_TOF : theApp.m_LastSettings.nLSAMask |= LSA_DISPLAY_TOF;
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonLsaScale() 
{
	theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_SCALE ? theApp.m_LastSettings.nLSAMask &= ~LSA_DISPLAY_SCALE : theApp.m_LastSettings.nLSAMask |= LSA_DISPLAY_SCALE;
	SetToolBarCheckedState();

	SetScalePosition();
}

void CLSABScanDlg::SetScalePosition()
{
	int nn;

	theApp.SuspendThread(this);
	if(theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_SCALE) {
		m_StaticPosView.setLeftPos(m_StaticPosScale.getLeftPos() + 33);
		m_StaticPosScale.setRightPos(m_StaticPosScale.getLeftPos() + 33);
	} else {
		m_StaticPosScale.setRightPos(nn=m_StaticPosScale.getLeftPos());
		m_StaticPosView.setLeftPos(nn);
	}
	m_StaticPosView.SetNewPos();	
	m_StaticPosScale.SetNewPos();	
	theApp.StartThread(L"LSA B-Scan Dlg", RenderBScan, this, 100, THREAD_PRIORITY_BELOW_NORMAL);

}


void CLSABScanDlg::OnButtonDeltaWp() 
{
	theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_DELTA_WP ? theApp.m_LastSettings.nLSAMask &= ~LSA_DISPLAY_DELTA_WP : theApp.m_LastSettings.nLSAMask |= LSA_DISPLAY_DELTA_WP;
	SetToolBarCheckedState();
}

void CLSABScanDlg::OnButtonFrameRate() 
{
	theApp.m_LastSettings.nLSAMask & LSA_DISPLAY_FRAME_RATE ? theApp.m_LastSettings.nLSAMask &= ~LSA_DISPLAY_FRAME_RATE : theApp.m_LastSettings.nLSAMask |= LSA_DISPLAY_FRAME_RATE;
	SetToolBarCheckedState();
}


BOOL CLSABScanDlg::PreTranslateMessage(MSG* pMsg)
{
	FRAME;
	int nStep = 0;

	if (pMsg->message == WM_KEYDOWN) {

		CPoint pt;
		CRect rr;

		switch (pMsg->wParam) {
		case 37://LeftKeyPressed
			break;
		case 39://RightKeyPressed
			break;
		case 38://UpKeyPressed
			theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN ? nStep = 1 : nStep = -1;
			break;
		case 40://DownKeyPressed
			theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN ? nStep = -1 : nStep = 1;
			break;
		case 34://PageDown
			theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN ? nStep = -10 : nStep = 10;
			break;
		case 33://PageUp
			theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN ? nStep = 10 : nStep = -10;
			break;
		case 36://Home
			break;
		case 35://Home
			break;

		default:
			return TRUE;
			break;
		}

		if (nStep) {
			theApp.m_LSA.m_nScopeViewLaw += nStep;
			theApp.m_UtUser.m_Global.nTimeSlot = MinMax(&theApp.m_LSA.m_nScopeViewLaw, 0, theApp.m_LSA.m_nActualLawsL - 1);
			pFrame->SendMessage(UI_FOCAL_LAW_CHANGED, _UPDATE_PHASED_ARRAY_SHEET | _UPDATE_ULTRASONICS_SHEET, NULL);
			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CLSABScanDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	FRAME;
	int nStep = 0;

	zDelta > 0 ? nStep=-1 : nStep=1;
	if (theApp.m_LastSettings.nLSAMask & LSA_INVERT_BSCAN) nStep *= -1;

	theApp.m_LSA.m_nScopeViewLaw += nStep;
	theApp.m_UtUser.m_Global.nTimeSlot = MinMax(&theApp.m_LSA.m_nScopeViewLaw, 0, theApp.m_LSA.m_nActualLawsL - 1);
	pFrame->SendMessage(UI_FOCAL_LAW_CHANGED, _UPDATE_PHASED_ARRAY_SHEET | _UPDATE_ULTRASONICS_SHEET, NULL);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
