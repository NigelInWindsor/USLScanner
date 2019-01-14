#pragma once
#include "afxwin.h"


// CDlgHwLink dialog

class CDlgHwLink : public CDialog
{
	DECLARE_DYNAMIC(CDlgHwLink)

public:
	CDlgHwLink(CHWDevice *pDevice,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHwLink();

// Dialog Data
	enum { IDD = IDD_DIALOG_HW_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	void InitSystemList();

	DECLARE_MESSAGE_MAP()
public:

	CHWDevice *m_pDevice;
	CListBox m_SystemList;
	CButton m_ButtonDeselect;
	CComboBox m_ComboDefaultHwLink;
	CListBox m_CurrentDevice;
	int m_iDeviceId;
	bool m_bIAmMaster;
	int m_iMasterDeviceId;
	int m_iSystemList;
	int m_iComboDefaultHwLink;
	BOOL m_bCheckMatchedDevice;

	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeListBoard();
	afx_msg void OnBnClickedButtonDeselect();
	afx_msg void OnCbnSelchangeComboDefaultHwLink();
	afx_msg void OnBnClickedCheckMatchedDevice();
};
