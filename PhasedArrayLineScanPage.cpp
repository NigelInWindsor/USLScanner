#include "stdafx.h"
#include "USLScanner.h"
#include "PhasedArrayLineScanPage.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CPhasedArrayLineScanPage, CPropertyPage)

CPhasedArrayLineScanPage::CPhasedArrayLineScanPage() : CPropertyPage(CPhasedArrayLineScanPage::IDD)
{
	m_nLineNumber = 0;

}

CPhasedArrayLineScanPage::~CPhasedArrayLineScanPage()
{
}

void CPhasedArrayLineScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContourScanPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_editSpinLineNumber);
	DDX_Control(pDX, IDC_COMBO_SCAN_SIDE, m_comboScanSide);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
}


BEGIN_MESSAGE_MAP(CPhasedArrayLineScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CContourScanPage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, &CPhasedArrayLineScanPage::OnEnChangeEditFilepath)
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, &CPhasedArrayLineScanPage::OnEnChangeEditLineNumber)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LINE_NUMBER, &CPhasedArrayLineScanPage::OnDeltaposSpinLineNumber)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CPhasedArrayLineScanPage::OnBnClickedButtonBrowse)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CPhasedArrayLineScanPage::OnBnClickedButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_SCAN_SIDE, &CPhasedArrayLineScanPage::OnCbnSelchangeComboScanSide)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, &CPhasedArrayLineScanPage::OnEnChangeEditScanSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, &CPhasedArrayLineScanPage::OnDeltaposSpinScanSpeed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CPhasedArrayLineScanPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	UDACCEL	Accel;
	Accel.nInc = 5;
	Accel.nSec = 1;

	m_editSpinLineNumber.Init((float)m_nLineNumber, 1.0f, 1000.0f, 1.0f, 1.0f, L"%.0f");
	switch (theApp.m_nSide0Orientation) {
	case 0:
		m_comboScanSide.AddString(theApp.m_DlgSideName[0]);
		m_comboScanSide.AddString(theApp.m_DlgSideName[1]);
		Buff.LoadString(IDS_Both); m_comboScanSide.AddString(Buff);
		break;
	case 1:
		m_comboScanSide.AddString(theApp.m_DlgSideName[1]);
		m_comboScanSide.AddString(theApp.m_DlgSideName[0]);
		Buff.LoadString(IDS_Both); m_comboScanSide.AddString(Buff);
		break;
	}
	m_spinScanSpeed.SetRange(1, 1000);
	m_spinScanSpeed.SetAccel(1, &Accel);


	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPhasedArrayLineScanPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CPhasedArrayLineScanPage::UpdateAllControls()
{
	CString Buff;

	m_editFilePath.SetWindowText(theApp.m_Data.m_FileName);

	int nNumberLines = PROFILE->GetNumberLines(TAUGHTLINES)+1;
	if (nNumberLines < 1) nNumberLines = 1;
	m_editSpinLineNumber.SetPos((float)nNumberLines);

	m_comboScanSide.SetCurSel(PROFILE->m_nScanSide);

	if (theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;
	Buff.Format(_T("%.*f %s/s"), theApp.m_nUnitsPrecision - 1, PROFILE->m_fScanSpeed*theApp.m_fUnits, theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);


}

BOOL CPhasedArrayLineScanPage::OnSetActive()
{
	SetTimer(1, 500, NULL);
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	return CPropertyPage::OnSetActive();
}


BOOL CPhasedArrayLineScanPage::OnKillActive()
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}


void CPhasedArrayLineScanPage::OnEnChangeEditFilepath()
{
	m_editFilePath.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}


void CPhasedArrayLineScanPage::OnEnChangeEditLineNumber()
{
	int nTemp = (int)m_editSpinLineNumber.GetValue() - 1;

	m_nLineNumber = nTemp;
}


void CPhasedArrayLineScanPage::OnDeltaposSpinLineNumber(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);


	int nTemp = (int)m_editSpinLineNumber.GetValue() - 1;

	m_nLineNumber = nTemp;

	*pResult = 0;
}


void CPhasedArrayLineScanPage::OnBnClickedButtonBrowse()
{
	int nPos;

	CFileDialog File(TRUE, _T(".dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"), NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if (File.DoModal() == IDOK) {
		theApp.m_Data.m_FilePath.Format(_T("%s"), File.GetPathName());
		theApp.m_Data.m_FileName.Format(_T("%s"), File.GetFileTitle());

		if (theApp.m_LastSettings.bUpdateDirectoryPath) {
			if ((nPos = theApp.m_Data.m_FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = theApp.m_Data.m_FilePath.Left(nPos);
			}
		}

		m_editFilePath.SetWindowText(File.GetFileTitle());
	};
}


void CPhasedArrayLineScanPage::OnTimer(UINT_PTR nIDEvent)
{
	int nNumberLines = PROFILE->GetNumberLines(TAUGHTLINES);
	if (nNumberLines < 1) nNumberLines = 1;
	m_editSpinLineNumber.SetRangeAndDelta(1.0f, (float)nNumberLines, 1.0f);

	CPropertyPage::OnTimer(nIDEvent);
}


void CPhasedArrayLineScanPage::OnBnClickedButtonStart()
{
	m_nLineNumber = (int)m_editSpinLineNumber.GetValue() - 1;
	if (m_nLineNumber < 0 || m_nLineNumber >= PROFILE->GetNumberLines(TAUGHTLINES)) {
		MessageBox(L"The line number is incorrect", L"Error", MB_ICONERROR);
		return;
	}

	PROFILE->GenerateSingleLineBScan(m_nLineNumber);

	FRAME;
	CString Buff, Buff1, Error;

	if (PROFILE->m_fScanSpeed < 10.0f) {
		if (MessageBox(L"The scan speed seems very low\r\nDo you wish to continue at this speed?", L"Warning", MB_ICONWARNING | MB_YESNO) == IDNO) {
			return;
		}
	}

	if (pFrame->IsScanImageStillOpen() == FALSE) {
		if (theApp.m_Data.IsNameRequired() == TRUE) {
			Buff.LoadString(IDS_Please_enter_a_scan_name);
			Error.LoadString(IDS_Error);
			MessageBox(Buff, Error, MB_ICONERROR);
			return;
		}
		if ((theApp.m_Data.m_FileName.IsEmpty()) && (theApp.m_LastSettings.nSaveScanEveryLine != 0)) {
			Buff.LoadString(IDS_Save_every_line_is_selected);
			Buff1.LoadString(IDS_please_enter_file_name);
			Error.LoadString(IDS_Error);
			MessageBox(Buff + "\r\n" + Buff1, Error, MB_ICONERROR);
			return;
		}
		theApp.m_nScanWhat = 0;
		PROFILE->setRUsed(false);
		PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;
	}
	PROFILE->m_nScanSide = m_comboScanSide.GetCurSel();
	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide;


	switch (theApp.m_Tank.nScannerDescription) {
	default:
		pFrame->StartScan(CONTOUR_SCAN);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
		if (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY) {
			pFrame->StartScan(ROBOT_ROLLER_PROBE_SCAN);
		}
		else {
			pFrame->StartScan(ROBOT_CONTOUR_SCAN);
		}
		break;
	}

}


void CPhasedArrayLineScanPage::OnCbnSelchangeComboScanSide()
{
	PROFILE->m_nScanSide = m_comboScanSide.GetCurSel();
}


void CPhasedArrayLineScanPage::OnEnChangeEditScanSpeed()
{
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff, fTemp);
	fTemp /= theApp.m_fUnits;
	if (fTemp<1.0f) fTemp = 1.0f;
	if (fTemp>1000.0f) fTemp = 1020.0f;
}


void CPhasedArrayLineScanPage::OnDeltaposSpinScanSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (theApp.m_LastSettings.nMeasurementUnits == 0) {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta);
		if (PROFILE->m_fScanSpeed<1.0f) PROFILE->m_fScanSpeed = 1.0f;
		if (PROFILE->m_fScanSpeed>1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	}
	else {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		if (PROFILE->m_fScanSpeed<25.4f) PROFILE->m_fScanSpeed = 25.4f;
		if (PROFILE->m_fScanSpeed>1016.0f) PROFILE->m_fScanSpeed = 1016.0f;
	}
	UpdateAllControls();

	*pResult = 0;
}
