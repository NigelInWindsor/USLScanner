#if !defined(AFX_MARKERPENPAGE_H__5A7BE829_E34A_48DC_9687_36A3F3AA1E06__INCLUDED_)
#define AFX_MARKERPENPAGE_H__5A7BE829_E34A_48DC_9687_36A3F3AA1E06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkerPenPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarkerPenPage dialog

class CMarkerPenPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMarkerPenPage)

// Construction
public:
	bool ExtractVector(CEdit *pEdit, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm = NULL,float *pfAngle = NULL);
	void UpdateAllControls();
	
	CMarkerPenPage();
	~CMarkerPenPage();

// Dialog Data
	//{{AFX_DATA(CMarkerPenPage)
	enum { IDD = IDD_MARKER_PEN_DLG };
	CEdit	m_editJunk;
	CEdit	m_editCoord;
	CSpinButtonCtrl	m_spinOnTime;
	CEdit	m_editOnTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMarkerPenPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMarkerPenPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCoord();
	afx_msg void OnChangeEditOnTime();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditJunk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKERPENPAGE_H__5A7BE829_E34A_48DC_9687_36A3F3AA1E06__INCLUDED_)
