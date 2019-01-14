// PmacArchivePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacArchivePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacArchivePage property page

IMPLEMENT_DYNCREATE(CPmacArchivePage, CResizablePage)

CPmacArchivePage::CPmacArchivePage() : CResizablePage(CPmacArchivePage::IDD)
{
	//{{AFX_DATA_INIT(CPmacArchivePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPmacArchivePage::~CPmacArchivePage()
{
}

void CPmacArchivePage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacArchivePage)
	DDX_Control(pDX, IDC_CHECK_PROGRAMS, m_checkPrograms);
	DDX_Control(pDX, IDC_CHECK_PLCS, m_checkPLCs);
	DDX_Control(pDX, IDC_CHECK_M_VARIABLES, m_checkMVariables);
	DDX_Control(pDX, IDC_CHECK_I_VARIABLES, m_checkIVariables);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacArchivePage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacArchivePage)
	ON_BN_CLICKED(IDC_CHECK_M_VARIABLES, OnCheckMVariables)
	ON_BN_CLICKED(IDC_CHECK_I_VARIABLES, OnCheckIVariables)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, OnButtonSaveAs)
	ON_BN_CLICKED(IDC_CHECK_PROGRAMS, OnCheckPrograms)
	ON_BN_CLICKED(IDC_CHECK_PLCS, OnCheckPlcs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacArchivePage message handlers

BOOL CPmacArchivePage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPmacArchivePage::UpdateAllControls()
{
	m_checkIVariables.SetCheck(theApp.m_LastSettings.nCheckPmacIVariables);
	m_checkMVariables.SetCheck(theApp.m_LastSettings.nCheckPmacMVariables);
	m_checkPrograms.SetCheck(theApp.m_LastSettings.nCheckPmacPrograms);
	m_checkPLCs.SetCheck(theApp.m_LastSettings.nCheckPmacPLCs);
}

void CPmacArchivePage::OnCheckMVariables() 
{
	theApp.m_LastSettings.nCheckPmacMVariables = m_checkMVariables.GetCheck();
}

void CPmacArchivePage::OnCheckIVariables() 
{
	theApp.m_LastSettings.nCheckPmacIVariables = m_checkIVariables.GetCheck();
}



void CPmacArchivePage::OnButtonOpen() 
{
	CString FilePath,Buff;
	int	nPos;

	CFileDialog FileDlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Pmac (*.txt)|*.txt|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PmacPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PmacPath = FilePath.Left(nPos);
			}
		}

		theApp.m_PmacUser.Retrieve(FilePath);
	}
}

void CPmacArchivePage::OnButtonSaveAs() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;

	CFileDialog FileDlg(FALSE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Pmac (*.txt)|*.txt|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PmacPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PmacPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			theApp.m_PmacUser.Save(&File);
			File.Close();
		}
	}
	
}

void CPmacArchivePage::OnCheckPrograms() 
{
	theApp.m_LastSettings.nCheckPmacPrograms = m_checkPrograms.GetCheck();
}

void CPmacArchivePage::OnCheckPlcs() 
{
	theApp.m_LastSettings.nCheckPmacPLCs = m_checkPLCs.GetCheck();
}
