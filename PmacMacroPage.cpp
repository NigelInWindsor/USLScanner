// PmacMacroPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacMacroPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacMacroPage property page

IMPLEMENT_DYNCREATE(CPmacMacroPage, CResizablePage)

CPmacMacroPage::CPmacMacroPage() : CResizablePage(CPmacMacroPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacMacroPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bCtrlButton = false;
	m_nStation = 0;
	m_nNode = 0;
	m_bMACROASCII = false;
	m_nLastTxPtr = 0;

}

CPmacMacroPage::~CPmacMacroPage()
{
}

void CPmacMacroPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacMacroPage)
	DDX_Control(pDX, IDC_COMBO_NODE, m_comboNode);
	DDX_Control(pDX, IDC_EDIT_SENT, m_editSent);
	DDX_Control(pDX, IDC_EDIT_MI998, m_editMI998);
	DDX_Control(pDX, IDC_EDIT_MI997, m_editMI997);
	DDX_Control(pDX, IDC_EDIT_MI996, m_editMI996);
	DDX_Control(pDX, IDC_EDIT_MI995, m_editMI995);
	DDX_Control(pDX, IDC_EDIT_MI992, m_editMI992);
	DDX_Control(pDX, IDC_EDIT_MI990, m_editMI990);
	DDX_Control(pDX, IDC_EDIT_MI10, m_editMI10);
	DDX_Control(pDX, IDC_EDIT_MI9, m_editMI9);
	DDX_Control(pDX, IDC_EDIT_MI8, m_editMI8);
	DDX_Control(pDX, IDC_BUTTON_MACRO_ASCII, m_buttonMACROASCII);
	DDX_Control(pDX, IDC_COMBO_STATION, m_comboStation);
	DDX_Control(pDX, IDC_EDIT_73, m_edit73);
	DDX_Control(pDX, IDC_EDIT_72, m_edit72);
	DDX_Control(pDX, IDC_EDIT_6891, m_edit6891);
	DDX_Control(pDX, IDC_EDIT_6890, m_edit6890);
	DDX_Control(pDX, IDC_EDIT_6857, m_edit6857);
	DDX_Control(pDX, IDC_EDIT_6856, m_edit6856);
	DDX_Control(pDX, IDC_EDIT_6855, m_edit6855);
	DDX_Control(pDX, IDC_EDIT_6854, m_edit6854);
	DDX_Control(pDX, IDC_EDIT_6853, m_edit6853);
	DDX_Control(pDX, IDC_EDIT_6852, m_edit6852);
	DDX_Control(pDX, IDC_EDIT_6851, m_edit6851);
	DDX_Control(pDX, IDC_EDIT_6850, m_edit6850);
	DDX_Control(pDX, IDC_EDIT_82, m_edit82);
	DDX_Control(pDX, IDC_EDIT_81, m_edit81);
	DDX_Control(pDX, IDC_EDIT_80, m_edit80);
	DDX_Control(pDX, IDC_EDIT_79, m_edit79);
	DDX_Control(pDX, IDC_EDIT_78, m_edit78);
	DDX_Control(pDX, IDC_EDIT_71, m_edit71);
	DDX_Control(pDX, IDC_EDIT_70, m_edit70);
	DDX_Control(pDX, IDC_EDIT_6841, m_edit6841);
	DDX_Control(pDX, IDC_EDIT_6840, m_edit6840);
	DDX_Control(pDX, IDC_EDIT_6807, m_edit6807);
	DDX_Control(pDX, IDC_EDIT_6806, m_edit6806);
	DDX_Control(pDX, IDC_EDIT_6805, m_edit6805);
	DDX_Control(pDX, IDC_EDIT_6804, m_edit6804);
	DDX_Control(pDX, IDC_EDIT_6803, m_edit6803);
	DDX_Control(pDX, IDC_EDIT_6802, m_edit6802);
	DDX_Control(pDX, IDC_EDIT_6801, m_edit6801);
	DDX_Control(pDX, IDC_EDIT_6800, m_edit6800);
	DDX_Control(pDX, IDC_EDIT_RESPONCE, m_editResponce);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_editCommand);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacMacroPage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacMacroPage)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_STATION, OnSelchangeComboStation)
	ON_BN_CLICKED(IDC_BUTTON_MACRO_ASCII, OnButtonMacroAscii)
	ON_BN_CLICKED(IDC_BUTTON_ALL_MI, OnButtonAllMi)
	ON_CBN_SELCHANGE(IDC_COMBO_NODE, OnSelchangeComboNode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacMacroPage message handlers

BOOL CPmacMacroPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString Buff;

	for(int ii=0;ii<16;ii++) {
		Buff.Format(L"Station %d",ii);
		m_comboStation.AddString(Buff);
	}
	for(int ii=0;ii<32;ii++) {
		Buff.Format(L"Node %d",ii);
		m_comboNode.AddString(Buff);
	};

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPmacMacroPage::PreTranslateMessage(MSG* pMsg) 
{
      BSTR szTest1 = ::SysAllocString(L"");;
      BSTR RxString = ::SysAllocString(L"");;
      CString szProgram;
      TCHAR  buf[500] = {0};
      CString szTest;
	  CString Buff;
	BSTR m_TxString = ::SysAllocString(L"I10");;
	CString TxBuff;
	TxBuff.Empty();


	switch(pMsg->message) {
	case WM_KEYUP:
		m_bCtrlButton = false;
		break;
	case WM_KEYDOWN:
		switch(pMsg->wParam) {
		case CURSOR_UP:
			if(&m_editCommand == FromHandle(pMsg->hwnd)) {
				if(m_Tx.GetSize()>0) {
					if(m_nLastTxPtr<m_Tx.GetSize()) {
						Buff.Format(L"%s",m_Tx.GetAt(m_nLastTxPtr));
						m_editCommand.SetWindowText(Buff);
					};
					--m_nLastTxPtr;
					if(m_nLastTxPtr<0) m_nLastTxPtr=0;
				}
			}
			break;
		case CURSOR_DOWN:
			if(&m_editCommand == FromHandle(pMsg->hwnd)) {
				if(m_Tx.GetSize()>0) {
					if(m_nLastTxPtr<m_Tx.GetSize()) {
						Buff.Format(L"%s",m_Tx.GetAt(m_nLastTxPtr));
						m_editCommand.SetWindowText(Buff);
					};
					++m_nLastTxPtr;
					if(m_nLastTxPtr>=m_Tx.GetSize()) m_nLastTxPtr=m_Tx.GetSize()-1;
				}
			}
			break;
		case 17: m_bCtrlButton=true;//Ctrl button down
			break;
		case 'T':
			if(m_bCtrlButton==true) {
				Buff.Format(L"%c",0x14);
				m_editCommand.SetWindowText(Buff);
			};
			break;
		case 'V':
			if(m_bCtrlButton==true) {
				Buff.Format(L"%c",0x16);
				m_editCommand.SetWindowText(Buff);
			};
			break;
		case 13:
			if(&m_editCommand == FromHandle(pMsg->hwnd)) {
				m_editCommand.GetWindowText(TxBuff);
				Buff.Empty();
				m_editCommand.SetWindowText(Buff);
			}
			if(&m_edit70 == FromHandle(pMsg->hwnd)) {
				m_edit70.GetWindowText(Buff);
				TxBuff.Format(L"I70=%s",Buff);
			}
			if(&m_edit71 == FromHandle(pMsg->hwnd)) {
				m_edit71.GetWindowText(Buff);
				TxBuff.Format(L"I71=%s",Buff);
			}
			if(&m_edit72 == FromHandle(pMsg->hwnd)) {
				m_edit72.GetWindowText(Buff);
				TxBuff.Format(L"I72=%s",Buff);
			}
			if(&m_edit73 == FromHandle(pMsg->hwnd)) {
				m_edit73.GetWindowText(Buff);
				TxBuff.Format(L"I73=%s",Buff);
			}
			if(&m_edit78 == FromHandle(pMsg->hwnd)) {
				m_edit78.GetWindowText(Buff);
				TxBuff.Format(L"I78=%s",Buff);
			}
			if(&m_edit79 == FromHandle(pMsg->hwnd)) {
				m_edit79.GetWindowText(Buff);
				TxBuff.Format(L"I79=%s",Buff);
			}
			if(&m_edit80 == FromHandle(pMsg->hwnd)) {
				m_edit80.GetWindowText(Buff);
				TxBuff.Format(L"I80=%s",Buff);
			}
			if(&m_edit81 == FromHandle(pMsg->hwnd)) {
				m_edit81.GetWindowText(Buff);
				TxBuff.Format(L"I81=%s",Buff);
			}
			if(&m_edit82 == FromHandle(pMsg->hwnd)) {
				m_edit82.GetWindowText(Buff);
				TxBuff.Format(L"I82=%s",Buff);
			}
			if(&m_edit6800 == FromHandle(pMsg->hwnd)) {
				m_edit6800.GetWindowText(Buff);
				TxBuff.Format(L"I6800=%s",Buff);
			}
			if(&m_edit6801 == FromHandle(pMsg->hwnd)) {
				m_edit6801.GetWindowText(Buff);
				TxBuff.Format(L"I6801=%s",Buff);
			}
			if(&m_edit6802 == FromHandle(pMsg->hwnd)) {
				m_edit6802.GetWindowText(Buff);
				TxBuff.Format(L"I6802=%s",Buff);
			}
			if(&m_edit6803 == FromHandle(pMsg->hwnd)) {
				m_edit6803.GetWindowText(Buff);
				TxBuff.Format(L"I6803=%s",Buff);
			}
			if(&m_edit6804 == FromHandle(pMsg->hwnd)) {
				m_edit6804.GetWindowText(Buff);
				TxBuff.Format(L"I6804=%s",Buff);
			}
			if(&m_edit6805 == FromHandle(pMsg->hwnd)) {
				m_edit6805.GetWindowText(Buff);
				TxBuff.Format(L"I6805=%s",Buff);
			}
			if(&m_edit6806 == FromHandle(pMsg->hwnd)) {
				m_edit6806.GetWindowText(Buff);
				TxBuff.Format(L"I6806=%s",Buff);
			}
			if(&m_edit6807 == FromHandle(pMsg->hwnd)) {
				m_edit6807.GetWindowText(Buff);
				TxBuff.Format(L"I6807=%s",Buff);
			}
			if(&m_edit6840 == FromHandle(pMsg->hwnd)) {
				m_edit6840.GetWindowText(Buff);
				TxBuff.Format(L"I6840=%s",Buff);
			}
			if(&m_edit6841 == FromHandle(pMsg->hwnd)) {
				m_edit6841.GetWindowText(Buff);
				TxBuff.Format(L"I6841=%s",Buff);
			}
			if(&m_edit6850 == FromHandle(pMsg->hwnd)) {
				m_edit6850.GetWindowText(Buff);
				TxBuff.Format(L"I6850=%s",Buff);
			}
			if(&m_edit6851 == FromHandle(pMsg->hwnd)) {
				m_edit6851.GetWindowText(Buff);
				TxBuff.Format(L"I6851=%s",Buff);
			}
			if(&m_edit6852 == FromHandle(pMsg->hwnd)) {
				m_edit6852.GetWindowText(Buff);
				TxBuff.Format(L"I6852=%s",Buff);
			}
			if(&m_edit6853 == FromHandle(pMsg->hwnd)) {
				m_edit6853.GetWindowText(Buff);
				TxBuff.Format(L"I6853=%s",Buff);
			}
			if(&m_edit6854 == FromHandle(pMsg->hwnd)) {
				m_edit6854.GetWindowText(Buff);
				TxBuff.Format(L"I6854=%s",Buff);
			}
			if(&m_edit6855 == FromHandle(pMsg->hwnd)) {
				m_edit6855.GetWindowText(Buff);
				TxBuff.Format(L"I6855=%s",Buff);
			}
			if(&m_edit6856 == FromHandle(pMsg->hwnd)) {
				m_edit6856.GetWindowText(Buff);
				TxBuff.Format(L"I6856=%s",Buff);
			}
			if(&m_edit6857 == FromHandle(pMsg->hwnd)) {
				m_edit6857.GetWindowText(Buff);
				TxBuff.Format(L"I6857=%s",Buff);
			}
			if(&m_edit6890 == FromHandle(pMsg->hwnd)) {
				m_edit6890.GetWindowText(Buff);
				TxBuff.Format(L"I6890=%s",Buff);
			}
			if(&m_edit6891 == FromHandle(pMsg->hwnd)) {
				m_edit6891.GetWindowText(Buff);
				TxBuff.Format(L"I6891=%s",Buff);
			}
			if(!TxBuff.IsEmpty()) {
				Buff.Empty();
				m_editResponce.SetWindowText(Buff);
				SendStr(TxBuff);
			}
			return TRUE;
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPmacMacroPage::UpdateAllControls()
{
	CString Buff;

	m_comboStation.SetCurSel(m_nStation);
	m_comboNode.SetCurSel(m_nNode);

	theApp.m_PmacUser.RequestVariable(_T("I"),70,&Buff);
	m_edit70.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),71,&Buff);
	m_edit71.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),72,&Buff);
	m_edit72.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),73,&Buff);
	m_edit73.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),78,&Buff);
	m_edit78.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),79,&Buff);
	m_edit79.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),80,&Buff);
	m_edit80.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),81,&Buff);
	m_edit81.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),82,&Buff);
	m_edit82.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6800,&Buff);
	m_edit6800.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6801,&Buff);
	m_edit6801.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6802,&Buff);
	m_edit6802.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6803,&Buff);
	m_edit6803.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6804,&Buff);
	m_edit6804.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6805,&Buff);
	m_edit6805.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6806,&Buff);
	m_edit6806.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6807,&Buff);
	m_edit6807.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6840,&Buff);
	m_edit6840.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6841,&Buff);
	m_edit6841.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6850,&Buff);
	m_edit6850.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6851,&Buff);
	m_edit6851.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6852,&Buff);
	m_edit6852.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6853,&Buff);
	m_edit6853.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6854,&Buff);
	m_edit6854.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6855,&Buff);
	m_edit6855.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6856,&Buff);
	m_edit6856.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6857,&Buff);
	m_edit6857.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6890,&Buff);
	m_edit6890.SetWindowText(Buff);
	theApp.m_PmacUser.RequestVariable(_T("I"),6891,&Buff);
	m_edit6891.SetWindowText(Buff);

	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,8,&Buff);		m_editMI8.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,9,&Buff);		m_editMI9.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,10,&Buff);	m_editMI10.SetWindowText(Buff);

	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,990,&Buff);	m_editMI990.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,992,&Buff);	ConvertToDecimal(Buff);	m_editMI992.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,995,&Buff);	m_editMI995.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,996,&Buff);	m_editMI996.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,997,&Buff);	m_editMI997.SetWindowText(Buff);
	theApp.m_PmacUser.RequestSlaveVariable(L"I",m_nNode,998,&Buff);	m_editMI998.SetWindowText(Buff);

}

void CPmacMacroPage::OnButtonRefresh() 
{
	UpdateAllControls();	
}

BOOL CPmacMacroPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	if(m_bTimerActive != TRUE) SetTimer(1,200,NULL);
	m_bTimerActive = TRUE;
	
	return CResizablePage::OnSetActive();
}

BOOL CPmacMacroPage::OnKillActive() 
{
	if(m_bTimerActive == TRUE) KillTimer(1);
	m_bTimerActive = FALSE;
	
	return CResizablePage::OnKillActive();
}

void CPmacMacroPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(theApp.m_PmacUser.GetStr(Buff)) {
		Buff.Replace(_T("\r"),_T("\r\n"));
		m_editResponce.SetWindowText(Buff);

	}
	
	CResizablePage::OnTimer(nIDEvent);
}

void CPmacMacroPage::OnSelchangeComboStation() 
{

	m_nStation = m_comboStation.GetCurSel();
}

void CPmacMacroPage::OnButtonMacroAscii() 
{
	CString Buff;

	if(m_bMACROASCII == false) {
		Buff.Format(L"MACSTA%d",m_nStation);
		SendStr(Buff);

		m_buttonMACROASCII.SetWindowText(L"Stop MACRO ASCII");
		m_bMACROASCII = true;
	} else {
		Buff.Format(L"%c",0x14);	//CTRL T
		SendStr(Buff);

		m_buttonMACROASCII.SetWindowText(L"Start MACRO ASCII");
		m_bMACROASCII = false;
	}
}

void CPmacMacroPage::OnButtonAllMi() 
{
	CString Buff,m_strMI;

	m_nNode = m_comboNode.GetCurSel();
	m_nStation = m_comboStation.GetCurSel();

	m_editMI8.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI8=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI9.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI9=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI10.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI10=%s",m_nNode,m_strMI);	SendStr(Buff);

	m_editMI990.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI990=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI992.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI992=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI995.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI995=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI996.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI996=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI997.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI997=%s",m_nNode,m_strMI);	SendStr(Buff);
	m_editMI998.GetWindowText(m_strMI);	Buff.Format(L"MS%d,MI998=%s",m_nNode,m_strMI);	SendStr(Buff);

}

void CPmacMacroPage::SendStr(CString Buff)
{
	CString str;

	theApp.m_PmacUser.SendStr(Buff);
	m_Tx.Add(Buff);
	m_nLastTxPtr = m_Tx.GetSize()-1;

	str.Empty();
	for(int ii=m_Tx.GetSize()-1;ii>=0;ii--) {
		str += m_Tx.GetAt(ii);
		str += "\r\n";
	}
	m_editSent.SetWindowText(str);
}

void CPmacMacroPage::OnSelchangeComboNode() 
{
	m_nNode = m_comboNode.GetCurSel();
	UpdateAllControls();
}

void CPmacMacroPage::ConvertToDecimal(CString &Buff)
{
	int nn;

	if(Buff.Find(L"$") == 0) {
		Buff.Replace(L"$",L"0x");
		swscanf_s(Buff,_T("%x"),&nn);
		Buff.Format(L"%d",nn);
	}

}
