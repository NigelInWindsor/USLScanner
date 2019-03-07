#pragma once
#include <afxdlgs.h>
#include "ResizablePage.h"

class CPhasedArrayFocalLawsDlg : public CResizablePage
{
public:
	int		m_nSide;
	HBRUSH m_hBr;
	CPhasedArrayFocalLawsDlg();
	~CPhasedArrayFocalLawsDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();

	void UpdateAllControls();

	enum { IDD = IDD_PHASEDARRAY_FOCAL_LAWS_DLG	};
	CSpinButtonCtrl	m_spinTxLastElement;
	CEdit			m_editTxLastElement;
	CSpinButtonCtrl	m_spinTxElementPitch;
	CEdit			m_editTxFirstElement;
	CSpinButtonCtrl	m_spinTxFirstElement;
	CEdit			m_editTxElementPitch;
	CSpinButtonCtrl	m_spinTxAperture;
	CEdit			m_editTxAperture;
	CComboBox		m_comboTXFocalLawPitch;
	CSpinButtonCtrl m_spinTxBeamAngle;
	CSpinButtonCtrl m_spinTxFocalLength;
	CEdit			m_editTxFocalLength;
	CEdit			m_editTxBeamAngle;

	CSpinButtonCtrl	m_spinRxLastElement;
	CEdit			m_editRxLastElement;
	CSpinButtonCtrl	m_spinRxElementPitch;
	CEdit			m_editRxFirstElement;
	CSpinButtonCtrl	m_spinRxFirstElement;
	CEdit			m_editRxElementPitch;
	CSpinButtonCtrl	m_spinRxAperture;
	CEdit			m_editRxAperture;
	CComboBox		m_comboRXFocalLawPitch;
	CEdit			m_editRxFocalLength[4];
	CSpinButtonCtrl m_spinRxFocalLength[4];
	CSpinButtonCtrl m_spinRxBeamAngle;
	CEdit			m_editRxBeamAngle;

	CComboBox		m_comboFiringOrder;
	CButton			m_checkReverseElements;



	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditTxAperture();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void ApplyFocalLaws();
	afx_msg void OnEnChangeEditRxAperture();
	afx_msg void OnCbnSelchangeComboTxPitch();
	afx_msg void OnCbnSelchangeComboRxPitch();
	afx_msg void OnEnChangeEditFirstTxElement();
	afx_msg void OnEnChangeEditLastTxElement();
	afx_msg void OnEnChangeEditFirstRxElement();
	afx_msg void OnEnChangeEditLastRxElement();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnCbnSelchangeComboFiringOrder();
	afx_msg void OnDeltaposSpinTxBeamAngle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTxFocalLength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRxBeamAngle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRxFocalLength0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRxFocalLength1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRxFocalLength2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRxFocalLength3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditRxFocalLength0();
	afx_msg void OnEnChangeEditRxFocalLength1();
	afx_msg void OnEnChangeEditRxFocalLength2();
	afx_msg void OnEnChangeEditRxFocalLength3();
	afx_msg void OnEnChangeEditTxBeamAngle();
	afx_msg void OnEnChangeEditRxBeamAngle();
	CEdit m_editFocalLaw;
	CSpinButtonCtrl m_spinFocalLaw;
	afx_msg void OnEnChangeEditFocalLaw();
	CButton m_checkRxEqualsTx;
	afx_msg void OnBnClickedCheckTxEqualsRx();
	afx_msg void OnBnClickedCheckReverseElements();
};

