// InclusionsDetailsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "InclusionsDetailsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInclusionsDetailsPage property page

IMPLEMENT_DYNCREATE(CInclusionsDetailsPage, CPropertyPage)

CInclusionsDetailsPage::CInclusionsDetailsPage(CInclusions* pInclusions) : CPropertyPage(CInclusionsDetailsPage::IDD)
{
	//{{AFX_DATA_INIT(CInclusionsDetailsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pInclusions = pInclusions;
	

}

CInclusionsDetailsPage::~CInclusionsDetailsPage()
{
}

void CInclusionsDetailsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInclusionsDetailsPage)
	DDX_Control(pDX, IDC_EDIT_CAST, m_editCast);
	DDX_Control(pDX, IDC_EDIT_GRADE, m_editGrade);
	DDX_Control(pDX, IDC_EDIT_ORDER_NUMBER, m_editOrderNumber);
	DDX_Control(pDX, IDC_EDIT_CUSTOMER, m_editCustomer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInclusionsDetailsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CInclusionsDetailsPage)
	ON_EN_CHANGE(IDC_EDIT_CUSTOMER, OnChangeEditCustomer)
	ON_EN_CHANGE(IDC_EDIT_CAST, OnChangeEditCast)
	ON_EN_CHANGE(IDC_EDIT_GRADE, OnChangeEditGrade)
	ON_EN_CHANGE(IDC_EDIT_ORDER_NUMBER, OnChangeEditOrderNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInclusionsDetailsPage message handlers

BOOL CInclusionsDetailsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInclusionsDetailsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_editCustomer.SetWindowText(m_pInclusions->m_Customer);
	m_editCast.SetWindowText(m_pInclusions->m_Cast);
	m_editGrade.SetWindowText(m_pInclusions->m_Grade);
	m_editOrderNumber.SetWindowText(m_pInclusions->m_OrderNumber);
}

void CInclusionsDetailsPage::OnChangeEditCustomer() 
{
	
	m_editCustomer.GetWindowText(m_pInclusions->m_Customer);

}


void CInclusionsDetailsPage::OnChangeEditCast() 
{
	m_editCast.GetWindowText(m_pInclusions->m_Cast);
}

void CInclusionsDetailsPage::OnChangeEditGrade() 
{
	m_editGrade.GetWindowText(m_pInclusions->m_Grade);
}

void CInclusionsDetailsPage::OnChangeEditOrderNumber() 
{
	m_editOrderNumber.GetWindowText(m_pInclusions->m_OrderNumber);
}
