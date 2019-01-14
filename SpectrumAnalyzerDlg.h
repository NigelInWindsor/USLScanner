//{{AFX_INCLUDES()
#include "uslbar.h"
//}}AFX_INCLUDES
#if !defined(AFX_SPECTRUMANALYZERDLG_H__26521398_0490_44C8_9417_DF70496CD3AD__INCLUDED_)
#define AFX_SPECTRUMANALYZERDLG_H__26521398_0490_44C8_9417_DF70496CD3AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpectrumAnalyzerDlg.h : header file
//
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CSpectrumAnalyzerDlg dialog

#include "FFT.h"	// Added by ClassView
class CSpectrumAnalyzerDlg : public CDialog
{
// Construction
public:
	int		nRealArray[8192];
	void UpdateAllControls();
	int		nGArray[0x8000];
	void CalculateGArray();
	CFFT m_FFT;
	void CalculateScrollBarFromFrequency();
	void DrawGraticule(CDC* pDC,CRect* prr,int nAction);
	void DrawFFTTrace(CPaintDC* pDC,CRect* prr,int nAction);
	void DeAllocateScreenMemory();
	void AllocateScreenMemory();
	CRect m_rectView;
	COLORREF* m_pScreenFFTArray;
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosFrequency;
	int	m_nSlot;
	HBRUSH m_hBr;
	
	CWnd** m_pDlg;
	CSpectrumAnalyzerDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpectrumAnalyzerDlg)
	enum { IDD = IDD_SPECTRUM_ANALYZER_DIALOG };
	CComboBox	m_comboWindow;
	CComboBox	m_comboTimeSlot;
	CComboBox	m_comboRFSource;
	CComboBox	m_comboResolvableFrequency;
	CSpinButtonCtrl	m_spinGain;
	CEdit	m_editGain;
	CStatic	m_staticView;
	CUSLBar	m_scrollbarFrequency;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectrumAnalyzerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpectrumAnalyzerDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnScrollHasMovedUslbarctrlFreq(long LowPosition, long HighPosition, long Width);
	afx_msg void OnSelchangeComboFftResolvable();
	afx_msg void OnChangeEditFftGain();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboRfSource();
	afx_msg void OnScrollHasMovedUslbarctrlFrequency(long LowPosition, long HighPosition, long Width);
	afx_msg void OnSelchangeComboTs();
	afx_msg void OnSelchangeComboWindow();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRUMANALYZERDLG_H__26521398_0490_44C8_9417_DF70496CD3AD__INCLUDED_)
