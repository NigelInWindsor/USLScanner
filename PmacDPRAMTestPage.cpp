// PmacDPRAMTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacDPRAMTestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacDPRAMTestPage property page

IMPLEMENT_DYNCREATE(CPmacDPRAMTestPage, CResizablePage)

CPmacDPRAMTestPage::CPmacDPRAMTestPage() : CResizablePage(CPmacDPRAMTestPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacDPRAMTestPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bTimerActive = FALSE;
	m_nFeederValue = -200;

}

CPmacDPRAMTestPage::~CPmacDPRAMTestPage()
{
}

void CPmacDPRAMTestPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacDPRAMTestPage)
	DDX_Control(pDX, IDC_EDIT_WRITTEN, m_editWritten);
	DDX_Control(pDX, IDC_EDIT_READ, m_editRead);
	DDX_Control(pDX, IDC_EDIT_ERRORS, m_editErrors);
	DDX_Control(pDX, IDC_CHECK_TEST, m_checkTest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacDPRAMTestPage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacDPRAMTestPage)
	ON_BN_CLICKED(IDC_CHECK_TEST, OnCheckTest)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacDPRAMTestPage message handlers

BOOL CPmacDPRAMTestPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPmacDPRAMTestPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CResizablePage::OnSetActive();
}

BOOL CPmacDPRAMTestPage::OnKillActive() 
{
	m_bTest = FALSE;
	if(m_bTimerActive==TRUE) KillTimer(1);
	
	return CResizablePage::OnKillActive();
}


void CPmacDPRAMTestPage::OnCheckTest() 
{
	m_bTest = m_checkTest.GetCheck() & 0x01;
	if(m_bTest == TRUE) {
		m_nErrorCount = 0;
		if(m_bTimerActive==FALSE) SetTimer(1,50,NULL);
		m_bTimerActive=TRUE;
	} else {
		if(m_bTimerActive==TRUE) KillTimer(1);
		m_bTimerActive=FALSE;
	}
}

void CPmacDPRAMTestPage::OnTimer(UINT nIDEvent) 
{
	CString Buff,Temp,Error;
	int ii,nTestType;
	int nSize = 16;

	Buff.Empty();
	for(ii=0;ii<nSize;ii++) {
		m_nWriteArray[ii]=m_nFeederValue+ii;
		Temp.Format(_T("%d\r\n"),m_nWriteArray[ii]);
		Buff += Temp;
	}
	m_editWritten.SetWindowText(Buff);

	switch(nTestType = 0) {
	case 0:
		theApp.m_Si10User.TestPatternOutput(1);
		theApp.m_PmacUser.DpramSetMem(112,nSize,m_nWriteArray,FALSE);
		theApp.m_Si10User.TestPatternOutput(0);

		theApp.m_Si10User.TestPatternOutput(1);
		theApp.m_PmacUser.DpramGetMem(112,nSize,m_nReadArray);
		theApp.m_Si10User.TestPatternOutput(0);
		break;
	case 1:
		theApp.m_Si10User.TestPatternOutput(1);
		for(ii=0;ii<nSize;ii++) {
			theApp.m_PmacUser.WriteDpram(112+ii,m_nWriteArray[ii]);
		}
		theApp.m_Si10User.TestPatternOutput(0);

		theApp.m_Si10User.TestPatternOutput(1);
		for(ii=0;ii<nSize;ii++) {
			m_nReadArray[ii] = theApp.m_PmacUser.ReadDpram(112+ii);
		}
		theApp.m_Si10User.TestPatternOutput(0);
		break;
	case 2:
		theApp.m_Si10User.TestPatternOutput(1);

		theApp.m_PmacUser.WriteDpram(112,m_nWriteArray[0]);
		theApp.m_Si10User.TestPatternOutput(0);

		theApp.m_Si10User.TestPatternOutput(1);
		m_nReadArray[0] = theApp.m_PmacUser.ReadDpram(112);
		theApp.m_Si10User.TestPatternOutput(0);
		break;
	}

	Buff.Empty();
	for(ii=0;ii<nSize;ii++) {
		if(ii<16) {
			Temp.Format(_T("%d\r\n"),m_nReadArray[ii]);
			Buff += Temp;
		}
		if(m_nReadArray[ii] != m_nWriteArray[ii]) {
			m_nErrorCount++;
			Error.Format(_T("%d write=%x read=%x"),m_nErrorCount, m_nWriteArray[ii],m_nReadArray[ii]);
			m_editErrors.SetWindowText(Error);
			KillTimer(1);
		}
	}
	m_editRead.SetWindowText(Buff);


	m_nFeederValue--;

	CResizablePage::OnTimer(nIDEvent);
}
