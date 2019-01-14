// WorkSpaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "WorkSpaceDlg.h"
#include "FileThumbNailDlg.h"
#include "MainFrm.h"
#include "ViewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDlg dialog

IMPLEMENT_DYNCREATE(CWorkSpaceDlg, CResizablePage)

CWorkSpaceDlg::CWorkSpaceDlg()	: CResizablePage(CWorkSpaceDlg::IDD)
{
	
	//{{AFX_DATA_INIT(CWorkSpaceDlg)
	//}}AFX_DATA_INIT
	
	m_pProfileItem = NULL;
	m_pEditCtrl = NULL;
	m_pStatic = NULL;
	m_pEditSpinItem = NULL;
	m_pnCutArray = NULL;


	m_nSecondsInAStep = 60;

	m_bLbDown = false;
	m_bProfileSelected = false;
	m_bBorderSelected = false;

	m_nLadderIndex = 0;
	m_nProfileIndex = 0;
	m_nMoveIndex = 0;


	switch(theApp.m_Tank.nScannerDescription) {
	default: m_bTwoSided = true;
		break;
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
	case DOUBLE_BRIDGE_TANK:
	case SINGLE_ROBOT:
		m_bTwoSided = false;
		break;
	}



	m_strAction[0].LoadString( IDS_No_action );
	m_strAction[1].LoadString( IDS_Absolute );
	m_strAction[2].LoadString( IDS_Relative );
	m_strStandard[0] = " ";
	m_strStandard[1].LoadString( IDS_Standard );
	m_strStatus[0].Empty();
	m_strStatus[1].LoadString(IDS_Scanning);
	m_strStatus[2].LoadString(IDS_Finished);
	m_strStatus[3].LoadString(IDS_Saved);


	if(theApp.m_LastSettings.m_fWorkSpaceVDividerPos<0.05f)  theApp.m_LastSettings.m_fWorkSpaceVDividerPos = 0.05f;
	if(theApp.m_LastSettings.m_fWorkSpaceVDividerPos>0.99f)  theApp.m_LastSettings.m_fWorkSpaceVDividerPos = 0.99f;

}

CWorkSpaceDlg::~CWorkSpaceDlg()
{

	SAFE_DELETE( m_pProfileItem );
};

void CWorkSpaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkSpaceDlg)
	DDX_Control(pDX, IDC_CHECK_SWITCH_OFF_PUMPS, m_checkSwitchOffPumps);
	DDX_Control(pDX, IDC_SPIN_SCAN_SPEED, m_spinScanSpeed);
	DDX_Control(pDX, IDC_EDIT_SCAN_SPEED, m_editScanSpeed);
	DDX_Control(pDX, IDC_EDIT_RESUME_LINE, m_editResumeLine);
	DDX_Control(pDX, IDC_EDIT_SCAN_LINE, m_editScanLine);
	DDX_Control(pDX, IDC_EDIT_LASTCALIBRATION_TIME, m_editLastCalTime);
	DDX_Control(pDX, IDC_EDIT_COUNTDOWN, m_editTimeDiff);
	DDX_Control(pDX, IDC_EDIT_REF_IMAGE, m_editRefImage);
	DDX_Control(pDX, IDC_EDIT_REF_ATTENUATION, m_editRefAttenuation);
	DDX_Control(pDX, IDC_LIST_REF_LEVELS, m_listRefLevels);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_LIST_PROFILES, m_listProfiles);
	DDX_Control(pDX, IDC_COMBO_MAX_SCAN_TIME, m_comboMaxScanTime);
	DDX_Control(pDX, IDC_SPIN_POSITIVE_ATTENUATION, m_spinPositiveAttenuation);
	DDX_Control(pDX, IDC_SPIN_NEGATIVE_ATTENUATION, m_spinNegativeAttenuation);
	DDX_Control(pDX, IDC_EDIT_POSITIVE_ATTENUATION, m_editPositiveAttenuation);
	DDX_Control(pDX, IDC_EDIT_NEGATIVE_ATTENUATION, m_editNegativeAttenuation);
	DDX_Control(pDX, IDC_LIST_INSPECTION, m_listPlayOrder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkSpaceDlg, CResizablePage)
	//{{AFX_MSG_MAP(CWorkSpaceDlg)
	ON_WM_SIZE()
	ON_COMMAND(ID_PROFILEWORKSPACE_NEWPROFILE, OnNewprofile)
	ON_COMMAND(ID_PROFILEWORKSPACE_SAVE, OnSaveAsProfile)
	ON_COMMAND(ID_PROFILEWORKSPACE_DELETE, OnDeleteProfile)
	ON_COMMAND(ID_PROFILEWORKSPACE_RENAME, OnRenameProfile)
	ON_COMMAND(ID_PROFILEWORKSPACE_NEWWORKSPACE, OnNewWorkspace)
	ON_COMMAND(ID_PROFILEWORKSPACE_SAVEASWORKSPACE, OnSaveAsWorkSpace)
	ON_COMMAND(ID_PROFILEWORKSPACE_OPENWORKSPACE, OnOpenWorkspace)
	ON_COMMAND(ID_PROFILEWORKSPACE_COPY, OnCopyProfile)
	ON_COMMAND(ID_PROFILEWORKSPACE_PASTE, OnPasteProfile)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_PROFILEWORKSPACE_ADDPROFILE, OnProfileworkspaceAddprofile)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INSPECTION, OnClickListInspection)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INSPECTION, OnDblclkListInspection)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_INSPECTION, OnRclickListInspection)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_INSPECTION, OnGetdispinfoListInspection)
	ON_COMMAND(ID_PROFILEWORKSPACE_SENDTOLADDER, OnProfileworkspaceSendtoladder)
	ON_COMMAND(ID_LADDER_CLEARCOMPONENTVARIABLES, OnLadderClearcomponentvariables)
	ON_COMMAND(ID_LADDER_CLEARFILENAMES, OnLadderClearfilenames)
	ON_COMMAND(ID_LADDER_CLEARPATHNAMES, OnLadderClearpathnames)
	ON_COMMAND(ID_LADDER_CLEARUNIQUEIDENTIFIERS, OnLadderClearuniqueidentifiers)
	ON_COMMAND(ID_LADDER_DELETE, OnLadderDelete)
	ON_COMMAND(ID_LADDER_FILENAMEPATHNAME, OnLadderFilenamepathname)
	ON_COMMAND(ID_LADDER_COPYFILENAME, OnLadderCopyfilename)
	ON_COMMAND(ID_LADDER_COPYINCFILENAME, OnLadderCopyincfilename)
	ON_COMMAND(ID_LADDER_COPYINCUNIQUEIDENTIFIER, OnLadderCopyincuniqueidentifier)
	ON_COMMAND(ID_LADDER_COPYPATHNAME, OnLadderCopypathname)
	ON_COMMAND(ID_LADDER_COPYUNIQUEIDENTIFIER, OnLadderCopyuniqueidentifier)
	ON_COMMAND(ID_LADDER_CUT, OnLadderCut)
	ON_COMMAND(ID_LADDER_PASTE, OnLadderPaste)
	ON_COMMAND(ID_PATH_COPY, OnPathCopy)
	ON_COMMAND(ID_PATH_PASTE, OnPathPaste)
	ON_COMMAND(ID_FILENAME_COPY, OnFilenameCopy)
	ON_COMMAND(ID_FILENAME_PASTE, OnFilenamePaste)
	ON_COMMAND(ID_FILENAME_PASTEA1, OnFilenamePastea1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NEGATIVE_ATTENUATION, OnDeltaposSpinNegativeAttenuation)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_POSITIVE_ATTENUATION, OnDeltaposSpinPositiveAttenuation)
	ON_CBN_SELCHANGE(IDC_COMBO_MAX_SCAN_TIME, OnSelchangeComboMaxScanTime)
	ON_EN_CHANGE(IDC_EDIT_NEGATIVE_ATTENUATION, OnChangeEditNegativeAttenuation)
	ON_EN_CHANGE(IDC_EDIT_POSITIVE_ATTENUATION, OnChangeEditPositiveAttenuation)
	ON_EN_CHANGE(IDC_EDIT_CORNER_0, OnChangeEditCorner0)
	ON_EN_CHANGE(IDC_EDIT_CORNER_1, OnChangeEditCorner1)
	ON_LBN_SELCHANGE(IDC_LIST_PROFILES, OnSelchangeListProfiles)
	ON_WM_CONTEXTMENU()
	ON_LBN_DBLCLK(IDC_LIST_PROFILES, OnDblclkListProfiles)
	ON_COMMAND(ID_STATUS_RESET, OnStatusReset)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_BUTTON_INSPECTION_CRITERIA, OnButtonInspectionCriteria)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_RESUME_LINE, OnChangeEditResumeLine)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_SPEED, OnDeltaposSpinScanSpeed)
	ON_EN_CHANGE(IDC_EDIT_SCAN_SPEED, OnChangeEditScanSpeed)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_SWITCH_OFF_PUMPS, OnCheckSwitchOffPumps)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDlg message handlers


BOOL CWorkSpaceDlg::OnInitDialog()
{
	
	CString Buff;
	int ii;
	UDACCEL	Accel;
	Accel.nInc=5;
	Accel.nSec=1;


	CResizablePage::OnInitDialog();

	m_nLastTimeDiff = -1;
	m_osLastCalibration = -1;


	CRect rect = theApp.m_LastSettings.RectWindowPos[CWorkSpaceDlg::IDD];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	};

	
	SendMessage(WM_SIZE);

	for(ii=0;ii<21601;ii+=m_nSecondsInAStep) {
		Buff.Format(L"%0dh %02d",ii/3600,(ii%3600) / 60);
		m_comboMaxScanTime.AddString(Buff);
	}

	m_spinScanSpeed.SetRange(1,1000);
	m_spinScanSpeed.SetAccel(1,&Accel);

	m_spinPositiveAttenuation.SetRange(0,1000);
	m_spinNegativeAttenuation.SetRange(0,1000);


	m_imageList.Create (16, 16, ILC_COLOR32 , 1,1);
	m_bitmap[0].LoadBitmap(IDB_BITMAP_OPEN_DIRECTORY);	m_imageList.Add(&m_bitmap[0],RGB(0, 0, 0));
	m_bitmap[1].LoadBitmap(IDB_BITMAP_PROFILE);			m_imageList.Add(&m_bitmap[1],RGB(0, 0, 0));
	m_bitmap[2].LoadBitmap(IDB_BITMAP_VARIABLES);		m_imageList.Add(&m_bitmap[2],RGB(0, 0, 0));
	m_bitmap[3].LoadBitmap(IDB_BITMAP_LINES);			m_imageList.Add(&m_bitmap[3],RGB(0, 0, 0));


	CreatePlayListColumns();
	FillProfileList();
	FillPlayList();

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkSpaceDlg::OnSize(UINT nType, int cx, int cy) 
{
	
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		AdjustLayout();
	}

	
}

void CWorkSpaceDlg::UpdateAllControls()
{
	
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	m_comboMaxScanTime.SetCurSel((int)(theApp.m_LastSettings.osTimeBetweenStandards / m_nSecondsInAStep));


	Buff.Format(L"%.01f dB",theApp.m_WS.m_fAttenuationRange[0]);	m_editNegativeAttenuation.SetWindowText(Buff);
	Buff.Format(L"+%.01f dB",theApp.m_WS.m_fAttenuationRange[1]);	m_editPositiveAttenuation.SetWindowText(Buff);

	m_spinPositiveAttenuation.SetPos((int)(theApp.m_WS.m_fAttenuationRange[0] * 10.0f));
	m_spinNegativeAttenuation.SetPos((int)(theApp.m_WS.m_fAttenuationRange[1] * 10.0f));

//	Buff.Format(L"H:%d V:%d", theApp.m_WS.m_rrSampleCheckArea.left, theApp.m_WS.m_rrSampleCheckArea.bottom);	m_editCorner0.SetWindowText(Buff);
//	Buff.Format(L"H:%d V:%d", theApp.m_WS.m_rrSampleCheckArea.right, theApp.m_WS.m_rrSampleCheckArea.top);		m_editCorner1.SetWindowText(Buff);

	Buff.Format(L"%d: %s",theApp.m_WS.m_nRefImage + 1,theApp.m_WS.m_strRefImageName);
	m_editRefImage.SetWindowText(Buff);
	Buff.Format(L"%.01fdB",theApp.m_WS.m_fdBRefLevel);
	m_editRefAttenuation.SetWindowText(Buff);


	m_listRefLevels.ResetContent();
	for(int ii=0;ii<theApp.m_WS.m_ndBRefLevelL;ii++) {
		Buff.Format(L"%.01f dB",theApp.m_WS.m_fdBRefLevelArray[ii]);
		m_listRefLevels.AddString(Buff);
	}

	Buff.Format(_T("%d"),PROFILE->m_nScanResumeLine+1);
	m_editResumeLine.SetWindowText(Buff);

	if(theApp.m_nUnitsPrecision<1) theApp.m_nUnitsPrecision = 1;
	Buff.Format(_T("%.*f %s/s"),theApp.m_nUnitsPrecision-1,PROFILE->m_fScanSpeed*theApp.m_fUnits,theApp.m_Units);
	m_editScanSpeed.SetWindowText(Buff);


	m_checkSwitchOffPumps.SetCheck(theApp.m_LastSettings.bWSPumpsSwitchOff);

	SetAccessPrivelage();
}


BOOL CWorkSpaceDlg::OnSetActive() 
{
	

	theApp.m_LastSettings.nLastWorkspaceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,500,NULL);

	return CResizablePage::OnSetActive();
}

BOOL CWorkSpaceDlg::OnKillActive() 
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}


void CWorkSpaceDlg::FillProfileList()
{
	
	CString Buff;
	int	nSize,nn;


	m_listProfiles.ResetContent();

	if((nSize = theApp.m_WS.m_nProfileL)>0) {

		if(m_pProfileItem == NULL) {
			m_pProfileItem = new ItemProfileData [nSize];
		} else {
			m_pProfileItem = (ItemProfileData *)realloc(m_pProfileItem,nSize * sizeof ItemProfileData);
		}

		for(nn=0; nn<nSize;nn++) {
			if(theApp.m_WS.m_pProfileArray[nn].m_strDescription.IsEmpty()) {
				Buff.Format(L"Profile %d",nn+1);
			} else {
				Buff.Format(L"%s",theApp.m_WS.m_pProfileArray[nn].m_strDescription);
			};
			m_listProfiles.AddString(Buff);
		}
	}

}

void CWorkSpaceDlg::CreatePlayListColumns()
{
	CString Buff;

	m_listPlayOrder.m_pParent = this;
	int nColumnCount = m_listPlayOrder.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) m_listPlayOrder.DeleteColumn(0);

	Buff.Empty();							m_listPlayOrder.InsertColumn(0,Buff, LVCFMT_LEFT,0);
	Buff.LoadString(IDS_Profile);			m_listPlayOrder.InsertColumn(1,Buff, LVCFMT_LEFT,100);
	Buff.LoadString(IDS_Path_Name);			m_listPlayOrder.InsertColumn(2,Buff, LVCFMT_LEFT,160);
	Buff.LoadString(IDS_File_Name);			m_listPlayOrder.InsertColumn(3,Buff, LVCFMT_LEFT,100);
	Buff.LoadString(IDS_Status);			m_listPlayOrder.InsertColumn(7,Buff, LVCFMT_LEFT,100);

	m_listPlayOrder.SetExtendedStyle( m_listPlayOrder.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT   );
	m_listPlayOrder.Init();

}

void CWorkSpaceDlg::FillPlayList()
{
	
	CString Buff;

	m_listPlayOrder.DeleteAllItems();
	for(int ii=0;ii<theApp.m_WS.GetLadderSize();ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listPlayOrder.InsertItem(ii, Buff.GetBuffer(255), ii);
		m_listPlayOrder.SetItemState(ii , 0,LVIS_SELECTED);
	}

	m_listPlayOrder.SetItemState(theApp.m_WS.GetLadderSize()-1 , LVIS_SELECTED,LVIS_SELECTED);
	m_listPlayOrder.EnsureVisible(theApp.m_WS.GetLadderSize()-1,FALSE);
	m_listPlayOrder.SetFocus();
}




BOOL CWorkSpaceDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	CString Buff;
	CPoint point;
	CRect rr;

	GetCursorPos(&point);
	ScreenToClient(&point);

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case 13:

			return TRUE;
		}
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			if((m_bLbDown == true) && (point != m_ptLbDown)) {

				if(m_bBorderSelected == true) {
					GetClientRect(&rr);
					theApp.m_LastSettings.m_fWorkSpaceVDividerPos = (float)point.x * 1.0f / (float)rr.Width();
					AdjustLayout();
				}

				m_ptLbDown = point;
			}
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		m_ptLbDown = point;
		m_bLbDown = true;
		m_listProfiles.GetWindowRect(&rr);
		ScreenToClient(&rr);
		if(rr.PtInRect(point)) {
			m_bProfileSelected = true;
		}

		break;

	case 0x202:	//LBUTTONUP


		m_bLbDown = false;
		m_bProfileSelected = false;
		m_bBorderSelected = false;
		SAFE_DELETE( m_pStatic );

		
		
		break;
	}
	
	
	
	return CResizablePage::PreTranslateMessage(pMsg);
}


void CWorkSpaceDlg::OnNewprofile() 
{
	

	theApp.m_WS.AddProfile();

	FillProfileList();
	FillPlayList();	
}



void CWorkSpaceDlg::OnSaveAsProfile() 
{
	// TODO: Add your command handler code here
	
}

void CWorkSpaceDlg::OnDeleteProfile() 
{
	

	theApp.m_WS.DeleteProfile();
	FillProfileList();
	FillPlayList();
}


void CWorkSpaceDlg::OnRenameProfile() 
{
	
}

void CWorkSpaceDlg::NewWorkspace() 
{
	OnNewWorkspace();
}

void CWorkSpaceDlg::OnNewWorkspace() 
{
	
	
	theApp.m_WS.Zero();
	theApp.m_WS.SelectProfile(-1);
	m_nLadderIndex = 0;
	m_nProfileIndex = 0;
	FillProfileList();
	FillPlayList();
	UpdateAllControls();
}

void CWorkSpaceDlg::SaveAsWorkSpace()
{
	OnSaveAsWorkSpace();
}

void CWorkSpaceDlg::OnSaveAsWorkSpace() 
{
	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = theApp.m_WS.m_FileName;
	if((nPos = theApp.m_WS.m_FileName.ReverseFind('.')) >0) {
		InitialFile = theApp.m_WS.m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".wspace"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Work Space (*.wspace)|*.wspace|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_WorkspacePathName;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_WorkspacePathName = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			theApp.m_WS.Save(&File);
			File.Close();
		}
	}

	
}

void CWorkSpaceDlg::OpenWorkspace()
{
	OnOpenWorkspace();
}


void CWorkSpaceDlg::OnOpenWorkspace() 
{
	
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileThumbNailDlg FileDlg(TRUE,_T(".wspace"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Work Space (*.wspace)|*.wspace|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_WorkspacePathName;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_WorkspacePathName = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			theApp.m_WS.Retrieve(&File);
			theApp.m_WS.SelectProfile(0);
			File.Close();
			Wait.Restore();
			FillProfileList();
			FillPlayList();
			UpdateAllControls();
		}
	}
}

void CWorkSpaceDlg::OnCopyProfile() 
{
	
	theApp.m_WS.CopyProfile();	
}

void CWorkSpaceDlg::OnPasteProfile() 
{
	
	theApp.m_WS.PasteProfile();
	FillProfileList();
	FillPlayList();	
	
}


BOOL CWorkSpaceDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(m_rrVBorder.PtInRect(point)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		return TRUE;
	}
	
	return CResizablePage::OnSetCursor(pWnd, nHitTest, message);
}

void CWorkSpaceDlg::AdjustLayout()
{
	
	CRect rrClient,rrPlayOrder,rrProfile;
	#define	MB 22

	GetClientRect(&rrClient);

	m_listProfiles.GetWindowRect( &rrProfile );
	ScreenToClient( rrProfile);


	m_listPlayOrder.GetWindowRect( &rrPlayOrder);
	rrPlayOrder.top = rrProfile.bottom + MB;
	rrPlayOrder.bottom = rrClient.bottom - MB;
	m_listPlayOrder.SetWindowPos( NULL , rrClient.left, rrPlayOrder.top, rrClient.Width(), rrPlayOrder.Height(), NULL);


}




void CWorkSpaceDlg::OnProfileworkspaceAddprofile() 
{
	
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileThumbNailDlg FileDlg(TRUE,_T(".pro"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			theApp.m_WS.AddProfile();
			theApp.m_WS.SelectLastProfile();
			theApp.m_WS.RetrieveProfile(&File);
			File.Close();
			Wait.Restore();
			FillProfileList();
		}
	}



}

void CWorkSpaceDlg::OnSelchangeListProfiles() 
{
	
	int nSize;

	SAFE_DELETE( m_pEditSpinItem );


	if((nSize = theApp.m_WS.m_nProfileL)>0) {
		m_nProfileIndex = m_listProfiles.GetCurSel();
		theApp.m_WS.SelectProfile(m_nProfileIndex);
		UpdateAllControls();
	}
}


//You cant do this because you then cant do multi selection
void CWorkSpaceDlg::OnClickListInspection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	SAFE_DELETE( m_pEditSpinItem );
	theApp.m_WS.m_nPlayListIndex=pDispInfo->item.mask;

//	m_nProfileIndex = theApp.m_WS.m_pPlayList[m_nLadderIndex].nProfileIndex;
//	theApp.m_WS.SelectProfile(m_nProfileIndex);
	
	m_listPlayOrder.Invalidate(false);
	*pResult = 0;
}

void CWorkSpaceDlg::OnDblclkListInspection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	SAFE_DELETE( m_pEditSpinItem );
	m_nLadderIndex=pDispInfo->item.mask;

	if(m_nLadderIndex<0) {
		return;
	}

	if((m_nLadderIndex>=0) && (m_nLadderIndex<theApp.m_WS.GetLadderSize())) {
		m_listPlayOrder.GetWindowRect(CtrlRect);
		m_listPlayOrder.GetSubItemRect(m_nLadderIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_nCurrentID = IDC_LIST_INSPECTION;
		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(m_nColumn = pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			GetDirectory(NULL, &theApp.m_WS.m_pPlayList[m_nLadderIndex].strComponentPathName);
			m_listPlayOrder.Invalidate();
			break;
		case 3:	m_pEditSpinItem->Initialize(&theApp.m_WS.m_pPlayList[m_nLadderIndex].strComponentFileName);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		case 4: m_pEditSpinItem->Initialize(&theApp.m_WS.m_pPlayList[m_nLadderIndex].strComponentUniqueIdentifier);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 5:
			if(theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask & _STANDARD) {
				theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask &= ~_STANDARD;
			} else {
				theApp.m_WS.ClearAllPlayListStandards();
				theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask |= _STANDARD;
			}
			m_listPlayOrder.Invalidate();
			break;
		case 6:
			theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask & _USE_ULTRASONICS ? theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask &= ~_USE_ULTRASONICS : theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask |= _USE_ULTRASONICS;
			m_listPlayOrder.Invalidate();
			break;
		}
	}	
	
	*pResult = 0;
}


void CWorkSpaceDlg::OnRclickListInspection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;


	GetCursorPos(&Point);
	m_nLadderIndex = pDispInfo->item.mask;

	switch(pDispInfo->item.iItem) {
	default:
		if (menu->LoadMenu(IDR_RB_WS_LADDER_MENU)) {
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	case 3:
		if (menu->LoadMenu(IDR_RB_WS_FILENAME_MENU)) {
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	case 2:
		if (menu->LoadMenu(IDR_RB_WS_PATH_MENU)) {
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	case 4:
		if (menu->LoadMenu(IDR_RB_WS_STATUS_MENU)) {
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;

	}
	delete menu;
	
	
	*pResult = 0;
}

void CWorkSpaceDlg::OnGetdispinfoListInspection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn = pDispInfo->item.iItem;
	CString Buff;
	const wchar_t Tick = 0x2714;

	if(theApp.m_WS.GetLadderSize()<=0) return;

	switch(pDispInfo->item.iSubItem) {
	case 0: swprintf_s(str,L"%d",nn+1);
		break;
	case 1: theApp.m_WS.GetProfileDescription(theApp.m_WS.m_pPlayList[nn].nProfileIndex,&Buff);
		if(nn == theApp.m_WS.m_nPlayListIndex) {
			swprintf_s(str,L"%s %wc",Buff.GetBuffer(),Tick);
		} else {
			swprintf_s(str,L"%s",Buff.GetBuffer());
		}
		break;
	case 2:  swprintf_s(str,L"%s",theApp.m_WS.m_pPlayList[nn].strComponentPathName.GetBuffer());
		break;
	case 3:  swprintf_s(str,L"%s",theApp.m_WS.m_pPlayList[nn].strComponentFileName.GetBuffer());
		break;
//	case 4: swprintf_s(str,L"%s",theApp.m_WS.m_pPlayList[nn].strComponentUniqueIdentifier);
//		break;
//	case 5: 
//		theApp.m_WS.m_pPlayList[nn].nUsageMask & _STANDARD ? nStrType = 1 : nStrType = 0;
//		swprintf_s(str,L"%s",m_strStandard[nStrType]);
//		break;
//	case 6:
//		if(theApp.m_WS.m_pPlayList[nn].nUsageMask & _USE_ULTRASONICS) {
//			ZeroMemory(str,sizeof str);
//			wcsncpy(str,&Tick,1);
//		} else {
//			str[0]=0;
//		}
//		break;
	case 4:
		swprintf_s(str, L"%s", m_strStatus[theApp.m_WS.m_pPlayList[nn].nStatus].GetBuffer());
		break;

	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CWorkSpaceDlg::OnProfileworkspaceSendtoladder() 
{
	
	
	theApp.m_WS.AddToLadder();
	FillPlayList();
}

void CWorkSpaceDlg::OnLadderClearcomponentvariables() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier.Empty();
			theApp.m_WS.m_pPlayList[ii].strComponentPathName.Empty();
			theApp.m_WS.m_pPlayList[ii].strComponentFileName.Empty();
		}
	}
	m_listPlayOrder.Invalidate();
	
}

void CWorkSpaceDlg::OnLadderClearfilenames() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentFileName.Empty();
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderClearpathnames() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentPathName.Empty();
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderClearuniqueidentifiers() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier.Empty();
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderDelete() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		theApp.m_WS.DeleteLadder(nStartIndex,nFinishIndex);
		FillPlayList();
	}
}



void CWorkSpaceDlg::OnLadderFilenamepathname() 
{
	
	int nPos,nStartIndex,nFinishIndex;
	CUSLFile File;
	CFileException e;
	CString PathName,FileName,Buff;

	CFileDialog FileDlg(FALSE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|CIP (*.cip)|*.cip|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		PathName=FileDlg.GetPathName();
		FileName=FileDlg.GetFileTitle();

		if((nPos = PathName.ReverseFind('\\')) >0) {
			PathName.GetBufferSetLength(nPos);
		}
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			theApp.m_StorePath[0] = PathName;
		}
		if((nPos = FileName.ReverseFind('.')) >0) {
			FileName.GetBufferSetLength(nPos);
		}

		if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {

			if(nFinishIndex - nStartIndex) {
				for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
					Buff.Format(L"%s-%d",FileName,ii-nStartIndex + 1);
					theApp.m_WS.m_pPlayList[ii].strComponentFileName = Buff;
					theApp.m_WS.m_pPlayList[ii].strComponentPathName = PathName;
				}
			} else {
				theApp.m_WS.m_pPlayList[nStartIndex].strComponentFileName = FileName;
				theApp.m_WS.m_pPlayList[nStartIndex].strComponentPathName = PathName;
			}
				
			m_listPlayOrder.Invalidate();
		}
	}
	
}

void CWorkSpaceDlg::OnLadderCopyfilename() 
{
	
	int nStartIndex,nFinishIndex;
	CString FileName;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			if(!theApp.m_WS.m_pPlayList[ii].strComponentFileName.IsEmpty()) {
				FileName = theApp.m_WS.m_pPlayList[ii].strComponentFileName;
				break;
			}
		}
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentFileName = FileName;
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderCopyincfilename() 
{
	
	int nStartIndex,nFinishIndex;
	CString FileName,Buff;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			if(!theApp.m_WS.m_pPlayList[ii].strComponentFileName.IsEmpty()) {
				FileName = theApp.m_WS.m_pPlayList[ii].strComponentFileName;
				break;
			}
		}
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			Buff.Format(L"%s-%d",FileName,ii-nStartIndex+1);
			theApp.m_WS.m_pPlayList[ii].strComponentFileName = Buff;
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderCopyincuniqueidentifier() 
{
	
	int nStartIndex,nFinishIndex;
	CString Identifier,Buff;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			if(!theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier.IsEmpty()) {
				Identifier = theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier;
				break;
			}
		}
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			Buff.Format(L"%s-%d",Identifier,ii-nStartIndex+1);
			theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier = Buff;
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderCopypathname() 
{
	
	int nStartIndex,nFinishIndex;
	CString PathName;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			if(!theApp.m_WS.m_pPlayList[ii].strComponentPathName.IsEmpty()) {
				PathName = theApp.m_WS.m_pPlayList[ii].strComponentPathName;
				break;
			}
		}
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentPathName = PathName;
		}
	}
	m_listPlayOrder.Invalidate();
}

void CWorkSpaceDlg::OnLadderCopyuniqueidentifier() 
{
	
	int nStartIndex,nFinishIndex;
	CString Identifier;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			if(!theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier.IsEmpty()) {
				Identifier = theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier;
				break;
			}
		}
		for(int ii=nStartIndex;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++) {
			theApp.m_WS.m_pPlayList[ii].strComponentUniqueIdentifier = Identifier;
		}
	}
	m_listPlayOrder.Invalidate();
	
}

void CWorkSpaceDlg::OnLadderCut() 
{
	
	int nStartIndex,nFinishIndex,ii,jj;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		SAFE_DELETE( m_pnCutArray );
		m_pnCutArray = new int[m_nCutPasteL = nFinishIndex-nStartIndex+1];
		for(ii=nStartIndex,jj=0;ii<=nFinishIndex && ii<theApp.m_WS.GetLadderSize();ii++,jj++) {
			m_pnCutArray[jj] = theApp.m_WS.m_pPlayList[ii].nProfileIndex;
		}
		theApp.m_WS.DeleteLadder(nStartIndex,nFinishIndex);
		FillPlayList();
	}

}

void CWorkSpaceDlg::OnLadderPaste() 
{
	
	int nStartIndex,nFinishIndex;

	if((m_pnCutArray!=NULL) && (m_nCutPasteL>0)) {
		if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
			theApp.m_WS.AddToLadder(nStartIndex,m_nCutPasteL,m_pnCutArray);
		} else {
			theApp.m_WS.AddToLadder(theApp.m_WS.GetLadderSize(),m_nCutPasteL,m_pnCutArray);
		}
		FillPlayList();
	}
}

void CWorkSpaceDlg::EraseAllFileNames()
{
	

	theApp.m_WS.EraseAllLadderFileNames();
	m_listPlayOrder.Invalidate();

	ZeroMemory(theApp.m_WS.m_fdBRefLevelArray, sizeof theApp.m_WS.m_fdBRefLevelArray);
	theApp.m_WS.m_ndBRefLevelL = 0;
	UpdateAllControls();

}






void CWorkSpaceDlg::GetDirectory(CEdit *pEdit, CString *pInitDir)
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


void CWorkSpaceDlg::OnPathCopy() 
{
	
	m_strCopy = theApp.m_WS.m_pPlayList[m_nLadderIndex].strComponentPathName;
}

void CWorkSpaceDlg::OnPathPaste() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int nIndex = nStartIndex; nIndex<=nFinishIndex; nIndex++) {
			theApp.m_WS.m_pPlayList[nIndex].strComponentPathName = m_strCopy;
		}
	}
	m_listPlayOrder.Invalidate();	
}


void CWorkSpaceDlg::OnFilenameCopy() 
{
	
	m_strCopy = theApp.m_WS.m_pPlayList[m_nLadderIndex].strComponentFileName;
}

void CWorkSpaceDlg::OnFilenamePaste() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int nIndex = nStartIndex; nIndex<=nFinishIndex; nIndex++) {
			theApp.m_WS.m_pPlayList[nIndex].strComponentFileName = m_strCopy;
		}
	}
	m_listPlayOrder.Invalidate();	
	
}

void CWorkSpaceDlg::OnFilenamePastea1() 
{
	
	int nPos;
	char cMax;
	CString Buff,Temp;
	int nStartIndex,nFinishIndex;
	int nMax = -1;

	Buff = m_strCopy;
	cMax = 'a' - 1;
	if((nPos = m_strCopy.Find('-')) > 1) {
		cMax = (char)m_strCopy.GetAt(nPos+1);

		if(isdigit(cMax)) {
			Buff = m_strCopy.Right(m_strCopy.GetLength()-nPos-1);
			swscanf_s(Buff,L"%d",&nMax);
		};

		Buff = m_strCopy.Left(nPos);
	}


	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int nIndex = nStartIndex; nIndex<=nFinishIndex; nIndex++) {
			if(nMax>-1) {
				Temp.Format(L"-%d",++nMax);
			} else {
				Temp.Format(L"-%c",++cMax);
			}
			theApp.m_WS.m_pPlayList[nIndex].strComponentFileName = Buff + Temp;
		}
	}
	m_listPlayOrder.Invalidate();	
}

void CWorkSpaceDlg::OnStatusReset() 
{
	
	int nStartIndex,nFinishIndex;

	if(GetStartFinishLadderIndex(&nStartIndex, &nFinishIndex)) {
		for(int nIndex = nStartIndex; nIndex<=nFinishIndex; nIndex++) {
			theApp.m_WS.m_pPlayList[nIndex].nStatus = 0;
		}
	}
	m_listPlayOrder.Invalidate();	
}


bool CWorkSpaceDlg::GetStartFinishLadderIndex(int *nStartIndex, int *nFinishIndex)
{
	POSITION pos = m_listPlayOrder.GetFirstSelectedItemPosition();
	if(pos) {
		*nFinishIndex=*nStartIndex=m_listPlayOrder.GetNextSelectedItem(pos);
		while(pos) {
			*nFinishIndex = m_listPlayOrder.GetNextSelectedItem(pos);
		}
		return true;
	}
	*nStartIndex = 0;
	*nFinishIndex = 0;
	return false;
}

int CWorkSpaceDlg::GetStartLadderIndex()
{

	return 0;
}


void CWorkSpaceDlg::OnDeltaposSpinNegativeAttenuation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = theApp.m_WS.m_fAttenuationRange[0];

	theApp.m_WS.m_fAttenuationRange[0] += ((float)pNMUpDown->iDelta * 0.1f);
	MinMax(&theApp.m_WS.m_fAttenuationRange[0],-10.0f,0.0f);

	if(fTemp - theApp.m_WS.m_fAttenuationRange[0]) {
		Buff.Format(L"%.01f dB",theApp.m_WS.m_fAttenuationRange[0]);	m_editNegativeAttenuation.SetWindowText(Buff);
	}
	*pResult = 0;
}

void CWorkSpaceDlg::OnDeltaposSpinPositiveAttenuation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = theApp.m_WS.m_fAttenuationRange[1];

	theApp.m_WS.m_fAttenuationRange[1] += ((float)pNMUpDown->iDelta * 0.1f);
	MinMax(&theApp.m_WS.m_fAttenuationRange[1],0.0f,10.0f);

	if(fTemp - theApp.m_WS.m_fAttenuationRange[1]) {
		Buff.Format(L"+%.01f dB",theApp.m_WS.m_fAttenuationRange[1]);	m_editPositiveAttenuation.SetWindowText(Buff);
	}
	*pResult = 0;
}

void CWorkSpaceDlg::OnSelchangeComboMaxScanTime() 
{
	

	theApp.m_LastSettings.osTimeBetweenStandards = m_comboMaxScanTime.GetCurSel() * m_nSecondsInAStep;
	
}

void CWorkSpaceDlg::OnChangeEditNegativeAttenuation() 
{
	
	CString Buff;

	m_editNegativeAttenuation.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_WS.m_fAttenuationRange[0]);
	MinMax(&theApp.m_WS.m_fAttenuationRange[0],-10.0f,0.0f);
}

void CWorkSpaceDlg::OnChangeEditPositiveAttenuation() 
{
	
	CString Buff;

	m_editPositiveAttenuation.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_WS.m_fAttenuationRange[1]);
	MinMax(&theApp.m_WS.m_fAttenuationRange[1],0.0f,10.0f);
}

void CWorkSpaceDlg::OnChangeEditCorner0() 
{
	
	CString Buff;
	int ii,nH,nV;

	m_editCorner0.GetWindowText(Buff);

	for(ii=0;ii<Buff.GetLength();ii++) {
		if(!(isdigit(Buff.GetAt(ii)) || Buff.GetAt(ii) == '.' || Buff.GetAt(ii) == '-' || Buff.GetAt(ii) == ' ')) {
			Buff.Delete(ii);
			ii--;
		}
	}

	int nCount = swscanf_s(Buff,L"%d %d",&nH,&nV);
	if(nCount == 2) {
		theApp.m_WS.m_rrHistogramSample.left = nH;
		theApp.m_WS.m_rrHistogramSample.bottom = nV;
	}

}

void CWorkSpaceDlg::OnChangeEditCorner1() 
{
	
	CString Buff;
	int ii,nH,nV;

	m_editCorner1.GetWindowText(Buff);

	for(ii=0;ii<Buff.GetLength();ii++) {
		if(!(isdigit(Buff.GetAt(ii)) || Buff.GetAt(ii) == '.' || Buff.GetAt(ii) == '-' || Buff.GetAt(ii) == ' ')) {
			Buff.Delete(ii);
			ii--;
		}
	}

	int nCount = swscanf_s(Buff,L"%d %d",&nH,&nV);
	if(nCount == 2) {
		theApp.m_WS.m_rrHistogramSample.right = nH;
		theApp.m_WS.m_rrHistogramSample.top = nV;
	}
}

void CWorkSpaceDlg::SetAccessPrivelage()
{
	if (GetSafeHwnd() == NULL) return;
	if(m_hWnd == NULL) return;

	

	bool bFlag = false;
	if(theApp.m_nLogonLevelMask &  theApp.m_cAccess[IDD_WORKSPACE_DLG]) {
		bFlag = true;
	}


	if(bFlag == false) {
		m_comboMaxScanTime.EnableWindow(bFlag);
		m_spinPositiveAttenuation.EnableWindow(bFlag);
		m_spinNegativeAttenuation.EnableWindow(bFlag);
		m_editPositiveAttenuation.EnableWindow(bFlag);
		m_editNegativeAttenuation.EnableWindow(bFlag);
//		m_editCorner1.EnableWindow(bFlag);
//		m_editCorner0.EnableWindow(bFlag);
		m_listPlayOrder.EnableWindow(bFlag);
		m_listProfiles.EnableWindow(bFlag);
//		m_listRefLevels.EnableWindow(bFlag);
	} else {

		theApp.m_WS.m_nScanMethod == 0 ? bFlag = false : bFlag = true;;

		m_comboMaxScanTime.EnableWindow(bFlag);
		m_spinPositiveAttenuation.EnableWindow(bFlag);
		m_spinNegativeAttenuation.EnableWindow(bFlag);
		m_editPositiveAttenuation.EnableWindow(bFlag);
		m_editNegativeAttenuation.EnableWindow(bFlag);
//		m_editCorner1.EnableWindow(bFlag);
//		m_editCorner0.EnableWindow(bFlag);
	}
	int nStyle = m_listRefLevels.GetStyle();
//	m_listRefLevels.ModifyStyleEx(0,nStyle | EM_SETREADONLY | ES_READONLY );
//	m_listRefLevels.EnableWindow(false);
//		m_listPlayOrder.EnableWindow(false);
//		m_listProfiles.EnableWindow(false);
}








void CWorkSpaceDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	int nMenu = IDR_RB_PROFILESELETED_MENU;
	int nSize;
	CRect rr;

	if((nSize = theApp.m_WS.m_nProfileL)>0) {
		m_nProfileIndex = m_listProfiles.GetCurSel();
		theApp.m_WS.SelectProfile(m_nProfileIndex);
	}
	GetCursorPos(&Point);

	m_listProfiles.GetWindowRect(rr);
	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(nMenu)) {

			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		}
	}
	delete menu;
	
	
}

void CWorkSpaceDlg::OnDblclkListProfiles() 
{
	
	int nSize;
	CRect CtrlRect,Rect;

	SAFE_DELETE( m_pEditSpinItem );

	if((nSize = theApp.m_WS.m_nProfileL)>0) {
		m_nProfileIndex = m_listProfiles.GetCurSel();
		theApp.m_WS.SelectProfile(m_nProfileIndex);


		if((m_nProfileIndex>=0) && (m_nProfileIndex<nSize)) {
			m_listProfiles.GetWindowRect(CtrlRect);
			m_listProfiles.GetItemRect(m_nProfileIndex,&Rect);
			Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
			Rect.InflateRect(0,1,0,2);

			m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

			m_nCurrentID = IDC_LIST_PROFILES;
			m_pEditSpinItem->Initialize(&theApp.m_WS.m_pProfileArray[m_nProfileIndex].m_strDescription);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		}
	}
	
}

long CWorkSpaceDlg::ItemChanged(unsigned int nTemp,long nlTempt)
{
	
	int ii;

	switch(m_nCurrentID) {
	case IDC_LIST_INSPECTION:
		switch(m_nColumn) {
		case 5:
			for(ii=0;ii<theApp.m_WS.m_nPlayListL;ii++) theApp.m_WS.m_pPlayList[ii].nUsageMask &= ~_STANDARD;
			m_nStrStandard == 1 ? theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask |= _STANDARD : theApp.m_WS.m_pPlayList[m_nLadderIndex].nUsageMask &= ~_STANDARD;
			m_listPlayOrder.RedrawWindow(false);
			break;
		}
		break;
	case IDC_LIST_PROFILES:
		m_listProfiles.DeleteString(m_nProfileIndex);
		m_listProfiles.InsertString(m_nProfileIndex,theApp.m_WS.m_pProfileArray[m_nProfileIndex].m_strDescription);
		m_listPlayOrder.Invalidate(false);
		break;
	}

	return 0;
}


//Readonly for CListBox
HBRUSH CWorkSpaceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd == &m_listRefLevels) {
		if (nCtlColor == CTLCOLOR_LISTBOX ) { 
		   HBRUSH br = ::GetSysColorBrush(COLOR_BTNFACE); 
		   pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE)); 
		   return br; 
		  }
	}
	return hbr;
}

void CWorkSpaceDlg::OnButtonInspectionCriteria() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff,strError;
	CViewPage *pViewPage;
	int nImage = 0;


	for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		if(pFrame->m_pViewSheet[nn] != NULL) nImage = nn;
	}

	strError.LoadString(IDS_Error);
	if(pFrame->m_pViewSheet[nImage] == NULL) {
		Buff.LoadString(IDS_There_is_no_acceptable_image_open);
		MessageBox(Buff,strError,MB_ICONERROR);
		return;
	}
	if(pFrame->m_pViewSheet[nImage]->m_pHistogramDlg == NULL) {
		Buff.LoadString(IDS_The_histogram_needs_to_be_open);
		MessageBox(Buff,strError,MB_ICONERROR);
		return;
	}

	theApp.m_WS.m_nRefImage = pFrame->m_pViewSheet[nImage]->GetActiveIndex();
	theApp.m_WS.m_strRefImageName = pFrame->m_pViewSheet[nImage]->m_Name[theApp.m_WS.m_nRefImage];
	pViewPage = pFrame->m_pViewSheet[nImage]->m_pViewPage[theApp.m_WS.m_nRefImage];
	theApp.m_WS.m_rrHistogramSample = pViewPage->m_rrHistogramSample;
	theApp.m_WS.m_SampleSize.cx = pViewPage->m_pData->m_nSamplesLine;
	theApp.m_WS.m_SampleSize.cy = pViewPage->m_pData->m_nNumberLines;
	theApp.m_WS.m_ScanSize.cx = (int)pViewPage->m_pData->m_fFastScanSize;
	theApp.m_WS.m_ScanSize.cy = (int)pViewPage->m_pData->m_fSlowScanSize;
	theApp.m_WS.m_fdBRefLevel = pViewPage->m_pData->m_Hist.fRefAmp;

	if((pViewPage->m_rrHistogramSample.Width() ==0) || (pViewPage->m_rrHistogramSample.Height() == 0)){
		Buff.LoadString(IDS_The_height_or_width_is_too_small);
		MessageBox(Buff,strError,MB_ICONERROR);
		return;
	}
	Buff.Format(L"Ref Level for calibration %.01f dB Image: %s",theApp.m_WS.m_fdBRefLevel, theApp.m_WS.m_strRefImageName);
	theApp.ActivityLog(Buff);

	theApp.m_WS.SetCalibrationTime();

	UpdateAllControls();
	Invalidate(false);


}

void CWorkSpaceDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	CRect rr,rrComponent,rrRef;
	float fScale;
	float fX,fY;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);


	if(theApp.m_WS.IsPartitionScan() == true) {
		dc.FillRect(&rr,&CBrush(RGB(255,255,255)));

		if(theApp.m_WS.m_ScanSize.cx >= theApp.m_WS.m_ScanSize.cy) {
			fScale = (float)theApp.m_WS.m_ScanSize.cx / (float)rr.Width();
		} else {
			fScale = (float)theApp.m_WS.m_ScanSize.cy / (float)rr.Height();
		}

		if(fScale != 0.0f) {
			//Draw the component
			rrComponent.left = rr.CenterPoint().x - (int)((float)(theApp.m_WS.m_ScanSize.cx / 2) / fScale);
			rrComponent.top = rr.CenterPoint().y - (int)((float)(theApp.m_WS.m_ScanSize.cy / 2) / fScale);
			rrComponent.right = (int)((float)theApp.m_WS.m_ScanSize.cx / fScale) + rrComponent.left;
			rrComponent.bottom = (int)((float)theApp.m_WS.m_ScanSize.cy / fScale) + rrComponent.top;
			rrComponent.NormalizeRect();
			dc.FillRect(rrComponent,&CBrush(RGB(0,128,0)));

			if(theApp.m_WS.m_SampleSize.cx != 0 && theApp.m_WS.m_SampleSize.cy != 0) {
				fX = (float)theApp.m_WS.m_ScanSize.cx * ((float)theApp.m_WS.m_rrHistogramSample.left / (float)theApp.m_WS.m_SampleSize.cx);
				rrRef.left = (int)(fX / fScale) + rrComponent.left;
				fX = (float)theApp.m_WS.m_ScanSize.cx * ((float)theApp.m_WS.m_rrHistogramSample.right / (float)theApp.m_WS.m_SampleSize.cx);
				rrRef.right = (int)(fX / fScale) + rrComponent.left;

				fY = (float)theApp.m_WS.m_ScanSize.cy * ((float)theApp.m_WS.m_rrHistogramSample.top / (float)theApp.m_WS.m_SampleSize.cy);
				rrRef.top = (int)(fY / fScale);
				fY = (float)theApp.m_WS.m_ScanSize.cy * ((float)theApp.m_WS.m_rrHistogramSample.bottom / (float)theApp.m_WS.m_SampleSize.cy);
				rrRef.bottom = (int)(fY / fScale);

				switch(theApp.m_LastSettings.nImageOrientation) {
				case 0: rrRef.top = rrComponent.top + rrRef.top;
					rrRef.bottom = rrComponent.top + rrRef.bottom;
					break;
				case 1: rrRef.top = rrComponent.bottom - rrRef.top;
					rrRef.bottom = rrComponent.bottom - rrRef.bottom;
					break;
				}

				rrRef.NormalizeRect();
				dc.FillRect(rrRef,&CBrush(RGB(200,200,0)));
			}
		}
	} else {

		dc.FillRect(&rr,&CBrush(::GetSysColor(COLOR_BTNFACE)));
	}

}

void CWorkSpaceDlg::OnTimer(UINT nIDEvent) 
{
	
	CString Buff;
	int nDiff;
	static CString strLastBuff; 

	if(m_nOldScanLine-theApp.m_Thread.m_nScanLine) {
		Buff.Format(_T("%d of %d"),(m_nOldScanLine=theApp.m_Thread.m_nScanLine) + 1, PROFILE->m_nScanFinishLine + 1);
		m_editScanLine.SetWindowText(Buff);
	}

	if(m_osLastCalibration - theApp.m_LastSettings.osLastStandardCalibration) {
		CTime LastCalTime = theApp.m_LastSettings.osLastStandardCalibration;
		Buff = LastCalTime.Format(L"%Hh %M");
		m_editLastCalTime.SetWindowText(Buff);
	}

	if(m_nLastTimeDiff - (nDiff = theApp.m_WS.TimeTillCalibration(&Buff))) {
		if(strLastBuff != Buff) {
			m_editTimeDiff.SetWindowText(Buff);
		}
	}

	strLastBuff = Buff; 
	m_osLastCalibration = theApp.m_LastSettings.osLastStandardCalibration;
	m_nLastTimeDiff = nDiff;
	m_nOldScanLine = theApp.m_Thread.m_nScanLine;

	CResizablePage::OnTimer(nIDEvent);
}



void CWorkSpaceDlg::OnChangeEditResumeLine() 
{
	
	CString	Buff;

	m_editResumeLine.GetWindowText(Buff);
	PROFILE->m_nScanResumeLine = _ttoi(Buff) -1;
	if(PROFILE->m_nScanResumeLine<PROFILE->m_nScanStartLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanStartLine;
	}
	if(PROFILE->m_nScanResumeLine>=PROFILE->m_nScanFinishLine) {
		PROFILE->m_nScanResumeLine = PROFILE->m_nScanFinishLine;
	}
}

void CWorkSpaceDlg::OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta);
		if(PROFILE->m_fScanSpeed<1.0f) PROFILE->m_fScanSpeed = 1.0f;
		if(PROFILE->m_fScanSpeed>1000.0f) PROFILE->m_fScanSpeed = 1000.0f;
	} else {
		PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * 1.016f);
		if(PROFILE->m_fScanSpeed<25.4f) PROFILE->m_fScanSpeed = 25.4f;
		if(PROFILE->m_fScanSpeed>1016.0f) PROFILE->m_fScanSpeed = 1016.0f;
	}
	UpdateAllControls();

	
	*pResult = 0;
}

void CWorkSpaceDlg::OnChangeEditScanSpeed() 
{
	
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	m_editScanSpeed.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp/=theApp.m_fUnits;
	if(fTemp<1.0f) fTemp = 1.0f;
	if(fTemp>1000.0f) fTemp = 1020.0f;
	if(PROFILE->m_fScanSpeed - fTemp) {
		PROFILE->m_fScanSpeed = fTemp;
	}

}

void CWorkSpaceDlg::OnCheckSwitchOffPumps() 
{
	
	
	if(m_checkSwitchOffPumps.GetCheck() == 0) {
		theApp.m_LastSettings.bWSPumpsSwitchOff = false;
	} else {
		theApp.m_LastSettings.bWSPumpsSwitchOff = true;
	}	

}
