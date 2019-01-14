// ImportRulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ImportRulesDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportRulesDlg dialog


CImportRulesDlg::CImportRulesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportRulesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportRulesDlg)
	//}}AFX_DATA_INIT
}


void CImportRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportRulesDlg)
	DDX_Control(pDX, IDC_COMBO_SIMPLE_DEST, m_comboSimpleDest);
	DDX_Control(pDX, IDC_COMBO1_REVERSE_LINE_ORDER, m_comboReverselineOrder);
	DDX_Control(pDX, IDC_COMBO_COMP_IN_SCANNER, m_comboComponentInScanner);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progressBar);
	DDX_Control(pDX, IDC_CHECK_READING_HOLES, m_buttonReadingHoles);
	DDX_Control(pDX, IDC_CHECK_READ_COORDINATES, m_buttonReadCoordinates);
	DDX_Control(pDX, IDC_CHECK_HOW_MANY_LINES, m_buttonHowManyLines);
	DDX_Control(pDX, IDC_CHECK_CONVERT_TO_UNICODE, m_editConvertToUnicode);
	DDX_Control(pDX, IDC_COMBO_APT_FILETYPE, m_comboFileType);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_comboDirection);
	DDX_Control(pDX, IDC_COMBO_UNITS, m_comboUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportRulesDlg, CDialog)
	//{{AFX_MSG_MAP(CImportRulesDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_APT_FILETYPE, OnSelchangeComboAptFiletype)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_CBN_SELCHANGE(IDC_COMBO_COMP_IN_SCANNER, OnSelchangeComboCompInScanner)
	ON_CBN_SELCHANGE(IDC_COMBO1_REVERSE_LINE_ORDER, OnSelchangeCombo1ReverseLineOrder)
	ON_CBN_SELCHANGE(IDC_COMBO_SIMPLE_DEST, OnSelchangeComboSimpleDest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportRulesDlg message handlers

BOOL CImportRulesDlg::OnInitDialog() 
{
	CString	Buff;
	CDialog::OnInitDialog();
	
	m_comboUnits.AddString(_T("Metric"));
	m_comboUnits.AddString(_T("Imperial"));
	m_comboUnits.SetCurSel(theApp.m_LastSettings.nImportAptUnits & 1);

	m_comboDirection.AddString(_T("Unidirectional"));
	m_comboDirection.AddString(_T("Zig Zag"));
	m_comboDirection.SetCurSel(theApp.m_LastSettings.nImportAptDirection & 1);


	m_comboFileType.AddString(_T("Simple Raster"));
	m_comboFileType.AddString(_T("BAE Samlesbury"));
	m_comboFileType.AddString(_T("Fokker"));
	m_comboFileType.AddString(_T("NLR"));
	m_comboFileType.AddString(_T("Hafei"));
	m_comboFileType.AddString(_T("GKN"));
	m_comboFileType.AddString(_T("Terma"));
	m_comboFileType.AddString(_T("GKN JSF"));
	m_comboFileType.AddString(_T("CTRM"));
	m_comboFileType.AddString(_T("Goodrich"));
	m_comboFileType.AddString(_T("Goodrich Machine 4"));
	m_comboFileType.AddString(_T("Xifei"));
	m_comboFileType.AddString(_T("Dowty"));
	m_comboFileType.SetCurSel(theApp.m_LastSettings.nImportAptFileType);

	m_comboSimpleDest.AddString(_T("Taught"));
	m_comboSimpleDest.AddString(_T("Scan"));
	m_comboSimpleDest.SetCurSel(theApp.m_LastSettings.nImportToolPathDest);

	Buff.LoadString(IDS_No); m_comboComponentInScanner.AddString(Buff);
	Buff.LoadString(IDS_Yes); m_comboComponentInScanner.AddString(Buff);

	Buff.LoadString(IDS_No); m_comboReverselineOrder.AddString(Buff);
	Buff.LoadString(IDS_Yes); m_comboReverselineOrder.AddString(Buff);

	m_comboComponentInScanner.SetCurSel(theApp.m_LastSettings.nPlaceComponentInScanner);
	m_comboReverselineOrder.SetCurSel(theApp.m_LastSettings.nReverseLineOrder);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportRulesDlg::OnOK() 
{
	theApp.m_LastSettings.nImportAptUnits = m_comboUnits.GetCurSel();
	theApp.m_LastSettings.nImportAptDirection = m_comboDirection.GetCurSel();
	theApp.m_LastSettings.nImportToolPathDest = m_comboSimpleDest.GetCurSel();

	CDialog::OnOK();
}


void CImportRulesDlg::OnSelchangeComboAptFiletype() 
{
	theApp.m_LastSettings.nImportAptFileType = m_comboFileType.GetCurSel();
}

void CImportRulesDlg::OnButtonImport() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString FileName;
	int	nPos;

	theApp.m_LastSettings.nImportAptFileType = m_comboFileType.GetCurSel();
	theApp.m_LastSettings.nImportAptUnits = m_comboUnits.GetCurSel();
	theApp.m_LastSettings.nImportAptDirection = m_comboDirection.GetCurSel();
	theApp.m_LastSettings.nImportToolPathDest = m_comboSimpleDest.GetCurSel();

	m_progressBar.SetRange(0,100);
	m_progressBar.SetPos(0);
	m_buttonReadingHoles.SetCheck(0);
	m_buttonReadCoordinates.SetCheck(0);
	m_buttonHowManyLines.SetCheck(0);
	m_editConvertToUnicode.SetCheck(0);

	CFileDialog FileDlg(TRUE,_T(".AptSource"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("AptSource (*.aptsource)|*.aptsource|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_AptSourcePath;
	if(FileDlg.DoModal()==IDOK) {
		FileName=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FileName.ReverseFind('\\')) >0) {
				theApp.m_AptSourcePath = FileName.Left(nPos);
			}
		}

		FileName.Format(_T("%s"),FileDlg.GetPathName());
		theApp.ActivityLog(L"Import aptsource : " + FileName);

		m_editConvertToUnicode.SetCheck(1);
		CheckItsUnicode(&FileName);

		switch(theApp.m_LastSettings.nImportAptFileType) {
		case 0: PROFILE->ImportAptSourceSimpleRaster(FileName);
			break;
		case 1: PROFILE->ImportAptSourceBAESamlesburyFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,0);
			break;
		case 2: PROFILE->ImportAptSourceFokkerSingleRapidFormat(FileName);
			break;
		case 3: PROFILE->ImportAptSourceBAESamlesburyFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 4: PROFILE->ImportAptSourceHafeiFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 5: PROFILE->ImportAptSourceGKNFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 6: PROFILE->ImportAptSourceTermaFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 7: PROFILE->ImportAptSourceGknJsfFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 8: PROFILE->ImportAptSourceCTRMFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 9: PROFILE->ImportAptSourceGoodrichFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 10: PROFILE->ImportAptSourceGoodrichMachine4(FileName);
			break;
		case 11:  PROFILE->ImportAptSourceXifeiFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		case 12:  PROFILE->ImportAptSourceDowtyFormat(FileName,&m_progressBar,&m_buttonHowManyLines,&m_buttonReadCoordinates,&m_buttonReadingHoles,1);
			break;
		}
		theApp.ActivityLog(L"Import succesfull");

	}
	m_progressBar.SetPos(0);
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG,(WPARAM)this,NULL);

}

bool CImportRulesDlg::CheckItsUnicode(CString *pFileName)
{
	CUSLFile file;
	CFileException e;
	CString	Buff,Language;
	char* pText;
	char Array[256];
	int	ii,jj;
	CArchive* pArchFile;

	USES_CONVERSION;

	if(!file.Open(pFileName->GetBuffer(100),CFile::modeRead|CFile::typeBinary,&e)) {
		file.DecodeFileException(&e,&Buff,pFileName->GetBuffer(100));
		Language.LoadString(IDS_Error);
		MessageBox(Buff,Language,MB_ICONERROR);
		return FALSE;
	} else {
		int nFileLength = (int)file.GetLength();

		pText = new char[nFileLength+1];
		file.Read(pText,nFileLength);
		file.Close();

		//See if it's already unicde;
		for(ii=0;ii<20;ii++) {
			if(pText[ii]==0) return TRUE;
		}

		//Convert to unicode
		file.Open(_T("Unicode.aptsource"),CFile::modeReadWrite|CFile::typeBinary|CFile::modeCreate,&e);
		pArchFile = new CArchive(&file,CArchive::store);


		int	nOldPos=0,nPos=0;
		for(ii=0,jj=0;ii<nFileLength;ii++,jj++) {
			nPos = MulDiv(ii,100,nFileLength);
			if(nPos-nOldPos)
				m_progressBar.SetPos(nOldPos = nPos);
			Array[jj]=pText[ii];
			if((pText[ii]==0x0a) || (pText[ii]==0x0d) || (ii==nFileLength-1)) {
				Array[jj]=0;
				if(strlen(Array)>1) {
					Buff.Format(_T("%s"), (CString) Array);
					pArchFile->WriteString(Buff+_T("\r\n"));
				}
				jj=-1;
			}
		}
		delete pText;

		pArchFile->Close();
		delete pArchFile;
		//Change filename to the unicode one
		pFileName->Format(_T("%s"),file.GetFilePath());
		file.Close();
		return TRUE;
	}
	return TRUE;
}

void CImportRulesDlg::OnSelchangeComboCompInScanner() 
{
	theApp.m_LastSettings.nPlaceComponentInScanner = m_comboComponentInScanner.GetCurSel();
}

void CImportRulesDlg::OnSelchangeCombo1ReverseLineOrder() 
{
	theApp.m_LastSettings.nReverseLineOrder = m_comboReverselineOrder.GetCurSel();
}


void CImportRulesDlg::OnSelchangeComboSimpleDest() 
{
	// TODO: Add your control notification handler code here
	
}

