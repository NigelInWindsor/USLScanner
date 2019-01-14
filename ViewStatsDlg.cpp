// ViewStatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ViewStatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewStatsDlg dialog


CViewStatsDlg::CViewStatsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CData* pData)
	: CDialog(CViewStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewStatsDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pData = pData;
}


void CViewStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewStatsDlg)
	DDX_Control(pDX, IDC_EDIT_MACHINE_LOCATION, m_editMachinLocation);
	DDX_Control(pDX, IDC_EDIT_MACHINE_NAME, m_editMachineName);
	DDX_Control(pDX, IDC_EDIT_SENTENCE, m_editSentence);
	DDX_Control(pDX, IDC_EDIT_OPERATOR, m_editOperator);
	DDX_Control(pDX, IDC_EDIT_FILE, m_editFile);
	DDX_Control(pDX, IDC_EDIT_TEST_DATE, m_editTestDate);
	DDX_Control(pDX, IDC_EDIT_START_LINE, m_editStartLine);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_AXIS, m_editSlowAxis);
	DDX_Control(pDX, IDC_EDIT_PROFILE_NAME, m_editProfileName);
	DDX_Control(pDX, IDC_EDIT_LINE_INCREMENT, m_editLineIncrement);
	DDX_Control(pDX, IDC_EDIT_FINISH_LINE, m_editFinishLine);
	DDX_Control(pDX, IDC_EDIT_FAST_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_EDIT_FAST_AXIS, m_editFastAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CViewStatsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStatsDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CViewUltrasonicsDlg message handlers

void CViewStatsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CViewStatsDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CViewStatsDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}


void CViewStatsDlg::OnDestroy() 
{
	

	if (CViewStatsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CViewStatsDlg::IDD]);
	}

	CDialog::OnDestroy();
	*m_pDlg = NULL;			
}

BOOL CViewStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateAllControls();


	return true;
}


void CViewStatsDlg::UpdateAllControls()
{

	
	int i=0;
	CString Buff;
	CString Temp;

	Buff.Format(_T("%s"),m_pData->m_FilePath);
	m_editFile.SetWindowText(Buff);

	Buff.Format(_T("%s"),m_pData->m_Operator);
	m_editOperator.SetWindowText(Buff);

	CTime time(m_pData->m_osTestDateTime);
	Buff = time.Format(_T("%d/%m/%Y %Hh%M"));
	m_editTestDate.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_pData->m_nScanStartLine + 1);
	m_editStartLine.SetWindowText(Buff);

	Buff.Format(_T("%0.1fmm"),m_pData->m_fSlowIncrement);
	m_editSlowIncrement.SetWindowText(Buff);

	Buff.Format(_T(""));
	Temp.Format(_T(""));
	for(i=0;i<32;i++){
		if((theApp.m_Axes[i].nPhysicalAxis-1) == m_pData->m_nSlowAxis) {
			Temp.Format(L"%s",theApp.m_Axes[i].cName);
//			theApp.CharToCString(theApp.m_Axes[i].cName, Temp);
			Buff.Format(_T("%s (%d)"),Temp,m_pData->m_nSlowAxis);
			break;
		}
	}
	m_editSlowAxis.SetWindowText(Buff);

	m_editProfileName.SetWindowText(m_pData->m_ProfileName);

	Buff.Format(_T("%d"),m_pData->m_nLineIncrement);
	m_editLineIncrement.SetWindowText(Buff);

	Buff.Format(_T("%d"),m_pData->m_nScanFinishLine + 1);
	m_editFinishLine.SetWindowText(Buff);


	Buff.Format(_T("%0.1fmm"),m_pData->m_fFastIncrement);
	m_editFastIncrement.SetWindowText(Buff);

	Buff.Format(_T(""));
	Temp.Format(_T(""));
	for(i=0;i<32;i++){
		if((theApp.m_Axes[i].nPhysicalAxis-1) == m_pData->m_nFastAxis) {
			Temp.Format(L"%s",theApp.m_Axes[i].cName);
//			theApp.CharToCString(theApp.m_Axes[i].cName, Temp);
			Buff.Format(_T("%s (%d)"),Temp,m_pData->m_nFastAxis);
			break;
		}
	}
	m_editFastAxis.SetWindowText(Buff);

	switch(m_pData->m_nSentence) {
	case 0:
		break;
	case 1:	m_editSentence.SetWindowText(L"Pass");
		break;
	case 2:	m_editSentence.SetWindowText(L"Fail");
		break;
	case 3:	m_editSentence.SetWindowText(L"Concession");
		break;
	}

	m_editMachineName.SetWindowText(m_pData->m_MachineName);
	m_editMachinLocation.SetWindowText(m_pData->m_MachineLocation);
}

