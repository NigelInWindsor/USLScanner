#pragma once


// CDlgTestFWMemory dialog

class CDlgTestFWMemory : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestFWMemory)

public:
	CDlgTestFWMemory(CHWDeviceOEMPA *pHWDeviceOEMPA,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTestFWMemory();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEST_FW_MEMORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CHWDeviceOEMPA *m_pHWDeviceOEMPA;
	int m_iEditCycleCount;
	afx_msg void OnBnClickedButtonCycle();
	afx_msg void OnBnClickedButtonFilter();
	CString m_strEditData;
};
