// DetailsEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DetailsEditDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetailsEditDlg dialog


CDetailsEditDlg::CDetailsEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CDetailsEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsEditDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
	m_nLeft = nLeft;
	m_nTop = nTop;

}


void CDetailsEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsEditDlg)
	DDX_Control(pDX, IDC_EDIT_UT_FILE, m_editUtFile);
	DDX_Control(pDX, IDC_COMBO_DESCRIPTION, m_comboDescription);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_SPIN_FAST_INCREMENT, m_spinFastIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_FASY_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailsEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_DESCRIPTION, OnChangeEditDescription)
	ON_EN_CHANGE(IDC_EDIT_FASY_INCREMENT, OnChangeEditFasyIncrement)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FAST_INCREMENT, OnDeltaposSpinFastIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_CBN_SELCHANGE(IDC_COMBO_DESCRIPTION, OnSelchangeComboDescription)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_UT_FILE, OnChangeEditUtFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsEditDlg message handlers

void CDetailsEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CDetailsEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CDetailsEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CDetailsEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);
	
	m_spinFastIncrement.SetRange(0,200);
	m_spinSlowIncrement.SetRange(0,200);

	Buff.LoadString(IDS_Rectilinear);	m_comboDescription.AddString(Buff);
	Buff.LoadString(IDS_Open_Rotary);	m_comboDescription.AddString(Buff);
	Buff.LoadString(IDS_Closed_Rotary);	m_comboDescription.AddString(Buff);

	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetailsEditDlg::UpdateAllControls()
{
	CString Buff;

	m_editDescription.SetWindowText(PROFILE->m_strDescription);
	Buff.Format(_T("%.*f %s"),theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fFastIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editFastIncrement.SetWindowText(Buff);
	Buff.Format(_T("%.*f %s"),theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	m_comboDescription.SetCurSel(PROFILE->m_nComponentDescription);
	m_editUtFile.SetWindowText(PROFILE->m_UltrasonicPathFileName);


}

void CDetailsEditDlg::OnChangeEditDescription() 
{
	m_editDescription.GetWindowText(PROFILE->m_strDescription);
	theApp.m_Data.m_ProfileDescription = PROFILE->m_strDescription;

}

void CDetailsEditDlg::OnChangeEditFasyIncrement() 
{
	CString Buff;
	float fTemp;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fFastIncrement - fTemp) {
		PROFILE->m_fFastIncrement = fTemp;
//		UpdateAllControls();
	}
	
}

void CDetailsEditDlg::OnChangeEditSlowIncrement() 
{
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
//		UpdateAllControls();
		UpdateMainFrameTools();
	}
	
}


void CDetailsEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);

}


void CDetailsEditDlg::OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	float fStep = 1.0f / pow(10.0f, (float)theApp.m_LastSettings.nStepSizePrecision);

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * fStep);
	} else {
		fStep *= 25.4f;
		PROFILE->m_fFastIncrement += ((float)pNMUpDown->iDelta * fStep);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CDetailsEditDlg::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	float fStep = 1.0f / pow(10.0f, (float)theApp.m_LastSettings.nStepSizePrecision);

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	} else {
		fStep *= 25.4f;
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	}
	UpdateAllControls();
	UpdateMainFrameTools();
	
	*pResult = 0;
}

void CDetailsEditDlg::UpdateMainFrameTools()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(	pFrame->m_pMotionToolsSheet ) {
		pFrame->m_pMotionToolsSheet->UpdateAllPages();
	}

}
void CDetailsEditDlg::OnSelchangeComboDescription() 
{
	PROFILE->m_nComponentDescription = (ComponentDescription)m_comboDescription.GetCurSel();
}

void CDetailsEditDlg::OnButtonBrowse() 
{
	CFileDialog FileDlg(TRUE,_T(".ul"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Ultrasonics (*.ul)|*.ul|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_UltrasonicsPath;
	if(FileDlg.DoModal()==IDOK) {
		PROFILE->m_UltrasonicPathFileName=FileDlg.GetPathName();
		m_editUtFile.SetWindowText(PROFILE->m_UltrasonicPathFileName);
	}
}

void CDetailsEditDlg::OnChangeEditUtFile() 
{
}
