#if !defined(AFX_PHASEDARRAYPROBEDLG_H__44EFEF1C_D506_49EC_BE75_89BAE28D4FF3__INCLUDED_)
#define AFX_PHASEDARRAYPROBEDLG_H__44EFEF1C_D506_49EC_BE75_89BAE28D4FF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhasedArrayProbeDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ResizablePage.h"


/////////////////////////////////////////////////////////////////////////////
// CPhasedArrayProbeDlg dialog

class CPhasedArrayProbeDlg : public CResizablePage
{
// Construction
public:
	void InvalidateDelays();
	void SetToolBarCheckedState();
	void ApplyFocalLaws();
	void setWindowTitle();
	CRect m_rrView;
	int m_nColumn;
	int	m_nIndex;
	int m_nSide;
	void FillList();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	void UpdateAllControls();
	HBRUSH m_hBr;
	HANDLE m_hSemaphore;
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticTable;
	CEditSpinItem* m_pEditSpinItem;
	
	bool m_bThreadEnabled;
	bool m_bThreadFinished;
	int m_nUpdateHardware;
	CWinThread* m_pThread;
	CPhasedArrayProbeDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhasedArrayProbeDlg)
	enum { IDD = IDD_PHASED_ARRAY_PROBE_DLG };
	CComboBox		m_comboElementCount;
	CEdit			m_editElementPitch;
	CSpinButtonCtrl	m_spinElementPitch;

	CSpinButtonCtrl	m_spinTxLastElement;
	CEdit			m_editTxLastElement;
	CSpinButtonCtrl	m_spinTxElementPitch;
	CEdit			m_editTxFirstElement;
	CSpinButtonCtrl	m_spinTxFirstElement;
	CEdit			m_editTxElementPitch;
	CSpinButtonCtrl	m_spinTxAperture;
	CEdit			m_editTxAperture;

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

	CEdit			m_editWedgeAngle;
	CSpinButtonCtrl m_spinWedgeAngle;
	CEdit	m_editFocalLength;
	CComboBox	m_comboDelayAlgorithm;
	CSpinButtonCtrl	m_spinFocalLength;
	CEdit	m_editDelays;
	CEdit	m_editDelayLineLength;
	CButton	m_checkReverseArray;
	CSpinButtonCtrl	m_spinFirstElement;
	CListCtrl	m_listElements;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhasedArrayProbeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	float m_fViewPortWidth;
	float m_fViewPortHeight;
	D3DXVECTOR3 m_vectO;


	// Generated message map functions
	//{{AFX_MSG(CPhasedArrayProbeDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InvalidateHardware(bool bFlag);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClickListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListElements(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPhasedarrayprobelistNumberelements128();
	afx_msg void OnPhasedarrayprobelistNumberelements16();
	afx_msg void OnPhasedarrayprobelistNumberelements24();
	afx_msg void OnPhasedarrayprobelistNumberelements256();
	afx_msg void OnPhasedarrayprobelistNumberelements32();
	afx_msg void OnPhasedarrayprobelistNumberelements64();
	afx_msg void OnPhasedarrayprobelistNumberelements8();
	afx_msg void OnChangeEditElementPitch();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditElements();
	afx_msg void OnCheckReverseArray();
	afx_msg void OnPhasedarrayprobelistAutofill();
	afx_msg void OnButtonElements();
	afx_msg void OnButtonFocalLaws();
	afx_msg void OnSelchangeComboNumberElements();
	afx_msg void OnChangeEditFocalLength();
	afx_msg void OnSelchangeComboDelayAlogorithm();
	afx_msg void OnDeltaposSpinElementPitch(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStatic m_staticView;
	afx_msg void OnEnChangeEditWedgeAngle();
	afx_msg void OnDeltaposSpinWedgeAngle(NMHDR *pNMHDR, LRESULT *pResult);
	void RenderLinearProbe(CDC * pDC, CRect * rr);
	void RenderConcaveProbe(CDC * pDC, CRect * rr);
	void RenderFocalLaws(CDC * pDC, CRect * rr);
	void WorldToClient(float fX, float fY, CRect * rrClient, CPoint * pt);
	void WorldToClient(D3DXVECTOR3 * vec, CRect * rrClient, CPoint * pt, D3DXMATRIXA16 * pMat = NULL);
	CEdit m_editTxBeamAngle;
	CSpinButtonCtrl m_spinTxBeamAngle;
	CEdit m_editRxBeamAngle;
	CSpinButtonCtrl m_spinRxBeamAngle;
	void RenderDelays(CDC * pDC, CRect * rr);
	CStatic m_staticDelays;
	CEdit m_editTxFocalLength;
	CSpinButtonCtrl m_spinTxFocalLength;
	afx_msg void OnButtonRxEqualTx();
	afx_msg void OnButtonDownloadToHardWare();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	CEdit m_editWedgeVelocity;
	CSpinButtonCtrl m_spinWedgeVelocity;
	CEdit m_editWedgeHeightElementOne;
	CSpinButtonCtrl m_spinWedgeHeightElementOne;
	afx_msg void OnEnChangeEditWedgeVelocity();
	afx_msg void OnDeltaposSpinWedgeVelocity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditWedgeHeightElement1();
	afx_msg void OnDeltaposSpinHeightElement1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_comboProbeType;
	afx_msg void OnCbnSelchangeComboProbeType();
	afx_msg void OnCbnSelchangeComboFiringOrder();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CListBox m_listCoords;
	CEdit m_editVecE0E1;
	afx_msg void OnEnChangeEdit1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHASEDARRAYPROBEDLG_H__44EFEF1C_D506_49EC_BE75_89BAE28D4FF3__INCLUDED_)
