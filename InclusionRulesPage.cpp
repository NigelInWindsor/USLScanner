// InclusionRulesPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "InclusionRulesPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInclusionRulesPage property page

IMPLEMENT_DYNCREATE(CInclusionRulesPage, CPropertyPage)

CInclusionRulesPage::CInclusionRulesPage(CInclusions* pInclusions, int nGlobalLocal) : CPropertyPage(CInclusionRulesPage::IDD)
{
	//{{AFX_DATA_INIT(CInclusionRulesPage)
	//}}AFX_DATA_INIT
	
	m_pInclusions = pInclusions;
	m_nGlobalLocal = nGlobalLocal;
}

CInclusionRulesPage::~CInclusionRulesPage()
{
}

void CInclusionRulesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInclusionRulesPage)
	DDX_Control(pDX, IDC_EDIT_REF_STANDARD, m_editRefStandard);
	DDX_Control(pDX, IDC_EDIT_TEST_SENSITIVITY2, m_editTestSensitivity);
	DDX_Control(pDX, IDC_EDIT_TEST_GAIN, m_editTestGain);
	DDX_Control(pDX, IDC_EDIT_TEST_DATE, m_editTestDate);
	DDX_Control(pDX, IDC_EDIT_REF_SENSITIVITY, m_editRefSensitivity);
	DDX_Control(pDX, IDC_EDIT_REF_DATE, m_editRefDate);
	DDX_Control(pDX, IDC_EDIT_REF_GAIN, m_editRefGain);
	DDX_Control(pDX, IDC_EDIT_DENSITY, m_editDensity);
	DDX_Control(pDX, IDC_EDIT_SHEARWAVE_VELOCITY, m_editShearWaveVelocity);
	DDX_Control(pDX, IDC_EDIT_MATERIAL_VELOCITY, m_editMaterialVelocity);
	DDX_Control(pDX, IDC_EDIT_FBH_SIZE, m_editFBHSize);
	DDX_Control(pDX, IDC_EDIT_FOCAL_DEPTH, m_editFocalDepth);
	DDX_Control(pDX, IDC_EDIT_FOCAL_LENGTH, m_editFocalLength);
	DDX_Control(pDX, IDC_EDIT_PROBE_ID, m_editProbeID);
	DDX_Control(pDX, IDC_EDIT_WEIGHT_C, m_editWeightC);
	DDX_Control(pDX, IDC_EDIT_WEIGHT_B, m_editWeightB);
	DDX_Control(pDX, IDC_EDIT_WEIGHT_A, m_editWeightA);
	DDX_Control(pDX, IDC_EDIT_GREATER_C, m_editGreaterC);
	DDX_Control(pDX, IDC_EDIT_GREATER_B, m_editGreaterB);
	DDX_Control(pDX, IDC_EDIT_GREATER_A, m_editGreaterA);
	DDX_Control(pDX, IDC_SPIN_Y_SAMPLES, m_spinYSamples);
	DDX_Control(pDX, IDC_SPIN_X_SAMPLES, m_spinXSamples);
	DDX_Control(pDX, IDC_SPIN_THRESHOLD, m_spinThreshold);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD, m_editThreshold);
	DDX_Control(pDX, IDC_EDIT_Y_SAMPLES, m_editYSamples);
	DDX_Control(pDX, IDC_EDIT_X_SAMPLES, m_editXSamples);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInclusionRulesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CInclusionRulesPage)
	ON_EN_CHANGE(IDC_EDIT_X_SAMPLES, OnChangeEditXSamples)
	ON_EN_CHANGE(IDC_EDIT_Y_SAMPLES, OnChangeEditYSamples)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, OnChangeEditThreshold)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_GREATER_A, OnChangeEditGreaterA)
	ON_EN_CHANGE(IDC_EDIT_GREATER_B, OnChangeEditGreaterB)
	ON_EN_CHANGE(IDC_EDIT_GREATER_C, OnChangeEditGreaterC)
	ON_EN_CHANGE(IDC_EDIT_WEIGHT_A, OnChangeEditWeightA)
	ON_EN_CHANGE(IDC_EDIT_WEIGHT_B, OnChangeEditWeightB)
	ON_EN_CHANGE(IDC_EDIT_WEIGHT_C, OnChangeEditWeightC)
	ON_EN_CHANGE(IDC_EDIT_FBH_SIZE, OnChangeEditFbhSize)
	ON_EN_CHANGE(IDC_EDIT_FOCAL_DEPTH, OnChangeEditFocalDepth)
	ON_EN_CHANGE(IDC_EDIT_FOCAL_LENGTH, OnChangeEditFocalLength)
	ON_EN_CHANGE(IDC_EDIT_PROBE_ID, OnChangeEditProbeId)
	ON_EN_CHANGE(IDC_EDIT_MATERIAL_VELOCITY, OnChangeEditMaterialVelocity)
	ON_EN_CHANGE(IDC_EDIT_SHEARWAVE_VELOCITY, OnChangeEditShearwaveVelocity)
	ON_BN_CLICKED(IDC_BUTTON_REF_RESULT, OnButtonRefResult)
	ON_BN_CLICKED(ID_BUTTON_TEST_RESULT, OnButtonTestResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInclusionRulesPage message handlers
BOOL CInclusionRulesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_spinXSamples.SetRange(1,100);
	m_spinYSamples.SetRange(1,100);
	m_spinThreshold.SetRange(1,100);

	
	UpdateAllControls();
	
	SetAccessPrivelages();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInclusionRulesPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"%d",theApp.m_LastSettings.nDefXSamples);
	m_editXSamples.SetWindowText(Buff);
	Buff.Format(L"%d",theApp.m_LastSettings.nDefYSamples);
	m_editYSamples.SetWindowText(Buff);

	Buff.Format(L"%d %%",theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1]);
	m_editThreshold.SetWindowText(Buff);

	m_spinXSamples.SetPos(theApp.m_LastSettings.nDefXSamples);
	m_spinYSamples.SetPos(theApp.m_LastSettings.nDefYSamples);
	m_spinThreshold.SetPos(theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1]);

	Buff.Format(L"%.0f %%FSH",theApp.m_LastSettings.fClassFSH[0]);
	m_editGreaterA.SetWindowText(Buff);
	Buff.Format(L"%.0f %%FSH",theApp.m_LastSettings.fClassFSH[1]);
	m_editGreaterB.SetWindowText(Buff);
	Buff.Format(L"%.0f %%FSH",theApp.m_LastSettings.fClassFSH[2]);
	m_editGreaterC.SetWindowText(Buff);

	Buff.Format(L"%.01f",theApp.m_LastSettings.fWeightFSH[0]);
	m_editWeightA.SetWindowText(Buff);
	Buff.Format(L"%.01f",theApp.m_LastSettings.fWeightFSH[1]);
	m_editWeightB.SetWindowText(Buff);
	Buff.Format(L"%.01f",theApp.m_LastSettings.fWeightFSH[2]);
	m_editWeightC.SetWindowText(Buff);



	m_editFBHSize.SetWindowText(m_pInclusions->m_RefFBH);
	m_editFocalDepth.SetWindowText(m_pInclusions->m_FocalDepth);
	m_editFocalLength.SetWindowText(m_pInclusions->m_FocalLength);
	m_editProbeID.SetWindowText(m_pInclusions->m_ProbeName);
	m_editRefGain.SetWindowText(m_pInclusions->m_RefGain);
	m_editRefStandard.SetWindowText(m_pInclusions->m_RefStandard);


	Buff.Format(L"%.01f g cm\x02c9\x0b3",theApp.m_LastSettings.fMaterialDensity);
	m_editDensity.SetWindowText(Buff);
	Buff.Format(L"%d m/s",theApp.m_LastSettings.nMaterialVelocity);
	m_editMaterialVelocity.SetWindowText(Buff);
	Buff.Format(L"%d m/s",theApp.m_LastSettings.nShearWaveVelocity);
	m_editShearWaveVelocity.SetWindowText(Buff);

	CTime RefTime(m_pInclusions->m_osRefDate);
	Buff = RefTime.Format(_T("%d %b %Y %Hh%M"));
	m_editRefDate.SetWindowText(Buff);
	m_editRefGain.SetWindowText( m_pInclusions->m_RefGain );
	m_editRefSensitivity.SetWindowText( m_pInclusions->m_RefSensitivityFBH );


	CTime TestTime(m_pInclusions->m_osTestDate);
	Buff = TestTime.Format(_T("%d %b %Y %Hh%M"));
	m_editTestDate.SetWindowText(Buff);
	m_editTestGain.SetWindowText( m_pInclusions->m_TestGain );
	m_editTestSensitivity.SetWindowText( m_pInclusions->m_TestSensitivityFBH );
}

void CInclusionRulesPage::OnChangeEditXSamples() 
{
	CString Buff;

	m_editXSamples.GetWindowText(Buff);
	theApp.m_LastSettings.nDefXSamples = _ttoi(Buff);
	MinMax(&theApp.m_LastSettings.nDefXSamples,1,100);

	m_spinXSamples.SetPos(theApp.m_LastSettings.nDefXSamples);
}

void CInclusionRulesPage::OnChangeEditYSamples() 
{
	CString Buff;

	m_editYSamples.GetWindowText(Buff);
	theApp.m_LastSettings.nDefYSamples = _ttoi(Buff);
	MinMax(&theApp.m_LastSettings.nDefYSamples,1,100);

	m_spinYSamples.SetPos(theApp.m_LastSettings.nDefYSamples);
}

void CInclusionRulesPage::OnChangeEditThreshold() 
{
	CString Buff;

	m_editThreshold.GetWindowText(Buff);
	theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1] = _ttoi(Buff);
	MinMax(&theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1],1,100);

	m_spinThreshold.SetPos(theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1]);
}

void CInclusionRulesPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl*)pScrollBar;
	int	nValue = pSpin->GetPos() & 0xffff;
	CString Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_X_SAMPLES:
		if(theApp.m_LastSettings.nDefXSamples - nValue) {
			Buff.Format(L"%d",theApp.m_LastSettings.nDefXSamples = nValue);
			m_editXSamples.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_Y_SAMPLES:
		if(theApp.m_LastSettings.nDefYSamples - nValue) {
			Buff.Format(L"%d",theApp.m_LastSettings.nDefYSamples = nValue);
			m_editYSamples.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_THRESHOLD:
		if(theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1] - nValue) {
			Buff.Format(L"%d%%",theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1] = nValue);
			m_editThreshold.SetWindowText(Buff);
		}
		break;
	}
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CInclusionRulesPage::OnChangeEditGreaterA() 
{
	CString Buff;

	m_editGreaterA.GetWindowText(Buff);
	_WTOF(Buff,	theApp.m_LastSettings.fClassFSH[0]);
}

void CInclusionRulesPage::OnChangeEditGreaterB() 
{
	CString Buff;

	m_editGreaterB.GetWindowText(Buff);
	_WTOF(Buff,	theApp.m_LastSettings.fClassFSH[1]);
}

void CInclusionRulesPage::OnChangeEditGreaterC() 
{
	CString Buff;

	m_editGreaterC.GetWindowText(Buff);
	_WTOF(Buff,	theApp.m_LastSettings.fClassFSH[2]);
}

void CInclusionRulesPage::OnChangeEditWeightA() 
{
	CString Buff;

	m_editWeightA.GetWindowText(Buff);
	_WTOF(Buff,	theApp.m_LastSettings.fWeightFSH[0]);
}

void CInclusionRulesPage::OnChangeEditWeightB() 
{
	CString Buff;

	m_editWeightB.GetWindowText(Buff);
	_WTOF(Buff,	theApp.m_LastSettings.fWeightFSH[1]);
}

void CInclusionRulesPage::OnChangeEditWeightC() 
{
	CString Buff;

	m_editWeightC.GetWindowText(Buff);
	_WTOF(Buff,	theApp.m_LastSettings.fWeightFSH[2]);
}


void CInclusionRulesPage::OnChangeEditFbhSize() 
{

	m_editFBHSize.GetWindowText(m_pInclusions->m_RefFBH);
}

void CInclusionRulesPage::OnChangeEditFocalDepth() 
{

	m_editFocalDepth.GetWindowText(m_pInclusions->m_FocalDepth);
}

void CInclusionRulesPage::OnChangeEditFocalLength() 
{

	m_editFocalLength.GetWindowText(m_pInclusions->m_FocalLength);
}


void CInclusionRulesPage::OnChangeEditProbeId() 
{

	m_editProbeID.GetWindowText(m_pInclusions->m_ProbeName);
}

void CInclusionRulesPage::OnChangeEditRefGain() 
{

	m_editRefGain.GetWindowText(m_pInclusions->m_RefGain);
}

void CInclusionRulesPage::OnChangeEditRefStandard() 
{

	m_editRefStandard.GetWindowText(m_pInclusions->m_RefStandard);
}






void CInclusionRulesPage::SetAccessPrivelages()
{

	
	m_editTestDate.SetReadOnly(true);
	m_editTestGain.SetReadOnly(true);
	m_editTestSensitivity.SetReadOnly(true);

	if(m_nGlobalLocal == 1) {
		m_editXSamples.SetReadOnly(true);
		m_editYSamples.SetReadOnly(true);

		m_editThreshold.SetReadOnly(true);

		m_spinXSamples.EnableWindow(false);
		m_spinYSamples.EnableWindow(false);
		m_spinThreshold.EnableWindow(false);

		m_editGreaterA.SetReadOnly(true);
		m_editGreaterB.SetReadOnly(true);
		m_editGreaterC.SetReadOnly(true);

		m_editWeightA.SetReadOnly(true);
		m_editWeightB.SetReadOnly(true);
		m_editWeightC.SetReadOnly(true);

		m_editFBHSize.SetReadOnly(true);
		m_editFocalDepth.SetReadOnly(true);
		m_editFocalLength.SetReadOnly(true);
		m_editProbeID.SetReadOnly(true);
		m_editRefGain.SetReadOnly(true);
		m_editRefStandard.SetReadOnly(true);
	}
}

void CInclusionRulesPage::OnChangeEditDensity() 
{
	CString	Buff;

	m_editDensity.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fMaterialDensity);
}

void CInclusionRulesPage::OnChangeEditMaterialVelocity() 
{
	CString	Buff;

	m_editMaterialVelocity.GetWindowText(Buff);
	theApp.m_LastSettings.nMaterialVelocity = _ttoi(Buff);
}

void CInclusionRulesPage::OnChangeEditShearwaveVelocity() 
{
	CString	Buff;

	m_editShearWaveVelocity.GetWindowText(Buff);
	theApp.m_LastSettings.nShearWaveVelocity = _ttoi(Buff);
}


void CInclusionRulesPage::OnButtonRefResult() 
{
	CTime CurrentTime = CTime::GetCurrentTime();

	m_pInclusions->m_osRefDate=(int)CurrentTime.GetTime();

	m_pInclusions->m_RefGain.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[0].Pr30.fLinGain);

	m_pInclusions->m_RefSensitivityFBH.Format(L"%d %%FSH",MulDiv(theApp.m_UtUser.m_TS[0].Gate.nAmplitude[1],100,127));

	UpdateAllControls();

}


void CInclusionRulesPage::OnButtonTestResult() 
{
	CTime CurrentTime = CTime::GetCurrentTime();

	m_pInclusions->m_osTestDate=(int)CurrentTime.GetTime();

	m_pInclusions->m_TestGain.Format(_T("%.01f dB"),theApp.m_UtUser.m_TS[0].Pr30.fLinGain);

	m_pInclusions->m_TestSensitivityFBH.Format(L"%d %%FSH",MulDiv(theApp.m_UtUser.m_TS[0].Gate.nAmplitude[1],100,127));

	UpdateAllControls();
}

