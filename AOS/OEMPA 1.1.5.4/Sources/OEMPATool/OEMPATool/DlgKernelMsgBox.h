#pragma once
#include "afxwin.h"


// CDlgKernelMsgBox dialog

class CDlgKernelMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CDlgKernelMsgBox)

public:
	CDlgKernelMsgBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgKernelMsgBox();
	static void Display();

// Dialog Data
	enum { IDD = IDD_DIALOG_KERNELMESSAGEBOX };

protected:
	static CDlgKernelMsgBox *m_pDlgKernelMsgBox;
	static void WINAPI myCallbackSystemMessageBox(const wchar_t *pMsg);
	static void WINAPI myCallbackSystemMessageBoxList(const wchar_t *pMsg);
	static UINT WINAPI myCallbackSystemMessageBoxButtons(const wchar_t *pMsg,const wchar_t *pTitle,UINT nType);
	static int WINAPI myCallbackOempaApiMessageBox(HWND hWnd,LPCTSTR lpszText,LPCTSTR lpszCaption,UINT nType);
	static int WaitMessageBox();

	void UpdateDialog_();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	void RegisterCallbackMsgBox(bool bRegister);

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);
public:
	CRITICAL_SECTION m_CriticalSectionData;
	CRITICAL_SECTION m_CriticalSectionMsgBox;
	HWND m_hWnd;
	wchar_t m_wcText[2*MAX_PATH];
	wchar_t m_wcCaption[2*MAX_PATH];
	UINT m_nType;
	int m_iResult;
	bool m_bStart;
	bool m_bStop;
	BOOL m_bCheckEnable;
	CListBox m_ListMsg;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCheckEnable();
	afx_msg void OnBnClickedCancel();
};
