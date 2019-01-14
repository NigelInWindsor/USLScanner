// CtrlPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CtrlPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtrlPropertiesDlg dialog


CCtrlPropertiesDlg::CCtrlPropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nIndex)
	: CDialog(CCtrlPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCtrlPropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pParent = pParent;
	
	m_nIndex = nIndex;
	m_nBorderStyle = 1;
}

void CCtrlPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCtrlPropertiesDlg)
	DDX_Control(pDX, IDC_CHECK_FILENAME, m_checkFileName);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_FULL_PATH, m_checkFullPathName);
	DDX_Control(pDX, IDC_CHECK_PRINT, m_checkPrint);
	DDX_Control(pDX, IDC_COMBO_ALIGN_TEXT, m_comboAlignText);
	DDX_Control(pDX, IDC_CHECK_BORDER, m_checkBorder);
	DDX_Control(pDX, IDC_CHECK_READ_ONLY, m_checkReadOnly);
	DDX_Control(pDX, IDC_CHECK_TIME, m_checkUpdateTime);
	DDX_Control(pDX, IDC_CHECK_UPDATE_OPERATOR, m_checkUpdateOperator);
	DDX_Control(pDX, IDC_CHECK_UPDATE_DATE, m_checkUpdateDate);
	DDX_Control(pDX, IDC_CHECK_CLEAR_LEVEL_3, m_checkClearLevel3);
	DDX_Control(pDX, IDC_CHECK_CLEAR_LEVEL_2, m_checkClearLevel2);
	DDX_Control(pDX, IDC_CHECK_CLEAR_LEVEL_1, m_checkClearLevel1);
	DDX_Control(pDX, IDC_EDIT_CAPTION, m_editCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCtrlPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CCtrlPropertiesDlg)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_CLEAR_LEVEL_1, OnCheckClearLevel1)
	ON_BN_CLICKED(IDC_CHECK_CLEAR_LEVEL_2, OnCheckClearLevel2)
	ON_BN_CLICKED(IDC_CHECK_CLEAR_LEVEL_3, OnCheckClearLevel3)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_DATE, OnCheckUpdateDate)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_OPERATOR, OnCheckUpdateOperator)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	ON_BN_CLICKED(IDC_CHECK_READ_ONLY, OnCheckReadOnly)
	ON_BN_CLICKED(IDC_CHECK_BORDER, OnCheckBorder)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGN_TEXT, OnSelchangeComboAlignText)
	ON_BN_CLICKED(IDC_CHECK_PRINT, OnCheckPrint)
	ON_BN_CLICKED(IDC_CHECK_FILENAME, OnCheckFilename)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_FULL_PATH, OnCheckDisplayFullPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtrlPropertiesDlg message handlers

void CCtrlPropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CCtrlPropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CCtrlPropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CCtrlPropertiesDlg::OnDestroy() 
{
	if (CCtrlPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CCtrlPropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CCtrlPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	
	if (CCtrlPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CCtrlPropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		};
	}

	Buff.LoadString(IDS_Left);		m_comboAlignText.AddString(Buff);
	Buff.LoadString(IDS_Centre);	m_comboAlignText.AddString(Buff);
	Buff.LoadString(IDS_Right);		m_comboAlignText.AddString(Buff);

	
	UpdateAllControls();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCtrlPropertiesDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	if(m_nIndex>=0) {
		if(theApp.m_pDescription[m_nIndex].pCaption) {
			m_editCaption.SetWindowText(*theApp.m_pDescription[m_nIndex].pCaption);
		}

		m_checkClearLevel1.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_LEVEL_0);
		m_checkClearLevel2.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_LEVEL_1);
		m_checkClearLevel3.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_LEVEL_2);
		m_checkUpdateDate.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_DATE);
		m_checkUpdateTime.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_TIME);
		m_checkUpdateOperator.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_OPERATOR);
		m_checkPrint.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_PRINT);
		m_checkFileName.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_FILENAME);
		m_checkFullPathName.SetCheck(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_FULLPATHNAME);

		m_checkReadOnly.SetCheck(theApp.m_pDescription[m_nIndex].nStyle & ES_READONLY);
		m_checkBorder.SetCheck(theApp.m_pDescription[m_nIndex].nExStyle & WS_EX_CLIENTEDGE);

		switch(theApp.m_pDescription[m_nIndex].nCtrlType) {
		case EDIT_CTL:	m_comboAlignText.SetCurSel(theApp.m_pDescription[m_nIndex].nStyle & (ES_LEFT | ES_CENTER | ES_RIGHT));
			break;
		case BUTTON_CTL:
			if((theApp.m_pDescription[m_nIndex].nStyle & BS_LEFT) == BS_LEFT ) m_comboAlignText.SetCurSel(0);
			if((theApp.m_pDescription[m_nIndex].nStyle & BS_RIGHT) == BS_RIGHT ) m_comboAlignText.SetCurSel(2);
			if((theApp.m_pDescription[m_nIndex].nStyle & BS_CENTER) == BS_CENTER ) m_comboAlignText.SetCurSel(1);
			break;
		}


		switch(theApp.m_pDescription[m_nIndex].nCtrlType) {
		case TEXT_CTL: SetWindowText(_T("Text Property"));
			break;
		case EDIT_CTL: SetWindowText(_T("Edit Property"));
			break;
		case BUTTON_CTL: SetWindowText(_T("Button Property"));
			break;
		}
	} else {
		m_editCaption.SetWindowText(theApp.m_DetailsWindowName);
	}

	SetAccessPrivelages();

}

void CCtrlPropertiesDlg::SetAccessPrivelages()
{
	if (GetSafeHwnd() == NULL) return;

	if(m_nIndex >= 0) {
		m_comboAlignText.EnableWindow(TRUE);
		m_checkReadOnly.EnableWindow(TRUE);
		m_checkBorder.EnableWindow(TRUE);
		m_checkUpdateTime.EnableWindow(TRUE);
		m_checkPrint.EnableWindow(TRUE);

		switch(theApp.m_pDescription[m_nIndex].nCtrlType) {
		case TEXT_CTL:
			m_editCaption.EnableWindow(TRUE);
			m_checkClearLevel1.EnableWindow(FALSE);
			m_checkClearLevel2.EnableWindow(FALSE);
			m_checkClearLevel3.EnableWindow(FALSE);
			m_checkUpdateDate.EnableWindow(FALSE);
			m_checkUpdateOperator.EnableWindow(FALSE);
			break;
		case EDIT_CTL:
			m_editCaption.EnableWindow(TRUE);
			m_checkClearLevel1.EnableWindow(TRUE);
			m_checkClearLevel2.EnableWindow(TRUE);
			m_checkClearLevel3.EnableWindow(TRUE);
			m_checkUpdateDate.EnableWindow(TRUE);
			m_checkUpdateOperator.EnableWindow(TRUE);
			break;
		case BUTTON_CTL:
			m_editCaption.EnableWindow(TRUE);
			m_checkClearLevel1.EnableWindow(TRUE);
			m_checkClearLevel2.EnableWindow(TRUE);
			m_checkClearLevel3.EnableWindow(TRUE);
			m_checkUpdateDate.EnableWindow(TRUE);
			m_checkUpdateOperator.EnableWindow(TRUE);
			break;
		}
		if(theApp.m_pDescription[m_nIndex].nActionFlag & CTL_FILENAME) {
			m_checkFullPathName.EnableWindow(TRUE);
		} else {
			m_checkFullPathName.EnableWindow(FALSE);
		}
	} else {
		m_editCaption.EnableWindow(TRUE);
		m_checkClearLevel1.EnableWindow(FALSE);
		m_checkClearLevel2.EnableWindow(FALSE);
		m_checkClearLevel3.EnableWindow(FALSE);
		m_checkUpdateDate.EnableWindow(FALSE);
		m_checkUpdateOperator.EnableWindow(FALSE);
		m_checkUpdateTime.EnableWindow(FALSE);
		m_checkPrint.EnableWindow(FALSE);
		m_comboAlignText.EnableWindow(FALSE);
		m_checkReadOnly.EnableWindow(FALSE);
		m_checkBorder.EnableWindow(FALSE);
	}
}

void CCtrlPropertiesDlg::OnChangeEditCaption() 
{
	CString Buff;

	m_editCaption.GetWindowText(Buff);
	if(m_nIndex>=0) {
		if(Buff.GetLength()) {
			if(theApp.m_pDescription[m_nIndex].pCaption == NULL) {
				theApp.m_pDescription[m_nIndex].pCaption = new CString(Buff);
			} else {
				*theApp.m_pDescription[m_nIndex].pCaption = Buff;
			}
		}
	} else {
		theApp.m_DetailsWindowName = Buff;
		m_pParent->SetWindowText(theApp.m_DetailsWindowName);
	}

	m_pParent->Invalidate(FALSE);
		
}


BOOL CCtrlPropertiesDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13: return TRUE;
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CCtrlPropertiesDlg::OnCheckClearLevel1() 
{
	m_checkClearLevel1.GetCheck() ? theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_LEVEL_0 : theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_LEVEL_0;
}

void CCtrlPropertiesDlg::OnCheckClearLevel2() 
{
	m_checkClearLevel2.GetCheck() ? theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_LEVEL_1 : theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_LEVEL_1;
}

void CCtrlPropertiesDlg::OnCheckClearLevel3() 
{
	m_checkClearLevel3.GetCheck() ? theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_LEVEL_2 : theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_LEVEL_2;
}


void CCtrlPropertiesDlg::OnCheckUpdateDate() 
{
	if(m_checkUpdateDate.GetCheck()) {
		theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_DATE;
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_OPERATOR;
	} else {
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_DATE;
	}
	UpdateAllControls();
}

void CCtrlPropertiesDlg::OnCheckTime() 
{
	if(m_checkUpdateTime.GetCheck()) {
		theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_TIME;
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_OPERATOR;
	} else {
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_TIME;
	}
	UpdateAllControls();
}

void CCtrlPropertiesDlg::OnCheckUpdateOperator() 
{
	if(m_checkUpdateOperator.GetCheck()) {
		theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_OPERATOR;
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~(CTL_DATE | CTL_TIME);
	} else {
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_OPERATOR;
	}
	UpdateAllControls();
}

void CCtrlPropertiesDlg::OnCheckPrint() 
{
	if(m_checkPrint.GetCheck()) {
		theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_PRINT;
	} else {
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_PRINT;
	}
	UpdateAllControls();
}

void CCtrlPropertiesDlg::OnCheckReadOnly() 
{
	m_checkReadOnly.GetCheck() ? theApp.m_pDescription[m_nIndex].nStyle |= ES_READONLY : theApp.m_pDescription[m_nIndex].nStyle &= ~ES_READONLY;
	m_pParent->Invalidate(TRUE);
}

void CCtrlPropertiesDlg::OnCheckBorder() 
{
	m_checkBorder.GetCheck() ? theApp.m_pDescription[m_nIndex].nExStyle |= WS_EX_CLIENTEDGE : theApp.m_pDescription[m_nIndex].nExStyle &= ~WS_EX_CLIENTEDGE;
	m_pParent->Invalidate(TRUE);
}

void CCtrlPropertiesDlg::OnSelchangeComboAlignText() 
{
	switch(theApp.m_pDescription[m_nIndex].nCtrlType) {
	case EDIT_CTL:
		theApp.m_pDescription[m_nIndex].nStyle &= ~(ES_LEFT | ES_CENTER | ES_RIGHT);
		switch(m_comboAlignText.GetCurSel()) {
		case 0: theApp.m_pDescription[m_nIndex].nStyle |= ES_LEFT;
			break;
		case 1: theApp.m_pDescription[m_nIndex].nStyle |= ES_CENTER;
			break;
		case 2: theApp.m_pDescription[m_nIndex].nStyle |= ES_RIGHT;
			break;
		}
		break;
	case BUTTON_CTL:
		theApp.m_pDescription[m_nIndex].nStyle &= ~(BS_LEFT | BS_CENTER | BS_RIGHT);
		switch(m_comboAlignText.GetCurSel()) {
		case 0: theApp.m_pDescription[m_nIndex].nStyle |= BS_LEFT;
			break;
		case 1: theApp.m_pDescription[m_nIndex].nStyle |= BS_CENTER;
			break;
		case 2: theApp.m_pDescription[m_nIndex].nStyle |= BS_RIGHT;
			break;
		}
		break;
	}
	m_pParent->Invalidate(TRUE);

}


void CCtrlPropertiesDlg::OnCheckFilename() 
{
	if(m_checkFileName.GetCheck()) {
		theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_FILENAME;
	} else {
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_FILENAME;
	}
	
}

void CCtrlPropertiesDlg::OnCheckDisplayFullPath() 
{
	if(m_checkFullPathName.GetCheck()) {
		theApp.m_pDescription[m_nIndex].nActionFlag |= CTL_FULLPATHNAME;
	} else {
		theApp.m_pDescription[m_nIndex].nActionFlag &= ~CTL_FULLPATHNAME;
	}
	
}
