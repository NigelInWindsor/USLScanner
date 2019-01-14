// ProtractorPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProtractorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProtractorPage property page

IMPLEMENT_DYNCREATE(CProtractorPage, CPropertyPage)

CProtractorPage::CProtractorPage() : CPropertyPage(CProtractorPage::IDD)
{
	//{{AFX_DATA_INIT(CProtractorPage)
	//}}AFX_DATA_INIT
	
	m_pFont = NULL;
	m_pArray = NULL;
	m_strAngle = L"0.00";
}

CProtractorPage::~CProtractorPage()
{
}

void CProtractorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProtractorPage)
	DDX_Control(pDX, IDC_COMBO_PORT_NUMBER, m_comboPortNumber);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_editAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProtractorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CProtractorPage)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_PORT_NUMBER, OnSelchangeComboPortNumber)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, OnChangeEditAngle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProtractorPage message handlers
void CProtractorPage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::PostNcDestroy();
}

void CProtractorPage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnClose();
}

void CProtractorPage::OnDestroy() 
{
	SAFE_DELETE( m_pFont );
	SAFE_DELETE( m_pArray );

	CPropertyPage::OnDestroy();
	
	
}

BOOL CProtractorPage::OnInitDialog() 
{
	
	CPropertyPage::OnInitDialog();
	CRect rr;
	LOGFONT	lf;
	float fScale;

	m_comboPortNumber.AddString(L"N/A");
	m_comboPortNumber.AddString(L"Com1");
	m_comboPortNumber.AddString(L"Com2");
	m_comboPortNumber.AddString(L"Com3");
	m_comboPortNumber.AddString(L"Com4");
	m_comboPortNumber.AddString(L"Com5");
	m_comboPortNumber.AddString(L"Com6");
	m_comboPortNumber.AddString(L"Com7");
	m_comboPortNumber.AddString(L"Com8");
	m_comboPortNumber.AddString(L"Com9");
	m_comboPortNumber.AddString(L"Com10");
	m_comboPortNumber.AddString(L"Com11");
	m_comboPortNumber.AddString(L"Com12");
	m_comboPortNumber.AddString(L"Com13");
	m_comboPortNumber.AddString(L"Com14");
	m_comboPortNumber.AddString(L"Com15");
	m_comboPortNumber.AddString(L"Com16");
	m_comboPortNumber.AddString(L"Com17");
	m_comboPortNumber.SetCurSel( theApp.m_LastSettings.nProtractorPortNumber );
	
	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_staticView.GetWindowRect(rr);
	lf = m_LogFont;
	fScale = (float)rr.Height() / (float)m_rrClient.Height();
	lf.lfHeight = (int)((float)rr.Height() / -1.80f);
	theApp.SetFontLanguage(&lf);
	SAFE_DELETE( m_pFont );
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&lf);

	Invalidate(false);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CProtractorPage::OnSetActive() 
{
	

	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,100,NULL);
	m_Serial.Start(theApp.m_LastSettings.nProtractorPortNumber);
	
	return CPropertyPage::OnSetActive();
}

BOOL CProtractorPage::OnKillActive() 
{

	KillTimer(1);
	m_Serial.Close();
	
	return CPropertyPage::OnKillActive();
}

void CProtractorPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;

	if(m_Serial.IStringReceived()) {
		m_Serial.GetStr(&Buff);

		if(m_strAngle != Buff) {
			m_strAngle = Buff;
			Invalidate(false);
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}


void CProtractorPage::OnPaint() 
{

	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	
	CRect		rr,rect,rrStore;
	CDC			dcMem;
	CBitmap		bmp;
	CSize size;
	CString Buff;
	
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

	dcMem.FillRect(&rect,&CBrush(theApp.m_LastSettings.rgbFaroBk));


	Buff.Format(L"%s%s",m_strAngle,theApp.m_Degrees);
	size = dcMem.GetTextExtent(Buff);
	dcMem.TextOut(rect.CenterPoint().x - size.cx/2, rect.CenterPoint().y - size.cy/2, Buff);

	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
	
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CProtractorPage::OnSelchangeComboPortNumber() 
{
	

	m_Serial.Close();
	m_Serial.Start(theApp.m_LastSettings.nProtractorPortNumber = m_comboPortNumber.GetCurSel());
}

BOOL CProtractorPage::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;
	int	nEncoderPos;
	float fAngle;


	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:
			m_editAngle.GetWindowText(Buff);
			_WTOF(Buff, fAngle);
			nEncoderPos = (int)(fAngle * 10000.0f / 360.0f);
			Buff.Format(L"%d",nEncoderPos);
			m_Serial.SendStr(&Buff);

			m_editAngle.SetWindowText(L"");

			return TRUE;
			break;
		}
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CProtractorPage::OnChangeEditAngle() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
