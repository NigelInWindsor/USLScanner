// USLPropertyGridProperty.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "USLPropertyGridProperty.h"


// CUSLPropertyGridProperty

IMPLEMENT_DYNAMIC(CUSLPropertyGridProperty, CMFCPropertyGridProperty)
#define	ID_PROPERTY_SPIN	100

CUSLPropertyGridProperty::CUSLPropertyGridProperty(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	m_pComboBox = NULL;
//	m_pSpinButtonCtrl = NULL;
}

// Simple property
CUSLPropertyGridProperty::CUSLPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData,
	LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars,CWnd* pParentWnd)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData,
	lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	m_pComboBox = NULL;
//	m_pSpinButtonCtrl = NULL;
	m_fMin = 0.0f;
	m_fMax = 0.0f;
	m_nPrecision = 0;
	m_bFloatSpinCtrl = false;
	m_pParentWnd = pParentWnd;
}


CUSLPropertyGridProperty::~CUSLPropertyGridProperty()
{
}


//BEGIN_MESSAGE_MAP(CUSLPropertyGridProperty, CMFCPropertyGridProperty)
//ON_WM_VSCROLL()
//END_MESSAGE_MAP()


CComboBox* CUSLPropertyGridProperty::CreateCombo(CWnd* pWndParent, CRect rect){

	return m_pComboBox = CMFCPropertyGridProperty::CreateCombo(pWndParent, rect);
}

// CUSLPropertyGridProperty message handlers




int CUSLPropertyGridProperty::GetCurSel()
{
	if (m_pComboBox != NULL && m_pComboBox->GetSafeHwnd()) {
		return m_pComboBox->GetCurSel();
	}
	return 0;
}


void CUSLPropertyGridProperty::SetCurSel(int nIndex)
{
	if ((m_pComboBox != NULL) && (m_pComboBox->GetSafeHwnd())) {
		m_pComboBox->SetCurSel(nIndex);
	}

}



CString CUSLPropertyGridProperty::FormatProperty()
{
	CString str;

	if (m_bFloatSpinCtrl == true)
	{
		str.Format(CMFCPropertyGridProperty::m_strEditMask, m_fValue);
	}
	else
	{
		str = CMFCPropertyGridProperty::FormatProperty();
	}

	return str;
}


void CUSLPropertyGridProperty::OnDrawValue(CPaintDC* pDC, CRect rect)
{
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
}


void CUSLPropertyGridProperty::ReDraw()
{
	CMFCPropertyGridProperty::Redraw();
}


BOOL CUSLPropertyGridProperty::OnUpdateValue()
{
	CString Buff;
	float fDivision,fTemp;


	if (m_bFloatSpinCtrl == true) {
		fDivision = pow(10.0f, m_nPrecision);

		CSpinButtonCtrl *pCtrl = CMFCPropertyGridProperty::m_pWndSpin;
		CWnd* pWnd = CMFCPropertyGridProperty::m_pWndInPlace;

		pWnd->GetWindowText(Buff);
		_WTOF(Buff, fTemp);

		if (pCtrl->GetPos32() - m_nLastPos) {
			fTemp /= fDivision;
			Buff.Format(CMFCPropertyGridProperty::m_strEditMask, m_fValue = fTemp);

			fTemp += ((float)(pCtrl->GetPos32() - m_nLastPos) / fDivision);
			m_nLastPos = pCtrl->GetPos32();
		}
		else {
			if (m_nLastPos - (int)(fTemp / fDivision)) {
				Buff.Format(CMFCPropertyGridProperty::m_strEditMask, m_fValue = fTemp);
			}
		}


		pWnd->SetWindowText(Buff);
		CDockablePane* pPane = (CDockablePane*)m_pParentWnd;
		pPane->SendMessage(AFX_WM_PROPERTY_CHANGED, 0, (LPARAM)this);
	}

	BOOL bFlag = CMFCPropertyGridProperty::OnUpdateValue();

	return bFlag;
}

void CUSLPropertyGridProperty::EnableSpinControl(bool bEnable, float fValue, float fMin, float fMax, int nPrecision)
{
	m_fValue = fValue;
	m_fMin = fMin;
	m_fMax = fMax;
	m_nPrecision = nPrecision;
	m_bFloatSpinCtrl = true;

	int nMin = (int)(fMin * pow(10.0f, nPrecision));
	int nMax = (int)(fMax * pow(10.0f, nPrecision));
	long nValue = (long)(m_fValue * pow(10.0f, nPrecision));

	CMFCPropertyGridProperty::EnableSpinControl(bEnable, nMin, nMax);
}

void CUSLPropertyGridProperty::UpdateSpinControl(float fValue, int nPrecision, LPCTSTR lpszEditMask)
{
	m_nPrecision = nPrecision;

	int nMin = (int)(m_fMin * pow(10.0f, nPrecision));
	int nMax = (int)(m_fMax * pow(10.0f, nPrecision));
	m_fValue = fValue;
	if(lpszEditMask != NULL)
		CMFCPropertyGridProperty::m_strEditMask.Format(L"%s", lpszEditMask);

	CMFCPropertyGridProperty::EnableSpinControl(true, nMin, nMax);
	CMFCPropertyGridProperty::Redraw();
}


CSpinButtonCtrl* CUSLPropertyGridProperty::CreateSpinControl(CRect rectSpin)
{
	CString Buff;
	CSpinButtonCtrl* pSpinCtrl = CMFCPropertyGridProperty::CreateSpinControl(rectSpin);

	int nValue = (int)(m_fValue * pow(10.0f, m_nPrecision));
	int nMin = (int)(m_fMin * pow(10.0f, m_nPrecision));
	int nMax = (int)(m_fMax * pow(10.0f, m_nPrecision));

	pSpinCtrl->SetRange32(nMin,nMax);
	pSpinCtrl->SetPos32(m_nLastPos = nValue);

	Buff.Format(CMFCPropertyGridProperty::m_strEditMask, m_fValue);

	CWnd* pWnd = CMFCPropertyGridProperty::m_pWndInPlace;
	pWnd->SetWindowText(Buff);

	return pSpinCtrl;
}



BOOL CUSLPropertyGridProperty::PushChar(UINT nChar) 
{
	return	CMFCPropertyGridProperty::PushChar(nChar);
}


float CUSLPropertyGridProperty::GetFloatSpinValue()
{
	return m_fValue;
}

BOOL CUSLPropertyGridProperty::OnEdit(LPPOINT lptClick)
{
	CString Buff;
	CWnd* pWnd = CMFCPropertyGridProperty::m_pWndInPlace;

	if (pWnd->GetSafeHwnd()) {
		pWnd->GetWindowText(Buff);
		_WTOF(Buff, m_fValue);

		CSpinButtonCtrl *pCtrl = CMFCPropertyGridProperty::m_pWndSpin;
		if (pCtrl->GetSafeHwnd()) {
			int nValue = (int)(m_fValue * pow(10.0f, m_nPrecision));
			pCtrl->SetPos32(m_nLastPos = nValue);
		}
	}

	return CMFCPropertyGridProperty::OnEdit(lptClick);
}
