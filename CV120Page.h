#pragma once
#ifdef NPTRACKINGTOOLS
#include "afxwin.h"
#include "CV120Trio.h"
#include "CRigidBody.h"
#include "afxcmn.h"
#include "MovingAverage.h"

// CV120Page dialog

class CV120Page : public CPropertyPage
{
	DECLARE_DYNAMIC(CV120Page)

public:
	CV120Page();
	virtual ~CV120Page();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CV120Page };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		virtual BOOL OnInitDialog();
	void UpdateEditRate();
	void UpdateEditMovingAverage();
	void UpdateEditPrintModulo();
	void OnClickedInitButton();
	afx_msg void OnClickedShutdownButton();
	void Update();
	void UpdateCoords();
	void UpdateStats();
	CEdit m_editI;
	CEdit m_editJ;
	CEdit m_editK;
	CEdit m_editNMarks;
	CEdit m_editNRgds;
	CEdit m_editX;
	CEdit m_editY;
	CEdit m_editZ;
private:
	CV120Trio * m_pV120Trio;
	int m_nRate;
	float m_nDeltaTms;
	int m_nMinRate;
	int m_nMaxRate;
	bool m_bUpdating;
	
public:
	CButton m_buttonInit;
	CButton m_buttonShutdown;
	CButton m_buttonUpdate;
	afx_msg void OnClickedButtonUpdate();
	CEdit m_editNCams;
	CRigidBody m_rigidBody;
	CEdit m_editQW;
	CEdit m_editQX;
	CEdit m_editQY;
	CEdit m_editQZ;
	CButton m_buttonStop;
	afx_msg void OnClickedButtonStop();
	CComboBox m_comboMode;
	CEdit m_editRate;
	CSpinButtonCtrl m_spinRate;
	afx_msg void OnDeltaposSpinRate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditRate();
	afx_msg
	void SetRate();
	void SetRate(int val);
	void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
//	CButton m_buttonZeroOrient;
	CButton m_buttonZeroOrient;
	afx_msg void OnClickedButtonZeroOrientOt();
	CButton m_buttonZeroPos;
	afx_msg void OnClickedButtonZeroPosOt();
	CButton m_buttonRefineRB;
	afx_msg void OnClickedButtonRefineRbOt();
	CEdit m_editErrX;
	CEdit m_editErrY;
	CEdit m_editErrZ;
	CEdit m_editErrK;
	CEdit m_editErrJ;
	CEdit m_editErrI;
	CComboBox m_comboErr;

	enum ERR_TYPE { STDEV, STDERR };
	void SetErrorType(ERR_TYPE val) { m_errType = val; }

private:
	ERR_TYPE m_errType = STDERR;
	int m_nPrintModulo = 1;
	int m_nCurrent = 0;
public:
	afx_msg void OnChangeEditMovingaverage();
//	CEdit m_editMeanJ;
	CEdit m_editMeanI;
	CEdit m_editMeanJ;
	CEdit m_editMeanK;
	CEdit m_editMovingAverage;
	CEdit m_editMeanX;
	CEdit m_editMeanY;
	CEdit m_editMeanZ;
	CSpinButtonCtrl m_spinMovingAverage;
	CEdit m_editStdJ;
	CEdit m_editStdI;
	CEdit m_editStdK;
	CEdit m_editStdX;
	CEdit m_editStdY;
	CEdit m_editStdZ;
	CEdit m_editPrintModulo;
	CSpinButtonCtrl m_spinPrintModulo;
	afx_msg void OnChangeEditPrintmodulo();
	afx_msg void OnDeltaposSpinMovingaverage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPrintmodulo(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_editDist;
	CEdit m_editErrDist;
	CEdit m_editMeanDist;
	CEdit m_editStdDist;
	CComboBox m_comboRigidBody;
	afx_msg void OnSelchangeComboRigidbody();
};
#endif //#ifdef NPTRACKINGTOOLS