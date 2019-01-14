// TranslationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TranslationDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "3DViewEditDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CTranslationDlg dialog


CTranslationDlg::CTranslationDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CTranslationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTranslationDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
	m_nLeft = nLeft;
	m_nTop = nTop;
}


void CTranslationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTranslationDlg)
	DDX_Control(pDX, IDC_SPIN_R_ROTATION, m_spinRRotation);
	DDX_Control(pDX, IDC_EDIT_R_ROTATION, m_editRRotation);
	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_comboOrientation);
	DDX_Control(pDX, IDC_COMBO_SECONDARY_AXIS, m_comboSecondaryAxis);
	DDX_Control(pDX, IDC_COMBO_PRIMARY_AXIS, m_comboPrimaryAxis);
	DDX_Control(pDX, IDC_SPIN_Z_TRANSLATION, m_spinZTranslation);
	DDX_Control(pDX, IDC_SPIN_Z_ROTATION, m_spinZRotation);
	DDX_Control(pDX, IDC_SPIN_Y_TRANSLATION, m_spinYTranslation);
	DDX_Control(pDX, IDC_SPIN_Y_ROTATION, m_spinYRotation);
	DDX_Control(pDX, IDC_SPIN_X_TRANSLATION, m_spinXTranslation);
	DDX_Control(pDX, IDC_SPIN_X_ROTATION, m_spinXRotation);
	DDX_Control(pDX, IDC_EDIT_Z_TRANSLATION, m_editZTranslation);
	DDX_Control(pDX, IDC_EDIT_Z_ROTATION, m_editZRotation);
	DDX_Control(pDX, IDC_EDIT_Y_TRANSLATION, m_editYTranslation);
	DDX_Control(pDX, IDC_EDIT_Y_ROTATION, m_editYRotation);
	DDX_Control(pDX, IDC_EDIT_X_TRANSLATION, m_editXTranslation);
	DDX_Control(pDX, IDC_EDIT_X_ROTATION, m_editXRotation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTranslationDlg, CDialog)
	//{{AFX_MSG_MAP(CTranslationDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_X_ROTATION, OnChangeEditXRotation)
	ON_EN_CHANGE(IDC_EDIT_Y_ROTATION, OnChangeEditYRotation)
	ON_EN_CHANGE(IDC_EDIT_Z_ROTATION, OnChangeEditZRotation)
	ON_EN_CHANGE(IDC_EDIT_X_TRANSLATION, OnChangeEditXTranslation)
	ON_EN_CHANGE(IDC_EDIT_Y_TRANSLATION, OnChangeEditYTranslation)
	ON_EN_CHANGE(IDC_EDIT_Z_TRANSLATION, OnChangeEditZTranslation)
	ON_BN_CLICKED(IDC_BUTTON_ENGAGE, OnButtonEngage)
	ON_CBN_SELCHANGE(IDC_COMBO_PRIMARY_AXIS, OnSelchangeComboPrimaryAxis)
	ON_CBN_SELCHANGE(IDC_COMBO_SECONDARY_AXIS, OnSelchangeComboSecondaryAxis)
	ON_CBN_SELCHANGE(IDC_COMBO_ORIENTATION, OnSelchangeComboOrientation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_X_TRANSLATION, OnDeltaposSpinXTranslation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_TRANSLATION, OnDeltaposSpinYTranslation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z_TRANSLATION, OnDeltaposSpinZTranslation)
	ON_EN_CHANGE(IDC_EDIT_R_ROTATION, OnChangeEditRRotation)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTranslationDlg message handlers

void CTranslationDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CTranslationDlg::OnClose() 
{
		
	CDialog::OnClose();
	DestroyWindow();}

void CTranslationDlg::OnDestroy() 
{

	CDialog::OnDestroy();
	*m_pDlg = NULL;			
}

BOOL CTranslationDlg::OnInitDialog() 
{
	CString	Buff;
	CDialog::OnInitDialog();
	
	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	m_spinXTranslation.SetRange(0,20000);
	m_spinYTranslation.SetRange(0,20000);
	m_spinZTranslation.SetRange(0,20000);
	m_spinXRotation.SetRange(0,7200);
	m_spinYRotation.SetRange(0,7200);
	m_spinZRotation.SetRange(0,7200);
	m_spinRRotation.SetRange(0,7200);

	m_comboPrimaryAxis.AddString(_T("X"));
	m_comboPrimaryAxis.AddString(_T("Y"));
	m_comboPrimaryAxis.AddString(_T("Z"));
	m_comboPrimaryAxis.AddString(_T("R"));
	m_comboSecondaryAxis.AddString(_T("X"));
	m_comboSecondaryAxis.AddString(_T("Y"));
	m_comboSecondaryAxis.AddString(_T("Z"));
	m_comboSecondaryAxis.AddString(_T("R"));

	m_comboOrientation.AddString(_T("Bottom Left"));
	m_comboOrientation.AddString(_T("Top Left"));
	m_comboOrientation.AddString(_T("Bottom right"));
	m_comboOrientation.AddString(_T("Top Right"));

	UpdateAllControls();



	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTranslationDlg::UpdateAllControls()
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*) m_pParent;
	CString	Buff;

	m_spinXRotation.SetPos((int)(PROFILE->m_fRotateX*10.0f)+3600);
	m_spinYRotation.SetPos((int)(PROFILE->m_fRotateY*10.0f)+3600);
	m_spinZRotation.SetPos((int)(PROFILE->m_fRotateZ*10.0f)+3600);
	m_spinRRotation.SetPos((int)(PROFILE->m_fRotateR*10.0f)+3600);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fTranslateX*theApp.m_fUnits,theApp.m_Units);
	m_editXTranslation.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fTranslateY*theApp.m_fUnits,theApp.m_Units);
	m_editYTranslation.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fTranslateZ*theApp.m_fUnits,theApp.m_Units);
	m_editZTranslation.SetWindowText(Buff);
	Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateX,theApp.m_Degrees);
	m_editXRotation.SetWindowText(Buff);
	Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateY,theApp.m_Degrees);
	m_editYRotation.SetWindowText(Buff);
	Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateZ,theApp.m_Degrees);
	m_editZRotation.SetWindowText(Buff);
	Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateR,theApp.m_Degrees);
	m_editRRotation.SetWindowText(Buff);

	m_comboPrimaryAxis.SetCurSel(PROFILE->m_nFastAxis);
	m_comboSecondaryAxis.SetCurSel(PROFILE->m_nSlowAxis);

	m_comboOrientation.SetCurSel(theApp.m_LastSettings.nOrientation3DEditImage);

}


void CTranslationDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	C3DViewEditDlg* pParent = (C3DViewEditDlg*) m_pParent;

	float fRotate=(float)(pSpin->GetPos()&0xffff);
	fRotate -= 3600.0f;
	fRotate /= 10.0;
	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_X_ROTATION:
		if(fRotate - PROFILE->m_fRotateX) {
			PROFILE->m_fRotateX = fRotate;
			Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateX,theApp.m_Degrees);
			m_editXRotation.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_Y_ROTATION:
		if(fRotate - PROFILE->m_fRotateY) {
			PROFILE->m_fRotateY = fRotate;
			Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateY,theApp.m_Degrees);
			m_editYRotation.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_Z_ROTATION:
		if(fRotate - PROFILE->m_fRotateZ) {
			PROFILE->m_fRotateZ = fRotate;
			Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateZ,theApp.m_Degrees);
			m_editZRotation.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_R_ROTATION:
		if(fRotate - PROFILE->m_fRotateR) {
			PROFILE->m_fRotateR = fRotate;
			Buff.Format(_T("%.01f%s"),PROFILE->m_fRotateR,theApp.m_Degrees);
			m_editRRotation.SetWindowText(Buff);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}




void CTranslationDlg::OnChangeEditXRotation() 
{
	CString	Buff;

	m_editXRotation.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fRotateX);
	m_spinXRotation.SetPos((int)(PROFILE->m_fRotateX*10.0f)+3600);

}

void CTranslationDlg::OnChangeEditYRotation() 
{
	CString	Buff;

	m_editYRotation.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fRotateY);
	m_spinYRotation.SetPos((int)(PROFILE->m_fRotateY*10.0f)+3600);
	
}

void CTranslationDlg::OnChangeEditZRotation() 
{
	CString	Buff;

	m_editZRotation.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fRotateZ);
	m_spinZRotation.SetPos((int)(PROFILE->m_fRotateZ*10.0f)+3600);
	
}

void CTranslationDlg::OnChangeEditRRotation() 
{
	CString	Buff;
	C3DViewEditDlg* pParent = (C3DViewEditDlg*) m_pParent;

	m_editRRotation.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fRotateR);
	m_spinRRotation.SetPos((int)(PROFILE->m_fRotateR*10.0f)+3600);
	
}

void CTranslationDlg::OnChangeEditXTranslation() 
{
	float fTemp;
	CString	Buff;

	m_editXTranslation.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fTranslateX - fTemp) {
		PROFILE->m_fTranslateX = fTemp;
	}
}

void CTranslationDlg::OnChangeEditYTranslation() 
{
	float fTemp;
	CString	Buff;

	m_editYTranslation.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fTranslateY - fTemp) {
		PROFILE->m_fTranslateY = fTemp;
	}
}

void CTranslationDlg::OnChangeEditZTranslation() 
{
	float fTemp;
	CString	Buff;

	m_editZTranslation.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fTranslateZ - fTemp) {
		PROFILE->m_fTranslateZ = fTemp;
	}
}


void CTranslationDlg::OnButtonEngage() 
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*) m_pParent;
	D3DXMATRIXA16 matWorld,matX,matY,matZ,matTranslation;

	D3DXMatrixIdentity( &matWorld);
	D3DXMatrixRotationX(&matX,PROFILE->m_fRotateX * DEG_TO_RAD);
	D3DXMatrixRotationY(&matY,PROFILE->m_fRotateY * DEG_TO_RAD);
	D3DXMatrixRotationZ(&matZ,PROFILE->m_fRotateZ * DEG_TO_RAD);
	D3DXMatrixTranslation(&matTranslation,PROFILE->m_fTranslateX,PROFILE->m_fTranslateY,PROFILE->m_fTranslateZ);
	D3DXMatrixMultiply( &matWorld, &matWorld, &matX );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matY );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matZ );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTranslation );

	PROFILE->ComponentMove(matWorld);

	PROFILE->AddRRotation(PROFILE->m_fRotateR);

	pParent->m_CompBall.Init(TRUE);

	UpdateAllControls();

	UpdateParent(FILLLIST);
	UpdateParent(INVALIDATE);
}



void CTranslationDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) {
		pParent->RefreshView();
	}

}


void CTranslationDlg::OnSelchangeComboPrimaryAxis() 
{
	PROFILE->m_nFastAxis = m_comboPrimaryAxis.GetCurSel();
	PROFILE->CalculateDangerZone();

	UpdateParent(INVALIDATE);
}

void CTranslationDlg::OnSelchangeComboSecondaryAxis() 
{
	PROFILE->m_nSlowAxis = m_comboSecondaryAxis.GetCurSel();
	PROFILE->CalculateDangerZone();

	UpdateParent(INVALIDATE);
}

void CTranslationDlg::OnSelchangeComboOrientation() 
{
	theApp.m_LastSettings.nOrientation3DEditImage = m_comboOrientation.GetCurSel();
	UpdateParent(INVALIDATE);
}

void CTranslationDlg::OnDeltaposSpinXTranslation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fTranslateX += ((float)pNMUpDown->iDelta * 1.0f);
	} else {
		PROFILE->m_fTranslateX += ((float)pNMUpDown->iDelta * 2.54f);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CTranslationDlg::OnDeltaposSpinYTranslation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fTranslateY += ((float)pNMUpDown->iDelta * 1.0f);
	} else {
		PROFILE->m_fTranslateY += ((float)pNMUpDown->iDelta * 2.54f);
	}
	UpdateAllControls();
	*pResult = 0;
}

void CTranslationDlg::OnDeltaposSpinZTranslation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fTranslateZ += ((float)pNMUpDown->iDelta * 1.0f);
	} else {
		PROFILE->m_fTranslateZ += ((float)pNMUpDown->iDelta * 2.54f);
	}
	UpdateAllControls();
	*pResult = 0;
}


void CTranslationDlg::OnButtonZero() 
{
	PROFILE->m_fTranslateZ = PROFILE->m_fTranslateY = PROFILE->m_fTranslateX = 0.0f;
	PROFILE->m_fRotateZ = PROFILE->m_fRotateY = PROFILE->m_fRotateX = PROFILE->m_fRotateR = 0.0f;
	UpdateAllControls();
}
