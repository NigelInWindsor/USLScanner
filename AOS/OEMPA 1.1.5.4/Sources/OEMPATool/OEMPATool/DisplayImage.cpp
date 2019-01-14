#include "stdafx.h"
#include "DisplayImage.h"
//#include "ChildView.h"

bool g_bColorDefault=true;
BYTE g_byColorRed[256];
BYTE g_byColorGreen[256];
BYTE g_byColorBlue[256];
BYTE g_byColorRedU[256];
BYTE g_byColorGreenU[256];
BYTE g_byColorBlueU[256];

#ifdef _MINMAX_BUFFER_
bool g_bCriticalAscan2=false;
CRITICAL_SECTION g_CriticalAscan2;
#endif _MINMAX_BUFFER_

long round1(double x)
{
	long nx = (long)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}

/////////////////////////////////////////////////////////////////////////////////

CParameterView::CParameterView(CDisplayImage *pImage) :
	m_dataImageHeight(0), m_dataImageWidth(0)
{
	m_scaleWidth = 0;
	m_scaleHeight = 0;
	m_dataImageWidth = 0;
	m_dataImageHeight = 0;
	m_clientRectWidth = 0;
	m_clientRectHeight = 0;
	m_bUnsigned = true;
	m_eDisplayType = eXY;
	m_bImage = false;
	m_bInvert = false;
	m_pImageView = NULL;
	if(pImage)
		m_pDisplayImage = pImage;
}

CDisplayImage::CDisplayImage(CChildView *pImageView) :
#pragma warning( disable : 4355 )
	CParameterView(this),
#pragma warning( default : 4355 )
	m_pSrcWnd(NULL), m_hDestDC(NULL)
{
#ifdef _MINMAX_BUFFER_
	if(!g_bCriticalAscan2)
	{
		InitializeCriticalSection(&g_CriticalAscan2);
		g_bCriticalAscan2 = true;
	}
	EnterCriticalSection(&g_CriticalAscan2);
	m_pImageAscanMin = NULL;
	LeaveCriticalSection(&g_CriticalAscan2);
	m_pPointMin = NULL;
	m_bBufferMin = false;
	m_bBufferMax = false;
#endif _MINMAX_BUFFER_

	m_pPoint = NULL;
	m_iPointCount = 0;
	m_pImageView = pImageView;
}

CDisplayImage::~CDisplayImage()
{
	Destroy();
}

bool CDisplayImage::Create(CWnd *pWnd,int iImageWidth,int iImageHeight,int iScaleWidth,int iScaleHeight,bool bSigned,bool bImage,bool bInvert)
{
	// no palettes
    CRect clientRect;
    pWnd->GetClientRect(clientRect);
	int height = clientRect.Height();
    int width = clientRect.Width();

	if (!height || !width)
		return false;

    Destroy();

	m_scaleWidth = iScaleWidth;
	m_scaleHeight = iScaleHeight;
	m_clientRectHeight = height;
    m_clientRectWidth = width;
	if(m_clientRectHeight>iImageHeight)
		m_stretchHeight = iImageHeight;
	else
		m_stretchHeight = m_clientRectHeight;
	if(m_clientRectWidth>iImageWidth)
		m_stretchWidth = iImageWidth;
	else
		m_stretchWidth = m_clientRectWidth;
	m_bUnsigned = !bSigned;
	m_bInvert = bInvert;
	if(true)
	{
		m_dataImageHeight = iImageHeight;
		m_dataImageWidth = iImageWidth;
	}else{
		m_dataImageHeight = height;
		m_dataImageWidth = width;
	}
	m_hDestDC = ::GetDC(pWnd->GetSafeHwnd());
	m_pSrcWnd = pWnd;
    ASSERT(m_hDestDC);

	// pass Negative Height to make it Top Down.
	width = m_dataImageWidth + m_kDeltaX; // not *2 because we don't need it on the left side.
	VERIFY(m_image.Create(width, -(m_dataImageHeight + m_kDeltaY*2), 8, 0));
	ASSERT((DWORD_PTR(m_image.GetBits()) & 0xf) == 0); // make sure we are at least 128 bit aligned.(SSE2)
	ASSERT(m_image.GetPitch() > 0); // Verify top down DIB
	VERIFY(m_image.GetDC() != NULL); // Prefer the DC to exist for life of object
	int iBPP=m_image.GetBPP();//The bits per pixel is usually 1, 4, 8, 16, 24, or 32.
	int iPitch=m_image.GetPitch();
	void *pData=m_image.GetPixelAddress(0,0);
	pData=m_image.GetBits();
	bool bNull=IsNull();
	pData=m_image.GetPixelAddress(0,1);
	RGBQUAD ColorTable[256];
	if(g_bColorDefault)
	{
		for(int i=0;i<256;i++)
		{
			//pixel = RGB(ColorTable[iColor].rgbRed,ColorTable[iColor].rgbGreen,ColorTable[iColor].rgbBlue);
			ColorTable[i].rgbRed=ColorTable[i].rgbGreen=ColorTable[i].rgbBlue=i;
		}
	}else{
		if(bSigned)
		{
			for(int i=0;i<256;i++)
			{
				//pixel = RGB(ColorTable[iColor].rgbRed,ColorTable[iColor].rgbGreen,ColorTable[iColor].rgbBlue);
				ColorTable[i].rgbRed = g_byColorRed[i];
				ColorTable[i].rgbGreen = g_byColorGreen[i];
				ColorTable[i].rgbBlue = g_byColorBlue[i];
			}
		}else{
			for(int i=0;i<256;i++)
			{
				//pixel = RGB(ColorTable[iColor].rgbRed,ColorTable[iColor].rgbGreen,ColorTable[iColor].rgbBlue);
				ColorTable[i].rgbRed = g_byColorRedU[i];
				ColorTable[i].rgbGreen = g_byColorGreenU[i];
				ColorTable[i].rgbBlue = g_byColorBlueU[i];
			}
		}
	}
	m_image.SetColorTable(0,256,ColorTable);

#ifdef _MINMAX_BUFFER_
	EnterCriticalSection(&g_CriticalAscan2);
	if(bInvert)
	{
		m_pImageAscanMin = new DWORD[m_dataImageHeight];
		if(!m_pImageAscanMin)
			memset(m_pImageAscanMin,0,sizeof(DWORD)*m_dataImageHeight);
	}else{
		m_pImageAscanMin = new DWORD[m_dataImageWidth];
		if(!m_pImageAscanMin)
			memset(m_pImageAscanMin,0,sizeof(DWORD)*m_dataImageWidth);
	}
	LeaveCriticalSection(&g_CriticalAscan2);

	if(!m_pImageAscanMin)
		return false;
#endif _MINMAX_BUFFER_
	return true;
}

void CDisplayImage::Destroy()
{ 
#ifdef _MINMAX_BUFFER_
	EnterCriticalSection(&g_CriticalAscan2);
	if(m_pImageAscanMin)
		delete m_pImageAscanMin;
	m_pImageAscanMin = NULL;
	LeaveCriticalSection(&g_CriticalAscan2);
	if(m_pPointMin)
		delete m_pPointMin;
	m_pPointMin = NULL;
#endif _MINMAX_BUFFER_

	m_dataImageHeight = 0;
	m_dataImageWidth = 0;
	if(m_pPoint)
		delete m_pPoint;
	m_pPoint = NULL;
	m_iPointCount = 0;
	if (IsNull())
		return;

	m_image.ReleaseDC();
	m_image.Destroy(); 
	::ReleaseDC(m_pSrcWnd->GetSafeHwnd(), m_hDestDC);
}

void CDisplayImage::Import(const CImage &image)
{
	image.BitBlt(m_image.GetDC(), 0, m_kDeltaY, 
		GetVisibleWidth(), GetVisibleHeight(), 0, 0);
	m_image.ReleaseDC();
	BlitBits();
}

void CDisplayImage::ClearBits()
{
	if(IsNull())
		return;
	if(!m_dataImageHeight || !m_dataImageWidth)
		return;
    int size = m_image.GetPitch() * m_image.GetHeight();
	memset(m_image.GetBits(), 0x00, size);
}

void CDisplayImage::Point(CPoint &point,int &x,int &y,unsigned char &usValue)
{
	double dAux;
	x=y=0;
	if(!m_dataImageHeight || !m_dataImageWidth)
		return;
	ASSERT(m_dataImageHeight && m_dataImageWidth);
	if(point.x<0)
		return;
	if(point.x>=m_clientRectWidth)
		return;
	if(point.y<0)
		return;
	if(point.y>=m_clientRectHeight)
		return;
	dAux = (double)point.x/(double)m_clientRectWidth;
	dAux *= (double)m_dataImageWidth;
	x = (int)dAux;
	dAux = (double)point.y/(double)m_clientRectHeight;
	dAux *= (double)m_dataImageHeight;
	y = (int)dAux;
	usValue = Random();
	//SetPointColor(x, y, usValue);//RGB(Random(), Random(), Random()), usValue);
}

void CDisplayImage::BlitBits()
{
	if(!m_dataImageHeight || !m_dataImageWidth)
		return;
	ASSERT(m_dataImageHeight && m_dataImageWidth);
    BOOL bStat = m_image.BitBlt(m_hDestDC, 0, 0, 
        m_dataImageWidth, m_dataImageHeight, 0, m_kDeltaY);

    ASSERT(bStat);
}

void CDisplayImage::Display(CWnd *pWnd,enumDisplayType eDisplayType,bool bImage)
{
	switch(eDisplayType)
	{
	default:
	case eXZ://Bscan
	case eXY://Cscan
	case eYZ://Dscan
		StretchBits();
		break;
	case eProfileZ://Ascan
	case eProfileX:
	case eProfileY:
		if(bImage)
			StretchBits();
		else
			LineBits(pWnd);
		break;
	}
	m_eDisplayType = eDisplayType;
	m_bImage = bImage;
}

void CDisplayImage::StretchBits()
{
	BOOL bStat;

	if(IsNull())
		return;
	if(!m_dataImageHeight || !m_dataImageWidth)
		return;
	ASSERT(m_dataImageHeight && m_dataImageWidth);
	bStat = m_image.StretchBlt(m_hDestDC, 0, 0, m_clientRectWidth, m_clientRectHeight, 0, m_kDeltaY, m_stretchWidth, m_stretchHeight);
}

void CDisplayImage::LineBits(CWnd *pWnd)
{
	if(IsNull())
		return;
	if(!m_dataImageHeight || !m_dataImageWidth)
		return;

    CRect clientRect;
	CPen penBlack,penWhite,penBlue2;
	//DON'T REMOVE THIS COMMENT : to do : move this code elsewhere
	//int iWidth = (dwOptionCursorH>>4)+1;
	//if(iWidth<(dwOptionCursorV>>4)+1)
	//	iWidth = (dwOptionCursorV>>4)+1;
	int iWidth1 = 1;
	int iWidth2 = 1;
	if(m_bBufferMin && m_bBufferMax)
		iWidth1 = 2;
	if (!penBlack.CreatePen(PS_SOLID, iWidth1, RGB(0,0,0)))
		return;
	if (!penWhite.CreatePen(PS_SOLID, iWidth1, RGB(255,255,255)))
		return;
	if (!penBlue2.CreatePen(PS_DASHDOTDOT/*PS_SOLID*/, iWidth2, RGB(0,0,255)))//PS_DASH
		return;
	CDC dc;
	dc.Attach(m_hDestDC);
	CPen* pOldPen=NULL;
	unsigned char usValue;
	unsigned char sValue;
	int iPointCount;
	double dAux;

#ifdef _MINMAX_BUFFER_
EnterCriticalSection(&g_CriticalAscan2);
#endif _MINMAX_BUFFER_
	if(!m_bInvert)
		iPointCount = m_dataImageWidth;
	else
		iPointCount = m_dataImageHeight;
	if(m_dataImageWidth==1)
		iPointCount++;
	if(!m_pPoint || (m_iPointCount != iPointCount))
	{
		CBrush brushWhite(RGB(255,255,255));
		CBrush* pOldBrush = dc.SelectObject(&brushWhite);
		pWnd->GetClientRect(clientRect);
		dc.FillRect(clientRect,&brushWhite);
		dc.SelectObject(pOldBrush);
		if(m_pPoint)
			delete m_pPoint;
		m_pPoint = NULL;
		pOldPen = dc.SelectObject(&penBlack);

#ifdef _MINMAX_BUFFER_
		if(m_pPointMin)
			delete m_pPointMin;
		m_pPointMin = NULL;
#endif _MINMAX_BUFFER_
	}else{
		pOldPen = dc.SelectObject(&penWhite);
		dc.Polyline(m_pPoint,m_iPointCount);
#ifdef _MINMAX_BUFFER_
		if(m_pImageAscanMin)
			dc.Polyline(m_pPointMin,m_iPointCount);
#endif _MINMAX_BUFFER_
		dc.SelectObject(&penBlack);
	}

	if(m_clientRectHeight)
	{
		if(!m_pPoint)
		{
			m_iPointCount = iPointCount;
			m_pPoint = new POINT[m_iPointCount];
#ifdef _MINMAX_BUFFER_
			m_pPointMin = new POINT[m_iPointCount];
#endif _MINMAX_BUFFER_
		}
		if(m_pPoint)
		{
			if(m_iPointCount==2)
			{
				GetPointColor(0, 0, usValue);
				if(!m_bUnsigned)
				{
					sValue = (char)usValue;
					usValue = sValue+128;
				}
				m_pPoint[0].x = 0;
				m_pPoint[1].x = m_clientRectWidth-1;
				//0, 256-1 => m_clientRectHeight-1, 0
				m_pPoint[0].y = m_clientRectHeight-1-(usValue*(m_clientRectHeight-1)/(256-1));
				m_pPoint[1].y = m_clientRectHeight-1-(usValue*(m_clientRectHeight-1)/(256-1));
			}else{
				if(!m_bInvert)
				{
					for(int i=0;i<m_iPointCount;i++)
					{
						GetPointColor(i, 0, usValue);
						m_pPoint[i].x = i*(m_clientRectWidth-1)/(m_iPointCount-1);
						dAux = (usValue*(m_clientRectHeight-1)/(256-1));
						m_pPoint[i].y = m_clientRectHeight-1-(int)dAux;
					}
#ifdef _MINMAX_BUFFER_
if(m_pImageAscanMin)
{
	for(int i=0;i<m_iPointCount;i++)
	{
		GetPointColor(i, 0, usValue);
		//usValue = usValue/2+128;//top part
		m_pPoint[i].x = i*(m_clientRectWidth-1)/(m_iPointCount-1);
		dAux = (usValue*(m_clientRectHeight-1)/(256-1));
		m_pPoint[i].y = m_clientRectHeight-1-(int)dAux;
		GetPointColorMin(i, usValue);
		//usValue = usValue/2;//bottom part
		m_pPointMin[i].x = i*(m_clientRectWidth-1)/(m_iPointCount-1);
		dAux = (usValue*(m_clientRectHeight-1)/(256-1));
		m_pPointMin[i].y = m_clientRectHeight-1-(int)dAux;
	}
}
#endif _MINMAX_BUFFER_
				}else{
					for(int i=0;i<m_iPointCount;i++)
					{
						GetPointColor(0, i, usValue);
						m_pPoint[i].y = i*(m_clientRectHeight-1)/(m_iPointCount-1);
						dAux = (usValue*(m_clientRectWidth-1)/(256-1));
						//m_pPoint[i].x = m_clientRectWidth-1-(int)dAux;
						m_pPoint[i].x = (int)dAux;
					}
#ifdef _MINMAX_BUFFER_
if(m_pImageAscanMin)
{
	for(int i=0;i<m_iPointCount;i++)
	{
		GetPointColor(0, i, usValue);
		//usValue = usValue/2+128;//top part
		m_pPoint[i].y = i*(m_clientRectHeight-1)/(m_iPointCount-1);
		dAux = (usValue*(m_clientRectWidth-1)/(256-1));
		m_pPoint[i].x = (int)dAux;//m_pPoint[i].x = m_clientRectWidth-1-(int)dAux;
		GetPointColorMin(i, usValue);
		//usValue = usValue/2;//bottom part
		m_pPointMin[i].y = i*(m_clientRectHeight-1)/(m_iPointCount-1);
		dAux = (usValue*(m_clientRectWidth-1)/(256-1));
		m_pPointMin[i].x = (int)dAux;//m_pPoint[i].x = m_clientRectWidth-1-(int)dAux;
	}
}
#endif _MINMAX_BUFFER_
				}
			}
			if(m_bBufferMax)
			dc.Polyline(m_pPoint,m_iPointCount);
#ifdef _MINMAX_BUFFER_
if(m_bBufferMin && m_pImageAscanMin)
{
	dc.SelectObject(&penBlue2);
	dc.Polyline(m_pPointMin,m_iPointCount);
}
#endif _MINMAX_BUFFER_
		}
	}

	dc.SelectObject(pOldPen);
	dc.Detach();
#ifdef _MINMAX_BUFFER_
LeaveCriticalSection(&g_CriticalAscan2);
#endif _MINMAX_BUFFER_
}

void CDisplayImage::DisplayCursor()
{
	if(IsNull())
		return;
	if(!m_dataImageHeight || !m_dataImageWidth)
		return;

	CDC dc;
	dc.Attach(m_hDestDC);

	dc.Detach();
}

bool CDisplayImage::IsImage()
{
	bool bImage;
	switch(m_eDisplayType)
	{
	default:
	case eXZ://Bscan
	case eXY://Cscan
	case eYZ://Dscan
		bImage = true;
		break;
	case eProfileZ://Ascan
	case eProfileX:
	case eProfileY:
		if(m_bImage)
			bImage = true;
		else
			bImage = false;
		break;
	}
	return bImage;
}

bool CDisplayImage::Check()
{
	if(!this || IsNull())
		return false;
	if(!m_dataImageHeight || !m_dataImageWidth)
		return false;
	return true;
}

void CDisplayImage::SetBufferMin(bool bEnable)
{
	m_bBufferMin = bEnable;
}

void CDisplayImage::SetBufferMax(bool bEnable)
{
	m_bBufferMax = bEnable;
}
