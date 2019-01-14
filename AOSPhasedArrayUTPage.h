#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "EditSpinCtrl.h"

class CAOSPhasedArrayUTPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOSPhasedArrayUTPage)

	// Construction
public:
	bool m_bThreadEnabled;
	HANDLE m_hEvent;
	CWinThread* m_pThread;
	int m_nUpdateHardware;
	HBRUSH m_hBr;
	HANDLE m_hSemaphore;
	

public:
	CAOSPhasedArrayUTPage();
	~CAOSPhasedArrayUTPage();

	// Dialog Data
	//{{AFX_DATA(CAOSPhasedArrayPage)
	enum { IDD = IDD_AOS_PHASEDARRAY_UT_PAGE};
	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOSPhasedArrayPage)
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	void StartThread();
	void SuspendThread();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CAOSPhasedArrayPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	void UpdateAllControls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CEditSpinCtrl m_editspinMinFilterFrequency;
	CEditSpinCtrl m_editspinMaxFilterFrequency;
	CEditSpinCtrl m_editspinRipple;
	CEditSpinCtrl m_editspinStopGain;
	CEditSpinCtrl m_editspinAnalogueGain;
	CEditSpinCtrl m_editspinDigitalGain;
	CEditSpinCtrl m_editspinPulseWidth;
	CEditSpinCtrl m_editspinPRF;
	CEditSpinCtrl m_editspinFilterGain;
	afx_msg void OnEnChangeEditAnalogueGain();
	afx_msg void OnEnChangeEditDigitalGain();
	afx_msg void OnDeltaposSpinAnalogueGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinDigitalGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditPulseWidth();
	afx_msg void OnDeltaposSpinPulseWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPrf();
	afx_msg void OnDeltaposSpinPrf(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_comboRectify;
	afx_msg void OnCbnSelchangeComboRectify();
	afx_msg void OnEnChangeEditMaxFilterFrequency();
	afx_msg void OnEnChangeEditMinFilterFrequency();
	CComboBox m_comboFilterType;
	afx_msg void OnCbnSelchangeComboFilterType();
	afx_msg void OnBnClickedButtonApply();
	CComboBox m_comboDisplayMode;
	afx_msg void OnCbnSelchangeComboDisplayMode();
	afx_msg void OnPaint();
	void RenderFrequency(CPaintDC * pDC);
	void RenderPhase(CPaintDC * pDC);
	CStatic m_staticView;
	afx_msg void OnDeltaposSpinMinFilterFrequency(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMaxFilterFrequency(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRipple(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinStopGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnDeltaposSpinFilterGain(NMHDR *pNMHDR, LRESULT *pResult);
};