// ViewCommentsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ViewCommentsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewCommentsDlg dialog


CViewCommentsDlg::CViewCommentsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, CData* pData)
	: CDialog(CViewCommentsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewCommentsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDlg = pDlg;
	m_pData = pData;
}


void CViewCommentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCommentsDlg)
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCommentsDlg, CDialog)
	//{{AFX_MSG_MAP(CViewCommentsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCommentsDlg message handlers

void CViewCommentsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CViewCommentsDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CViewCommentsDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CViewCommentsDlg::OnDestroy() 
{
	

	if (CViewCommentsDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CViewCommentsDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CViewCommentsDlg::OnInitDialog() 
{
	
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	

	m_StaticPosText.Initialize(this, m_editText, TRUE, TRUE, TRUE, TRUE);

	if (CViewCommentsDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CViewCommentsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosText.SetNewPos();

	m_editText.SetWindowText(m_pData->m_Details[D_NOTES]);

//	this->SetActiveWindow()
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewCommentsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosText.SetNewPos();
	
}

void CViewCommentsDlg::OnChangeEditText() 
{
	
	m_editText.GetWindowText(m_pData->m_Details[D_NOTES]);
}
