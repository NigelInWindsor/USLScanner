#include "stdafx.h"
#include "EditSpinCtrl.h"


#define EPS 1.0e-15f

/////////////////////////////////////////////////////////////////////////////
// CEditSpinCtrl

CEditSpinCtrl::CEditSpinCtrl()
{
	m_fDelta = 1.0f;
	m_fMinVal = 0.0f;
	m_fMaxVal = 100.0f;
	m_IntRange = 100;
	m_fScreenDiv = 1.0f;
	m_bRdOnly = false;

	lconv* pLconv = localeconv();
	SetFormatString(NULL);
}

CEditSpinCtrl::~CEditSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CEditSpinCtrl)
	ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltapos)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSpinCtrl message handlers

void CEditSpinCtrl::Init(float fVariable, float fMin, float fMax, float fDelta, float fScreenDiv, LPCTSTR lpszFormatString)
{
	SetRangeAndDelta(fMin, fMax, fDelta);
	m_fScreenDiv = fScreenDiv;
	SetFormatString(lpszFormatString);
	SetPos(fVariable);
}

void CEditSpinCtrl::SetPos(float val)
{
	SetValueForBuddy(val);
	SetIntPos(val);
}

float CEditSpinCtrl::GetValue()
{
	return GetPos();
}

float CEditSpinCtrl::GetPos()
{
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		CString str;
		pEdit->GetWindowText(str);
		float fVal;

		WCHAR *STOPstring;
		fVal = (float)wcstod(str, &STOPstring) * m_fScreenDiv;

		return fVal;
	}
	else
	{
		//ASSERT (FALSE); // you didn't set buddy
		return 0.0;
	}
}

void CEditSpinCtrl::SetValueForBuddy(float fVal)
{
	CWnd* pEdit = GetBuddy();
	if (pEdit)
	{
		CString str;
		if (m_strFormat.IsEmpty())
			str.Format(L"%f",fVal);
		else
			str.Format(m_strFormat, fVal / m_fScreenDiv);

		pEdit->SetWindowText(str);
	}
}


void CEditSpinCtrl::SetRangeAndDelta(float fMin, float fMax, float fDelta)
{
	m_fMinVal = fMin;
	m_fMaxVal = fMax;
	m_fDelta = fDelta;

//	ASSERT(m_fMaxVal > m_fMinVal); // reversed min/max is not implemented, although it's probably easy

								 //// avoid division by zero
	if (m_fDelta == 0.0)
		return;

	/// Figure out the integer range to use, so that acceleration can work properly
	float range = fabs((m_fMaxVal - m_fMinVal) / m_fDelta);
	if (range > (float)UD_MAXVAL)
		m_IntRange = UD_MAXVAL;
	else
		m_IntRange = (int)range;
	CSpinButtonCtrl::SetRange32(0, m_IntRange);

	/// Set integer position
	SetIntPos(GetPos());
}

void CEditSpinCtrl::SetIntPos(float pos)
{
	//// avoid division by zero
	if (m_fMaxVal == m_fMinVal)
		return;

	int int_pos;

	if (pos < m_fMinVal)
		int_pos = 0;
	else if (pos > m_fMaxVal)
		int_pos = m_IntRange;
	else
	{
		// where in the range is current position?
		float pos_in_range = (pos - m_fMinVal) / (m_fMaxVal - m_fMinVal);
		int_pos = (int)(m_IntRange * pos_in_range + 0.5);
	}
	CSpinButtonCtrl::SetPos(int_pos);
}

void CEditSpinCtrl::GetRangeAndDelta(float& fMin, float& fMax, float& fDelta)
{
	fMin = m_fMinVal;
	fMax = m_fMaxVal;
	fDelta = m_fDelta;
}

BOOL CEditSpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pUD = (NM_UPDOWN*)pNMHDR;

	if (m_bRdOnly == false) {

		float fVal = GetPos() + m_fDelta * pUD->iDelta;

		const bool can_wrap = (UDS_WRAP & GetStyle());


		if (pUD->iDelta < 0) // spin down
		{
			float abs_eps = fabsf(EPS * max(fVal, m_fMinVal));
			if (abs_eps < EPS) abs_eps = EPS;

			if (m_fMinVal - fVal > abs_eps) {  //if (val < m_fMinVal){		
				if (can_wrap) {
					fVal = m_fMaxVal;
				}
				else {
					fVal = m_fMinVal;
				}
			}
		}
		else  // spin up
		{
			float abs_eps = fabsf(EPS * max(fVal, m_fMaxVal));
			if (abs_eps < EPS) abs_eps = EPS;

			if (fVal - m_fMaxVal > abs_eps) {   //if (val > m_fMaxVal){		
				if (can_wrap) {
					fVal = m_fMinVal;
				}
				else {
					fVal = m_fMaxVal;
				}
			}
		}

		SetValueForBuddy(fVal);
	}

	*pResult = 0;

	return FALSE; // let parent process this notification too.
}

int CEditSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitSpinCtrl();

	return 0;
}

void CEditSpinCtrl::PreSubclassWindow()
{
	CSpinButtonCtrl::PreSubclassWindow();
	InitSpinCtrl();
}

void CEditSpinCtrl::InitSpinCtrl()
{
	ASSERT((GetStyle() & UDS_SETBUDDYINT) != UDS_SETBUDDYINT); // Otherwise control won't work properly!
															   //ModifyStyle (UDS_SETBUDDYINT, UDS_ARROWKEYS);
	SetRangeAndDelta(m_fMinVal, m_fMaxVal, m_fDelta); // set default values
}



void CEditSpinCtrl::SetFormatString(LPCTSTR lpszFormatString /*= NULL*/)
{
	if (lpszFormatString == NULL)
		m_strFormat.Empty();
	else
		m_strFormat = lpszFormatString;
}

BOOL CEditSpinCtrl::SetReadOnly(BOOL flag)
{
	CEdit* pEdit = (CEdit*)GetBuddy();
	if (pEdit) {
		return pEdit->SetReadOnly(m_bRdOnly = flag);
	}
	return false;
}
