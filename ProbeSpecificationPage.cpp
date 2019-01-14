// ProbeSpecificationPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeSpecificationPage.h"
#include "ProbeCharacterisationSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeSpecificationPage property page

IMPLEMENT_DYNCREATE(CProbeSpecificationPage, CResizablePage)

CProbeSpecificationPage::CProbeSpecificationPage(CProbe* pProbe) : CResizablePage(CProbeSpecificationPage::IDD)
{
	//{{AFX_DATA_INIT(CProbeSpecificationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pProbe = pProbe;
}

CProbeSpecificationPage::~CProbeSpecificationPage()
{
}

void CProbeSpecificationPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeSpecificationPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeSpecificationPage, CResizablePage)
	//{{AFX_MSG_MAP(CProbeSpecificationPage)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, OnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, OnChangeEdit9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeSpecificationPage message handlers

BOOL CProbeSpecificationPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CStatic* pStatic;


	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC1)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(0));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC2)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(1));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC3)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(2));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC4)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(3));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC5)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(4));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC6)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(5));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC7)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(6));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC8)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(7));
	if(pStatic = (CStatic*)GetDlgItem(IDC_STATIC9)) pStatic->SetWindowText(m_pProbe->m_Fields.GetAt(8));

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProbeSpecificationPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CEdit* pEdit;

	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT1)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(0));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT2)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(1));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT3)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(2));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT4)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(3));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT5)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(4));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT6)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(5));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT7)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(6));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT8)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(7));
	if(pEdit = (CEdit*)GetDlgItem(IDC_EDIT9)) pEdit->SetWindowText(m_pProbe->m_strValues.GetAt(8));
}

void CProbeSpecificationPage::OnChangeEdit1() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(0) != Buff) {
		m_pProbe->m_strValues.SetAt(0,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit2() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(1) != Buff) {
		m_pProbe->m_strValues.SetAt(1,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit3() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(2) != Buff) {
		m_pProbe->m_strValues.SetAt(2,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit4() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT4);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(3) != Buff) {
		m_pProbe->m_strValues.SetAt(3,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit5() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT5);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(4) != Buff) {
		m_pProbe->m_strValues.SetAt(4,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit6() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT6);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(5) != Buff) {
		m_pProbe->m_strValues.SetAt(5,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit7() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT7);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(6) != Buff) {
		m_pProbe->m_strValues.SetAt(6,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit8() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT8);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(7) != Buff) {
		m_pProbe->m_strValues.SetAt(7,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

void CProbeSpecificationPage::OnChangeEdit9() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT9);
	CString Buff;
	
	pEdit->GetWindowText(Buff);
	if(m_pProbe->m_strValues.GetAt(8) != Buff) {
		m_pProbe->m_strValues.SetAt(8,Buff);
		m_pProbe->m_bModified = TRUE;
	}
}

BOOL CProbeSpecificationPage::OnSetActive() 
{
	((CProbeCharacterisationSheet*)GetParent())->SetToolBarCheckedState();	//SetToolBarCheckedState();;	
	((CProbeCharacterisationSheet*)GetParent())->m_pCurrentPage = (CPropertyPage*)this;
	
	return CResizablePage::OnSetActive();
}

