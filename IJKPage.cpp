// IJKPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "IJKPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIJKPage property page

IMPLEMENT_DYNCREATE(CIJKPage, CPropertyPage)

CIJKPage::CIJKPage() : CPropertyPage(CIJKPage::IDD)
{
	//{{AFX_DATA_INIT(CIJKPage)
	//}}AFX_DATA_INIT
	
	m_nEditTipsNormals=0;
	m_CpHead.Zero();
	m_CpSurface.Zero();

}

CIJKPage::~CIJKPage()
{
}

void CIJKPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIJKPage)
	DDX_Control(pDX, IDC_EDIT_ROTATE_ABOUT_Z, m_editRotateAboutZ);
	DDX_Control(pDX, IDC_SPIN_ROTATE_ABOUT_Z, m_spinRotateAboutZ);
	DDX_Control(pDX, IDC_SPIN_YT_RIGHT, m_spinYTRight);
	DDX_Control(pDX, IDC_SPIN_I_RIGHT, m_spinIRight);
	DDX_Control(pDX, IDC_SPIN_J_RIGHT, m_spinJRight);
	DDX_Control(pDX, IDC_SPIN_K_RIGHT, m_spinKRight);
	DDX_Control(pDX, IDC_SPIN_XT_RIGHT, m_spinXTRight);
	DDX_Control(pDX, IDC_SPIN_K_LEFT, m_spinKLeft);
	DDX_Control(pDX, IDC_SPIN_J_LEFT, m_spinJLeft);
	DDX_Control(pDX, IDC_SPIN_I_LEFT, m_spinILeft);
	DDX_Control(pDX, IDC_SPIN_YT_LEFT, m_spinYTipLeft);
	DDX_Control(pDX, IDC_SPIN_XT_LEFT, m_spinXTipLeft);
	DDX_Control(pDX, IDC_COMBO_VIRTUAL_ACTUAL, m_comboVirtualActual);
	DDX_Control(pDX, IDC_EDIT_LEFT_W, m_editLeftW);
	DDX_Control(pDX, IDC_EDIT_RIGHT_W, m_editRightW);
	DDX_Control(pDX, IDC_EDIT_LEFT_XT2, m_editLeftXTip2);
	DDX_Control(pDX, IDC_EDIT_RIGHT_YT2, m_editRightYTip2);
	DDX_Control(pDX, IDC_EDIT_RIGHT_XT2, m_editRightXTip2);
	DDX_Control(pDX, IDC_EDIT_LEFT_YT2, m_editLeftYTip2);
	DDX_Control(pDX, IDC_EDIT_Z, m_editZ);
	DDX_Control(pDX, IDC_EDIT_Y, m_editY);
	DDX_Control(pDX, IDC_EDIT_X, m_editX);
	DDX_Control(pDX, IDC_EDIT_RIGHT_YT, m_editRightYt);
	DDX_Control(pDX, IDC_EDIT_RIGHT_XT, m_editRightXt);
	DDX_Control(pDX, IDC_EDIT_RIGHT_K, m_editRightK);
	DDX_Control(pDX, IDC_EDIT_RIGHT_J, m_editRightJ);
	DDX_Control(pDX, IDC_EDIT_RIGHT_I, m_editRightI);
	DDX_Control(pDX, IDC_EDIT_LEFT_YT, m_editLeftYt);
	DDX_Control(pDX, IDC_EDIT_LEFT_XT, m_editLeftXt);
	DDX_Control(pDX, IDC_EDIT_LEFT_K, m_editLeftK);
	DDX_Control(pDX, IDC_EDIT_LEFT_J, m_editLeftJ);
	DDX_Control(pDX, IDC_EDIT_LEFT_I, m_editLeftI);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIJKPage, CPropertyPage)
	//{{AFX_MSG_MAP(CIJKPage)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_Z, OnChangeEditZ)
	ON_EN_CHANGE(IDC_EDIT_LEFT_XT, OnChangeEditLeftXt)
	ON_EN_CHANGE(IDC_EDIT_LEFT_YT, OnChangeEditLeftYt)
	ON_CBN_SELCHANGE(IDC_COMBO_VIRTUAL_ACTUAL, OnSelchangeComboVirtualActual)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_ROTATE_ABOUT_Z, OnChangeEditRotateAboutZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ROTATE_ABOUT_Z, OnDeltaposSpinRotateAboutZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIJKPage message handlers

BOOL CIJKPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_comboVirtualActual.InsertString(0,L"Virtual");
	m_comboVirtualActual.InsertString(1,L"Actual");
	m_comboVirtualActual.InsertString(2,L"ClipBoard");

	m_spinXTipLeft.SetRange(0,360);
	m_spinYTipLeft.SetRange(0,360);
	m_spinILeft.SetRange(0,200);
	m_spinJLeft.SetRange(0,200);
	m_spinKLeft.SetRange(0,200);
	m_spinXTRight.SetRange(0,360);
	m_spinYTRight.SetRange(0,360);
	m_spinIRight.SetRange(0,200);
	m_spinJRight.SetRange(0,200);
	m_spinKRight.SetRange(0,200);

	m_spinRotateAboutZ.SetRange(0,1800);
	m_spinRotateAboutZ.SetPos(900);
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CIJKPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CIJKPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboVirtualActual.SetCurSel(theApp.m_LastSettings.nIJKDiagCombo);

	Buff.Format(_T("%.1f mm"),theApp.m_fXScanOffset);
	m_editX.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_fYScanOffset);
	m_editY.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_fZScanOffset);
	m_editZ.SetWindowText(Buff);

	m_spinXTipLeft.SetPos((int)m_CpHead.Side0.fXt + 180);
	m_spinYTipLeft.SetPos((int)m_CpHead.Side0.fYt + 180);
	m_spinXTRight.SetPos((int)m_CpHead.Side1.fXt + 180);
	m_spinYTRight.SetPos((int)m_CpHead.Side1.fYt + 180);

	m_spinILeft.SetPos((int)(m_CpHead.Side0.fI * 100.0f) + 100);

	Buff.Format(L"%.01f%s",theApp.m_fRotateAboutZDuringScan,DEGREES);
	m_editRotateAboutZ.SetWindowText(Buff);


}

void CIJKPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	switch(theApp.m_LastSettings.nIJKDiagCombo) {
	case 1:
		theApp.m_Motors.GetHeadPos(&m_CpHead);
		theApp.m_Kinematics.NormalsFromTips(&m_CpHead,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpHead,STARBOARD);
		break;
	case 0:
		if(m_nEditTipsNormals == 0) {
			theApp.m_Kinematics.NormalsFromTips(&m_CpHead,PORTSIDE);
			theApp.m_Kinematics.NormalsFromTips(&m_CpHead,STARBOARD);
		}
		if(m_nEditTipsNormals == 1) {
//			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE);
//			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD);
		}
		break;
	case 2:
		theApp.m_CopyLine.GetCoord(0,&m_CpHead);
		theApp.m_Kinematics.NormalsFromTips(&m_CpHead,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpHead,STARBOARD);
		break;
	}


	if(m_CpHead.Side0.fXt - m_CpOldHead.Side0.fXt) {
		Buff.Format(_T("%.03f"),m_CpHead.Side0.fXt);
		m_editLeftXt.SetWindowText(Buff);
	}
	if(m_CpHead.Side0.fYt - m_CpOldHead.Side0.fYt) {
		Buff.Format(_T("%.03f"),m_CpHead.Side0.fYt);
		m_editLeftYt.SetWindowText(Buff);
	}
	if(m_CpHead.Side0.fI - m_CpOldHead.Side0.fI) {
		Buff.Format(_T("%.03f"),m_CpHead.Side0.fI);
		m_editLeftI.SetWindowText(Buff);
	}
	if(m_CpHead.Side0.fJ - m_CpOldHead.Side0.fJ) {
		Buff.Format(_T("%.03f"),m_CpHead.Side0.fJ);
		m_editLeftJ.SetWindowText(Buff);
	}
	if(m_CpHead.Side0.fK - m_CpOldHead.Side0.fK) {
		Buff.Format(_T("%.03f"),m_CpHead.Side0.fK);
		m_editLeftK.SetWindowText(Buff);
	}
	if(m_CpHead.Side0.fW - m_CpOldHead.Side0.fW) {
		Buff.Format(_T("%.03f"),m_CpHead.Side0.fW);
		m_editLeftW.SetWindowText(Buff);
	}

	if(m_CpHead.Side1.fXt - m_CpOldHead.Side1.fXt) {
		Buff.Format(_T("%.03f"),m_CpHead.Side1.fXt);
		m_editRightXt.SetWindowText(Buff);
	}
	if(m_CpHead.Side1.fYt - m_CpOldHead.Side1.fYt) {
		Buff.Format(_T("%.03f"),m_CpHead.Side1.fYt);
		m_editRightYt.SetWindowText(Buff);
	}
	if(m_CpHead.Side1.fI - m_CpOldHead.Side1.fI) {
		Buff.Format(_T("%.03f"),m_CpHead.Side1.fI);
		m_editRightI.SetWindowText(Buff);
	}
	if(m_CpHead.Side1.fJ - m_CpOldHead.Side1.fJ) {
		Buff.Format(_T("%.03f"),m_CpHead.Side1.fJ);
		m_editRightJ.SetWindowText(Buff);
	}
	if(m_CpHead.Side1.fK - m_CpOldHead.Side1.fK) {
		Buff.Format(_T("%.03f"),m_CpHead.Side1.fK);
		m_editRightK.SetWindowText(Buff);
	}
	if(m_CpHead.Side1.fW - m_CpOldHead.Side1.fW) {
		Buff.Format(_T("%.03f"),m_CpHead.Side1.fW);
		m_editRightW.SetWindowText(Buff);
	}
	m_CpSurface = m_CpHead;
	theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,PORTSIDE,TRUE,TRUE);
	theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,STARBOARD,TRUE,TRUE);

	theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE,theApp.m_fRotateThetaRevNumber[PORTSIDE]);
	theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD,theApp.m_fRotateThetaRevNumber[STARBOARD]);
	if(m_CpSurface.Side0.fXt - m_CpOldSurface.Side0.fXt) {
		Buff.Format(_T("%.03f"),m_CpSurface.Side0.fXt);
		m_editLeftXTip2.SetWindowText(Buff);
	}
	if(m_CpSurface.Side0.fYt - m_CpOldSurface.Side0.fYt) {
		Buff.Format(_T("%.03f"),m_CpSurface.Side0.fYt);
		m_editLeftYTip2.SetWindowText(Buff);
	}

	if(m_CpSurface.Side1.fXt - m_CpOldSurface.Side1.fXt) {
		Buff.Format(_T("%.03f"),m_CpSurface.Side1.fXt);
		m_editRightXTip2.SetWindowText(Buff);
	}
	if(m_CpSurface.Side1.fYt - m_CpOldSurface.Side1.fYt) {
		Buff.Format(_T("%.03f"),m_CpSurface.Side1.fYt);
		m_editRightYTip2.SetWindowText(Buff);
	}

	m_CpOldHead = m_CpHead ;
	m_CpOldSurface = m_CpSurface;

	CPropertyPage::OnTimer(nIDEvent);
}

void CIJKPage::OnChangeEditX() 
{
	CString Buff;

	m_editX.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_fXScanOffset);
}

void CIJKPage::OnChangeEditY() 
{
	CString Buff;

	m_editY.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_fYScanOffset);
}

void CIJKPage::OnChangeEditZ() 
{
	CString Buff;

	m_editZ.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_fZScanOffset);
}

BOOL CIJKPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();

	SetTimer(1,200,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CIJKPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CIJKPage::OnChangeEditLeftXt() 
{
	CString Buff;
	float fTemp;

	m_editLeftXt.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos = (int)(fTemp / theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);

}

void CIJKPage::OnChangeEditLeftYt() 
{
	CString Buff;
	float fTemp;

	m_editLeftYt.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos = (int)(fTemp / theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
	
}

void CIJKPage::OnSelchangeComboVirtualActual() 
{
	theApp.m_LastSettings.nVirtualActualRobot = m_comboVirtualActual.GetCurSel() & 0x01;
	theApp.m_LastSettings.nIJKDiagCombo = m_comboVirtualActual.GetCurSel();
	UpdateAllControls();
}


void CIJKPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_XT_LEFT:
		m_nEditTipsNormals=0;
		fTemp=m_CpHead.Side0.fXt;
		m_CpHead.Side0.fXt=(float)(pSpin->GetPos()&0xffff)-180.0f;
		if(fTemp-m_CpHead.Side0.fXt) {
			UpdateAllControls();
		}

		break;
	case IDC_SPIN_YT_LEFT:
		m_nEditTipsNormals=0;
		fTemp=m_CpHead.Side0.fYt;
		m_CpHead.Side0.fYt=(float)(pSpin->GetPos()&0xffff)-180.0f;
		if(fTemp-m_CpHead.Side0.fYt) {
			UpdateAllControls();
		}

		break;
	case IDC_SPIN_XT_RIGHT:
		m_nEditTipsNormals=0;
		fTemp=m_CpHead.Side1.fXt;
		m_CpHead.Side1.fXt=(float)(pSpin->GetPos()&0xffff)-180.0f;
		if(fTemp-m_CpHead.Side1.fXt) {
			UpdateAllControls();
		}

		break;
	case IDC_SPIN_YT_RIGHT:
		m_nEditTipsNormals=0;
		fTemp=m_CpHead.Side1.fYt;
		m_CpHead.Side1.fYt=(float)(pSpin->GetPos()&0xffff)-180.0f;
		if(fTemp-m_CpHead.Side1.fYt) {
			UpdateAllControls();
		}

		break;
	case IDC_SPIN_I_LEFT:
		m_nEditTipsNormals=1;
		fTemp=m_CpHead.Side0.fI;
		m_CpHead.Side0.fI=(float)((pSpin->GetPos()&0xffff)-100) / 100.0f;
		if(fTemp-m_CpHead.Side0.fI) {
			m_CpSurface.Side0.fI = m_CpHead.Side0.fI;
			m_CpSurface.Side0.fJ = m_CpHead.Side0.fJ = -cosf(asinf(m_CpHead.Side0.fI));
			UpdateAllControls();
		}

		break;
	case IDC_SPIN_J_LEFT:
		m_nEditTipsNormals=1;
		fTemp=m_CpHead.Side0.fJ;
		m_CpHead.Side0.fJ=(float)((pSpin->GetPos()&0xffff)-100) / 100.0f;
		if(fTemp-m_CpHead.Side0.fJ) {
			m_CpSurface.Side0.fJ = m_CpHead.Side0.fJ;
			m_CpSurface.Side0.fI = m_CpHead.Side0.fI = -cosf(asinf(m_CpHead.Side0.fJ));
			UpdateAllControls();
		}

		break;
	};


	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CIJKPage::OnChangeEditRotateAboutZ() 
{
	CString Buff;

	m_editRotateAboutZ.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_fRotateAboutZDuringScan);
}

void CIJKPage::OnDeltaposSpinRotateAboutZ(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_fRotateAboutZDuringScan += ((float)pNMUpDown->iDelta * 0.1f);
	UpdateAllControls();

	*pResult = 0;
}
