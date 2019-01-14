#if !defined(AFX_BMPVIEWERDLG_H__58BFAAEA_7429_4DD9_AC25_763DCCFDC3E8__INCLUDED_)
#define AFX_BMPVIEWERDLG_H__58BFAAEA_7429_4DD9_AC25_763DCCFDC3E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMPViewerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBMPViewerDlg dialog

class CBMPViewerDlg : public CDialog
{
// Construction
public:
	void DrawImage(CPaintDC* pDC,CRect rr);
	void RetrieveInitialImage();
	BITMAPFILEHEADER m_BmpFileHeader;
	BITMAPINFOHEADER m_BmpHeader;
	COLORREF* m_pForeImage;
	CFont* m_pFont;
	LOGFONT m_LogFont;
	CString m_FilePath;
	
	CWnd** m_pDlg;
	CBMPViewerDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBMPViewerDlg)
	enum { IDD = IDD_BMP_VIEWER_DLG };
	CStatic	m_staticView;
	CSliderCtrl	m_sliderBlue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMPViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBMPViewerDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPVIEWERDLG_H__58BFAAEA_7429_4DD9_AC25_763DCCFDC3E8__INCLUDED_)
