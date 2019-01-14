#if !defined(AFX_MACHINEMONITORPAGE_H__A853E6A7_6D3E_44D2_A3E4_8671CCB3BBC6__INCLUDED_)
#define AFX_MACHINEMONITORPAGE_H__A853E6A7_6D3E_44D2_A3E4_8671CCB3BBC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MachineMonitorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMachineMonitorPage dialog

class CMachineMonitorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMachineMonitorPage)

// Construction
public:
	int			m_nTopRow;
	int			m_nEntries;
	COLORREF	m_rgbText[300];
	COLORREF	m_rgbBk[300];
	CStringArray m_strArray;
	

	void FillNextEntry(CString Buff, COLORREF rgbText, COLORREF rgbBk);
	void RenderHengshen();
	void UpdateAllControls();
	CMachineMonitorPage();
	~CMachineMonitorPage();

// Dialog Data
	//{{AFX_DATA(CMachineMonitorPage)
	enum { IDD = IDD_MACHINE_MONITOR_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMachineMonitorPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMachineMonitorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACHINEMONITORPAGE_H__A853E6A7_6D3E_44D2_A3E4_8671CCB3BBC6__INCLUDED_)
