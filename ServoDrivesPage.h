#if !defined(AFX_SERVODRIVESPAGE_H__3FC1FFDA_48A4_4BBB_9B94_23FE82D46BD3__INCLUDED_)
#define AFX_SERVODRIVESPAGE_H__3FC1FFDA_48A4_4BBB_9B94_23FE82D46BD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServoDrivesPage.h : header file
//
#include "EditColour.h"
/////////////////////////////////////////////////////////////////////////////
// CServoDrivesPage dialog

class CServoDrivesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CServoDrivesPage)

// Construction
public:
	int m_nShelf;
	
	void UpdateAllControls();
	unsigned char m_cCondition[256];
	bool RequestConditionData();
	CStringArray m_DeviceNames;
	CString m_strName;
	CString m_strAddress;
	CString m_strDateTime;
	HANDLE m_hDevice;
	int m_nNumDevices;
	int m_nWhichDevice;

	bool RequestIDData(CString *Name, CString *Address, CString *DateTime);
	void InitializeDiagnostics();

	CServoDrivesPage();
	~CServoDrivesPage();

// Dialog Data
	//{{AFX_DATA(CServoDrivesPage)
	enum { IDD = IDD_SERVO_DRIVES_PAGE };
	CComboBox	m_comboAmpType3;
	CComboBox	m_comboAmpType2;
	CComboBox	m_comboAmpType1;
	CEditColour	m_editTemperature;
	CEditColour	m_editScanner5V;
	CEditColour	m_editScanner12V;
	CEditColour	m_editController5V;
	CEditColour	m_editPOP3;
	CEditColour	m_editPOP2;
	CEditColour	m_editPOP1;
	CEditColour	m_editNOP3;
	CEditColour	m_editNOP2;
	CEditColour	m_editNOP1;
	CEditColour	m_editPDriveAmp3;
	CEditColour	m_editPDriveAmp2;
	CEditColour	m_editNDriveAmp3;
	CEditColour	m_editNDriveAmp2;
	CEditColour	m_editHTAmp3;
	CEditColour	m_editHTAmp2;
	CEditColour	m_editFaultAmp3;
	CEditColour	m_editFaultAmp2;
	CEditColour	m_editCurrentAmp3;
	CEditColour	m_editCurrentAmp2;
	CEditColour	m_editPDriveAmp1;
	CEditColour	m_editNDriveAmp1;
	CEditColour	m_editHTAmp1;
	CEditColour	m_editFaultAmp1;
	CEditColour	m_editEnableAmp1;
	CEditColour	m_editCurrentAmp1;
	CEditColour	m_editAddress;
	CEditColour	m_editDate;
	CEditColour	m_editName;
	CComboBox	m_comboDrive;
	CEditColour	m_editMains;
	CEditColour	m_editEnable3;
	CEditColour	m_editEnable2;
	CEditColour	m_editEnable1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CServoDrivesPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CServoDrivesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboDrive();
	afx_msg void OnSelchangeComboAmpType1();
	afx_msg void OnSelchangeComboAmpType2();
	afx_msg void OnSelchangeComboAmpType3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVODRIVESPAGE_H__3FC1FFDA_48A4_4BBB_9B94_23FE82D46BD3__INCLUDED_)
