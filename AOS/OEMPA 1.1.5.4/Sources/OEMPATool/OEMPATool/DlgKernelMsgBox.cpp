// DlgKernelMsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPATool.h"
#include "DlgKernelMsgBox.h"

#define	WPARAM_MSGBOX					0x100
#define	WPARAM_MSGBOX_LIST				0x101
#define	WPARAM_MSGBOX_BUTTONS			0x102
#define	WPARAM_MSGBOX_CUSTOMIZEDAPI		0x103

extern UINT g_uiUTEventMessage;
DllImport bool UTDriver_IsUserInterfaceThread();//this function could be called to know if the current thread is attached to the management of window.

CDlgKernelMsgBox *CDlgKernelMsgBox::m_pDlgKernelMsgBox=NULL;

int CDlgKernelMsgBox::WaitMessageBox()
{
	bool bEnd;
	int iResult;

	bEnd = false;
	do{
		EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		bEnd = m_pDlgKernelMsgBox->m_bStop;
		iResult = m_pDlgKernelMsgBox->m_iResult;
		LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		if(bEnd)
			break;
		Sleep(50);
	}while(1);
	return iResult;
}
void WINAPI CDlgKernelMsgBox::myCallbackSystemMessageBox(const wchar_t *pMsg)
{
	if(!m_pDlgKernelMsgBox)
		return;
	while(!TryEnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox))
		Sleep(50);
	EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	m_pDlgKernelMsgBox->m_hWnd = 0;
	if(wcslen(pMsg)<2*MAX_PATH)
		wcscpy_s(m_pDlgKernelMsgBox->m_wcText,2*MAX_PATH,pMsg);
	m_pDlgKernelMsgBox->m_wcCaption[0] = 0;
	m_pDlgKernelMsgBox->m_nType = MB_OK;
	m_pDlgKernelMsgBox->m_iResult = 0;
	m_pDlgKernelMsgBox->m_bStart = true;
	m_pDlgKernelMsgBox->m_bStop = false;
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	if(UTDriver_IsUserInterfaceThread())
		m_pDlgKernelMsgBox->SendMessage(g_uiUTEventMessage,WPARAM_MSGBOX,0);//current thread already manage a window, so "SendMessage" is required to process immediately the display (with "PostMessage" a deadlock occurs: impossible to wait the next message inside the current processing).
	else
		m_pDlgKernelMsgBox->PostMessage(g_uiUTEventMessage,WPARAM_MSGBOX,0);//forbidden to display directly the MessageBox, need to use intermediary window
	WaitMessageBox();
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox);
}
void WINAPI CDlgKernelMsgBox::myCallbackSystemMessageBoxList(const wchar_t *pMsg)
{
	if(!m_pDlgKernelMsgBox)
		return;
	while(!TryEnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox))
		Sleep(50);
	EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	m_pDlgKernelMsgBox->m_hWnd = 0;
	if(wcslen(pMsg)<2*MAX_PATH)
		wcscpy_s(m_pDlgKernelMsgBox->m_wcText,2*MAX_PATH,pMsg);
	m_pDlgKernelMsgBox->m_wcCaption[0] = 0;
	m_pDlgKernelMsgBox->m_nType = MB_OK;
	m_pDlgKernelMsgBox->m_iResult = 0;
	m_pDlgKernelMsgBox->m_bStart = true;
	m_pDlgKernelMsgBox->m_bStop = false;
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	if(UTDriver_IsUserInterfaceThread())
		m_pDlgKernelMsgBox->SendMessage(g_uiUTEventMessage,WPARAM_MSGBOX_LIST,0);//current thread already manage a window, so "SendMessage" is required to process immediately the display (with "PostMessage" a deadlock occurs: impossible to wait the next message inside the current processing).
	else
		m_pDlgKernelMsgBox->PostMessage(g_uiUTEventMessage,WPARAM_MSGBOX_LIST,0);//forbidden to display directly the MessageBox, need to use intermediary window
	WaitMessageBox();
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox);
}
UINT WINAPI CDlgKernelMsgBox::myCallbackSystemMessageBoxButtons(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType)
{
	int iResult;

	if(!m_pDlgKernelMsgBox)
	{
		switch(nType)
		{
		case MB_OK:					return IDOK;break;
		case MB_OKCANCEL:			return IDOK;break;
		case MB_ABORTRETRYIGNORE:	return IDABORT;break;
		case MB_YESNOCANCEL:		return IDCANCEL;break;
		case MB_YESNO:				return IDNO;break;
		case MB_RETRYCANCEL:		return IDCANCEL;break;
		default:					return IDOK;break;
		}
	}
	while(!TryEnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox))
		Sleep(50);
	EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	m_pDlgKernelMsgBox->m_hWnd = 0;
	if(wcslen(pMsg)<2*MAX_PATH)
		wcscpy_s(m_pDlgKernelMsgBox->m_wcText,2*MAX_PATH,pMsg);
	if(wcslen(pTitle)<2*MAX_PATH)
		wcscpy_s(m_pDlgKernelMsgBox->m_wcCaption,2*MAX_PATH,pTitle);
	m_pDlgKernelMsgBox->m_nType = nType;
	m_pDlgKernelMsgBox->m_iResult = 0;
	m_pDlgKernelMsgBox->m_bStart = true;
	m_pDlgKernelMsgBox->m_bStop = false;
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	if(UTDriver_IsUserInterfaceThread())
		m_pDlgKernelMsgBox->SendMessage(g_uiUTEventMessage,WPARAM_MSGBOX_BUTTONS,0);//current thread already manage a window, so "SendMessage" is required to process immediately the display (with "PostMessage" a deadlock occurs: impossible to wait the next message inside the current processing).
	else
		m_pDlgKernelMsgBox->PostMessage(g_uiUTEventMessage,WPARAM_MSGBOX_BUTTONS,0);//forbidden to display directly the MessageBox, need to use intermediary window
	iResult = WaitMessageBox();
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox);
	return iResult;
}
int WINAPI CDlgKernelMsgBox::myCallbackOempaApiMessageBox(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType)
{
	int iResult;

	if(!m_pDlgKernelMsgBox)
	{
		switch(nType)
		{
		case MB_OK:					return IDOK;break;
		case MB_OKCANCEL:			return IDOK;break;
		case MB_ABORTRETRYIGNORE:	return IDABORT;break;
		case MB_YESNOCANCEL:		return IDCANCEL;break;
		case MB_YESNO:				return IDNO;break;
		case MB_RETRYCANCEL:		return IDCANCEL;break;
		default:					return IDOK;break;
		}
	}
	while(!TryEnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox))
		Sleep(50);
	EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	m_pDlgKernelMsgBox->m_hWnd = 0;
	if(wcslen(lpszText)<2*MAX_PATH)
		wcscpy_s(m_pDlgKernelMsgBox->m_wcText,2*MAX_PATH,lpszText);
	if(wcslen(lpszCaption)<2*MAX_PATH)
		wcscpy_s(m_pDlgKernelMsgBox->m_wcCaption,2*MAX_PATH,lpszCaption);
	m_pDlgKernelMsgBox->m_nType = nType;
	m_pDlgKernelMsgBox->m_iResult = 0;
	m_pDlgKernelMsgBox->m_bStart = true;
	m_pDlgKernelMsgBox->m_bStop = false;
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
	if(UTDriver_IsUserInterfaceThread())
		m_pDlgKernelMsgBox->SendMessage(g_uiUTEventMessage,WPARAM_MSGBOX_CUSTOMIZEDAPI,0);//current thread already manage a window, so "SendMessage" is required to process immediately the display (with "PostMessage" a deadlock occurs: impossible to wait the next message inside the current processing).
	else
		m_pDlgKernelMsgBox->PostMessage(g_uiUTEventMessage,WPARAM_MSGBOX_CUSTOMIZEDAPI,0);//forbidden to display directly the MessageBox, need to use intermediary window
	iResult = WaitMessageBox();
	LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionMsgBox);
	return iResult;
}

LRESULT CDlgKernelMsgBox::OnEventMessage(WPARAM wParam,LPARAM lParam)
{
	int iResult;

	switch(wParam)
	{
	case WPARAM_MSGBOX:
		AfxMessageBox(m_wcText);
		EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		m_bStop = true;
		m_iResult = 0;
		LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		break;
	case WPARAM_MSGBOX_LIST:
	case WPARAM_MSGBOX_CUSTOMIZEDAPI:
		m_ListMsg.AddString(m_wcText);
		EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		m_bStop = true;
		m_iResult = 0;
		LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		break;
	case WPARAM_MSGBOX_BUTTONS:
		iResult = MessageBox(m_wcText,m_wcCaption,m_nType);
		EnterCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		m_bStop = true;
		m_iResult = iResult;
		LeaveCriticalSection(&m_pDlgKernelMsgBox->m_CriticalSectionData);
		break;
	default:
		break;
	}
	return 0;
}

// CDlgKernelMsgBox dialog

IMPLEMENT_DYNAMIC(CDlgKernelMsgBox, CDialog)

CDlgKernelMsgBox::CDlgKernelMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKernelMsgBox::IDD, pParent)
	, m_bCheckEnable(FALSE)
{
	InitializeCriticalSection(&m_CriticalSectionMsgBox);
	InitializeCriticalSection(&m_CriticalSectionData);
	EnterCriticalSection(&m_CriticalSectionData);
	m_hWnd = 0;
	m_wcText[0] = 0;
	m_wcCaption[0] = 0;
	m_nType = MB_OK;
	m_iResult = 0;
	m_bStart = false;
	m_bStop = false;
	LeaveCriticalSection(&m_CriticalSectionData);
	Create(IDD_DIALOG_KERNELMESSAGEBOX,pParent);
	ShowWindow(SW_SHOW);
}

CDlgKernelMsgBox::~CDlgKernelMsgBox()
{
	DeleteCriticalSection(&m_CriticalSectionMsgBox);
	DeleteCriticalSection(&m_CriticalSectionData);
}

void CDlgKernelMsgBox::PostNcDestroy()
{
	RegisterCallbackMsgBox(false);
	if(m_pDlgKernelMsgBox==this)
		m_pDlgKernelMsgBox = NULL;
	delete this;
}

void CDlgKernelMsgBox::Display()
{
	if(!m_pDlgKernelMsgBox)
		m_pDlgKernelMsgBox = new CDlgKernelMsgBox();
	if(m_pDlgKernelMsgBox->IsIconic())
		m_pDlgKernelMsgBox->ShowWindow(SW_RESTORE);
	m_pDlgKernelMsgBox->ShowWindow(SW_SHOW);
}

void CDlgKernelMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bCheckEnable);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_ListMsg);
}

BEGIN_MESSAGE_MAP(CDlgKernelMsgBox, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgKernelMsgBox::OnBnClickedButtonDelete)
	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CDlgKernelMsgBox::OnEventMessage)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, &CDlgKernelMsgBox::OnBnClickedCheckEnable)
	ON_BN_CLICKED(IDCANCEL, &CDlgKernelMsgBox::OnBnClickedCancel)
END_MESSAGE_MAP()

// CDlgKernelMsgBox message handlers
BOOL CDlgKernelMsgBox::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgKernelMsgBox::OnBnClickedButtonDelete()
{
	m_ListMsg.ResetContent();
}

void CDlgKernelMsgBox::OnBnClickedCheckEnable()
{
	UpdateData();
	RegisterCallbackMsgBox(m_bCheckEnable?true:false);
}

void CDlgKernelMsgBox::RegisterCallbackMsgBox(bool bRegister)
{
	if(bRegister)
	{
		SetCallbackSystemMessageBox(myCallbackSystemMessageBox);
		SetCallbackSystemMessageBoxList(myCallbackSystemMessageBoxList);
		SetCallbackSystemMessageBoxButtons(myCallbackSystemMessageBoxButtons);
		SetCallbackMessageBox(myCallbackOempaApiMessageBox);
	}else{
		SetCallbackSystemMessageBox(NULL);
		SetCallbackSystemMessageBoxList(NULL);
		SetCallbackSystemMessageBoxButtons(NULL);
		SetCallbackMessageBox(NULL);
	}
}

void CDlgKernelMsgBox::OnBnClickedCancel()
{
	DestroyWindow();
}
