// AxisServoStatus.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AxisServoStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisServoStatus dialog


CAxisServoStatus::CAxisServoStatus(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop,int nAxis)
	: CDialog(CAxisServoStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAxisServoStatus)
	m_stringDesirePos = _T("");
	m_stringPosition = _T("");
	m_stringStatus = _T("");
	m_stringJoyDev = _T("");
	m_bCheckHomed = FALSE;
	m_bCheckInPos = FALSE;
	m_bCheckMaxSwitch = FALSE;
	m_bCheckMinSwitch = FALSE;
	m_bCheckSoftError = FALSE;
	m_bCheckHardError = FALSE;
	m_stringTitle = _T("");
	m_bCheckEncoderFailure = FALSE;
	m_bCheckAmplifierEnabled = FALSE;
	m_bCheckAmplifierFault = FALSE;
	//}}AFX_DATA_INIT
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_nAxis = nAxis;
}


void CAxisServoStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxisServoStatus)
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS, m_stringDesirePos);
	DDX_Text(pDX, IDC_EDIT_POSITION, m_stringPosition);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_stringStatus);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV, m_stringJoyDev);
	DDX_Check(pDX, IDC_CHECK_HOMED, m_bCheckHomed);
	DDX_Check(pDX, IDC_CHECK_IN_POS, m_bCheckInPos);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH, m_bCheckMaxSwitch);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH, m_bCheckMinSwitch);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR, m_bCheckSoftError);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR, m_bCheckHardError);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_stringTitle);
	DDX_Check(pDX, IDC_CHECK_ENCODER_FAILURE, m_bCheckEncoderFailure);
	DDX_Check(pDX, IDC_CHECK_ENCODER_AMPLIFIER_ENABLED, m_bCheckAmplifierEnabled);
	DDX_Check(pDX, IDC_CHECK_AMPLIFIER_FAULT, m_bCheckAmplifierFault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxisServoStatus, CDialog)
	//{{AFX_MSG_MAP(CAxisServoStatus)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisServoStatus message handlers

void CAxisServoStatus::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CAxisServoStatus::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CAxisServoStatus::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CAxisServoStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	m_stringTitle = theApp.m_Axes[m_nAxis].cName;
	UpdateData(FALSE);

	SetTimer(1,200,NULL);
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAxisServoStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAxisServoStatus::OnTimer(UINT nIDEvent) 
{
	CString Units[3]={L"mm", L"\x022", L"\x0b0"};
	bool bFlag=FALSE;

	if(m_nOldPos-theApp.m_Axes[m_nAxis].nPos) {
		m_stringPosition.Format(_T("%d"),theApp.m_Axes[m_nAxis].nPos+theApp.m_Axes[m_nAxis].nOffset);
		bFlag=TRUE;
	}

	if(m_nOldJoyDev - theApp.m_PmacUser.m_nJoyDev[theApp.m_Axes[m_nAxis].nPhysicalAxis-1]) {
		m_stringJoyDev.Format(_T("%d"),m_nOldJoyDev=theApp.m_PmacUser.m_nJoyDev[theApp.m_Axes[m_nAxis].nPhysicalAxis-1]);
		bFlag=TRUE;
	}

	float fMult = 1.0f;
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: if(theApp.m_Tank.fServoUpdateRate==0.0f) theApp.m_Tank.fServoUpdateRate = 2258.815f;
		fMult = 3072.0f / theApp.m_Tank.fServoUpdateRate;
		break;
	case FIELD_BUS:
		break;
	}


	if(m_nOldVelocity - theApp.m_PmacUser.m_nVelocity[theApp.m_Axes[m_nAxis].nPhysicalAxis-1]) {
		m_stringDesirePos.Format(_T("%.1f %s/s"),((float)theApp.m_PmacUser.m_nVelocity[theApp.m_Axes[m_nAxis].nPhysicalAxis-1]*theApp.m_Axes[m_nAxis].fStepSize)/fMult,Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
		bFlag=TRUE;
	}

	if(m_nOldFollowingError - theApp.m_Axes[m_nAxis].nFollowingError) {
		m_stringStatus.Format(_T("%.03f%s"),(theApp.m_Axes[m_nAxis].nFollowingError / 3072) * theApp.m_Axes[m_nAxis].fStepSize,Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	}


	if(m_nOldStatus-theApp.m_Axes[m_nAxis].nStatus) {
		m_nOldStatus = theApp.m_Axes[m_nAxis].nStatus;


		m_nOldStatus & PMAC_IN_POS				? m_bCheckInPos = TRUE				: m_bCheckInPos = FALSE;
		m_nOldStatus & PMAC_SOFT_LIMIT			? m_bCheckSoftError = TRUE			: m_bCheckSoftError = FALSE;
		m_nOldStatus & PMAC_HARD_LIMIT			? m_bCheckHardError = TRUE			: m_bCheckHardError = FALSE;
		m_nOldStatus & PMAC_MIN_SWITCH			? m_bCheckMinSwitch = TRUE			: m_bCheckMinSwitch = FALSE;
		m_nOldStatus & PMAC_MAX_SWITCH			? m_bCheckMaxSwitch = TRUE			: m_bCheckMaxSwitch = FALSE;
		m_nOldStatus & PMAC_HOMED				? m_bCheckHomed = TRUE				: m_bCheckHomed = FALSE;
		m_nOldStatus & PMAC_ENCODER_FAILURE		? m_bCheckEncoderFailure = TRUE		: m_bCheckEncoderFailure = FALSE;
		m_nOldStatus & PMAC_AMPLIFIER_ENABLED	? m_bCheckAmplifierEnabled = TRUE	: m_bCheckAmplifierEnabled = FALSE;
		m_nOldStatus & PMAC_AMPLIFIER_FAULT		? m_bCheckAmplifierFault = TRUE		: m_bCheckAmplifierFault = FALSE;
		bFlag=TRUE;
	}

	if(bFlag==TRUE) UpdateData(FALSE);
	
	m_nOldPos = theApp.m_Axes[m_nAxis].nPos+theApp.m_Axes[m_nAxis].nOffset;
	m_nOldJoyDev = theApp.m_PmacUser.m_nJoyDev[theApp.m_Axes[m_nAxis].nPhysicalAxis-1];
	m_nOldVelocity = theApp.m_PmacUser.m_nVelocity[theApp.m_Axes[m_nAxis].nPhysicalAxis-1];
	m_nOldFollowingError = theApp.m_Axes[m_nAxis].nFollowingError;

	CDialog::OnTimer(nIDEvent);
}
