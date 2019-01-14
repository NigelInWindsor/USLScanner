// AssignAxesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AssignAxesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssignAxesDlg dialog


CAssignAxesDlg::CAssignAxesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg )
	: CDialog(CAssignAxesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssignAxesDlg)
	//}}AFX_DATA_INIT

	m_pDlg = pDlg;
	
}

CAssignAxesDlg::~CAssignAxesDlg()
{
}


void CAssignAxesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssignAxesDlg)
	DDX_Control(pDX, IDC_EDIT_SAFETY_OUTPUT, m_editSafetyOutput);
	DDX_Control(pDX, IDC_EDIT_X_HOLDING_FIXED, m_editXHoldingFixed);
	DDX_Control(pDX, IDC_COMBO_X_HOLDING_FIXED, m_comboXHoldingFixed);
	DDX_Control(pDX, IDC_EDIT_PUMP_1_BIT, m_editPump1Bit);
	DDX_Control(pDX, IDC_EDIT_PUMP_0_BIT, m_editPump0Bit);
	DDX_Control(pDX, IDC_COMBO_AUDIO_PORT, m_comboAudioPort);
	DDX_Control(pDX, IDC_EDIT_AUDIO_BIT, m_editAudioBit);
	DDX_Control(pDX, IDC_EDIT_SI10_OUTP, m_editSi10Output);
	DDX_Control(pDX, IDC_COMBO_OUT_PORT, m_comboAndonOutputPort);
	DDX_Control(pDX, IDC_EDIT_ANDON_STATUS, m_editAndonStatus);
	DDX_Control(pDX, IDC_EDIT_OUTP, m_editOutput);
	DDX_Control(pDX, IDC_EDIT_ANDON_WARNING, m_editAndonWarning);
	DDX_Control(pDX, IDC_EDIT_ANDON_RUNNING, m_editAndonRunning);
	DDX_Control(pDX, IDC_EDIT_ANDON_FAULT, m_editAndonFault);
	DDX_Control(pDX, IDC_EDIT_ANDON_OKAY, m_editAndonOkay);
	DDX_Control(pDX, IDC_EDIT_KILL, m_editKill);
	DDX_Control(pDX, IDC_EDIT_INP, m_editInput);
	DDX_Control(pDX, IDC_EDIT_DOCKED, m_editDocked);
	DDX_Control(pDX, IDC_EDIT_AUTOMATIC, m_editAutomatic);
	DDX_Control(pDX, IDC_EDIT_LIFT_DOWN_IN, m_editLiftDownIn);
	DDX_Control(pDX, IDC_EDIT_LIFT_UP_IN, m_editLiftUpIn);
	DDX_Control(pDX, IDC_EDIT_LIFT_DOWN, m_editLiftDown);
	DDX_Control(pDX, IDC_EDIT_LIFT_UP, m_editLiftUp);
	DDX_Control(pDX, IDC_EDIT_INPUT_XOR, m_editInputXOR);
	DDX_Control(pDX, IDC_CHECK_TURNTABLE, m_checkTurntable);
	DDX_Control(pDX, IDC_CHECK_BRIDGE, m_checkBridge);
	DDX_Control(pDX, IDC_CHECK_HOLDING_FIXTURE, m_checkHoldingFixture);
	DDX_Control(pDX, IDC_EDIT_X_BRIDGE_NUMBER, m_editXBridgeNumber);
	DDX_Control(pDX, IDC_COMBO_X_AXIS_BRIDGE_TYPE, m_comboXBridgeType);
	DDX_Control(pDX, IDC_EDIT_E_BIT, m_editEBit);
	DDX_Control(pDX, IDC_EDIT_HOLDING_BRAKE, m_editHoldingBrake);
	DDX_Control(pDX, IDC_EDIT_LEFT_AIR, m_editLeftAir);
	DDX_Control(pDX, IDC_EDIT_RIGHT_AIR, m_editRightAir);
	DDX_Control(pDX, IDC_EDIT_GATE_BIT, m_editGateBit);
	DDX_Control(pDX, IDC_EDIT_DOOR_BIT, m_editDoorBit);
	DDX_Control(pDX, IDC_EDIT_BRAKE_BIT, m_editBrakeBit);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_0, m_checkAvailable0);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_1, m_checkAvailable1);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_2, m_checkAvailable2);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_3, m_checkAvailable3);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_4, m_checkAvailable4);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_5, m_checkAvailable5);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_6, m_checkAvailable6);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_7, m_checkAvailable7);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_8, m_checkAvailable8);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_9, m_checkAvailable9);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_10, m_checkAvailable10);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_11, m_checkAvailable11);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_12, m_checkAvailable12);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_13, m_checkAvailable13);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_14, m_checkAvailable14);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_15, m_checkAvailable15);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_16, m_checkAvailable16);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_17, m_checkAvailable17);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_18, m_checkAvailable18);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_19, m_checkAvailable19);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_20, m_checkAvailable20);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_21, m_checkAvailable21);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_22, m_checkAvailable22);
	DDX_Control(pDX, IDC_CHECK_AVAILABLE_23, m_checkAvailable23);
	DDX_Control(pDX, IDC_EDIT_LEFT_PUMP, m_editLeftPump);
	DDX_Control(pDX, IDC_EDIT_RIGHT_PUMP, m_editRightPump);
	DDX_Control(pDX, IDC_EDIT_X_HOLDING_MOVE, m_editXHoldingMove);
	DDX_Control(pDX, IDC_EDIT_Y_HOLDING_MOVE, m_editYHoldingMove);
	DDX_Control(pDX, IDC_EDIT_Z_HOLDING_MOVE, m_editZHoldingMove);
	DDX_Control(pDX, IDC_EDIT_Y_HOLDING_FIXED, m_editYHoldingFixed);
	DDX_Control(pDX, IDC_EDIT_Z_HOLDING_FIXED, m_editZHoldingFixed);
	DDX_Control(pDX, IDC_EDIT_X_RIGHT_NUMBER, m_editXRightNumber);
	DDX_Control(pDX, IDC_EDIT_Y_RIGHT_NUMBER, m_editYRightNumber);
	DDX_Control(pDX, IDC_EDIT_Z_RIGHT_NUMBER, m_editZRightNumber);
	DDX_Control(pDX, IDC_EDIT_XT_RIGHT_NUMBER, m_editXTRightNumber);
	DDX_Control(pDX, IDC_EDIT_YT_RIGHT_NUMBER, m_editYTRightNumber);
	DDX_Control(pDX, IDC_EDIT_R_RIGHT_NUMBER, m_editRRightNumber);
	DDX_Control(pDX, IDC_EDIT_X_LEFT_NUMBER, m_editXLeftNumber);
	DDX_Control(pDX, IDC_EDIT_Y_LEFT_NUMBER, m_editYLeftNumber);
	DDX_Control(pDX, IDC_EDIT_Z_LEFT_NUMBER, m_editZLeftNumber);
	DDX_Control(pDX, IDC_EDIT_XT_LEFT_NUMBER, m_editXTLeftNumber);
	DDX_Control(pDX, IDC_EDIT_YT_LEFT_NUMBER, m_editYTLeftNumber);
	DDX_Control(pDX, IDC_EDIT_R_LEFT_NUMBER, m_editRLeftNumber);
	DDX_Control(pDX, IDC_COMBO_LEFT_PUMP, m_comboLeftPump);
	DDX_Control(pDX, IDC_COMBO_RIGHT_PUMP, m_comboRightPump);
	DDX_Control(pDX, IDC_COMBO_X_HOLDING_MOVE, m_comboXHoldingMove);
	DDX_Control(pDX, IDC_COMBO_Y_HOLDING_MOVE, m_comboYHoldingMove);
	DDX_Control(pDX, IDC_COMBO_Z_HOLDING_MOVE, m_comboZHoldingMove);
	DDX_Control(pDX, IDC_COMBO_Y_HOLDING_FIXED, m_comboYHoldingFixed);
	DDX_Control(pDX, IDC_COMBO_Z_HOLDING_FIXED, m_comboZHoldingFixed);
	DDX_Control(pDX, IDC_COMBO_X_AXIS_RIGHT_TYPE, m_comboXRightType);
	DDX_Control(pDX, IDC_COMBO_Y_AXIS_RIGHT_TYPE, m_comboYRightType);
	DDX_Control(pDX, IDC_COMBO_Z_AXIS_RIGHT_TYPE, m_comboZRightType);
	DDX_Control(pDX, IDC_COMBO_XT_AXIS_RIGHT_TYPE, m_comboXTRightType);
	DDX_Control(pDX, IDC_COMBO_YT_AXIS_RIGHT_TYPE, m_comboYTRightType);
	DDX_Control(pDX, IDC_COMBO_R_AXIS_RIGHT_TYPE, m_comboRRightType);
	DDX_Control(pDX, IDC_COMBO_X_AXIS_LEFT_TYPE, m_comboXLeftType);
	DDX_Control(pDX, IDC_COMBO_Y_AXIS_LEFT_TYPE, m_comboYLeftType);
	DDX_Control(pDX, IDC_COMBO_Z_AXIS_LEFT_TYPE, m_comboZLeftType);
	DDX_Control(pDX, IDC_COMBO_XT_AXIS_LEFT_TYPE, m_comboXTLeftType);
	DDX_Control(pDX, IDC_COMBO_YT_AXIS_LEFT_TYPE, m_comboYTLeftType);
	DDX_Control(pDX, IDC_COMBO_R_AXIS_LEFT_TYPE, m_comboRLeftType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_AUDIO, m_listAudio);

}


BEGIN_MESSAGE_MAP(CAssignAxesDlg, CDialog)
	//{{AFX_MSG_MAP(CAssignAxesDlg)
	ON_BN_CLICKED(IDC_CHECK_HOLDING_FIXTURE, OnCheckHoldingFixture)
	ON_BN_CLICKED(IDC_CHECK_BRIDGE, OnCheckBridge)
	ON_BN_CLICKED(IDC_CHECK_TURNTABLE, OnCheckTurntable)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_AUTOMATIC, OnChangeEditAutomatic)
	ON_EN_CHANGE(IDC_EDIT_BRAKE_BIT, OnChangeEditBrakeBit)
	ON_EN_CHANGE(IDC_EDIT_DOCKED, OnChangeEditDocked)
	ON_EN_CHANGE(IDC_EDIT_DOOR_BIT, OnChangeEditDoorBit)
	ON_EN_CHANGE(IDC_EDIT_E_BIT, OnChangeEditEBit)
	ON_EN_CHANGE(IDC_EDIT_GATE_BIT, OnChangeEditGateBit)
	ON_EN_CHANGE(IDC_EDIT_INPUT_XOR, OnChangeEditInputXor)
	ON_EN_CHANGE(IDC_EDIT_HOLDING_BRAKE, OnChangeEditHoldingBrake)
	ON_EN_CHANGE(IDC_EDIT_KILL, OnChangeEditKill)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnButtonAccept)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_PORT, OnSelchangeComboAudioPort)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_AUDIO, OnCheckChangeListAudio)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssignAxesDlg message handlers
void CAssignAxesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CAssignAxesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CAssignAxesDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CAssignAxesDlg::OnDestroy() 
{
	if (CAssignAxesDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CAssignAxesDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CAssignAxesDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();

	if (CAssignAxesDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CAssignAxesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	m_comboAndonOutputPort.AddString(L"PMAC");
	m_comboAndonOutputPort.AddString(L"Si10");
	m_comboAndonOutputPort.AddString(L"Field bus");
	m_comboAudioPort.AddString(L"PMAC");
	m_comboAudioPort.AddString(L"Si10");

	
	SetTimer(1,200,NULL);

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAssignAxesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_Tank.nXBridge+1);
	m_editXBridgeNumber.SetWindowText(Buff);
	m_comboXBridgeType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXBridge].nMotorType);
	
	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.Format(_T("%d"),theApp.m_Tank.nXLeft+1);
		m_editXLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYLeft+1);
		m_editYLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nZLeft+1);
		m_editZLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nXtLeft+1);
		m_editXTLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYtLeft+1);
		m_editYTLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nRLeft+1);
		m_editRLeftNumber.SetWindowText(Buff);

		Buff.Format(_T("%d"),theApp.m_Tank.nXRight+1);
		m_editXRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYRight+1);
		m_editYRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nZRight+1);
		m_editZRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nXtRight+1);
		m_editXTRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYtRight+1);
		m_editYTRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nRRight+1);
		m_editRRightNumber.SetWindowText(Buff);

		Buff.Format(_T("%d"),theApp.m_Tank.nLeftPump+1);
		m_editLeftPump.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nRightPump+1);
		m_editRightPump.SetWindowText(Buff);

		m_comboXLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType);
		m_comboYLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType);
		m_comboZLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType);
		m_comboXTLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType);
		m_comboYTLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType);
		m_comboRLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType);

		m_comboXRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType);
		m_comboYRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType);
		m_comboZRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType);
		m_comboXTRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType);
		m_comboYTRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType);
		m_comboRRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRRight].nMotorType);

		m_comboLeftPump.SetCurSel(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMotorType);
		m_comboRightPump.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRightPump].nMotorType);

		Buff.Format(_T("%02xh"),theApp.m_Tank.nLeftAirBit);
		m_editLeftAir.SetWindowText(Buff);
		Buff.Format(_T("%02xh"),theApp.m_Tank.nRightAirBit);
		m_editRightAir.SetWindowText(Buff);
		break;
	case 1:
		Buff.Format(_T("%d"),theApp.m_Tank.nXRight+1);
		m_editXLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYRight+1);
		m_editYLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nZRight+1);
		m_editZLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nXtRight+1);
		m_editXTLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYtRight+1);
		m_editYTLeftNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nRRight+1);
		m_editRLeftNumber.SetWindowText(Buff);

		Buff.Format(_T("%d"),theApp.m_Tank.nXLeft+1);
		m_editXRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYLeft+1);
		m_editYRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nZLeft+1);
		m_editZRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nXtLeft+1);
		m_editXTRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nYtLeft+1);
		m_editYTRightNumber.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nRLeft+1);
		m_editRRightNumber.SetWindowText(Buff);

		Buff.Format(_T("%d"),theApp.m_Tank.nRightPump+1);
		m_editLeftPump.SetWindowText(Buff);
		Buff.Format(_T("%d"),theApp.m_Tank.nLeftPump+1);
		m_editRightPump.SetWindowText(Buff);

		m_comboXLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType);
		m_comboYLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType);
		m_comboZLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType);
		m_comboXTLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType);
		m_comboYTLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType);
		m_comboRLeftType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRRight].nMotorType);

		m_comboXRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType);
		m_comboYRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType);
		m_comboZRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType);
		m_comboXTRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType);
		m_comboYTRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType);
		m_comboRRightType.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType);

		m_comboLeftPump.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRightPump].nMotorType);
		m_comboRightPump.SetCurSel(theApp.m_Axes[theApp.m_Tank.nLeftPump].nMotorType);

		Buff.Format(_T("%02xh"),theApp.m_Tank.nRightAirBit);
		m_editLeftAir.SetWindowText(Buff);
		Buff.Format(_T("%02xh"),theApp.m_Tank.nLeftAirBit);
		m_editRightAir.SetWindowText(Buff);
		break;
	}
	Buff.Format(_T("%d"),theApp.m_Tank.nXStep+1);
	m_editXHoldingMove.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_Tank.nYStep+1);
	m_editYHoldingMove.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_Tank.nZStep+1);
	m_editZHoldingMove.SetWindowText(Buff);

	Buff.Format(_T("%d"),theApp.m_Tank.nRStep+1);
	m_editXHoldingFixed.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_Tank.nXtStep+1);
	m_editYHoldingFixed.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_Tank.nYtStep+1);
	m_editZHoldingFixed.SetWindowText(Buff);
	theApp.m_Tank.nXStep %= 32;
	theApp.m_Tank.nYStep %= 32;
	theApp.m_Tank.nZStep %= 32;
	theApp.m_Tank.nXtStep %= 32;
	theApp.m_Tank.nYtStep %= 32;
	theApp.m_Tank.nRStep %= 32;

	m_comboXHoldingMove.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXStep].nMotorType);
	m_comboYHoldingMove.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYStep].nMotorType);
	m_comboZHoldingMove.SetCurSel(theApp.m_Axes[theApp.m_Tank.nZStep].nMotorType);
	m_comboXHoldingFixed.SetCurSel(theApp.m_Axes[theApp.m_Tank.nRStep].nMotorType);
	m_comboYHoldingFixed.SetCurSel(theApp.m_Axes[theApp.m_Tank.nXtStep].nMotorType);
	m_comboZHoldingFixed.SetCurSel(theApp.m_Axes[theApp.m_Tank.nYtStep].nMotorType);

	m_checkAvailable0.SetCheck(theApp.m_Axes[0].bAvailable&1);
	m_checkAvailable1.SetCheck(theApp.m_Axes[1].bAvailable&1);
	m_checkAvailable2.SetCheck(theApp.m_Axes[2].bAvailable&1);
	m_checkAvailable3.SetCheck(theApp.m_Axes[3].bAvailable&1);
	m_checkAvailable4.SetCheck(theApp.m_Axes[4].bAvailable&1);
	m_checkAvailable5.SetCheck(theApp.m_Axes[5].bAvailable&1);
	m_checkAvailable6.SetCheck(theApp.m_Axes[6].bAvailable&1);
	m_checkAvailable7.SetCheck(theApp.m_Axes[7].bAvailable&1);
	m_checkAvailable8.SetCheck(theApp.m_Axes[8].bAvailable&1);
	m_checkAvailable9.SetCheck(theApp.m_Axes[9].bAvailable&1);
	m_checkAvailable10.SetCheck(theApp.m_Axes[10].bAvailable&1);
	m_checkAvailable11.SetCheck(theApp.m_Axes[11].bAvailable&1);
	m_checkAvailable12.SetCheck(theApp.m_Axes[12].bAvailable&1);
	m_checkAvailable13.SetCheck(theApp.m_Axes[13].bAvailable&1);
	m_checkAvailable14.SetCheck(theApp.m_Axes[14].bAvailable&1);
	m_checkAvailable15.SetCheck(theApp.m_Axes[15].bAvailable&1);
	m_checkAvailable16.SetCheck(theApp.m_Axes[16].bAvailable&1);
	m_checkAvailable17.SetCheck(theApp.m_Axes[17].bAvailable&1);
	m_checkAvailable18.SetCheck(theApp.m_Axes[18].bAvailable&1);
	m_checkAvailable19.SetCheck(theApp.m_Axes[19].bAvailable&1);
	m_checkAvailable20.SetCheck(theApp.m_Axes[20].bAvailable&1);
	m_checkAvailable21.SetCheck(theApp.m_Axes[21].bAvailable&1);
	m_checkAvailable22.SetCheck(theApp.m_Axes[22].bAvailable&1);
	m_checkAvailable23.SetCheck(theApp.m_Axes[23].bAvailable&1);

	Buff.Format(_T("%02xh"),theApp.m_Tank.nBrakeBit0);
	m_editBrakeBit.SetWindowText(Buff);
	Buff.Format(_T("%02xh"),theApp.m_Tank.nBrakeBit1);
	m_editHoldingBrake.SetWindowText(Buff);

	Buff.Format(_T("%02xh"),theApp.m_Tank.nEmergencyBit);
	m_editEBit.SetWindowText(Buff);
	Buff.Format(_T("%02xh"),theApp.m_Tank.nGateBit);
	m_editGateBit.SetWindowText(Buff);
	Buff.Format(_T("%02xh"),theApp.m_Tank.nDoorBit);
	m_editDoorBit.SetWindowText(Buff);
	Buff.Format(_T("%02xh"),theApp.m_Tank.nAutomaticBit);
	m_editAutomatic.SetWindowText(Buff);

	m_checkHoldingFixture.SetCheck(theApp.m_Tank.bHoldingFixture & 1);
	m_checkBridge.SetCheck(theApp.m_Tank.bBridge & 1);
	m_checkTurntable.SetCheck(theApp.m_Tank.bTurnTable & 1);


	Buff.Format(_T("%02x h"),theApp.m_Tank.nGeneralInputXOR);
	m_editInputXOR.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nCommandLiftUp);
	m_editLiftUp.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nCommandLiftDown);
	m_editLiftDown.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nDockedBit);
	m_editDocked.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nKillBit);
	m_editKill.SetWindowText(Buff);

	Buff.Format(_T("%02x h"),theApp.m_Tank.nLiftUpLimitSwitch);
	m_editLiftUpIn.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nLiftDownLimitSwitch);
	m_editLiftDownIn.SetWindowText(Buff);

	Buff.Format(_T("%02x h"),theApp.m_Tank.nAndonOkay);
	m_editAndonOkay.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nAndonCycleRunning);
	m_editAndonRunning.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nAndonWarning);
	m_editAndonWarning.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Tank.nAndonFault);
	m_editAndonFault.SetWindowText(Buff);

	m_comboAndonOutputPort.SetCurSel(theApp.m_Tank.nAndonOutputPort);

	m_comboAudioPort.SetCurSel(theApp.m_Tank.nAudioPort);

	Buff.Format(L"%02x",theApp.m_Tank.nAudioBit);
	m_editAudioBit.SetWindowText(Buff);

	Buff.Format(L"%02x",theApp.m_Tank.nPumpBit[0]);
	m_editPump0Bit.SetWindowText(Buff);
	Buff.Format(L"%02x",theApp.m_Tank.nPumpBit[1]);
	m_editPump1Bit.SetWindowText(Buff);

	m_listAudio.ResetContent();
	m_listAudio.SetCheckStyle(BS_AUTOCHECKBOX);

	Buff.LoadString(IDS_Magnetic_Knock_off);	m_listAudio.AddString(Buff);
	Buff.LoadString(IDS_Water_Velocity);		m_listAudio.AddString(Buff);

	m_listAudio.SetCheck(0,(theApp.m_Tank.nAudioMaskSelect & 1)>>0);
	m_listAudio.SetCheck(1,(theApp.m_Tank.nAudioMaskSelect & 2)>>1);


	SetAccessPrivelages();
}


int CAssignAxesDlg::MinMax(int nV, int nMin, int nMax)
{

	if(nV<nMin) return nMin;
	if(nV>nMax) return nMax;
	return nV;

}

void CAssignAxesDlg::OnCheckHoldingFixture() 
{
	theApp.m_Tank.bHoldingFixture = m_checkHoldingFixture.GetCheck()&1;

	SetAccessPrivelages();
}

void CAssignAxesDlg::SetAccessPrivelages()
{
	bool bFlag;

	theApp.m_Tank.bHoldingFixture==FALSE ? bFlag = FALSE : bFlag = TRUE;


	m_editXHoldingMove.EnableWindow(bFlag);
	m_editYHoldingMove.EnableWindow(bFlag);
	m_editZHoldingMove.EnableWindow(bFlag);
	m_editXHoldingFixed.EnableWindow(bFlag);
	m_editYHoldingFixed.EnableWindow(bFlag);
	m_editZHoldingFixed.EnableWindow(bFlag);
	m_comboXHoldingMove.EnableWindow(bFlag);
	m_comboYHoldingMove.EnableWindow(bFlag);
	m_comboZHoldingMove.EnableWindow(bFlag);
	m_comboXHoldingFixed.EnableWindow(bFlag);
	m_comboYHoldingFixed.EnableWindow(bFlag);
	m_comboZHoldingFixed.EnableWindow(bFlag);

}

void CAssignAxesDlg::OnCheckBridge() 
{
	theApp.m_Tank.bBridge = m_checkBridge.GetCheck()&1;

	SetAccessPrivelages();
}

void CAssignAxesDlg::OnCheckTurntable() 
{
	theApp.m_Tank.bTurnTable = m_checkTurntable.GetCheck()&1;

	SetAccessPrivelages();
}



void CAssignAxesDlg::OnChangeEditAutomatic() 
{
	CString Buff;

	m_editAutomatic.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nAutomaticBit);
}


void CAssignAxesDlg::OnChangeEditDocked() 
{
	CString Buff;

	m_editDocked.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nDockedBit);
}

void CAssignAxesDlg::OnChangeEditDoorBit() 
{
	CString Buff;

	m_editDoorBit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nDoorBit);
}

void CAssignAxesDlg::OnChangeEditEBit() 
{
	CString Buff;

	m_editEBit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nEmergencyBit);
}

void CAssignAxesDlg::OnChangeEditGateBit() 
{
	CString Buff;

	m_editGateBit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nGateBit);

}

void CAssignAxesDlg::OnChangeEditInputXor() 
{
	CString Buff;

	m_editInputXOR.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nGeneralInputXOR);
	theApp.m_PmacUser.SetGeneralInputXor();
}

void CAssignAxesDlg::OnChangeEditBrakeBit() 
{
	CString Buff;

	m_editBrakeBit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nBrakeBit0);
}

void CAssignAxesDlg::OnChangeEditHoldingBrake() 
{
	CString Buff;

	m_editHoldingBrake.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nBrakeBit1);
}

void CAssignAxesDlg::OnTimer(UINT nIDEvent) 
{
	CButton* pButton;
	static AndonEventType nEvent;

	int nInp = theApp.m_Motors.GetInputPort();	
	pButton = (CButton* ) GetDlgItem(IDC_RADIO_E_SWITCH);
	theApp.m_Tank.nEmergencyBit & nInp ? pButton->SetCheck(1) : pButton->SetCheck(0);

	pButton = (CButton* ) GetDlgItem(IDC_RADIO_GATE);
	theApp.m_Tank.nGateBit & nInp ? pButton->SetCheck(1) : pButton->SetCheck(0);

	pButton = (CButton* ) GetDlgItem(IDC_RADIO_DOOR);
	theApp.m_Tank.nDoorBit & nInp ? pButton->SetCheck(1) : pButton->SetCheck(0);

	pButton = (CButton* ) GetDlgItem(IDC_RADIO_DOCKED);
	theApp.m_Tank.nDockedBit & nInp ? pButton->SetCheck(1) : pButton->SetCheck(0);

	pButton = (CButton* ) GetDlgItem(IDC_RADIO_AUTOMATIC);
	theApp.m_Tank.nAutomaticBit & nInp ? pButton->SetCheck(1) : pButton->SetCheck(0);

	pButton = (CButton* ) GetDlgItem(IDC_RADIO_KILLBIT);
	theApp.m_Tank.nKillBit & nInp ? pButton->SetCheck(1) : pButton->SetCheck(0);


	CString Buff;
	Buff.Format(_T("%02x h"),nInp);
	m_editInput.SetWindowText(Buff);

	Buff.Format(L"%02x h",theApp.m_PmacUser.m_nGeneralOutput);
	m_editOutput.SetWindowText(Buff);

	Buff.Format(L"%02x h",theApp.m_Si10User.m_nGeneralOutput);
	m_editSi10Output.SetWindowText(Buff);

	Buff.Format(L"%02x h",theApp.m_SafetySerial.m_nOutputPort);
	m_editSafetyOutput.SetWindowText(Buff);

//	if(nEvent != theApp.m_nOldAndonAction) {
		switch(theApp.m_nOldAndonAction) {
		case MachineOkay: Buff.LoadString(IDS_Machine_Okay);
			break;
		case CycleRunning: Buff.LoadString(IDS_Cycle_Running);
			break;
		case MachineWarning: Buff.LoadString(IDS_Machine_Warning);
			break;
		case MachineFault: Buff.LoadString(IDS_Machine_Fault);
			break;
		}
		nEvent = theApp.m_nOldAndonAction;
		m_editAndonStatus.SetWindowText(Buff);
//	}

	CDialog::OnTimer(nIDEvent);
}


void CAssignAxesDlg::OnChangeEditKill() 
{
	CString Buff;

	m_editKill.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nKillBit);
	
}


void CAssignAxesDlg::OnButtonAccept() 
{
	CString Buff;

	theApp.m_Tank.nFingerLeft = 10;

	m_editXBridgeNumber.GetWindowText(Buff);
	theApp.m_Tank.nXBridge = MinMax(_ttoi(Buff)-1,0,31);
	theApp.m_Axes[theApp.m_Tank.nXBridge].nMotorType=m_comboXBridgeType.GetCurSel();

	switch(theApp.m_nSide0Orientation) {
	case 0:
		m_editXLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nXLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editYLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nYLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editZLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nZLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editXTLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nXtLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editYTLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nYtLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editRLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nRLeft = MinMax(_ttoi(Buff)-1,0,31);

		m_editXRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nXRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editYRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nYRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editZRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nZRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editXTRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nXtRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editYTRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nYtRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editRRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nRRight = MinMax(_ttoi(Buff)-1,0,31);

		m_editLeftPump.GetWindowText(Buff);
		theApp.m_Tank.nLeftPump = MinMax(_ttoi(Buff)-1,0,31);
		m_editRightPump.GetWindowText(Buff);
		theApp.m_Tank.nRightPump = MinMax(_ttoi(Buff)-1,0,31);

		theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType = m_comboXLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType = m_comboYLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType = m_comboZLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType = m_comboXTLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType = m_comboYTLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType = m_comboRLeftType.GetCurSel();

		theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType = m_comboXRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType = m_comboYRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType = m_comboZRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType = m_comboXTRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType = m_comboYTRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nRRight].nMotorType = m_comboRRightType.GetCurSel();

		theApp.m_Axes[theApp.m_Tank.nLeftPump].nMotorType = m_comboLeftPump.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nRightPump].nMotorType = m_comboRightPump.GetCurSel();

		m_editLeftAir.GetWindowText(Buff);
		swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nLeftAirBit);
		m_editRightAir.GetWindowText(Buff);
		swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nRightAirBit);
		break;
	case 1:
		m_editXLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nXRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editYLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nYRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editZLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nZRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editXTLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nXtRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editYTLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nYtRight = MinMax(_ttoi(Buff)-1,0,31);
		m_editRLeftNumber.GetWindowText(Buff);
		theApp.m_Tank.nRRight = MinMax(_ttoi(Buff)-1,0,31);

		m_editXRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nXLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editYRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nYLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editZRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nZLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editXTRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nXtLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editYTRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nYtLeft = MinMax(_ttoi(Buff)-1,0,31);
		m_editRRightNumber.GetWindowText(Buff);
		theApp.m_Tank.nRLeft = MinMax(_ttoi(Buff)-1,0,31);

		m_editLeftPump.GetWindowText(Buff);
		theApp.m_Tank.nRightPump = MinMax(_ttoi(Buff)-1,0,31);
		m_editRightPump.GetWindowText(Buff);
		theApp.m_Tank.nLeftPump = MinMax(_ttoi(Buff)-1,0,31);

		theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType = m_comboXLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType = m_comboYLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType = m_comboZLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType = m_comboXTLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType = m_comboYTLeftType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nRRight].nMotorType = m_comboRLeftType.GetCurSel();

		theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType = m_comboXRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType = m_comboYRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType = m_comboZRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType = m_comboXTRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType = m_comboYTRightType.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType = m_comboRRightType.GetCurSel();

		theApp.m_Axes[theApp.m_Tank.nRightPump].nMotorType = m_comboLeftPump.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nLeftPump].nMotorType = m_comboRightPump.GetCurSel();

		m_editLeftAir.GetWindowText(Buff);
		swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nRightAirBit);
		m_editRightAir.GetWindowText(Buff);
		swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nLeftAirBit);

		break;
	}

	m_editXHoldingMove.GetWindowText(Buff);
	theApp.m_Tank.nXStep = MinMax(_ttoi(Buff)-1,0,31);
	m_editYHoldingMove.GetWindowText(Buff);
	theApp.m_Tank.nYStep = MinMax(_ttoi(Buff)-1,0,31);
	m_editZHoldingMove.GetWindowText(Buff);
	theApp.m_Tank.nZStep = MinMax(_ttoi(Buff)-1,0,31);

	m_editXHoldingFixed.GetWindowText(Buff);
	theApp.m_Tank.nRStep = MinMax(_ttoi(Buff)-1,0,31);
	m_editYHoldingFixed.GetWindowText(Buff);
	theApp.m_Tank.nXtStep = MinMax(_ttoi(Buff)-1,0,31);
	m_editZHoldingFixed.GetWindowText(Buff);
	theApp.m_Tank.nYtStep = MinMax(_ttoi(Buff)-1,0,31);


	if (theApp.m_Tank.bHoldingFixture == TRUE) {
		theApp.m_Axes[theApp.m_Tank.nXStep].nMotorType = m_comboXHoldingMove.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYStep].nMotorType = m_comboYHoldingMove.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nZStep].nMotorType = m_comboZHoldingMove.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nRStep].nMotorType = m_comboXHoldingFixed.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nXtStep].nMotorType = m_comboYHoldingFixed.GetCurSel();
		theApp.m_Axes[theApp.m_Tank.nYtStep].nMotorType = m_comboZHoldingFixed.GetCurSel();
	};

	theApp.m_Axes[0].bAvailable = m_checkAvailable0.GetCheck() & 1;
	theApp.m_Axes[1].bAvailable = m_checkAvailable1.GetCheck() & 1;
	theApp.m_Axes[2].bAvailable = m_checkAvailable2.GetCheck() & 1;
	theApp.m_Axes[3].bAvailable = m_checkAvailable3.GetCheck() & 1;
	theApp.m_Axes[4].bAvailable = m_checkAvailable4.GetCheck() & 1;
	theApp.m_Axes[5].bAvailable = m_checkAvailable5.GetCheck() & 1;
	theApp.m_Axes[6].bAvailable = m_checkAvailable6.GetCheck() & 1;
	theApp.m_Axes[7].bAvailable = m_checkAvailable7.GetCheck() & 1;
	theApp.m_Axes[8].bAvailable = m_checkAvailable8.GetCheck() & 1;
	theApp.m_Axes[9].bAvailable = m_checkAvailable9.GetCheck() & 1;
	theApp.m_Axes[10].bAvailable = m_checkAvailable10.GetCheck() & 1;
	theApp.m_Axes[11].bAvailable = m_checkAvailable11.GetCheck() & 1;
	theApp.m_Axes[12].bAvailable = m_checkAvailable12.GetCheck() & 1;
	theApp.m_Axes[13].bAvailable = m_checkAvailable13.GetCheck() & 1;
	theApp.m_Axes[14].bAvailable = m_checkAvailable14.GetCheck() & 1;
	theApp.m_Axes[15].bAvailable = m_checkAvailable15.GetCheck() & 1;
	theApp.m_Axes[16].bAvailable = m_checkAvailable16.GetCheck() & 1;
	theApp.m_Axes[17].bAvailable = m_checkAvailable17.GetCheck() & 1;
	theApp.m_Axes[18].bAvailable = m_checkAvailable18.GetCheck() & 1;
	theApp.m_Axes[19].bAvailable = m_checkAvailable19.GetCheck() & 1;
	theApp.m_Axes[20].bAvailable = m_checkAvailable20.GetCheck() & 1;
	theApp.m_Axes[21].bAvailable = m_checkAvailable21.GetCheck() & 1;
	theApp.m_Axes[22].bAvailable = m_checkAvailable22.GetCheck() & 1;
	theApp.m_Axes[23].bAvailable = m_checkAvailable23.GetCheck() & 1;




	m_editLiftUp.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nCommandLiftUp);
	m_editLiftDown.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nCommandLiftDown);

	m_editLiftUpIn.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nLiftUpLimitSwitch);
	m_editLiftDownIn.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nLiftDownLimitSwitch);


	m_editAndonOkay.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nAndonOkay);
	m_editAndonRunning.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nAndonCycleRunning);
	m_editAndonWarning.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nAndonWarning);
	m_editAndonFault.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nAndonFault);

	theApp.m_Tank.nAndonOutputPort = m_comboAndonOutputPort.GetCurSel();

	AndonEventType Temp = theApp.m_nOldAndonAction;
	theApp.m_nOldAndonAction=Undefined;
	theApp.AndonEvent(Temp,L"Testing");

	m_editAudioBit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nAudioBit);

	m_editPump0Bit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nPumpBit[0]);
	m_editPump1Bit.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Tank.nPumpBit[1]);


	theApp.SaveTank();
	
}

void CAssignAxesDlg::OnSelchangeComboAudioPort() 
{
	theApp.m_Tank.nAudioPort = m_comboAudioPort.GetCurSel();
}

void CAssignAxesDlg::OnCheckChangeListAudio()
{
	theApp.m_Tank.nAudioMaskSelect = 0;
	if(m_listAudio.GetCheck(0)) theApp.m_Tank.nAudioMaskSelect |= 1;
	if(m_listAudio.GetCheck(1)) theApp.m_Tank.nAudioMaskSelect |= 2;
}
