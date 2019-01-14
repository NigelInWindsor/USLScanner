#pragma once
#include "afxpropertygridctrl.h"
// CUSLPropertyGridProperty

class CUSLPropertyGridProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CUSLPropertyGridProperty)


public:
	CWnd* m_pParentWnd;
	CComboBox* m_pComboBox;
	CSpinButtonCtrl* m_pSpinButtonCtrl;
	CUSLPropertyGridProperty(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);

	// Simple property
	CUSLPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL, CWnd* pParentWnd = NULL);

	virtual ~CUSLPropertyGridProperty();
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);

protected:
//	DECLARE_MESSAGE_MAP()

public:
	float m_fValue;
	float m_fMin;
	float m_fMax;
	int m_nPrecision;
	int	m_nLastPos;
	bool m_bFloatSpinCtrl;

	int GetCurSel();
	void SetCurSel(int nIndex);
	void EnableSpinControl(bool bEnable, float fValue, float fMin, float fMax, int nPrecision);
	void UpdateSpinControl(float fValue, int nPrecision, LPCTSTR lpszEditMask = NULL);
	virtual CString FormatProperty();
	virtual void OnDrawValue(CPaintDC* pDC, CRect rect);
	void ReDraw();
	virtual BOOL OnUpdateValue();

	virtual CSpinButtonCtrl* CreateSpinControl(CRect rectSpin);
	virtual BOOL PushChar(UINT nChar);
	virtual BOOL OnEdit(LPPOINT lptClick);

	float GetFloatSpinValue();
};


