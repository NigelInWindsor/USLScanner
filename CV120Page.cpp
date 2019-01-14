// CV120Page.cpp : implementation file
//


#include "stdafx.h"
#include "uslscanner.h"
#include "CV120Page.h"
#include "afxdialogex.h"

#ifdef NPTRACKINGTOOLS
// CV120Page dialog

IMPLEMENT_DYNAMIC(CV120Page, CPropertyPage)

CV120Page::CV120Page()
	: CPropertyPage(IDD_CV120Page)
{
	
	m_pV120Trio = thetheApp.m_pV120Trio;
	thetheApp.m_pTracker = &m_rigidBody;
	m_nMinRate=0;
	m_nMaxRate=120;
}

CV120Page::~CV120Page()
{
	delete m_pV120Trio;
}

void CV120Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_I_OT, m_editI);
	DDX_Control(pDX, IDC_EDIT_J_OT, m_editJ);
	DDX_Control(pDX, IDC_EDIT_K_OT, m_editK);
	DDX_Control(pDX, IDC_EDIT_NMARKS_OT, m_editNMarks);
	DDX_Control(pDX, IDC_EDIT_NRGDS_OT, m_editNRgds);
	DDX_Control(pDX, IDC_EDIT_X_OT, m_editX);
	DDX_Control(pDX, IDC_EDIT_Y_OT, m_editY);
	DDX_Control(pDX, IDC_EDIT_Z_OT, m_editZ);
	DDX_Control(pDX, ID_INIT_BUTTON_OT, m_buttonInit);
	DDX_Control(pDX, ID_SHUTDOWN_BUTTON_OT, m_buttonShutdown);
	DDX_Control(pDX, ID_BUTTON_UPDATE_OT, m_buttonUpdate);
	DDX_Control(pDX, IDC_EDIT_NCAM_OT, m_editNCams);
	DDX_Control(pDX, IDC_EDIT_QW_OT, m_editQW);
	DDX_Control(pDX, IDC_EDIT_QX_OT, m_editQX);
	DDX_Control(pDX, IDC_EDIT_QY_OT, m_editQY);
	DDX_Control(pDX, IDC_EDIT_QZ_OT, m_editQZ);
	DDX_Control(pDX, ID_BUTTON_STOP_OT, m_buttonStop);
	DDX_Control(pDX, IDC_EDIT_RATE_OT, m_editRate);
	DDX_Control(pDX, IDC_SPIN_RATE_OT, m_spinRate);
	DDX_Control(pDX, ID_BUTTON_ZERO_ORIENT_OT, m_buttonZeroOrient);
	DDX_Control(pDX, ID_BUTTON_ZERO_POS_OT, m_buttonZeroPos);
	DDX_Control(pDX, ID_BUTTON_REFINE_RB_OT, m_buttonRefineRB);
	DDX_Control(pDX, IDC_EDIT_XERR_OT, m_editErrX);
	DDX_Control(pDX, IDC_EDIT_YERR_OT, m_editErrY);
	DDX_Control(pDX, IDC_EDIT_ZERR_OT, m_editErrZ);
	DDX_Control(pDX, IDC_EDIT_KERR_OT, m_editErrK);
	DDX_Control(pDX, IDC_EDIT_JERR_OT, m_editErrJ);
	DDX_Control(pDX, IDC_EDIT_IERR_OT, m_editErrI);
	DDX_Control(pDX, IDC_EDIT_IMEAN_OT, m_editMeanI);
	DDX_Control(pDX, IDC_EDIT_JMEAN_OT, m_editMeanJ);
	DDX_Control(pDX, IDC_EDIT_KMEAN, m_editMeanK);
	DDX_Control(pDX, IDC_EDIT_MOVINGAVERAGE, m_editMovingAverage);
	DDX_Control(pDX, IDC_EDIT_XMEAN_OT, m_editMeanX);
	DDX_Control(pDX, IDC_EDIT_YMEAN_OT, m_editMeanY);
	DDX_Control(pDX, IDC_EDIT_ZMEAN_OT, m_editMeanZ);
	DDX_Control(pDX, IDC_SPIN_MOVINGAVERAGE, m_spinMovingAverage);
	DDX_Control(pDX, IDC_EDIT_JSTD_OT, m_editStdJ);
	DDX_Control(pDX, IDC_EDIT_ISTD_OT, m_editStdI);
	DDX_Control(pDX, IDC_EDIT_KSTD_OT, m_editStdK);
	DDX_Control(pDX, IDC_EDIT_XSTD_OT, m_editStdX);
	DDX_Control(pDX, IDC_EDIT_YSTD_OT, m_editStdY);
	DDX_Control(pDX, IDC_EDIT_ZSTD_OT, m_editStdZ);
	DDX_Control(pDX, IDC_EDIT_PRINTMODULO, m_editPrintModulo);
	DDX_Control(pDX, IDC_SPIN_PRINTMODULO, m_spinPrintModulo);
	DDX_Control(pDX, IDC_EDIT_DIST_OT, m_editDist);
	DDX_Control(pDX, IDC_EDIT_DISTERR_OT, m_editErrDist);
	DDX_Control(pDX, IDC_EDIT_DISTMEAN_OT, m_editMeanDist);
	DDX_Control(pDX, IDC_EDIT_DISTSTD_OT, m_editStdDist);
	DDX_Control(pDX, IDC_COMBO_RIGIDBODY, m_comboRigidBody);
}


BEGIN_MESSAGE_MAP(CV120Page, CPropertyPage)
	ON_BN_CLICKED(ID_INIT_BUTTON_OT, &CV120Page::OnClickedInitButton)
	ON_BN_CLICKED(ID_SHUTDOWN_BUTTON_OT, &CV120Page::OnClickedShutdownButton)
	ON_BN_CLICKED(ID_BUTTON_UPDATE_OT, &CV120Page::OnClickedButtonUpdate)
	ON_BN_CLICKED(ID_BUTTON_STOP_OT, &CV120Page::OnClickedButtonStop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RATE_OT, &CV120Page::OnDeltaposSpinRate)
	ON_EN_CHANGE(IDC_EDIT_RATE_OT, &CV120Page::OnChangeEditRate)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_ZERO_ORIENT_OT, &CV120Page::OnClickedButtonZeroOrientOt)
	ON_BN_CLICKED(ID_BUTTON_ZERO_POS_OT, &CV120Page::OnClickedButtonZeroPosOt)
	ON_BN_CLICKED(ID_BUTTON_REFINE_RB_OT, &CV120Page::OnClickedButtonRefineRbOt)
	ON_EN_CHANGE(IDC_EDIT_MOVINGAVERAGE, &CV120Page::OnChangeEditMovingaverage)
	ON_EN_CHANGE(IDC_EDIT_PRINTMODULO, &CV120Page::OnChangeEditPrintmodulo)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVINGAVERAGE, &CV120Page::OnDeltaposSpinMovingaverage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRINTMODULO, &CV120Page::OnDeltaposSpinPrintmodulo)
	ON_CBN_SELCHANGE(IDC_COMBO_RIGIDBODY, &CV120Page::OnSelchangeComboRigidbody)
END_MESSAGE_MAP()

BOOL CV120Page::OnInitDialog() {
	CDialog::OnInitDialog();
	if (m_pV120Trio == NULL) {
		m_pV120Trio = new CV120Trio();
	}
	const wchar_t* sGUID = L"{0AAA8B22-8324-4F01-9248-F020731E398F}"; //unique ID for shared memory object.
	m_spinRate.SetRange(0, 200);
	m_spinPrintModulo.SetRange(1, 10000);
	m_spinMovingAverage.SetRange(1, 10000);
	m_nRate = 100;
	m_nPrintModulo = 10;
	m_rigidBody.SetStatsLength(10);
	UpdateEditRate();
	UpdateEditPrintModulo();
	UpdateEditMovingAverage();

	m_comboRigidBody.AddString(L"Rigid body 1");
	m_comboRigidBody.AddString(L"Rigid body 2");
	m_comboRigidBody.AddString(L"Rigid body 3");
	m_comboRigidBody.AddString(L"Rigid body 4");
	m_comboRigidBody.SetCurSel(m_rigidBody.GetIdent());

	return TRUE;
}

void CV120Page::UpdateEditRate() {
	CString buff;
	buff.Format(L"%d", m_nRate);
	m_editRate.SetWindowText(buff);
}

void CV120Page::UpdateEditMovingAverage()
{
	CString buff;
	buff.Format(L"%d", m_rigidBody.GetStatsLength());
	m_editMovingAverage.SetWindowText(buff);
}

void CV120Page::UpdateEditPrintModulo()
{
	CString buff;
	buff.Format(L"%d", m_nPrintModulo);
	m_editPrintModulo.SetWindowText(buff);
}

// CV120Page message handlers

void CV120Page::OnClickedInitButton()
{
	m_buttonInit.EnableWindow(false);
	bool initResult = m_pV120Trio->Initialize();
	if (initResult) {
		m_buttonShutdown.EnableWindow(true);
		m_buttonStop.EnableWindow(true);
		m_buttonZeroOrient.EnableWindow(true);
		m_buttonZeroPos.EnableWindow(true);
	}
	else {
		m_buttonInit.EnableWindow(true);
	}
}


void CV120Page::OnClickedShutdownButton()
{
	m_buttonShutdown.EnableWindow(false);
	if (m_pV120Trio->Shutdown()) {
		m_buttonUpdate.EnableWindow(false);
		m_buttonStop.EnableWindow(false);
		m_buttonInit.EnableWindow(true);
		m_buttonZeroOrient.EnableWindow(false);
		m_buttonZeroPos.EnableWindow(false);
	}
	else {
		m_buttonShutdown.EnableWindow(true);
	}
}


void CV120Page::Update() {
	bool bTracked = m_pV120Trio->Update(&m_rigidBody);
	m_nCurrent++;
	if (m_nCurrent >= m_nPrintModulo) {
		CString val;
		val.Format(L"%d", m_pV120Trio->MarkerCount());
		m_editNMarks.SetWindowText(val);
		val.Format(L"%d", m_pV120Trio->CameraCount());
		m_editNCams.SetWindowText(val);
		val.Format(L"%d", m_pV120Trio->RigidBodyCount());
		m_editNRgds.SetWindowText(val);

		if (!bTracked) {
			val = L"NM";
			m_editX.SetWindowText(val);
			m_editY.SetWindowText(val);
			m_editZ.SetWindowText(val);
			m_editDist.SetWindowText(val);

			m_editI.SetWindowText(val);
			m_editJ.SetWindowText(val);
			m_editK.SetWindowText(val);
		}
		else {
			UpdateCoords();
			UpdateStats();
		}
		m_nCurrent = 0;
	}
}

void CV120Page::UpdateCoords() {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 angle;
	D3DXQUATERNION quat;
	float dist;
	m_rigidBody.GetPositionData(&pos, &angle, &quat, &dist);
	CString val;
	val.Format(L"%.3f", pos.x);
	m_editX.SetWindowText(val);
	val.Format(L"%.3f", pos.y);
	m_editY.SetWindowText(val);
	val.Format(L"%.3f", pos.z);
	m_editZ.SetWindowText(val);
	val.Format(L"%.3f", dist);
	m_editDist.SetWindowText(val);

	val.Format(L"%.3f", angle.x);
	m_editI.SetWindowText(val);
	val.Format(L"%.3f", angle.y);
	m_editJ.SetWindowText(val);
	val.Format(L"%.3f", angle.z);
	m_editK.SetWindowText(val);
}

void CV120Page::UpdateStats() {
	CString val;
	val.Format(L"%.3f", m_rigidBody.GetMAx()->Mean());
	m_editMeanX.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAy()->Mean());
	m_editMeanY.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAz()->Mean());
	m_editMeanZ.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMADist()->Mean());
	m_editMeanDist.SetWindowText(val);

	val.Format(L"%.3f", m_rigidBody.GetMAyaw()->Mean());
	m_editMeanI.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMApitch()->Mean());
	m_editMeanJ.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAroll()->Mean());
	m_editMeanK.SetWindowText(val);

	val.Format(L"%.3f", m_rigidBody.GetMAx()->STDev());
	m_editStdX.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAy()->STDev());
	m_editStdY.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAz()->STDev());
	m_editStdZ.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMADist()->STDev());
	m_editStdDist.SetWindowText(val);
		
	val.Format(L"%.3f", m_rigidBody.GetMAyaw()->STDev());
	m_editStdI.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMApitch()->STDev());
	m_editStdJ.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAroll()->STDev());
	m_editStdK.SetWindowText(val);
	
	val.Format(L"%.3f", m_rigidBody.GetMAx()->STDErr());
	m_editErrX.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAy()->STDErr());
	m_editErrY.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAz()->STDErr());
	m_editErrZ.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMADist()->STDErr());
	m_editErrDist.SetWindowText(val);

	val.Format(L"%.3f", m_rigidBody.GetMAyaw()->STDErr());
	m_editErrI.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMApitch()->STDErr());
	m_editErrJ.SetWindowText(val);
	val.Format(L"%.3f", m_rigidBody.GetMAroll()->STDErr());
	m_editErrK.SetWindowText(val);

}

void CV120Page::OnClickedButtonUpdate()
{
	m_buttonUpdate.EnableWindow(false);
	m_bUpdating = true;
	m_buttonStop.EnableWindow(true);

}


void CV120Page::OnClickedButtonStop()
{
	m_buttonStop.EnableWindow(false);
	m_bUpdating = false;
	m_buttonUpdate.EnableWindow(true);
}

void CV120Page::OnDeltaposSpinRate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString buff;
	m_nRate += (pNMUpDown->iDelta);
	UpdateEditRate();
	*pResult = 0;
}


void CV120Page::OnChangeEditRate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	CString buff;
	int nValue = m_nRate;
	m_editRate.GetWindowText(buff);
	SetRate(_wtoi(buff));
	m_spinRate.SetPos(m_nRate);
}


void CV120Page::SetRate() {
	SetTimer(1, m_nDeltaTms, NULL);
}

void CV120Page::SetRate(int val) {
	m_nRate = val;
	if (m_nRate != 0) {
		m_nDeltaTms = 1000 / m_nRate;
	}
	else {
		m_nDeltaTms = 999999999;
	}
	SetRate();
}

void CV120Page::OnTimer(UINT_PTR nIDEvent)
{
	CPropertyPage::OnTimer(nIDEvent);
	if (!m_bUpdating) return;
	Update();
}


BOOL CV120Page::OnSetActive()
{
	SetRate();

	return CPropertyPage::OnSetActive();
}


BOOL CV120Page::OnKillActive()
{
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}


void CV120Page::OnClickedButtonZeroOrientOt()
{

	m_rigidBody.ZeroOrientation();
	
}


void CV120Page::OnClickedButtonZeroPosOt()
{
	m_rigidBody.SetOrigin();
}


void CV120Page::OnClickedButtonRefineRbOt()
{

}



void CV120Page::OnChangeEditMovingaverage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.	CString buff;
	int nValue;
	CString buff;
	m_editMovingAverage.GetWindowText(buff);
	nValue = _wtoi(buff);
	m_rigidBody.SetStatsLength(nValue);
	m_spinMovingAverage.SetPos(nValue);
}


void CV120Page::OnChangeEditPrintmodulo()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	CString buff;
	m_editPrintModulo.GetWindowText(buff);
	m_nPrintModulo = _wtoi(buff);
	m_spinPrintModulo.SetPos(m_nPrintModulo);
}


void CV120Page::OnDeltaposSpinMovingaverage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int val = m_rigidBody.GetStatsLength();
	val += (pNMUpDown->iDelta);
	m_rigidBody.SetStatsLength(val);
	UpdateEditMovingAverage();
	*pResult = 0;
}


void CV120Page::OnDeltaposSpinPrintmodulo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_nPrintModulo += (pNMUpDown->iDelta);
	UpdateEditPrintModulo();
	*pResult = 0;
}


void CV120Page::OnSelchangeComboRigidbody()
{
	int nSel = m_comboRigidBody.GetCurSel();
	int rbCount = m_pV120Trio->RigidBodyCount();
	if (nSel >= rbCount) {
		nSel = rbCount - 1;
		m_comboRigidBody.SetCurSel(nSel);
	}
	m_rigidBody.SetIdent(nSel);
}
#endif //ifdef NPTRACKINGTOOLS