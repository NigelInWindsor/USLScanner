// StaticPosManage.h: interface for the CStaticPosManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICPOSMANAGE_H__6CF57DA9_2D6D_11D4_A108_0050DAE2EB3D__INCLUDED_)
#define AFX_STATICPOSMANAGE_H__6CF57DA9_2D6D_11D4_A108_0050DAE2EB3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStaticPosManage  
{
public:
	int getRightPos();
	int getLeftPos();
	void setRightPos(int nPos);
	void setLeftPos(int nPos);
	CStaticPosManage();
	virtual ~CStaticPosManage();
	CRect m_Rect;
	void ChangePos();
	void Initialize(CWnd* Ptr, CWnd &Static,bool top, bool right, bool bottom, bool left);
	void SetNewPos(bool bAction=TRUE);
	bool PointIsInRect(CPoint point);
	
	CRect CalculateWindowsSize(CRect ObjectSize);
	CRect CalculateObjectSize(CRect WindowsRect);
private:
	CRect m_RectGapSameSide;
	CRect m_RectGapOppositeSide;
	bool m_Top, m_Right, m_Bottom, m_Left;
	CWnd *m_Ptr;
	CWnd* m_CWnd;
	bool m_DialogCreated ;
	
};



#endif // !defined(AFX_STATICPOSMANAGE_H__6CF57DA9_2D6D_11D4_A108_0050DAE2EB3D__INCLUDED_)
