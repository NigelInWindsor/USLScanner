// ProbeAxialPlotPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeAxialPlotPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeAxialPlotPage property page

IMPLEMENT_DYNCREATE(CProbeAxialPlotPage, CPropertyPage)

CProbeAxialPlotPage::CProbeAxialPlotPage(CProbe *pProbe) : CPropertyPage(CProbeAxialPlotPage::IDD)
{
	//{{AFX_DATA_INIT(CProbeAxialPlotPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pProbe = pProbe;
	
	m_pRectilinearArray[0] = NULL;
	m_pRectilinearArray[1] = NULL;

}

CProbeAxialPlotPage::~CProbeAxialPlotPage()
{
}

void CProbeAxialPlotPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeAxialPlotPage)
	DDX_Control(pDX, IDC_COMBO_PLOT2_SCANAXES, m_comboScanAxes2);
	DDX_Control(pDX, IDC_COMBO_PLOT1_SCANAXES, m_comboScanAxes1);
	DDX_Control(pDX, IDC_STATIC_VIEW2, m_staticView2);
	DDX_Control(pDX, IDC_EDIT_FINISH_POS, m_editFinishPos);
	DDX_Control(pDX, IDC_EDIT_START_POS, m_editStartPos);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeAxialPlotPage, CPropertyPage)
	//{{AFX_MSG_MAP(CProbeAxialPlotPage)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_START_POS, OnChangeEditStartPos)
	ON_EN_CHANGE(IDC_EDIT_FINISH_POS, OnChangeEditFinishPos)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_START_POS, OnButtonEnterStartPos)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_FINISH_POS, OnButtonEnterFinishPos)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	ON_CBN_SELCHANGE(IDC_COMBO_PLOT1_SCANAXES, OnSelchangeComboPlot1Scanaxes)
	ON_CBN_SELCHANGE(IDC_COMBO_PLOT2_SCANAXES, OnSelchangeComboPlot2Scanaxes)
	ON_BN_CLICKED(IDC_BUTTON_START2, OnButtonStart2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeAxialPlotPage message handlers

BOOL CProbeAxialPlotPage::OnInitDialog() 
{
	
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.Format(_T("XY"));
	m_comboScanAxes1.InsertString(0,Buff);
	m_comboScanAxes2.InsertString(0,Buff);
	Buff.Format(_T("XZ"));
	m_comboScanAxes1.InsertString(1,Buff);
	m_comboScanAxes2.InsertString(1,Buff);
	Buff.Format(_T("YZ"));
	m_comboScanAxes1.InsertString(2,Buff);
	m_comboScanAxes2.InsertString(2,Buff);
	Buff.Format(_T("ZX"));
	m_comboScanAxes1.InsertString(3,Buff);
	m_comboScanAxes2.InsertString(3,Buff);
	Buff.Format(_T("YX"));
	m_comboScanAxes1.InsertString(4,Buff);
	m_comboScanAxes2.InsertString(4,Buff);

	switch(theApp.m_LastSettings.nAxialPlotAxes1){
	case XY:
		m_comboScanAxes1.SetCurSel(0);
		break;
	case XZ:
		m_comboScanAxes1.SetCurSel(1);
		break;
	case YZ:
		m_comboScanAxes1.SetCurSel(2);
		break;
	case ZX:
		m_comboScanAxes1.SetCurSel(3);
		break;
	case YX:
		m_comboScanAxes1.SetCurSel(4);
		break;
	}


	switch(theApp.m_LastSettings.nAxialPlotAxes2){
	case XY:
		m_comboScanAxes2.SetCurSel(0);
		break;
	case XZ:
		m_comboScanAxes1.SetCurSel(1);
		break;
	case YZ:
		m_comboScanAxes1.SetCurSel(2);
		break;
	case ZX:
		m_comboScanAxes2.SetCurSel(3);
		break;
	case YX:
		m_comboScanAxes2.SetCurSel(4);
		break;
	}


	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CProbeAxialPlotPage::OnSetActive() 
{

	UpdateAllControls();
	return CPropertyPage::OnSetActive();
}

void CProbeAxialPlotPage::UpdateAllControls()
{
	CString Buff;
	int nSide = theApp.m_LastSettings.nProbeCharacterisationSide;

	if (GetSafeHwnd() == NULL) return;

	Buff.Format(_T("X:%.0f Y:%.0f Z:%.0f Wp:%.01f G:%.02fus"),m_pProbe->m_Cp[0].Side[nSide].fX,m_pProbe->m_Cp[0].Side[nSide].fY,m_pProbe->m_Cp[0].Side[nSide].fZ,m_pProbe->m_Cp[0].fWp[0],(float)m_pProbe->m_Cp[0].nDelay[0]/1000.0f);
	m_editStartPos.SetWindowText(Buff);

	Buff.Format(_T("X:%.0f Y:%.0f Z:%.0f Wp:%.01f G:%.02fus"),m_pProbe->m_Cp[1].Side[nSide].fX,m_pProbe->m_Cp[1].Side[nSide].fY,m_pProbe->m_Cp[1].Side[nSide].fZ,m_pProbe->m_Cp[1].fWp[0],(float)m_pProbe->m_Cp[1].nDelay[0]/1000.0f);
	m_editFinishPos.SetWindowText(Buff);


}

void CProbeAxialPlotPage::OnButtonStart() 
{
	theApp.m_LastSettings.nWhichAxialPlot=0;
	Start();
}


void CProbeAxialPlotPage::OnButtonStart2() 
{
	theApp.m_LastSettings.nWhichAxialPlot=1;
	Start();	
}



void CProbeAxialPlotPage::Start() 
{
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
	theApp.m_Thread.m_pProbeBeamSectionsPage = NULL;
	theApp.m_Thread.m_pProbeAxialPlotPage = this;
	theApp.m_Thread.m_pProbe = m_pProbe;
	theApp.m_Thread.m_nThreadAction=PROBE_AXIAL_PLOT;
	
}


void CProbeAxialPlotPage::TransferRectilinearData(CPolyCoord *pLine,int nNumberLines,int nLine)
{
	if((theApp.m_LastSettings.nWhichAxialPlot<0)||(theApp.m_LastSettings.nWhichAxialPlot>1))
		return;

	if((nNumberLines != m_RectilinearRect.Height()) || (pLine->m_nCoordL != m_RectilinearRect.Width()) || (m_pRectilinearArray[theApp.m_LastSettings.nWhichAxialPlot] == NULL)) {
		m_RectilinearRect.SetRect(0,0,pLine->m_nCoordL,nNumberLines);
		SAFE_DELETE(m_pRectilinearArray[theApp.m_LastSettings.nWhichAxialPlot]);
		m_pRectilinearArray[theApp.m_LastSettings.nWhichAxialPlot] = new char[m_RectilinearRect.Height() * m_RectilinearRect.Width()];
	}

	int nY = nNumberLines - nLine - 1;
	if(nY<0) nY = 0;
	if(nY>=nNumberLines) nY = nNumberLines-1;

	int nOffset = nLine * m_RectilinearRect.Width();
	for(int ii=0;ii<m_RectilinearRect.Width();ii++) {
		m_pRectilinearArray[theApp.m_LastSettings.nWhichAxialPlot][nOffset+ii] = pLine->m_pCp[ii].nAmp;
	}

}


void CProbeAxialPlotPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	RenderScan(&dc);
	
}

void CProbeAxialPlotPage::RenderScan(CPaintDC *pDC)
{
	CRect rr[2];
	CDC  dcMem;
	CBitmap bmp;
	int	nPtr,nColor,ii,nX,nY;
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;
	int nLine,nSample;

	m_staticView.GetWindowRect(&rr[0]);
	m_staticView2.GetWindowRect(&rr[1]);

	for(int jj=0;jj<1;jj++){
		if(m_pRectilinearArray[jj]!=NULL) {
			

			ScreenToClient(&rr[jj]);
			rr[jj].DeflateRect(2,2);

			char *pArrayView = new char[(rr[jj].Width()+2) * (rr[jj].Height()+2) * nBytesPixel];
			char *pDest;

			char	cColor[256][4];
			for(ii=0;ii<256;ii++) {
				cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>16)&0xff);
				cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>8)&0xff);
				cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>0)&0xff);
			}

			for(nY=0;nY<rr[jj].Height();nY++) {
				nSample = MulDiv(nY,m_RectilinearRect.Width()-1,rr[jj].Height()-1);
				pDest = pArrayView + (nY * rr[jj].Width() * nBytesPixel);
				for(nX=0;nX<rr[jj].Width();nX++) {
					nLine = MulDiv(nX,m_RectilinearRect.Height()-1,rr[jj].Width()-1);

					nPtr = nLine * m_RectilinearRect.Width() + nSample;
					nColor = m_pRectilinearArray[jj][nPtr] + 128;
					*pDest++=cColor[nColor][0];
					*pDest++=cColor[nColor][1];
					*pDest++=cColor[nColor][2];
					if(nBytesPixel>3) pDest++;
				}
			}



			bmp.CreateCompatibleBitmap(pDC,rr[jj].Width(),rr[jj].Height());
			bmp.SetBitmapBits((DWORD)(rr[jj].Width()*rr[jj].Height()*nBytesPixel),pArrayView);

			dcMem.CreateCompatibleDC(pDC);

			CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
			pDC->BitBlt(rr[jj].left,rr[jj].top,rr[jj].Width(),rr[jj].Height(),&dcMem,0,0,SRCCOPY);
			dcMem.SelectObject(pOldBitmap);

			delete pArrayView;
		}
	}


}


void CProbeAxialPlotPage::TransferScanToBeamProfile()
{
	CCoord CpHead;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_Motors.GetHeadPos(&CpHead);
	CpHead.fWp[0] = theApp.m_UtUser.GetGateWaterPath(0);

	int nBeamProfile = m_pProbe->m_nNumberBeamProfiles - 1;

	if(m_pProbe->m_pProbeBeamProfile[nBeamProfile]->m_bLocked == FALSE) {
		m_pProbe->AddBeamProfileScan(0,nBeamProfile,m_pRectilinearArray[theApp.m_LastSettings.nWhichAxialPlot],m_RectilinearRect.Width(),m_RectilinearRect.Height(),
									theApp.m_LastSettings.fProbeFastScanSize,theApp.m_LastSettings.fProbeSlowScanSize,
										theApp.m_LastSettings.fProbeFastIncrement,theApp.m_LastSettings.fProbeSlowIncrement,
										CpHead);
		if(pFrame->m_pProbeCharacterisationSheet) pFrame->m_pProbeCharacterisationSheet->UpdateAllPages();
	}
}

void CProbeAxialPlotPage::OnChangeEditStartPos() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CProbeAxialPlotPage::OnChangeEditFinishPos() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CProbeAxialPlotPage::OnButtonEnterStartPos() 
{
	theApp.m_Motors.GetHeadPos(&m_pProbe->m_Cp[0]);
	m_pProbe->m_Cp[0].fWp[0] = theApp.m_UtUser.GetGateWaterPath(0);
	m_pProbe->m_Cp[0].nDelay[0] = theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0];
	UpdateAllControls();
	
}

void CProbeAxialPlotPage::OnButtonEnterFinishPos() 
{
	theApp.m_Motors.GetHeadPos(&m_pProbe->m_Cp[1]);
	m_pProbe->m_Cp[1].fWp[0] = theApp.m_UtUser.GetGateWaterPath(0);
	m_pProbe->m_Cp[1].nDelay[0] = theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0];

	UpdateAllControls();
}

void CProbeAxialPlotPage::OnButtonGoto0() 
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

void CProbeAxialPlotPage::OnButtonGoto1() 
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

void CProbeAxialPlotPage::OnSelchangeComboPlot1Scanaxes() 
{
	switch(m_comboScanAxes1.GetCurSel()){
	case 0:
		theApp.m_LastSettings.nAxialPlotAxes1 = XY;
		break;
	case 1:
		theApp.m_LastSettings.nAxialPlotAxes1 = XZ;
		break;
	case 2:
		theApp.m_LastSettings.nAxialPlotAxes1 = YZ;
		break;
	case 3:
		theApp.m_LastSettings.nAxialPlotAxes1 = ZX;
		break;
	case 4:
		theApp.m_LastSettings.nAxialPlotAxes1 = YX;
		break;
	}
}

void CProbeAxialPlotPage::OnSelchangeComboPlot2Scanaxes() 
{
	switch(m_comboScanAxes2.GetCurSel()){
	case 0:
		theApp.m_LastSettings.nAxialPlotAxes2 = XY;
		break;
	case 1:
		theApp.m_LastSettings.nAxialPlotAxes2 = XZ;
		break;
	case 2:
		theApp.m_LastSettings.nAxialPlotAxes2 = YZ;
		break;
	case 3:
		theApp.m_LastSettings.nAxialPlotAxes2 = ZX;
		break;
	case 4:
		theApp.m_LastSettings.nAxialPlotAxes2 = YX;
		break;
	}
}

