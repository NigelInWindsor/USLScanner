// AxesPmacStatus1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AxesPmacStatus1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesPmacStatus property page

IMPLEMENT_DYNCREATE(CAxesPmacStatus, CPropertyPage)

CAxesPmacStatus::CAxesPmacStatus() : CPropertyPage(CAxesPmacStatus::IDD)
{
	//{{AFX_DATA_INIT(CAxesPmacStatus)
	m_bCheckHardError1 = FALSE;
	m_bCheckHomed1 = FALSE;
	m_bCheckInPos1 = FALSE;
	m_bCheckMaxSwitch1 = FALSE;
	m_bCheckMinSwitch1 = FALSE;
	m_bCheckSoftError1 = FALSE;
	m_stringDesirePos1 = _T("");
	m_stringPosition1 = _T("");
	m_stringStatus1 = _T("");
	m_bCheckHardError2 = FALSE;
	m_bCheckHomed2 = FALSE;
	m_bCheckInPos2 = FALSE;
	m_bCheckMaxSwitch2 = FALSE;
	m_bCheckMinSwitch2 = FALSE;
	m_bCheckSoftError2 = FALSE;
	m_stringDesirePos2 = _T("");
	m_stringPosition2 = _T("");
	m_stringStatus2 = _T("");
	m_bCheckHardError3 = FALSE;
	m_bCheckHomed3 = FALSE;
	m_bCheckInPos3 = FALSE;
	m_bCheckMaxSwitch3 = FALSE;
	m_bCheckMinSwitch3 = FALSE;
	m_bCheckSoftError3 = FALSE;
	m_stringDesirePos3 = _T("");
	m_stringPosition3 = _T("");
	m_stringStatus3 = _T("");
	m_bCheckHardError4 = FALSE;
	m_bCheckHomed4 = FALSE;
	m_bCheckInPos4 = FALSE;
	m_bCheckMaxSwitch4 = FALSE;
	m_bCheckMinSwitch4 = FALSE;
	m_bCheckSoftError4 = FALSE;
	m_stringDesirePos4 = _T("");
	m_stringPosition4 = _T("");
	m_stringStatus4 = _T("");
	m_bCheckHardError5 = FALSE;
	m_bCheckHomed5 = FALSE;
	m_bCheckInPos5 = FALSE;
	m_bCheckMaxSwitch5 = FALSE;
	m_bCheckMinSwitch5 = FALSE;
	m_bCheckSoftError5 = FALSE;
	m_stringDesirePos5 = _T("");
	m_stringPosition5 = _T("");
	m_stringStatus5 = _T("");
	m_bCheckHardError6 = FALSE;
	m_bCheckHomed6 = FALSE;
	m_bCheckInPos6 = FALSE;
	m_bCheckMaxSwitch6 = FALSE;
	m_bCheckMinSwitch6 = FALSE;
	m_bCheckSoftError6 = FALSE;
	m_stringDesirePos6 = _T("");
	m_stringPosition6 = _T("");
	m_stringStatus6 = _T("");
	m_bCheckHardError7 = FALSE;
	m_bCheckHomed7 = FALSE;
	m_bCheckInPos7 = FALSE;
	m_bCheckMaxSwitch7 = FALSE;
	m_bCheckMinSwitch7 = FALSE;
	m_bCheckSoftError7 = FALSE;
	m_stringDesirePos7 = _T("");
	m_stringPosition7 = _T("");
	m_stringStatus7 = _T("");
	m_bCheckHardError8 = FALSE;
	m_bCheckHomed8 = FALSE;
	m_bCheckInPos8 = FALSE;
	m_bCheckMaxSwitch8 = FALSE;
	m_bCheckMinSwitch8 = FALSE;
	m_bCheckSoftError8 = FALSE;
	m_stringDesirePos8 = _T("");
	m_stringPosition8 = _T("");
	m_stringStatus8 = _T("");
	m_bCheckHardError9 = FALSE;
	m_bCheckHomed9 = FALSE;
	m_bCheckInPos9 = FALSE;
	m_bCheckMaxSwitch9 = FALSE;
	m_bCheckMinSwitch9 = FALSE;
	m_bCheckSoftError9 = FALSE;
	m_stringDesirePos9 = _T("");
	m_stringPosition9 = _T("");
	m_stringStatus9 = _T("");
	m_bCheckHardError10 = FALSE;
	m_bCheckHomed10 = FALSE;
	m_bCheckInPos10 = FALSE;
	m_bCheckMaxSwitch10 = FALSE;
	m_bCheckMinSwitch10 = FALSE;
	m_bCheckSoftError10 = FALSE;
	m_stringDesirePos10 = _T("");
	m_stringPosition10 = _T("");
	m_stringStatus10 = _T("");
	m_stringJoyDev1 = _T("");
	m_stringJoyDev10 = _T("");
	m_stringJoyDev2 = _T("");
	m_stringJoyDev3 = _T("");
	m_stringJoyDev4 = _T("");
	m_stringJoyDev5 = _T("");
	m_stringJoyDev6 = _T("");
	m_stringJoyDev7 = _T("");
	m_stringJoyDev8 = _T("");
	m_stringJoyDev9 = _T("");
	m_bGateStatus = FALSE;
	m_bESwitchStatus = FALSE;
	//}}AFX_DATA_INIT

	for(int ii=0;ii<10;ii++) {
		m_nOldPos[ii]=0xcccccc;
		m_nOldDesirePos[ii]=0xcccccc;
		m_nOldStatus[ii]=0xcccccc;
	}
	m_nESwitchStatus=0xcccccc;
	m_nGateStatus=0xcccccc;
}

CAxesPmacStatus::~CAxesPmacStatus()
{
}

void CAxesPmacStatus::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxesPmacStatus)
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_1, m_bCheckHardError1);
	DDX_Check(pDX, IDC_CHECK_HOMED_1, m_bCheckHomed1);
	DDX_Check(pDX, IDC_CHECK_IN_POS_1, m_bCheckInPos1);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_1, m_bCheckMaxSwitch1);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_1, m_bCheckMinSwitch1);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_1, m_bCheckSoftError1);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_1, m_stringDesirePos1);
	DDX_Text(pDX, IDC_EDIT_POSITION_1, m_stringPosition1);
	DDX_Text(pDX, IDC_EDIT_STATUS_1, m_stringStatus1);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_2, m_bCheckHardError2);
	DDX_Check(pDX, IDC_CHECK_HOMED_2, m_bCheckHomed2);
	DDX_Check(pDX, IDC_CHECK_IN_POS_2, m_bCheckInPos2);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_2, m_bCheckMaxSwitch2);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_2, m_bCheckMinSwitch2);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_2, m_bCheckSoftError2);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_2, m_stringDesirePos2);
	DDX_Text(pDX, IDC_EDIT_POSITION_2, m_stringPosition2);
	DDX_Text(pDX, IDC_EDIT_STATUS_2, m_stringStatus2);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_3, m_bCheckHardError3);
	DDX_Check(pDX, IDC_CHECK_HOMED_3, m_bCheckHomed3);
	DDX_Check(pDX, IDC_CHECK_IN_POS_3, m_bCheckInPos3);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_3, m_bCheckMaxSwitch3);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_3, m_bCheckMinSwitch3);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_3, m_bCheckSoftError3);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_3, m_stringDesirePos3);
	DDX_Text(pDX, IDC_EDIT_POSITION_3, m_stringPosition3);
	DDX_Text(pDX, IDC_EDIT_STATUS_3, m_stringStatus3);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_4, m_bCheckHardError4);
	DDX_Check(pDX, IDC_CHECK_HOMED_4, m_bCheckHomed4);
	DDX_Check(pDX, IDC_CHECK_IN_POS_4, m_bCheckInPos4);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_4, m_bCheckMaxSwitch4);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_4, m_bCheckMinSwitch4);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_4, m_bCheckSoftError4);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_4, m_stringDesirePos4);
	DDX_Text(pDX, IDC_EDIT_POSITION_4, m_stringPosition4);
	DDX_Text(pDX, IDC_EDIT_STATUS_4, m_stringStatus4);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_5, m_bCheckHardError5);
	DDX_Check(pDX, IDC_CHECK_HOMED_5, m_bCheckHomed5);
	DDX_Check(pDX, IDC_CHECK_IN_POS_5, m_bCheckInPos5);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_5, m_bCheckMaxSwitch5);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_5, m_bCheckMinSwitch5);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_5, m_bCheckSoftError5);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_5, m_stringDesirePos5);
	DDX_Text(pDX, IDC_EDIT_POSITION_5, m_stringPosition5);
	DDX_Text(pDX, IDC_EDIT_STATUS_5, m_stringStatus5);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_6, m_bCheckHardError6);
	DDX_Check(pDX, IDC_CHECK_HOMED_6, m_bCheckHomed6);
	DDX_Check(pDX, IDC_CHECK_IN_POS_6, m_bCheckInPos6);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_6, m_bCheckMaxSwitch6);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_6, m_bCheckMinSwitch6);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_6, m_bCheckSoftError6);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_6, m_stringDesirePos6);
	DDX_Text(pDX, IDC_EDIT_POSITION_6, m_stringPosition6);
	DDX_Text(pDX, IDC_EDIT_STATUS_6, m_stringStatus6);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_7, m_bCheckHardError7);
	DDX_Check(pDX, IDC_CHECK_HOMED_7, m_bCheckHomed7);
	DDX_Check(pDX, IDC_CHECK_IN_POS_7, m_bCheckInPos7);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_7, m_bCheckMaxSwitch7);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_7, m_bCheckMinSwitch7);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_7, m_bCheckSoftError7);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_7, m_stringDesirePos7);
	DDX_Text(pDX, IDC_EDIT_POSITION_7, m_stringPosition7);
	DDX_Text(pDX, IDC_EDIT_STATUS_7, m_stringStatus7);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_8, m_bCheckHardError8);
	DDX_Check(pDX, IDC_CHECK_HOMED_8, m_bCheckHomed8);
	DDX_Check(pDX, IDC_CHECK_IN_POS_8, m_bCheckInPos8);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_8, m_bCheckMaxSwitch8);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_8, m_bCheckMinSwitch8);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_8, m_bCheckSoftError8);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_8, m_stringDesirePos8);
	DDX_Text(pDX, IDC_EDIT_POSITION_8, m_stringPosition8);
	DDX_Text(pDX, IDC_EDIT_STATUS_8, m_stringStatus8);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_9, m_bCheckHardError9);
	DDX_Check(pDX, IDC_CHECK_HOMED_9, m_bCheckHomed9);
	DDX_Check(pDX, IDC_CHECK_IN_POS_9, m_bCheckInPos9);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_9, m_bCheckMaxSwitch9);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_9, m_bCheckMinSwitch9);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_9, m_bCheckSoftError9);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_9, m_stringDesirePos9);
	DDX_Text(pDX, IDC_EDIT_POSITION_9, m_stringPosition9);
	DDX_Text(pDX, IDC_EDIT_STATUS_9, m_stringStatus9);
	DDX_Check(pDX, IDC_CHECK_HARD_ERROR_10, m_bCheckHardError10);
	DDX_Check(pDX, IDC_CHECK_HOMED_10, m_bCheckHomed10);
	DDX_Check(pDX, IDC_CHECK_IN_POS_10, m_bCheckInPos10);
	DDX_Check(pDX, IDC_CHECK_MAX_SWITCH_10, m_bCheckMaxSwitch10);
	DDX_Check(pDX, IDC_CHECK_MIN_SWITCH_10, m_bCheckMinSwitch10);
	DDX_Check(pDX, IDC_CHECK_SOFT_ERROR_10, m_bCheckSoftError10);
	DDX_Text(pDX, IDC_EDIT_DESIRE_POS_10, m_stringDesirePos10);
	DDX_Text(pDX, IDC_EDIT_POSITION_10, m_stringPosition10);
	DDX_Text(pDX, IDC_EDIT_STATUS_10, m_stringStatus10);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_1, m_stringJoyDev1);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_10, m_stringJoyDev10);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_2, m_stringJoyDev2);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_3, m_stringJoyDev3);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_4, m_stringJoyDev4);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_5, m_stringJoyDev5);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_6, m_stringJoyDev6);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_7, m_stringJoyDev7);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_8, m_stringJoyDev8);
	DDX_Text(pDX, IDC_EDIT_JOY_DEV_9, m_stringJoyDev9);
	DDX_Check(pDX, IDC_CHECK_GATE_STATUS, m_bGateStatus);
	DDX_Check(pDX, IDC_CHECK_E_SWITCH, m_bESwitchStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxesPmacStatus, CPropertyPage)
	//{{AFX_MSG_MAP(CAxesPmacStatus)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesPmacStatus message handlers

BOOL CAxesPmacStatus::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboBrake.
	SetTimer(1,300,NULL);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAxesPmacStatus::OnTimer(UINT nIDEvent) 
{

	APP App = (APP) AfxGetApp();
	bool bFlag=FALSE;
	int ii=0;
	int	nTemp;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition1.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos1.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev1.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus1.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos1 = TRUE		: m_bCheckInPos1 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError1 = TRUE	: m_bCheckSoftError1 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError1 = TRUE	: m_bCheckHardError1 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch1 = TRUE	: m_bCheckMinSwitch1 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch1 = TRUE	: m_bCheckMaxSwitch1 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed1 = TRUE		: m_bCheckHomed1 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition2.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos2.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev2.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus2.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos2 = TRUE		: m_bCheckInPos2 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError2 = TRUE	: m_bCheckSoftError2 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError2 = TRUE	: m_bCheckHardError2 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch2 = TRUE	: m_bCheckMinSwitch2 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch2 = TRUE	: m_bCheckMaxSwitch2 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed2 = TRUE		: m_bCheckHomed2 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition3.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos3.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev3.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus3.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos3 = TRUE		: m_bCheckInPos3 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError3 = TRUE	: m_bCheckSoftError3 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError3 = TRUE	: m_bCheckHardError3 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch3 = TRUE	: m_bCheckMinSwitch3 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch3 = TRUE	: m_bCheckMaxSwitch3 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed3 = TRUE		: m_bCheckHomed3 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition4.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos4.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev4.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus4.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos4 = TRUE		: m_bCheckInPos4 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError4 = TRUE	: m_bCheckSoftError4 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError4 = TRUE	: m_bCheckHardError4 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch4 = TRUE	: m_bCheckMinSwitch4 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch4 = TRUE	: m_bCheckMaxSwitch4 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed4 = TRUE		: m_bCheckHomed4 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition5.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos5.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev5.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus5.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos5 = TRUE		: m_bCheckInPos5 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError5 = TRUE	: m_bCheckSoftError5 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError5 = TRUE	: m_bCheckHardError5 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch5 = TRUE	: m_bCheckMinSwitch5 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch5 = TRUE	: m_bCheckMaxSwitch5 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed5 = TRUE		: m_bCheckHomed5 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition6.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos6.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev6.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus6.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos6 = TRUE		: m_bCheckInPos6 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError6 = TRUE	: m_bCheckSoftError6 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError6 = TRUE	: m_bCheckHardError6 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch6 = TRUE	: m_bCheckMinSwitch6 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch6 = TRUE	: m_bCheckMaxSwitch6 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed6 = TRUE		: m_bCheckHomed6 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition7.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos7.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev7.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus7.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos7 = TRUE		: m_bCheckInPos7 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError7 = TRUE	: m_bCheckSoftError7 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError7 = TRUE	: m_bCheckHardError7 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch7 = TRUE	: m_bCheckMinSwitch7 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch7 = TRUE	: m_bCheckMaxSwitch7 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed7 = TRUE		: m_bCheckHomed7 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition8.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos8.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev8.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus8.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos8 = TRUE		: m_bCheckInPos8 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError8 = TRUE	: m_bCheckSoftError8 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError8 = TRUE	: m_bCheckHardError8 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch8 = TRUE	: m_bCheckMinSwitch8 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch8 = TRUE	: m_bCheckMaxSwitch8 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed8 = TRUE		: m_bCheckHomed8 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition9.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos9.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev9.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus9.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos9 = TRUE		: m_bCheckInPos9 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError9 = TRUE	: m_bCheckSoftError9 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError9 = TRUE	: m_bCheckHardError9 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch9 = TRUE	: m_bCheckMinSwitch9 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch9 = TRUE	: m_bCheckMaxSwitch9 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed9 = TRUE		: m_bCheckHomed9 = FALSE;
		bFlag=TRUE;
	}
	ii++;

	if(m_nOldPos[ii]-App->m_Axes[ii].nPos) {
		m_stringPosition10.Format("%d",m_nOldPos[ii]=App->m_Axes[ii].nPos);
		bFlag=TRUE;
	}
	if(m_nOldDesirePos[ii]-App->m_Axes[ii].nDesirePos) {
		m_stringDesirePos10.Format("%d",m_nOldDesirePos[ii]=App->m_Axes[ii].nDesirePos);
		bFlag=TRUE;
	}
	if(m_nOldJoyDev[ii] - (nTemp=App->m_ServoUser.ReadDpram(App->m_Axes[ii].nMemOffset+PMAC_JOY_DEV))) {
		m_stringJoyDev10.Format("%d",m_nOldJoyDev[ii]=nTemp);
		bFlag=TRUE;
	}
	if(m_nOldStatus[ii]-App->m_Axes[ii].nStatus) {
		m_stringStatus10.Format("%06x",m_nOldStatus[ii]=App->m_Axes[ii].nStatus);
		m_nOldStatus[ii] & PMAC_IN_POS		? m_bCheckInPos10 = TRUE		: m_bCheckInPos10 = FALSE;
		m_nOldStatus[ii] & PMAC_SOFT_LIMIT	? m_bCheckSoftError10 = TRUE	: m_bCheckSoftError10 = FALSE;
		m_nOldStatus[ii] & PMAC_HARD_LIMIT	? m_bCheckHardError10 = TRUE	: m_bCheckHardError10 = FALSE;
		m_nOldStatus[ii] & PMAC_MIN_SWITCH	? m_bCheckMinSwitch10 = TRUE	: m_bCheckMinSwitch10 = FALSE;
		m_nOldStatus[ii] & PMAC_MAX_SWITCH	? m_bCheckMaxSwitch10 = TRUE	: m_bCheckMaxSwitch10 = FALSE;
		m_nOldStatus[ii] & PMAC_HOMED		? m_bCheckHomed10 = TRUE		: m_bCheckHomed10 = FALSE;
		bFlag=TRUE;
	}
	ii++;

/*
	if(m_nGateStatus-App->m_ServoUser.GetGateStatus()) {
		m_nGateStatus=App->m_ServoUser.GetGateStatus();
		m_nGateStatus==0 ? m_bGateStatus = FALSE	: m_bGateStatus = TRUE;
		bFlag=TRUE;
	}
	*/
	if(m_nESwitchStatus-App->m_ServoUser.GetESwitchStatus()) {
		m_nESwitchStatus=App->m_ServoUser.GetESwitchStatus();
		m_nESwitchStatus==0 ? m_bESwitchStatus = FALSE	: m_bESwitchStatus = TRUE;
		bFlag=TRUE;
	}



	if(bFlag==TRUE)	UpdateData(FALSE);
	CPropertyPage::OnTimer(nIDEvent);
}
