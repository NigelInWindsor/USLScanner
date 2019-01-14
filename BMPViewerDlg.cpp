// BMPViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BMPViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBMPViewerDlg dialog


CBMPViewerDlg::CBMPViewerDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CBMPViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBMPViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pForeImage = NULL;
}


void CBMPViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMPViewerDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_sliderBlue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMPViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CBMPViewerDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMPViewerDlg message handlers

void CBMPViewerDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CBMPViewerDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CBMPViewerDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CBMPViewerDlg::OnDestroy() 
{
	if (CBMPViewerDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CBMPViewerDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CBMPViewerDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();

	if (CBMPViewerDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CBMPViewerDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	RetrieveInitialImage();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBMPViewerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBMPViewerDlg::OnPaint() 
{
	CRect rr;

	CPaintDC dc(this); // device context for painting
	
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.Width()%4) {
		rr.right=(rr.right-rr.left)/4+rr.left;
		rr.right=(rr.right-rr.left)*4+rr.left;
	};


	DrawImage(&dc,rr);
}

void CBMPViewerDlg::DrawImage(CPaintDC* pDC,CRect rr)
{
	if (GetSafeHwnd() == NULL) return;
	CDC  dcMem;
	CBitmap bmp;
	int nSrc,nDest,xx,yy,nX0,nX1,nWidth,nHeight,nY0;
	CRect rrScreen;
	GetWindowRect(&rrScreen);

	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nDestBytesPixel = nBitsPixel/8;


	nWidth = m_BmpHeader.biWidth;
	nHeight = m_BmpHeader.biHeight;
	nY0 = 0;
	nY0 = m_BmpHeader.biHeight-1;
	rr.bottom = rr.top + m_BmpHeader.biHeight;
	nX0=0;
	nX1=m_BmpHeader.biWidth-1;
	rr.right = rr.left + m_BmpHeader.biWidth;

	COLORREF* m_pScreen = new COLORREF[rr.Width()*rr.Height()];
	memset(m_pScreen,0xff,rr.Width() * rr.Height() * (sizeof COLORREF));
	char* pArray = (char *)m_pScreen;

	int m_nDisplayMode = 0;
	int nDevice = 0;
	int nFullScreenMode=0;
	int	m_nWindowSize=1;

	switch(m_nDisplayMode=0) {
	case 0:
		if((m_nWindowSize==1) && (nFullScreenMode==1) && (nDevice==0)) {
			for(yy=0,nDest=0;yy<rr.Height();yy++) {
				for(xx=0;xx<rr.Width();xx++) {
					nSrc=MulDiv(xx,nWidth,rr.Width()) + nX0 + (nY0+ MulDiv(yy,nHeight,rr.Height())) * m_BmpHeader.biWidth;
					pArray[nDest*nDestBytesPixel]=GetRValue(m_pForeImage[nSrc]);
					pArray[nDest*nDestBytesPixel+1]=GetGValue(m_pForeImage[nSrc]);
					pArray[nDest*nDestBytesPixel+2]=GetBValue(m_pForeImage[nSrc]);
					nDest++;
				}
			}
		}
		if((m_nWindowSize==1) && (nFullScreenMode==2) && (nDevice==0)) {
			for(yy=0,nDest=0;yy<rr.Height();yy++) {
				for(xx=rr.Width()-1;xx>=0;xx--) {
					nSrc=MulDiv(xx,nWidth,rr.Width()) + nX0 + (nY0+ MulDiv(yy,nHeight,rr.Height())) * m_BmpHeader.biWidth;
					pArray[nDest*nDestBytesPixel]=GetRValue(m_pForeImage[nSrc]);
					pArray[nDest*nDestBytesPixel+1]=GetGValue(m_pForeImage[nSrc]);
					pArray[nDest*nDestBytesPixel+2]=GetBValue(m_pForeImage[nSrc]);
					nDest++;
				}
			}
		}
		if((m_nWindowSize==0) || (nDevice==1) || (nFullScreenMode==0)) {
			for(yy=rr.Height()-1,nDest=0;yy>=0;yy--) {
				nSrc = yy * (m_BmpHeader.biWidth);
				for(xx=0;xx<rr.Width();xx++,nSrc++) {
//					nSrc=MulDiv(xx,nWidth,rr.Width()) + nX0 + (nY0+ MulDiv(yy,nHeight,rr.Height())) * m_BmpHeader.biWidth;
					pArray[nDest*nDestBytesPixel+2]=GetRValue(m_pForeImage[nSrc]);
					pArray[nDest*nDestBytesPixel+1]=GetGValue(m_pForeImage[nSrc]);
					pArray[nDest*nDestBytesPixel+2]=GetBValue(m_pForeImage[nSrc]);
					nDest++;
				}
			}
		}
		break;
	case 1:
		/*
		for(yy=rr.Height()-1,nDest=0;yy>=0;yy--) {
			for(xx=0;xx<rr.Width();xx++) {
				nSrc=MulDiv(xx,nWidth,rr.Width()) + nX0 + MulDiv(yy,m_BmpHeader.biHeight,rr.Height()) * m_BmpHeader.biWidth;
				m_pScreen[nDest++]=m_pBackImage[nSrc];
			}
		}
		*/
		break;
	case 2:
		/*
		for(yy=rr.Height()-1,nDest=0;yy>=0;yy--) {
			for(xx=0;xx<rr.Width();xx++) {
				nSrc=MulDiv(xx,nWidth,rr.Width()) + nX0 + MulDiv(yy,m_BmpHeader.biHeight,rr.Height()) * m_BmpHeader.biWidth;
				if((abs(GetRValue(m_pBackImage[nSrc]) - GetRValue(m_pForeImage[nSrc])) < 30) &&
					(abs(GetGValue(m_pBackImage[nSrc]) - GetGValue(m_pForeImage[nSrc])) < 30) &&
					(abs(GetBValue(m_pBackImage[nSrc]) - GetBValue(m_pForeImage[nSrc])) < 30))
				{
					m_pScreen[nDest++]=RGB(255,255,255);
				} else {
					m_pScreen[nDest++]=m_pForeImage[nSrc];
				}
			}
		}
		*/
		break;
	}
	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*nDestBytesPixel),m_pScreen);

	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete m_pScreen;



}


void CBMPViewerDlg::RetrieveInitialImage()
{
	CUSLFile File;
	CFileException e;
	CString Buff;
	int nSrc,nDest,xx,yy;
	unsigned int nB;
	unsigned char cc;
	unsigned char Array[1024];

	m_FilePath="C:\\Users\\Nigel\\Documents\\My Scans\\Seal 24 bit.bmp";
	if(File.Open(m_FilePath,CFile::typeBinary|CFile::modeReadWrite,&e)==0) {
		File.DecodeFileException(&e,&Buff,m_FilePath);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
	} else {
		File.Read(&m_BmpFileHeader,sizeof m_BmpFileHeader);
		File.Read(&m_BmpHeader,sizeof m_BmpHeader);

		CDC* pDC = GetDC();
		int nSrcBytesPixel = m_BmpHeader.biBitCount / 8;
		int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
		int nDestBytesPixel = nBitsPixel/8;
		int	nArraySize = m_BmpHeader.biWidth * m_BmpHeader.biHeight;
		char* pBuff = new char[(int)File.GetLength()];


		File.Seek(0,CFile::begin);
		File.Read(Array,1024);


		int nOffset = (int)File.GetLength() -  nArraySize * nSrcBytesPixel;
		File.Seek(m_BmpFileHeader.bfOffBits,CFile::begin);
		File.Read(pBuff,(int)File.GetLength() - m_BmpFileHeader.bfOffBits);
		if(m_pForeImage==NULL) m_pForeImage = new COLORREF[nArraySize];
		char* pDestArray = (char*)m_pForeImage;
		nSrc=0;
		nDest=0;
		for(yy=0;yy<m_BmpHeader.biHeight;yy++) {
			for(xx=0;xx<m_BmpHeader.biWidth;xx++) {

				nB=pBuff[nSrc] & 0xff;
				nB = (nB*150) / 100;
				if(nB>255) nB=255;
				cc=nB;
				pBuff[nSrc]=cc;

//				nR=pBuff[nSrc+2] & 0xff;
//				nR = (nR*150) / 100;
//				if(nR>255) nR=255;
//				cc=nR;
//				pBuff[nSrc+2]=cc;

				pDestArray[nDest++]=pBuff[nSrc++];
				pDestArray[nDest++]=pBuff[nSrc++];
				pDestArray[nDest++]=pBuff[nSrc++];
				if((nSrcBytesPixel==4) && (nDestBytesPixel==4)) {
					pDestArray[nDest++]=pBuff[nSrc++];
				}
				if((nSrcBytesPixel==3) && (nDestBytesPixel==4)) {
					pDestArray[nDest++]=0;
				}
			}
			nSrc++;
		}

		File.Seek(m_BmpFileHeader.bfOffBits,CFile::begin);
		File.Write(pBuff,(int)File.GetLength() - m_BmpFileHeader.bfOffBits);
		File.Close();
		
		delete pBuff;
		ReleaseDC(pDC);

	}

}
