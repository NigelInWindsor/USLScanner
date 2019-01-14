#include "stdafx.h"
#include "CMFCToolBarSpinButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CMFCToolBarSpinButton, CMFCToolBarButton, 1)

static const int nDefaultWidth = 25;
static const int nDefaultHeight = 20;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4355)

CMFCToolBarSpinButton::CMFCToolBarSpinButton() :
	m_wndSpinButton(*this)
{
	m_dwStyle = WS_CHILD | WS_VISIBLE;
		//| TBS_HORZ | TBS_NOTICKS;
	m_iWidth = nDefaultWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
}
//**************************************************************************************
CMFCToolBarSpinButton::CMFCToolBarSpinButton(UINT uiId,
	int iImage,
	DWORD dwStyle,
	int iWidth) :
	CMFCToolBarButton(uiId, iImage),
	m_wndSpinButton(*this)
{
	m_dwStyle = dwStyle | WS_CHILD | WS_VISIBLE;
		//| TBS_HORZ | TBS_NOTICKS;
	m_iWidth = (iWidth == 0) ? nDefaultWidth : iWidth;
	m_nMin = 0;
	m_nMax = 0;
	m_nValue = 0;
}

#pragma warning (default : 4355)

//**************************************************************************************
CMFCToolBarSpinButton::~CMFCToolBarSpinButton()
{
	if (m_wndSpinButton.GetSafeHwnd() != NULL)
	{
		m_wndSpinButton.DestroyWindow();
	}
}
//**************************************************************************************
SIZE CMFCToolBarSpinButton::OnCalculateSize(CDC* /*pDC*/, const CSize& /*sizeDefault*/, BOOL bHorz)
{
	if (!IsVisible())
	{
		if (m_wndSpinButton.GetSafeHwnd() != NULL)
		{
			m_wndSpinButton.ShowWindow(SW_HIDE);
		}

		return CSize(0, 0);
	}

	if (bHorz)
	{
		if (m_wndSpinButton.GetSafeHwnd() != NULL && !IsHidden())
		{
			m_wndSpinButton.ShowWindow(SW_SHOWNOACTIVATE);
			m_wndSpinButton.ModifyStyle(TBS_VERT, TBS_HORZ);
		}

		return CSize(m_iWidth, nDefaultHeight);
	}
	else
	{
		if (m_wndSpinButton.GetSafeHwnd() != NULL && !IsHidden())
		{
			m_wndSpinButton.ShowWindow(SW_SHOWNOACTIVATE);
			m_wndSpinButton.ModifyStyle(TBS_HORZ, TBS_VERT);
		}

		return CSize(nDefaultHeight, m_iWidth);
	}
}
//**************************************************************************************
void CMFCToolBarSpinButton::Serialize(CArchive& ar)
{
	CMFCToolBarButton::Serialize(ar);

	if (ar.IsLoading())
	{
		ar >> m_iWidth;
		ar >> m_dwStyle;

		int		nMin;
		int		nMax;
		int		nValue;

		ar >> nMin;
		ar >> nMax;
		ar >> nValue;

		SetRange(nMin, nMax);
		SetValue(nValue, FALSE);
	}
	else
	{
		ar << m_iWidth;
		ar << m_dwStyle;
		ar << m_nMin;
		ar << m_nMax;
		ar << m_nValue;
	}
}
//**************************************************************************************
void CMFCToolBarSpinButton::OnMove()
{
	if (m_wndSpinButton.GetSafeHwnd() != NULL &&
		(m_wndSpinButton.GetStyle() & WS_VISIBLE))
	{
		m_wndSpinButton.SetWindowPos(NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width() - 2, m_rect.Height() - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CMFCToolBarSpinButton::OnSize(int iSize)
{
	m_iWidth = iSize;
	m_rect.right = m_rect.left + m_iWidth;

	if (m_wndSpinButton.GetSafeHwnd() != NULL &&
		(m_wndSpinButton.GetStyle() & WS_VISIBLE))
	{
		m_wndSpinButton.SetWindowPos(NULL,
			m_rect.left + 1, m_rect.top + 1, m_rect.Width() - 2, m_rect.Height() - 2,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
//**************************************************************************************
void CMFCToolBarSpinButton::OnChangeParentWnd(CWnd* pWndParent)
{
	if (m_wndSpinButton.GetSafeHwnd() != NULL)
	{
		CWnd* pWndParentCurr = m_wndSpinButton.GetParent();
		ASSERT(pWndParentCurr != NULL);

		if (pWndParent != NULL && pWndParentCurr != NULL &&
			pWndParentCurr->GetSafeHwnd() == pWndParent->GetSafeHwnd())
		{
			return;
		}

		m_wndSpinButton.DestroyWindow();
	}

	if (pWndParent == NULL || pWndParent->GetSafeHwnd() == NULL)
	{
		return;
	}

	if (!m_wndSpinButton.Create(m_dwStyle, m_rect, pWndParent, m_nID))
	{
		ASSERT(FALSE);
		return;
	}

	m_wndSpinButton.SetRange(m_nMin, m_nMax);
	m_wndSpinButton.SetPos(m_nValue);
	m_wndSpinButton.SetOwner(pWndParent->GetTopLevelFrame());
}
//**************************************************************************************
void CMFCToolBarSpinButton::CopyFrom(const CMFCToolBarButton& s)
{
	CMFCToolBarButton::CopyFrom(s);

	const CMFCToolBarSpinButton& src = (const CMFCToolBarSpinButton&)s;

	m_dwStyle = src.m_dwStyle;
	m_iWidth = src.m_iWidth;
	m_nMin = src.m_nMin;
	m_nMax = src.m_nMax;
	m_nValue = src.m_nValue;
}
//************************************************************************************
void CMFCToolBarSpinButton::SetRange(int iMin, int iMax)
{
	m_nMin = iMin;
	m_nMax = iMax;

	if (m_wndSpinButton.GetSafeHwnd() != NULL)
	{
		m_wndSpinButton.SetRange(iMin, iMax);
	}

	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons(m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition(); pos != NULL;)
		{
			CMFCToolBarSpinButton* pSpinButton = DYNAMIC_DOWNCAST(CMFCToolBarSpinButton,
				listButtons.GetNext(pos));
			if (pSpinButton != NULL && pSpinButton != this)
			{
				pSpinButton->m_nMin = iMin;
				pSpinButton->m_nMax = iMax;

				if (pSpinButton->m_wndSpinButton.GetSafeHwnd() != NULL)
				{
					pSpinButton->m_wndSpinButton.SetRange(iMin, iMax);
				}
			}
		}
	}
}
//************************************************************************************
void CMFCToolBarSpinButton::SetValue(int iValue, BOOL bNotify)
{
	m_nValue = iValue;
	if (GetHwnd() != NULL)
	{
		m_wndSpinButton.SetPos(iValue);
	}

	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons(m_nID, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition(); pos != NULL;)
		{
			CMFCToolBarSpinButton* pSpinButton = DYNAMIC_DOWNCAST(CMFCToolBarSpinButton,
				listButtons.GetNext(pos));
			if (pSpinButton != NULL && pSpinButton != this)
			{
				pSpinButton->m_nValue = iValue;

				if (pSpinButton->GetHwnd() != NULL)
				{
					pSpinButton->m_wndSpinButton.SetPos(iValue);
				}
			}
		}
	}

	if (bNotify && m_wndSpinButton.GetSafeHwnd() != NULL)
	{
		CWnd* pOwner = m_wndSpinButton.GetOwner();
		if (pOwner != NULL) {
			pOwner->SendMessage(WM_COMMAND, m_nID);
		}
	}
}
int CMFCToolBarSpinButton::SetRange(UINT uiCmd, int iMin, int iMax)
{
	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons(uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition(); pos != NULL;)
		{
			CMFCToolBarSpinButton* pSpinButton = DYNAMIC_DOWNCAST(CMFCToolBarSpinButton,
				listButtons.GetNext(pos));
			if (pSpinButton != NULL && pSpinButton->GetHwnd() != NULL)
			{
				pSpinButton->m_wndSpinButton.SetRange(iMin, iMax);
			}
		}
	}

	return -1;
}
int CMFCToolBarSpinButton::SetPos(UINT uiCmd, int iValue)
{
	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons(uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition(); pos != NULL;)
		{
			CMFCToolBarSpinButton* pSpinButton = DYNAMIC_DOWNCAST(CMFCToolBarSpinButton,
				listButtons.GetNext(pos));
			if (pSpinButton != NULL && pSpinButton->GetHwnd() != NULL)
			{
				pSpinButton->m_wndSpinButton.SetPos(iValue);
			}
		}
	}
	return -1;
}
//************************************************************************************
int CMFCToolBarSpinButton::GetPos(UINT uiCmd)
{
	CObList listButtons;
	if (CMFCToolBar::GetCommandButtons(uiCmd, listButtons) > 0)
	{
		for (POSITION pos = listButtons.GetHeadPosition(); pos != NULL;)
		{
			CMFCToolBarSpinButton* pSpinButton = DYNAMIC_DOWNCAST(CMFCToolBarSpinButton,
				listButtons.GetNext(pos));
			if (pSpinButton != NULL && pSpinButton->GetHwnd() != NULL)
			{
				return pSpinButton->m_wndSpinButton.GetPos();
			}
		}
	}

	return -1;
}
//****************************************************************************************
void CMFCToolBarSpinButton::OnShow(BOOL bShow)
{
	if (m_wndSpinButton.GetSafeHwnd() != NULL)
	{
		if (bShow)
		{
			m_wndSpinButton.ShowWindow(SW_SHOWNOACTIVATE);
			OnMove();
		}
		else
		{
			m_wndSpinButton.ShowWindow(SW_HIDE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomSpinButtonCtrl

CCustomSpinButtonCtrl::CCustomSpinButtonCtrl(CMFCToolBarSpinButton& btnSpinButton) :
	m_btnSpinButton(btnSpinButton)
{
}

CCustomSpinButtonCtrl::~CCustomSpinButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomSpinButtonCtrl, CSpinButtonCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHWHEEL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomSpinButtonCtrl message handlers

void CCustomSpinButtonCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nPos = GetPos();

	CSpinButtonCtrl::OnMouseMove(nFlags, point);
	
	SetFocus();

	if ((nFlags & MK_LBUTTON) && GetPos() != nPos)
	{
		m_btnSpinButton.SetValue(GetPos(), TRUE);
	}
}

void CCustomSpinButtonCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nPos = GetPos();

	CSpinButtonCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (GetPos() != nPos)
	{
		m_btnSpinButton.SetValue(GetPos(), TRUE);
	}
}

void CCustomSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
	if (IsWindowVisible())
	{
		m_btnSpinButton.SetValue(GetPos(), TRUE);
	}
}

BOOL CCustomSpinButtonCtrl::OnMouseWheel(UINT nFlags, short zdelta, CPoint point)
{
//	int nPos = GetPos();

	return CSpinButtonCtrl::OnMouseWheel(nFlags, zdelta, point);

	//if ((nFlags & MK_LBUTTON) && GetPos() != nPos)
	//{
	//	m_btnSpinButton.SetValue(GetPos(), TRUE);
	//}
}

void CCustomSpinButtonCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	
	
	
	CSpinButtonCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}