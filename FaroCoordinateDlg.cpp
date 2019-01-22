// FaroCoordinateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FaroCoordinateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaroCoordinateDlg dialog


CFaroCoordinateDlg::CFaroCoordinateDlg(CWnd* pParent,CWnd** pDlg)
	: CDialog(CFaroCoordinateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFaroCoordinateDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pFont = NULL;
	m_pArray = NULL;
	m_nCoordinateAddedCountDown = 0;
}


void CFaroCoordinateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFaroCoordinateDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFaroCoordinateDlg, CDialog)
	//{{AFX_MSG_MAP(CFaroCoordinateDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FAROCOORDS_RIGHTJUSTIFY, OnFarocoordsRightjustify)
	ON_COMMAND(ID_FAROCOORDS_LEFTJUSTIFY, OnFarocoordsLeftjustify)
	ON_COMMAND(ID_FAROCOORDS_TEXTCOLOUR, OnFarocoordsTextcolour)
	ON_COMMAND(ID_FAROCOORDS_BACKGROUNDCOLOUR, OnFarocoordsBackgroundcolour)
	ON_COMMAND(ID_BUTTON_POLAR, OnButtonPolar)
	ON_COMMAND(ID_BUTTON_RECTANGLE, OnButtonRectangle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaroCoordinateDlg message handlers

void CFaroCoordinateDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CFaroCoordinateDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CFaroCoordinateDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CFaroCoordinateDlg::OnDestroy() 
{
	SAFE_DELETE( m_pFont );
	SAFE_DELETE( m_pArray );

	if (CFaroCoordinateDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CFaroCoordinateDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CFaroCoordinateDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	
	GetClientRect(m_rrClient);

	m_StaticView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	if (CFaroCoordinateDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CFaroCoordinateDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		}
	}

	m_StaticView.SetNewPos();

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	

	SendMessage(WM_SIZE);

	SetTimer(1,200,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFaroCoordinateDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticView.SetNewPos();
	CRect rr;
	LOGFONT	lf;
	float fScale;

	if(GetSafeHwnd()) {
		m_staticView.GetWindowRect(rr);
		lf = m_LogFont;
		fScale = (float)rr.Height() / (float)m_rrClient.Height();
		lf.lfHeight = (int)((float)rr.Height() / -4.8f);
		theApp.SetFontLanguage(&lf);
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);
		Invalidate(false);
	}
	
}

void CFaroCoordinateDlg::OnTimer(UINT nIDEvent) 
{

	Invalidate(false);


	CDialog::OnTimer(nIDEvent);
}

void CFaroCoordinateDlg::OnPaint() 
{
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	CRect		rr,rect,rrStore;
	CDC			dcMem;
	CBitmap		bmp;
	static	int	nWidth;
	static	int	nHeight;
	static int nLoop=0;
	CString Label[4],Value[4],Name;
	int nNumberLines;
	CSize size;
	D3DXVECTOR3 vPos,vAZD;
	int nStatus,nRow;
	float fAzimuth,fZenith,fRadius;
	
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
//	rr.DeflateRect(1,1);
	if((m_pArray==NULL) || (rr.Width()-nWidth) || (rr.Height()-nHeight)) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		nWidth=rr.Width();
		nHeight=rr.Height();
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width()-2;
	rect.bottom = rr.Height()-2;

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(m_pFont);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbFaroText);
	dcMem.SetBkMode(TRANSPARENT);

	dcMem.FillRect(&rect,&CBrush(theApp.m_LastSettings.rgbFaroBk));


	nStatus = theApp.m_FaroWorkerThread.GetRawXYZCoordinate(&vPos);

	if(theApp.m_LastSettings.nFaroDisplayRectangularPolar != 1) {
		switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
		default:	Name.LoadString(IDS_World_coordinates);
			break;
		case 1:	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPos, &vPos);
				Name.LoadString(IDS_Frame_coordinates);
			break;
		case 2:	theApp.m_FaroWorkerThread.GetToolCoordinate(&vPos, &vPos);
				Name.LoadString(IDS_Tool_coordinates);
			break;
		}

		nNumberLines = 4;
		Label[0].Format(L"X: ");
		Label[1].Format(L"Y: ");
		Label[2].Format(L"Z: ");
		Label[3].Format(L"^: ");

		if(nStatus == 1) {
			Value[0].Format(L"%.03f mm",vPos.x);
			Value[1].Format(L"%.03f mm",vPos.y);
			Value[2].Format(L"%.03f mm",vPos.z);
			Value[3].Format(L"%.03f mm",D3DXVec3Length(&vPos));
		} else {
			Value[0].Format(L"***** mm");
			Value[1].Format(L"***** mm");
			Value[2].Format(L"***** mm");
			Value[3].Format(L"***** mm");
		}

	} else {

		switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
		default: theApp.m_FaroWorkerThread.GetAZDCoordinate(&fAzimuth,&fZenith,&fRadius);
			 Name.LoadString(IDS_World_coordinates);
			nNumberLines = 3;
			Label[0].Format(L"Azimuth: ");
			Label[1].Format(L"Zenith: ");
			Label[2].Format(L"Radius: ");
			if(nStatus == 0) {
				Value[0].Format(L"%.03f %s",fAzimuth * RAD_TO_DEG,DEGREES);
				Value[1].Format(L"%.03f %s",fZenith * RAD_TO_DEG,DEGREES);
				Value[2].Format(L"%.03f m",fRadius);
			} else {
				Value[0].Format(L"***** %s",DEGREES);
				Value[1].Format(L"***** %s",DEGREES);
				Value[2].Format(L"***** m");
			}
			break;
		case 1:
			theApp.m_FaroWorkerThread.GetFrameRotateAngles(&vPos, &vPos);
			Name.LoadString(IDS_Frame_coordinates);
			nNumberLines = 3;
			Label[0].Format(L"Rot X: ");
			Label[1].Format(L"Rot Y: ");
			Label[2].Format(L"Rot Z: ");
//			Label[3].Format(L"Radius: ");
			if(nStatus == 0) {
				Value[0].Format(L"%.03f %s",vPos.x * RAD_TO_DEG,DEGREES);
				Value[1].Format(L"%.03f %s",vPos.y * RAD_TO_DEG,DEGREES);
				Value[2].Format(L"%.03f %s",vPos.z * RAD_TO_DEG,DEGREES);
//				Value[3].Format(L"%.03f m",fRadius);
			} else {
				Value[0].Format(L"***** %s",DEGREES);
				Value[1].Format(L"***** %s",DEGREES);
				Value[2].Format(L"***** %s",DEGREES);
//				Value[3].Format(L"***** m");
			}
			break;
		case 2:
			theApp.m_FaroWorkerThread.GetToolRotateAngles(&vPos, &vPos);
			Name.LoadString(IDS_Tool_coordinates);
			nNumberLines = 3;
			Label[0].Format(L"Rot X: ");
			Label[1].Format(L"Rot Y: ");
			Label[2].Format(L"Rot Z: ");
//			Label[3].Format(L"Radius: ");
			if(nStatus == 0) {
				Value[0].Format(L"%.03f %s",vPos.x * RAD_TO_DEG,DEGREES);
				Value[1].Format(L"%.03f %s",vPos.y * RAD_TO_DEG,DEGREES);
				Value[2].Format(L"%.03f %s",vPos.z * RAD_TO_DEG,DEGREES);
//				Value[3].Format(L"%.03f m",fRadius);
			} else {
				Value[0].Format(L"***** %s",DEGREES);
				Value[1].Format(L"***** %s",DEGREES);
				Value[2].Format(L"***** %s",DEGREES);
//				Value[3].Format(L"***** m");
			}
			break;
		}



	}
	SetWindowText(Name);

	size = dcMem.GetTextExtent(Label[0]);
	for(nRow = 0;nRow<nNumberLines;nRow++) {
		dcMem.TextOut(size.cy/3,size.cy * nRow,Label[nRow]);
	}

	if(theApp.m_LastSettings.nFaroDisplayJustify==0) {
		for(nRow = 0;nRow<nNumberLines;nRow++) {
			dcMem.TextOut(size.cy/3 + size.cx,size.cy * nRow,Value[nRow]);
		}
	} else {
		for(nRow = 0;nRow<nNumberLines;nRow++) {
			size = dcMem.GetTextExtent(Value[nRow]);
			dcMem.TextOut(rect.right - size.cy/3 - size.cx,size.cy * nRow,Value[nRow]);
		}
	}

	if(m_nCoordinateAddedCountDown > 0) {
		rrStore = rect;
		rrStore.right = size.cy/3 - 3;
		dcMem.FillRect(&rrStore,&CBrush(RGB(0,255,0)));
		m_nCoordinateAddedCountDown--;
	}


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
	
}

void CFaroCoordinateDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CRect rr;

	m_staticView.GetWindowRect(rr);
	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB_FARO_DLG_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

		}
	}

	delete menu;
}

void CFaroCoordinateDlg::OnFarocoordsRightjustify() 
{
	theApp.m_LastSettings.nFaroDisplayJustify = 1;
}

void CFaroCoordinateDlg::OnFarocoordsLeftjustify() 
{
	theApp.m_LastSettings.nFaroDisplayJustify = 0;
}

void CFaroCoordinateDlg::OnFarocoordsTextcolour() 
{
	CMFCColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbFaroText=dlg.GetColor();
	};
	
}

void CFaroCoordinateDlg::OnFarocoordsBackgroundcolour() 
{
	CMFCColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbFaroBk=dlg.GetColor();
	};
}

void CFaroCoordinateDlg::OnButtonPolar() 
{
	theApp.m_LastSettings.nFaroDisplayRectangularPolar = 1;
}

void CFaroCoordinateDlg::OnButtonRectangle() 
{
	theApp.m_LastSettings.nFaroDisplayRectangularPolar = 0;
}
