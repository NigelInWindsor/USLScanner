// BootUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BootUpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBootUpDlg dialog


CBootUpDlg::CBootUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBootUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBootUpDlg)
	//}}AFX_DATA_INIT
	m_pImage = NULL;
	FindFile(&m_FileName);

	if(m_FileName.Find(_T(".jpg"))>0) {
		RetrieveJPGImage(m_FileName);
	} else {
		if(m_FileName.Find(_T(".bmp"))>0) {
			RetrieveBMPImage(m_FileName);
		}
	}
}


void CBootUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBootUpDlg)
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBootUpDlg, CDialog)
	//{{AFX_MSG_MAP(CBootUpDlg)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBootUpDlg message handlers
void CBootUpDlg::PostNcDestroy() 
{
	SAFE_DELETE( m_pImage );
	
	CDialog::PostNcDestroy();
}

void CBootUpDlg::OnClose() 
{
	SAFE_DELETE( m_pImage );
	
	CDialog::OnClose();
}

void CBootUpDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	SAFE_DELETE( m_pImage );
	
}

void CBootUpDlg::UpdatePrompt(CString Prompt)
{

	m_editPrompt.SetWindowText(Prompt);
	m_editPrompt.UpdateWindow();
	m_editPrompt.HideCaret();
}

BOOL CBootUpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editPrompt.SetWindowText(_T("Starting Program"));

	m_editPrompt.HideCaret();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBootUpDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);

	RenderTitle(&dc);

	if(m_FileName.Find(_T(".jpg"))>0) {
		RenderJPG(&dc,rr,NULL);
	} else {
		if(m_FileName.Find(_T(".bmp"))>0) {
			RenderBMP(&dc,rr);
		}
	}
}

void CBootUpDlg::RenderBMP(CDC* pDC,CRect rr)
{
	CBitmap bmp;
	CDC dcMem;

	bmp.CreateCompatibleBitmap(pDC,m_BmpHeader.biWidth, m_BmpHeader.biHeight);
	dcMem.CreateCompatibleDC(pDC);
	dcMem.SelectObject(&bmp);
	bmp.SetBitmapBits((DWORD)(m_BmpHeader.biWidth * m_BmpHeader.biHeight * 4),m_pImage);

	pDC->StretchBlt(rr.left,rr.top,rr.Width(),rr.Height(),
			&dcMem,0,0,m_BmpHeader.biWidth,m_BmpHeader.biHeight,SRCCOPY);
}


bool CBootUpDlg::FindFile(CString *pBuff)
{
	WIN32_FIND_DATA ffd;
	CString FilePath;
	HANDLE hFind;

	//Find a jpg
	FilePath.Format(_T("%s%s"),theApp.m_WorkingDirectory, _T("\\*.jpg"));

	hFind = FindFirstFile(FilePath, &ffd);
	if (hFind != INVALID_HANDLE_VALUE )
	{
		do{
			pBuff->Format(_T("%s\\%s"),theApp.m_WorkingDirectory,ffd.cFileName);
			return true;
		} while( FindNextFile(hFind, &ffd)!= 0 );
 
	}


	//otherwise find bmp
	FilePath.Format(_T("%s%s"),theApp.m_WorkingDirectory, _T("\\*.bmp"));

	hFind = FindFirstFile(FilePath, &ffd);
	if (hFind != INVALID_HANDLE_VALUE )
	{
		do{
			pBuff->Format(_T("%s\\%s"),theApp.m_WorkingDirectory,ffd.cFileName);
			return true;
		} while( FindNextFile(hFind, &ffd)!= 0 );
 
	}
	return false;
}

bool CBootUpDlg::RetrieveBMPImage(CString FileName)
{
	CUSLFile File;
	CFileException e;
	CString Buff;

	int nDestBytesPixel = 4;

	if(File.Open(FileName,CFile::typeBinary|CFile::modeReadWrite,&e)==0) {
		File.DecodeFileException(&e,&Buff,FileName);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
		return false;
	} else {
		File.Read(&m_BmpFileHeader,sizeof m_BmpFileHeader);
		File.Read(&m_BmpHeader,sizeof m_BmpHeader);

		int nSrcBytesPixel = m_BmpHeader.biBitCount / 8;
		char *pArray = new char[m_BmpHeader.biSizeImage];

		File.Seek(m_BmpFileHeader.bfOffBits,CFile::begin);
		File.Read(pArray,m_BmpHeader.biSizeImage);
		File.Close();

		int	nArraySize = m_BmpHeader.biWidth * m_BmpHeader.biHeight;

		m_pImage = new COLORREF[nArraySize];
		char* pDestArray = (char*)m_pImage;
		int nSrc=0;
		int nDest=0;
		for(int yy=0;yy<m_BmpHeader.biHeight;yy++) {
			nSrc = (m_BmpHeader.biHeight - yy - 1) * m_BmpHeader.biWidth * nSrcBytesPixel;
			for(int xx=0;xx<m_BmpHeader.biWidth;xx++) {

				pDestArray[nDest++]=pArray[nSrc++];
				pDestArray[nDest++]=pArray[nSrc++];
				pDestArray[nDest++]=pArray[nSrc++];
				pDestArray[nDest++]=(char)0xff;
				if((nSrcBytesPixel==4) && (nDestBytesPixel==4)) {
					pDestArray[nDest++]=pArray[nSrc++];
				}
				if((nSrcBytesPixel==3) && (nDestBytesPixel==4)) {
					pDestArray[nDest++]=0;
				}
			}
		}

		delete pArray;
		return true;
	}


}

BOOL CBootUpDlg::RetrieveJPGImage(CString FileName)
{
	CUSLFile File;
	CFileException e;
	CString Buff;

	if(File.Open(FileName,CFile::typeBinary|CFile::modeReadWrite,&e)==0) {
		File.DecodeFileException(&e,&Buff,FileName);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
		return false;
	} else {
		
		CArchive ar(&File, CArchive::load | CArchive::bNoFlushOnDelete);

		BOOL bFlag = Load(ar);

		File.Close();
		return bFlag;
	}
}

//////////////////
// Load from archive--create stream and load from stream.
//
BOOL CBootUpDlg::Load(CArchive& ar)
{
	CArchiveStream arcstream(&ar);
	return Load((IStream*)&arcstream);
}

//////////////////
// Load from stream (IStream). This is the one that really does it: call
// OleLoadPicture to do the work.
//
BOOL CBootUpDlg::Load(IStream* pstm)
{
	Free();
	HRESULT hr = OleLoadPicture(pstm, 0, FALSE,
		IID_IPicture, (void**)&m_spIPicture);
	ASSERT(SUCCEEDED(hr) && m_spIPicture);	
	return TRUE;
}

BOOL CBootUpDlg::RenderJPG(CDC* pDC, CRect rc, LPCRECT prcMFBounds) const
{
	ASSERT(pDC);

	if (rc.IsRectNull()) {
		CSize sz = GetImageSize(pDC);
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	long hmWidth,hmHeight; // HIMETRIC units
	GetHIMETRICSize(hmWidth, hmHeight);

	m_spIPicture->Render(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
		0, hmHeight, hmWidth, -hmHeight, prcMFBounds);

	return TRUE;
}


CSize CBootUpDlg::GetImageSize(CDC* pDC) const
{
	if (!m_spIPicture)
		return CSize(0,0);
	
	LONG hmWidth, hmHeight; // HIMETRIC units
	m_spIPicture->get_Width(&hmWidth);
	m_spIPicture->get_Height(&hmHeight);
	CSize sz(hmWidth,hmHeight);
	if (pDC==NULL) {
		CWindowDC dc(NULL);
		dc.HIMETRICtoDP(&sz); // convert to pixels
	} else {
		pDC->HIMETRICtoDP(&sz);
	}
	return sz;
}

void CBootUpDlg::RenderTitle(CDC *pDC)
{
	CRect rr;
	CString Buff;
	LOGFONT lf;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&lf);

	pFont = new CFont();
	lf.lfHeight=24;
	lf.lfWeight=4;
	pFont->CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(pFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(100,255,100));

	m_staticTitle.GetWindowRect(rr);
	ScreenToClient(rr);
	pDC->FillRect(rr,&CBrush(RGB(200,200,200)));

	Buff = _T("Ultrasonic Sciences Ltd (C)2012");
	CSize size = pDC->GetTextExtent(Buff);

	pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.CenterPoint().y-size.cy/2,Buff);

	pDC->SelectObject(pOldFont);
	delete pFont;
}
