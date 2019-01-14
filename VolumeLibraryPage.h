#if !defined(AFX_VOLUMELIBRARYPAGE_H__4F2F3435_4A0B_4485_8C1A_44A18721DEB5__INCLUDED_)
#define AFX_VOLUMELIBRARYPAGE_H__4F2F3435_4A0B_4485_8C1A_44A18721DEB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeLibraryPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVolumeLibraryPage dialog

class CVolumeLibraryPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CVolumeLibraryPage)

// Construction
public:
	HBRUSH m_hBr;
	float TimeToDistance(float fTime);
	void InvalidateParent();
	
	CVolumeLibraryPage(CWnd* pParent = NULL, GatesData* pGate = NULL);
	~CVolumeLibraryPage();

// Dialog Data
	//{{AFX_DATA(CVolumeLibraryPage)
	enum { IDD = IDD_VOLUME_CSCAN_LIBRARY_PAGE };
	CSpinButtonCtrl	m_spinGateInterval;
	CSpinButtonCtrl	m_spinStart;
	CSpinButtonCtrl	m_spinNumberSlices;
	CSpinButtonCtrl	m_spinGateWidth;
	CComboBox	m_comboGateNumber;
	CEdit	m_editStartDepth;
	CEdit	m_editTotalDepth;
	CEdit	m_editNumberofGates;
	CEdit	m_editGateWidth;
	CEdit	m_editGateInterval;
	//}}AFX_DATA

	void UpdateAllControls();


	CWnd* m_pParent;
	GatesData* m_pGate;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CVolumeLibraryPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CVolumeLibraryPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnChangeEditGateWidth();
	afx_msg void OnChangeEditGateInterval();
	afx_msg void OnChangeEditNumOfGates();
	afx_msg void OnButtonCreate();
	afx_msg void OnChangeEditStart();
	afx_msg void OnSelchangeComboGateNumber();
	afx_msg void OnDeltaposSpinGateInterval(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinGateWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinNumberSlices(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinStart(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonGateWidth();
	void InvalidateNumberSlices();
	void InvalidateGateInterval();
	CEdit m_editStopDepth;
	CSpinButtonCtrl m_spinStopDepth;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMELIBRARYPAGE_H__4F2F3435_4A0B_4485_8C1A_44A18721DEB5__INCLUDED_)
