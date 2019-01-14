// HolesEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "HolesEditDlg.h"
#include "3DViewEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04

/////////////////////////////////////////////////////////////////////////////
// CHolesEditDlg dialog


CHolesEditDlg::CHolesEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop)
	: CDialog(CHolesEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHolesEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pExclusionZonesDlg = NULL;

}


void CHolesEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHolesEditDlg)
	DDX_Control(pDX, IDC_CHECK_VECTOR_LENGTHS, m_checkVectorLengths);
	DDX_Control(pDX, IDC_CHECK_LOCK_REF, m_checkLockRefPts);
	DDX_Control(pDX, IDC_EDIT_OVERALLHEIGHT, m_editOverallHeight);
	DDX_Control(pDX, IDC_EDIT_BALL_DIAMETER, m_editBallDiameter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHolesEditDlg, CDialog)
	//{{AFX_MSG_MAP(CHolesEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_BALL_DIAMETER, OnChangeEditBallDiameter)
	ON_EN_CHANGE(IDC_EDIT_OVERALLHEIGHT, OnChangeEditOverallheight)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, OnButtonCompute)
	ON_BN_CLICKED(IDC_BUTTON_EXCLUSION, OnButtonExclusion)
	ON_BN_CLICKED(IDC_BUTTON_INVERT_J, OnButtonInvertJ)
	ON_BN_CLICKED(IDC_CHECK_LOCK_REF, OnCheckLockRef)
	ON_BN_CLICKED(IDC_CHECK_VECTOR_LENGTHS, OnCheckVectorLengths)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHolesEditDlg message handlers

void CHolesEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CHolesEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CHolesEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CHolesEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHolesEditDlg::UpdateAllControls()
{
	CString	Buff;

	Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fBallDiameter);
	m_editBallDiameter.SetWindowText(Buff);
	Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fBallHeight);
	m_editOverallHeight.SetWindowText(Buff);

	m_checkLockRefPts.SetCheck(PROFILE->m_nMeasuredRefPtsLocked&1);
	m_checkVectorLengths.SetCheck(theApp.m_LastSettings.nRenderHoleRefLengths);


}

void CHolesEditDlg::OnChangeEditBallDiameter() 
{
	CString	Buff;

	m_editBallDiameter.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fBallDiameter);
	
}

void CHolesEditDlg::OnChangeEditOverallheight() 
{
	CString	Buff;

	m_editOverallHeight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fBallHeight);
	
}


void CHolesEditDlg::OnButtonCompute() 
{
	if(PROFILE->m_CpCadHole[0].nType || PROFILE->m_CpCadHole[1].nType || PROFILE->m_CpCadHole[2].nType) {
		if(theApp.m_LastSettings.fBallHeight < theApp.m_LastSettings.fBallDiameter) {
			MessageBox(L"The height must be greater than the diameter\nComputation failed",L"Error",MB_ICONERROR);
			return;
		}
	}

//	PROFILE->MountAtSurface();
	PROFILE->MountAtSurfaceD3DX();
	
	UpdateParent(INVALIDATE|FILLLIST);

}


void CHolesEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;

	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);

}

void CHolesEditDlg::OnButtonExclusion() 
{
	if (m_pExclusionZonesDlg == NULL) {
		m_pExclusionZonesDlg = new CExclusionZonesDlg(this, (CWnd**)&m_pExclusionZonesDlg);
		m_pExclusionZonesDlg->Create(CExclusionZonesDlg::IDD,this);
	} else {
		m_pExclusionZonesDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}

void CHolesEditDlg::OnButtonInvertJ() 
{
	PROFILE->ChangeSignOfAnElement('j');
	
	UpdateParent(INVALIDATE|FILLLIST);
}

void CHolesEditDlg::OnCheckLockRef() 
{
	PROFILE->m_nMeasuredRefPtsLocked = m_checkLockRefPts.GetCheck();
}

void CHolesEditDlg::OnCheckVectorLengths() 
{
	theApp.m_LastSettings.nRenderHoleRefLengths = m_checkVectorLengths.GetCheck();
	UpdateParent(INVALIDATE);
}
