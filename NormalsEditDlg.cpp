// NormalsEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "NormalsEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNormalsEditDlg dialog


CNormalsEditDlg::CNormalsEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop)
	: CDialog(CNormalsEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNormalsEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
	m_nLeft = nLeft;
	m_nTop = nTop;
}


void CNormalsEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNormalsEditDlg)
	DDX_Control(pDX, IDC_CHECK_CAD, m_checkCAD);
	DDX_Control(pDX, IDC_CHECK_BRAIN, m_checkBrain);
	DDX_Control(pDX, IDC_CHECK_TAUGHT, m_checkTaught);
	DDX_Control(pDX, IDC_CHECK_SCAN, m_checkScan);
	DDX_Control(pDX, IDC_CHECK_K_NORMAL, m_checkKNormal);
	DDX_Control(pDX, IDC_CHECK_J_NORMAL, m_checkJNormal);
	DDX_Control(pDX, IDC_CHECK_I_NORMAL, m_checkINormal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNormalsEditDlg, CDialog)
	//{{AFX_MSG_MAP(CNormalsEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_I_NORMAL, OnCheckINormal)
	ON_BN_CLICKED(IDC_CHECK_J_NORMAL, OnCheckJNormal)
	ON_BN_CLICKED(IDC_CHECK_K_NORMAL, OnCheckKNormal)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalsEditDlg message handlers

void CNormalsEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CNormalsEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CNormalsEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CNormalsEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNormalsEditDlg::OnCheckINormal() 
{
	// TODO: Add your control notification handler code here
	
}

void CNormalsEditDlg::OnCheckJNormal() 
{
	// TODO: Add your control notification handler code here
	
}

void CNormalsEditDlg::OnCheckKNormal() 
{
	// TODO: Add your control notification handler code here
	
}

void CNormalsEditDlg::OnButtonApply() 
{
	bool bTaught, bScan, bBrain, bCAD;
	bool bI, bJ, bK;


	bTaught = m_checkTaught.GetCheck() & 0x01;
	bScan = m_checkScan.GetCheck() & 0x01;
	bBrain = m_checkBrain.GetCheck() & 0x01;	
	bCAD= m_checkCAD.GetCheck() & 0x01;

	bI = m_checkINormal.GetCheck() & 0x01;
	bJ = m_checkJNormal.GetCheck() & 0x01;
	bK = m_checkKNormal.GetCheck() & 0x01;

	PROFILE->InvertNormals(bTaught, bScan, bBrain, bCAD, bI, bJ, bK);

	m_checkTaught.SetCheck(false);
	m_checkScan.SetCheck(false);
	m_checkBrain.SetCheck(false);
	m_checkCAD.SetCheck(false);

	m_checkINormal.SetCheck(false);
	m_checkJNormal.SetCheck(false);
	m_checkKNormal.SetCheck(false);

	Invalidate(FALSE);
	
}
