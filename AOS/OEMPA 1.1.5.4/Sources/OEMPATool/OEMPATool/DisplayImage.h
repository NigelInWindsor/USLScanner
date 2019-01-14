#pragma once
#include <atlimage.h>

inline int Random() { return(rand()); }
inline int Random(int n) { return(rand() % n ); }
template<class T> T ABS(T x) { return(x < 0 ? -x : x); }

class CUTAcquisitionData;
class CDisplayImage;
class CChildView;

class CParameterView
{
public:
	CParameterView(CDisplayImage *pImage=NULL);

	int m_scaleWidth;//scale size width.
	int m_scaleHeight;//scale size height.
	int m_dataImageWidth;//data image size width.
	int m_dataImageHeight;//data image size height.
	int m_clientRectWidth;//window size
	int m_clientRectHeight;//window size
	int m_stretchWidth;
	int m_stretchHeight;
	bool m_bUnsigned;
	enumDisplayType m_eDisplayType;
	bool m_bImage;
	bool m_bInvert;

	CChildView *m_pImageView;
	CDisplayImage *m_pDisplayImage;
};

class CDisplayImage :	virtual public CParameterView
{
public:
	CDisplayImage(CChildView *pImageView);
	virtual ~CDisplayImage();

	bool Create(CWnd *pWnd,int iImageWidth,int iImageHeight,int iScaleWidth,int iScaleHeight,bool bSigned,bool bImage,bool bInvert);
	void Destroy();

	void Import(const CImage &image);

    void ClearBits();
    void BlitBits();
	void StretchBits();
	void LineBits(CWnd *pWnd);
	void Display(CWnd *pWnd,enumDisplayType eDisplayType,bool bImage);
	void Point(CPoint &point,int &x,int &y,unsigned char &usValue);
	bool IsImage();
	void DisplayCursor();

	// These methods compensate for m_kDelta
	bool Check();
	void GetPointColor(int x, int y, unsigned char &usValue);
	void SetPointColor(int x, int y, unsigned char usValue);
	BYTE *GetPixelAddress(int x, int y) const
	{ return((BYTE*)m_image.GetPixelAddress(x, y+m_kDeltaY)); }

	bool IsNull() const
	{ return(m_image.IsNull()); }
	int GetVisibleWidth() const
	{ return(m_dataImageWidth); }
	int GetVisibleHeight() const
	{ return(m_dataImageHeight); }
	int GetPitch() const
	{ return(m_image.GetPitch()); }

	CImage *GetImage()
	{ return(&m_image); }

#ifdef _MINMAX_BUFFER_
	void GetPointColorMin(int x, unsigned char &usValue);
	void SetPointColorMin(int x, unsigned char usValue);
	void SetBufferMin(bool bEnable);
	void SetBufferMax(bool bEnable);
#endif _MINMAX_BUFFER_
// Implementation
protected:

	static const int m_kDeltaX = 1;
	static const int m_kDeltaY = 1;
private:
	CWnd *m_pSrcWnd;
   	HDC m_hDestDC;
	CImage m_image;
	int m_iPointCount;
	POINT *m_pPoint;
#ifdef _MINMAX_BUFFER_
	bool m_bBufferMax;
	bool m_bBufferMin;
	POINT *m_pPointMin;
	DWORD *m_pImageAscanMin;
#endif _MINMAX_BUFFER_
};

inline void CDisplayImage::GetPointColor(int x, int y, unsigned char &usValue)
{
	// m_image.SetPixel() call ::SetPixel() which is too slow
	// since it has to work with all DCs.
	BYTE *p = (BYTE*)m_image.GetPixelAddress(x, (y)+m_kDeltaY);
	usValue = *p;
}

inline void CDisplayImage::SetPointColor(int x, int y, unsigned char usValue)
{
	// m_image.SetPixel() call ::SetPixel() which is too slow
	// since it has to work with all DCs.
	BYTE *p = (BYTE*)m_image.GetPixelAddress(x, (y)+m_kDeltaY);
	*p = usValue;
}

#ifdef _MINMAX_BUFFER_
inline void CDisplayImage::GetPointColorMin(int x, unsigned char &usValue)
{
	// m_image.SetPixel() call ::SetPixel() which is too slow
	// since it has to work with all DCs.
	usValue = (BYTE)m_pImageAscanMin[x];
}
inline void CDisplayImage::SetPointColorMin(int x, unsigned char usValue)
{
	// m_image.SetPixel() call ::SetPixel() which is too slow
	// since it has to work with all DCs.
	m_pImageAscanMin[x] = (DWORD)usValue;
}
#endif _MINMAX_BUFFER_
