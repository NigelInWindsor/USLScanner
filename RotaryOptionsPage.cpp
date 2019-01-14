// RotaryOptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "RotaryOptionsPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotaryOptionsPage property page

IMPLEMENT_DYNCREATE(CRotaryOptionsPage, CPropertyPage)

CRotaryOptionsPage::CRotaryOptionsPage(CWnd* pViewSheet,CData* pData) : CPropertyPage(CRotaryOptionsPage::IDD)
{
	//{{AFX_DATA_INIT(CRotaryOptionsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	m_pData = pData;
}

CRotaryOptionsPage::~CRotaryOptionsPage()
{
}

void CRotaryOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotaryOptionsPage)
	DDX_Control(pDX, IDC_EDIT_DIAMETER_1, m_editDiameter1);
	DDX_Control(pDX, IDC_EDIT_DIAMETER_0, m_editDiameter0);
	DDX_Control(pDX, IDC_EDIT_ANGLE_1, m_editAngle1);
	DDX_Control(pDX, IDC_EDIT_ANGLE_0, m_editAngle0);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_MODE, m_comboDisplayMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotaryOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRotaryOptionsPage)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, OnSelchangeComboDisplayMode)
	ON_EN_CHANGE(IDC_EDIT_ANGLE_0, OnChangeEditAngle0)
	ON_EN_CHANGE(IDC_EDIT_ANGLE_1, OnChangeEditAngle1)
	ON_EN_CHANGE(IDC_EDIT_DIAMETER_0, OnChangeEditDiameter0)
	ON_EN_CHANGE(IDC_EDIT_DIAMETER_1, OnChangeEditDiameter1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotaryOptionsPage message handlers

BOOL CRotaryOptionsPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

BOOL CRotaryOptionsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Rectilinear);	m_comboDisplayMode.AddString(Buff);
	Buff.LoadString(IDS_Polar);			m_comboDisplayMode.AddString(Buff);

	m_bDlgCreated = TRUE;
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotaryOptionsPage::UpdateAllControls()
{

	if(m_bDlgCreated != TRUE) return;
	CString Buff;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	Buff.Format(_T("%.1f mm"),m_pData->m_fComponentDiameter[0]);
	m_editDiameter0.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),m_pData->m_fComponentDiameter[1]);
	m_editDiameter1.SetWindowText(Buff);

	Buff.Format(_T("%.1f mm"),m_pData->m_CpStart.Side0.fR);
	m_editAngle0.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),m_pData->m_CpFinish.Side0.fR);
	m_editAngle1.SetWindowText(Buff);

	m_comboDisplayMode.SetCurSel(pSheet->m_nRectPolarDisplayMode);

}

void CRotaryOptionsPage::OnSelchangeComboDisplayMode() 
{
	// TODO: Add your control notification handler code here
	
}

void CRotaryOptionsPage::OnChangeEditAngle0() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CRotaryOptionsPage::OnChangeEditAngle1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CRotaryOptionsPage::OnChangeEditDiameter0() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CRotaryOptionsPage::OnChangeEditDiameter1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
