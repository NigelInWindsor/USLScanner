// PmacDownLoadPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacDownLoadPage.h"
#include "PromptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacDownLoadPage property page

IMPLEMENT_DYNCREATE(CPmacDownLoadPage, CResizablePage)

CPmacDownLoadPage::CPmacDownLoadPage() : CResizablePage(CPmacDownLoadPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacDownLoadPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bModified = false;
	m_nIndex = 0;
}

CPmacDownLoadPage::~CPmacDownLoadPage()
{
}

void CPmacDownLoadPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacDownLoadPage)
	DDX_Control(pDX, IDC_COMBO_TRACKING_TENTH, m_comboTrackingTenthAxis);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
	DDX_Control(pDX, IDC_EDIT_PROGRAM, m_editProgram);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_progressDownLoad);
	DDX_Control(pDX, IDC_LIST_PROGRAMS, m_listPrograms);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacDownLoadPage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacDownLoadPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
	ON_LBN_SELCHANGE(IDC_LIST_PROGRAMS, OnSelchangeListPrograms)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_PROGRAM, OnChangeEditProgram)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_TRACKING_TENTH, OnSelchangeComboTrackingTenth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacDownLoadPage message handlers


BOOL CPmacDownLoadPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	int nTabs[10] = {12,24,36,48,60,72,84,96,108,120};

	m_StaticPosProgram.Initialize(this, m_editProgram, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosProgram.SetNewPos();

	m_comboTrackingTenthAxis.AddString(L"Starboard XTip");
	m_comboTrackingTenthAxis.AddString(L"Starboard YTip");

	m_editProgram.SetTabStops(10,nTabs);
	

	UpdateAllControls();
	ImportTextFile();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPmacDownLoadPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPmacDownLoadPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		m_StaticPosProgram.SetNewPos();
	};	
}

void CPmacDownLoadPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	WIN32_FIND_DATA ffd;

	m_listPrograms.ResetContent();
	m_comboTrackingTenthAxis.SetCurSel(theApp.m_Tank.nTrackingTenthAxis);


	HANDLE hFind = FindFirstFile(theApp.m_PmacProgramPath + "\\*.txt", &ffd);
	if (hFind != INVALID_HANDLE_VALUE )
	{
		do{
			m_listPrograms.AddString(ffd.cFileName);

		} while( FindNextFile(hFind, &ffd)!= 0 );
 
	}

	m_listPrograms.SetCurSel(0);
	m_editPath.SetWindowText(theApp.m_PmacProgramPath);

	SetPathColour();
}

void CPmacDownLoadPage::OnButtonDownload() 
{
	CWaitCursor Wait;
	CString Buff,Text;
	CString FilePath;
	int nIndex = m_listPrograms.GetCurSel();
	int	ii,nAddrs;
	int nOldDriver = theApp.m_PmacUser.m_nWhichDLL;

	theApp.m_Thread.StopCurrentThreadAction();

	OnButtonSave();

	m_listPrograms.GetText(nIndex,Buff);


	if(theApp.m_PmacUser.m_nWhichDLL == PMACCOMMSUSL) {
		theApp.m_PmacUser.SuspendDriver();
		theApp.m_PmacUser.m_nWhichDLL = COMMSSERVER;
		theApp.m_PmacUser.StartDriver();
	}

	
	FilePath=theApp.m_PmacProgramPath +"\\" + Buff;
	theApp.m_PmacUser.DownLoad(FilePath);

	if(Buff.Find(L"General M Variables") == 0) {
		for(ii=200,nAddrs=0x60070;ii<1024;ii++,nAddrs++) {
			Text.Format(_T("M%d->DP:$%lx"),ii,nAddrs);
			theApp.m_PmacUser.SendStr(Text,TRUE);
		};


		theApp.m_PmacUser.SendStr(L"WX:$7009,$D,$05");		//configure for dual port ram
	}
/*
	if(Buff.Find(L"General M Variables") == 0) {
		for(ii=200,nAddrs=0xD070;ii<1024;ii++,nAddrs++) {
			Text.Format(_T("M%d->DP:$%lx"),ii,nAddrs);
			theApp.m_PmacUser.SendStr(Text);
		};

		theApp.m_PmacUser.SendStr("WX:$E009,$D,$05");		//configure for dual port ram
		theApp.m_PmacUser.SendStr("WX:$786,$D,$05");		//configure for dual port ram
	}
*/
	if(nOldDriver - theApp.m_PmacUser.m_nWhichDLL) {
		theApp.m_PmacUser.SuspendDriver();
		theApp.m_PmacUser.m_nWhichDLL = PMACCOMMSUSL;
		theApp.m_PmacUser.StartDriver();
	}

	Wait.Restore();
	MessageBox(_T("Download complete"),_T("Message"),MB_OK);

}

void CPmacDownLoadPage::OnSelchangeListPrograms() 
{

	if(m_bModified == true) {
		if(MessageBox(L"The program has been modified. Continue without saving?",L"Warning",MB_YESNO) != IDYES) {
			m_listPrograms.SetCurSel(m_nIndex);
			return;
		}
	}

	m_nIndex = m_listPrograms.GetCurSel();

	ImportTextFile();


}

#define	MAX_FILE_LENGTH	1000000

bool CPmacDownLoadPage::ImportTextFile()
{
	CString Buff;
	CString FilePath;
	char Text[MAX_FILE_LENGTH];
	CUSLFile File;
	CFileException e;
	CArchive* pArchFile;

	USES_CONVERSION;

	if(m_listPrograms.GetCount() <=0) return false;

	m_listPrograms.GetText(m_nIndex,Buff);
	FilePath=theApp.m_PmacProgramPath +"\\" + Buff;
	
	if(!File.Open(FilePath,CFile::modeRead|CFile::typeBinary)) {

	} else {
		int nFileLength = (int)File.GetLength();
		if((nFileLength > 0) && (nFileLength < MAX_FILE_LENGTH)) {
			pArchFile = new CArchive(&File,CArchive::load);
			pArchFile->Read(&Text,nFileLength);
			Text[nFileLength]='\0';
			pArchFile->Close();
			delete pArchFile;

			Buff.Empty();
			for(int nn=0;nn<nFileLength;nn++) {
				Buff += Text[nn];
			}
		}

		File.Close();
	}
	m_editProgram.SetWindowText(Buff);
	m_bModified = false;
	SetPathColour();

	return true;
}

void CPmacDownLoadPage::GetDirectory(CEdit *pEdit, CString *pInitDir)
{
	BROWSEINFO bi;
	ITEMIDLIST* li;
	WCHAR DisplayName[MAX_PATH];
	WCHAR Title[MAX_PATH]={_T("Select new directory")};
	CString Buff;


	memset(&bi,0,sizeof bi);
	memset(DisplayName,0,sizeof DisplayName);
	bi.ulFlags = BIF_EDITBOX;
	bi.pszDisplayName = DisplayName;
	bi.lpszTitle = Title;
	li=SHBrowseForFolder(&bi);
	if(SHGetPathFromIDList(li,DisplayName) ) {
		Buff.Format(_T("%s"),DisplayName);
		if(pEdit) pEdit->SetWindowText(Buff);
		if(pInitDir) pInitDir->Format(_T("%s"),Buff);
	};	

}
void CPmacDownLoadPage::OnButtonBrowse() 
{
	GetDirectory(&m_editPath,&theApp.m_PmacProgramPath);

	theApp.SaveRegistryVariables();

	UpdateAllControls();
}


BOOL CPmacDownLoadPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	

	UpdateAllControls();
	m_listPrograms.SetCurSel(m_nIndex);
	
	return CResizablePage::OnSetActive();
}

BOOL CPmacDownLoadPage::OnKillActive() 
{
	
	
	return CResizablePage::OnKillActive();
}

void CPmacDownLoadPage::OnChangeEditProgram() 
{
	m_bModified = true;
	SetPathColour();

}

void CPmacDownLoadPage::SetPathColour()
{
	if(m_bModified == true) {
		m_editPath.SetTextColour(RGB(255,0,0));
	} else {
		m_editPath.SetTextColour(RGB(0,0,255));
	}

}

void CPmacDownLoadPage::OnButtonSave() 
{
	CString Buff;
	CString FilePath;
	char Text[1000000];
	CUSLFile File;
	CFileException e;
	int nn;

	int nIndex = m_listPrograms.GetCurSel();

	USES_CONVERSION;

	m_listPrograms.GetText(nIndex,Buff);
	FilePath=theApp.m_PmacProgramPath +"\\" + Buff;
	
	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(Buff,_T("Warning"), MB_ICONERROR);
		return;
	} else {

		m_editProgram.GetWindowText(Buff);
		for(nn=0;nn<Buff.GetLength();nn++) {
			Text[nn] = (char)Buff.GetAt(nn);
		}
		Text[nn]=0;
		File.Write(Text,Buff.GetLength()+1);
		File.Close();

	}

	m_bModified = false;
	SetPathColour();
	
}


BOOL CPmacDownLoadPage::PreTranslateMessage(MSG* pMsg) 
{
	HANDLE hWnd;
	CString Buff;

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case '\t':
			hWnd = ::GetFocus();
			if(hWnd == m_editProgram.m_hWnd) {
				m_editProgram.ReplaceSel(L"\t");
				return TRUE;
			};
			break;
		}
	}
	
	return CResizablePage::PreTranslateMessage(pMsg);
}


void CPmacDownLoadPage::OnSelchangeComboTrackingTenth() 
{
	theApp.m_Tank.nTrackingTenthAxis = m_comboTrackingTenthAxis.GetCurSel();
}
