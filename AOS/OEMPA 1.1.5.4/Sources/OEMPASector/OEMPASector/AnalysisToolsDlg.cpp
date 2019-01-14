// AnalysisToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UTKernelLayoutFile.h"
#include "SectorApp.h"
#include "AnalysisToolsDlg.h"
#include "afxdialogex.h"
#include "BscopeView.h"
#include "CscopeView.h"
#include "DscopeView.h"
#include "DscopeCorrectedView.h"

bool CAnalysisToolsDlg::m_bCursor=false;
double CAnalysisToolsDlg::m_dX=0.0;
double CAnalysisToolsDlg::m_dZ=0.0;
int CAnalysisToolsDlg::m_iComboCycle=0;
int GetAnalysisCycle()
{
	return CAnalysisToolsDlg::m_iComboCycle;
}

// CAnalysisToolsDlg dialog

IMPLEMENT_DYNAMIC(CAnalysisToolsDlg, CDialog)

CAnalysisToolsDlg::CAnalysisToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalysisToolsDlg::IDD, pParent)
	, m_csYPos(_T("0.0000"))
	, m_csXMin(_T("0.0000"))
	, m_csXMax(_T("0.0000"))
	, m_csYMin(_T("0.0000"))
	, m_csYMax(_T("0.0000"))
	, m_csZMin(_T("0.0000"))
	, m_csZMax(_T("0.0000"))
	, m_iProbe(0)
	, m_iMode(0)
	, m_bCheckBackwallRebound(TRUE)
{
	m_bProbeHasChanged = false;
	m_bModeHasChanged = false;
	m_bSlicePositionHasChanged = false;
	m_bGateSettingHasChanged = false;
	m_iIndexLayoutFile = -1;//integration
}

CAnalysisToolsDlg::~CAnalysisToolsDlg()
{
}

void CAnalysisToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_Y_POS, m_csYPos);
	DDX_Text(pDX, IDC_EDIT_X_MIN, m_csXMin);
	DDX_Text(pDX, IDC_EDIT_X_MAX, m_csXMax);
	DDX_Text(pDX, IDC_EDIT_Y_MIN, m_csYMin);
	DDX_Text(pDX, IDC_EDIT_Y_MAX, m_csYMax);
	DDX_Text(pDX, IDC_EDIT_Z_MIN, m_csZMin);
	DDX_Text(pDX, IDC_EDIT_Z_MAX, m_csZMax);
	DDX_Radio(pDX, IDC_RADIO_PROBE_1, m_iProbe);
	DDX_Radio(pDX, IDC_RADIO_SLICE, m_iMode);
	DDX_Control(pDX, IDC_COMBO_CYCLE, m_ComboCycle);
	DDX_CBIndex(pDX, IDC_COMBO_CYCLE, m_iComboCycle);
	DDX_Check(pDX, IDC_CHECK_BACKWALL_REBOUND, m_bCheckBackwallRebound);
	DDX_Control(pDX, IDC_EDIT_CURSOR_X, m_EditCursorX);
	DDX_Control(pDX, IDC_EDIT_CURSOR_Z, m_EditCursorZ);
	DDX_Control(pDX, IDC_STATIC_CURSOR_X, m_StaticCursorX);
	DDX_Control(pDX, IDC_STATIC_CURSOR_Z, m_StaticCursorZ);
	DDX_Control(pDX, IDC_STATIC_CURSOR_UNIT_X, m_StaticCursorUnitX);
	DDX_Control(pDX, IDC_STATIC_CURSOR_UNIT_Z, m_StaticCursorUnitZ);
}

BEGIN_MESSAGE_MAP(CAnalysisToolsDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_POS, &CAnalysisToolsDlg::OnDeltaposSpinYPos)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_MIN, &CAnalysisToolsDlg::OnDeltaposSpinXMin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_MAX, &CAnalysisToolsDlg::OnDeltaposSpinXMax)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_MIN, &CAnalysisToolsDlg::OnDeltaposSpinYMin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_MAX, &CAnalysisToolsDlg::OnDeltaposSpinYMax)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z_MIN, &CAnalysisToolsDlg::OnDeltaposSpinZMin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z_MAX, &CAnalysisToolsDlg::OnDeltaposSpinZMax)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_POS, &CAnalysisToolsDlg::OnEnKillfocusEditYPos)
	ON_EN_KILLFOCUS(IDC_EDIT_X_MIN, &CAnalysisToolsDlg::OnEnKillfocusEditXMin)
	ON_EN_KILLFOCUS(IDC_EDIT_X_MAX, &CAnalysisToolsDlg::OnEnKillfocusEditXMax)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_MIN, &CAnalysisToolsDlg::OnEnKillfocusEditYMin)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_MAX, &CAnalysisToolsDlg::OnEnKillfocusEditYMax)
	ON_EN_KILLFOCUS(IDC_EDIT_Z_MIN, &CAnalysisToolsDlg::OnEnKillfocusEditZMin)
	ON_EN_KILLFOCUS(IDC_EDIT_Z_MAX, &CAnalysisToolsDlg::OnEnKillfocusEditZMax)
	ON_BN_CLICKED(IDC_RADIO_PROBE_1, &CAnalysisToolsDlg::OnBnClickedRadioProbe)
	ON_BN_CLICKED(IDC_RADIO_PROBE_2, &CAnalysisToolsDlg::OnBnClickedRadioProbe)
	ON_BN_CLICKED(IDC_RADIO_BOTH_PROBE, &CAnalysisToolsDlg::OnBnClickedRadioProbe)
	ON_BN_CLICKED(IDC_RADIO_SLICE, &CAnalysisToolsDlg::OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_GATE, &CAnalysisToolsDlg::OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_BUTTON_RESET_X_MIN, &CAnalysisToolsDlg::OnBnClickedButtonResetXMin)
	ON_BN_CLICKED(IDC_BUTTON_RESET_X_MAX, &CAnalysisToolsDlg::OnBnClickedButtonResetXMax)
	ON_BN_CLICKED(IDC_BUTTON_RESET_Y_MIN, &CAnalysisToolsDlg::OnBnClickedButtonResetYMin)
	ON_BN_CLICKED(IDC_BUTTON_RESET_Y_MAX, &CAnalysisToolsDlg::OnBnClickedButtonResetYMax)
	ON_BN_CLICKED(IDC_BUTTON_RESET_Z_MIN, &CAnalysisToolsDlg::OnBnClickedButtonResetZMin)
	ON_BN_CLICKED(IDC_BUTTON_RESET_Z_MAX, &CAnalysisToolsDlg::OnBnClickedButtonResetZMax)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_CBN_SELCHANGE(IDC_COMBO_CYCLE, &CAnalysisToolsDlg::OnCbnSelchangeComboCycle)
	ON_BN_CLICKED(IDC_CHECK_BACKWALL_REBOUND, &CAnalysisToolsDlg::OnBnClickedCheckBackwallRebound)
END_MESSAGE_MAP()

// CAnalysisToolsDlg message handlers

void CAnalysisToolsDlg::OnDeltaposSpinYPos(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csYPos) - dir * SystemAnalysis.dEncoderStep;

	if (dValue < Analysis_Y_Axis_MIN)
		dValue = Analysis_Y_Axis_MIN;

	if (dValue > Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep)
		dValue = Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep;

	m_csYPos.Format(_T("%.4f"), dValue);
	Analysis_YPOS = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bSlicePositionHasChanged = true;
}

void CAnalysisToolsDlg::OnDeltaposSpinXMin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csXMin) - (double)dir / 1000.0;

	if (dValue < Analysis_X_Axis_MIN)
		dValue = Analysis_X_Axis_MIN;

	if (dValue >= Analysis_XMAX)
		dValue = Analysis_XMAX - 0.001;

	m_csXMin.Format(_T("%.4f"), dValue);
	Analysis_XMIN = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnDeltaposSpinXMax(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csXMax) - (double)dir / 1000.0;

	if (dValue > Analysis_X_Axis_MAX)
		dValue = Analysis_X_Axis_MAX;

	if (dValue <= Analysis_XMIN)
		dValue = Analysis_XMIN + 0.001;

	m_csXMax.Format(_T("%.4f"), dValue);
	Analysis_XMAX = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnDeltaposSpinYMin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csYMin) - (double)dir * SystemAnalysis.dEncoderStep;

	if (dValue < Analysis_Y_Axis_MIN)
		dValue = Analysis_Y_Axis_MIN;

	if (dValue >= Analysis_YMAX)
		dValue = Analysis_YMAX - SystemAnalysis.dEncoderStep;

	m_csYMin.Format(_T("%.4f"), dValue);
	Analysis_YMIN = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnDeltaposSpinYMax(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csYMax) - (double)dir * SystemAnalysis.dEncoderStep;

	if (dValue > Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep)
		dValue = Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep;

	if (dValue <= Analysis_YMIN)
		dValue = Analysis_YMIN + SystemAnalysis.dEncoderStep;

	m_csYMax.Format(_T("%.4f"), dValue);
	Analysis_YMAX = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnDeltaposSpinZMin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csZMin) - (double)dir / 1000.0;

	if (dValue < Analysis_Z_Axis_MIN)
		dValue = Analysis_Z_Axis_MIN;

	if (dValue >= Analysis_ZMAX)
		dValue = Analysis_ZMAX - 0.001;

	m_csZMin.Format(_T("%.4f"), dValue);
	Analysis_ZMIN = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnDeltaposSpinZMax(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int dir = pNMUpDown->iDelta;
	UpdateData();
	double dValue = _wtof(m_csZMax) - (double)dir / 1000.0;

	if (dValue > Analysis_Z_Axis_MAX)
		dValue = Analysis_Z_Axis_MAX;

	if (dValue <= Analysis_ZMIN)
		dValue = Analysis_ZMIN + 0.001;

	m_csZMax.Format(_T("%.4f"), dValue);
	Analysis_ZMAX = dValue;
	UpdateData(FALSE);
	*pResult = 0;

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditYPos()
{
	UpdateData();
	double dValue = _wtof(m_csYPos);

	if (dValue < Analysis_Y_Axis_MIN)
		dValue = Analysis_Y_Axis_MIN;

	if (dValue > Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep)
		dValue = Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep;

	m_csYPos.Format(_T("%.4f"), dValue);
	Analysis_YPOS = dValue;
	UpdateData(FALSE);

	m_bSlicePositionHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditXMin()
{
	UpdateData();
	double dValue = _wtof(m_csXMin);

	if (dValue < Analysis_X_Axis_MIN)
		dValue = Analysis_X_Axis_MIN;

	if (dValue >= Analysis_XMAX)
		dValue = Analysis_XMAX - 0.001;

	m_csXMin.Format(_T("%.4f"), dValue);
	Analysis_XMIN = dValue;
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditXMax()
{
	UpdateData();
	double dValue = _wtof(m_csXMax);

	if (dValue > Analysis_X_Axis_MAX)
		dValue = Analysis_X_Axis_MAX;

	if (dValue <= Analysis_XMIN)
		dValue = Analysis_XMIN + 0.001;

	m_csXMax.Format(_T("%.4f"), dValue);
	Analysis_XMAX = dValue;
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditYMin()
{
	UpdateData();
	double dValue = _wtof(m_csYMin);

	if (dValue < Analysis_Y_Axis_MIN)
		dValue = Analysis_Y_Axis_MIN;

	if (dValue >= Analysis_YMAX)
		dValue = Analysis_YMAX - SystemAnalysis.dEncoderStep;

	m_csYMin.Format(_T("%.4f"), dValue);
	Analysis_YMIN = dValue;
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditYMax()
{
	UpdateData();
	double dValue = _wtof(m_csYMax);

	if (dValue > Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep)
		dValue = Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep;

	if (dValue <= Analysis_YMIN)
		dValue = Analysis_YMIN + SystemAnalysis.dEncoderStep;

	m_csYMax.Format(_T("%.4f"), dValue);
	Analysis_YMAX = dValue;
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditZMin()
{
	UpdateData();
	double dValue = _wtof(m_csZMin);

	if (dValue < Analysis_Z_Axis_MIN)
		dValue = Analysis_Z_Axis_MIN;

	if (dValue >= Analysis_ZMAX)
		dValue = Analysis_ZMAX - 0.001;

	m_csZMin.Format(_T("%.4f"), dValue);
	Analysis_ZMIN = dValue;
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnEnKillfocusEditZMax()
{
	UpdateData();
	double dValue = _wtof(m_csZMax);

	if (dValue > Analysis_Z_Axis_MAX)
		dValue = Analysis_Z_Axis_MAX;

	if (dValue <= Analysis_ZMIN)
		dValue = Analysis_ZMIN + 0.001;

	m_csZMax.Format(_T("%.4f"), dValue);
	Analysis_ZMAX = dValue;
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedRadioProbe()
{
	UpdateData();
	g_uiProbeAnalysis = m_iProbe;
	::UpdateAxisLimitation(1);
	this->UpdateLimitation();

	m_bProbeHasChanged = true;
	OnBnClickedRadioMode();
}

void CAnalysisToolsDlg::OnBnClickedRadioMode()
{
	UpdateData();

	if (m_iMode == 0)
	{
		GetDlgItem(IDC_EDIT_Y_POS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_Y_POS)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_X_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_X_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_X_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_X_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_Y_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_Y_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_Z_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_Z_MAX)->EnableWindow(FALSE);
		m_ComboCycle.EnableWindow(FALSE);
	}
	else if (m_iMode == 1)
	{
		GetDlgItem(IDC_EDIT_Y_POS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_Y_POS)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_X_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_X_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_X_MAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_X_MAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_Y_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y_MAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_Y_MAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_Z_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z_MAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_Z_MAX)->EnableWindow(TRUE);
		m_ComboCycle.EnableWindow(TRUE);
	}

	g_uiMode = m_iMode;
	m_bModeHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedButtonResetXMin()
{
	Analysis_XMIN = Analysis_X_Axis_MIN;
	m_csXMin.Format(_T("%.4f"), Analysis_XMIN);
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedButtonResetXMax()
{
	Analysis_XMAX = Analysis_X_Axis_MAX;
	m_csXMax.Format(_T("%.4f"), Analysis_XMAX);
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedButtonResetYMin()
{
	Analysis_YMIN = Analysis_Y_Axis_MIN;
	m_csYMin.Format(_T("%.4f"), Analysis_YMIN);
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedButtonResetYMax()
{
	Analysis_YMAX = Analysis_Y_Axis_MAX - SystemAnalysis.dEncoderStep;
	m_csYMax.Format(_T("%.4f"), Analysis_XMAX);
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedButtonResetZMin()
{
	Analysis_ZMIN = Analysis_Z_Axis_MIN;
	m_csZMin.Format(_T("%.4f"), Analysis_ZMIN);
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedButtonResetZMax()
{
	Analysis_ZMAX = Analysis_Z_Axis_MAX;
	m_csZMax.Format(_T("%.4f"), Analysis_ZMAX);
	UpdateData(FALSE);

	m_bGateSettingHasChanged = true;
}

BOOL CAnalysisToolsDlg::OnInitDialog()
{
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;
	CString string;

	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_Y_POS)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN_Y_POS)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_X_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_X_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_X_MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_X_MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Y_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_Y_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Y_MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_Y_MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_Z_MIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z_MAX)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_Z_MAX)->EnableWindow(FALSE);

	SetTimer(TIMER_UPDATE_ANALYSIS, 33, NULL);

	//integration
	if(UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
	{
		if(UTKernel_LayoutFile()->RegisterWindow(pTitle,m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
		{
			if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,pTitle,sysTime,iResHorz,iResVert,wndPlacement,iData))
			if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}else
			UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,pTitle,iResHorz,iResVert,wndPlacement,0);
	}
	m_ComboCycle.ResetContent();
	for(int iCycle=0;iCycle<InfoProbe1Analysis.iCycleCount;iCycle++)
	{
		string.Format(L"%d",iCycle);
		m_ComboCycle.AddString(string);
	}
	UpdateData(FALSE);
	OnBnClickedRadioMode();

	return TRUE;
}

void CAnalysisToolsDlg::UpdateLimitation()
{
	CString string;

	m_iProbe = g_uiProbeAnalysis;
	m_iMode = g_uiMode;

	m_csYPos.Format(_T("%.4f"), Analysis_YPOS);
	m_csXMin.Format(_T("%.4f"), Analysis_XMIN);
	m_csXMax.Format(_T("%.4f"), Analysis_XMAX);
	m_csYMin.Format(_T("%.4f"), Analysis_YMIN);
	m_csYMax.Format(_T("%.4f"), Analysis_YMAX);
	m_csZMin.Format(_T("%.4f"), Analysis_ZMIN);
	m_csZMax.Format(_T("%.4f"), Analysis_ZMAX);

	m_ComboCycle.ResetContent();
	for(int iCycle=0;iCycle<InfoProbe1Analysis.iCycleCount;iCycle++)
	{
		string.Format(L"%d",iCycle);
		m_ComboCycle.AddString(string);
	}
	m_iComboCycle = 0;

	UpdateData(FALSE);
}

void CAnalysisToolsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_UPDATE_ANALYSIS)
	{
		if (m_bProbeHasChanged == true)
		{
			if (g_pBscope)
				g_pBscope->SetEvent(2);

			if (g_pCscope)
				g_pCscope->SetEvent(2);

			if (g_pDscope)
				g_pDscope->SetEvent(2);

			if (g_pDscopeCorrected)
				g_pDscopeCorrected->SetEvent(2);

			m_bProbeHasChanged = false;
		}
		else if (m_bModeHasChanged == true)
		{
			if (g_pBscope)
				g_pBscope->SetEvent(3);

			if (g_pCscope)
				g_pCscope->SetEvent(3);

			if (g_pDscope)
				g_pDscope->SetEvent(3);

			if (g_pDscopeCorrected)
				g_pDscopeCorrected->SetEvent(3);

			m_bModeHasChanged = false;
		}
		else if (m_bSlicePositionHasChanged == true)
		{
			if (g_pBscope)
				g_pBscope->SetEvent(6);

			if (g_pCscope)
				g_pCscope->SetEvent(6);

			if (g_pDscope)
				g_pDscope->SetEvent(6);

			if (g_pDscopeCorrected)
				g_pDscopeCorrected->SetEvent(6);

			m_bSlicePositionHasChanged = false;
		}
		else if (m_bGateSettingHasChanged == true)
		{
			if (g_pBscope)
				g_pBscope->SetEvent(4);

			if (g_pCscope)
				g_pCscope->SetEvent(4);

			if (g_pDscope)
				g_pDscope->SetEvent(4);

			if (g_pDscopeCorrected)
				g_pDscopeCorrected->SetEvent(4);

			m_bGateSettingHasChanged = false;
		}
		if(m_bCursor)
		{
			CString string;
			string.Format(L"%.2lf",m_dX*1000.0);
			m_EditCursorX.SetWindowText(string);
			string.Format(L"%.2lf",m_dZ*1000.0);
			m_EditCursorZ.SetWindowText(string);
			m_EditCursorX.ShowWindow(SW_SHOW);
			m_EditCursorZ.ShowWindow(SW_SHOW);
			m_StaticCursorX.ShowWindow(SW_SHOW);
			m_StaticCursorZ.ShowWindow(SW_SHOW);
			m_StaticCursorUnitX.ShowWindow(SW_SHOW);
			m_StaticCursorUnitZ.ShowWindow(SW_SHOW);
		}else{
			m_EditCursorX.ShowWindow(SW_HIDE);
			m_EditCursorZ.ShowWindow(SW_HIDE);
			m_StaticCursorX.ShowWindow(SW_HIDE);
			m_StaticCursorZ.ShowWindow(SW_HIDE);
			m_StaticCursorUnitX.ShowWindow(SW_HIDE);
			m_StaticCursorUnitZ.ShowWindow(SW_HIDE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CAnalysisToolsDlg::OnClose()//integration
{
	// TODO: Add your message handler code here and/or call default

	//CDialog::OnClose();
	ShowWindow(SW_HIDE);
}

void CAnalysisToolsDlg::OnMove(int x, int y)//integration
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];

	CDialog::OnMove(x, y);

	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,pTitle,iResHorz,iResVert,wndPlacement,0);
}

void CAnalysisToolsDlg::OnCbnSelchangeComboCycle()
{
	UpdateData();
	m_bModeHasChanged = true;
}

void CAnalysisToolsDlg::OnBnClickedCheckBackwallRebound()
{
	UpdateData();
	if(m_bCheckBackwallRebound)
		g_bBackwallRebound = true;
	else
		g_bBackwallRebound = false;
	m_bModeHasChanged = true;
}
