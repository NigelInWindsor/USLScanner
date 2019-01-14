#if !defined(AFX_SINEWAVEDLG_H__865C7919_7D10_451F_9088_9D85C77D3F78__INCLUDED_)
#define AFX_SINEWAVEDLG_H__865C7919_7D10_451F_9088_9D85C77D3F78__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SineWaveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSineWaveDlg dialog

class CSineWaveDlg : public CDialog
{
// Construction
public:
	int m_nMode;
	float m_fNumberCycles;
	float m_fSeg;
	float m_fPhase;
	float m_fAmp;
	float m_fPitch;
	CCoord m_Cp[2];
	
	void UpdateAllControls();
	CSineWaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSineWaveDlg)
	enum { IDD = IDD_SINEWAVE_DLG };
	CEdit	m_editCycles;
	CComboBox	m_comboMode;
	CEdit	m_editSegment;
	CEdit	m_editFinish;
	CEdit	m_editStart;
	CEdit	m_editPitch;
	CEdit	m_editPhase;
	CEdit	m_editAmplitude;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSineWaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSineWaveDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonStoreStart();
	afx_msg void OnButtonFinish();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINEWAVEDLG_H__865C7919_7D10_451F_9088_9D85C77D3F78__INCLUDED_)
