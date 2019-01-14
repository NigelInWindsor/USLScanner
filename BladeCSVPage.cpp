// BladeCSVPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "BladeCSVPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define AUTO_INCREMENT_CHOICE	0x0001
#define SAVE_WHAT				0x0002
#define WHICH_BLADE_LINES		0x0004
#define SAVE_FILE_MODE			0x0008


/////////////////////////////////////////////////////////////////////////////
// CBladeCSVPage property page

IMPLEMENT_DYNCREATE(CBladeCSVPage, CPropertyPage)

CBladeCSVPage::CBladeCSVPage(CWnd* pParent, CData *pData) : CPropertyPage(CBladeCSVPage::IDD)
{
	//{{AFX_DATA_INIT(CBladeCSVPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pParent = pParent;
	m_pData = pData;
}

CBladeCSVPage::~CBladeCSVPage()
{
}

void CBladeCSVPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBladeCSVPage)
	DDX_Control(pDX, IDC_COMBO_WHAT, m_comboWhat);
	DDX_Control(pDX, IDC_COMBO_WHICH_LINES, m_comboWhichLines);
	DDX_Control(pDX, IDC_COMBO_SAVE_TO, m_comboSaveTo);
	DDX_Control(pDX, IDC_COMBO_AUTO_INCREMENTER, m_comboAutoIncrementer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBladeCSVPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBladeCSVPage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_INCREMENTER, OnSelchangeComboAutoIncrementer)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVE_TO, OnSelchangeComboSaveTo)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_LINES, OnSelchangeComboWhichLines)
	ON_CBN_SELCHANGE(IDC_COMBO_WHAT, OnSelchangeComboWhat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBladeCSVPage message handlers

BOOL CBladeCSVPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	

	m_comboAutoIncrementer.AddString(L"a - z");
	m_comboAutoIncrementer.AddString(L"1 - 99");

	m_comboWhat.AddString(L"Everything");
	m_comboWhat.AddString(L"Traffic Lights");

	m_comboWhichLines.AddString(L"Currentline");
	m_comboWhichLines.AddString(L"All lines this side");

	m_comboSaveTo.AddString(L"Single file");
	m_comboSaveTo.AddString(L"Multiple files");

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBladeCSVPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CBladeCSVPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}


void CBladeCSVPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	theApp.m_LastSettings.nBladeCSVMask & AUTO_INCREMENT_CHOICE ? m_comboAutoIncrementer.SetCurSel(1) :	m_comboAutoIncrementer.SetCurSel(0);
	theApp.m_LastSettings.nBladeCSVMask & SAVE_WHAT ?				m_comboWhat.SetCurSel(1) :				m_comboWhat.SetCurSel(0);
	theApp.m_LastSettings.nBladeCSVMask & WHICH_BLADE_LINES ?		m_comboWhichLines.SetCurSel(1) :		m_comboWhichLines.SetCurSel(0);
	theApp.m_LastSettings.nBladeCSVMask & SAVE_FILE_MODE ?		m_comboSaveTo.SetCurSel(1) :			m_comboSaveTo.SetCurSel(0);

}


void CBladeCSVPage::OnSelchangeComboAutoIncrementer() 
{
	switch(m_comboAutoIncrementer.GetCurSel()) {
	case 0:	theApp.m_LastSettings.nBladeCSVMask &= ~AUTO_INCREMENT_CHOICE;
		break;
	case 1:	theApp.m_LastSettings.nBladeCSVMask |= AUTO_INCREMENT_CHOICE;
		break;
	}

}

void CBladeCSVPage::OnSelchangeComboSaveTo() 
{
	switch(m_comboSaveTo.GetCurSel()) {
	case 0:	theApp.m_LastSettings.nBladeCSVMask &= ~SAVE_FILE_MODE;
		break;
	case 1:	theApp.m_LastSettings.nBladeCSVMask |= SAVE_FILE_MODE;
		break;
	}
}

void CBladeCSVPage::OnSelchangeComboWhichLines() 
{
	switch(m_comboWhichLines.GetCurSel()) {
	case 0:	theApp.m_LastSettings.nBladeCSVMask &= ~WHICH_BLADE_LINES;
		break;
	case 1:	theApp.m_LastSettings.nBladeCSVMask |= WHICH_BLADE_LINES;
		break;
	}
}

void CBladeCSVPage::OnSelchangeComboWhat() 
{
	switch(m_comboWhichLines.GetCurSel()) {
	case 0:	theApp.m_LastSettings.nBladeCSVMask &= ~SAVE_WHAT;
		break;
	case 1:	theApp.m_LastSettings.nBladeCSVMask |= SAVE_WHAT;
		break;
	}
}

void CBladeCSVPage::OnButtonSave() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_pData->m_FileName;
	CString IncrementingPathName;
	int nLine;
	bool bEverything_TrafficLights;

	char strHeaderName[] = "Ultrasonic Sciences Ltd USLScanner Blade Scan Data\r\n";
	char strColumnHeader[] ="X,Y,Z,i,j,k,Act,Nom,TL,Min,Max,flag,Amp\r\n";


	theApp.m_LastSettings.nBladeCSVMask & SAVE_WHAT ? bEverything_TrafficLights = true : bEverything_TrafficLights = false;

	if((nPos = m_pData->m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_pData->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".csv"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("CSV (*.csv)|*.csv|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}


		if((theApp.m_LastSettings.nBladeCSVMask & SAVE_FILE_MODE) && (theApp.m_LastSettings.nBladeCSVMask & WHICH_BLADE_LINES)) {	//Multiple files
			IncrementingPathName = FilePath;
			if((nPos = FilePath.ReverseFind('.')) >0) {
				IncrementingPathName = FilePath.Left(nPos);
			}

			for(nLine = 0; nLine<m_pData->m_nLineBladeL[theApp.m_Thread.m_nBladeSide]; nLine++) {
				if(theApp.m_LastSettings.nBladeCSVMask & AUTO_INCREMENT_CHOICE) {
					FilePath.Format(L"%s-%d.csv",IncrementingPathName,nLine+1);
				} else {
					FilePath.Format(L"%s-%c.csv",IncrementingPathName,'a' + nLine+1);
				}

				if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
					File.DecodeFileException(&e,&Buff,FilePath);
					MessageBox(Buff,_T("Error"),MB_ICONERROR);
				} else {
					File.Write(strHeaderName, strlen(strHeaderName));
					File.Write(strColumnHeader, strlen(strColumnHeader));

					m_pData->SaveBladeCSV(&File,theApp.m_Thread.m_nBladeSide, nLine, bEverything_TrafficLights);
					File.Close();
				}
			}


		} else {		// Single file

			if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
				File.DecodeFileException(&e,&Buff,FilePath);
				MessageBox(Buff,_T("Error"),MB_ICONERROR);
			} else {
				File.Write(strHeaderName, strlen(strHeaderName));
				File.Write(strColumnHeader, strlen(strColumnHeader));

				if(theApp.m_LastSettings.nBladeCSVMask & WHICH_BLADE_LINES) { // All Lines
					for(nLine = 0; nLine<m_pData->m_nLineBladeL[theApp.m_Thread.m_nBladeSide]; nLine++) {
						m_pData->SaveBladeCSV(&File,theApp.m_Thread.m_nBladeSide, nLine, bEverything_TrafficLights);
					};
				} else {		//Just 1 line
					m_pData->SaveBladeCSV(&File,theApp.m_Thread.m_nBladeSide, theApp.m_nBladeLine, bEverything_TrafficLights);
				}
				File.Close();
			}
		}
	}
}
