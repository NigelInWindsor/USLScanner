#pragma once


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
	CEdit m_EditSystem;
	int m_iDeviceId;
	CString m_strEditSystem;
	bool m_bIAmMaster;
	int m_iMasterDeviceId;
	int m_iSystemList;
	int m_iComboDefaultHwLink;
	BOOL m_bCheckMatchedDevice;
	CString m_strEditMatchedDevice;

	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeListBoard();
	afx_msg void OnBnClickedButtonDeselect();
	afx_msg void OnCbnSelchangeComboDefaultHwLink();
	afx_msg void OnBnClickedCheckMatchedDevice();
};
