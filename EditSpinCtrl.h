#pragma once


class CEditSpinCtrl : public CSpinButtonCtrl
{
	// Construction
public:
	CEditSpinCtrl();
	virtual ~CEditSpinCtrl();

	// Operations
public:
	void GetRangeAndDelta(float &fLower, float& fUpper, float& fDelta);
	void SetRangeAndDelta(float fLower, float fUpper, float fDelta);

	float GetPos();
	void   SetPos(float fVal);

	float GetValue();

	void SetFormatString(LPCTSTR lpszFormatString = NULL);
	BOOL SetReadOnly(BOOL flag);
	void Init(float fVariable, float fMin, float fMax, float fDelta, float fScreenDiv, LPCTSTR lpszFormatString);


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumSpinCtrl)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitSpinCtrl();
	void SetIntPos(float fPos);
	void SetValueForBuddy(float fVal);
	CString FormatValue(float fVal);

	// Generated message map functions
protected:
	//{{AFX_MSG(CNumSpinCtrl)
	afx_msg BOOL OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	/// Attributes
protected:
	bool	m_bRdOnly;
	float	m_fMinVal;
	float	m_fMaxVal;
	float	m_fDelta;
	float	m_fScreenDiv;
	UINT	m_IntRange;
	CString m_strFormat;
};

