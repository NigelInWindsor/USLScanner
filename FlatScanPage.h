#if !defined(AFX_FLATSCANPAGE_H__FE5E2D5D_7FC2_4642_BD4E_953973BC068E__INCLUDED_)
#define AFX_FLATSCANPAGE_H__FE5E2D5D_7FC2_4642_BD4E_953973BC068E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlatScanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlatScanPage dialog

class CFlatScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFlatScanPage)

// Construction
public:
	void Invalidate(bool bErase);
	BOOL RedrawWindow(LPCRECT lpRectUpdate, CRgn* prgnUpdate, UINT flags);
	int	m_nOldScanLine;
	int GetCurrentProbeNumber();
	bool m_bInvalidateFlag;
	bool m_bPumpStatus;
	void WorldToClient(float fX, float fY, CPoint &pt);
	void WorldToClient(CCoord &CpSurface, CPoint &pt);
	void ClientToWorld(CPoint &pt, CCoord &CpSurface);
	CPoint	m_ptClient;
	COLORREF	*m_pArray;
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void SetToolBarCheckedState();
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	void SetAccessPrivelage();
	void UpdateAllControls();
	
	CFlatScanPage();
	~CFlatScanPage();

// Dialog Data
	//{{AFX_DATA(CFlatScanPage)
	enum { IDD = IDD_FLAT_SCAN_DLG };
	CComboBox	m_comboFirstProbe;
	CEdit	m_editScanTime;
	CStatic	m_staticFinishLength;
	CEdit	m_editScanLine;
	CStatic	m_staticView;
	CComboBox	m_comboProbeCount;
	CEdit	m_editCurrentLine;
	CEdit	m_editProbeSeparation;
	CSpinButtonCtrl	m_spinScanSpeed;
	CSpinButtonCtrl	m_spinSlowLength;
	CSpinButtonCtrl	m_spinFastLength;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editScanSpeed;
	CEdit	m_editResumeLine;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFastIncrement;
	CEdit	m_editFinishPt;
	CEdit	m_editStartPt;
	CEdit	m_editFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFlatScanPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFlatScanPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStart();
	afx_msg void OnChangeEditSpeed();
	afx_msg void OnChangeEditPt0();
	afx_msg void OnChangeEditPt1();
	afx_msg void OnChangeEditIncrement0();
	afx_msg void OnChangeEditIncrement1();
	afx_msg void OnChangeEditFilepath();
	afx_msg void OnDeltaposSpinIncrement0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinIncrement1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonBrowse();
	afx_msg void OnChangeEditProbeSeparation();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonModeCoordinates();
	afx_msg void OnButtonModeSize();
	afx_msg void OnButtonAbort();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFlatGotohere();
	afx_msg void OnFlatGotostart();
	afx_msg void OnFlatGotofinish();
	afx_msg void OnFlatStorestart();
	afx_msg void OnFlatStorefinish();
	afx_msg void OnButtonStoreStart();
	afx_msg void OnButtonStoreFinish();
	afx_msg void OnButtonLeftPump();
	afx_msg void OnButtonNomalizeAmplitudes();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnButtonGotoZero();
	afx_msg void OnButtonOrigin();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveas();
	afx_msg void OnSelchangeComboFirstProbe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATSCANPAGE_H__FE5E2D5D_7FC2_4642_BD4E_953973BC068E__INCLUDED_)
