// FaroElementExpansionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "FaroElementExpansionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaroElementExpansionDlg dialog


CFaroElementExpansionDlg::CFaroElementExpansionDlg(CWnd* pParent ,CWnd** pDlg/*=NULL*/, int nElementIndex)
	: CDialog(CFaroElementExpansionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFaroElementExpansionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_nElementIndex = nElementIndex;
}


void CFaroElementExpansionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFaroElementExpansionDlg)
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFaroElementExpansionDlg, CDialog)
	//{{AFX_MSG_MAP(CFaroElementExpansionDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaroElementExpansionDlg message handlers

void CFaroElementExpansionDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CFaroElementExpansionDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CFaroElementExpansionDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CFaroElementExpansionDlg::OnDestroy() 
{
	if (CFaroElementExpansionDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CFaroElementExpansionDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CFaroElementExpansionDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	int nTabs[10] = {0,60,60,60,60,60,60,60,60,60};
	CRect rr;

	MapDialogRect(&rr);
	int nWidth = GetDialogBaseUnits();
	nWidth = 60;
	for(int ii=1;ii<10;ii++) {
		nTabs[ii] = nTabs[ii-1] + nWidth;
	}
	m_editText.SetTabStops(10,nTabs);
	

	m_StaticText.Initialize(this, m_editText, TRUE, TRUE, TRUE, TRUE);

	if (CFaroElementExpansionDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CFaroElementExpansionDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		}
	}

	m_StaticText.SetNewPos();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFaroElementExpansionDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticText.SetNewPos();
	
}

void CFaroElementExpansionDlg::UpdateAllControls()
{
	CString Buff,Temp;
	int ii;
	D3DXVECTOR3 vPt;
	CVertexArray *pVertexArray = &theApp.m_FaroWorkerThread.m_Element[m_nElementIndex];

	if (GetSafeHwnd() == NULL) return;
	Buff.Format(L"%5s\t%5s\t%5s\r\n",L"X",L"Y",L"Z");
	for(ii=0;ii<pVertexArray->GetSize();ii++) {
		pVertexArray->GetPt(ii,&vPt);

		switch(	theApp.m_LastSettings.nFaroTrackerCoordinateDisplayMode ) {
		default:
			break;
		case 1:	theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt, &vPt);
			break;
		case 2:	theApp.m_FaroWorkerThread.GetToolCoordinate(&vPt, &vPt);
			break;
		}
		
		
		Temp.Format(L"%.03f\t%.03f\t%.03f\r\n",vPt.x,vPt.y,vPt.z);
		Buff += Temp;
	}

	m_editText.SetWindowText(Buff);
}

void CFaroElementExpansionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	m_editText.SetSel(0,-1);
//	m_editText.Copy();
//	m_editText.Paste();
}
