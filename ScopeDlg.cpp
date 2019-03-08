// ScopeDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ScopeDlg.h"
#include <math.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum RulerMode {
	RULER_OFF,
	OFF_MATERIAL,
	WATER_MATERIAL,
	MATERIAL_MATERIAL,
};


/////////////////////////////////////////////////////////////////////////////
// CScopeDlg dialog


CScopeDlg::CScopeDlg(CWnd* pParent /*=NULL*/, CWnd** pDlg,int nTimeSlot,int nDlgNumber)
	: CDialog(CScopeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScopeDlg)
	//}}AFX_DATA_INIT

	m_nTimeSlot = nTimeSlot;
	m_nDlgNumber = nDlgNumber;
	
	m_pDlg=pDlg;
	m_nGate = 0;
	bModifyGatePosition=FALSE;
	m_bModifyDelayWidth=FALSE;
	m_bModifyDacPosition=FALSE;
	m_pArray = NULL;
	m_pPtTrace = NULL;
	m_nArrayWidth=-1;
	m_nArrayHeight=-1;
	m_Units[0] = "ns";
	m_Units[1] = "\x0b5s";
	m_Units[2]="mm";
	m_Units[3]="\x022";
	m_bDelayLockedToZ = FALSE;
	m_nTraceDelay=-1;
	m_nTraceWidth=-1;
	m_bRecording=FALSE;
	theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]=0;
	m_bTrackCurrentTimeSlot=FALSE;
	m_nMaxSample = 80;

}


void CScopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScopeDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_USLBARCTRL, m_scrollbarDelayWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScopeDlg, CDialog)
	//{{AFX_MSG_MAP(CScopeDlg)
	ON_WM_CLOSE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_SCOPE_LOCKDELAYTOZ, OnScopeLockdelaytoz)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SCOPE_DACREFAMPLITUDE, OnScopeDacrefamplitude)
	ON_COMMAND(ID_SCOPE_STOREDACPOINT, OnScopeStoredacpoint)
	ON_COMMAND(ID_SCOPE_DISPLAYDACPOINTS, OnScopeDisplaydacpoints)
	ON_COMMAND(ID_SCOPE_DELETEDACPOINT, OnScopeDeletedacpoint)
	ON_COMMAND(ID_SCOPE_SAVEASBMP, OnScopeSaveasbmp)
	ON_COMMAND(ID_SCOPE_STARTRECORDING, OnScopeStartrecording)
	ON_COMMAND(ID_SCOPE_STOPRECORDING, OnScopeStoprecording)
	ON_COMMAND(ID_SCOPE_PLAYRECORDING, OnScopePlayrecording)
	ON_COMMAND(ID_SCOPE_DRAWMODE_VECTOR, OnScopeDrawmodeVector)
	ON_COMMAND(ID_SCOPE_DRAWMODE_DOTS, OnScopeDrawmodeDots)
	ON_COMMAND(ID_SCOPE_CURRENTTIMESLOT, OnScopeCurrenttimeslot)
	ON_COMMAND(ID_SCOPE_SLOT1, OnScopeSlot1)
	ON_COMMAND(ID_SCOPE_SLOT2, OnScopeSlot2)
	ON_COMMAND(ID_SCOPE_SLOT3, OnScopeSlot3)
	ON_COMMAND(ID_SCOPE_SLOT4, OnScopeSlot4)
	ON_COMMAND(ID_SCOPE_SLOT5, OnScopeSlot5)
	ON_COMMAND(ID_SCOPE_SLOT6, OnScopeSlot6)
	ON_COMMAND(ID_SCOPE_SLOT7, OnScopeSlot7)
	ON_COMMAND(ID_SCOPE_SLOT8, OnScopeSlot8)
	ON_COMMAND(ID_SCOPE_AUTOINTERFACEGATE, OnScopeAutointerfacegate)
	ON_COMMAND(ID_SCOPE_MAINBANGDELAY_ZERO, OnScopeMainbangdelayZero)
	ON_COMMAND(ID_SCOPE_MAINBANGDELAY_MOUSEPOS, OnScopeMainbangdelayMousepos)
	ON_COMMAND(ID_SCOPE_MAINBANGDELAY_INTERFACE, OnScopeMainbangdelayInterface)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_REFRESH_SCOPE_DISPLAY, RefreshScopeDisplay)
	ON_COMMAND(ID_RULER_OFF, &CScopeDlg::OnRulerOff)
	ON_COMMAND(ID_RULER_OFF_MATERIAL, &CScopeDlg::OnRulerOffMaterial)
	ON_COMMAND(ID_RULER_WATER_MATERIAL, &CScopeDlg::OnRulerWaterMaterial)
	ON_COMMAND(ID_RULER_MATERIAL_MATERIAL, &CScopeDlg::OnRulerMaterialMaterial)
	ON_COMMAND(ID_SCOPE_PROPERTIES, &CScopeDlg::OnScopeProperties)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScopeDlg message handlers

void CScopeDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
	delete m_pArray;
	delete m_pPtTrace;
	delete this;
}

void CScopeDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	theApp.SuspendThread(this);
	theApp.m_LastSettings.bWindowOpenOnLastExit[(IDD_OSCILLOSCOPE + m_nDlgNumber) % MAXIMUMWINDOWSIDNUMBER]=FALSE;

	CDialog::OnClose();
	DestroyWindow();
}

void CScopeDlg::OnDestroy()
{
	theApp.SuspendThread(this);

	GetWindowRect(theApp.m_LastSettings.RectWindowPos[(IDD_OSCILLOSCOPE+m_nDlgNumber) % MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();

	*m_pDlg = NULL;
}

BOOL CScopeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString Buff;

	if (theApp.m_AOSPhasedArray.isConnected() == TRUE) {
		theApp.m_LSA.m_bDevicePresent = TRUE;
		theApp.m_LastSettings.nDesiredDevice = PHASED_ARRAY;
	}
	else {
		theApp.m_LSA.m_bDevicePresent = false;
		theApp.m_LastSettings.nDesiredDevice = NOT_PHASED_ARRAY;
	}


	if(theApp.m_Scope.m_fMaxRange<1e-6f) theApp.m_Scope.m_fMaxRange=1e-6f;

	pGate = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;

	m_scrollbarDelayWidth.SetMax((int)(theApp.m_Scope.m_fMaxRange/1e-9f));
	m_scrollbarDelayWidth.SetMin(0);
	m_scrollbarDelayWidth.SetHorizontal(TRUE);
	m_scrollbarDelayWidth.SetWidthHighCursor(15);
	m_scrollbarDelayWidth.SetWidthLowCursor(15);
	m_scrollbarDelayWidth.SetMinimumScreenWidth(1);
	CalculateScrollBarFromDelayWidth();


	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosDelayWidth.Initialize(this, m_scrollbarDelayWidth, FALSE, TRUE, TRUE, TRUE);

	CRect rect = theApp.m_LastSettings.RectWindowPos[(IDD_OSCILLOSCOPE + m_nDlgNumber) % MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		if(rect.Width()>1024) rect.right=rect.left+1024;
		if(rect.Height()>512) rect.bottom=rect.top+512;
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	};

	m_StaticPosView.SetNewPos();
	m_StaticPosDelayWidth.SetNewPos();

	UpdateAllControls();
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	SetWindowName();


	AllocateScreenArray();
	Invalidate(TRUE);
	

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	m_ThreadName.Format(L"A-Scan-%d", m_nDlgNumber + 1);
	theApp.StartThread(m_ThreadName, &OscilloscopeThread, this, 50, THREAD_PRIORITY_NORMAL);

	EnableToolTips();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CScopeDlg::UpdateAllControls()
{
	CalculateScrollBarFromDelayWidth();
}

void CScopeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CScopeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
/*
	switch(pSpin->GetDlgCtrlID()) {
	};
	*/
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScopeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}

void CScopeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//Store Pos and Size of the Dialog

	m_StaticPosView.SetNewPos();
	m_StaticPosGateMode.SetNewPos();
	m_StaticPosGateNumber.SetNewPos();
	m_StaticPosSpinDelay.SetNewPos();
	m_StaticPosSpinWidth.SetNewPos();
	m_StaticPosGateDelay.SetNewPos();
	m_StaticPosGateWidth.SetNewPos();
	m_StaticPosDelay.SetNewPos();
	m_StaticPosWidth.SetNewPos();
	m_StaticPosSpinThreshold.SetNewPos();
	m_StaticPosEditThreshold.SetNewPos();
	m_StaticPosThreshold.SetNewPos();
	m_StaticPosDelayWidth.SetNewPos();
	m_StaticPosAlarmText.SetNewPos();
	m_StaticPosAlarm.SetNewPos();

	if (GetSafeHwnd()) {

		Invalidate(TRUE);
	}
}


void CScopeDlg::CalculateGArray()
{
	CRect	rr;
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	static int nPlayLoop=0;

	if(GetSafeHwnd() == NULL) return;

	m_nMaxSample = 127;

	switch(theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]) {
	case 0:
		switch(theApp.m_LastSettings.nAdcCardType) {
		case USL_ADC100: theApp.m_UtUser.Adc100ReadArray(pAdc,theApp.m_Scope.m_RFTrace[m_nTimeSlot]);
			break;
		case USL_ADC200: theApp.m_UtUser.Adc200ReadArray(pAdc,theApp.m_Scope.m_RFTrace[m_nTimeSlot]);
			break;
		default:
			break;
		}
		if(m_bRecording==TRUE) {
			m_FileRecord.Write(theApp.m_Scope.m_RFTrace,pAdc->nAcqLength);
			m_nPtrRecording++;
		}
		break;
	case 1:
		m_FileRecord.Read(theApp.m_Scope.m_RFTrace[m_nTimeSlot],pAdc->nAcqLength);
		theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[m_nTimeSlot],m_nTimeSlot);
		if(m_nPtrRecording<m_nRecordingL) {
			m_nPtrRecording++;
		} else {
			m_nPtrRecording=0;
			m_FileRecord.Seek(m_nOffsetRecording,CFile::begin);
		}
		break;
	case 2:
		m_FileRecord.Seek((m_nPtrRecording+nPlayLoop)*pAdc->nAcqLength+m_nOffsetRecording,CFile::begin);
		m_FileRecord.Read(&theApp.m_Scope.m_RFTrace[m_nTimeSlot],pAdc->nAcqLength);
		theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[m_nTimeSlot],m_nTimeSlot);
		nPlayLoop++;
		nPlayLoop%=8;
		break;
	case 3:
		theApp.m_Scope.SignalGenerator(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc,theApp.m_Scope.m_RFTrace[m_nTimeSlot],m_nTimeSlot);
		break;
	}
/*
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(pAdc->nAcqLength==0) pAdc->nAcqLength=1;
	for(xx=0;xx<rr.Width();xx++) {
		ptr=MulDiv(xx,pAdc->nAcqLength,rr.Width());
		switch(pAdc->nRfType) {
		case RFTYPELINEAR:
			vv=MulDiv(theApp.m_Scope.m_RFTrace[m_nTimeSlot][ptr],rr.Height()/2,-m_nMaxSample)+rr.Height()/2;
			break;
		case RFTYPERECTIFIED:
			vv=MulDiv(theApp.m_Scope.m_RFTrace[m_nTimeSlot][ptr],rr.Height(),-m_nMaxSample)+rr.Height();
			break;
		case RFTYPELOG:
			vv=MulDiv(theApp.m_Scope.m_RFTrace[m_nTimeSlot][ptr],rr.Height(),-m_nMaxSample)+rr.Height();
			break;
		}
		if(vv<0) vv=0;
		if(vv>rr.Height()-1) vv=rr.Height()-1;
		nGArray[xx]=vv;
	};
*/
	CheckLockDelayFromZHeight();
}


void CScopeDlg::AllocateScreenArray()
{
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(m_pArray!=NULL) delete m_pArray;
	m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	if(m_pPtTrace!=NULL) delete m_pPtTrace;
	m_pPtTrace = new CPoint[rr.Width() * 2];
}


void CScopeDlg::DrawGTrace(int nDisplayDevice,CUSLFile* pFile)
{

	if(GetSafeHwnd() == NULL) return;

	static int nPrf;
	CString Buff,Language,Temp;
	CString BuffDelay,BuffWidth;
	CRect	rr;
	CDC* pDC=GetDC();
	CPen* pOldPen;
	CSize size;
	int	gg,nPtr,xx,vv;
	CDC  dcMem;
	CBitmap bmp;
	int nInterfacePos,nStart,nFinish,nn;
	CBrush brushGrey(RGB(80,80,80));
	char *pSrc;
	__int16 *pwSrc;
	int nTimeSlot = m_nTimeSlot;
	GatesData* pGate;
	ADC200Data* pAdc;
	DSP200Data* pDsp;
	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = theApp.m_Scope.m_nTextSize;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);

	ASSERT(pDC);


	if(((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) || theApp.m_AOSPhasedArray.IsConnected()) {
		nTimeSlot = theApp.m_LSA.m_nScopeViewLaw;
		theApp.m_UtUser.m_TS[0].Pr30.nMuxRx = theApp.m_UtUser.m_TS[0].Pr30.nMuxTx; //this is here to ensure the timeslot is PE
		pGate = &theApp.m_UtUser.m_TS[0].Gate;
		pAdc = &theApp.m_UtUser.m_TS[0].Adc;
		pDsp = &theApp.m_UtUser.m_TS[0].Dsp;
	} else {
		nTimeSlot = m_nTimeSlot;
		pGate = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
		pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
		pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;
	}


	SetWindowName();

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if((m_nArrayWidth-rr.Width()) || m_nArrayHeight-rr.Height()) {
		AllocateScreenArray();
		m_nArrayWidth=rr.Width();
		m_nArrayHeight=rr.Height();
	}

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(&Font);
	ASSERT(pOldBitmap);

	CBrush brushBk(theApp.m_Scope.m_rgbBkColor);
	CBrush* pOldBrush = dcMem.SelectObject(&brushBk);
	dcMem.Rectangle(0,0,rr.right,rr.bottom);
//	FillMemory(m_pArray,rr.Width()*rr.Height()*4,0xff);
	dcMem.SelectObject(pOldBrush);


	//Grey in BSCAN section
	switch(pAdc->nBscanMode) {
	case 0:
		break;
	case 1:	pAdc->nBscanLength = pAdc->nAcqLength + 4;
		break;
	case 2:
		pAdc->nBscanLength =
			(int)((float)(pAdc->nBscanPreTrigger + pAdc->nBscanPostTrigger) / pAdc->fSamplePeriod) + 4;

		nInterfacePos=(int)((float)pDsp->InterfacePos*pAdc->fSamplePeriod);
		nStart=MulDiv(nInterfacePos-pAdc->nBscanPreTrigger,rr.Width(),pAdc->nWidth);
		nFinish=MulDiv(nInterfacePos+pAdc->nBscanPostTrigger,rr.Width(),pAdc->nWidth);

		pOldBrush = dcMem.SelectObject(&brushGrey);
		dcMem.Rectangle(nStart,0,nFinish,rr.Height());
		dcMem.SelectObject(pOldBrush);
	}

	DrawGraticule(&dcMem,rr);

	//Draw Trace
	CPen OnPen(PS_SOLID,1,theApp.m_Scope.m_rgbTraceColor);
	pOldPen = dcMem.SelectObject(&OnPen);
	ASSERT(pOldPen);

	if(theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]<0) theApp.m_Scope.m_nAdcIndexFirstPoint[m_nTimeSlot]=0;
	if(theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]>=32) {
		theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]=0;
	}
	int	nS0,nS1,nMin,nMax;
	char *pArray = (char *) m_pArray;
	switch(pAdc->nRfType) {
	case RFTYPELINEAR:
		if(rr.Width()<(int)pAdc->nAcqLength) {
			for(xx=0,nn=0;xx<(rr.Width()-1);xx++) {
				nS0=MulDiv(xx,pAdc->nAcqLength,rr.Width());
				nS1=MulDiv(xx+1,pAdc->nAcqLength,rr.Width());
				nMin=10000;
				nMax=0;
				pSrc = &theApp.m_Scope.m_RFTrace[nTimeSlot][theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]];
				pwSrc = (__int16*)&theApp.m_Scope.m_wRFTrace[nTimeSlot][theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]];
				for(nPtr=nS0;nPtr<nS1;nPtr++) {
					switch(theApp.m_Scope.m_nBits) {
					default:vv=MulDiv(pSrc[nPtr],rr.Height()/2,-m_nMaxSample)+rr.Height()/2;
						break;
					case 16: vv=MulDiv(pwSrc[nPtr],rr.Height()/2,-0x7fff)+rr.Height()/2;
						break;
					}
					if(vv<0) vv=0;
					if(vv>rr.Height()-1) vv=rr.Height()-1;
					if(vv<nMin) nMin=vv;
					if(vv>nMax) nMax=vv;
				}
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = nMin;
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = nMax;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		} else {
			for(xx=0,nn=0;xx<rr.Width();xx++) {
				nPtr=MulDiv(xx,pAdc->nAcqLength,rr.Width());
				switch(theApp.m_Scope.m_nBits) {
				default: vv=MulDiv(theApp.m_Scope.m_RFTrace[nTimeSlot][nPtr+theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]],rr.Height()/2,-m_nMaxSample)+rr.Height()/2;
					break;
				case 16: vv=MulDiv(theApp.m_Scope.m_wRFTrace[nTimeSlot][nPtr+theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]],rr.Height()/2,-0x7fff)+rr.Height()/2;
					break;
				};
				if(vv<0) vv=0;
				if(vv>rr.Height()-1) vv=rr.Height()-1;
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = vv;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		}
		break;
	case RFTYPERECTIFIED:
	case RFTYPELOG:
		if(rr.Width()<(int)pAdc->nAcqLength) {
			for(xx=0,nn=0;xx<(rr.Width()-1);xx++) {
				nS0=MulDiv(xx,pAdc->nAcqLength,rr.Width());
				nS1=MulDiv(xx+1,pAdc->nAcqLength,rr.Width());
				nMin=10000;
				nMax=0;
				for(nPtr=nS0;nPtr<nS1;nPtr++) {
					switch(theApp.m_Scope.m_nBits) {
					default: vv=MulDiv(theApp.m_Scope.m_RFTrace[nTimeSlot][nPtr+theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]],rr.Height(),-m_nMaxSample)+rr.Height();
						break;
					case 16: vv=MulDiv(theApp.m_Scope.m_wRFTrace[nTimeSlot][nPtr+theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]],rr.Height(),-0x7fff)+rr.Height();
						break;
					}
					if(vv<0) vv=0;
					if(vv>rr.Height()-1) vv=rr.Height()-1;
					if(vv<nMin) nMin=vv;
					if(vv>nMax) nMax=vv;
				}
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = nMin;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		} else {
			for(xx=0,nn=0;xx<rr.Width();xx++) {
				nPtr=MulDiv(xx,pAdc->nAcqLength,rr.Width());
				switch(theApp.m_Scope.m_nBits) {
				default: vv=MulDiv(theApp.m_Scope.m_RFTrace[nTimeSlot][nPtr+theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]],rr.Height(),-m_nMaxSample)+rr.Height();
					break;
				case 16: vv=MulDiv(theApp.m_Scope.m_wRFTrace[nTimeSlot][nPtr+theApp.m_Scope.m_nAdcIndexFirstPoint[nTimeSlot]],rr.Height(),-0x7fff)+rr.Height();
					break;
				}
				if(vv<0) vv=0;
				if(vv>rr.Height()-1) vv=rr.Height()-1;
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = vv;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		}
		break;
	}
	//Put Text on Screen
	dcMem.SetTextColor(theApp.m_Scope.m_rgbTextColor);
	dcMem.SetBkMode(TRANSPARENT);

	switch(theApp.m_Scope.m_nTraceDelayUnits) {
	case 0:	BuffDelay.Format(_T("%.0f"), (float)pAdc->nDelay);
		BuffDelay += m_Units[0];
		break;
	case 1:	BuffDelay.Format(_T("%.02f"), (float)pAdc->nDelay / 1000);
		BuffDelay += m_Units[1];
		break;
	case 2:	BuffDelay.Format(_T("%.01f"),(float)(pAdc->nDelay*theApp.m_LastSettings.nWaterVelocity)/2e6);
		BuffDelay+=m_Units[2];
		break;
	case 3:	BuffDelay.Format(_T("%.01f"),(float)(pAdc->nDelay*theApp.m_LastSettings.nMaterialVelocity)/2e6);
		BuffDelay+=m_Units[2];
		break;
	case 4:	BuffDelay.Format(_T("%.03f"),(float)(pAdc->nDelay*theApp.m_LastSettings.nWaterVelocity)/50.8e6);
		BuffDelay+=m_Units[3];
		break;
	case 5:	BuffDelay.Format(_T("%.03f"),(float)(pAdc->nDelay*theApp.m_LastSettings.nMaterialVelocity)/50.8e6);
		BuffDelay+=m_Units[3];
		break;
	}
	switch(theApp.m_Scope.m_nTraceWidthUnits) {
	case 0:	BuffWidth.Format(_T("%.01f"), (float)pAdc->nWidth/10);
		BuffWidth += m_Units[0];
		break;
	case 1:	BuffWidth.Format(_T("%.03f"), (float)pAdc->nWidth / 10000);
		BuffWidth += m_Units[1];
		break;
	case 2:	BuffWidth.Format(_T("%.01f"),(float)(pAdc->nWidth*theApp.m_LastSettings.nWaterVelocity)/20e6);
		BuffWidth+=m_Units[2];
		break;
	case 3:	BuffWidth.Format(_T("%.01f"),(float)(pAdc->nWidth*theApp.m_LastSettings.nMaterialVelocity)/20e6);
		BuffWidth+=m_Units[2];
		break;
	case 4:	BuffWidth.Format(_T("%.03f"),(float)(pAdc->nWidth*theApp.m_LastSettings.nWaterVelocity)/508e6);
		BuffWidth+=m_Units[3];
		break;
	case 5:	BuffWidth.Format(_T("%.03f"),(float)(pAdc->nWidth*theApp.m_LastSettings.nMaterialVelocity)/508e6);
		BuffWidth+=m_Units[3];
		break;
	}
	BuffWidth+="/div";
	Buff=BuffDelay+"   "+BuffWidth;

	float fPath;
	if(theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_WATER_PATH) {
		fPath=theApp.m_UtUser.GetGateWaterPath(nTimeSlot);
		CString BuffPath;
		Language.LoadString(IDS_WP);
		if(theApp.m_LastSettings.nMeasurementUnits == 0) {
			BuffPath.Format(_T(" %s=%.*fmm"),Language,theApp.m_nWpPrecision,fPath);
		} else {
			BuffPath.Format(_T(" %s=%.*f\x022"),Language,theApp.m_nWpPrecision,fPath/25.4f);
		}
		Buff += BuffPath;
	}
	if(theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_MATERIAL_PATH) {
		fPath=theApp.m_UtUser.GetGateThickness(nTimeSlot,1) * 1e6f;
		CString BuffPath;
		Language.LoadString(IDS_MP);
		if(theApp.m_LastSettings.nMeasurementUnits == 0) {
			BuffPath.Format(_T(" %s=%.02fmm"),Language,fPath);
		} else {
			BuffPath.Format(_T(" %s=%.04f\x022"),Language,fPath * theApp.m_fUnits);
		}
		Buff += BuffPath;
	}
	if(theApp.m_Scope.m_bTimeSlotName) {
		Language.LoadString(IDS_Probe);
		Buff += L" " + Language + L"=";
		Buff += theApp.m_UtUser.m_TS[nTimeSlot].cName;
	}

	if((theApp.m_bTracking==TRUE) && (theApp.m_nTrackingTS == nTimeSlot)) {
		Buff += ".";
	}

	size=dcMem.GetTextExtent(Buff);
	dcMem.TextOut(rr.Width()/2-size.cx/2,0,Buff);

	CString Gain;
	Gain.LoadString(IDS_Gain);
	Buff.Format(_T("%s=%.01f + %.01fdB"),Gain,theApp.m_UtUser.m_TS[nTimeSlot].Pr30.fLinGain, (float)theApp.m_UtUser.m_TS[nTimeSlot].Pr30.nExtraLinGain /10.0f);
	size = dcMem.GetTextExtent(Buff);
	dcMem.TextOut(rr.right - size.cx - 12,rr.top,Buff);


	Buff = "TOF 8 = 99999mm";
	size = dcMem.GetTextExtent(Buff);
	int nGStop,nGStart;
	int nGate=1;
	int nPath;
	if(theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_COLLECT_TEXT) {
		for(gg=0;gg<8;gg++) {
			if((pGate->nGateMode[gg]>0) && (pGate->nCollectAmp[gg])) {
				switch(pAdc->nRfType) {
				case 0:
				case 1:
					if(gg==0) {
						Language.LoadString(IDS_Interface);
						Buff.Format(_T("%s=%%%d"),Language,MulDiv(pGate->nAmplitude[gg],100,128));
					} else {
						Language.LoadString(IDS_Gate);
						switch(theApp.m_Palette.m_nAmplitudeScaleType[gg]) {
						default: Buff.Format(_T("%s %d=%%%d"),Language,gg,MulDiv(pGate->nAmplitude[gg],100,128));
							break;
/*						case 3:dFBH = theApp.m_Palette.FBHFromAmplitude(MulDiv(pGate->nAmplitude[gg],100,127));
							if(pGate->nAmplitude[gg]>=127) {
								Buff.Format(_T("%s %d>%.1fmm"),Language,gg,dFBH);
							} else {
								Buff.Format(_T("%s %d=%.1fmm"),Language,gg,dFBH);
							}
							break;
							*/
						case 4: //to shut the compiler up;
							break;
						}
					}
					break;
				case 2:
					if(gg==0) {
						Language.LoadString(IDS_Interface);
						Buff.Format(_T("%s=%ddB"),Language,-theApp.m_LastSettings.nLogRange-MulDiv(pGate->nAmplitude[gg],-theApp.m_LastSettings.nLogRange,127));
/*						Buff.Format(_T("%s=%.01fdB"),Language,-theApp.m_LastSettings.nLogRange-((float)pGate->nAmplitude[gg]*(float)-theApp.m_LastSettings.nLogRange)/1277.0f);
*/					} else {
						Language.LoadString(IDS_Gate);
						Buff.Format(_T("%s %d=%ddB"),Language,gg,-theApp.m_LastSettings.nLogRange-MulDiv(pGate->nAmplitude[gg],-theApp.m_LastSettings.nLogRange,127));
					}
					break;
				}
				dcMem.TextOut(rr.right - size.cx - 12,rr.top+size.cy*nGate,Buff);
/*				dcMem.TextOut(0,rr.top+size.cy*nGate,Buff);
*/				nGate++;
			}
		}


	}
	if(theApp.m_Scope.m_nTOFText[1]>0) {
		for(gg=1;gg<8;gg++) {
			if(pGate->nCollectTime[gg]>0) {
				nGStop = pGate->nTOFStopGate[gg];
				nGStart = pGate->nTOFStartGate[gg];
				MinMax(&nGStart,0,7);
				MinMax(&nGStop,0,7);
				nPath = (int)((float)(pGate->nTimeSample[nGStop] - pGate->nTimeSample[nGStart-1]) * pAdc->fSamplePeriod);

				switch(theApp.m_Scope.m_nTOFText[1]) {
				case 0:
				case 1:
					break;
				default:
					nPath /= 2;
					break;
				}
				theApp.TimeToThickness(nPath, NULL, theApp.m_Scope.m_nTOFText[1], &Temp);
				Buff.Format(_T("TOF %d=%s"), gg, Temp);
				dcMem.TextOut(rr.right - size.cx - 12,rr.top+size.cy*nGate,Buff);
				nGate++;
			}
		}
	}

	for(gg=0;gg<8;gg++) {
		if(pGate->nCollectPhase[gg]>0) {
			if(pGate->nPhase[gg]==0) {
				Buff.Format(L"%s +ve",pGate->cName[gg]);
			} else {
				Buff.Format(L"%s -ve",pGate->cName[gg]);
			};
			dcMem.TextOut(rr.right - size.cx - 12,rr.top+size.cy*nGate,Buff);
			nGate++;
		}
	}
	
	/*
	int nAmp;
	if(nTimeSlot==0) {
		if(theApp.m_UtUser.m_TS[0].Pr30.nOutputSelect==2) {
			nAmp = -theApp.m_LastSettings.nLogRange-MulDiv(pGate->nAmplitude[0],-theApp.m_LastSettings.nLogRange,m_nMaxSample);
			if(theApp.m_bScanning==TRUE) {
				theApp.m_UtUser.m_TS[0].Pr30.fLogGain += (float)(m_nNominalAmp-nAmp) / 5.0f;
				theApp.m_UtUser.Pr30LogFloatGain(&theApp.m_UtUser.m_TS[0].Pr30);
			} else {
				m_nNominalAmp = nAmp;
				m_fNominalGain = theApp.m_UtUser.m_TS[0].Pr30.fLogGain;
			}
		}
	}
*/
	bool bMultipleTS = false;
	if((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) bMultipleTS=true;
	c_f = clock();
	double dElapsedTime = (double)(c_f-c_i)/CLOCKS_PER_SEC;

	if(dElapsedTime > 0.5) {
		int nConversions = theApp.m_Acqiris.m_nConversions - m_nOldConversions;
		c_i = c_f;
		m_nOldConversions = theApp.m_Acqiris.m_nConversions;
		theApp.m_nRealTotalPRF = (int)((double)nConversions/dElapsedTime);
		if((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) {
			if(theApp.m_LSA.m_nActualLawsL>0){
				theApp.m_nRealPRFPerTS = (theApp.m_nRealTotalPRF / theApp.m_LSA.m_nActualLawsL);
			}
		}else{
			theApp.m_nRealPRFPerTS = theApp.m_nRealTotalPRF / theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;
		}
	}


	RenderStatistics(&dcMem, rr, pAdc);
	DrawDacPts(&dcMem,rr);
	DrawGates(&dcMem,rr);
	DrawTimsGates(&dcMem,rr);
	DrawRuler(&dcMem, rr);

	if(m_bRecording==TRUE) {
		DrawRecordingControls(&dcMem,rr);
	}
	switch(theApp.m_Scope.m_nPlayRecording[nTimeSlot]) {
	case 1:
	case 2:
		DrawPlayBackControls(&dcMem,rr);
		break;
	}


	if(nDisplayDevice==SCREEN) {
		pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	} else {
		int nBytesPixel = dcMem.GetDeviceCaps(BITSPIXEL) / 8;
		COLORREF *pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		bmp.GetBitmapBits((DWORD)(rr.Width()*rr.Height()*nBytesPixel),pArray);
		char* Buff;
		int	yy;

		for(yy=rr.Height()-1;yy>=0;yy--) {
			Buff = (char *) &(pArray[yy*rr.Width()]);
			pFile->Write(Buff, rr.Width() * (sizeof COLORREF));
		};
		delete pArray;
	}

	dcMem.SelectObject(pOldBitmap);
	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldFont);
//	memcpy(nOldGArray,nGArray,sizeof nGArray);
//	delete pArray;


	gg=ReleaseDC(pDC);
}

void CScopeDlg::RenderStatistics(CDC* pDC, CRect rr, ADC200Data* pAdc)
{
	CString Buff;
	int nRow = 1;
	CSize size;
	bool bMultipleTS = false;

	if (theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_SAMPLE_RATE) {
		Buff.Format(_T("Sample Rate %.0fMSPS"), (1000.0 / pAdc->fSamplePeriod));
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(0, rr.bottom - (size.cy*nRow), Buff);
		nRow++;
	}

	if (theApp.m_LastSettings.nScopeMask & SCOPE_SAMPLES_PER_TRACE) {
		Buff.Format(_T("Array Length %d"), pAdc->nAcqLength);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(0, rr.bottom - (size.cy * nRow), Buff);
		nRow++;
	};

	if (theApp.m_LastSettings.nScopeMask & SCOPE_RENDER_CONVERSION_RATE) {
		if (bMultipleTS) {
			Buff.Format(_T("Total PRF %d, %d per FL"), theApp.m_nRealTotalPRF, theApp.m_nRealPRFPerTS);
		}
		else {
			Buff.Format(_T("PRF %d Hz"), theApp.m_nRealTotalPRF);
		}
		Buff.Format(L"Timeslot conversions %d", theApp.m_UtUser.m_TS[m_nTimeSlot].nIrqCount);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(0, rr.bottom - (size.cy * nRow), Buff);
		nRow++;
	}

}

void CScopeDlg::DrawRuler(CDC *pDC, CRect rr)
{
	CString Buff;
	int nTimeSlot = m_nTimeSlot;
	float fTotalWidth;

	if ((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY)) {
		nTimeSlot = theApp.m_LSA.m_nScopeViewLaw;
	}

	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
	GatesData* pGates = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;

	int nMask = theApp.m_Scope.m_nRulerDisplayMask;
	nMask >>= (m_nTimeSlot * 2);
	nMask &= 0x03;

	if (nMask) {
		CPen penGrey(PS_SOLID, 1, RGB(200, 200, 200));
		CPen* pOldPen = pDC->SelectObject(&penGrey);
		CPoint ptLine[2];
		LOGFONT lf;
		memset((void*)&lf, 0, sizeof(lf));
		lf.lfHeight = theApp.m_Scope.m_nTextSize;
		theApp.SetFontLanguage(&lf);
		CFont Font;
		Font.CreateFontIndirect(&lf);
		CFont *pOldFont = pDC->SelectObject(&Font);
		float fSmallestStep, fStep;
		int nCount, nPtr, nArraySize;


		//Material right hand part of ruler
		ptLine[0].y = ptLine[1].y = rr.bottom - rr.Height() / 10;
		ptLine[0].x = MulDiv(pGates->nTimeSample[0], rr.Width(), pAdc->nAcqLength);
		ptLine[1].x = rr.Width();
		pDC->MoveTo(ptLine[0]);
		pDC->LineTo(ptLine[1]);

		fTotalWidth = (float)(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nMaterialVelocity) / 2e6f;
		switch (theApp.m_LastSettings.nMeasurementUnits) {
		case 0:	fStep = fSmallestStep = 1.0f;
			break;
		case 1:	fStep = fSmallestStep = 2.54f;
			break;
		}

		nArraySize = (int)(fTotalWidth / fSmallestStep);
		if (nArraySize > 0) {
			nArraySize *= 2;
			CPoint *pt = new CPoint[nArraySize + 4];
			BYTE *pByte = new BYTE[nArraySize + 4];

			for (nCount = 0, nPtr = 0; nPtr < nArraySize && fStep < fTotalWidth; nCount++) {

				fStep = (float)nCount * fSmallestStep;

				pt[nPtr].x = ptLine[0].x + (int)(fStep * (float)rr.Width() / fTotalWidth);	pt[nPtr].y = ptLine[0].y; pByte[nPtr++] = PT_MOVETO;
				if (pt[nPtr - 1].x < rr.Width()) {

					if (nCount % 10 == 0) {
						pt[nPtr].y = pt[nPtr - 1].y - 10;

						Buff.Format(L"%d", nCount);
						CSize size = pDC->GetTextExtent(Buff);
						pDC->TextOutW(pt[nPtr - 1].x - size.cx / 2, pt[nPtr].y - size.cy, Buff);
					}
					else {
						if (nCount % 5 == 0) {
							pt[nPtr].y = pt[nPtr - 1].y - 7;
						}
						else {
							pt[nPtr].y = pt[nPtr - 1].y - 5;
						}
					}
					pt[nPtr].x = pt[nPtr - 1].x;  pByte[nPtr++] = PT_LINETO;
				}
				else {
					break;
				}
			}
			pDC->PolyDraw(pt, pByte, nPtr);

			delete pt;
			delete pByte;
		}

		//Left hand part of ruler
		if (nMask & 0x02) {
			switch (theApp.m_LastSettings.nMeasurementUnits) {
			case 0:	fStep = fSmallestStep = 1.0f;
				break;
			case 1:	fStep = fSmallestStep = 2.54f;
				break;
			}

			ptLine[0].y = ptLine[1].y = rr.bottom - rr.Height() / 10;
			ptLine[1].x = MulDiv(pGates->nTimeSample[0], rr.Width(), pAdc->nAcqLength);
			ptLine[0].x = 0;
			pDC->MoveTo(ptLine[0]);
			pDC->LineTo(ptLine[1]);

			if (nMask == (int)RulerMode::WATER_MATERIAL) {
				fTotalWidth = (float)(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nWaterVelocity) / 2e6f;

				float fWaterDelay = (float)(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay*theApp.m_LastSettings.nWaterVelocity) / 2e6f;

				nArraySize = (int)(fTotalWidth / fSmallestStep);
				if (nArraySize > 0) {
					nArraySize *= 2;
					CPoint *pt = new CPoint[nArraySize + 4];
					BYTE *pByte = new BYTE[nArraySize + 4];

					fStep = (float)((int)(fWaterDelay + fSmallestStep));
					for (nCount = 1, nPtr = 0; nPtr < nArraySize && fStep < (fTotalWidth + fWaterDelay); nCount++) {

						fStep = (float)((int)((float)nCount * fSmallestStep + fWaterDelay));

						pt[nPtr].x = ptLine[0].x + (int)((fStep- fWaterDelay) * (float)rr.Width() / fTotalWidth);	pt[nPtr].y = ptLine[0].y; pByte[nPtr++] = PT_MOVETO;

						if(pt[nPtr-1].x < ptLine[1].x) {

							if(fmod(fStep,10.0f) == 0.0f) {
								Buff.Format(L"%d", (int)fStep);
								pt[nPtr].y = pt[nPtr - 1].y - 10;

								CSize size = pDC->GetTextExtent(Buff);
								pDC->TextOutW(pt[nPtr - 1].x - size.cx / 2, pt[nPtr].y - size.cy, Buff);
							}
							else {
								if (nCount % 5 == 0) {
									pt[nPtr].y = pt[nPtr - 1].y - 7;
								}
								else {
									pt[nPtr].y = pt[nPtr - 1].y - 5;
								}
							}
							pt[nPtr].x = pt[nPtr - 1].x;  pByte[nPtr++] = PT_LINETO;
						}
						else {
							break;
						}
					}

					pDC->PolyDraw(pt, pByte, nPtr);

					delete pt;
					delete pByte;
				}
			}
			else {
				fTotalWidth = (float)(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nMaterialVelocity) / 2e6f;

				nArraySize = (int)(fTotalWidth / fSmallestStep);
				if (nArraySize > 0) {
					nArraySize *= 2;
					CPoint *pt = new CPoint[nArraySize + 4];
					BYTE *pByte = new BYTE[nArraySize + 4];

					for (nCount = 1, nPtr = 0; nPtr < nArraySize && fStep < fTotalWidth; nCount++) {

						fStep = (float)nCount * fSmallestStep;

						pt[nPtr].x = ptLine[1].x - (int)(fStep * (float)rr.Width() / fTotalWidth);	pt[nPtr].y = ptLine[0].y; pByte[nPtr++] = PT_MOVETO;
						if (pt[nPtr - 1].x < rr.Width()) {

							if (nCount % 10 == 0) {
								pt[nPtr].y = pt[nPtr - 1].y - 10;

								Buff.Format(L"%d", nCount);
								CSize size = pDC->GetTextExtent(Buff);
								pDC->TextOutW(pt[nPtr - 1].x - size.cx / 2, pt[nPtr].y - size.cy, Buff);
							}
							else {
								if (nCount % 5 == 0) {
									pt[nPtr].y = pt[nPtr - 1].y - 7;
								}
								else {
									pt[nPtr].y = pt[nPtr - 1].y - 5;
								}
							}
							pt[nPtr].x = pt[nPtr - 1].x;  pByte[nPtr++] = PT_LINETO;
						}
						else {
							break;
						}
					}
					pDC->PolyDraw(pt, pByte, nPtr);

					delete pt;
					delete pByte;
				}
			}
		}

		pDC->SelectObject(pOldFont);
		pDC->SelectObject(pOldPen);
	}
}

void CScopeDlg::DrawGates(CDC *pDC,CRect rr)
{
	ADC200Data* pAdc;
	GatesData* pGates;
	DSP200Data* pDsp;
	CPen* pPen;
	CPen *pOldPen;
	int	nStart(0),nFinish(0),nPny(0),gg(0),nInterfacePos(0),nPnt(0);
	int nTimeSlot;

	if((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) {
		nTimeSlot = theApp.m_LSA.m_nScopeViewLaw;	

		pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
		pGates = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
		pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;
	} else {
		nTimeSlot = m_nTimeSlot;

		pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
		pGates = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
		pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;
	}


	for(gg=0;gg<8;gg++) {
		switch(pGates->nGateMode[gg]) {
		case 0:
			break;
		case 1:
			nStart=MulDiv(pGates->nNsDelay[gg]-pAdc->nDelay,rr.Width(),pAdc->nWidth);
			nFinish=MulDiv(pGates->nNsWidth[gg],rr.Width(),pAdc->nWidth);
			break;
		case 2:
		case 3:
			nInterfacePos=(int)((float)pGates->nTimeSample[0]*pAdc->fSamplePeriod);
			nStart=MulDiv(pGates->nNsDelay[gg]+nInterfacePos,rr.Width(),pAdc->nWidth);
			if (pGates->nTimeSample[0] >= 0) {
				nFinish = MulDiv((int)((float)pGates->nSampleWidth[gg] * pAdc->fSamplePeriod), rr.Width(), pAdc->nWidth);
			}
			else {
				nFinish = 0;
			}
			break;
		};
		switch(pAdc->nRfType) {
		case RFTYPELINEAR:
			if(pGates->nThreshold[gg]>=0) {
				nPny=rr.Height()/2-MulDiv(pGates->nAmplitude[gg],rr.Height()/2,127);
			} else {
				nPny=rr.Height()/2-MulDiv(pGates->nAmplitude[gg],rr.Height()/2,127);
			}
			nPnt=rr.Height()/2-MulDiv(pGates->nThreshold[gg],rr.Height()/2,100);
			break;
		case RFTYPERECTIFIED:
			nPny=rr.Height()-MulDiv(pGates->nAmplitude[gg],rr.Height(),127);
			nPnt=rr.Height()-MulDiv(pGates->nThreshold[gg],rr.Height(),100);
			break;
		case RFTYPELOG:
			nPny=rr.Height()-MulDiv(pGates->nAmplitude[gg],rr.Height(),127);
			nPnt=rr.Height()-MulDiv(pGates->nThreshold[gg],rr.Height(),100);
			break;
		}
		if((nFinish+nStart)>=rr.Width()) nFinish = (rr.Width() - 1) - nStart;
		if((nStart<rr.Width())&&(nFinish>=0)&&pGates->nGateMode[gg]) {
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGateColor[gg]);
			pOldPen=pDC->SelectObject(pPen);

			pDC->MoveTo(nStart,nPny);
			pDC->LineTo(nFinish+nStart,nPny);
			pDC->MoveTo(nFinish+nStart,nPnt);
			pDC->LineTo(nStart,nPnt);
//			pDC->LineTo(nStart,nPny);

			pDC->SelectObject(pOldPen);
			delete pPen;
		};
	};

}

void CScopeDlg::DrawTimsGates(CDC *pDC,CRect rr)
{
	int	gg,nPnx,nTimeSlot;
	nTimeSlot = m_nTimeSlot;

	if((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) {
		nTimeSlot = theApp.m_LSA.m_nScopeViewLaw;
	}

	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
	GatesData* pGates = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;
	CPen* pPen;
	CPen *pOldPen;

	for(gg=0;gg<8;gg++) {
		if(pGates->nTimsDetectionMode[gg]>0) {
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGateColor[gg]);
			pOldPen=pDC->SelectObject(pPen);

			nPnx=MulDiv(pGates->nTimeSample[gg],rr.Width(),pAdc->nAcqLength);
			pDC->MoveTo(nPnx,rr.top);
			pDC->LineTo(nPnx,rr.bottom);

			pDC->SelectObject(pOldPen);
			delete pPen;
		};
	}

}


void CScopeDlg::DrawGraticule(CDC *pDC,CRect rr)
{
	CPen* pPen;
	CPen *pOldPen;
	int	nPnx,nPny,nRow,nCol,nY0;
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	CString Buff;
	Buff.Format(_T("100"));
	CSize size = pDC->GetTextExtent(Buff);
	float fHDiv;


	if(!(theApp.m_LastSettings.nScopeMask & SCOPE_GRATICULE)) return;

	pDC->SetTextColor(theApp.m_Scope.m_rgbGraticuleColor);
	pDC->SetBkMode(TRANSPARENT);

	pPen = new CPen;
	pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGraticuleColor);
	pOldPen=pDC->SelectObject(pPen);

	int nMaxRows = theApp.m_LastSettings.nLogRange / 10;
	if(nMaxRows<=0) nMaxRows=1;

	switch(pAdc->nRfType) {
	case RFTYPELINEAR:
		nY0=(rr.top+rr.bottom)/2;
		for(nRow=1;nRow<4;nRow++) {
			nPny=MulDiv(nRow,rr.Height()/2,4);
			pDC->MoveTo(0,nY0-nPny);
			pDC->LineTo(rr.right,nY0-nPny);
			pDC->MoveTo(0,nY0+nPny);
			pDC->LineTo(rr.right,nY0+nPny);
			if(theApp.m_Scope.m_bLeftScale==TRUE) {
				Buff.Format(_T("%d %%"),nRow*25);
				pDC->TextOut(0,nY0+nPny,Buff);
				pDC->TextOut(0,nY0-nPny-size.cy,Buff);
			}
			if(theApp.m_Scope.m_bRightScale==TRUE) {
				Buff.Format(_T("%d %%"),nRow*25);
				size=pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.right-size.cx-20,nY0+nPny,Buff);
				pDC->TextOut(rr.right-size.cx-20,nY0-nPny-size.cy,Buff);
			}
		};
		break;
	case RFTYPERECTIFIED:
		for(nRow=1;nRow<10;nRow++) {
			nPny=MulDiv(nRow,rr.Height(),10);
			pDC->MoveTo(0,nPny+rr.top);
			pDC->LineTo(rr.right,nPny+rr.top);
			if(theApp.m_Scope.m_bLeftScale==TRUE) {
				Buff.Format(_T("%d %%"),100-nRow*10);
				pDC->TextOut(0,nPny+rr.top-size.cy,Buff);
			}
			if(theApp.m_Scope.m_bRightScale==TRUE) {
				Buff.Format(_T("%d %%"),100-nRow*10);
				size=pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.right-size.cx-20,nPny+rr.top-size.cy,Buff);
			}
		};
		break;
	case RFTYPELOG:
		for(nRow=1;nRow<=nMaxRows;nRow++) {
			nPny=MulDiv(nRow*10,rr.Height(),theApp.m_LastSettings.nLogRange);
			pDC->MoveTo(0,nPny+rr.top);
			pDC->LineTo(rr.right,nPny+rr.top);
			if(theApp.m_Scope.m_bLeftScale==TRUE) {
				Buff.Format(_T("%ddB"),(nRow)*10);
				pDC->TextOut(0,nPny+rr.top-size.cy,Buff);
			}
			if(theApp.m_Scope.m_bRightScale==TRUE) {
				Buff.Format(_T("%ddB"),(nRow)*10);
				size=pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.right-size.cx-20,nPny+rr.top-size.cy,Buff);
			}
		};
		break;
	};

	switch(theApp.m_Scope.m_nTraceWidthUnits) {
	case 0:	fHDiv = (float)theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth/10000;
		break;
	case 1:	fHDiv = (float)(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nWaterVelocity)/20e6f;
		break;
	case 2:	fHDiv = (float)(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nMaterialVelocity)/20e6f;
		break;
	}

	for(nCol=1;nCol<10;nCol++) {
		nPnx=MulDiv(nCol,rr.Width(),10);
		pDC->MoveTo(nPnx,rr.top);
		pDC->LineTo(nPnx,rr.bottom);
		if(theApp.m_Scope.m_bBottomScale) {
			Buff.Format(_T("%.01f"),(float)nCol * fHDiv);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnx-size.cx,rr.bottom-size.cy,Buff);
		}
		if(theApp.m_Scope.m_bTopScale) {
			Buff.Format(_T("%.01f"),(float)nCol * fHDiv);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnx-size.cx,rr.top+size.cy,Buff);
		}
	};


	pDC->SelectObject(pOldPen);
	delete pPen;
}


void CScopeDlg::DrawRecordingControls(CDC *pDC,CRect rr)
{
	CString Buff;
	float fTotal = ((float)m_nPtrRecording * (float)theApp.m_UtUser.m_TS[0].Adc.nAcqLength) / 1e6f;
	Buff.Format(_T("%.01f MByte"),fTotal);
	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.Width()-size.cx,0,Buff);
}

void CScopeDlg::DrawPlayBackControls(CDC *pDC,CRect rr)
{
	CPen penYellow(PS_SOLID,1,RGB(255,255,0));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen *pOldPen = pDC->SelectObject(&penYellow);
	CBrush brushRed(RGB(255,0,0));
	CString Buff = L"X";
	CSize size = pDC->GetTextExtent(Buff);
	int nRadius = size.cy/2-2;
	if(nRadius<4) nRadius=4;
	int	nX0,nX1,nY;
	m_rectPlayBar.left = MulDiv(rr.Width(),85,100);
	m_rectPlayBar.right = rr.Width()-nRadius*2-2;
	m_rectPlayBar.top=0;
	m_rectPlayBar.bottom=nRadius*2;
	nY=size.cy/2;
	pDC->MoveTo(m_rectPlayBar.left,nY);
	pDC->LineTo(m_rectPlayBar.right,nY);
	int nXp = MulDiv(m_nPtrRecording,m_rectPlayBar.right-m_rectPlayBar.left,m_nRecordingL) + m_rectPlayBar.left;
	pDC->MoveTo(nXp,size.cy);
	pDC->LineTo(nXp,0);
	pDC->Ellipse(nXp-4,nY-4,nXp+4,nY+4);


	//Draw the stop button
	nX1=m_rectPlayBar.right+2;
	pDC->Ellipse(nX1,nY-nRadius,rr.Width(),nY+nRadius);
	CRect rrStop(nX1+nRadius-3,nY-3,nX1+nRadius+3,nY+3);
	pDC->FillRect(rrStop,&brushRed);

	nX0=m_rectPlayBar.left-2-nRadius*2;
	nX1=nX0+nRadius*2;
	pDC->Ellipse(nX0,nY-nRadius,nX1,nY+nRadius);
	nX0=(nX0+nX1)/2;
	pDC->SelectObject(&penBlack);
	switch(theApp.m_Scope.m_nPlayRecording[m_nTimeSlot])	{//Playing
	case 1:
		pDC->MoveTo(nX0-3,nY-3);
		pDC->LineTo(nX0-3,nY+3);
		pDC->MoveTo(nX0-2,nY-3);
		pDC->LineTo(nX0-2,nY+3);

		pDC->MoveTo(nX0+2,nY-3);
		pDC->LineTo(nX0+2,nY+3);
		pDC->MoveTo(nX0+1,nY-3);
		pDC->LineTo(nX0+1,nY+3);
		break;
	case 2:
		pDC->MoveTo(nX0-2,nY-4);
		pDC->LineTo(nX0-2,nY+4);
		pDC->MoveTo(nX0-1,nY-3);
		pDC->LineTo(nX0-1,nY+3);
		pDC->MoveTo(nX0-0,nY-2);
		pDC->LineTo(nX0-0,nY+2);
		pDC->MoveTo(nX0+1,nY-1);
		pDC->LineTo(nX0+1,nY+1);
		break;
	}

	pDC->SelectObject(pOldPen);
}


UINT OscilloscopeThread(LPVOID pParam)
{
	CScopeDlg* pParent = (CScopeDlg *)pParam;

	if (pParent->GetSafeHwnd() && pParent->IsWindowVisible() == TRUE) {

		if (WaitForSingleObject(pParent->m_hSemaphore, 10) == WAIT_OBJECT_0) {
			pParent->SendMessage(UI_REFRESH_SCOPE_DISPLAY, UI_REFRESH_SCOPE_DISPLAY);
		}
	};
	return THREAD_CONTINUE;
}


HRESULT CScopeDlg::RefreshScopeDisplay(WPARAM, LPARAM)
{
	int nTS;
	bool bTSChanged=false;

	//This is where the thread re-writes the hardware if the scroll bar is moved
	if((theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay-m_nTraceDelay) ||
		(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth-m_nTraceWidth) || (bTSChanged) ){
		m_nTraceDelay=theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay;
		m_nTraceWidth=theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth;
		switch(theApp.m_LastSettings.nAdcCardType) {
		case USL_ADC200:
			theApp.m_UtUser.Adc200CalculateHardware();
			theApp.m_UtUser.Adc200Delay(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
			break;
		case USL_ADC100:
			theApp.m_UtUser.Adc100CalculateHardware(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
			theApp.m_UtUser.Adc100ConvTypeFreq(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
			theApp.m_UtUser.Adc100Delay(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
			break;
		case Acqiris_U1071AHZ2:
		case Acqiris_U1071AHZ4:
		case Acqiris_DP1400:
		case Acqiris_DP210:
			theApp.m_Acqiris.Configure();
			break;
		case Alazar_9626:
		case Alazar_9462:
		case Alazar_860:
			theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc);
			break;
		case SpectrumM3i_2120:
		case SpectrumM3i_2122:
		case SpectrumM3i_2130:
		case SpectrumM3i_2132:
			theApp.m_Spectrum.m_bTimeBaseHW = true;
			break;
		case USL_PR40:
			theApp.m_PM40User.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[m_nTimeSlot], 0);
			break;
		}

		theApp.m_PhasedArray[PORTSIDE].setWidthDelay();

		for(nTS=0;nTS<256;nTS++) {
			theApp.m_UtUser.Dsp200CalculateHardware(nTS);
		}
		CalculateScrollBarFromDelayWidth();
	};

	CalculateGArray();
	DrawGTrace();
	theApp.LinearTracker();
	ReleaseSemaphore(m_hSemaphore,1,NULL); //Increment Semaphore count so it's signalled
	return NULL;
}



void CScopeDlg::SetRfInputType(int nType)
{
	theApp.m_Scope.m_nRfType=nType;
}



bool CScopeDlg::FindNearestGate(CPoint point, CRect rr, int *nGate, int *nEnd)
{
	int	nStart,nFinish,gg,nMaxDx,nTemp;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	GatesData* pGates = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp;
	bool bFlag=FALSE;

	point.x-=rr.left;
	point.y-=rr.top;
	nMaxDx=8000;

	for(gg=0;gg<8;gg++) {
		switch(pGate->nGateMode[gg]) {
		case 0:
			break;
		case 1:
			nStart=MulDiv(pGates->nNsDelay[gg]-pAdc->nDelay,rr.Width(),pAdc->nWidth);
			nFinish=MulDiv(pGates->nNsWidth[gg],rr.Width(),pAdc->nWidth);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		case 2:
			nStart = MulDiv(pGates->nSampleDelay[gg]+pGates->nTimeSample[0],rr.Width(),pAdc->nAcqLength);
			nFinish = MulDiv(pGates->nSampleWidth[gg], rr.Width(), pAdc->nAcqLength);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
		default:
			nStart = MulDiv(pGates->nSampleDelay[gg]+pGates->nTimeSample[0],rr.Width(),pAdc->nAcqLength);
			nFinish = MulDiv(pGates->nSampleWidth[gg], rr.Width(), pAdc->nAcqLength);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		};
	};
	return bFlag;
}

void CScopeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int	nGate,nEnd;
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		if(IsPlayBack(point,rr)) return;
		if(FindNearestDacPt(point,rr, &m_nWhichDacPt) == TRUE) {
			m_bModifyDacPosition=TRUE;
		} else {
			if(FindNearestGate(point, rr, &nGate, &nEnd)==TRUE) {
				m_nGate=nGate;
				m_nEnd=nEnd;
				UpdateAllControls();
				bModifyGatePosition=TRUE;
			}
		}
	} else {
//		theApp.m_bTracking^=1;
//		theApp.m_nTrackingTS = m_nTimeSlot;
//		theApp.m_nTrackingAmp[m_nTimeSlot] = theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nAmplitude[0];
	}




	CDialog::OnLButtonDown(nFlags, point);
}

void CScopeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int nTimeSlot = m_nTimeSlot;

	if((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) {
		nTimeSlot = theApp.m_LSA.m_nScopeViewLaw;
		nTimeSlot = 0;
	}

	CRect	rr;
	int	nStart,nFinish,nInterfacePos;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
	GatesData* pGates = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;
	float fGain;
	static CPoint ptLast;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		point.x-=rr.left;
		point.y-=rr.top;

		if((theApp.m_LastSettings.nScanMask & _LOCK_UT) && (theApp.m_pScanData != NULL)) {
			bModifyGatePosition = false;
		}
		if(bModifyGatePosition==TRUE) {

			switch(pGates->nGateMode[m_nGate]) {
			case 0:
				break;
			case 1:
				nStart=MulDiv(point.x,pAdc->nWidth,rr.Width())+pAdc->nDelay;
				nFinish=MulDiv(point.x,pAdc->nWidth,rr.Width())+pAdc->nDelay;
				if(m_nEnd==0) pGates->nNsDelay[m_nGate]=nStart;
				if(m_nEnd==1) pGates->nNsWidth[m_nGate]=nFinish-pGates->nNsDelay[m_nGate];
				break;
			case 2:
				nInterfacePos=(int)((float)pDsp->InterfacePos*pAdc->fSamplePeriod);
				nStart=MulDiv(point.x,pAdc->nWidth,rr.Width())-nInterfacePos;
				nFinish=MulDiv(point.x,pAdc->nWidth,rr.Width())-nInterfacePos;
				if(m_nEnd==0) pGates->nNsDelay[m_nGate]=nStart;
				if(m_nEnd==1) pGates->nNsWidth[m_nGate]=nFinish-pGates->nNsDelay[m_nGate];
			default:
				nInterfacePos=(int)((float)pDsp->InterfacePos*pAdc->fSamplePeriod);
				nStart=MulDiv(point.x,pAdc->nWidth,rr.Width())-nInterfacePos;
				nFinish=(int)((float)pGates->nTimeSample[pGates->nGateMode[m_nGate]-2]*pAdc->fSamplePeriod) - MulDiv(point.x,pAdc->nWidth,rr.Width());
				if(m_nEnd==0) {
					pGates->nNsDelay[m_nGate]=nStart;
				}
				if(m_nEnd==1) pGates->nNsFarDeadZone[m_nGate]=nFinish;
				
				break;
			}
			switch(pAdc->nRfType) {
			case RFTYPELINEAR:
				pGates->nThreshold[m_nGate]=MulDiv(rr.Height()/2-point.y,100,rr.Height()/2);
				break;
			case RFTYPERECTIFIED:
			case RFTYPELOG:
				pGates->nThreshold[m_nGate]=MulDiv(rr.Height()-point.y,100,rr.Height());
				break;
			};
			UpdateAllControls();
			RefreshLockVariables();
			CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

			theApp.m_UtUser.Dsp200CalculateHardware(nTimeSlot,m_nGate);

			if (((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY)) || theApp.m_AOSPhasedArray.IsConnected()) {
				if (theApp.m_PhasedArray[PORTSIDE].getDacMode() == 2) {
					theApp.m_PhasedArray[PORTSIDE].setDacInterfaceGate(true);
				}
			}

			if(pFrame->m_pUltrasonicsSheet) {
				if(pFrame->m_pUltrasonicsSheet->m_pGatesPage) {
					pFrame->m_pUltrasonicsSheet->m_pGatesPage->SetGateTimeSlot(m_nGate,nTimeSlot);
					pFrame->m_pUltrasonicsSheet->m_pGatesPage->UpdateAllControls();
				}
			}
			CopyGates();
		};

		if((m_bModifyDacPosition==TRUE) && (ptLast != point)) {

			switch(pAdc->nRfType) {
			case RFTYPELINEAR:
				fGain =	((float)(((rr.top+rr.bottom)/2)-point.y) * 100.0f) / (float)(rr.Height()/2);
				break;
			case RFTYPERECTIFIED:
			case RFTYPELOG:
				fGain =	((float)(rr.bottom-point.y) * 100.0f) / (float)rr.Height();
				break;
			}
			fGain = (float)(20.0 * log10(theApp.m_LastSettings.fDACRefAmplitude/fGain));

			Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[nTimeSlot].Pr30;
			int	nT = pPr30->nDacTable;
			pPr30->fDacGain[nT][m_nWhichDacPt]=fGain;
			DacTableChanged();
			ptLast = point;
			CopyDAC();
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CScopeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	bModifyGatePosition=FALSE;
	m_bModifyDelayWidth=FALSE;
	m_bModifyDacPosition=FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

BEGIN_EVENTSINK_MAP(CScopeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CScopeDlg)
	ON_EVENT(CScopeDlg, IDC_USLBARCTRL, 1 /* ScrollHasMoved */, OnScrollHasMovedUslbarctrl, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CScopeDlg::OnScrollHasMovedUslbarctrl(long LowPosition, long HighPosition, long Width)
{
	if((theApp.m_LastSettings.nScanMask & _LOCK_UT) && (theApp.m_pScanData != NULL)) {
		return;
	}

	CalculateDelayWidthFromScrollBar();
	RefreshLockVariables();

}



bool CScopeDlg::CalculateScrollBarFromDelayWidth()
{

	float fMax = (theApp.m_Scope.m_fMaxRange / 1e-9f) * (theApp.m_Scope.m_fMaxRange / 1e-9f);

	float fDelay=((float)theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay* (theApp.m_Scope.m_fMaxRange / 1e-9f)) / (theApp.m_Scope.m_fMaxRange / 1e-9f);
	float fWidth=((float)theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth*fMax) / (theApp.m_Scope.m_fMaxRange / 1e-9f);

	fWidth = (float)sqrt(fWidth);

	m_scrollbarDelayWidth.SetLowPosition((int)fDelay);
	m_scrollbarDelayWidth.SetHighPosition((int)fDelay + (int)fWidth);
	Invalidate(false);

	
	return TRUE;
}


bool CScopeDlg::CalculateDelayWidthFromScrollBar()
{

	//If collecting volume data don't allow user to change scope delay/width
	if((theApp.m_Thread.m_nThreadAction==CONTOUR_SCAN) && (theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nBscanMode)){
		CalculateScrollBarFromDelayWidth();
		return FALSE;
	}


	float fDelay = (float)m_scrollbarDelayWidth.GetLowPosition();
	float fWidth = (float)m_scrollbarDelayWidth.GetHighPosition() - fDelay;

	float fMax = (theApp.m_Scope.m_fMaxRange / 1e-9f) * (theApp.m_Scope.m_fMaxRange / 1e-9f);
	fWidth *= fWidth;

	int nDelay = (int)((fDelay * (theApp.m_Scope.m_fMaxRange/1e-9f)) / (theApp.m_Scope.m_fMaxRange/1e-9f));
	int nWidth = (int)((fWidth * (theApp.m_Scope.m_fMaxRange/1e-9f)) / fMax);

	theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay = nDelay;
	theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth = nWidth;
	//Hardware rewritten in thread

	CopyScopeDelayWidth();


	return TRUE;
}

void CScopeDlg::DrawDacCurve(CDC *pDC, CRect rr)
{

	int	ptr,xx,vv,jj;

	if(GetSafeHwnd() == NULL) return;

	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;


	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(pAdc->nAcqLength==0) pAdc->nAcqLength=1;
	for(xx=0;xx<rr.Width();xx++) {
		ptr=MulDiv(xx,pAdc->nAcqLength,rr.Width());
		vv=rr.Height() + MulDiv(Trace2[ptr],rr.Height(),-128);
		if(vv<0) vv=0;
		if(vv>rr.Height()-1) vv=rr.Height()-1;
		nGArray2[xx]=vv;
	};
	pDC->MoveTo(0,nGArray2[0]);
	for(jj=0;jj<rr.Width();jj++) {
		pDC->LineTo(jj,nGArray2[jj]);
	};


}

void CScopeDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* menu = (CMenu *) new CMenu;
	m_ptClient = point;
	CRect rr;

	m_staticView.GetWindowRect(rr);
	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB_SCOPE_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);

			UINT id = pPopup->GetMenuItemID(0);
			if(m_bDelayLockedToZ==TRUE) {
	//			pPopup->ModifyMenu(id,MF_BYCOMMAND,id,"UnLock from Z");
	//			pPopup->CheckMenuItem(0,MF_ENABLED|MF_BYPOSITION|MF_CHECKED);
			} else {
	//			pPopup->ModifyMenu(id,MF_BYCOMMAND,id,"Lock to Z");
			}
	//		if(m_bRecording==FALSE) {
	//			pPopup->EnableMenuItem( 10   , MF_GRAYED|MF_BYPOSITION);
	//		} else {
	//			pPopup->EnableMenuItem( 9   , MF_GRAYED|MF_BYPOSITION);
	//		}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

		}
	} else {
		GetWindowRect(rr);
		rr.right = rr.left+20;
		rr.bottom = rr.top + 20;
		if(rr.PtInRect(point)) {
			theApp.m_bTracking^=1;
			theApp.m_nTrackingTS = m_nTimeSlot;
			theApp.m_nTrackingAmp[m_nTimeSlot] = theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nAmplitude[0];
		}
	}

	delete menu;

}

void CScopeDlg::OnScopeLockdelaytoz()
{

	if(m_bDelayLockedToZ==FALSE) {
		m_bDelayLockedToZ=TRUE;
		RefreshLockVariables();
	} else {
		m_bDelayLockedToZ=FALSE;
	};

}


void CScopeDlg::CheckLockDelayFromZHeight()
{
	CCoord Cp;
	int nDeltaDelay;
	float dDeltaZ;
	static int nOldDeltaDelay=0;

	if(m_bDelayLockedToZ == FALSE) return;

	theApp.m_Si6User.GetCoord(&Cp);
	dDeltaZ = Cp.Side0.fZ - (float)m_fInitialZPosLock;

	nDeltaDelay = (int)(dDeltaZ * 2e6) / theApp.m_LastSettings.nWaterVelocity;
	if(nOldDeltaDelay - nDeltaDelay) {
		nOldDeltaDelay = nDeltaDelay;
		theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay = m_nScopeDelayLock - nDeltaDelay;
		if(theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay<0) theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay=0;
		CalculateScrollBarFromDelayWidth();
		Invalidate(FALSE);
		//Hardware rewritten in thread

		switch(theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode[0]) {
		case 0:
			break;
		case 1:
			theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay[0] = m_nInterfaceDelayLock - nDeltaDelay;
			if(theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay[0]<0) theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay[0]=0;
			theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,0);
			break;
		}
	}
}

void CScopeDlg::RefreshLockVariables()
{
	CCoord	Cp;

	m_nScopeDelayLock = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay;
	m_nInterfaceDelayLock = theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay[0];
//	theApp.m_Si6User.GetCoord(&Cp);
	m_fInitialZPosLock = Cp.Side0.fZ;

}

void CScopeDlg::OnScopeDacrefamplitude()
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	ScreenToClient(&m_ptClient);

	switch(pAdc->nRfType) {
	case RFTYPELINEAR:
		theApp.m_LastSettings.fDACRefAmplitude = (float)MulDiv((rr.top+rr.bottom)/2-m_ptClient.y,100,rr.Height()/2);
		break;
	case RFTYPERECTIFIED:
	case RFTYPELOG:
		theApp.m_LastSettings.fDACRefAmplitude = (float)MulDiv(rr.bottom-m_ptClient.y,100,rr.Height());
		break;
	}

}

void CScopeDlg::OnScopeStoredacpoint()
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[m_nTimeSlot].Pr30;
	CPhasedArrayProbe* pPA = &theApp.m_PhasedArray[PORTSIDE];
	int	nT = pPr30->nDacTable;
	int	nStart,ii,jj;
	CRect rr;
	float fGain;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	ScreenToClient(&m_ptClient);

	if(FindDACStartPt(&nStart) == FALSE) return;

	float fDelay = (float)(MulDiv((m_ptClient.x-rr.left),pAdc->nAcqLength,rr.Width())-nStart) * pAdc->fSamplePeriod;
	fDelay = fDelay / 1000.0f;
	switch(theApp.m_LastSettings.nDacDelayUnits) {
	case 0:
		break;
	case 1:	fDelay = fDelay * (float)theApp.m_LastSettings.nWaterVelocity / 2e3f;
		break;
	case 2:	fDelay = fDelay * (float)theApp.m_LastSettings.nMaterialVelocity / 2e3f;
		break;
	}

	switch(pAdc->nRfType) {
	case RFTYPELINEAR:
		fGain =	((float)(((rr.top+rr.bottom)/2)-m_ptClient.y) * 100.0f) / (float)(rr.Height()/2);
		break;
	case RFTYPERECTIFIED:
	case RFTYPELOG:
		fGain =	((float)(rr.bottom-m_ptClient.y) * 100.0f) / (float)rr.Height();
		break;
	}
	fGain = (float)(20.0 * log10(theApp.m_LastSettings.fDACRefAmplitude/fGain));

	if (((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY)) || theApp.m_AOSPhasedArray.IsConnected()) {
		for (ii = 0; ii < 64; ii++) {
			if (fabs(fDelay - pPA->m_fDacDelay[0][ii]) < 0.1) {
				pPA->m_fDacDelay[0][ii] = fDelay;
				pPA->m_fDacGain[0][ii] = fGain;
				DacTableChanged();
				return;
			}
		}
		for (ii = 0; ii < 63; ii++) {
			if ((fDelay > pPA->m_fDacDelay[0][ii]) && (fDelay < pPA->m_fDacDelay[0][ii + 1])) {
				for (jj = 63; jj > (ii + 1); jj--) {
					pPA->m_fDacDelay[0][jj] = pPA->m_fDacDelay[0][jj - 1];
					pPA->m_fDacGain[0][jj] = pPA->m_fDacGain[0][jj - 1];
				}
				pPA->m_fDacDelay[0][ii + 1] = fDelay;
				pPA->m_fDacGain[0][ii + 1] = fGain;
				DacTableChanged();
				return;
			}
		}
		for (ii = 1; ii < 64; ii++) {
			if (pPA->m_fDacDelay[0][ii] == 0.0) {
				pPA->m_fDacDelay[0][ii] = fDelay;
				pPA->m_fDacGain[0][ii] = fGain;
				DacTableChanged();
				return;
			}
		}
	}
	else {
		for (ii = 0; ii < 64; ii++) {
			if (fabs(fDelay - pPr30->fDacDelay[nT][ii]) < 0.1) {
				pPr30->fDacDelay[nT][ii] = fDelay;
				pPr30->fDacGain[nT][ii] = fGain;
				DacTableChanged();
				return;
			}
		}
		for (ii = 0; ii < 63; ii++) {
			if ((fDelay > pPr30->fDacDelay[nT][ii]) && (fDelay < pPr30->fDacDelay[nT][ii + 1])) {
				for (jj = 63; jj > (ii + 1); jj--) {
					pPr30->fDacDelay[nT][jj] = pPr30->fDacDelay[nT][jj - 1];
					pPr30->fDacGain[nT][jj] = pPr30->fDacGain[nT][jj - 1];
				}
				pPr30->fDacDelay[nT][ii + 1] = fDelay;
				pPr30->fDacGain[nT][ii + 1] = fGain;
				DacTableChanged();
				return;
			}
		}
		for (ii = 1; ii < 64; ii++) {
			if (pPr30->fDacDelay[nT][ii] == 0.0) {
				pPr30->fDacDelay[nT][ii] = fDelay;
				pPr30->fDacGain[nT][ii] = fGain;
				DacTableChanged();
				return;
			}
		}
	}
}

void CScopeDlg::OnScopeDisplaydacpoints()
{
	if (((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY)) || theApp.m_AOSPhasedArray.IsConnected()) {
		theApp.m_Scope.m_cDisplayDACPtsMask & 1 ? theApp.m_Scope.m_cDisplayDACPtsMask = 0 : theApp.m_Scope.m_cDisplayDACPtsMask = 1;
	}
	else {
		unsigned char cMask = 1;
		cMask <<= m_nTimeSlot;
		theApp.m_Scope.m_cDisplayDACPtsMask & cMask ? theApp.m_Scope.m_cDisplayDACPtsMask &= ~cMask : theApp.m_Scope.m_cDisplayDACPtsMask |= cMask;
	}
}


bool CScopeDlg::FindDACStartPt(int *nStart)
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	int xx;
	*nStart = -1;
	int nDacTriggerThreshold = 0;
	int nDacBlanking = 0;

	if (((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY)) || theApp.m_AOSPhasedArray.IsConnected()) {
		if (theApp.m_PhasedArray[PORTSIDE].m_nDacMode < 2) {
			*nStart = 0;
			return true;
		}
		else {
			*nStart = theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTimeSample[0];
			return theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nTimeStatus[0];
		}
	}
	else {
		nDacBlanking = theApp.m_UtUser.m_TS[m_nTimeSlot].Pr30.nDacBlanking;
		nDacTriggerThreshold = theApp.m_UtUser.m_TS[m_nTimeSlot].Pr30.nDacTriggerThreshold;
	}

	if(theApp.m_LastSettings.nDacMaxThreshold == 0) theApp.m_LastSettings.nDacMaxThreshold = 100;

	float fBlanking_ns = (float)(nDacBlanking * 100);
	xx = (int)((fBlanking_ns - (float)pAdc->nDelay) / pAdc->fSamplePeriod);

	if(xx<0) xx=0;
	
	int nThreshold = MulDiv(nDacTriggerThreshold-128,theApp.m_LastSettings.nDacMaxThreshold, 100);

	for( ;xx<(int)pAdc->nAcqLength;xx++) {
		if(theApp.m_Scope.m_RFTrace[m_nTimeSlot][xx] > nThreshold) {
			*nStart = xx;
			return TRUE;
		}
	}
	return FALSE;
}


void CScopeDlg::DrawDacPts(CDC *pDC, CRect rr)
{
	if (((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY)) || theApp.m_AOSPhasedArray.IsConnected()) {
		DrawDacPtsPhasedArray(pDC, rr);
	}
	else {
		DrawDacPtsPR(pDC, rr);
	}

}

void CScopeDlg::DrawDacPtsPhasedArray(CDC *pDC,CRect rr)
{
	if (theApp.m_Scope.m_cDisplayDACPtsMask == 0) return;

	CPhasedArrayProbe* pPA = &theApp.m_PhasedArray[PORTSIDE];
	int	nStart;
	int	ii,nPnX,nPnY;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	float fGain,fDelay;
	CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));
	CPen penGrey(PS_SOLID, 1, RGB(128, 128, 128));
	CPen *pOldPen = pDC->SelectObject(&penWhite);
	CPen penDashed(PS_DOT, 1, RGB(255, 255, 255));

	//Draw DAC ref amplitude
	pDC->SelectObject(&penDashed);
	switch (pAdc->nRfType) {
	case RFTYPELINEAR:
		nPnY = (rr.top + rr.bottom) / 2 - (int)((theApp.m_LastSettings.fDACRefAmplitude*(float)rr.Height() / 2.0f) / 100.0f);
		break;
	case RFTYPERECTIFIED:
	case RFTYPELOG:
		nPnY = rr.bottom - (int)((theApp.m_LastSettings.fDACRefAmplitude*(float)rr.Height()) / 100.0f);
		break;
	}
	pDC->MoveTo(0, nPnY);
	pDC->LineTo(rr.Width(), nPnY);
	pDC->SelectObject(pOldPen);


	if(FindDACStartPt(&nStart)==FALSE) return;

	//Put the crosses up
	for(ii=0;ii<64;ii++) {
		if(pPA->m_fDacDelay[0][ii]!=0.0) {
			fDelay = pPA->m_fDacDelay[0][ii];
			switch(theApp.m_LastSettings.nDacDelayUnits) {
			case 0:	fDelay = pPA->m_fDacDelay[0][ii] * 1000;		//ns
				break;
			case 1:	fDelay = pPA->m_fDacDelay[0][ii] / (float)theApp.m_LastSettings.nWaterVelocity * 2000000.0f;		//mm
				break;
			case 2:	fDelay = pPA->m_fDacDelay[0][ii] / (float)theApp.m_LastSettings.nMaterialVelocity * 2000000.0f;		//mm
				break;
			}

			nPnX = (int)(fDelay/pAdc->fSamplePeriod) + nStart;
			nPnX = MulDiv(nPnX,rr.Width(),pAdc->nAcqLength);
			fGain = theApp.m_LastSettings.fDACRefAmplitude / (float)pow(10.0,(double)pPA->m_fDacGain[0][ii]/20.0);
			switch(pAdc->nRfType) {
			case RFTYPELINEAR:
				nPnY = (rr.top+rr.bottom)/2 - (int)((fGain*(float)rr.Height()/2.0f)/100.0f);
				break;
			case RFTYPERECTIFIED:
			case RFTYPELOG:
				nPnY = rr.bottom - (int)((fGain*(float)rr.Height())/100.0f);
				break;
			}
			m_ptDac[ii].x = nPnX;
			m_ptDac[ii].y = nPnY;
			pDC->MoveTo(nPnX-3,nPnY-3);
			pDC->LineTo(nPnX+4,nPnY+4);
			pDC->MoveTo(nPnX-3,nPnY+3);
			pDC->LineTo(nPnX+4,nPnY-4);
		}
	}


}

void CScopeDlg::DrawDacPtsPR(CDC *pDC, CRect rr)
{
	unsigned char cMask = 1;
	cMask <<= m_nTimeSlot;
	if (!(theApp.m_Scope.m_cDisplayDACPtsMask & cMask)) return;

	int	nStart;
	Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[m_nTimeSlot].Pr30;
	int	nT = pPr30->nDacTable;
	int	ii, nPnX, nPnY;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	float fGain, fDelay;


	if (FindDACStartPt(&nStart) == FALSE) return;

	CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));
	CPen penGrey(PS_SOLID, 1, RGB(128, 128, 128));
	CPen *pOldPen = pDC->SelectObject(&penWhite);
	//Put the crosses up
	for (ii = 0; ii < 64; ii++) {
		if (pPr30->fDacDelay[nT][ii] != 0.0) {
			fDelay = pPr30->fDacDelay[nT][ii];
			switch (theApp.m_LastSettings.nDacDelayUnits) {
			case 0:	fDelay = pPr30->fDacDelay[nT][ii] * 1000;		//ns
				break;
			case 1:	fDelay = pPr30->fDacDelay[nT][ii] / (float)theApp.m_LastSettings.nWaterVelocity * 2000000.0f;		//mm
				break;
			case 2:	fDelay = pPr30->fDacDelay[nT][ii] / (float)theApp.m_LastSettings.nMaterialVelocity * 2000000.0f;		//mm
				break;
			}

			nPnX = (int)(fDelay / pAdc->fSamplePeriod) + nStart;
			nPnX = MulDiv(nPnX, rr.Width(), pAdc->nAcqLength);
			fGain = theApp.m_LastSettings.fDACRefAmplitude / (float)pow(10.0, (double)pPr30->fDacGain[nT][ii] / 20.0);
			switch (pAdc->nRfType) {
			case RFTYPELINEAR:
				nPnY = (rr.top + rr.bottom) / 2 - (int)((fGain*(float)rr.Height() / 2.0f) / 100.0f);
				break;
			case RFTYPERECTIFIED:
			case RFTYPELOG:
				nPnY = rr.bottom - (int)((fGain*(float)rr.Height()) / 100.0f);
				break;
			}
			m_ptDac[ii].x = nPnX;
			m_ptDac[ii].y = nPnY;
			pDC->MoveTo(nPnX - 3, nPnY - 3);
			pDC->LineTo(nPnX + 4, nPnY + 4);
			pDC->MoveTo(nPnX - 3, nPnY + 3);
			pDC->LineTo(nPnX + 4, nPnY - 4);
		}
	}
	//draw dac threshold and curve
	int nPercentage = MulDiv(pPr30->nDacTriggerThreshold - 128, theApp.m_LastSettings.nDacMaxThreshold, 127);
	int nZeroLevel;
	switch (pAdc->nRfType) {
	case RFTYPELINEAR:
		nPnY = (rr.top + rr.bottom) / 2 - MulDiv(nPercentage, rr.Height() / 2, 100);
		nZeroLevel = (rr.top + rr.bottom) / 2;
		break;
	case RFTYPERECTIFIED:
	case RFTYPELOG:
		nPnY = rr.bottom - MulDiv(nPercentage, rr.Height(), 100);
		nZeroLevel = rr.bottom;
		break;
	}

	float fBlanking_ns = (float)(pPr30->nDacBlanking * 100) - (theApp.m_Scope.m_fMainBangConstant * 1e9f);
	int xx = (int)((fBlanking_ns - (float)pAdc->nDelay) / pAdc->fSamplePeriod);
	nPnX = MulDiv(xx, rr.Width(), pAdc->nAcqLength);
	if (nPnX < 0) nPnX = 0;

	pDC->SelectObject(&penGrey);
	pDC->MoveTo(0, nPnY);
	pDC->LineTo(nPnX, nPnY);
	pDC->LineTo(nPnX, nZeroLevel);


	CPen penDashed(PS_DOT, 1, RGB(255, 255, 255));
	pDC->SelectObject(&penDashed);

	switch (pAdc->nRfType) {
	case RFTYPELINEAR:
		nPnY = (rr.top + rr.bottom) / 2 - (int)((theApp.m_LastSettings.fDACRefAmplitude*(float)rr.Height() / 2.0f) / 100.0f);
		break;
	case RFTYPERECTIFIED:
	case RFTYPELOG:
		nPnY = rr.bottom - (int)((theApp.m_LastSettings.fDACRefAmplitude*(float)rr.Height()) / 100.0f);
		break;
	}
	pDC->MoveTo(0, nPnY);
	pDC->LineTo(rr.Width(), nPnY);

	pDC->SelectObject(pOldPen);

}

void CScopeDlg::DacTableChanged()
{
	FRAME;

	theApp.m_UtUser.Pr30CalculateDacData(&theApp.m_UtUser.m_TS[m_nTimeSlot].Pr30);
	if(pFrame->m_pUltrasonicsSheet) {
		pFrame->m_pUltrasonicsSheet->UpdateDacTable();
	}
	pFrame->SendMessage(UI_UPDATE_PHASED_ARRAY_SHEET, 0, 0);
	theApp.m_PhasedArray[PORTSIDE].setAllDacVariables();

}


bool CScopeDlg::FindNearestDacPt(CPoint point, CRect rr, int *nPt)
{
	unsigned char cMask = 1;
	cMask <<= m_nTimeSlot;
	if(!(theApp.m_Scope.m_cDisplayDACPtsMask & cMask)) return false;

	point.x -= rr.left;
	point.y -= rr.top;

	for(int ii=0;ii<64;ii++) {

		if(abs(m_ptDac[ii].x-point.x)<3 && abs(m_ptDac[ii].y-point.y)<3) {
			*nPt = ii;
			return TRUE;
		}
	};
	return false;
}


void CScopeDlg::OnScopeDeletedacpoint()
{
	Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[m_nTimeSlot].Pr30;
	int	nT = pPr30->nDacTable;
	CRect rr;
	int ii;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	ScreenToClient(&m_ptClient);

	if(FindNearestDacPt(m_ptClient, rr, &m_nWhichDacPt) == TRUE) {

		for (ii = m_nWhichDacPt; ii < 63; ii++) {
			pPr30->fDacDelay[nT][ii] = pPr30->fDacDelay[nT][ii + 1];
			pPr30->fDacGain[nT][ii] = pPr30->fDacGain[nT][ii + 1];
		}
		for (ii = m_nWhichDacPt; ii < 63; ii++) {
			theApp.m_PhasedArray[PORTSIDE].m_fDacDelay[0][ii] = theApp.m_PhasedArray[PORTSIDE].m_fDacDelay[0][ii + 1];
			theApp.m_PhasedArray[PORTSIDE].m_fDacGain[0][ii] = theApp.m_PhasedArray[PORTSIDE].m_fDacGain[0][ii + 1];
		}
		DacTableChanged();
	}
}

void CScopeDlg::UpdateFromMainFrame()
{
	if(GetSafeHwnd() == NULL) return;

	UpdateAllControls();
	CalculateScrollBarFromDelayWidth();

	Invalidate(FALSE);
	RefreshLockVariables();
	m_nTraceDelay=-1;
	m_nTraceWidth=-1;

}

void CScopeDlg::UpdateHardware()
{
	int	gg;
	for(gg=0;gg<8;gg++) {
		theApp.m_UtUser.Dsp200CalculateHardware(m_nTimeSlot,gg);
	}
}



void CScopeDlg::OnScopeSaveasbmp()
{
	#define NUMBERENTRIESPALETTE 0
	CUSLFile File;
	CFileException e;
	CString Buff;
	CRect rr;

	CFileDialog FileDlg(FALSE,_T(".bmp"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("BitMap (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {

		if(File.Open(FileDlg.GetPathName(), CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)==0) {
			File.DecodeFileException(&e,&Buff,FileDlg.GetPathName());
			MessageBox(Buff,_T("Warning"), MB_ICONERROR);
			return;
		} else {
			m_staticView.GetWindowRect(&rr);
			int m_SizePixels = rr.Width()*rr.Height()*(sizeof COLORREF);
			BITMAPFILEHEADER BmpFileHeader;

			char *Ptr = (char*)&BmpFileHeader.bfType;
			strcpy_s(Ptr,2,"BM");
			BmpFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) +
										sizeof(BITMAPINFOHEADER) +
										4*NUMBERENTRIESPALETTE     +
										m_SizePixels;

			BmpFileHeader.bfReserved1 = 0;
			BmpFileHeader.bfReserved2 = 0;
			BmpFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) +
										sizeof(BITMAPINFOHEADER) +
										4*NUMBERENTRIESPALETTE  ;


			BITMAPINFOHEADER BmpHeader;
			BmpHeader.biSize          = sizeof(BITMAPINFOHEADER);
			BmpHeader.biWidth         = rr.Width();
			BmpHeader.biHeight        = rr.Height();
			BmpHeader.biPlanes        = 1;
			BmpHeader.biBitCount      = 32;
			BmpHeader.biCompression   = 0;
			BmpHeader.biSizeImage     = m_SizePixels;
			BmpHeader.biXPelsPerMeter = 0;
			BmpHeader.biYPelsPerMeter = 0;
			BmpHeader.biClrUsed       = 0;
			BmpHeader.biClrImportant  = 0;

			File.Write(&BmpFileHeader, sizeof(BITMAPFILEHEADER));
			File.Write(&BmpHeader, sizeof(BITMAPINFOHEADER));

			DrawGTrace(1,&File);
			File.Close();



			return;
		}
	}
	return;
}


void CScopeDlg::OnScopeStartrecording()
{
	CFileException	e;
	CString Buff;
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	int nTraceOffset,nAdcOffset;
	char Identifier[60]={"Ultrasonic Sciences Ltd GenericScanner Trace"};

	if(m_bRecording	== FALSE) {
		if(m_FileRecord.Open(theApp.m_StorePath[0] + _T("\\Recording.tra"), CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)==0) {
			m_FileRecord.DecodeFileException(&e,&Buff,theApp.m_StorePath[0] + L"\\Recording.tra");
			MessageBox(Buff,_T("Warning"), MB_ICONERROR);
			return;
		} else {
			char* pArray = new char[10000];
			ZeroMemory(pArray,1000);
			m_FileRecord.Write(pArray,10000);
			delete pArray;

			m_FileRecord.Seek(0,CFile::begin);
			m_FileRecord.Write(Identifier,strlen(Identifier));
			m_FileRecord.Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);


			m_FileRecord.Seek(nAdcOffset=0x400,CFile::begin);
			m_FileRecord.WriteStruct(pAdc,pAdc->nStructSize = sizeof (ADC200Data));

			m_FileRecord.Seek(nTraceOffset=(int)m_FileRecord.GetPosition()+1024,CFile::begin);

			m_FileRecord.Seek(0x200,CFile::begin);
			m_FileRecord.Write(&nAdcOffset, sizeof nAdcOffset);
			m_FileRecord.Write(&nTraceOffset, sizeof nTraceOffset);

			m_FileRecord.Seek(nTraceOffset,CFile::begin);

			m_nPtrRecording=0;

			m_bRecording = TRUE;
		}
	}

}

void CScopeDlg::OnScopeStoprecording()
{
	if(m_bRecording	== TRUE) {
		m_bRecording = FALSE;
		m_FileRecord.Close();

		CFileDialog FileDlg(FALSE,_T(".tra"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Trace (*.tra)|*.tra|All Files (*.*)|*.*||"),NULL);
		FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
		if(FileDlg.DoModal()==IDOK) {
			CFile::Rename(theApp.m_StorePath[0] + _T("\\Recording.tra"),FileDlg.GetPathName());
		}
	}

}

void CScopeDlg::OnScopePlayrecording()
{
	double dVersion;
	int	nAdcOffset;
	CFileException e;
	CString Buff;
	char Identifier[60]={"Ultrasonic Sciences Ltd GenericScanner Trace"};
	char FileIdentifier[60];
	ZeroMemory(FileIdentifier,sizeof FileIdentifier);

	if(m_bRecording == FALSE) {
		switch(theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]) {
		case 1:
		case 2:
			theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]=0;
			m_FileRecord.Close();
			break;
		}

		CFileDialog FileDlg(TRUE,_T(".tra"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Trace (*.tra)|*.tra|All Files (*.*)|*.*||"),NULL);
		FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
		if(FileDlg.DoModal()==IDOK) {
			if(m_FileRecord.Open(FileDlg.GetPathName(), CFile::typeBinary|CFile::modeRead, &e)==0) {
				m_FileRecord.DecodeFileException(&e,&Buff,FileDlg.GetPathName());
				MessageBox(Buff,_T("Warning"), MB_ICONERROR);
				return;
			} else {
				theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.bPlayBack=TRUE;
				m_FileRecord.Seek(0,CFile::begin);
				m_FileRecord.Read(FileIdentifier,strlen(Identifier));
				m_FileRecord.Read(&dVersion,sizeof dVersion);
				if(strcmp(FileIdentifier,Identifier)==0) {

					m_FileRecord.Seek(0x200,CFile::begin);
					m_FileRecord.Read(&nAdcOffset, sizeof nAdcOffset);
					m_FileRecord.Read(&m_nOffsetRecording, sizeof m_nOffsetRecording);

					m_FileRecord.Seek(nAdcOffset=0x400,CFile::begin);
					m_FileRecord.ReadStruct(&m_AdcRecording, sizeof m_AdcRecording);

					theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nAcqLength = m_AdcRecording.nAcqLength;
					theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nRfType = m_AdcRecording.nRfType;
					theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay = m_AdcRecording.nDelay;
					theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth = m_AdcRecording.nWidth;
					theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.fSamplePeriod = m_AdcRecording.fSamplePeriod;

					m_nRecordingL = ((int)m_FileRecord.GetLength() - m_nOffsetRecording) / m_AdcRecording.nAcqLength;

					m_FileRecord.Seek(m_nOffsetRecording,CFile::begin);
					m_nPtrRecording=0;
					theApp.m_Scope.m_nPlayRecording[m_nTimeSlot] = 1;
				} else {
					m_FileRecord.Close();
				}
			}
		}
	}

}

bool CScopeDlg::IsPlayBack(CPoint pt,CRect rr)
{
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	CRect rectCircle = m_rectPlayBar;
	int	nRadius = (m_rectPlayBar.bottom-m_rectPlayBar.top)/2;
	pt.x-=rr.left;


	switch(theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]) {
	case 1:
	case 2:
		rectCircle.left=m_rectPlayBar.right;
		rectCircle.right=m_rectPlayBar.right+2*nRadius;
		if(rectCircle.PtInRect(pt)) {
			theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.bPlayBack=FALSE;
			theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]=0;
			m_FileRecord.Close();
			return TRUE;
		}
		rectCircle.left=m_rectPlayBar.left-2*nRadius;
		rectCircle.right=m_rectPlayBar.left;
		if(rectCircle.PtInRect(pt)) {
			theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]==1 ? theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]=2 : theApp.m_Scope.m_nPlayRecording[m_nTimeSlot]=1;
			return TRUE;
		}
		if(m_rectPlayBar.PtInRect(pt)) {
			m_nPtrRecording=MulDiv(pt.x-m_rectPlayBar.left,m_nRecordingL-1,m_rectPlayBar.Width());
			if(m_nPtrRecording<0) m_nPtrRecording=0;
			if(m_nPtrRecording>=m_nRecordingL) m_nPtrRecording=m_nRecordingL-8;
			m_FileRecord.Seek(m_nPtrRecording*pAdc->nAcqLength+m_nOffsetRecording,CFile::begin);
			return TRUE;
		}

		return FALSE;
		break;
	}
	return FALSE;

}


void CScopeDlg::OnScopeScaleBottom()
{
	theApp.m_Scope.m_bBottomScale==FALSE ? theApp.m_Scope.m_bBottomScale=TRUE : theApp.m_Scope.m_bBottomScale=FALSE;
}

void CScopeDlg::OnScopeScaleLeft()
{
	theApp.m_Scope.m_bLeftScale==FALSE ? theApp.m_Scope.m_bLeftScale=TRUE : theApp.m_Scope.m_bLeftScale=FALSE;
}

void CScopeDlg::OnScopeScaleRight()
{
	theApp.m_Scope.m_bRightScale==FALSE ? theApp.m_Scope.m_bRightScale=TRUE : theApp.m_Scope.m_bRightScale=FALSE;
}

void CScopeDlg::OnScopeScaleTop()
{
	theApp.m_Scope.m_bTopScale==FALSE ? theApp.m_Scope.m_bTopScale=TRUE : theApp.m_Scope.m_bTopScale=FALSE;
}

void CScopeDlg::OnScopeDrawmodeVector()
{
	theApp.m_Scope.m_bTraceMode=FALSE;
}

void CScopeDlg::OnScopeDrawmodeDots()
{
	theApp.m_Scope.m_bTraceMode=TRUE;
}


void CScopeDlg::OnScopeCurrenttimeslot()
{
	m_bTrackCurrentTimeSlot = TRUE;
	m_nTimeSlot = theApp.m_UtUser.m_Global.nTimeSlot;
	SetWindowName();

}

void CScopeDlg::OnScopeSlot1()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 0;
	SetWindowName();
}

void CScopeDlg::OnScopeSlot2()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 1;
	SetWindowName();
}

void CScopeDlg::OnScopeSlot3()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 2;
	SetWindowName();
}

void CScopeDlg::OnScopeSlot4()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 3;
	SetWindowName();
}

void CScopeDlg::OnScopeSlot5()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 4;
	SetWindowName();

}

void CScopeDlg::OnScopeSlot6()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 5;
	SetWindowName();

}

void CScopeDlg::OnScopeSlot7()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 6;
	SetWindowName();

}

void CScopeDlg::OnScopeSlot8()
{
	m_bTrackCurrentTimeSlot = FALSE;
	m_nTimeSlot = 7;
	SetWindowName();

}

int CScopeDlg::MinMax(int *pnV, int nMin, int nMax)
{
	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

void CScopeDlg::OnScopeAutointerfacegate() 
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[m_nTimeSlot].Adc;
	GatesData* pGates = &theApp.m_UtUser.m_TS[m_nTimeSlot].Gate;
	int xx;
	int nStart = -1;

	for(xx=0 ;xx<(int)pAdc->nAcqLength && (nStart == -1);xx++) {
		if(theApp.m_Scope.m_RFTrace[m_nTimeSlot][xx] > 20) {
			nStart = xx;
		}
	}

	if(nStart == -1) {
		pGates->nNsDelay[0] = pAdc->nDelay+(pAdc->nWidth/2) - (pAdc->nWidth/4);
	} else {
		pGates->nNsDelay[0] = pAdc->nDelay+(int)((float)nStart*pAdc->fSamplePeriod) - (pAdc->nWidth/4);
	}
	pGates->nNsWidth[0] = pAdc->nWidth/2;

	if(pGates->nNsDelay[0] < pAdc->nDelay) {
		pGates->nNsDelay[0] = pAdc->nDelay;// + (int)(theApp.m_Scope.m_fMainBangConstant * 1e9f);
	}
	if((pGates->nNsDelay[0] + pGates->nNsWidth[0]) >= (pAdc->nDelay + pAdc->nWidth)) {
		pGates->nNsWidth[0] = (pAdc->nDelay + pAdc->nWidth) - pGates->nNsDelay[0];
	}


}



void CScopeDlg::CopyScopeDelayWidth()
{
	for(int nTS=0;nTS<256;nTS++) {
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_SCOPE_DELAY || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			theApp.m_UtUser.m_TS[nTS].Adc.nDelay = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nDelay;
			theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nAcqLength;
			theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.fSamplePeriod;
			theApp.m_UtUser.m_TS[nTS].Adc.nConversionRate = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nConversionRate;
			theApp.m_UtUser.m_TS[nTS].Adc.nHardwareDelay = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nHardwareDelay;
			
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_SCOPE_WIDTH || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			theApp.m_UtUser.m_TS[nTS].Adc.nWidth = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nWidth;
			theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nAcqLength;
			theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.fSamplePeriod;
			theApp.m_UtUser.m_TS[nTS].Adc.nConversionRate = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nConversionRate;
			theApp.m_UtUser.m_TS[nTS].Adc.nHardwareDelay = theApp.m_UtUser.m_TS[m_nTimeSlot].Adc.nHardwareDelay;
		}
	}
}

void CScopeDlg::CopyGates()
{
	for(int nTS=0;nTS<256;nTS++) {
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_MODE || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nGateMode, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nGateMode);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_DELAY || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nNsDelay, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsDelay);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nSampleDelay, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleDelay, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleDelay);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.Delay, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Delay, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Delay);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_WIDTH || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nNsWidth, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsWidth, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nNsWidth);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nSampleWidth, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleWidth, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nSampleWidth);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.Width, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Width, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.Width);
		}
		if(theApp.m_LastSettings.n64CopyUtVariable & _UT_GATE_THRESHOLD || theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Gate.nThreshold, theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nThreshold, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Gate.nThreshold);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.InterfaceThreshold, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.InterfaceThreshold, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.InterfaceThreshold);
			CopyMemory(theApp.m_UtUser.m_TS[nTS].Dsp.SignDetect, theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.SignDetect, sizeof theApp.m_UtUser.m_TS[m_nTimeSlot].Dsp.SignDetect);
		}
	}

}

void CScopeDlg::CopyDAC()
{

}

void CScopeDlg::OnScopeMainbangdelayZero() 
{
	theApp.m_Scope.m_fMainBangConstant = 0.0f;

	switch(theApp.m_LastSettings.nAdcCardType) {
	case Alazar_860: theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
		theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc, 0);
		break;
	case Alazar_9626:
	case Alazar_9462: theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
		theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc, 0);
		break;
	case Acqiris_U1071AHZ4:
	case Acqiris_DP1400:
	case Acqiris_DP210: theApp.m_Acqiris.Configure();
		break;
	}
	
}

void CScopeDlg::OnScopeMainbangdelayMousepos() 
{
	// TODO: Add your command handler code here
	
}

void CScopeDlg::OnScopeMainbangdelayInterface() 
{
	int nDelay=theApp.m_UtUser.GetGateWaterTimeNs(m_nTimeSlot);

	theApp.m_Scope.m_fMainBangConstant += (float)nDelay / 1e9f;

	switch(theApp.m_LastSettings.nAdcCardType) {
	case Alazar_860:theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc, 0);
		break;
	case Alazar_9626:
	case Alazar_9462: theApp.m_Alazar.Configure(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc);
		theApp.m_Alazar.ConfigureWidthDelay(&theApp.m_UtUser.m_TS[m_nTimeSlot].Adc, 0);
		break;
	case Acqiris_U1071AHZ4:
	case Acqiris_DP1400:
	case Acqiris_DP210: theApp.m_Acqiris.Configure();
		break;
	}
	
}

void CScopeDlg::OnRulerOff()
{
	int nMask = 0x03;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask &= ~nMask;

	nMask = (int)RulerMode::RULER_OFF;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask |= nMask;
}

void CScopeDlg::OnRulerOffMaterial()
{
	int nMask = 0x03;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask &= ~nMask;

	nMask = (int)RulerMode::OFF_MATERIAL;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask |= nMask;
}

void CScopeDlg::OnRulerWaterMaterial()
{
	int nMask = 0x03;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask &= ~nMask;

	nMask = (int)RulerMode::WATER_MATERIAL;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask |= nMask;
}

void CScopeDlg::OnRulerMaterialMaterial()
{
	int nMask = 0x03;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask &= ~nMask;

	nMask = (int)RulerMode::MATERIAL_MATERIAL;
	nMask <<= (m_nTimeSlot * 2);
	theApp.m_Scope.m_nRulerDisplayMask |= nMask;
}


void CScopeDlg::OnScopeProperties()
{
	FRAME;
	pFrame->SendMessage(UI_SHOW_PROPERTIES_PANE, NULL, Scope);
}

void CScopeDlg::SetWindowName()
{
	CString Buff;

	if (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY) {
		Buff.Format(L"%s FocalLaw #:%d", theApp.m_UtUser.m_TS[m_nTimeSlot].cName, theApp.m_LSA.m_nScopeViewLaw + 1);
	}
	else {
		Buff.Format(L"%s", theApp.m_UtUser.m_TS[m_nTimeSlot].cName);
	}

	if (Buff != m_WindowName) {
		m_WindowName = Buff;
		SetWindowText(m_WindowName);
	}
}

