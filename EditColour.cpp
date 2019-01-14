// EditColour.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "EditColour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditColour

CEditColour::CEditColour()
{
	m_rgbBkColour = ::GetSysColor(COLOR_3DFACE);
	m_rgbTextColour = RGB(0,0,0);
	m_brushBkgnd.CreateSolidBrush(m_rgbBkColour);

}

CEditColour::~CEditColour()
{
}


BEGIN_MESSAGE_MAP(CEditColour, CEdit)
	//{{AFX_MSG_MAP(CEditColour)
		ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditColour message handlers

void CEditColour::SetTextColour(COLORREF rgbColour)
{
	m_rgbTextColour = rgbColour;
	RedrawWindow();
}

void CEditColour::SetTextColour(CString Buff, COLORREF rgbColour)
{
	m_rgbTextColour = rgbColour;
	SetWindowText(Buff);
}

void CEditColour::SetBkColor(COLORREF rgbColor)
{
	m_rgbBkColour = rgbColor;
	m_brushBkgnd.DeleteObject();
	m_brushBkgnd.CreateSolidBrush(rgbColor);
	RedrawWindow();
}

HBRUSH CEditColour::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;
	hbr = (HBRUSH)m_brushBkgnd; // Passing a Handle to the Brush
	pDC->SetBkColor(m_rgbBkColour); // Setting the Color of the Text Background to the one passed by the Dialog
	pDC->SetTextColor(m_rgbTextColour); // Setting the Text Color to the one Passed by the Dialog

	if (nCtlColor)       // To get rid of compiler warning
      nCtlColor += 0;

	return hbr;
}


BOOL CEditColour::SetReadOnly(BOOL flag)
{
   if (flag == TRUE)
      SetBkColor(m_rgbBkColour);
   else
      SetBkColor(RGB(255,255,255));

   return CEdit::SetReadOnly(flag);

}

void CEditColour::EnableSpinCtrl()
{
	CRect rrEdit;
	GetWindowRect(rrEdit);
//	ScreenToClient(rrEdit);
	rrEdit.left = rrEdit.right - 8;
	int nID = GetDlgCtrlID();


	m_spinEdit.Create(UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS, rrEdit, this, nID);
//	m_spinEdit.SetWindowPos(m_spinEdit.GetBuddy(), rrEdit.Width() - rrEdit.Height(), 0, rrEdit.Height(), rrEdit.Height(), NULL);

	m_spinEdit.EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);

}