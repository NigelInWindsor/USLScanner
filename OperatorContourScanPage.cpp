// OperatorContourScanPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "OperatorContourScanPage.h"
#include "MainFrm.h"
#include "FileThumbNailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COperatorContourScanPage property page

IMPLEMENT_DYNCREATE(COperatorContourScanPage, CPropertyPage)

COperatorContourScanPage::COperatorContourScanPage() : CPropertyPage(COperatorContourScanPage::IDD)
{
	//{{AFX_DATA_INIT(COperatorContourScanPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

COperatorContourScanPage::~COperatorContourScanPage()
{
}

void COperatorContourScanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COperatorContourScanPage)
	DDX_Control(pDX, IDC_EDIT_BLADE_TYPE, m_editBladeType);
	DDX_Control(pDX, IDC_EDIT_BATCH_ID, m_editBatchID);
	DDX_Control(pDX, IDC_CHECK_USE_BRANE, m_checkUseBrane);
	DDX_Control(pDX, IDC_EDIT_DATE, m_editDate);
	DDX_Control(pDX, IDC_EDIT_OPERATOR, m_editOperator);
	DDX_Control(pDX, IDC_EDIT_SERIAL_NUMBER, m_editSerialNumber);
	DDX_Control(pDX, IDC_EDIT_PART_NUMBER, m_editPartNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COperatorContourScanPage, CPropertyPage)
	//{{AFX_MSG_MAP(COperatorContourScanPage)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EDIT_PART_NUMBER, OnChangeEditPartNumber)
	ON_BN_CLICKED(IDC_BUTTON_PART_NUMBER, OnButtonPartNumber)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_NUMBER, OnButtonSerialNumber)
	ON_COMMAND(ID_BUTTON_START, OnButtonStart)
	ON_COMMAND(ID_BUTTON_RESUME, OnButtonResume)
	ON_COMMAND(ID_BUTTON_ABORT, OnButtonAbort)
	ON_BN_CLICKED(IDC_CHECK_USE_BRANE, OnCheckUseBrane)
	ON_EN_CHANGE(IDC_EDIT_SERIAL_NUMBER, OnChangeEditSerialNumber)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperatorContourScanPage message handlers
int COperatorContourScanPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_OPERATOR_SCAN_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndRebar.Create(this,NULL,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	GetClientRect(rect);

	m_wndRebar.SetWindowPos(&wndTop   , 0,0,200,30,SWP_SHOWWINDOW);
	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL COperatorContourScanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COperatorContourScanPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	if(theApp.m_pScanData != NULL) {
		m_editDate.SetWindowText(theApp.m_pScanData->m_TestDateTime);
		m_editOperator.SetWindowText(theApp.m_pScanData->m_Operator);
//		m_editBatchID.SetWindowText(theApp.m_pScanData->m_Details[MES_BATCH_ID]);
//		m_editBladeType.SetWindowText(theApp.m_pScanData->m_Details[MES_BLADE_TYPE]);
	}

	if(PROFILE->m_nBrainLineL > 0) {
		m_checkUseBrane.SetCheck(theApp.m_LastSettings.bUseBrainCompensation & 1);
	} else {
		m_checkUseBrane.SetCheck(theApp.m_LastSettings.bUseBrainCompensation = FALSE);
	}

	SetToolBarCheckedState();
}

BOOL COperatorContourScanPage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}


void COperatorContourScanPage::SetToolBarCheckedState()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nIndex;

	if(pFrame->m_pScanView == NULL) {
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_START))>=0) m_wndToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RESUME))>=0) m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
		if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ABORT))>=0) m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
	} else {
		if(theApp.m_Thread.m_nThreadAction == CONTOUR_SCAN) {
			if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_START))>=0) m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED|TBBS_CHECKED);
			if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RESUME))>=0) m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
			if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ABORT))>=0) m_wndToolBar.SetButtonStyle(nIndex,0);
		} else {
			if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_START))>=0) m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED|TBBS_CHECKED);
			if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RESUME))>=0) m_wndToolBar.SetButtonStyle(nIndex,0);
			if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ABORT))>=0) m_wndToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
		}
	}

}

BOOL COperatorContourScanPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

 	switch(nID) {
	case ID_BUTTON_START:  strTipText.LoadString(IDS_Start_Scan);
		break;
	case ID_BUTTON_RESUME:  strTipText.LoadString(IDS_Resume_Scan);
		break;
	case ID_BUTTON_ABORT:  strTipText.LoadString(IDS_Abort);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}
void COperatorContourScanPage::OnChangeEditSerialNumber() 
{
	m_editSerialNumber.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}

void COperatorContourScanPage::OnChangeEditPartNumber() 
{
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	m_editPartNumber.GetWindowText(Buff);
	if(pFrame->TryToLoadProfile(Buff) == TRUE) {
		PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL - 1;
		::SetFocus(m_editSerialNumber.m_hWnd);
	}
}

void COperatorContourScanPage::OnButtonPartNumber() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;
	int nNewLength;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

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
			PROFILE->Retrieve(&File);
			File.Close();
			Wait.Restore();
			UpdateAllControls();
			pFrame->SendMessage(UI_PROFILE_OPENED,(WPARAM)this,NULL);
	
			Buff = PROFILE->m_FileName;
			if((nNewLength=Buff.Find(_T(".pro"),0))>0) {
				Buff.GetBufferSetLength(nNewLength);
			}
			m_editPartNumber.SetWindowText(Buff);

			theApp.ActivityLog(L"Profile loaded : " + FilePath);
		}
	}
	
}

void COperatorContourScanPage::OnButtonSerialNumber() 
{
	m_editSerialNumber.GetWindowText(theApp.m_Data.m_FileName);
	theApp.m_Data.m_FilePath = theApp.m_StorePath[0] + L"\\" + theApp.m_Data.m_FileName + L".dat";
}

void COperatorContourScanPage::OnButtonStart() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(theApp.m_Data.MESRetrieveConsumable(theApp.m_Data.m_FileName) == false) {
		UpdateAllControls();
		return;
	}

	PROFILE->m_nScanStartLine = 0;
	PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL - 1;
	theApp.m_nScanWhat = 0;
	PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;


	pFrame->StartScan(CONTOUR_SCAN);
	UpdateAllControls();
	
}

void COperatorContourScanPage::OnButtonResume() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	PROFILE->m_nScanResumeLine = PROFILE->FindNearestScanLine(CpSurface);

	pFrame->StartScan(CONTOUR_SCAN);
	SetToolBarCheckedState();
}

void COperatorContourScanPage::OnButtonAbort() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_Thread.m_bAbortFlag = TRUE;
	theApp.ActivityLog(L"Scan aborted");
	Sleep(1500);
	SetToolBarCheckedState();
}

void COperatorContourScanPage::OnCheckUseBrane() 
{
	if(PROFILE->m_nBrainLineL > 0) {
		theApp.m_LastSettings.bUseBrainCompensation = m_checkUseBrane.GetCheck() & 1;
	} else {
		m_checkUseBrane.SetCheck(theApp.m_LastSettings.bUseBrainCompensation = FALSE);
		MessageBox(_T("There is no acceptable brane defined"),_T("Brane Error"),NULL);
	}

}

