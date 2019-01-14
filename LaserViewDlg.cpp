// LaserViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LaserViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaserViewDlg dialog


CLaserViewDlg::CLaserViewDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CLaserViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaserViewDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_bDlgCreated = FALSE;
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_bDlgCreated = false;
}


void CLaserViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserViewDlg)
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	DDX_Control(pDX, IDC_EDIT_MIRROR_DISTANCE, m_editMirrorDistance);
	DDX_Control(pDX, IDC_EDIT_ANGLE_RANGE, m_editAngleRange);
	DDX_Control(pDX, IDC_EDIT_OFFSET, m_editOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLaserViewDlg, CDialog)
	//{{AFX_MSG_MAP(CLaserViewDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW, OnSelchangeComboView)
	ON_EN_CHANGE(IDC_EDIT_OFFSET, OnChangeEditOffset)
	ON_BN_CLICKED(IDC_BUTTON_TAKE_TAUGHT_PONTS, OnButtonTakeTaughtPonts)
	ON_EN_CHANGE(IDC_EDIT_MIN_DISTANCE, OnChangeEditMinDistance)
	ON_EN_CHANGE(IDC_EDIT_MAX_DISTANCE, OnChangeEditMaxDistance)
	ON_EN_CHANGE(IDC_EDIT_ANGLE_RANGE, OnChangeEditAngleRange)
	ON_EN_CHANGE(IDC_EDIT_SAMPLESPERSEC, OnChangeEditSamplespersec)
	ON_EN_CHANGE(IDC_EDIT_MOTORSPEED, OnChangeEditMotorspeed)
	ON_EN_CHANGE(IDC_EDIT_AVERAGESAMPLES, OnChangeEditAveragesamples)
	ON_EN_CHANGE(IDC_EDIT_AVERAGELINES, OnChangeEditAveragelines)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_JITTER, OnButtonJitter)
	ON_EN_CHANGE(IDC_EDIT_CALLBACKPERCENT, OnChangeEditCallbackpercent)
	ON_BN_CLICKED(IDC_CHECK_DISCARD, OnCheckDiscard)
	ON_EN_CHANGE(IDC_EDIT_MIN_AMP, OnChangeEditMinAmp)
	ON_BN_CLICKED(IDC_CHECK_FIR, OnCheckFir)
	ON_BN_CLICKED(IDC_CHECK_REVERSE_LASER_ENCODER, OnCheckReverseLaserEncoder)
	ON_BN_CLICKED(IDC_CHECK_FLIP_DISPLAY, OnCheckLaserFlipDisplay)
	ON_BN_CLICKED(IDC_CHECK_OPEN_LASER, OnCheckOpenLaser)
	ON_EN_CHANGE(IDC_EDIT_MAX_Y_DEVIATION, OnChangeEditMaxYDeviation)
	ON_BN_CLICKED(IDC_CHECK_SINGLE_READING, OnCheckSingleReading)
	ON_EN_CHANGE(IDC_EDIT_BLOCK_SIZE, OnChangeEditBlockSize)
	ON_EN_CHANGE(IDC_EDIT_MIRROR_DISTANCE, OnChangeEditMirrorDistance)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKER_DISPLAY, OnSelchangeComboMarkerDisplay)
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LINE_NUMBER, OnDeltaposSpinLineNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserViewDlg message handlers

BOOL CLaserViewDlg::OnInitDialog() 
{

	APP App = (APP) AfxGetApp();
	CDialog::OnInitDialog();
	CString Buff;
	
	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	m_spinLineNumber.SetRange(0,100);

	m_bDlgCreated = true;
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLaserViewDlg::UpdateAllControls()
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	if(m_bDlgCreated != true) return;

	Buff.Format(L"%d",App->m_nLine + 1);
	m_editLineNumber.SetWindowText(Buff);

	m_spinLineNumber.SetPos(App->m_nLine);


	Buff.Format(_T("%0.1f"),App->m_LastSettings.fLaserMirrorDistance);
	m_editMirrorDistance.SetWindowText(Buff);

	Buff.Format(_T("%0.1f"),App->m_LastSettings.fAngleRange);
	m_editAngleRange.SetWindowText(Buff);

	Buff.Format(_T("%0.1f"),App->m_LastSettings.fRotateOffset);
	m_editOffset.SetWindowText(Buff);



}

void CLaserViewDlg::OnSelchangeComboView() 
{
	CLazerTopographyPage* pParent = (CLazerTopographyPage*)m_pParent;
	pParent->m_nView = m_comboView.GetCurSel();
	
}

void CLaserViewDlg::OnChangeEditOffset() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	float fAngle;

	m_editOffset.GetWindowText(Buff);
	_WTOF(Buff,fAngle);

	if(	App->m_LastSettings.fRotateOffset != fAngle) {
		App->m_LastSettings.fRotateOffset = fAngle;
//		result = App->m_LaserUser.setAngleOffset(1,fAngle); 
//		App->m_LaserUser.ResetDataSets();
	}

	
}

void CLaserViewDlg::OnButtonTakeTaughtPonts() 
{
	APP App = (APP) AfxGetApp();

//	App->m_LaserUser.TakeTaughtPoints();
	
}


void CLaserViewDlg::OnChangeEditMinDistance() 
{
	APP App = (APP) AfxGetApp();
	float fMinRange;
	CString Buff;

	m_editMinRange.GetWindowText(Buff);
	_WTOF(Buff,fMinRange);

	if(	App->m_LastSettings.fMinDistance != fMinRange){
		App->m_LastSettings.fMinDistance = fMinRange;
//		App->m_LaserUser.setMinValidRange(fMinRange);
	}

}

void CLaserViewDlg::OnChangeEditMaxDistance() 
{
	APP App = (APP) AfxGetApp();
	float fMaxRange;
	CString Buff;

	m_editMaxRange.GetWindowText(Buff);
	_WTOF(Buff,fMaxRange);

	if(	App->m_LastSettings.fMaxDistance != fMaxRange){
		App->m_LastSettings.fMaxDistance = fMaxRange;
//		App->m_LaserUser.setMaxValidRange(fMaxRange);
	}

}

void CLaserViewDlg::OnChangeEditAngleRange() 
{
	APP App = (APP) AfxGetApp();
	float fAngle;
	CString Buff;

	m_editAngleRange.GetWindowText(Buff);
	_WTOF(Buff,fAngle);

	if(	App->m_LastSettings.fAngleRange != fAngle) {
		App->m_LastSettings.fAngleRange = fAngle;	
//		App->m_LaserUser.setMinValidAngle(1,360-(fAngle/2)); 
//		App->m_LaserUser.setMaxValidAngle(1,(fAngle/2)); 
	}

}

void CLaserViewDlg::OnChangeEditSamplespersec() 
{
	APP App = (APP) AfxGetApp();
	int nSamplesPerSecond;
	CString Buff;

	m_editSamplesPerSecond.GetWindowText(Buff);
	nSamplesPerSecond = _ttoi(Buff);

	if(	App->m_LastSettings.nLaserSamplesPerSecond != nSamplesPerSecond) {
		App->m_LastSettings.nLaserSamplesPerSecond = nSamplesPerSecond;
//		App->m_LaserUser.setSamplesPerSec(nSamplesPerSecond);
	}
	
}

void CLaserViewDlg::OnChangeEditMotorspeed() 
{

	APP App = (APP) AfxGetApp();
	int nMotorSpeed;
	CString Buff;

	m_editMotorSpeed.GetWindowText(Buff);
	nMotorSpeed = _ttoi(Buff);

	if(	App->m_LastSettings.nLaserMotorRPM != nMotorSpeed) {
		App->m_LastSettings.nLaserMotorRPM = nMotorSpeed;
//		App->m_LaserUser.setMotorRPM(1,nMotorSpeed,1);
	}
	
}

void CLaserViewDlg::OnChangeEditAveragesamples() 
{
	APP App = (APP) AfxGetApp();
	int nAvgSmpls;
	CString Buff;

	m_editAverageSamples.GetWindowText(Buff);
	nAvgSmpls = _ttoi(Buff);
	App->m_LastSettings.nLaserAverageSamples = nAvgSmpls;


	
}

void CLaserViewDlg::OnChangeEditAveragelines() 
{
	APP App = (APP) AfxGetApp();
	int nAvgLines;
	CString Buff;

	m_editAverageLines.GetWindowText(Buff);
	nAvgLines = _ttoi(Buff);

	App->m_LastSettings.nLaserAverageLines = nAvgLines;
	
}

void CLaserViewDlg::OnTimer(UINT nIDEvent) 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	
	CDialog::OnTimer(nIDEvent);
}

void CLaserViewDlg::OnButtonJitter() 
{
	APP App = (APP) AfxGetApp();
	CLazerTopographyPage* pParent = (CLazerTopographyPage*)m_pParent;
	
	
	pParent->fTempJitterMin = 10000;
	pParent->fTempJitterMax = -1;
	
}

void CLaserViewDlg::OnChangeEditCallbackpercent() 
{
	APP App = (APP) AfxGetApp();
	int nPercent;
	CString Buff;

	m_editCallbackPercent.GetWindowText(Buff);
	nPercent = _ttoi(Buff);

	if(nPercent<1) nPercent=1;
	if(nPercent>100) nPercent=100;


	if(	App->m_LastSettings.nLaserCallbackThresholdPercent != nPercent) {
		App->m_LastSettings.nLaserCallbackThresholdPercent = nPercent;
//		App->m_LaserUser.setCallbackThreshold(nPercent);
	}


}

void CLaserViewDlg::OnCheckDiscard() 
{
	APP App = (APP) AfxGetApp();

	bool bDiscard = m_checkDiscard.GetCheck() & 1;

	if(	App->m_LastSettings.bLaserDiscard != bDiscard){
		App->m_LastSettings.bLaserDiscard = bDiscard;
		if(bDiscard) {
//			App->m_LaserUser.setDiscardInvalidOn();
		}else{
//			App->m_LaserUser.setDiscardInvalidOff();
		}
	}
	
}

void CLaserViewDlg::OnChangeEditMinAmp() 
{
	APP App = (APP) AfxGetApp();
	long nAmp;
	CString Buff;

	m_editMinValidAmplitude.GetWindowText(Buff);
	nAmp = (long)_ttoi(Buff);

	if(nAmp<0) nAmp=0;
	if(nAmp>255) nAmp=255;


	if(	App->m_LastSettings.nLaserMinValidAmplitude != nAmp) {
		App->m_LastSettings.nLaserMinValidAmplitude = nAmp;
//		App->m_LaserUser.setMinValidAmplitude(nAmp);
	}
	
}

void CLaserViewDlg::OnCheckFir() 
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.bLaserFIR = m_checkFIR.GetCheck() & 1;
	
}


void CLaserViewDlg::OnCheckReverseLaserEncoder()
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.bLaserReverseEncoder = m_checkReverseEncoder.GetCheck() & 1;

}


void CLaserViewDlg::OnCheckLaserFlipDisplay()
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.bLaserFlipDisplay = m_checkFlipDisplay.GetCheck() & 1;

}

void CLaserViewDlg::OnCheckOpenLaser() 
{
	APP App = (APP) AfxGetApp();

	//0x0000 -> 0x00ff = Pmac
	//0x0100 -> 0xff00 = si10

	//Probably should be a general IO class to do all this!
	int m_nPmacIOGeneralOutput = App->m_PmacUser.ReadDpram(PMAC_GENERAL_OUTPUTS);
	int m_nSi10IOGeneralOutput = App->m_Si10User.GetGeneralOuput();

	if(App->m_Tank.nLaserCoverBit<=0xFF) { //Pmac

	}else{
		if(!m_checkOpenLaserDoor.GetCheck()) {
			m_nSi10IOGeneralOutput |= (App->m_Tank.nLaserCoverBit>>8);
			App->m_Si10User.WriteGeneralOuput(m_nSi10IOGeneralOutput);
		}else{
			m_nSi10IOGeneralOutput &= ((App->m_Tank.nLaserCoverBit>>8)^0xff);
			App->m_Si10User.WriteGeneralOuput(m_nSi10IOGeneralOutput);
		}
	}
	
}


void CLaserViewDlg::OnOK() 
{
}

void CLaserViewDlg::OnChangeEditMaxYDeviation() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editMaxYDeviation.GetWindowText(Buff);
	_WTOF(Buff,App->m_LastSettings.fLaserMaxYDeviation);

}

void CLaserViewDlg::OnCheckSingleReading() 
{

	APP App = (APP) AfxGetApp();

	MessageBox(_T("Not Implemented"),_T("Warning"), MB_ICONERROR);

	 //If you use the the function setSamplesPerSecond, you cannot then use getSingleSample without a restart.

	if(MessageBox(_T("If you change this you will be required to:\nClose this software\nhard restart the Laser PC\nWait 10 mins\nRestart this software\nStill Proceed?"),_T("Warning"),MB_YESNO) != IDYES) return;
	
	App->m_LastSettings.bLaserSingleShot = m_checkSingleReading.GetCheck() & 1;
//	App->m_LaserUser.setSingleShotData(App->m_LastSettings.bLaserSingleShot);

	
}

void CLaserViewDlg::OnChangeEditBlockSize() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editBlockSize.GetWindowText(Buff);
	_WTOF(Buff,App->m_LastSettings.fLaserBlockSize);

	
}

void CLaserViewDlg::OnChangeEditMirrorDistance() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editMirrorDistance.GetWindowText(Buff);
	_WTOF(Buff,App->m_LastSettings.fLaserMirrorDistance);
//	App->m_LaserUser.ResetDataSets();
	
}

void CLaserViewDlg::OnSelchangeComboMarkerDisplay() 
{
	APP App = (APP) AfxGetApp();
	App->m_LastSettings.nLaserMarkerDisplay = m_comboMarkerDisplay.GetCurSel();
}

void CLaserViewDlg::OnChangeEditLineNumber() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editLineNumber.GetWindowText(Buff);
	App->m_nLine = _ttoi(Buff) -1;
}

void CLaserViewDlg::OnDeltaposSpinLineNumber(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	APP App = (APP)AfxGetApp();
	CString Buff;

	App->m_nLine += pNMUpDown->iDelta;
	if(App->m_nLine>=PROFILE->m_nTaughtLineL) App->m_nLine=PROFILE->m_nTaughtLineL-1;
	if(App->m_nLine<0) App->m_nLine=0;
	
	Buff.Format(L"%d",App->m_nLine + 1);
	m_editLineNumber.SetWindowText(Buff);

	*pResult = 0;
}
