#pragma once
#include <afxdlgs.h>
#include "ResizablePage.h"

class CPhasedArrayFocalLawsDlg : public CResizablePage
{
public:
	int		m_nSide;
	int		m_nUpdateHardware;
	HBRUSH m_hBr;
	HANDLE	m_hSemaphore;
	CPhasedArrayFocalLawsDlg();
	~CPhasedArrayFocalLawsDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	void ApplyFocalLaws();


	void StartThread();

	void SuspendThread();

	void UpdateAllControls();

	enum { IDD = IDD_PHASEDARRAY_FOCAL_LAWS_DLG	};
	CSpinButtonCtrl	m_spinTxElementPitch;
	CEdit			m_editTxElementPitch;
	CSpinButtonCtrl	m_spinTxAperture;
	CEdit			m_editTxAperture;
	CComboBox		m_comboTXFocalLawPitch;
	CSpinButtonCtrl m_spinTxBeamAngle;
	CSpinButtonCtrl m_spinTxFocalLength;
	CEdit			m_editTxFocalLength;
	CEdit			m_editTxBeamAngle;

	CSpinButtonCtrl	m_spinRxElementPitch;
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
	afx_msg void OnEnChangeEditRxAperture();
	afx_msg void OnCbnSelchangeComboTxPitch();
	afx_msg void OnCbnSelchangeComboRxPitch();
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
	CComboBox m_comboFirstElementTx;
	CComboBox m_comboLastElementTx;
	CComboBox m_comboFirstElementRx;
	CComboBox m_comboLastElementRx;
	afx_msg void OnCbnSelchangeComboFirstElementTx();
	afx_msg void OnCbnSelchangeComboLastElementTx();
	afx_msg void OnCbnSelchangeComboFirstElementRx();
	afx_msg void OnCbnSelchangeComboLastElementRx();
	afx_msg void OnEnChangeEditTxFocalLength();
	CEdit m_editGain;
	CSpinButtonCtrl m_spinGain;
	afx_msg void OnEnChangeEditGain();
	afx_msg void OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult);
};

