#if !defined(AFX_MOSAICDLG_H__6C3DCB02_8760_40AF_A9DC_8585FF8C2320__INCLUDED_)
#define AFX_MOSAICDLG_H__6C3DCB02_8760_40AF_A9DC_8585FF8C2320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MosaicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMosaicDlg dialog

class CMosaicDlg : public CDialog
{
// Construction
public:
	int m_nNumberLines;
	int m_nNumberSamples;
	void UpdateAllControls();
	void AppendFileTo(int nWhichSide);
	CWnd *m_pParent;
	CWnd** m_pDlg;
	CData *m_pData;
	
	CMosaicDlg(CWnd* pParent = NULL ,CWnd** pDlg = NULL, CData *pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMosaicDlg)
	enum { IDD = IDD_MOSAIC_DLG };
	CSpinButtonCtrl	m_spinNumberSamples;
	CEdit	m_editNumberSamples;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMosaicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMosaicDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAppendtobottom();
	afx_msg void OnButtonAppendtoleft();
	afx_msg void OnButtonAppendtoright();
	afx_msg void OnButtonAppendtotop();
	afx_msg void OnButtonCropverticalline();
	afx_msg void OnChangeEditNumberSamples();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonCropleft();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOSAICDLG_H__6C3DCB02_8760_40AF_A9DC_8585FF8C2320__INCLUDED_)
