#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include <afxdlgs.h>

class CDiscsShaftsPage :
	public CPropertyPage
{
	DECLARE_DYNCREATE(CDiscsShaftsPage)

public:
	HBRUSH m_hBr;
	enum { IDD = IDD_DISCS_SHAFTS_PAGE };

	CDiscsShaftsPage();
	~CDiscsShaftsPage();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	void UpdateAllControls();
	HRESULT SetAccessPrivelages(WPARAM wp = NULL, LPARAM lp = NULL);
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
protected:
	int		m_nOldScanLine;
	CEdit m_editFilePath;
	CEdit m_editScanSpeed;
	CSpinButtonCtrl m_spinScanSpeed;
	CEdit m_editCurrentLine;
	CEdit m_editResumeLine;
	CEdit m_editStartCoordinate;
	CEdit m_editFinishCoordinate;
	void AddCoordinate(int nIndex);
	CEdit m_editFastIncrement;
	CEdit m_editSlowIncrement;
	CEdit m_editFastEquivelantIncrement;
	CSpinButtonCtrl m_spinFastIncrement;
	CSpinButtonCtrl m_spinSlowIncrement;
	afx_msg void OnEnChangeEditFastIndex();
	afx_msg void OnEnChangeEditIncrement1();
	afx_msg void OnDeltaposSpinFastIndex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinIncrement1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditStartCoord();
	afx_msg void OnEnChangeEditFinishCoord();
	afx_msg void OnBnClickedButtonGotoStart();
	afx_msg void OnBnClickedButtonGotoFinish();
	afx_msg void OnBnClickedButtonStoreStart();
	afx_msg void OnBnClickedButtonStoreFinish();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnEnChangeEditSpeed();
	afx_msg void OnDeltaposSpinSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	void GenerateScanProfile();
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonAbort();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditResumeLine();
	CEdit m_editAdditionalAngle;
	CSpinButtonCtrl m_spinAdditionalAngle;
	CButton m_checkAdditionalAngle;
	afx_msg void OnEnChangeEditAdditionalTip();
	afx_msg void OnDeltaposSpinAdditionalTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckAdditionalTipAngle();
	afx_msg void OnBnClickedButtonZeroTt();
};

