// DynamicSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "DynamicSettingDlg.h"
#include "DynamicView.h"
#include "DynamicNCView.h"

// CDynamicSettingDlg dialog

IMPLEMENT_DYNAMIC(CDynamicSettingDlg, CMyDialogBar)

CDynamicSettingDlg::CDynamicSettingDlg(CWnd* pParent /*=NULL*/) : CMyDialogBar()
{
}

CDynamicSettingDlg::~CDynamicSettingDlg()
{

}

void CDynamicSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogBar::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CYCLE_QUANTITY_1, m_csCycleQuantity[0]);
	DDX_Text(pDX, IDC_EDIT_CYCLE_QUANTITY_2, m_csCycleQuantity[1]);
	DDX_Text(pDX, IDC_EDIT_POINT_QUANTITY_1, m_csPointQuantity[0]);
	DDX_Text(pDX, IDC_EDIT_POINT_QUANTITY_2, m_csPointQuantity[1]);
	DDX_Text(pDX, IDC_EDIT_MINIMUM_ANGLE_1, m_csMinimumAngle[0]);
	DDX_Text(pDX, IDC_EDIT_MINIMUM_ANGLE_2, m_csMinimumAngle[1]);
	DDX_Text(pDX, IDC_EDIT_STEP_ANGLE_1, m_csStepAngle[0]);
	DDX_Text(pDX, IDC_EDIT_STEP_ANGLE_2, m_csStepAngle[1]);
	DDX_Text(pDX, IDC_EDIT_ENCODER_AXIS_LENGTH, m_csEncoderAxisLength);
	DDX_Text(pDX, IDC_EDIT_ENCODER_AXIS_STEP, m_csEncoderAxisStep);
	DDX_Text(pDX, IDC_EDIT_PIECE_THICKNESS, m_csPieceThickness);
	DDX_Text(pDX, IDC_EDIT_VELOCITY, m_csVelocity);
	DDX_Text(pDX, IDC_EDIT_SAMPLING_PERIOD, m_csSamplingPeriod);
	DDX_Radio(pDX, IDC_RADIO_PROBE_1, m_iProbe);
}

BEGIN_MESSAGE_MAP(CDynamicSettingDlg, CMyDialogBar)
	ON_BN_CLICKED(IDC_RADIO_PROBE_1, &CDynamicSettingDlg::OnBnClickedRadioProbe)
	ON_BN_CLICKED(IDC_RADIO_PROBE_2, &CDynamicSettingDlg::OnBnClickedRadioProbe)
	ON_BN_CLICKED(IDC_RADIO_PROBE_3, &CDynamicSettingDlg::OnBnClickedRadioProbe)
	ON_COMMAND(IDC_BUTTON_APPLY_SETTING, &CDynamicSettingDlg::OnBnClickedButtonApply)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_APPLY_SETTING, &CDynamicSettingDlg::OnBnClickedButtonApplyUpdate)
END_MESSAGE_MAP()

// CDynamicSettingDlg message handlers

BOOL CDynamicSettingDlg::OnInitDialogBar()
{
	CMyDialogBar::OnInitDialogBar();

	m_csCycleQuantity[0] = _T("50");
	m_csCycleQuantity[1] = _T("50");
	m_csPointQuantity[0] = _T("500");
	m_csPointQuantity[1] = _T("500");
	m_iProbe = 0;
	m_csMinimumAngle[0] = _T("20.0");
	m_csMinimumAngle[1] = _T("-70.0");
	m_csStepAngle[0] = _T("1.0");
	m_csStepAngle[1] = _T("1.0");
	m_csEncoderAxisLength = _T("0.01");
	m_csEncoderAxisStep = _T("0.001");
	m_csPieceThickness = _T("0.100");
	m_csVelocity = _T("5900.0");
	m_csSamplingPeriod = _T("0.000000020");

	GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_POINT_QUANTITY_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_STEP_ANGLE_1)->EnableWindow(TRUE);

	UpdateData(FALSE);

	return TRUE;
}

void CDynamicSettingDlg::OnBnClickedButtonApply()
{
	UpdateData();

	EnterCriticalSection(&CRT_UPDATE_SETTING);

	g_dVelocity = _wtof(m_csVelocity);
	g_dSamplingPeriod = _wtof(m_csSamplingPeriod);
	g_dEncoderAxisStep = _wtof(m_csEncoderAxisStep);
	
	g_uiCycleQuantity[0] = _wtoi(m_csCycleQuantity[0]);
	g_uiCycleQuantity[1] = _wtoi(m_csCycleQuantity[1]);

	g_uiPointQuantity[0] = _wtoi(m_csPointQuantity[0]);
	g_uiPointQuantity[1] = _wtoi(m_csPointQuantity[1]);

	g_dMinimumAngle[0] = _wtof(m_csMinimumAngle[0]) * PI / 180.0;
	g_dMinimumAngle[1] = _wtof(m_csMinimumAngle[1]) * PI / 180.0;

	g_dStepAngle[0] = _wtof(m_csStepAngle[0]) * PI / 180.0;
	g_dStepAngle[1] = _wtof(m_csStepAngle[1]) * PI / 180.0;

	DeleteAscanMemory(0);
	DeleteAscanMemory(1);
	AllocateAscanMemory(0);
	AllocateAscanMemory(1);

	if (g_pDynamicView)
		g_pDynamicView->m_bResetConfiguration = true;

	LeaveCriticalSection(&CRT_UPDATE_SETTING);
}

void CDynamicSettingDlg::OnBnClickedRadioProbe()
{
	UpdateData();

	if (m_iProbe == 0)
	{
		GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT_QUANTITY_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_ANGLE_1)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POINT_QUANTITY_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_ANGLE_2)->EnableWindow(FALSE);
	}
	else if (m_iProbe == 1)
	{
		GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POINT_QUANTITY_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP_ANGLE_1)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT_QUANTITY_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_ANGLE_2)->EnableWindow(TRUE);
	}
	else if (m_iProbe == 2)
	{
		GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT_QUANTITY_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_ANGLE_1)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_CYCLE_QUANTITY_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POINT_QUANTITY_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MINIMUM_ANGLE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP_ANGLE_2)->EnableWindow(TRUE);
	}
}

void CDynamicSettingDlg::OnBnClickedButtonApplyUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}