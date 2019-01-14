#if !defined(AFX_IMAGEPROPERTIESDLG_H__DC3850CC_32BA_4086_BB32_3C5EC793D314__INCLUDED_)
#define AFX_IMAGEPROPERTIESDLG_H__DC3850CC_32BA_4086_BB32_3C5EC793D314__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImagePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImagePropertiesDlg dialog

class CImagePropertiesDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void UpdateAllControls();
	CData* m_pData;
	
	FeatureStruct* m_pFeature;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CImagePropertiesDlg(CWnd* pParent=NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImagePropertiesDlg)
	enum { IDD = IDD_IMAGE_PROPERTIES_DIALOG };
	CComboBox	m_comboScale;
	CComboBox	m_comboCaptionPosition;
	CButton	m_checkDisplayFileName;
	CButton	m_checkCaptionRight;
	CButton	m_checkCaptionCenter;
	CEdit	m_editCaption;
	CButton	m_checkCaptionBottomEdge;
	CButton	m_checkCaptionTopEdge;
	CButton	m_checkTopBorder;
	CButton	m_checkRightBorder;
	CButton	m_checkLeftBorder;
	CButton	m_checkBottomBorder;
	CStatic	m_staticBorderColor;
	CSpinButtonCtrl	m_spinBorderWidth;
	CEdit	m_editBorderWidth;
	CComboBox	m_comboGate;
	CComboBox	m_comboBorder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImagePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImagePropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboGate();
	afx_msg void OnSelchangeComboBorder();
	afx_msg void OnChangeEditBorderWidth();
	afx_msg void OnStaticBorderColor();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnButtonGraticuleFont();
	afx_msg void OnCheckBottom();
	afx_msg void OnCheckLeft();
	afx_msg void OnCheckRight();
	afx_msg void OnCheckTop();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnCheckCaptionBottom();
	afx_msg void OnCheckCaptionTop();
	afx_msg void OnCheckCaptionCenter();
	afx_msg void OnCheckCaptionRight();
	afx_msg void OnCheckDisplayFilename();
	afx_msg void OnSelchangeComboCaptionPosition();
	afx_msg void OnSelchangeComboScale();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROPERTIESDLG_H__DC3850CC_32BA_4086_BB32_3C5EC793D314__INCLUDED_)
