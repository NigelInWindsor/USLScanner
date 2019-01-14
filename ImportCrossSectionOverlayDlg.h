#if !defined(AFX_IMPORTCROSSSECTIONOVERLAYDLG_H__7070CF82_68C0_4B75_B748_35FC4391407E__INCLUDED_)
#define AFX_IMPORTCROSSSECTIONOVERLAYDLG_H__7070CF82_68C0_4B75_B748_35FC4391407E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportCrossSectionOverlayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportCrossSectionOverlayDlg dialog

class CImportCrossSectionOverlayDlg : public CDialog
{
// Construction
public:
	bool GetHeader(float *pArray);
	float *m_pfHeader;
	int m_nHeaderSize;
	int m_nOffset;
	bool m_bMirror;
	void InvalidateParent();
	int getSamplesLine();
	bool GetLine(int nMaxLength,float *pArray);
	float* m_pfThickness;
	CStringArray m_strTextRows;
	int	m_nSamplesLine;
	int m_nNumberLines;
	int m_nLineNumber;
	void UpdateAllControls();
	int m_nFormat;
	bool ImportRRSingaporeCSV(CFile *pFile);
	CWnd** m_pDlg;
	CWnd* m_pParent;
	
	CImportCrossSectionOverlayDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportCrossSectionOverlayDlg)
	enum { IDD = IDD_IMPORT_CROSSSECTION_OVERLAY_DLG };
	CSliderCtrl	m_sliderOverlayOffset;
	CButton	m_checkMirror;
	CSliderCtrl	m_sliderOffset;
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	CComboBox	m_comboFormat;
	CEdit	m_editFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportCrossSectionOverlayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportCrossSectionOverlayDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFormat();
	afx_msg void OnButtonOpen();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckMirror();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTCROSSSECTIONOVERLAYDLG_H__7070CF82_68C0_4B75_B748_35FC4391407E__INCLUDED_)
