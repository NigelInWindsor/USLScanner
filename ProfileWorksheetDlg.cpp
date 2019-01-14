// ProfileWorksheetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ProfileWorksheetDlg.h"
#include "WorksheetData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileWorksheetDlg dialog


CProfileWorksheetDlg::CProfileWorksheetDlg(CWnd* pParent,CWnd** pDlg)
	: CDialog(CProfileWorksheetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfileWorksheetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDataDlg = NULL;
	m_pDlg = pDlg;


}


void CProfileWorksheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileWorksheetDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileWorksheetDlg, CDialog)
	//{{AFX_MSG_MAP(CProfileWorksheetDlg)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_DATA, OnEditData)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileWorksheetDlg message handlers


BOOL CProfileWorksheetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	m_StaticPosView.SetNewPos();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int CProfileWorksheetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	CMenu* menu = (CMenu *) new CMenu;
	menu->LoadMenu(IDR_WORKSHEET_MENU);
	SetMenu(menu);
	
	
	return 0;
}

//Jpeg Stuff
BOOL CProfileWorksheetDlg::Render(CDC* pDC, CRect rc, LPCRECT prcMFBounds) const
{
	ASSERT(pDC);
	int nRwidth;
	int nOffset;

	if (rc.IsRectNull()) {
		CSize sz = GetImageSize(pDC);
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	long hmWidth,hmHeight; // HIMETRIC units
	GetHIMETRICSize(hmWidth, hmHeight);

	nRwidth= rc.Width();
	rc.right = (rc.Height()*( ((double)hmWidth/(double)hmHeight)))+rc.left;

	nOffset = (nRwidth-rc.Width())/2;
	rc.right+=nOffset;
	rc.left+=nOffset;
	
	m_spIPicture->Render(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
		0, hmHeight, hmWidth, -hmHeight, prcMFBounds);

	return TRUE;
}


CSize CProfileWorksheetDlg::GetImageSize(CDC* pDC) const
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

BOOL CProfileWorksheetDlg::Load(CFile& file)
{
	CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	return Load(ar);
}

//////////////////
// Load from archive--create stream and load from stream.
//
BOOL CProfileWorksheetDlg::Load(CArchive& ar)
{
	CArchiveStream arcstream(&ar);
	return Load((IStream*)&arcstream);
}

//////////////////
// Load from stream (IStream). This is the one that really does it: call
// OleLoadPicture to do the work.
//
BOOL CProfileWorksheetDlg::Load(IStream* pstm)
{
	Free();
	HRESULT hr = OleLoadPicture(pstm, 0, FALSE,
		IID_IPicture, (void**)&m_spIPicture);
	ASSERT(SUCCEEDED(hr) && m_spIPicture);	
	return TRUE;
}

void CProfileWorksheetDlg::OnEditData()
{

	if(m_pDataDlg==NULL) {
		m_pDataDlg = new CWorksheetData(this,(CWnd**)&m_pDataDlg);
		m_pDataDlg->Create(CWorksheetData::IDD,this);
		m_pDataDlg->ShowWindow(1);
	}
}

void CProfileWorksheetDlg::OnFilePrint()
{

	CPrintDialog* pPrintDlg;
	HDC hdcPrinter;
	CDC dcPrinter;
	CRect rectPaper,rectPrintArea;
	CRect rr,rc;


	//Get printer
	pPrintDlg = new CPrintDialog(FALSE);
	pPrintDlg->GetDefaults();
	hdcPrinter = pPrintDlg->GetPrinterDC();
	
	if(hdcPrinter == NULL) {
		MessageBox(_T("No printer Drivers installed"));
		delete pPrintDlg;
		pPrintDlg = NULL;
	} else {
		dcPrinter.Attach(hdcPrinter);
	}

	CPaintDC* pDC = (CPaintDC*)&dcPrinter;

	DOCINFO docinfo;
	memset(&docinfo,0,sizeof docinfo);
	docinfo.cbSize = sizeof(docinfo);
	docinfo.lpszDocName = _T("USL Scanner Worksheet");


	if(dcPrinter.StartDoc(&docinfo) < 0) {
		MessageBox(_T("Printer wouldn't initialize"));
		return;
	}


	if(dcPrinter.StartPage() < 0) {
		MessageBox(_T("Could not start page"));
		dcPrinter.AbortDoc();
		return;
	}

	rectPrintArea.SetRectEmpty();
	rectPrintArea.left = dcPrinter.GetDeviceCaps(PHYSICALOFFSETX);
	rectPrintArea.top = dcPrinter.GetDeviceCaps(PHYSICALOFFSETY);
	rectPrintArea.right = rectPrintArea.left + dcPrinter.GetDeviceCaps(HORZRES);
	rectPrintArea.bottom = rectPrintArea.top + dcPrinter.GetDeviceCaps(VERTRES);

	CWaitCursor Wait;

	rr = rc = rectPrintArea;

	rc.top = rr.top + (rr.Height()/(1.5));
	rr.bottom = rc.top;

	DrawText(pDC, rr);
	DrawImage(pDC, rc);

	dcPrinter.EndPage();
	dcPrinter.EndDoc();

	Wait.Restore();
}

void CProfileWorksheetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr,rc;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);

	CBrush brushBk(RGB(255,255,255));
	dc.SelectObject(&brushBk);
	dc.Rectangle(rr.left,rr.top,rr.right,rr.bottom);

	rc=rr;
	rc.top = rr.top + (rr.Height()/(1.5));
	rr.bottom = rc.top;

	DrawText(&dc, rr);
	DrawImage(&dc, rc);
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CProfileWorksheetDlg::DrawImage(CDC *pDC, CRect rr)
{
	CFile file;
	bool res;

	rr.DeflateRect(10,10,10,10);

	if(file.Open(ImageFilename,CFile::modeRead)){
		Load(file);			
		Render(pDC,rr,NULL);
	}

}


void CProfileWorksheetDlg::DrawText(CDC *pDC, CRect rr)
{
	APP App = (APP) AfxGetApp();
	CPoint pt;
	int ii;
	CString Buff;
	CSize size;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;

	rr.DeflateRect(10,10,10,10);
	
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = -MulDiv(12, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	_tcscpy(lf.lfFaceName, "Times");
	pFont = (CFont *) new CFont;
	pFont->CreateFontIndirect(&lf);
	pOldFont = pDC->SelectObject(pFont);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);
	size = pDC->GetTextExtent("A");

	pDC->DrawText("USL Program Sheet",rr,DT_CENTER );
	
	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Project",rr,DT_LEFT );
	pDC->DrawText(m_Project,rr,DT_CENTER );
	
	rr.top += size.cy;
	pDC->DrawText("Product No",rr,DT_LEFT );
	pDC->DrawText(m_ProductNum,rr,DT_CENTER );
	
	rr.top += size.cy;
	pDC->DrawText("Product",rr,DT_LEFT );
	pDC->DrawText(m_Product,rr,DT_CENTER );
	
	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Program No",rr,DT_LEFT );
	pDC->DrawText(PROFILE->m_FileName,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Date",rr,DT_LEFT );
	pDC->DrawText(m_Product,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Path",rr,DT_LEFT );
	pDC->DrawText(App->m_ProfilePath,rr,DT_CENTER );

	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Fixturing",rr,DT_CENTER );

	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Holding Fixture USL",rr,DT_LEFT );
	pDC->DrawText(m_HoldingFixture,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Carriage 1",rr,DT_LEFT );
	pDC->DrawText(m_Carriage1[0],rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText(m_Carriage1[1],rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText(m_Carriage1[2],rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Carriage 2",rr,DT_LEFT );
	pDC->DrawText(m_Carriage2[0],rr,DT_CENTER );
	
	rr.top += size.cy;
	pDC->DrawText(m_Carriage2[1],rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText(m_Carriage2[2],rr,DT_CENTER );

	
	rr.top += size.cy;
	pDC->DrawText("Note",rr,DT_LEFT );
	pDC->DrawText(m_FixtureNote,rr,DT_CENTER );

	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Probes and Squirters",rr,DT_CENTER );

	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Probe Frequency",rr,DT_LEFT );
	pDC->DrawText(m_ProbeFrequency,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Squirter length left",rr,DT_LEFT );
	pDC->DrawText(m_SqrtLenLeft,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Squirter opening left",rr,DT_LEFT );
	pDC->DrawText(m_SqrtOpenLeft,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Squirter length right",rr,DT_LEFT );
	pDC->DrawText(m_SqrtLenRight,rr,DT_CENTER );

	rr.top += size.cy;
	pDC->DrawText("Squirter opening right",rr,DT_LEFT );
	pDC->DrawText(m_SqrtOpenRight,rr,DT_CENTER );

	rr.top += size.cy;
	rr.top += size.cy;
	pDC->DrawText("Note",rr,DT_LEFT );
	pDC->DrawText(m_Note,rr,DT_CENTER );

	

}
	

void CProfileWorksheetDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();	

	Invalidate();
}

void CProfileWorksheetDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	DestroyWindow();
}

void CProfileWorksheetDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	*m_pDlg = NULL;
		
}
