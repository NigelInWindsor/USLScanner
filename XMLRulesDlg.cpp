// XMLRulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "XMLRulesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXMLRulesDlg dialog


CXMLRulesDlg::CXMLRulesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CData* pData,int nImageType,int nImageNumber)
	: CDialog(CXMLRulesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMLRulesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pData = pData;
	m_nImageNumber = nImageNumber;
	m_nImageType = nImageType;
}


void CXMLRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMLRulesDlg)
	DDX_Control(pDX, IDC_CHECK_TWIST, m_checkTwist);
	DDX_Control(pDX, IDC_COMBO_COORDINATE_FRAME, m_comboCoordinateFrame);
	DDX_Control(pDX, IDC_COMBO_TOF_RULE, m_comboTOFRule);
	DDX_Control(pDX, IDC_EDIT_ORIGIONAL, m_editOrigional);
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
	DDX_Control(pDX, IDC_CHECK_HEADER, m_checkHeader);
	DDX_Control(pDX, IDC_CHECK_DATA, m_checkData);
	DDX_Control(pDX, IDC_CHECK_COORDINATES, m_checkCoordinates);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXMLRulesDlg, CDialog)
	//{{AFX_MSG_MAP(CXMLRulesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_COORDINATES, OnCheckCoordinates)
	ON_BN_CLICKED(IDC_CHECK_DATA, OnCheckData)
	ON_BN_CLICKED(IDC_CHECK_HEADER, OnCheckHeader)
	ON_CBN_SELCHANGE(IDC_COMBO_TOF_RULE, OnSelchangeComboTofRule)
	ON_BN_CLICKED(IDC_CHECK_TWIST, OnCheckTwist)
	ON_CBN_SELCHANGE(IDC_COMBO_COORDINATE_FRAME, OnSelchangeComboCoordinateFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLRulesDlg message handlers

void CXMLRulesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CXMLRulesDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CXMLRulesDlg::IDD]=FALSE;

	
	CDialog::OnClose();
	DestroyWindow();
}

void CXMLRulesDlg::OnDestroy() 
{
	

	if (CXMLRulesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CXMLRulesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CXMLRulesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;
	
	CString Micro = L"\x0b5s";


	if (CXMLRulesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CXMLRulesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	Buff.Format(L"ns");	m_comboTOFRule.AddString(Buff);
	m_comboTOFRule.AddString(Micro);
	Buff.Format(L"mm");	m_comboTOFRule.AddString(Buff);

	Buff.LoadString(IDS_Scanner);	m_comboCoordinateFrame.AddString(Buff);
	Buff.LoadString(IDS_CAD);		m_comboCoordinateFrame.AddString(Buff);

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXMLRulesDlg::UpdateAllControls()
{
	
	CString Buff;
	__int64	n64Total = 0;
	int nArraySize = m_pData->m_nSamplesLine * m_pData->m_nNumberLines;

	m_checkHeader.SetCheck(theApp.m_LastSettings.nXMLSaveHeader);
	m_checkCoordinates.SetCheck(theApp.m_LastSettings.nXMLSaveCoordinates);
	m_checkData.SetCheck(theApp.m_LastSettings.nXMLSaveData);

	if(theApp.m_LastSettings.nXMLSaveHeader == 1) n64Total += 1024;
	if(theApp.m_LastSettings.nXMLSaveCoordinates == 1) {
		n64Total += ((__int64)nArraySize * (__int64)47 * (__int64)2);
	}
	if(theApp.m_LastSettings.nXMLSaveData == 1) {
		n64Total += ((__int64)nArraySize * (__int64)32 * (__int64)2);
	}
	if(n64Total < 1000000) {
		Buff.Format(_T("File size : %d Bytes"),(int)n64Total);
	} else {
		Buff.Format(_T("File size : %.01f MBytes"),(float)n64Total / 1e6f);
	}
	m_editPrompt.SetWindowText(Buff);

	if(m_nImageType == 0) {
		Buff.Format(_T("Size : %.01f MBytes"),(float)nArraySize / 1e6f);
	} else {
		Buff.Format(_T("Size : %.01f MBytes"),(float)(nArraySize * 4) / 1e6f);
	}
	m_editOrigional.SetWindowText(Buff);

	m_comboTOFRule.SetCurSel(theApp.m_LastSettings.nXMLTOFRule);

	if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_CAD_COORDINATES) {
		m_comboCoordinateFrame.SetCurSel(1);
	} else {
		m_comboCoordinateFrame.SetCurSel(0);
	}
	if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_TWIST_COORDINATES) {
		m_checkTwist.SetCheck(true);
	} else {
		m_checkTwist.SetCheck(false);
	}

}

void CXMLRulesDlg::OnCheckCoordinates() 
{
	
	
	theApp.m_LastSettings.nXMLSaveCoordinates = m_checkCoordinates.GetCheck();
	UpdateAllControls();
}

void CXMLRulesDlg::OnCheckData() 
{
	
	
	theApp.m_LastSettings.nXMLSaveData = m_checkData.GetCheck();
	UpdateAllControls();
}

void CXMLRulesDlg::OnCheckHeader() 
{
	
	
	theApp.m_LastSettings.nXMLSaveHeader = m_checkHeader.GetCheck();
	UpdateAllControls();
}


void CXMLRulesDlg::OnSelchangeComboTofRule() 
{
	
	
	theApp.m_LastSettings.nXMLTOFRule = m_comboTOFRule.GetCurSel();
}

void CXMLRulesDlg::OnCheckTwist() 
{
	

	if(m_checkTwist.GetCheck()) {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame |= EXPORT_TWIST_COORDINATES;
	} else {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame &= ~EXPORT_TWIST_COORDINATES;
	}
}

void CXMLRulesDlg::OnSelchangeComboCoordinateFrame() 
{
	

	if(m_comboCoordinateFrame.GetCurSel() == 0) {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame &= ~EXPORT_CAD_COORDINATES;
	} else {
		theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame |= EXPORT_CAD_COORDINATES;
	}
	
}
