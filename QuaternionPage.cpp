// QuaternionPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "QuaternionPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuaternionPage property page

IMPLEMENT_DYNCREATE(CQuaternionPage, CDialog)

CQuaternionPage::CQuaternionPage(CWnd* pParent /*=NULL*/,CWnd** pDlg) : CDialog(CQuaternionPage::IDD)
{
	//{{AFX_DATA_INIT(CQuaternionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pArray = NULL;
	m_pFont = NULL;
	m_pDlg = pDlg;
}

CQuaternionPage::~CQuaternionPage()
{
	SAFE_DELETE(m_pArray);
	SAFE_DELETE(m_pFont);
}

void CQuaternionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuaternionPage)
	DDX_Control(pDX, IDC_SLIDER_V, m_sliderV);
	DDX_Control(pDX, IDC_SLIDER_Z, m_sliderZ);
	DDX_Control(pDX, IDC_SLIDER_Y, m_sliderY);
	DDX_Control(pDX, IDC_SLIDER_X, m_sliderX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuaternionPage, CDialog)
	//{{AFX_MSG_MAP(CQuaternionPage)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuaternionPage message handlers

void CQuaternionPage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CQuaternionPage::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CQuaternionPage::IDD & MAXIMUMWINDOWSIDNUMBER] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CQuaternionPage::OnDestroy() 
{
	

	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CQuaternionPage::IDD & MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CQuaternionPage::OnInitDialog() 
{
	
	CDialog::OnInitDialog();
	LOGFONT	lf;

//	CRect rect = theApp.m_LastSettings.RectWindowPos[CQuaternionPage::IDD & MAXIMUMWINDOWSIDNUMBER];
//	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
//		theApp.ConfineToScreen(&rect);
//		SetWindowPos( &wndTop , rect.left, rect.top, NULL, NULL, NULL);
//	}



	MinMax(&theApp.m_LastSettings.vQuatTest.x, -180.0f, 180.0f); 
	MinMax(&theApp.m_LastSettings.vQuatTest.y, -180.0f, 180.0f); 
	MinMax(&theApp.m_LastSettings.vQuatTest.z, -180.0f, 180.0f); 
	
	m_sliderX.SetRange(0,360);
	m_sliderX.SetPos(180 - (int)theApp.m_LastSettings.vQuatTest.x);
	m_sliderY.SetRange(0,360);
	m_sliderY.SetPos(180 - (int)theApp.m_LastSettings.vQuatTest.y);
	m_sliderZ.SetRange(0,360);
	m_sliderZ.SetPos(180 + (int)theApp.m_LastSettings.vQuatTest.z);
	m_sliderV.SetRange(0,360);
	m_sliderV.SetPos(180 + (int)theApp.m_LastSettings.vQuatTest.w);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	lf = m_LogFont;
	lf.lfHeight = -14;
	theApp.SetFontLanguage(&lf);
	SAFE_DELETE( m_pFont );
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&lf);
	
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuaternionPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

}


#define	BLACK	RGB(0,0,0)
#define	BLUE	RGB(0,0,255)

void CQuaternionPage::OnPaint() 
{
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	
	CRect		rr,rect,rrStore;
	CDC			dcMem;
	CBitmap		bmp;
	CSize size;
	CString Buff;
	CPoint ptText = CPoint(2,2);
	D3DXMATRIX mRotate, mTemp, mAxis, mOut;
	D3DXVECTOR4 vOut;
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 xAxis,yAxis, vSide, vSurface, vScale, vEuler;
	D3DXQUATERNION	qSurface;
	CCoord Cp;
	
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
//	rr.DeflateRect(1,1);
	if(m_pArray==NULL) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width();
	rect.bottom = rr.Height();

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(m_pFont);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbFaroText);
	dcMem.SetBkMode(TRANSPARENT);

	dcMem.FillRect(&rect,&CBrush(RGB(255,255,255)));


	//Rotate angles
	Buff.Format(L"X:%.0f Y:%.0f Z:%.0f W:%.0f",theApp.m_LastSettings.vQuatTest.x,theApp.m_LastSettings.vQuatTest.y,theApp.m_LastSettings.vQuatTest.z,theApp.m_LastSettings.vQuatTest.w);
	size = dcMem.GetTextExtent(Buff);
	dcMem.TextOut(ptText.x, ptText.y, Buff);
	ptText.y += (size.cy * 2);


	//Surface vector
	RenderText(&dcMem, &ptText,BLUE,1,L"Euler angles to vector No rotation about vector");
	D3DXMatrixIdentity(&mRotate);

	D3DXMatrixRotationY(&mTemp,theApp.m_LastSettings.vQuatTest.y * DEG_TO_RAD);
	D3DXMatrixMultiply(&mRotate,&mRotate,&mTemp);

	D3DXMatrixRotationZ(&mTemp,theApp.m_LastSettings.vQuatTest.z * DEG_TO_RAD);
	D3DXMatrixMultiply(&mRotate,&mRotate,&mTemp);

	D3DXMatrixRotationAxis(&mAxis, &vSurface, theApp.m_LastSettings.vQuatTest.w * DEG_TO_RAD);
	D3DXMatrixMultiply(&mRotate,&mRotate,&mAxis);

	CopyMemory(&m_vSurface, &mRotate._31, sizeof (D3DXVECTOR4));
	CopyMemory(&vSurface, &mRotate._31, sizeof (D3DXVECTOR3));
	RenderText(&dcMem, &ptText,BLACK,2,vSurface);


	RenderText(&dcMem, &ptText,BLACK,2,mRotate);

	vEuler.x = 0.0f;
	vEuler.y = acosf(vSurface.z) * RAD_TO_DEG;
	vEuler.z = atan2f(vSurface.y, vSurface.x) * RAD_TO_DEG;

	if(vSurface.y >= 0.0f) {
		vEuler.z = vEuler.z - 180.0f;
	} else {
		vEuler.z = 180 + vEuler.z;
	}
	if(vSurface.x >= 0.0f) {

	} else {
		vEuler.y *= -1.0f;
	}

	RenderText(&dcMem, &ptText,BLACK,2,vEuler);


	/*
	vSurface = (D3DXVECTOR3)m_vSurface;
	D3DXMatrixIdentity(&mRotate);
	vUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Cross(&vSide, &vUp, &vSurface);
	D3DXVec3Cross(&vUp, &vSide, &vSurface);
	D3DXVec3Normalize(&vSide,&vSide);
	D3DXVec3Normalize(&vUp,&vUp);

	if(m_vSurface.y >= 0.0f) {

	} else {
//		vUp *= -1.0f;
//		vSide *= -1.0f;
	}

	CopyMemory(&mRotate._11, &vUp, sizeof D3DXVECTOR3);
	CopyMemory(&mRotate._21, &vSide, sizeof D3DXVECTOR3);
	CopyMemory(&mRotate._31, &vSurface, sizeof D3DXVECTOR3);



	RenderText(&dcMem, &ptText,BLACK,2,mRotate);

	


	//RotaionAxis
	RenderText(&dcMem, &ptText,BLUE,1,"Euler angles to vector With rotation about vector");
	vSurface = (D3DXVECTOR3)m_vSurface;
	D3DXVec3Transform(&m_vSurface,&vUp,&mRotate);
	RenderText(&dcMem, &ptText,BLACK,1,(D3DXVECTOR3)m_vSurface);

	RenderText(&dcMem, &ptText,BLUE,1,"Quaternion from rotation matrix with rotation about vector");
	D3DXQuaternionRotationMatrix(&qSurface, &mRotate);
	RenderText(&dcMem, &ptText,BLACK,2,qSurface);


	//Surface vector to quaternion
	RenderText(&dcMem, &ptText,BLUE,1,"Surface vector to Quaternion No rotation about vector");
	RenderText(&dcMem, &ptText,BLACK,1,(D3DXVECTOR3)m_vSurface);

	RenderText(&dcMem, &ptText,BLUE,1,"Quaternion from rotation matrix NO rotation about vector");
	vSurface = (D3DXVECTOR3)m_vSurface;
	D3DXMatrixIdentity(&mRotate);
	vUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Cross(&vSide, &vUp, &vSurface);
	D3DXVec3Cross(&vUp, &vSide, &vSurface);
	D3DXVec3Normalize(&vSide,&vSide);
	D3DXVec3Normalize(&vUp,&vUp);
	CopyMemory(&mRotate._11, &vUp, sizeof D3DXVECTOR3);
	CopyMemory(&mRotate._21, &vSide, sizeof D3DXVECTOR3);
	CopyMemory(&mRotate._31, &vSurface, sizeof D3DXVECTOR3);


	D3DXQuaternionRotationMatrix(&qSurface, &mRotate);
	D3DXQuaternionNormalize(&qSurface,&qSurface);
	RenderText(&dcMem, &ptText,BLACK,2,qSurface);
	RenderText(&dcMem, &ptText,BLACK,2,mRotate);


	RenderText(&dcMem, &ptText,BLUE,1,"Quaternion from rotation matrix with rotation about vector");
	D3DXMatrixMultiply(&mRotate,&mRotate,&mAxis);
	D3DXQuaternionRotationMatrix(&qSurface, &mRotate);
	RenderText(&dcMem, &ptText,BLACK,2,qSurface);
*/

	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
}

void CQuaternionPage::RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXVECTOR3 vec)
{
	CString Buff;

	Buff.Format(L"i:%.03f j:%.03f k:%.03f",vec.x,vec.y,vec.z);
	RenderText(pDC,pt,rgb,nLines,Buff);
}

void CQuaternionPage::RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXVECTOR4 vec)
{
	CString Buff;

	Buff.Format(L"i:%.03f j:%.03f k:%.03f w:%.03f",vec.x,vec.y,vec.z, vec.w * RAD_TO_DEG);
	RenderText(pDC,pt,rgb,nLines,Buff);
}

void CQuaternionPage::RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXQUATERNION q)
{
	CString Buff;

	Buff.Format(L"A:%.03f B:%.03f C:%.03f U:%.03f",q.x,q.y,q.z,q.w);
	RenderText(pDC,pt,rgb,nLines,Buff);
}

void CQuaternionPage::RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXMATRIX m)
{
	CString Buff;

	Buff.Format(L"%.03f %.03f %.03f %.03f",m._11,m._12,m._13,m._14);
	RenderText(pDC,pt,rgb,1,Buff);
	Buff.Format(L"%.03f %.03f %.03f %.03f",m._21,m._22,m._23,m._24);
	RenderText(pDC,pt,rgb,1,Buff);
	Buff.Format(L"%.03f %.03f %.03f %.03f",m._31,m._32,m._33,m._34);
	RenderText(pDC,pt,rgb,1,Buff);
	Buff.Format(L"%.03f %.03f %.03f %.03f",m._41,m._42,m._43,m._44);
	RenderText(pDC,pt,rgb,nLines,Buff);
}

void CQuaternionPage::RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, CString Buff)
{
	CSize size;
	pDC->SetTextColor(rgb);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt->x, pt->y, Buff);
	pt->y += (size.cy * nLines);
}

void CQuaternionPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	int nTemp = pSlider->GetPos()&0xffff;
	nTemp = 15 * (nTemp / 15);
	float fVertical = ((float)nTemp - 180.0f) * -1.0f;

	switch(pSlider->GetDlgCtrlID()) {
	case IDC_SLIDER_X:
		theApp.m_LastSettings.vQuatTest.x = fVertical;
		break;
	case IDC_SLIDER_Y:
		theApp.m_LastSettings.vQuatTest.y = fVertical;
		break;
	}
	Invalidate(false);


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CQuaternionPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	int nTemp = pSlider->GetPos()&0xffff;
	nTemp = 15 * (nTemp / 15);

	float fHorizontal = (float) nTemp - 180.0f;

	switch(pSlider->GetDlgCtrlID()) {
	case IDC_SLIDER_Z:
		theApp.m_LastSettings.vQuatTest.z = fHorizontal;
		break;
	case IDC_SLIDER_V:
		theApp.m_LastSettings.vQuatTest.w = fHorizontal;
		break;
	}
	Invalidate(false);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

