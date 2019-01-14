// PmacFaultFindingPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacFaultFindingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacFaultFindingPage dialog

IMPLEMENT_DYNCREATE(CPmacFaultFindingPage, CResizablePage)

CPmacFaultFindingPage::CPmacFaultFindingPage() : CResizablePage(CPmacFaultFindingPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacFaultFindingPage)
	//}}AFX_DATA_INIT
	
}


void CPmacFaultFindingPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacFaultFindingPage)
	DDX_Control(pDX, IDC_EDIT_SUGGESTIONS, m_editSuggestions);
	DDX_Control(pDX, IDC_COMBO_AXIS, m_comboAxis);
	DDX_Control(pDX, IDC_EDIT_INSTRUCTIONS, m_editInstructions);
	DDX_Control(pDX, IDC_EDIT_START_POS, m_editStartPos);
	DDX_Control(pDX, IDC_EDIT_FINISH_POS, m_editFinishPos);
	DDX_Control(pDX, IDC_EDIT_MOVE_PERIOD, m_editMovePeriod);
	DDX_Control(pDX, IDC_COMBO_TEST, m_comboTest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacFaultFindingPage, CDialog)
	//{{AFX_MSG_MAP(CPmacFaultFindingPage)
	ON_CBN_SELCHANGE(IDC_COMBO_TEST, OnSelchangeComboTest)
	ON_BN_CLICKED(IDC_BUTTON_PROCEED, OnButtonProceed)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacFaultFindingPage message handlers

BOOL CPmacFaultFindingPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	int ii;

	
	m_comboTest.AddString(_T("1: Does motor move at all"));
	m_comboTest.AddString(_T("2: Does motor move in correct direction"));
	m_comboTest.AddString(_T("3: Is encoder working & counting in correct direction"));
	m_comboTest.SetCurSel(m_nTestType = 0);
	

	for(ii=0;ii<16;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_comboAxis.AddString(Buff);
	}
	m_comboAxis.SetCurSel(m_nTestAxis = 0);

	Buff.Format(_T("%d mS"),m_nMovePeriodInmS = 100);
	m_editMovePeriod.SetWindowText(Buff);

//	SetTimer(1,100,NULL);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPmacFaultFindingPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CResizablePage::OnSetActive();
}


void CPmacFaultFindingPage::OnSelchangeComboTest() 
{
	// TODO: Add your control notification handler code here
	
}

void CPmacFaultFindingPage::OnButtonProceed() 
{
	CString Buff,AppendBuff,I5Buff,IX24Buff;
	int nI5;

	m_editMovePeriod.GetWindowText(Buff);
	m_nMovePeriodInmS = _ttoi(Buff);
	m_nTestAxis = m_comboAxis.GetCurSel();
	m_nTestType = m_comboTest.GetCurSel();

	Buff.Empty();
	m_editInstructions.SetWindowText(Buff);

	//Switch off unwanted PLC and PLCC
	theApp.m_PmacUser.RequestVariable(_T("I"),5,&I5Buff);
	I5Buff.GetBufferSetLength(I5Buff.GetLength()-2);
	nI5 = _ttoi(I5Buff);
	nI5=3;
	if(nI5 & 2) {
		theApp.m_PmacUser.SetVariable(_T("I"),5,nI5 & 1);
		AppendBuff.Format(_T("I5=%d		//Switch off unwanted PLC\r\n"),nI5 & 1);
		Buff += AppendBuff;
		m_editInstructions.SetWindowText(Buff);
	}
	//Switch off limit switches
	theApp.m_PmacUser.RequestVariable(_T("I"),100*(m_nTestAxis+1) + 24,&IX24Buff);
	IX24Buff.GetBufferSetLength(IX24Buff.GetLength()-2);
	AppendBuff.Format(_T("I%d=$20000"),100*(m_nTestAxis+1) + 24);
	theApp.m_PmacUser.SendStr(AppendBuff);
	Buff += AppendBuff;
	Buff += _T("	//Switch off limit switches\r\n");
	m_editInstructions.SetWindowText(Buff);

	//Select relevent axis
	AppendBuff.Format(_T("#%d"),m_nTestAxis+1);
	theApp.m_PmacUser.SendStr(AppendBuff);
	Buff += AppendBuff;
	AppendBuff.Format(_T("		//Select axis %d\r\n"),m_nTestAxis+1);
	Buff += AppendBuff;
	//Get start position
	theApp.m_PmacUser.RequestResponce(_T("P"),&AppendBuff);
	AppendBuff.GetBufferSetLength(AppendBuff.GetLength()-2);
	m_editStartPos.SetWindowText(AppendBuff);
	m_nStartPos=_ttoi(AppendBuff);

	switch(m_nTestType) {
	case 0:
		AppendBuff.Format(_T("o100"));
		theApp.m_PmacUser.SendStr(AppendBuff);
		Buff += AppendBuff;
		Buff += _T("		 //Open loop move 100%\r\n");
		m_editInstructions.SetWindowText(Buff);
		m_editInstructions.UpdateWindow();

		Sleep(m_nMovePeriodInmS);
		AppendBuff.Format(_T("k"));
		theApp.m_PmacUser.SendStr(AppendBuff);
		Buff += AppendBuff;
		Buff += _T("		 //Kill movement\r\n");
		m_editInstructions.SetWindowText(Buff);
		break;
	case 1:
		AppendBuff.Format(_T("o100"));
		theApp.m_PmacUser.SendStr(AppendBuff);
		Buff += AppendBuff;
		Buff += _T("		 //Open loop move 100%\r\n");
		m_editInstructions.SetWindowText(Buff);
		m_editInstructions.UpdateWindow();

		Sleep(m_nMovePeriodInmS);
		AppendBuff.Format(_T("k"));
		theApp.m_PmacUser.SendStr(AppendBuff);
		Buff += AppendBuff;
		Buff += _T("		 //Kill movement\r\n");
		m_editInstructions.SetWindowText(Buff);
		break;
	case 2:
		Buff += "\r\nPhysically note where the axis is\r\nIf necessary, place a pen line on the axis\r\n";
		Buff += "Now move the axis by hand in a positive direction\r\n\r\n";
		Buff += "The axis should then be returned to its starting position\r\n\r\n";
		break;
	}


	//Get finish position
//	theApp.m_PmacUser.RequestResponce(_T("P"),&AppendBuff);
//	AppendBuff.GetBufferSetLength(AppendBuff.GetLength()-2);
//	m_editFinishPos.SetWindowText(AppendBuff);
//	m_nFinishPos=_ttoi(AppendBuff);
	//Return limit Switches to origional value
	AppendBuff.Format(_T("I%d=%s"),100*(m_nTestAxis+1) + 24,IX24Buff);
	theApp.m_PmacUser.SendStr(AppendBuff);
	Buff += AppendBuff;
	Buff += _T("		//reset limit switches\r\n");
	m_editInstructions.SetWindowText(Buff);
	//Switch off unwanted PLC and PLCC
	AppendBuff.Format(_T("I5=%s"),I5Buff);
	theApp.m_PmacUser.SendStr(AppendBuff);
	Buff += AppendBuff;
	Buff += _T("		//Restore PLC and PLCC");
	m_editInstructions.SetWindowText(Buff);

	theApp.m_PmacUser.RequestResponce(_T("P"),&Buff);
	Buff.GetBufferSetLength(Buff.GetLength()-2);
	m_editFinishPos.SetWindowText(Buff);
	m_nFinishPos=_ttoi(Buff);

	switch(m_nTestType) {
	case 0:
		if(abs(m_nStartPos - m_nFinishPos) < 10) {
			Buff = "The position didn't seem to change\r\nIf the axis DIDN'T move then consider the following\r\n";
			Buff += "	1: The amplifiers aren't powered\r\n";
			Buff += "	2: The motor has failed\r\n";
			Buff += "	3: The cable to the motor has failed\r\n";
			Buff += "	4: The axis has jammed\r\n";
			Buff += "\r\n";
			Buff += "If the axis DID move then consider the following\r\n";
			Buff += "	5: The encoder doesn't work\r\n";
			Buff += "	6: Power to the encoder has failed\r\n";
			Buff += "	7: Cabling to the encoder has failed\r\n";
		} else {
			Buff = "The position returned from the encoder has changed.\r\nIt is likely therefore, that the motor and amplifier are fine.";
		}
		break;
	case 1:
		Buff = "Did the axis PHYSICALLY move in a positive direction.\r\nIf the axis did not move in a positive direction then reverse the wires\r\nto the motor.\r\n";
		Buff += "Do NOT use the encoder position to determine direction as this may be\r\nwired incorrectly,";
		break;
	case 2:
		Buff = "If the position is changing by only a few counts, then the encoder\r\nis either faulty or one of the signal wires has failed.\r\nCheck the quadrature lines are functing correctly\r\n\r\n";
		Buff += "If the encoder is counting in the wrong direction\r\nreverse the A and !A lines.\r\n\r\n";
		Buff += "If when the axis has returned to it's origional start position, the positions\r\ndisplayed are sugnificantly different, then either the supply level is low or\r\nthe encoder is slipping with respect to the physical axis";
		break;
	}
	m_editSuggestions.SetWindowText(Buff);


}

void CPmacFaultFindingPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	
	theApp.m_PmacUser.RequestResponce(_T("P"),&Buff);
	Buff.GetBufferSetLength(Buff.GetLength()-2);
	m_editFinishPos.SetWindowText(Buff);
	m_nFinishPos=_ttoi(Buff);

	CDialog::OnTimer(nIDEvent);
}
