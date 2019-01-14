// BladeMasterPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "BladeMasterPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBladeMasterPage property page

IMPLEMENT_DYNCREATE(CBladeMasterPage, CPropertyPage)

CBladeMasterPage::CBladeMasterPage(CWnd* pParent, CData* pData) : CPropertyPage(CBladeMasterPage::IDD)
{
	//{{AFX_DATA_INIT(CBladeMasterPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pParent = pParent;
	m_pData = pData;
}

CBladeMasterPage::~CBladeMasterPage()
{
}

void CBladeMasterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBladeMasterPage)
	DDX_Control(pDX, IDC_BUTTON_FILE_SAVEAS, m_buttonSaveAs);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC_LIGHTS, m_buttonTrafficLights);
	DDX_Control(pDX, IDC_BUTTON_BRANE, m_buttonBrane);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBladeMasterPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBladeMasterPage)
	ON_BN_CLICKED(IDC_BUTTON_TRAFFIC_LIGHTS, OnButtonTrafficLights)
	ON_BN_CLICKED(IDC_BUTTON_BRANE, OnButtonBrane)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVEAS, OnButtonFileSaveas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBladeMasterPage message handlers

BOOL CBladeMasterPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetAccessPrivelages();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBladeMasterPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CBladeMasterPage::SetAccessPrivelages()
{
	bool	bFlagEnable;

	theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_BLADE_MASTER_PAGE] ? bFlagEnable = TRUE : bFlagEnable = FALSE;

	m_buttonTrafficLights.EnableWindow(bFlagEnable);
	m_buttonBrane.EnableWindow(bFlagEnable);
	m_buttonSaveAs.EnableWindow(bFlagEnable);
}

void CBladeMasterPage::OnButtonTrafficLights() 
{
	int nLine,nSample;

	int nSide = theApp.m_Thread.m_nBladeSide;

	if(PROFILE->m_LineBlade[nSide] == NULL) return;
	if(PROFILE->m_nLineBladeL[nSide] != m_pData->m_nLineBladeL[ nSide ]) return;

	for(nLine = 0;nLine<m_pData->m_nLineBladeL[ nSide ];nLine++) {;
		for(nSample = 0;nSample<m_pData->m_LineBlade[ nSide ][nLine].GetSize();nSample++) {
			PROFILE->m_LineBlade[nSide][nLine].m_pCp[nSample].nType = m_pData->m_LineBlade[nSide][nLine].m_pCp[nSample].nType;
		}
	}	
}

void CBladeMasterPage::OnButtonBrane() 
{
	int nSide = theApp.m_Thread.m_nBladeSide;
	switch(theApp.m_Thread.m_nBladeSide) {
	case OUTSIDE:	PROFILE->CopyAllLines(BRANEOUTSIDELINES,BRAINLINES);
		break;
	case INSIDE:	PROFILE->CopyAllLines(BRANEINSIDELINES,BRAINLINES);
		break;
	}

	
}

void CBladeMasterPage::OnButtonFileSaveas() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = PROFILE->m_FileName;

	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".pro"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			PROFILE->Save(&File);
			File.Close();
			theApp.ActivityLog(L"Profile saved : " + FilePath);
		}
	}
	
	
}
