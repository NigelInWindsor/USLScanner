// DlgCalibration.cpp : implementation file
//

#include "stdafx.h"
//#include "OEMPATool.h"
#include "DlgCalibration.h"
#include "afxdialogex.h"

void EditFile(wchar_t *pFile,bool bCloseWaiting);
float CDlgCalibration::m_fCalibrationOffset=-1.0f;
bool CDlgCalibration::m_bCalibrationOffset=false;
CString CDlgCalibration::m_strStaticAlignment=L"";
CString CDlgCalibration::m_strStaticOffset=L"";

// CDlgCalibration dialog

IMPLEMENT_DYNAMIC(CDlgCalibration, CDialog)

CDlgCalibration::CDlgCalibration(CHWDeviceOEMPA *pHWDeviceOEMPA,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibration::IDD, pParent)
	, m_bCheckEnable(false)
	, m_bCheckOffset(FALSE)
{
	m_pHWDeviceOEMPA = pHWDeviceOEMPA;
}

CDlgCalibration::~CDlgCalibration()
{
}

void CDlgCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CALIBRATION_RESET, m_ButtonReset);
	DDX_Control(pDX, IDC_CHECK_CALIBRATION_ENABLE, m_CheckEnable);
	DDX_Check(pDX, IDC_CHECK_CALIBRATION_ENABLE, m_bCheckEnable);
	DDX_Control(pDX, IDC_CHECK_CALIBRATION_OFFSET, m_CheckOffset);
	DDX_Check(pDX, IDC_CHECK_CALIBRATION_OFFSET, m_bCheckOffset);
	DDX_Text(pDX, IDC_STATIC_ALIGNMENT, m_strStaticAlignment);
	DDX_Text(pDX, IDC_STATIC_OFFSET, m_strStaticOffset);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_ButtonUpdate);
}


BEGIN_MESSAGE_MAP(CDlgCalibration, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalibration::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_RESET, &CDlgCalibration::OnBnClickedButtonCalibrationReset)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATION_ENABLE, &CDlgCalibration::OnBnClickedCheckCalibrationEnable)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_PERFORM, &CDlgCalibration::OnBnClickedButtonCalibrationPerform)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATION_OFFSET, &CDlgCalibration::OnBnClickedCheckCalibrationOffset)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDlgCalibration::OnBnClickedButtonUpdate)
END_MESSAGE_MAP()


// CDlgCalibration message handlers


void CDlgCalibration::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CDlgCalibration::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_bCalibrationOffset && (m_fCalibrationOffset>=0.0f))
		m_bCheckOffset = TRUE;
	else
		m_bCheckOffset = FALSE;
	UpdateDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgCalibration::UpdateDialog()
{
	bool bDone=m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsAlignmentPerformed();
	bool bEnable=m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsAlignmentEnabled();

	m_ButtonUpdate.EnableWindow(bDone);
	m_ButtonReset.EnableWindow(bDone);
	m_CheckEnable.EnableWindow(bDone);
	if(bDone)
		m_bCheckEnable = bEnable?TRUE:FALSE;
	else
		m_bCheckEnable = FALSE;
	m_CheckOffset.EnableWindow(bDone && (m_fCalibrationOffset>=0.0f?true:false));
	m_CheckOffset.ShowWindow(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsTimeOffsetSupported()?true:false);
	UpdateData(FALSE);
}

void CDlgCalibration::OnBnClickedButtonCalibrationReset()
{
	UpdateData();
	if(AfxMessageBox(L"Are you sure ?",MB_OKCANCEL)==IDCANCEL)
		return;
	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->ResetAlignment();
	m_bCheckOffset = FALSE;
	UpdateData(FALSE);
	OnBnClickedCheckCalibrationOffset();
	m_strStaticAlignment = L"";
	m_strStaticOffset = L"";
	UpdateDialog();
}

void CDlgCalibration::OnBnClickedCheckCalibrationEnable()
{
	UpdateData();
	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableAlignment(m_bCheckEnable?true:false);
	UpdateDialog();
}

void CDlgCalibration::OnBnClickedCheckCalibrationOffset()
{
	bool bRet=true;
	float fCalibrationOffset=0.0f;

	UpdateData();
	bRet = true;
	if(m_pHWDeviceOEMPA->LockDevice())
	{
		if(m_bCheckOffset)
		{
			m_bCalibrationOffset = true;
			if(!m_pHWDeviceOEMPA->SetTimeOffset(m_fCalibrationOffset))
				bRet = false;
		}else{
			m_bCalibrationOffset = false;
			if(!m_pHWDeviceOEMPA->SetTimeOffset(fCalibrationOffset))
				bRet = false;
		}
		if(!m_pHWDeviceOEMPA->UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"HW Error");
}

void CDlgCalibration::OnBnClickedButtonUpdate()
{
	bool bRet=true;
	CWaitCursor wait;

	UpdateData();
	if(!m_pHWDeviceOEMPA->UpdateCalibration())
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"HW Error");
}

void CDlgCalibration::OnBnClickedButtonCalibrationPerform()
{
	wchar_t wcFileReport[MAX_PATH];
	DWORD dwStart,dwStop;
	CString string;
	bool bReportFile;
	float FCorrectionMax;

	CSWDeviceOEMPA::AlignmentCfgUpdated();
	UpdateData();
	if(AfxMessageBox(L"Please remove the probe!",MB_OKCANCEL)==IDCANCEL)
		return;
	CWaitCursor wait;
	bReportFile = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetCalibrationFileReport(MAX_PATH,wcFileReport);
	dwStart = GetTickCount();
	if(!m_pHWDeviceOEMPA->PerformCalibration(FCorrectionMax,m_fCalibrationOffset))
	{
		m_fCalibrationOffset = -1.0;
		if(bReportFile)
			EditFile(wcFileReport,false);
		else
			AfxMessageBox(L"Error!");
		return;
	}
	dwStop = GetTickCount();
	string.Format(L"AlignmentDelayMax = %.0f ns\r\nCorrectionOffset = %.0f ns\r\nCalibrationTime = %d ms",FCorrectionMax*1e9f,m_fCalibrationOffset*1e9f,(int)(dwStop-dwStart));
	m_strStaticAlignment.Format(L"(%.0f ns)",FCorrectionMax*1e9f);
	m_strStaticOffset.Format(L"(%.0f ns)",m_fCalibrationOffset*1e9f);
	AfxMessageBox(string);
	UpdateDialog();
}
