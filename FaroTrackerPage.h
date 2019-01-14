#if !defined(AFX_FAROTRACKERPAGE_H__1BB68F23_2E35_4E29_8C23_9FBFDE3A90F1__INCLUDED_)
#define AFX_FAROTRACKERPAGE_H__1BB68F23_2E35_4E29_8C23_9FBFDE3A90F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaroTrackerPage.h : header file
//
#include "FaroElementExpansionDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFaroTrackerPage dialog

class CFaroTrackerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFaroTrackerPage)

// Construction
public:
	bool CheckFileExists(CString FileName);
	bool CheckDirExists(CString Dir);
	bool CheckAllFilesExist();
	bool AddCloudPoint();
	void UpdateExpandedElement();
	void Update3DView();
	bool FillNewElement(VertexArrayType Type);
	void SaveTracker(CUSLFile *pFile);
	void SaveTracker();
	void RetrieveTracker(CUSLFile* pFile);
	void RetrieveTracker();
	CString m_FileName;
	int	m_nOrder;
	int m_nIndex;
	bool m_bCtrlButton;
	bool m_bShiftButton;
	void FillElementList();
	void FillList();
	void CreateColumns();
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CFaroElementExpansionDlg* m_pFaroElementExpansionDlg;
	HBRUSH m_hBr;
	
	void UpdateAllControls();
	void SetAccessPrivelages();
	void MoveAllCloudPointsToNewTaughtLine(bool bDeleteCloudPoints);
	void UpdateProfileDlg();
	CFaroTrackerPage();
	~CFaroTrackerPage();

// Dialog Data
	//{{AFX_DATA(CFaroTrackerPage)
	enum { IDD = IDD_FARO_TRACKER_PAGE };
	CComboBox	m_comboPageDown;
	CComboBox	m_comboPageUp;
	CEdit	m_editCloudLength;
	CIPAddressCtrl	m_ipAddress;
	CEdit	m_editMessage;
	CEdit	m_editElementStats;
	CListCtrl	m_listElements;
	CListCtrl	m_listCoords;
	CButton	m_buttonStartStop;
	CEdit	m_editCoordinate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFaroTrackerPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFaroTrackerPage)
	afx_msg void OnRclickListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonConnect();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonMeasure();
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnSelchangeListElements();
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFarocoordsDelete();
	afx_msg void OnFarocoordsLine();
	afx_msg void OnFarocoordsArc();
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStartUp();
	afx_msg void OnGetdispinfoListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFaroelementOrigin();
	afx_msg void OnFarocoordsAdd000();
	afx_msg void OnFaroelementDelete();
	afx_msg void OnFarocoordsLookatthispoint();
	afx_msg void OnFarocoordsFrameOrigin();
	afx_msg void OnFarocoordsFrameXvector();
	afx_msg void OnFarocoordsFrameYvector();
	afx_msg void OnFarocoordsTooltipOrigin();
	afx_msg void OnButtonWorldCoordinates();
	afx_msg void OnButtonToolCoordinates();
	afx_msg void OnButtonFrameCoordinates();
	afx_msg void OnButtonDelete();
	afx_msg void OnFileSaveas();
	afx_msg void OnFileOpen();
	afx_msg void OnButtonPolar();
	afx_msg void OnButtonRectangular();
	afx_msg void OnFaroelementCircumcentertolist();
	afx_msg void OnFarocoordsTooltipPlane();
	afx_msg void OnFaroelementExpand();
	afx_msg void OnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFarocoordsFrameHorizontalplane();
	afx_msg void OnFarocoordsFrameVerticalplane();
	afx_msg void OnFarocoordsFrameZvector();
	afx_msg void OnFarocoordsAngle();
	afx_msg void OnChangeEditCloudLength();
	afx_msg void OnButtonEarthLevel();
	afx_msg void OnSelchangeComboRemotePageUp();
	afx_msg void OnSelchangeComboRemotePageDown();
	afx_msg void OnFarocoordsTransferTaughtPointsNewLine();
	afx_msg void OnFaroCoordsTransferReferencePoints();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAROTRACKERPAGE_H__1BB68F23_2E35_4E29_8C23_9FBFDE3A90F1__INCLUDED_)
