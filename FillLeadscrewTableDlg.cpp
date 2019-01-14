// FillLeadscrewTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FillLeadscrewTableDlg.h"
#include "LeadScrewCompensationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFillLeadscrewTableDlg dialog


CFillLeadscrewTableDlg::CFillLeadscrewTableDlg(CWnd* pParent /*=NULL*/, int nAxis)
	: CDialog(CFillLeadscrewTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFillLeadscrewTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_nAxis = nAxis;
}


void CFillLeadscrewTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillLeadscrewTableDlg)
	DDX_Control(pDX, IDC_EDIT_STEPSIZE, m_editStepsize);
	DDX_Control(pDX, IDC_EDIT_MAX_POS, m_editMaxPos);
	DDX_Control(pDX, IDC_EDIT_MIN_POS, m_editMinPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillLeadscrewTableDlg, CDialog)
	//{{AFX_MSG_MAP(CFillLeadscrewTableDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillLeadscrewTableDlg message handlers

BOOL CFillLeadscrewTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editMinPos.SetWindowText(L"0.0");
	m_editMaxPos.SetWindowText(L"0.0");
	m_editStepsize.SetWindowText(L"0.0");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFillLeadscrewTableDlg::OnButtonApply() 
{
	CString Buff;
	float fMin, fMax, fStepsize;

	m_editMinPos.GetWindowText(Buff);
	_WTOF(Buff,fMin);
	m_editMaxPos.GetWindowText(Buff);
	_WTOF(Buff,fMax);
	m_editStepsize.GetWindowText(Buff);
	_WTOF(Buff,fStepsize);

	if(fMin >= fMax) {
		MessageBox(L"Max pos needs to be greater than Min pos",L"Error", MB_OK);
		return;
	}
	if((fMax - fMin) < fStepsize) {
		MessageBox(L"Max - Min < fStepsize",L"Error", MB_OK);
		return;
	}

	theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[m_nAxis].nPhysicalAxis - 1 ].FillTable(fMin, fMax, fStepsize);

	CLeadScrewCompensationPage *pPage = (CLeadScrewCompensationPage *)m_pParent;
	pPage->FillList();
}

