// InterfacePcommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "InterfacePcommDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	long testInterface,dwDevice;
	BOOL pbSuccess; 
	IPmacDevice pmacDevice;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterfacePcommDlg dialog

CInterfacePcommDlg::CInterfacePcommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInterfacePcommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterfacePcommDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCtrlButton = false;
}

void CInterfacePcommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterfacePcommDlg)
	DDX_Control(pDX, IDC_EDIT_73, m_edit73);
	DDX_Control(pDX, IDC_EDIT_72, m_edit72);
	DDX_Control(pDX, IDC_EDIT_6891, m_edit6891);
	DDX_Control(pDX, IDC_EDIT_6890, m_edit6890);
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
	DDX_Control(pDX, IDC_EDIT_6802, m_edit6802);
	DDX_Control(pDX, IDC_EDIT_6801, m_edit6801);
	DDX_Control(pDX, IDC_EDIT_6800, m_edit6800);
	DDX_Control(pDX, IDC_EDIT_RESPONCE, m_editResponce);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_editCommand);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInterfacePcommDlg, CDialog)
	//{{AFX_MSG_MAP(CInterfacePcommDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DeviceOpen, OnDeviceOpen)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterfacePcommDlg message handlers

BOOL CInterfacePcommDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// TODO: Add extra initialization here
//	long testInterface,dwDevice;
//	BOOL pbSuccess; 
//	IPmacDevice pmacDevice;
//  CoInitialize(NULL);
//	testInterface = pmacDevice.CreateDispatch(_T("PcommServer.PmacDevice.1"));

  CoInitialize(NULL);
	testInterface = pmacDevice.CreateDispatch(_T("PcommServer.PmacDevice.1"));
    if (!testInterface)
      AfxMessageBox("Can Not Connect PcommServer Interface ");
    pmacDevice.SelectDevice(NULL,&dwDevice,&pbSuccess);

	int pbDriverOpen;
	pmacDevice.Open(dwDevice,&pbDriverOpen);
	if(pbDriverOpen)
		UpdateAllControls();
	else
		AfxMessageBox("Unable to communicate to PMAC");

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInterfacePcommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInterfacePcommDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInterfacePcommDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInterfacePcommDlg::OnDeviceOpen() 
{
  int pbDriverOpen;

	pmacDevice.Open(dwDevice,&pbDriverOpen);
  if(pbDriverOpen)
    AfxMessageBox("You now communicating to PMAC");
  else
    AfxMessageBox("Unable to communicate to PMAC");
  // TODO: Add your control notification handler code here
	
}


void CInterfacePcommDlg::OnTimer(UINT nIDEvent) 
{
	
	CDialog::OnTimer(nIDEvent);
}



BOOL CInterfacePcommDlg::PreTranslateMessage(MSG* pMsg) 
{
      BOOL bSuccess;
      long status,i=0;
      BSTR szTest1 = ::SysAllocString(L"");;
      BSTR RxString = ::SysAllocString(L"");;
      FILE  *fstream;
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
		case 17: m_bCtrlButton=true;//Ctrl button down
			break;
		case 'T':
			if(m_bCtrlButton==true) {
				Buff.Format("%c",0x14);
				m_editCommand.SetWindowText(Buff);
			};
			break;
		case 'V':
			if(m_bCtrlButton==true) {
				Buff.Format("%c",0x16);
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
				TxBuff.Format("I70=%s",Buff);
			}
			if(&m_edit71 == FromHandle(pMsg->hwnd)) {
				m_edit71.GetWindowText(Buff);
				TxBuff.Format("I71=%s",Buff);
			}
			if(&m_edit72 == FromHandle(pMsg->hwnd)) {
				m_edit72.GetWindowText(Buff);
				TxBuff.Format("I72=%s",Buff);
			}
			if(&m_edit73 == FromHandle(pMsg->hwnd)) {
				m_edit73.GetWindowText(Buff);
				TxBuff.Format("I73=%s",Buff);
			}
			if(&m_edit78 == FromHandle(pMsg->hwnd)) {
				m_edit78.GetWindowText(Buff);
				TxBuff.Format("I78=%s",Buff);
			}
			if(&m_edit79 == FromHandle(pMsg->hwnd)) {
				m_edit79.GetWindowText(Buff);
				TxBuff.Format("I79=%s",Buff);
			}
			if(&m_edit80 == FromHandle(pMsg->hwnd)) {
				m_edit80.GetWindowText(Buff);
				TxBuff.Format("I80=%s",Buff);
			}
			if(&m_edit81 == FromHandle(pMsg->hwnd)) {
				m_edit81.GetWindowText(Buff);
				TxBuff.Format("I81=%s",Buff);
			}
			if(&m_edit82 == FromHandle(pMsg->hwnd)) {
				m_edit82.GetWindowText(Buff);
				TxBuff.Format("I82=%s",Buff);
			}
			if(&m_edit6800 == FromHandle(pMsg->hwnd)) {
				m_edit6800.GetWindowText(Buff);
				TxBuff.Format("I6800=%s",Buff);
			}
			if(&m_edit6801 == FromHandle(pMsg->hwnd)) {
				m_edit6801.GetWindowText(Buff);
				TxBuff.Format("I6801=%s",Buff);
			}
			if(&m_edit6802 == FromHandle(pMsg->hwnd)) {
				m_edit6802.GetWindowText(Buff);
				TxBuff.Format("I6802=%s",Buff);
			}
			if(&m_edit6840 == FromHandle(pMsg->hwnd)) {
				m_edit6840.GetWindowText(Buff);
				TxBuff.Format("I6840=%s",Buff);
			}
			if(&m_edit6841 == FromHandle(pMsg->hwnd)) {
				m_edit6841.GetWindowText(Buff);
				TxBuff.Format("I6841=%s",Buff);
			}
			if(&m_edit6850 == FromHandle(pMsg->hwnd)) {
				m_edit6850.GetWindowText(Buff);
				TxBuff.Format("I6850=%s",Buff);
			}
			if(&m_edit6851 == FromHandle(pMsg->hwnd)) {
				m_edit6851.GetWindowText(Buff);
				TxBuff.Format("I6901=%s",Buff);
			}
			if(&m_edit6852 == FromHandle(pMsg->hwnd)) {
				m_edit6852.GetWindowText(Buff);
				TxBuff.Format("I6852=%s",Buff);
			}
			if(&m_edit6890 == FromHandle(pMsg->hwnd)) {
				m_edit6890.GetWindowText(Buff);
				TxBuff.Format("I6890=%s",Buff);
			}
			if(&m_edit6891 == FromHandle(pMsg->hwnd)) {
				m_edit6891.GetWindowText(Buff);
				TxBuff.Format("I6891=%s",Buff);
			}
			if(!TxBuff.IsEmpty()) {
				pmacDevice.GetResponseEx(dwDevice,TxBuff,0,&RxString,&status);
				Buff.Empty();
				if(::SysStringLen(RxString)) {
					Buff = RxString;
				}
				m_editResponce.SetWindowText(Buff);
			}
			return TRUE;
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CInterfacePcommDlg::UpdateAllControls()
{
	long status,i=0;
	CString Buff;
	BSTR RxString = ::SysAllocString(L"");;

	pmacDevice.GetResponseEx(dwDevice,_T("I70"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit70.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I71"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit71.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I72"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit72.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I73"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit73.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I78"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit78.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I79"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit79.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I80"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit80.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I81"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit81.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I82"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit82.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6800"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6800.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6801"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6801.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6802"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6802.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6840"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6840.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6841"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6841.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6850"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6850.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6851"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6851.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6852"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6852.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6890"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6890.SetWindowText(Buff);
	}
	pmacDevice.GetResponseEx(dwDevice,_T("I6891"),0,&RxString,&status);
	if(::SysStringLen(RxString)) {
		Buff = RxString;
		m_edit6891.SetWindowText(Buff);
	}

}

void CInterfacePcommDlg::OnButtonRefresh() 
{
	UpdateAllControls();	
}
