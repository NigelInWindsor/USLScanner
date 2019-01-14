// WorksheetData.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "WorksheetData.h"
#include "ProfileWorksheetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorksheetData dialog


CWorksheetData::CWorksheetData(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CWorksheetData::IDD, pParent)
{

	m_pDlg = pDlg;
	m_pParent = pParent;

	//{{AFX_DATA_INIT(CWorksheetData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWorksheetData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorksheetData)
	DDX_Control(pDX, IDC_EDIT_DATA, m_editData);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_comboItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorksheetData, CDialog)
	//{{AFX_MSG_MAP(CWorksheetData)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM, OnSelchangeComboItem)
	ON_EN_CHANGE(IDC_EDIT_DATA, OnChangeEditData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorksheetData message handlers

void CWorksheetData::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;		
}

void CWorksheetData::OnClose() 
{
	DestroyWindow();	
	CDialog::OnClose();
}

BOOL CWorksheetData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_comboItem.InsertString(0,"Project");
	m_comboItem.InsertString(1,"Product Number");
	m_comboItem.InsertString(2,"Product");

	m_comboItem.InsertString(3,"Holding Fixture");
	m_comboItem.InsertString(4,"Carriage 1 Line 1");
	m_comboItem.InsertString(5,"Carriage 1 Line 2");
	m_comboItem.InsertString(6,"Carriage 1 Line 3");
	m_comboItem.InsertString(7,"Carriage 2 Line 1");
	m_comboItem.InsertString(8,"Carriage 2 Line 2");
	m_comboItem.InsertString(9,"Carriage 2 Line 3");
	m_comboItem.InsertString(10,"Note");

	m_comboItem.InsertString(11,"Probe Frequency");
	m_comboItem.InsertString(12,"Squirter length Left");
	m_comboItem.InsertString(13,"Squirter opening left");
	m_comboItem.InsertString(14,"Squirter length right");
	m_comboItem.InsertString(15,"Squirter opening right");
	
	m_comboItem.InsertString(16,"Image Note");
	m_comboItem.InsertString(17,"Image File");

	m_comboItem.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorksheetData::OnSelchangeComboItem() 
{
	
	CProfileWorksheetDlg* pParent = (CProfileWorksheetDlg*)m_pParent;

	switch(m_comboItem.GetCurSel()) {
	case 0:
		m_editData.SetWindowText(pParent->m_Project);
		break;
	case 1:
		m_editData.SetWindowText(pParent->m_ProductNum);
		break;
	case 2:
		m_editData.SetWindowText(pParent->m_Product);
		break;
	case 3:
		m_editData.SetWindowText(pParent->m_HoldingFixture);
		break;
	case 4:
		m_editData.SetWindowText(pParent->m_Carriage1[0]);
		break;
	case 5:
		m_editData.SetWindowText(pParent->m_Carriage1[1]);
		break;
	case 6:
		m_editData.SetWindowText(pParent->m_Carriage1[2]);
		break;
	case 7:
		m_editData.SetWindowText(pParent->m_Carriage2[0]);
		break;
	case 8:
		m_editData.SetWindowText(pParent->m_Carriage2[1]);
		break;
	case 9:
		m_editData.SetWindowText(pParent->m_Carriage2[2]);
		break;
	case 10:
		m_editData.SetWindowText(pParent->m_FixtureNote);
		break;
	case 11:
		m_editData.SetWindowText(pParent->m_ProbeFrequency);
		break;
	case 12:
		m_editData.SetWindowText(pParent->m_SqrtLenLeft);
		break;
	case 13:
		m_editData.SetWindowText(pParent->m_SqrtOpenLeft);
		break;
	case 14:
		m_editData.SetWindowText(pParent->m_SqrtLenRight);
		break;
	case 15:
		m_editData.SetWindowText(pParent->m_SqrtOpenRight);
		break;
	case 16:
		m_editData.SetWindowText(pParent->m_Note);
		break;
	case 17:
		m_editData.SetWindowText(pParent->ImageFilename);
		break;
	}
}

void CWorksheetData::OnChangeEditData() 
{
	CProfileWorksheetDlg* pParent = (CProfileWorksheetDlg*)m_pParent;

	switch(m_comboItem.GetCurSel()) {
	case 0:
		 m_editData.GetWindowText(pParent->m_Project);
		 break;
 	case 1:
		 m_editData.GetWindowText(pParent->m_ProductNum);
		 break;
 	case 2:
		 m_editData.GetWindowText(pParent->m_Product);
		 break;
 	case 3:
		 m_editData.GetWindowText(pParent->m_HoldingFixture);
		 break;
 	case 4:
		 m_editData.GetWindowText(pParent->m_Carriage1[0]);
		 break;
 	case 5:
		 m_editData.GetWindowText(pParent->m_Carriage1[1]);
		 break;
 	case 6:
		 m_editData.GetWindowText(pParent->m_Carriage1[2]);
		 break;
 	case 7:
		 m_editData.GetWindowText(pParent->m_Carriage2[0]);
		 break;
 	case 8:
		 m_editData.GetWindowText(pParent->m_Carriage2[1]);
		 break;
 	case 9:
		 m_editData.GetWindowText(pParent->m_Carriage2[2]);
		 break;
 	case 10:
		 m_editData.GetWindowText(pParent->m_FixtureNote);
		 break;
 	case 11:
		 m_editData.GetWindowText(pParent->m_ProbeFrequency);
		 break;
 	case 12:
		 m_editData.GetWindowText(pParent->m_SqrtLenLeft);
		 break;
 	case 13:
		 m_editData.GetWindowText(pParent->m_SqrtOpenLeft);
		 break;
 	case 14:
		 m_editData.GetWindowText(pParent->m_SqrtLenRight);
		 break;
 	case 15:
		 m_editData.GetWindowText(pParent->m_SqrtOpenRight);
		 break;
 	case 16:
		 m_editData.GetWindowText(pParent->m_Note);
		 break;
 	case 17:
		 m_editData.GetWindowText(pParent->ImageFilename);
		 break;
	}

	pParent->Invalidate();

	
}
