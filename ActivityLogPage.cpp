// ActivityLogPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ActivityLogPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActivityLogPage property page

IMPLEMENT_DYNCREATE(CActivityLogPage, CPropertyPage)

CActivityLogPage::CActivityLogPage() : CPropertyPage(CActivityLogPage::IDD)
{
	//{{AFX_DATA_INIT(CActivityLogPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_LastWriteTime = 0;
	m_nTopRow = 0;
	m_nRowsL = 20;

}

CActivityLogPage::~CActivityLogPage()
{
}

void CActivityLogPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActivityLogPage)
	DDX_Control(pDX, IDC_SCROLLBAR_VERT, m_scrollVert);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActivityLogPage, CPropertyPage)
	//{{AFX_MSG_MAP(CActivityLogPage)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActivityLogPage message handlers

BOOL CActivityLogPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CActivityLogPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	RetrieveFile(theApp.m_LastSettingsPath + L"\\ActivityLog.txt");

	return CPropertyPage::OnSetActive();
}

BOOL CActivityLogPage::OnKillActive() 
{
	
	return CPropertyPage::OnKillActive();
}


bool CActivityLogPage::RetrieveFile(CString FileName)
{
	CUSLFile file;
	CFileException e;
	CString	Buff,Language;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary,&e)) {
		file.DecodeFileException(&e,&Buff,FileName);
		Language.LoadString(IDS_Error);
		MessageBox(Buff,Language,MB_ICONERROR);
		return FALSE;
	} else {
		m_Str.RemoveAll();

		m_nIndex = 0;
		m_nTopRow = 0;

		CArchive ar(&file,CArchive::load);
		while(ar.ReadString(Buff)) {
			m_Str.Add(Buff);
		}
		ar.Close();

		m_nLength = m_Str.GetSize();
		return TRUE;
	}
	return TRUE;
}

void CActivityLogPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CBitmap bmp;
	CRect rrStatic;
	CString Buff;
	CFont Font;
	CRect rrBK;

	LOGFONT	lf = theApp.m_LastSettings.lfViewGraticule;
	lf.lfEscapement=0;
	Font.CreateFontIndirect(&lf);

	m_staticView.GetWindowRect(&rrStatic);
	ScreenToClient(&rrStatic);
	rrStatic.DeflateRect(1,1);
	COLORREF	*pArray = new COLORREF[(rrStatic.Width()+2) * (rrStatic.Height()+2)];

	bmp.CreateCompatibleBitmap(&dc,rrStatic.Width(),rrStatic.Height());
	bmp.SetBitmapBits((DWORD)(rrStatic.Width()*rrStatic.Height()*4),pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	
	CRect rr = rrStatic;
	rr.OffsetRect(-rrStatic.left,-rrStatic.top);
	dcMem.FillRect(&rr,&CBrush(RGB(255,255,255)));
	CFont* pOldFont = dcMem.SelectObject(&Font);

	dcMem.SetTextColor(RGB(0,0,0));

	Buff = "Hello";
	m_size = dcMem.GetTextExtent(Buff);
	m_nRowsL = rr.Height() / m_size.cy;

	dcMem.SetBkMode(TRANSPARENT);
	for(int nRow=0;nRow<m_nRowsL && nRow<m_nLength;nRow++) {
		rrBK = rr;
		rrBK.top = nRow*m_size.cy;
		rrBK.bottom = rrBK.top + m_size.cy;

		if((nRow + m_nTopRow) < m_nLength) {
			dcMem.TextOut(1,nRow*m_size.cy,m_Str.GetAt(nRow + m_nTopRow));
		}
	}

	dc.BitBlt(rrStatic.left,rrStatic.top,rrStatic.Width(),rrStatic.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;
	

}

void CActivityLogPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode) {
	case SB_LINEDOWN:
		m_nTopRow+=(m_nRowsL/10);
		break;
	case SB_LINEUP:
		 m_nTopRow-=(m_nRowsL/10);
		break;
	case SB_PAGEDOWN:
		m_nTopRow+=m_nRowsL;
		break;
	case SB_PAGEUP:
		m_nTopRow-=m_nRowsL;
		break;
	case SB_THUMBTRACK:
		m_nTopRow = (int)m_scrollVert.GetTrackPos();
		break;
	default:
		break;
	}
	if(m_nTopRow+m_nRowsL > m_nLength) {
		m_nTopRow = m_nLength-m_nRowsL;
	}
	if(m_nTopRow < 0) m_nTopRow = 0;

	m_scrollVert.SetScrollRange(0.0,(double)m_nLength,FALSE);
	m_scrollVert.SetPageSize((double)m_nRowsL,FALSE);
	m_scrollVert.SetScrollPos((double)m_nTopRow, TRUE);

	Invalidate(FALSE);
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CActivityLogPage::PreTranslateMessage(MSG* pMsg) 
{

	switch(pMsg->message) {
	case WM_MOUSEWHEEL:
		switch(pMsg->wParam) {
		case 0x00780000: m_nTopRow--;
			break;
		case 0xff880000: m_nTopRow++;
			break;
		}
		if(m_nTopRow+m_nRowsL > m_nLength) {
			m_nTopRow = m_nLength-m_nRowsL;
		}
		if(m_nTopRow < 0) m_nTopRow = 0;
		Invalidate(FALSE);
		break;
	
	}
	
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CActivityLogPage::MouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	if(zDelta>0) m_nTopRow--;
	if(zDelta<0) m_nTopRow++;

	if(m_nTopRow+m_nRowsL > m_nLength) {
		m_nTopRow = m_nLength-m_nRowsL;
	}
	if(m_nTopRow < 0) m_nTopRow = 0;
	Invalidate(FALSE);

}