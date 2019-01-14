// RotateSwivelGimbalPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RotateSwivelGimbalPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateSwivelGimbalPage property page

IMPLEMENT_DYNCREATE(CRotateSwivelGimbalPage, CPropertyPage)

CRotateSwivelGimbalPage::CRotateSwivelGimbalPage() : CPropertyPage(CRotateSwivelGimbalPage::IDD)
{
	//{{AFX_DATA_INIT(CRotateSwivelGimbalPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bVirtual = false;
	

//	theApp.m_Kinematics.m_vLimb0[PORTSIDE] = D3DXVECTOR3(170.0f, 0.0f, -40.0f);
//	theApp.m_Kinematics.m_vLimb0[PORTSIDE] = D3DXVECTOR3(170.0f, 0.0f, 0.0f);
//	theApp.m_Kinematics.m_vLimb1[PORTSIDE] = D3DXVECTOR3(48.0f, -32.0f, 133.0f);
}

CRotateSwivelGimbalPage::~CRotateSwivelGimbalPage()
{
}

void CRotateSwivelGimbalPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateSwivelGimbalPage)
	DDX_Control(pDX, IDC_EDIT_LIMB_1, m_editLimb1);
	DDX_Control(pDX, IDC_EDIT_LIMB_0, m_editLimb0);
	DDX_Control(pDX, IDC_SPIN_SWIVEL, m_spinSwivel);
	DDX_Control(pDX, IDC_SPIN_ROTATE, m_spinRotate);
	DDX_Control(pDX, IDC_SPIN_GIMBAL, m_spinGimbal);
	DDX_Control(pDX, IDC_CHECK_VIRTUAL, m_checkvirtual);
	DDX_Control(pDX, IDC_EDIT_INVERSE_X, m_editInverseX);
	DDX_Control(pDX, IDC_EDIT_INVERSE_Y, m_editInverseY);
	DDX_Control(pDX, IDC_EDIT_INVERSE_Z, m_editInverseZ);
	DDX_Control(pDX, IDC_EDIT_INVERSE_ROTATE, m_editInverseRotate);
	DDX_Control(pDX, IDC_EDIT_INVERSE_SWIVEL, m_editInverseSwivel);
	DDX_Control(pDX, IDC_EDIT_INVERSE_GIMBAL, m_editInverseGimbal);
	DDX_Control(pDX, IDC_EDIT_FORWARD_X, m_editForwardX);
	DDX_Control(pDX, IDC_EDIT_FORWARD_Y, m_editForwardY);
	DDX_Control(pDX, IDC_EDIT_FORWARD_Z, m_editForwardZ);
	DDX_Control(pDX, IDC_EDIT_FORWARD_I, m_editForwardI);
	DDX_Control(pDX, IDC_EDIT_FORWARD_J, m_editForwardJ);
	DDX_Control(pDX, IDC_EDIT_FORWARD_K, m_editForwardK);
	DDX_Control(pDX, IDC_EDIT_ACTUAL_X, m_editActualX);
	DDX_Control(pDX, IDC_EDIT_ACTUAL_Y, m_editActualY);
	DDX_Control(pDX, IDC_EDIT_ACTUAL_Z, m_editActualZ);
	DDX_Control(pDX, IDC_EDIT_ACTUAL_ROTATE, m_editActualRotate);
	DDX_Control(pDX, IDC_EDIT_ACTUAL_SWIVEL, m_editActualSwivel);
	DDX_Control(pDX, IDC_EDIT_ACTUAL_GIMBAL, m_editActualGimbal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotateSwivelGimbalPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRotateSwivelGimbalPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_HMZ_ROTSWIVELGIMBAL, OnButtonHmzRotswivelgimbal)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_TO_ZERO, OnButtonRotateToZero)
	ON_BN_CLICKED(IDC_CHECK_VIRTUAL, OnCheckVirtual)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_LIMB_0, OnChangeEditLimb0)
	ON_EN_CHANGE(IDC_EDIT_LIMB_1, OnChangeEditLimb1)
	ON_EN_CHANGE(IDC_EDIT_ACTUAL_X, OnChangeEditActualX)
	ON_EN_CHANGE(IDC_EDIT_ACTUAL_Y, OnChangeEditActualY)
	ON_EN_CHANGE(IDC_EDIT_ACTUAL_Z, OnChangeEditActualZ)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateSwivelGimbalPage message handlers

BOOL CRotateSwivelGimbalPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UDACCEL	Accel;
	Accel.nInc=10;
	Accel.nSec=1;
	
	m_spinRotate.SetRange(0,360);
	m_spinSwivel.SetRange(0,360);
	m_spinGimbal.SetRange(0,360);
	m_spinRotate.SetAccel(1,&Accel);
	m_spinSwivel.SetAccel(1,&Accel);
	m_spinGimbal.SetAccel(1,&Accel);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotateSwivelGimbalPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CCoord CpActual;
	CString Buff;

	m_checkvirtual.SetCheck(m_bVirtual);

	m_spinRotate.ShowWindow(m_bVirtual);
	m_spinSwivel.ShowWindow(m_bVirtual);
	m_spinGimbal.ShowWindow(m_bVirtual);

	switch(m_bVirtual) {
	case false:
		break;
	case true:
		theApp.m_Motors.GetEncoderPos(&CpActual);
		m_spinRotate.SetPos((int)CpActual.Side0.fR + 180);
		m_spinSwivel.SetPos((int)CpActual.Side0.fXt + 180);
		m_spinGimbal.SetPos((int)CpActual.Side0.fYt + 180);
		break;
	};

	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_Kinematics.m_vLimb0[PORTSIDE].x,theApp.m_Kinematics.m_vLimb0[PORTSIDE].y,theApp.m_Kinematics.m_vLimb0[PORTSIDE].z);
	m_editLimb0.SetWindowText(Buff);
	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),theApp.m_Kinematics.m_vLimb1[PORTSIDE].x,theApp.m_Kinematics.m_vLimb1[PORTSIDE].y,theApp.m_Kinematics.m_vLimb1[PORTSIDE].z);
	m_editLimb1.SetWindowText(Buff);

}

BOOL CRotateSwivelGimbalPage::OnSetActive() 
{
	
	
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CRotateSwivelGimbalPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CRotateSwivelGimbalPage::OnTimer(UINT nIDEvent) 
{
	
	CString Buff;
	CCoord CpActual,CpForward,CpInverse,CpHead;

	theApp.m_Motors.GetEncoderPos(&CpActual);

	theApp.m_Motors.GetHeadPos(&CpHead);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpForward,PORTSIDE,TRUE,TRUE, CURRENT_FL);
	theApp.m_Kinematics.HeadFromSurface(&CpForward,&CpInverse,PORTSIDE,TRUE,TRUE,NULL,CURRENT_FL);
	
	//Actual
	if(m_CpOldActual.Side0.fX - CpActual.Side0.fX) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpActual.Side0.fX * theApp.m_fUnits, theApp.m_Units);
		m_editActualX.SetWindowText(Buff);
	}
	if(m_CpOldActual.Side0.fY - CpActual.Side0.fY) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpActual.Side0.fY * theApp.m_fUnits, theApp.m_Units);
		m_editActualY.SetWindowText(Buff);
	}
	if(m_CpOldActual.Side0.fZ - CpActual.Side0.fZ) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpActual.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
		m_editActualZ.SetWindowText(Buff);
	}
	if(m_CpOldActual.Side0.fR - CpActual.Side0.fR) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpActual.Side0.fR,DEGREES);
		m_editActualRotate.SetWindowText(Buff);
	}
	if(m_CpOldActual.Side0.fXt - CpActual.Side0.fXt) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,CpActual.Side0.fXt,DEGREES);
		m_editActualSwivel.SetWindowText(Buff);
	}
	if(m_CpOldActual.Side0.fYt - CpActual.Side0.fYt) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,CpActual.Side0.fYt,DEGREES);
		m_editActualGimbal.SetWindowText(Buff);
	}
	

	//Forward
	if(m_CpOldForward.Side0.fX - CpForward.Side0.fX) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpForward.Side0.fX * theApp.m_fUnits, theApp.m_Units);
		m_editForwardX.SetWindowText(Buff);
	}
	if(m_CpOldForward.Side0.fY - CpForward.Side0.fY) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpForward.Side0.fY * theApp.m_fUnits, theApp.m_Units);
		m_editForwardY.SetWindowText(Buff);
	}
	if(m_CpOldForward.Side0.fZ - CpForward.Side0.fZ) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpForward.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
		m_editForwardZ.SetWindowText(Buff);
	}
	if(m_CpOldForward.Side0.fI - CpForward.Side0.fI) {
		Buff.Format(_T("%.4f"),CpForward.Side0.fI);
		m_editForwardI.SetWindowText(Buff);
	}
	if(m_CpOldForward.Side0.fJ - CpForward.Side0.fJ) {
		Buff.Format(_T("%.4f"),CpForward.Side0.fJ);
		m_editForwardJ.SetWindowText(Buff);
	}
	if(m_CpOldForward.Side0.fK - CpForward.Side0.fK) {
		Buff.Format(_T("%.4f"),CpForward.Side0.fK);
		m_editForwardK.SetWindowText(Buff);
	}

	//Inverse
	if(m_CpOldInverse.Side0.fX - CpInverse.Side0.fX) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,CpInverse.Side0.fX * theApp.m_fUnits, theApp.m_Units);
		m_editInverseX.SetWindowText(Buff);
	}
	if(m_CpOldInverse.Side0.fY - CpInverse.Side0.fY) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,CpInverse.Side0.fY * theApp.m_fUnits, theApp.m_Units);
		m_editInverseY.SetWindowText(Buff);
	}
	if(m_CpOldInverse.Side0.fZ - CpInverse.Side0.fZ) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,CpInverse.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
		m_editInverseZ.SetWindowText(Buff);
	}
	if(m_CpOldInverse.Side0.fR - CpInverse.Side0.fR) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPrecision,CpInverse.Side0.fR,DEGREES);
		m_editInverseRotate.SetWindowText(Buff);
	}
	if(m_CpOldInverse.Side0.fXt - CpInverse.Side0.fXt) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,CpInverse.Side0.fXt,DEGREES);
		m_editInverseSwivel.SetWindowText(Buff);
	}
	if(m_CpOldInverse.Side0.fYt - CpInverse.Side0.fYt) {
		Buff.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,CpInverse.Side0.fYt,DEGREES);
		m_editInverseGimbal.SetWindowText(Buff);
	}

	m_CpOldActual = CpActual;
	m_CpOldForward = CpForward;
	m_CpOldInverse = CpInverse;

	CPropertyPage::OnTimer(nIDEvent);
}

void CRotateSwivelGimbalPage::OnButtonHmzRotswivelgimbal() 
{
	

	theApp.m_PmacUser.HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis);
	theApp.m_PmacUser.HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis);
	theApp.m_PmacUser.HomeAxisWithoutMoving(theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis);
	
}

void CRotateSwivelGimbalPage::OnButtonRotateToZero() 
{
	
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = 0.0f;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
}

void CRotateSwivelGimbalPage::OnCheckVirtual() 
{

	m_bVirtual == true ? m_bVirtual = false : m_bVirtual = true;

	m_checkvirtual.SetCheck(m_bVirtual);
	UpdateAllControls();
}


void CRotateSwivelGimbalPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp;
	CString Buff;
	

	nTemp = (pSpin->GetPos()&0xffff) - 180;
	nTemp = (nTemp / 10) * 10;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_ROTATE:
		if(m_bVirtual == true) {
			theApp.m_Axes[theApp.m_Tank.nRLeft].nPos = (int)((float)nTemp / theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
		}
		break;
	case IDC_SPIN_SWIVEL:
		if(m_bVirtual == true) {
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos = (int)((float)nTemp / theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		}
		break;
	case IDC_SPIN_GIMBAL:
		if(m_bVirtual == true) {
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos = (int)((float)nTemp / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		}
		break;
	};
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRotateSwivelGimbalPage::OnChangeEditLimb0() 
{
	
	CString Buff;

	m_editLimb0.GetWindowText(Buff);
	ExtractVector(Buff,&theApp.m_Kinematics.m_vLimb0[PORTSIDE]);

}

void CRotateSwivelGimbalPage::OnChangeEditLimb1() 
{
	
	CString Buff;

	m_editLimb1.GetWindowText(Buff);
	ExtractVector(Buff,&theApp.m_Kinematics.m_vLimb1[PORTSIDE]);
}

void CRotateSwivelGimbalPage::OnChangeEditActualX() 
{
	
	CString Buff;
	float fTemp;

	if(m_bVirtual == true) {
		m_editActualX.GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[theApp.m_Tank.nXLeft].nPos = (int)(fTemp / theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize)  - theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
	}
}

void CRotateSwivelGimbalPage::OnChangeEditActualY() 
{
	
	CString Buff;
	float fTemp;

	if(m_bVirtual == true) {
		m_editActualY.GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[theApp.m_Tank.nYLeft].nPos = (int)(fTemp / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset;
	}
}

void CRotateSwivelGimbalPage::OnChangeEditActualZ() 
{
	
	CString Buff;
	float fTemp;

	if(m_bVirtual == true) {
		m_editActualZ.GetWindowText(Buff);
		_WTOF(Buff,fTemp);
		theApp.m_Axes[theApp.m_Tank.nZLeft].nPos = (int)(fTemp / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset;
	}
}

void CRotateSwivelGimbalPage::OnButtonSave() 
{
	
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;


	CFileDialog FileDlg(FALSE,_T(".manipulator"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Manipulator (*.manipulator)|*.manipulator|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
			return ;
		} else {
			theApp.m_Kinematics.SaveManipulator(&File, PORTSIDE);

			File.Close();
			return ;
		}
	}
}

void CRotateSwivelGimbalPage::OnButtonOpen() 
{
	
	CString FilePath,Buff;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".manipulator"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Manipulator (*.manipulator)|*.manipulator|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			theApp.m_Kinematics.RetrieveManipulator(&File, PORTSIDE);

			File.Close();

			theApp.m_LimitAndKinematicOffset = FileDlg.GetFileTitle();
			UpdateAllControls();
			theApp.SaveTank();
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		}
	}
	
}
