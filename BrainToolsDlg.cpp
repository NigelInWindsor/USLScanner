// BrainToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BrainToolsDlg.h"
#include "BrainEditDlg.h"
#include "3DViewEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrainToolsDlg dialog


CBrainToolsDlg::CBrainToolsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CBrainToolsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrainToolsDlg)
	//}}AFX_DATA_INIT
	
	m_pDlg = pDlg;
}


void CBrainToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrainToolsDlg)
//	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_comboOrientation);
	DDX_Control(pDX, IDC_STATIC_SLOW, m_staticSlow);
	DDX_Control(pDX, IDC_STATIC_FAST, m_staticFast);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_EDIT_TOP_MARGIN, m_editTopMargin);
	DDX_Control(pDX, IDC_EDIT_RIGHT_MARGIN, m_editRightMargin);
	DDX_Control(pDX, IDC_EDIT_LEFT_MARGIN, m_editLeftMargin);
	DDX_Control(pDX, IDC_EDIT_BOTTOM_MARGIN, m_editBottomMargin);
	DDX_Control(pDX, IDC_EDIT_MESH_SLOW_SIZE, m_editMeshSlowSize);
	DDX_Control(pDX, IDC_EDIT_MESH_FAST_SIZE, m_editMeshFastSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrainToolsDlg, CDialog)
	//{{AFX_MSG_MAP(CBrainToolsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MESH, OnButtonMesh)
	ON_EN_CHANGE(IDC_EDIT_MESH_SLOW_SIZE, OnChangeEditMeshSlowSize)
	ON_EN_CHANGE(IDC_EDIT_MESH_FAST_SIZE, OnChangeEditMeshFastSize)
	ON_EN_CHANGE(IDC_EDIT_TOP_MARGIN, OnChangeEditTopMargin)
	ON_EN_CHANGE(IDC_EDIT_BOTTOM_MARGIN, OnChangeEditBottomMargin)
	ON_EN_CHANGE(IDC_EDIT_LEFT_MARGIN, OnChangeEditLeftMargin)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_MARGIN, OnChangeEditRightMargin)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_CBN_SELCHANGE(IDC_COMBO_ORIENTATION, OnSelchangeComboOrientation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrainToolsDlg message handlers

void CBrainToolsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CBrainToolsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CBrainToolsDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();}

void CBrainToolsDlg::OnDestroy() 
{
	if (CBrainToolsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CBrainToolsDlg::IDD]);
	}

	CDialog::OnDestroy();
	*m_pDlg = NULL;			
	
}

BOOL CBrainToolsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	if (CBrainToolsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CBrainToolsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	Buff.LoadString(IDS_Size); m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Row_Column); m_comboMode.AddString(Buff);

//	Buff.LoadString(IDS_Horizontal);	m_comboOrientation.AddString(Buff);
//	Buff.LoadString(IDS_Vertical);		m_comboOrientation.AddString(Buff);

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrainToolsDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fMeshTopMargin);
	m_editTopMargin.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fMeshBottomMargin);
	m_editBottomMargin.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fMeshLeftMargin);
	m_editLeftMargin.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fMeshRightMargin);
	m_editRightMargin.SetWindowText(Buff);

	m_comboMode.SetCurSel(theApp.m_LastSettings.nBrainMeshMode);

	switch(theApp.m_LastSettings.nBrainMeshMode) {
	case 0: Buff.LoadString(IDS_Fast_Size); m_staticFast.SetWindowText(Buff);
		Buff.LoadString(IDS_Slow_Size); m_staticSlow.SetWindowText(Buff);
		Buff.Format(_T("%.0f mm"),theApp.m_LastSettings.fMeshFastSize);
		m_editMeshFastSize.SetWindowText(Buff);
		Buff.Format(_T("%.0f mm"),theApp.m_LastSettings.fMeshSlowSize);
		m_editMeshSlowSize.SetWindowText(Buff);
		break;
	case 1: Buff.LoadString(IDS_Columns); m_staticFast.SetWindowText(Buff);
		Buff.LoadString(IDS_Rows); m_staticSlow.SetWindowText(Buff);
		Buff.Format(_T("%.0f"),theApp.m_LastSettings.fMeshFastSize);
		m_editMeshFastSize.SetWindowText(Buff);
		Buff.Format(_T("%.0f"),theApp.m_LastSettings.fMeshSlowSize);
		m_editMeshSlowSize.SetWindowText(Buff);
		break;
	}
//	m_comboOrientation.SetCurSel(PROFILE->m_nBraneStyle);

}		


void CBrainToolsDlg::OnChangeEditMeshSlowSize() 
{
	CString Buff;

	m_editMeshSlowSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshSlowSize);
}

void CBrainToolsDlg::OnChangeEditMeshFastSize() 
{
	CString Buff;

	m_editMeshFastSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshFastSize);
}

void CBrainToolsDlg::OnButtonMesh() 
{
	CBrainEditDlg* pParent = (CBrainEditDlg*)GetParent();

	PROFILE->m_nBraneStyle = 0;
	PROFILE->GenerateMesh();
	pParent->UpdateParent(FILLLIST|INVALIDATE);
	
}



void CBrainToolsDlg::OnChangeEditTopMargin() 
{
	CString Buff;

	m_editTopMargin.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshTopMargin);
	
}

void CBrainToolsDlg::OnChangeEditBottomMargin() 
{
	CString Buff;

	m_editBottomMargin.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshBottomMargin);
	
}

void CBrainToolsDlg::OnChangeEditLeftMargin() 
{
	CString Buff;

	m_editLeftMargin.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshLeftMargin);
	
}

void CBrainToolsDlg::OnChangeEditRightMargin() 
{
	CString Buff;

	m_editRightMargin.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fMeshRightMargin);
	
}

void CBrainToolsDlg::OnSelchangeComboMode() 
{

	theApp.m_LastSettings.nBrainMeshMode = m_comboMode.GetCurSel();
	UpdateAllControls();
}

void CBrainToolsDlg::OnSelchangeComboOrientation() 
{
	PROFILE->m_nBraneStyle = m_comboOrientation.GetCurSel();
}
