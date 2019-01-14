// ProbeBeamSectionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeBeamSectionsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeBeamSectionsPage property page

IMPLEMENT_DYNCREATE(CProbeBeamSectionsPage, CPropertyPage)

CProbeBeamSectionsPage::CProbeBeamSectionsPage(CProbe* pProbe) : CPropertyPage(CProbeBeamSectionsPage::IDD)
{
	//{{AFX_DATA_INIT(CProbeBeamSectionsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pProbe = pProbe;
	
	m_pRectilinearArray = FALSE;
	m_nImageOrientation = 1;
	m_RectilinearRect.SetRectEmpty();

}

CProbeBeamSectionsPage::~CProbeBeamSectionsPage()
{
	SAFE_DELETE( m_pRectilinearArray );
}

void CProbeBeamSectionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeBeamSectionsPage)
	DDX_Control(pDX, IDC_CHECK_TRACK_GATE, m_checkTrackGate);
	DDX_Control(pDX, IDC_SPIN_NUMBER_SECTIONS, m_spinNumberSections);
	DDX_Control(pDX, IDC_EDIT_START_POS, m_editStartPos);
	DDX_Control(pDX, IDC_EDIT_SLOW_SIZE, m_editSlowScanSize);
	DDX_Control(pDX, IDC_EDIT_NUMBER_SECTIONS, m_editNumberSections);
	DDX_Control(pDX, IDC_EDIT_INCREMENT, m_editIncrement);
	DDX_Control(pDX, IDC_EDIT_FINISH_POS, m_editFinishPos);
	DDX_Control(pDX, IDC_EDIT_FAST_SIZE, m_editFastScanSize);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeBeamSectionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CProbeBeamSectionsPage)
	ON_EN_CHANGE(IDC_EDIT_FAST_SIZE, OnChangeEditFastSize)
	ON_EN_CHANGE(IDC_EDIT_FINISH_POS, OnChangeEditFinishPos)
	ON_EN_CHANGE(IDC_EDIT_INCREMENT, OnChangeEditIncrement)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_SECTIONS, OnChangeEditNumberSections)
	ON_EN_CHANGE(IDC_EDIT_SLOW_SIZE, OnChangeEditSlowSize)
	ON_EN_CHANGE(IDC_EDIT_START_POS, OnChangeEditStartPos)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ENTER_START_POS, OnButtonEnterStartPos)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_FINISH_POS, OnButtonEnterFinishPos)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_TRACK_GATE, OnCheckTrackGate)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeBeamSectionsPage message handlers

BOOL CProbeBeamSectionsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_spinNumberSections.SetRange(1,100);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProbeBeamSectionsPage::UpdateAllControls()
{
	CString Buff;
	int nSide = theApp.m_LastSettings.nProbeCharacterisationSide;

	if (GetSafeHwnd() == NULL) return;

	Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fProbeFastScanSize);
	m_editFastScanSize.SetWindowText(Buff);

	Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fProbeSlowScanSize);
	m_editSlowScanSize.SetWindowText(Buff);

	Buff.Format(_T("%.02f mm"),theApp.m_LastSettings.fProbeFastIncrement);
	m_editIncrement.SetWindowText(Buff);


	Buff.Format(_T("%d"),theApp.m_LastSettings.nProbeNumberSections);
	m_editNumberSections.SetWindowText(Buff);
	m_spinNumberSections.SetPos(theApp.m_LastSettings.nProbeNumberSections);

	Buff.Format(_T("X:%.0f Y:%.0f Z:%.0f Wp:%.01f G:%.02fus"),m_pProbe->m_Cp[0].Side[nSide].fX,m_pProbe->m_Cp[0].Side[nSide].fY,m_pProbe->m_Cp[0].Side[nSide].fZ,m_pProbe->m_Cp[0].fWp[0],(float)m_pProbe->m_Cp[0].nDelay[0]/1000.0f);
	m_editStartPos.SetWindowText(Buff);

	Buff.Format(_T("X:%.0f Y:%.0f Z:%.0f Wp:%.01f G:%.02fus"),m_pProbe->m_Cp[1].Side[nSide].fX,m_pProbe->m_Cp[1].Side[nSide].fY,m_pProbe->m_Cp[1].Side[nSide].fZ,m_pProbe->m_Cp[1].fWp[0],(float)m_pProbe->m_Cp[1].nDelay[0]/1000.0f);
	m_editFinishPos.SetWindowText(Buff);

	m_checkTrackGate.SetCheck(theApp.m_LastSettings.bProbeTrackgate);

}


void CProbeBeamSectionsPage::OnChangeEditFinishPos() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CProbeBeamSectionsPage::OnChangeEditStartPos() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CProbeBeamSectionsPage::OnChangeEditIncrement() 
{
	CString Buff;

	m_editIncrement.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fProbeFastIncrement);
	theApp.m_LastSettings.fProbeSlowIncrement = theApp.m_LastSettings.fProbeFastIncrement;
}

void CProbeBeamSectionsPage::OnChangeEditNumberSections() 
{
	CString Buff;
	int nTemp = theApp.m_LastSettings.nProbeNumberSections;

	m_editNumberSections.GetWindowText(Buff);
	theApp.m_LastSettings.nProbeNumberSections = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nProbeNumberSections) {
		m_spinNumberSections.SetPos(theApp.m_LastSettings.nProbeNumberSections);
	}
}

void CProbeBeamSectionsPage::OnChangeEditSlowSize() 
{
	CString Buff;

	m_editSlowScanSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fProbeSlowScanSize);
}

void CProbeBeamSectionsPage::OnChangeEditFastSize() 
{
	CString Buff;

	m_editFastScanSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fProbeFastScanSize);
}


void CProbeBeamSectionsPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_NUMBER_SECTIONS:
		nTemp = theApp.m_LastSettings.nProbeNumberSections;
		theApp.m_LastSettings.nProbeNumberSections = (pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_LastSettings.nProbeNumberSections) {
			UpdateAllControls();
		}
		break;
	}	
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CProbeBeamSectionsPage::OnButtonEnterStartPos() 
{
	theApp.m_Motors.GetHeadPos(&m_pProbe->m_Cp[0]);
	m_pProbe->m_Cp[0].fWp[0] = theApp.m_UtUser.GetGateWaterPath(0);
	m_pProbe->m_Cp[0].nDelay[0] = theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0];
	UpdateAllControls();
	
}

void CProbeBeamSectionsPage::OnButtonEnterFinishPos() 
{
	theApp.m_Motors.GetHeadPos(&m_pProbe->m_Cp[1]);
	m_pProbe->m_Cp[1].fWp[0] = theApp.m_UtUser.GetGateWaterPath(0);
	m_pProbe->m_Cp[1].nDelay[0] = theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0];

	UpdateAllControls();
}

void CProbeBeamSectionsPage::OnButtonStart() 
{
	theApp.m_Thread.m_bAbortFlag = false;
	if(theApp.m_LastSettings.nProbeNumberSections <= 1) {
		theApp.m_Thread.StopCurrentThreadAction();
		switch(theApp.m_LastSettings.nPeakSide) {
		case 0:
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXLeft;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYLeft;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
			break;
		case 1:
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = theApp.m_nSecondarySide;
			break;
		}
		theApp.m_LastSettings.nPeakOrthogonalMode = 1;
		theApp.m_Thread.m_pNormalizePage = NULL;
		theApp.m_Thread.m_pProbeBeamSectionsPage = this;
		theApp.m_Thread.m_nThreadAction=BEAM_PROFILE_SECTIONS;
	} else {
	
		m_nTimerAction = BEAM_PROFILE_SECTIONS;
		m_nSubTimerAction = 0;
		SetTimer(1,500,NULL);
	}
}

void CProbeBeamSectionsPage::OnPaint() 
{
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting

	RenderScan(&dc);
	
}

void CProbeBeamSectionsPage::RenderScan(CPaintDC *pDC)
{
	CRect rr;
	CDC  dcMem;
	CBitmap bmp;
	int	nOffset,nPtr,nColor,ii,nX,nY,pnX,pnY;
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);

	if(m_pRectilinearArray==NULL) {
		pDC->FillRect(rr,&CBrush(RGB(255,255,255)));
		return;
	}

	char *pArrayView = new char[(rr.Width()+2) * (rr.Height()+2) * nBytesPixel];
	char *pDest;

	char	cColor[256][4];
	for(ii=0;ii<256;ii++) {
		cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>16)&0xff);
		cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>8)&0xff);
		cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>0)&0xff);
	}

	for(nY=0;nY<rr.Height();nY++) {
		if(m_nImageOrientation == 0) {
			nOffset = MulDiv(nY,m_RectilinearRect.Height()-1,rr.Height()-1) * m_RectilinearRect.Width();
		} else {
			nOffset = MulDiv(rr.Height()-nY-1,m_RectilinearRect.Height()-1,rr.Height()-1) * m_RectilinearRect.Width();
		}
		pDest = pArrayView + (nY * rr.Width() * nBytesPixel);
		for(nX=0;nX<rr.Width();nX++) {
			nPtr = MulDiv(nX,m_RectilinearRect.Width()-1,rr.Width()-1);
			nColor = m_pRectilinearArray[nOffset + nPtr] + 128;
			*pDest++=cColor[nColor][0];
			*pDest++=cColor[nColor][1];
			*pDest++=cColor[nColor][2];
			if(nBytesPixel>3) pDest++;
		}
	}



	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*nBytesPixel),pArrayView);

	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArrayView;

	FindRectilinearCentre(&nX,&nY);

	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&penRed);

	pnX = MulDiv(nX,rr.Width(),m_RectilinearRect.Width()) + rr.left;
	if(m_nImageOrientation==0) {
		pnY = MulDiv(nY,rr.Height(),m_RectilinearRect.Height()) + rr.top;
	} else {
		pnY = MulDiv(m_RectilinearRect.Height()-nY-1,rr.Height(),m_RectilinearRect.Height()) + rr.top;
	}
	pDC->MoveTo(rr.left,pnY);
	pDC->LineTo(rr.right,pnY);
	
	pDC->SelectObject(&penBlue);
	pDC->MoveTo(pnX,rr.top);
	pDC->LineTo(pnX,rr.bottom);

	pDC->SelectObject(pOldPen);

}

void CProbeBeamSectionsPage::TransferRectilinearData(CPolyCoord *pLine,int nNumberLines,int nLine)
{
	if((nNumberLines != m_RectilinearRect.Height()) || (pLine->m_nCoordL != m_RectilinearRect.Width()) || (m_pRectilinearArray == NULL)) {
		m_RectilinearRect.SetRect(0,0,pLine->m_nCoordL,nNumberLines);
		SAFE_DELETE(m_pRectilinearArray);
		m_pRectilinearArray = new char[m_RectilinearRect.Height() * m_RectilinearRect.Width()];
	}

	int nY = nNumberLines - nLine - 1;
	if(nY<0) nY = 0;
	if(nY>=nNumberLines) nY = nNumberLines-1;

	int nOffset = nLine * m_RectilinearRect.Width();
	for(int ii=0;ii<m_RectilinearRect.Width();ii++) {
		m_pRectilinearArray[nOffset+ii] = pLine->m_pCp[ii].nAmp;
	}

}

void CProbeBeamSectionsPage::FindRectilinearCentre(int *pnSample, int *pnLine)
{
	int nX,nY,nMax,nn,nThresh,nX0,nX1,nY0,nY1,nNumberGreaterThanThresh,nSum;

	*pnSample = m_RectilinearRect.CenterPoint().x;
	*pnLine = m_RectilinearRect.CenterPoint().y;
	nMax=0;
	for(nY=0,nn=0;nY<m_RectilinearRect.Height();nY++) {
		for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
			if(m_pRectilinearArray[nn]>nMax) {
				*pnSample = nX;
				*pnLine = nY;
				nMax = m_pRectilinearArray[nn];
			}
		}
	}
	
	switch(theApp.m_LastSettings.nPeakMode[0]) {
	case 0:
		break;
	case 1:
		nX0=nX1=*pnSample;
		nY0=nY1=*pnLine;
		nThresh = nMax / 2;
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Width();
		for(nY=*pnLine;(nY>=0) && (nSum>0);nY--) {
			nn=nY * m_RectilinearRect.Width();
			nSum=0;
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nY0=nY;
			}
		}
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Width();
		for(nY=*pnLine;(nY<m_RectilinearRect.Height()) && (nSum>0);nY++) {
			nn=nY * m_RectilinearRect.Width();
			nSum=0;
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nY1=nY;
			}
		}
		*pnLine = (nY1 + nY0) / 2;

		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Height();
		for(nX=*pnSample;(nX>=0) && (nSum>0);nX--) {
			for(nY=0,nn=nX,nSum=0;nY<m_RectilinearRect.Height();nY++,nn+=m_RectilinearRect.Width()) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nX0=nX;
			}
		}
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Height();
		for(nX=*pnSample;(nX<m_RectilinearRect.Width()) && (nSum>0);nX++) {
			for(nY=0,nn=nX,nSum=0;nY<m_RectilinearRect.Height();nY++,nn+=m_RectilinearRect.Width()) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nX1=nX;
			}
		}
		*pnSample = (nX1 + nX0) / 2;

		break;
	}
}

void CProbeBeamSectionsPage::TransferScanToBeamProfile()
{
	CCoord CpHead;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_Motors.GetHeadPos(&CpHead);
	CpHead.fWp[0] = theApp.m_UtUser.GetGateWaterPath(0);

	int nBeamProfile = FindAppropriateBeamProfile();

	nBeamProfile = m_pProbe->m_nNumberBeamProfiles - 1;

	if(m_pProbe->m_pProbeBeamProfile[nBeamProfile]->m_bLocked != TRUE) {

		m_pProbe->AddBeamProfileScan(1,nBeamProfile,m_pRectilinearArray,m_RectilinearRect.Width(),m_RectilinearRect.Height(),
									theApp.m_LastSettings.fProbeFastScanSize,theApp.m_LastSettings.fProbeSlowScanSize,
										theApp.m_LastSettings.fProbeFastIncrement,theApp.m_LastSettings.fProbeSlowIncrement,
										CpHead);

		if(pFrame->m_pProbeCharacterisationSheet) pFrame->m_pProbeCharacterisationSheet->UpdateAllPages();
	}
}

void CProbeBeamSectionsPage::OnTimer(UINT nIDEvent) 
{

	switch(m_nTimerAction) {
	case 0:
		break;
	case BEAM_PROFILE_SECTIONS:
		MultipleBeamProfileSections();
		break;
	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CProbeBeamSectionsPage::MultipleBeamProfileSections()
{
	int nn;
	int nDelay;

	switch(m_nSubTimerAction) {
	case 0:
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();

		m_nSectionNumber=0;
		m_nSubTimerAction = 10;
		break;
	case 10:
		if((m_nSectionNumber < theApp.m_LastSettings.nProbeNumberSections) && (theApp.m_Thread.m_bAbortFlag == false)) {
			for(nn=0;nn<20;nn++) {
				theApp.m_Thread.m_CpMoveTo.fPos[nn] = (m_pProbe->m_Cp[1].fPos[nn] - m_pProbe->m_Cp[0].fPos[nn]) * (float)m_nSectionNumber / (float)(theApp.m_LastSettings.nProbeNumberSections - 1);
				theApp.m_Thread.m_CpMoveTo.fPos[nn] += m_pProbe->m_Cp[0].fPos[nn];
			}

			if(theApp.m_LastSettings.bProbeTrackgate == TRUE) {
				nDelay = (int)((float)(m_pProbe->m_Cp[1].nDelay[0] - m_pProbe->m_Cp[0].nDelay[0]) * (float)m_nSectionNumber / (float)(theApp.m_LastSettings.nProbeNumberSections - 1));
				nDelay += m_pProbe->m_Cp[0].nDelay[0];
				theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0] = nDelay;
				theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide],0);
			}

			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
			m_nSubTimerAction=20;
		} else {
			m_nSubTimerAction=999;
		}
		break;
	case 20:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {

			theApp.m_Thread.StopCurrentThreadAction();
			switch(theApp.m_LastSettings.nPeakSide) {
			case 0:
				theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXLeft;
				theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nZLeft;
				theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
				theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
				break;
			case 1:
				theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
				theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nZRight;
				theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
				theApp.m_Thread.m_nNormSide = theApp.m_nSecondarySide;
				break;
			}
			theApp.m_LastSettings.nPeakOrthogonalMode = 1;
			theApp.m_Thread.m_pNormalizePage = NULL;
			theApp.m_Thread.m_pProbeBeamSectionsPage = this;
			theApp.m_Thread.m_nThreadAction=PROBE_CROSS_SECTION;

			m_nSubTimerAction = 30;
		}
		break;
	case 30:
		if(theApp.m_Thread.HasProbeCrossSectionFinished()==TRUE) {
			m_nSectionNumber++;
			m_nSubTimerAction = 10;
		}
		break;

	case 999:
		m_nTimerAction=0;
		m_nSubTimerAction=0;
		theApp.m_Thread.m_bEnableJoystick=TRUE;
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		KillTimer(1);
		break;
	}

}

BOOL CProbeBeamSectionsPage::OnSetActive() 
{

	UpdateAllControls();
	return CPropertyPage::OnSetActive();
}

int CProbeBeamSectionsPage::FindAppropriateBeamProfile()
{

//	if(m_pProbeCharacterisationSheet) m_pProbeCharacterisationSheet->UpdateAllPages();

	return 0;

}

void CProbeBeamSectionsPage::OnCheckTrackGate() 
{
	theApp.m_LastSettings.bProbeTrackgate = m_checkTrackGate.GetCheck() & 0x01;
	UpdateAllControls();
}

void CProbeBeamSectionsPage::OnButtonGoto0() 
{
	theApp.m_Thread.m_bEnableJoystick=FALSE;
	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Thread.m_CpMoveTo = m_pProbe->m_Cp[0];
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

	if(theApp.m_LastSettings.bProbeTrackgate == TRUE) {
		int nDelay = m_pProbe->m_Cp[0].nDelay[0];
		theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0] = nDelay;
		theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide],0);
	}
}

void CProbeBeamSectionsPage::OnButtonGoto1() 
{
	theApp.m_Thread.m_bEnableJoystick=FALSE;
	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Thread.m_CpMoveTo = m_pProbe->m_Cp[1];
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

	if(theApp.m_LastSettings.bProbeTrackgate == TRUE) {
		int nDelay = m_pProbe->m_Cp[1].nDelay[0];
		theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0] = nDelay;
		theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide],0);
	}

}
