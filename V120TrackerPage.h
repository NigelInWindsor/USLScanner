#if !defined(AFX_V120TRACKERPAGE_H__13086DA5_00FB_4952_ADAA_200203A57B85__INCLUDED_)
#define AFX_V120TRACKERPAGE_H__13086DA5_00FB_4952_ADAA_200203A57B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// V120TrackerPage.h : header file
//

#include "usm.h"

/////////////////////////////////////////////////////////////////////////////
// CV120TrackerPage dialog

class CV120TrackerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CV120TrackerPage)

// Construction
public:
	CV120TrackerPage();
	~CV120TrackerPage();

// Dialog Data
	//{{AFX_DATA(CV120TrackerPage)
	enum { IDD = IDD_V120TRACKER_PAGE };
	CEdit	m_editZ;
	CEdit	m_editYaw;
	CEdit	m_editY;
	CEdit	m_editRoll;
	CEdit	m_editQZ;
	CEdit	m_editQY;
	CEdit	m_editQX;
	CEdit	m_editQW;
	CEdit	m_editPitch;
	CEdit	m_editX;
	CButton	m_buttonDisconnect;
	CButton	m_buttonConnect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CV120TrackerPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CV120TrackerPage)
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	usm<float>* m_pUSM; //Shared memory class.
	float m_x;
	void Update();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_V120TRACKERPAGE_H__13086DA5_00FB_4952_ADAA_200203A57B85__INCLUDED_)
