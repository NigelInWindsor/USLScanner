// DetailsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DetailsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_EDIT		40000
#define	ID_BUTTON	40200

#define	TOP_EDGE	1
#define	BOTTOM_EDGE	2
#define	LEFT_EDGE	4
#define	RIGHT_EDGE	8

/////////////////////////////////////////////////////////////////////////////
// CDetailsPage property page

IMPLEMENT_DYNCREATE(CDetailsPage, CPropertyPage)

CDetailsPage::CDetailsPage(CWnd** pDlg,CData* pData,bool bDesignDlg) : CPropertyPage(CDetailsPage::IDD)
{
	//{{AFX_DATA_INIT(CDetailsPage)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pData = pData;
	

	m_pStatic = NULL;
	m_pEdit = NULL;
	m_pButton = NULL;

	m_bDesignDlg = bDesignDlg;
	m_nIndex = -1;
	m_hcursorHMove = AfxGetApp()->LoadCursor(IDC_CURSOR_H_MOVE);
	m_hcursorVMove = AfxGetApp()->LoadCursor(IDC_CURSOR_V_MOVE);
	m_hcursorAllMove = AfxGetApp()->LoadCursor(IDC_CURSOR_ALL_MOVE);
	m_bLButtonDown = FALSE;
	m_bCtrlMoved = FALSE;
	m_nWhichEdge=0;
	m_pCtrlPropertiesDlg = NULL;
	m_bTabOrder = FALSE;
	m_nTabNumber = 0;
	m_bShiftKeyDown = FALSE;
}

CDetailsPage::~CDetailsPage()
{
}

void CDetailsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDetailsPage)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsPage message handlers

void CDetailsPage::PostNcDestroy() 
{
	SAFE_DELETE_ARRAY( m_pStatic );
	SAFE_DELETE_ARRAY( m_pEdit );
	SAFE_DELETE_ARRAY( m_pButton );
	
	CPropertyPage::PostNcDestroy();
	delete this;
}

void CDetailsPage::OnClose() 
{
	CDialog::OnClose();
	DestroyWindow();
}

void CDetailsPage::OnDestroy() 
{
	if (CDetailsPage::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CDetailsPage::IDD]);

	CDialog::OnDestroy();
	
	if(*m_pDlg) *m_pDlg	= NULL;	
	
}

int CDetailsPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

BOOL CDetailsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if (CDetailsPage::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CDetailsPage::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	CDC* pDC = GetDC();
	DisplayControls(pDC);
	ReleaseDC(pDC);

	
	SetWindowText(theApp.m_DetailsWindowName);
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetailsPage::OnPaint() 
{
	CSize size;
	CRect rr;
	CString Buff;
	CFont *pFont = GetFont();

	CPaintDC dc(this); // device context for painting
	CFont* pOldFont = dc.SelectObject(pFont);

	int nTextL=0;
	int	nEditL=0;
	int	nButtonL=0;
	int nStyle,nExStyle;


	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {

		switch(theApp.m_pDescription[nn].nCtrlType) {
		case 0:
			break;
		case TEXT_CTL:
			if(m_pStatic[nTextL].m_hWnd==NULL) {
				m_pStatic[nTextL].Create(*theApp.m_pDescription[nn].pCaption,WS_CHILD|WS_VISIBLE,theApp.m_pDescription[nn].rrCtrl,this);
				m_pStatic[nTextL].SetFont(pFont);
			}

			m_pStatic[nTextL].SetWindowText(*theApp.m_pDescription[nn].pCaption);
			size = dc.GetTextExtent(*theApp.m_pDescription[nn].pCaption);
			theApp.m_pDescription[nn].rrCtrl.right = theApp.m_pDescription[nn].rrCtrl.left + size.cx;
			theApp.m_pDescription[nn].rrCtrl.bottom = theApp.m_pDescription[nn].rrCtrl.top + size.cy;
			m_pStatic[nTextL].MoveWindow(theApp.m_pDescription[nn].rrCtrl.left,theApp.m_pDescription[nn].rrCtrl.top,size.cx,size.cy,TRUE);
			nTextL++;
			break;

		case EDIT_CTL:
			if(m_pEdit[nEditL].m_hWnd==NULL) {
				m_pEdit[nEditL].CreateEx( theApp.m_pDescription[nn].nExStyle,_T("EDIT"),*theApp.m_pDescription[nn].pCaption,WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | theApp.m_pDescription[nn].nStyle,
					theApp.m_pDescription[nn].rrCtrl, this, ID_EDIT+nEditL);
				m_pEdit[nEditL].SetFont(pFont);
			}
			nStyle = m_pEdit[nEditL].GetStyle();
			nExStyle = m_pEdit[nEditL].GetExStyle();

			if((nStyle - (WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | theApp.m_pDescription[nn].nStyle)) || (nExStyle - theApp.m_pDescription[nn].nExStyle)) {
				m_pEdit[nEditL].DestroyWindow();
				m_pEdit[nEditL].CreateEx( theApp.m_pDescription[nn].nExStyle,_T("EDIT"),*theApp.m_pDescription[nn].pCaption,WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | theApp.m_pDescription[nn].nStyle,
					theApp.m_pDescription[nn].rrCtrl, this, ID_EDIT+nEditL);
				m_pEdit[nEditL].SetFont(pFont);
			}

			m_pEdit[nEditL].MoveWindow(theApp.m_pDescription[nn].rrCtrl.left,theApp.m_pDescription[nn].rrCtrl.top,
				theApp.m_pDescription[nn].rrCtrl.Width(),theApp.m_pDescription[nn].rrCtrl.Height(),TRUE);
			nEditL++;
			break;

		case BUTTON_CTL:
			if(m_pButton[nButtonL].m_hWnd==NULL) {
				m_pButton[nButtonL].Create(*theApp.m_pDescription[nn].pCaption,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON | theApp.m_pDescription[nn].nStyle,
					theApp.m_pDescription[nn].rrCtrl, this, ID_BUTTON+nButtonL);
				m_pButton[nButtonL].SetFont(pFont);
			}
			nStyle = m_pButton[nButtonL].GetStyle();
			if(nStyle - (WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON | theApp.m_pDescription[nn].nStyle) ) {
				m_pButton[nButtonL].DestroyWindow();
				m_pButton[nButtonL].Create(*theApp.m_pDescription[nn].pCaption,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON | theApp.m_pDescription[nn].nStyle,
					theApp.m_pDescription[nn].rrCtrl, this, ID_BUTTON+nButtonL);
				m_pButton[nButtonL].SetFont(pFont);
			}


			m_pButton[nButtonL].SetWindowText(*theApp.m_pDescription[nn].pCaption);
			m_pButton[nButtonL].MoveWindow(theApp.m_pDescription[nn].rrCtrl.left,theApp.m_pDescription[nn].rrCtrl.top,
				theApp.m_pDescription[nn].rrCtrl.Width(),theApp.m_pDescription[nn].rrCtrl.Height(),TRUE);
			nButtonL++;
			break;
		}
		if(m_bTabOrder == TRUE) {
			Buff.Format(_T("%d"),nn+1);
			size = dc.GetTextExtent(Buff);
			rr.left = theApp.m_pDescription[nn].rrCtrl.left-size.cx;
			rr.right = theApp.m_pDescription[nn].rrCtrl.left;
			rr.top = theApp.m_pDescription[nn].rrCtrl.top-size.cy;
			rr.bottom = theApp.m_pDescription[nn].rrCtrl.top;
	
			dc.FillRect(rr,&CBrush(RGB(0,255,0)));

			int nOldMode = dc.SetBkMode(TRANSPARENT);
			dc.TextOut(rr.left,rr.top,Buff);
			dc.SetBkMode(nOldMode);
		}
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			rr = theApp.m_pDescription[nn].rrCtrl;
			rr.InflateRect(4,4);
			DrawCtrlHighLight(&dc,rr,RGB(180,180,255));
		}


	}
	dc.SelectObject(pOldFont);

	if((m_bDesignDlg == TRUE) && (m_nIndex >= 0) && (m_bTabOrder == FALSE)) {
		DrawCtrlHighLight(&dc,m_rrIndex,RGB(110,110,255));
	}
	if((m_bDesignDlg == TRUE) && (m_bLButtonDown) && (m_bTabOrder == FALSE)) {
		DrawDrag(&dc);
	}
	UpdateAllControls();
}

void CDetailsPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CDetailsPage::UpdateAllControls()
{
	int nEditL=0;
	int nButtonL=0;
	int nTextL=0;
	CString Buff;
	int ll;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		switch(theApp.m_pDescription[nn].nCtrlType) {
		case TEXT_CTL:
			nTextL++;
			break;
		case EDIT_CTL:
			if(m_pEdit[nEditL].m_hWnd) {
				if((theApp.m_pDescription[nn].nActionFlag & CTL_FILENAME) && !(theApp.m_pDescription[nn].nActionFlag & CTL_FULLPATHNAME)) {
					Buff = m_pData->m_Details[nEditL];
					if((ll=Buff.ReverseFind('\\')) > -1)
						Buff = Buff.Right(Buff.GetLength() - ll-1);
					if((ll=Buff.Find('.')) > -1)
						Buff = Buff.Left(ll);
					m_pEdit[nEditL].SetWindowText(Buff);
				} else {
					m_pEdit[nEditL].SetWindowText(m_pData->m_Details[nEditL]);
				}
			}
			nEditL++;
			break;
		case BUTTON_CTL:
			nButtonL++;
			break;
		}
	}
}




void CDetailsPage::DisplayControls(CDC *pDC)
{
	SAFE_DELETE_ARRAY(m_pStatic);
	SAFE_DELETE_ARRAY(m_pEdit);
	SAFE_DELETE_ARRAY(m_pButton);

	m_pStatic = new CStatic[100];
	m_pEdit = new CEdit[100];
	m_pButton = new CButton[100];
}




BOOL CDetailsPage::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CString Buff;
	int nn;
	int nButtonL=0;

	switch(nCode) {
	case 0x400:	//Text entered
		GetDlgItemText(nID,Buff);
		if((nID>=ID_EDIT) && (nID<(ID_EDIT + 100))) {
			m_pData->m_Details[nID-ID_EDIT]=Buff;
			return TRUE;
		}
		break;
	case 0x000: //Button pressed and released
		if((nID>=ID_BUTTON) && (nID<(ID_BUTTON + 100))) {

			for(nn=0;nn<theApp.m_nDescriptionL;nn++) {
				if(theApp.m_pDescription[nn].nCtrlType == BUTTON_CTL) {
					if(nButtonL == (nID - ID_BUTTON)) {
						m_pData->ClearDetails(theApp.m_pDescription[nn].nActionFlag);
						m_pData->FillAutomaticDetails(theApp.m_pDescription[nn].nActionFlag);
						UpdateAllControls();
						return TRUE;
					}
					nButtonL++;
				}
			}

			return TRUE;
		}
		break;

	}
	
	return CPropertyPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDetailsPage::Zero()
{
	COleDateTime CurrentTime = COleDateTime::GetCurrentTime();

	TCHAR sLongDate[255];
	TCHAR sShortDate[255];

    SYSTEMTIME st;

    st.wYear = (WORD)CurrentTime.GetYear();
    st.wMonth = (WORD)CurrentTime.GetMonth();
    st.wDayOfWeek = 0;
    st.wDay = (WORD)CurrentTime.GetDay();
    st.wHour = 0;
    st.wMinute = 0;
    st.wSecond = 0;
    st.wMilliseconds = 0;

    GetDateFormat(LOCALE_USER_DEFAULT,DATE_LONGDATE,&st,NULL,sLongDate,254);
    GetDateFormat(LOCALE_USER_DEFAULT,DATE_SHORTDATE,&st,NULL,sShortDate,254);


	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		m_pData->m_Details[nn].Empty();
/*
		switch(theApp.m_pDescription[nn].nAutomaticValue) {
		case A_LONGDATE:		m_pData->m_Details[nn].Format(_T("%s"),sLongDate);
			break;
		case A_LONGDATE_TIME:	m_pData->m_Details[nn].Format(_T("%s %s"),sLongDate,CurrentTime.Format(VAR_TIMEVALUEONLY));
			break;
		case A_SHORTDATE:		m_pData->m_Details[nn].Format(_T("%s"),sShortDate);
			break;
		case A_SHORTDATE_TIME:	m_pData->m_Details[nn].Format(_T("%s %s"),sShortDate,CurrentTime.Format(VAR_TIMEVALUEONLY));
			break;
		case A_OPERATOR:		m_pData->m_Details[nn] = theApp.m_Operator;
			break;
		}
		*/
	}
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Edit the design of the dialog
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CDetailsPage::PreTranslateMessage(MSG* pMsg) 
{
	CPoint pt;
	CString Buff;
	CSize Size;
	TCHAR ch;
	int nn;

	if(m_bDesignDlg == TRUE) {
		CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

		switch(pMsg->message) {
		case WM_KEYDOWN: //keyboa
			ch = TCHAR(pMsg->wParam);
			if(ch == 0x10) {
				m_bShiftKeyDown ^= TRUE;
				if(m_bShiftKeyDown == FALSE) {
					for(nn=0;nn<theApp.m_nDescriptionL;nn++) {
						theApp.m_pDescription[nn].nActionFlag &= ~CTL_HIGHLIGHT;
					}
					Invalidate(TRUE);
				}
			}
//			return TRUE;
			break;
//		case WM_KEYUP:
//			m_bShiftKeyDown = FALSE;
//			return TRUE;
//			break;

		case 0x200: // Mouse move
			switch(pMsg->wParam) {
			case 1:
				if(m_bLButtonDown == TRUE) {
					pt = pMsg->pt;
					ScreenToClient(&pt);
					
					if(m_nWhichEdge & LEFT_EDGE) m_rrDrag.left = pt.x  - (m_ptButtonDown.x - theApp.m_pDescription[m_nIndex].rrCtrl.left);
					if(m_nWhichEdge & RIGHT_EDGE) m_rrDrag.right = pt.x  - (m_ptButtonDown.x - theApp.m_pDescription[m_nIndex].rrCtrl.right);
					if(m_nWhichEdge & TOP_EDGE) m_rrDrag.top = pt.y - (m_ptButtonDown.y - theApp.m_pDescription[m_nIndex].rrCtrl.top);
					if(m_nWhichEdge & BOTTOM_EDGE) m_rrDrag.bottom = pt.y - (m_ptButtonDown.y - theApp.m_pDescription[m_nIndex].rrCtrl.bottom);

					m_bCtrlMoved = TRUE;
					Invalidate(TRUE);
					return TRUE;
				}
				break;
			}
			break;

		case 0x201://LBUTTONDOWN
			m_bLButtonDown = TRUE;
			m_ptButtonDown = pMsg->pt;
			m_bCtrlMoved = FALSE;
			ScreenToClient(&m_ptButtonDown);
			switch(pMsg->wParam) {
			case 1:	
				switch(pFrame->m_CtrlType) {
				case 0:	
					FindNearestCtrl();
					if(m_bTabOrder == TRUE) {
						ChangeOrder(m_nIndex,m_nTabNumber++);
						if(m_nTabNumber>=theApp.m_nDescriptionL) m_nTabNumber=0;
						m_bLButtonDown = FALSE;
					}
					Invalidate(TRUE);
					break;
				case TEXT_CTL:
				case EDIT_CTL:
				case BUTTON_CTL:
					AddCtrl(pFrame->m_CtrlType);
					Invalidate(TRUE);
					pFrame->m_CtrlType = (CtrlType)0;
					break;
				}
				return TRUE;
				break;
			}
			break;

		case 0x202:	//LBUTTONUP
			if((m_bCtrlMoved) && (m_bLButtonDown == TRUE)) {
				m_rrIndex = m_rrDrag;
				m_rrIndex.InflateRect(4,4);
				theApp.m_pDescription[m_nIndex].rrCtrl = m_rrDrag;
			}
			m_bLButtonDown = FALSE;
			m_bCtrlMoved = FALSE;
			if(m_pCtrlPropertiesDlg) {
				m_pCtrlPropertiesDlg->m_nIndex = m_nIndex;
				m_pCtrlPropertiesDlg->UpdateAllControls();
			};
			Invalidate(TRUE);
			return TRUE;
			break;

		case 0x203: //LBUTTONDOUBLECLICK
			switch(pMsg->wParam) {
			case 1:	OnEditProperties();
				return TRUE;
				break;
			}
			break;

		case 0x204://RBUTTONDOWN
			m_ptButtonDown = pMsg->pt;
			ScreenToClient(&m_ptButtonDown);
			switch(pMsg->wParam) {
			case 2:	OnContextMenu(this, pMsg->pt);
//				return TRUE;
				break;
			}
			break;

		case 273:
			switch(pMsg->wParam) {
			case ID_EDIT_CUT: OnEditCut();
				return TRUE;
				break;				
			case ID_EDIT_PROPERTIES: OnEditProperties();
				return TRUE;
				break;
			case ID_EDIT_TABORDER: OnTabOrder();
				return TRUE;
				break;
			}
			break;
			
		}

		
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CDetailsPage::FindNearestCtrl()
{
	CPoint	Point;
	CRect rr;

	GetCursorPos(&Point);
	ScreenToClient(&Point);

	m_nIndex = -1;
	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		rr = theApp.m_pDescription[nn].rrCtrl;
		rr.InflateRect(4,4);

		if(m_bShiftKeyDown == FALSE) {
			theApp.m_pDescription[nn].nActionFlag &= ~CTL_HIGHLIGHT;
		}

		if(rr.PtInRect(Point)) {
			m_nIndex = nn;
			m_rrDrag = m_rrIndex = theApp.m_pDescription[nn].rrCtrl;
			m_rrIndex.InflateRect(4,4);
			theApp.m_pDescription[nn].nActionFlag |= CTL_HIGHLIGHT;
		}
	}

}



void CDetailsPage::AddCtrl(CtrlType nType)
{
	CPoint	Point;
	CDC* pDC = GetDC();
	
	GetCursorPos(&Point);
	ScreenToClient(&Point);

	int	nn=theApp.m_nDescriptionL++;
	int	ii;
	DescriptionStruct* pDescriptionTemp;

	if(nn==0) {
		SAFE_DELETE(theApp.m_pDescription);
		theApp.m_pDescription = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
	} else {
		pDescriptionTemp = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
		for(ii=0;ii<nn;ii++) {
			pDescriptionTemp[ii] = theApp.m_pDescription[ii];
			if(theApp.m_pDescription[ii].pCaption) {
				pDescriptionTemp[ii].pCaption = new CString(*theApp.m_pDescription[ii].pCaption);
				SAFE_DELETE(theApp.m_pDescription[ii].pCaption);
			}
		}
		SAFE_DELETE(theApp.m_pDescription);

		theApp.m_pDescription = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
		for(ii=0;ii<nn;ii++) {
			theApp.m_pDescription[ii] = pDescriptionTemp[ii];
			if(pDescriptionTemp[ii].pCaption) {
				theApp.m_pDescription[ii].pCaption = new CString(*pDescriptionTemp[ii].pCaption);
				SAFE_DELETE(pDescriptionTemp[ii].pCaption);
			}
		}
		delete pDescriptionTemp;
	}



	ZeroMemory(&theApp.m_pDescription[nn],sizeof theApp.m_pDescription[nn]);
	theApp.m_pDescription[nn].nStructSize = sizeof DescriptionStruct;
	theApp.m_pDescription[nn].pCaption = new CString(_T("Static"));
	CSize Size = pDC->GetTextExtent(*theApp.m_pDescription[nn].pCaption);
	switch(theApp.m_pDescription[nn].nCtrlType = (CtrlType)nType) {
	case TEXT_CTL:	theApp.m_pDescription[nn].rrCtrl.SetRect(Point.x,Point.y,Point.x+Size.cx,Point.y+Size.cy);
		break;
	case EDIT_CTL:	theApp.m_pDescription[nn].rrCtrl.SetRect(Point.x,Point.y,Point.x+(2*Size.cx),Point.y+Size.cy+6);
		theApp.m_pDescription[nn].pCaption->Empty();
		theApp.m_pDescription[nn].nExStyle = WS_EX_CLIENTEDGE;
		break;
	case BUTTON_CTL:	theApp.m_pDescription[nn].rrCtrl.SetRect(Point.x,Point.y,Point.x+(2*Size.cx),Point.y+Size.cy+6);
		theApp.m_pDescription[nn].pCaption->Format(_T("Button"));
		theApp.m_pDescription[nn].nStyle |= BS_CENTER;
		break;
	};

	ReleaseDC(pDC);

}

void CDetailsPage::DrawCtrlHighLight(CPaintDC *pDC,CRect rr,COLORREF rgbColor)
{
	
	CPen penBlue(PS_DOT,2,rgbColor);
	CPen* pOldPen = pDC->SelectObject(&penBlue);

	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.right,rr.top);
	pDC->LineTo(rr.BottomRight());
	pDC->LineTo(rr.left,rr.bottom);
	pDC->LineTo(rr.TopLeft());

	pDC->SelectObject(pOldPen);
}

void CDetailsPage::DrawDrag(CPaintDC *pDC)
{
	
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	pDC->MoveTo(m_rrDrag.TopLeft());
	pDC->LineTo(m_rrDrag.right,m_rrDrag.top);
	pDC->LineTo(m_rrDrag.BottomRight());
	pDC->LineTo(m_rrDrag.left,m_rrDrag.bottom);
	pDC->LineTo(m_rrDrag.TopLeft());

	pDC->SelectObject(pOldPen);
}


BOOL CDetailsPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(m_bLButtonDown == FALSE) {
		if(m_rrIndex.PtInRect(point)) {
			if(theApp.m_pDescription[m_nIndex].rrCtrl.PtInRect(point)) {
				SetCursor(m_hcursorAllMove);
				m_nWhichEdge = LEFT_EDGE | RIGHT_EDGE | TOP_EDGE | BOTTOM_EDGE;
				return TRUE;
			}
			if((m_rrIndex.left <= point.x) && (point.x <= theApp.m_pDescription[m_nIndex].rrCtrl.left)) {
				SetCursor(m_hcursorHMove);
				m_nWhichEdge = LEFT_EDGE;
				return TRUE;
			}
			if((m_rrIndex.right >= point.x) && (point.x >= theApp.m_pDescription[m_nIndex].rrCtrl.right)) {
				SetCursor(m_hcursorHMove);
				m_nWhichEdge = RIGHT_EDGE;
				return TRUE;
			}
			if((m_rrIndex.top <= point.y) && (point.y <= theApp.m_pDescription[m_nIndex].rrCtrl.top)) {
				SetCursor(m_hcursorVMove);
				m_nWhichEdge = TOP_EDGE;
				return TRUE;
			}
			if((m_rrIndex.bottom >= point.y) && (point.y >= theApp.m_pDescription[m_nIndex].rrCtrl.bottom)) {
				SetCursor(m_hcursorVMove);
				m_nWhichEdge = BOTTOM_EDGE;
				return TRUE;
			}
			return TRUE;
		}
	} else {
		return TRUE;
	}

	return CPropertyPage::OnSetCursor(pWnd, nHitTest, message);
}





void CDetailsPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_bDesignDlg == TRUE) {
		CMenu* menu = (CMenu *) new CMenu;

		if (menu->LoadMenu(IDR_RB_EDIT_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
		}

		delete menu;
		
	}
}

void CDetailsPage::OnEditCut() 
{
	int	nn=theApp.m_nDescriptionL;
	int ii;

	if(theApp.m_nDescriptionL > 0) {
		for(ii=m_nIndex;ii<(theApp.m_nDescriptionL - 1);ii++) {
			SAFE_DELETE(theApp.m_pDescription[ii].pCaption);
			theApp.m_pDescription[ii] = theApp.m_pDescription[ii+1];
			if(theApp.m_pDescription[ii+1].pCaption) {
				theApp.m_pDescription[ii].pCaption = new CString(*theApp.m_pDescription[ii+1].pCaption);
			}
		}
		SAFE_DELETE(theApp.m_pDescription[ii].pCaption);
		theApp.m_nDescriptionL--;
		m_nIndex = -1;
		m_rrIndex.SetRectEmpty();

		
		SAFE_DELETE_ARRAY( m_pStatic );
		SAFE_DELETE_ARRAY( m_pEdit );
		SAFE_DELETE_ARRAY( m_pButton );
		CDC* pDC = GetDC();
		DisplayControls(pDC);
		ReleaseDC(pDC);
		Invalidate(TRUE);
	}

}

void CDetailsPage::OnEditProperties() 
{
	if (m_pCtrlPropertiesDlg == NULL) {
		m_pCtrlPropertiesDlg = new CCtrlPropertiesDlg(this,(CWnd**)&m_pCtrlPropertiesDlg,m_nIndex);
		m_pCtrlPropertiesDlg->Create(CCtrlPropertiesDlg::IDD, this);
	} else {
		m_pCtrlPropertiesDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_pCtrlPropertiesDlg->m_nIndex = m_nIndex;
		m_pCtrlPropertiesDlg->UpdateAllControls();
	}
}

void CDetailsPage::OnTabOrder()
{
	m_bTabOrder == FALSE ? m_bTabOrder = TRUE : m_bTabOrder = FALSE;
	m_nTabNumber = 0;
	Invalidate(TRUE);

}

void CDetailsPage::ChangeOrder(int nSrc,int nDest)
{
	DescriptionStruct Description;
	CString Buff;

	if((nSrc>=0) && (nSrc<theApp.m_nDescriptionL) && (nDest>=0) && (nDest<theApp.m_nDescriptionL)) {
		Description = theApp.m_pDescription[nDest];
		if(theApp.m_pDescription[nDest].pCaption) {
			Buff = *theApp.m_pDescription[nDest].pCaption;
			SAFE_DELETE(theApp.m_pDescription[nDest].pCaption);
		}

		theApp.m_pDescription[nDest] = theApp.m_pDescription[nSrc];
		if(theApp.m_pDescription[nSrc].pCaption) {
			theApp.m_pDescription[nDest].pCaption = new CString(*theApp.m_pDescription[nSrc].pCaption);
			SAFE_DELETE(theApp.m_pDescription[nSrc].pCaption);
		}
		
		theApp.m_pDescription[nSrc] = Description;
		if(theApp.m_pDescription[nSrc].pCaption) {
			theApp.m_pDescription[nSrc].pCaption = new CString(Buff);
		}
	}

}

void CDetailsPage::SetAccessPrivelages()
{
}


void CDetailsPage::LeftAlign()
{	
	CRect rr = theApp.m_pDescription[m_nIndex].rrCtrl;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			theApp.m_pDescription[nn].rrCtrl.OffsetRect(rr.left-theApp.m_pDescription[nn].rrCtrl.left,0);
		}
	}
	Invalidate(TRUE);

}

void CDetailsPage::RightAlign()
{
	CRect rr = theApp.m_pDescription[m_nIndex].rrCtrl;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			theApp.m_pDescription[nn].rrCtrl.OffsetRect(rr.right-theApp.m_pDescription[nn].rrCtrl.right,0);
		}
	}
	Invalidate(TRUE);
}

void CDetailsPage::TopAlign()
{
	CRect rr = theApp.m_pDescription[m_nIndex].rrCtrl;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			theApp.m_pDescription[nn].rrCtrl.OffsetRect(0,rr.top-theApp.m_pDescription[nn].rrCtrl.top);
		}
	}
	Invalidate(TRUE);

}


void CDetailsPage::SameWidth()
{
	CRect rr = theApp.m_pDescription[m_nIndex].rrCtrl;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			theApp.m_pDescription[nn].rrCtrl.right = theApp.m_pDescription[nn].rrCtrl.left + rr.Width();
		}
	}
	Invalidate(TRUE);

}

void CDetailsPage::SameHeight()
{
	CRect rr = theApp.m_pDescription[m_nIndex].rrCtrl;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			theApp.m_pDescription[nn].rrCtrl.bottom = theApp.m_pDescription[nn].rrCtrl.top + rr.Height();
		}
	}
	Invalidate(TRUE);

}

void CDetailsPage::SpaceVertically()
{
	int nTop,nBottom,nNumber,nY;
	CRect rr;

	nTop = 2000;
	nBottom = 0;
	nNumber=0;

	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
			if(nTop>theApp.m_pDescription[nn].rrCtrl.top) nTop = theApp.m_pDescription[nn].rrCtrl.top;
			if(nBottom<theApp.m_pDescription[nn].rrCtrl.top) nBottom = theApp.m_pDescription[nn].rrCtrl.top;
			nNumber++;
		}
	}
	if(nNumber>1) {
		for(int nn=0,gg=0;nn<theApp.m_nDescriptionL;nn++) {
			if(theApp.m_pDescription[nn].nActionFlag & CTL_HIGHLIGHT) {
				nY=MulDiv(gg,nBottom-nTop,nNumber-1) + nTop;
				rr = theApp.m_pDescription[nn].rrCtrl;
				theApp.m_pDescription[nn].rrCtrl.SetRect(rr.left,nY,rr.right,nY + rr.Height());
				gg++;
			}
		}
	}
	
	Invalidate(TRUE);

}

