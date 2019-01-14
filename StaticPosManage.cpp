// StaticPosManage.cpp: implementation of the CStaticPosManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "RRBarnoldswick.h"
#include "StaticPosManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticPosManage::CStaticPosManage()
{
	m_DialogCreated = FALSE;

}

CStaticPosManage::~CStaticPosManage()
{

}


bool CStaticPosManage::PointIsInRect(CPoint point)
{
	//m_Ptr->ScreenToClient(point);
	return m_Rect.PtInRect(point)  & 0x01 ;
}

void CStaticPosManage::Initialize(CWnd* Ptr, CWnd &cwnd,bool top, bool right, bool bottom, bool left)
{
 m_Ptr = Ptr;
 
 CRect WindowsRect; 
 m_Ptr->GetWindowRect(WindowsRect);
 m_Ptr->ScreenToClient(WindowsRect);

 m_CWnd = &cwnd;
 m_CWnd->CWnd::GetWindowRect(m_Rect);
 m_Ptr->CWnd::ScreenToClient(m_Rect);

 m_Top    = top;
 m_Left   = left;
 m_Bottom = bottom;
 m_Right  = right;

 m_RectGapSameSide.top        = m_Rect.top         - WindowsRect.top;
 m_RectGapSameSide.left       = m_Rect.left        - WindowsRect.left;
 m_RectGapSameSide.right      = WindowsRect.right  - m_Rect.right;
 m_RectGapSameSide.top        = m_Rect.top         - WindowsRect.top;
 m_RectGapSameSide.bottom     = WindowsRect.bottom - m_Rect.bottom;

 m_RectGapOppositeSide.top    = WindowsRect.bottom - m_Rect.top; 
 m_RectGapOppositeSide.right  = m_Rect.right       - WindowsRect.left;
 m_RectGapOppositeSide.bottom = m_Rect.bottom      - WindowsRect.top ;
 m_RectGapOppositeSide.left   = WindowsRect.right  - m_Rect.left;
 
 m_DialogCreated = TRUE;
}

void CStaticPosManage::SetNewPos(bool bAction)
{
if (m_DialogCreated)
{
 CRect WindowsRect; 
 m_Ptr->GetWindowRect(WindowsRect);
 m_Ptr->ScreenToClient(WindowsRect);

 if (m_Left == TRUE)  
 m_Rect.left = m_RectGapSameSide.left + WindowsRect.left;
 else 
 m_Rect.left =  WindowsRect.right - m_RectGapOppositeSide.left;

 if (m_Right == TRUE)
 m_Rect.right = WindowsRect.right -m_RectGapSameSide.right;
 else
 m_Rect.right  = m_RectGapOppositeSide.right  +  WindowsRect.left;
 
 if (m_Top == TRUE)
 m_Rect.top = WindowsRect.top + m_RectGapSameSide.top;
 else 
 m_Rect.top =  WindowsRect.bottom - m_RectGapOppositeSide.top;
 
 if (m_Bottom == TRUE)
 m_Rect.bottom  =  WindowsRect.bottom -m_RectGapSameSide.bottom;
 else 
 m_Rect.bottom = m_RectGapOppositeSide.bottom + WindowsRect.top ;

 //CRect ObjectRect;
 //ObjectRect = m_Rect;
 //m_Ptr->ClientToScreen(ObjectRect);
 if(m_CWnd->IsWindowVisible() || (bAction==TRUE)) {
	 m_CWnd->SetWindowPos(NULL, m_Rect.left,m_Rect.top, m_Rect.Width(), m_Rect.Height(),SWP_SHOWWINDOW);
 } else {
	 m_CWnd->SetWindowPos(NULL, m_Rect.left,m_Rect.top, m_Rect.Width(), m_Rect.Height(),SWP_HIDEWINDOW|SWP_NOREDRAW);
 }
}
}

CRect CStaticPosManage::CalculateWindowsSize(CRect ObjectSize)
{
 CRect WindowsRect;
 if (m_Left == TRUE)  
 WindowsRect.left = ObjectSize.left - m_RectGapSameSide.left ;
 else 
 WindowsRect.right = ObjectSize.left +m_RectGapOppositeSide.left  ; 

 if (m_Right == TRUE)
 WindowsRect.right = ObjectSize.right +m_RectGapSameSide.right;
 else
 WindowsRect.left = ObjectSize.right - m_RectGapOppositeSide.right ;
 
 if (m_Top == TRUE)
 WindowsRect.top  = ObjectSize.top - m_RectGapSameSide.top ;
 else 
 WindowsRect.bottom  = ObjectSize.top + m_RectGapOppositeSide.top;   
 if (m_Bottom == TRUE)
 WindowsRect.bottom = ObjectSize.bottom  + m_RectGapSameSide.bottom ;  
 else 
 WindowsRect.top = ObjectSize.bottom - m_RectGapOppositeSide.bottom    ;

 return WindowsRect;

}

CRect CStaticPosManage::CalculateObjectSize(CRect WindowsRect)
{
CRect RectObject;
if (m_Left == TRUE)  
 RectObject.left = m_RectGapSameSide.left + WindowsRect.left;
 else 
 RectObject.left =  WindowsRect.right - m_RectGapOppositeSide.left;

 if (m_Right == TRUE)
 RectObject.right = WindowsRect.right -m_RectGapSameSide.right;
 else
 RectObject.right  = m_RectGapOppositeSide.right  +  WindowsRect.left;
 
 if (m_Top == TRUE)
 RectObject.top = WindowsRect.top + m_RectGapSameSide.top;
 else 
 RectObject.top =  WindowsRect.bottom - m_RectGapOppositeSide.top;
 
 if (m_Bottom == TRUE)
 RectObject.bottom  =  WindowsRect.bottom -m_RectGapSameSide.bottom;
 else 
 RectObject.bottom = m_RectGapOppositeSide.bottom + WindowsRect.top ;

 return RectObject;
}



void CStaticPosManage::ChangePos()
{
	if (m_DialogCreated)
	{
	 CRect WindowsRect; 
	 m_Ptr->GetWindowRect(WindowsRect);
	 m_Ptr->ScreenToClient(WindowsRect);

	 m_RectGapSameSide.top        = m_Rect.top         - WindowsRect.top;
	 m_RectGapSameSide.left       = m_Rect.left        - WindowsRect.left;
	 m_RectGapSameSide.right      = WindowsRect.right  - m_Rect.right;
	 m_RectGapSameSide.top        = m_Rect.top         - WindowsRect.top;
	 m_RectGapSameSide.bottom     = WindowsRect.bottom - m_Rect.bottom;

	 m_RectGapOppositeSide.top    = WindowsRect.bottom - m_Rect.top; 
	 m_RectGapOppositeSide.right  = m_Rect.right       - WindowsRect.left;
	 m_RectGapOppositeSide.bottom = m_Rect.bottom      - WindowsRect.top ;
	 m_RectGapOppositeSide.left   = WindowsRect.right  - m_Rect.left;
 
	 m_CWnd->SetWindowPos(NULL, m_Rect.left,m_Rect.top, m_Rect.Width(), m_Rect.Height(),SWP_SHOWWINDOW);

	}
}




void CStaticPosManage::setLeftPos(int nPos)
{
	CRect WindowsRect; 
	m_Ptr->GetWindowRect(WindowsRect);
	m_Ptr->ScreenToClient(WindowsRect);

	m_RectGapSameSide.left       = nPos;
	m_RectGapOppositeSide.left   = WindowsRect.right + m_RectGapSameSide.left;

}

void CStaticPosManage::setRightPos(int nPos)
{
	CRect WindowsRect; 
	m_Ptr->GetWindowRect(WindowsRect);
	m_Ptr->ScreenToClient(WindowsRect);

	m_RectGapSameSide.right      = nPos;
	m_RectGapOppositeSide.right  = m_RectGapSameSide.right + WindowsRect.left;
}

int CStaticPosManage::getLeftPos()
{

	return m_RectGapSameSide.left;
}

int CStaticPosManage::getRightPos()
{
	return m_RectGapSameSide.right;
}
