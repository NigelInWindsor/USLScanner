// FlattenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FlattenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlattenDlg dialog


CFlattenDlg::CFlattenDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CFlattenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlattenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
}


void CFlattenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlattenDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlattenDlg, CDialog)
	//{{AFX_MSG_MAP(CFlattenDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_BUTTON_FLATTEN_FROM_LEFT, OnButtonFlattenFromLeft)
	ON_COMMAND(ID_BUTTON_FLATTEN_FROM_CENTRE, OnButtonFlattenFromCentre)
	ON_COMMAND(ID_BUTTON_FLATTEN_FROM_RIGHT, OnButtonFlattenFromRight)
	ON_COMMAND(ID_BUTTON_PRESS_FLAT, OnButtonPressFlat)
	ON_COMMAND(ID_BUTTON_FLATTEN_FROM_VERTICAL, OnButtonFlattenFromVertical)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlattenDlg message handlers
int CFlattenDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_FLATTEN_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	GetClientRect(rect);

	if (!m_wndRebar.Create(this,NULL,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,500,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolsToolBar);

	return 0;
}

void CFlattenDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}

void CFlattenDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CFlattenDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CFlattenDlg::OnDestroy() 
{
	if (CFlattenDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CFlattenDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CFlattenDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	if (CFlattenDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CFlattenDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		}
	}

	m_StaticPosView.SetNewPos();

	GenerateSamplePositionVertices();
	
	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlattenDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();
	if(GetSafeHwnd()) {
		Invalidate(FALSE);
	}
}


void CFlattenDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CBitmap bmp;
	CRect rr;
	COLORREF* m_pArray;

	m_staticView.GetWindowRect(&m_rrView);
	ScreenToClient(&m_rrView);
	m_rrView.DeflateRect(1,1);

	m_pArray = new COLORREF[(m_rrView.Width()+2) * (m_rrView.Height()+2)];

	bmp.CreateCompatibleBitmap(&dc,m_rrView.Width(),m_rrView.Height());
	bmp.SetBitmapBits((DWORD)(m_rrView.Width()*m_rrView.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	
	rr=m_rrView;
	rr.OffsetRect(-m_rrView.left,-m_rrView.top);
	dcMem.FillRect(&rr,&CBrush(RGB(255,255,255)));

	RenderSamplePosition(&dcMem);

	dc.BitBlt(m_rrView.left,m_rrView.top,m_rrView.Width(),m_rrView.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	SAFE_DELETE(m_pArray);
}

void CFlattenDlg::RenderSamplePosition(CDC *pDC)
{
	int nLine;
	CPoint pt[2];

	if(m_Data.m_vLineSample == NULL) return;

	CPen pen(PS_SOLID,1,RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&pen);

	for(nLine = 0;nLine<m_Data.m_nNumberLines;nLine++) {

		pt[0].x = MulDiv(m_Data.m_vLineSample[nLine].m_nFirstSample,m_rrView.Width(),m_Data.m_nSamplesLine);
		pt[0].y = m_rrView.Height() - MulDiv(nLine,m_rrView.Height(),m_Data.m_nNumberLines);

		pt[1].x = pt[0].x + MulDiv(m_Data.m_vLineSample[nLine].m_nCoordL,m_rrView.Width(),m_Data.m_nSamplesLine);
		pt[1].y = pt[0].y;

		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[1]);

	}

	pDC->SelectObject(pOldPen);
}

BOOL CFlattenDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText,strCtrlKey;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   strCtrlKey.LoadString(IDS_Ctrl_Key);
   switch(nID) {
   case ID_BUTTON_FLATTEN_FROM_LEFT: strTipText.LoadString(IDS_Flatten_from_left);
	   break;
   case ID_BUTTON_FLATTEN_FROM_CENTRE: strTipText.LoadString(IDS_Flatten_from_centre);
	   break;
   case ID_BUTTON_FLATTEN_FROM_RIGHT: strTipText.LoadString(IDS_Flatten_from_right);
	   break;
   case ID_BUTTON_PRESS_FLAT: strTipText.LoadString(IDS_Press_flat);
	   break;
   case ID_BUTTON_FLATTEN_FROM_VERTICAL: strTipText.LoadString(IDS_Flatten_from_vertical);
	   break;
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CFlattenDlg::OnButtonFlattenFromLeft() 
{
	PROFILE->m_nFlattenMode = 0;
	SetToolBarCheckedState();
	GenerateSamplePositionVertices();
	Invalidate(false);
}

void CFlattenDlg::OnButtonFlattenFromCentre() 
{
	PROFILE->m_nFlattenMode = 1;
	SetToolBarCheckedState();
	GenerateSamplePositionVertices();
	Invalidate(false);
}

void CFlattenDlg::OnButtonFlattenFromRight() 
{
	PROFILE->m_nFlattenMode = 2;
	SetToolBarCheckedState();
	GenerateSamplePositionVertices();
	Invalidate(false);
}

void CFlattenDlg::OnButtonPressFlat() 
{
	PROFILE->m_nFlattenMode = 3;
	SetToolBarCheckedState();
	GenerateSamplePositionVertices();
	Invalidate(false);
}

void CFlattenDlg::OnButtonFlattenFromVertical() 
{
	PROFILE->m_nFlattenMode = 4;
	SetToolBarCheckedState();
	GenerateSamplePositionVertices();
	Invalidate(false);
}

void CFlattenDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_FLATTEN_FROM_LEFT))>=0)
		PROFILE->m_nFlattenMode==0 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_FLATTEN_FROM_CENTRE))>=0)
		PROFILE->m_nFlattenMode==1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_FLATTEN_FROM_RIGHT))>=0)
		PROFILE->m_nFlattenMode==2 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_PRESS_FLAT))>=0)
		PROFILE->m_nFlattenMode==3 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_FLATTEN_FROM_VERTICAL))>=0)
		PROFILE->m_nFlattenMode==4 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

}

bool CFlattenDlg::GenerateSamplePositionVertices()
{
	if(PROFILE->m_nScanLineL>1) {
		CWaitCursor Wait;
		m_Data.Zero();

		m_Data.m_fFastIncrement = PROFILE->m_fFastIncrement;
		m_Data.m_fSlowIncrement = PROFILE->m_fSlowIncrement * PROFILE->m_nLineIncrement;
		m_Data.m_nScanStartLine = PROFILE->m_nScanStartLine;
		m_Data.m_nScanFinishLine = PROFILE->m_nScanFinishLine;
		m_Data.m_nLineIncrement = PROFILE->m_nLineIncrement;
		m_Data.m_nLineIncrementMode = PROFILE->m_nLineIncrementMode;
		m_Data.m_nComponentDescription = PROFILE->m_nComponentDescription;
		m_Data.m_nScanType = PROFILE->m_nScanType = CONTOUR_SCAN;
		m_Data.m_nCollectAxis = PROFILE->m_nCollectAxis;
		m_Data.m_nFlattenMode = PROFILE->m_nFlattenMode;

		m_Data.m_nFastAxis = PROFILE->m_nFastAxis;
		m_Data.m_nSlowAxis = PROFILE->m_nSlowAxis;
		m_Data.m_nCollectScanLines = PROFILE->m_nCollectScanLines;
		m_Data.m_nCollectSurface = PROFILE->m_nCollectSurface;

		PROFILE->FindMaximumSamplesLine(&m_Data.m_nSamplesLine);
		PROFILE->FindNumberScanLines(&m_Data.m_nNumberLines);
		PROFILE->FindScanSizes(&m_Data.m_fFastScanSize,&m_Data.m_fSlowScanSize,&m_Data.m_CpStart,&m_Data.m_CpFinish);

		m_Data.GenerateSamplePositionVertices();

		Wait.Restore();
		return true;
	}

	return false;
}


