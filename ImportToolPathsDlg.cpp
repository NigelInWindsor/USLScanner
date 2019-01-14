// ImportToolPathsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ImportToolPathsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportToolPathsDlg dialog

#define	TP_START		0
#define	TP_EOL			1
#define	TP_FINISH		2
#define	TP_REF_PTS		3
#define	TP_ANC_PTS		4
#define TP_COORD		5
#define	TP_ENV_START	6
#define	TP_ENV_FINISH	7

#define	RGB_HIGHLIGHTED		RGB(0,0,255)

#define	DISPLAY_LINE_NUMBERS	1
#define	IMPORT_APTSOURCE		2
#define	IMPORT_ASCII			4
#define	IMPORT_CLOUDSET			8

CImportToolPathsDlg::CImportToolPathsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CImportToolPathsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportToolPathsDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_nIndex = 0;
	m_nLength = 0;
	m_nTopRow = 0;
	m_nRowsL = 20;
	m_pFlag = NULL;
	m_bLBDown = false;
	m_bRBDown = false;
	m_bShiftKey = false;
	m_bCtrlKey = false;
	m_nShiftIndex = 0;
	m_nToolPathL = 0;



//	RetrieveFile(_T("C:\\AptSource\\BAE ForePlane\\NDT-R-252-OML.aptsource"));
//	SetFlags();
}


void CImportToolPathsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportToolPathsDlg)
	DDX_Control(pDX, IDC_COMBO_EXTRACT_METHOD, m_comboExtractionMethod);
	DDX_Control(pDX, IDC_CHECK_LINE_NUMBERS, m_checkLineNumbers);
	DDX_Control(pDX, IDC_LIST_EOP, m_listEOP);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_comboDirection);
	DDX_Control(pDX, IDC_STATIC_ENV_FINISH, m_staticFinishEnv);
	DDX_Control(pDX, IDC_STATIC_ENV_START, m_staticStartEnv);
	DDX_Control(pDX, IDC_EDIT_COORDINATE, m_editCoordinate);
	DDX_Control(pDX, IDC_STATIC_START_TP, m_staticStartTP);
	DDX_Control(pDX, IDC_STATIC_REF, m_staticREF);
	DDX_Control(pDX, IDC_STATIC_FINISH_TP, m_staticFinishTP);
	DDX_Control(pDX, IDC_STATIC_EOL, m_staticEOL);
	DDX_Control(pDX, IDC_STATIC_ANC, m_staticAnc);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_ProgressBar);
	DDX_Control(pDX, IDC_COMBO_UNITS, m_comboUnits);
	DDX_Control(pDX, IDC_COMBO_DESTINATION, m_comboDestination);
	DDX_Control(pDX, IDC_EDIT_ANCILLARY_PTS, m_editAncillaryPts);
	DDX_Control(pDX, IDC_EDIT_REF_PTS, m_editRefPoints);
	DDX_Control(pDX, IDC_SCROLLBAR_VERT, m_scrollVert);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_LINE_SEPERATOR, m_editLineSeperator);
	DDX_Control(pDX, IDC_EDIT_START_TOOLPATHS, m_editStartToolPath);
	DDX_Control(pDX, IDC_EDIT_FINISH_TOOLPATHS, m_editFinishToolPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportToolPathsDlg, CDialog)
	//{{AFX_MSG_MAP(CImportToolPathsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_IMPORT_TOOLPATHSTART, OnImportToolpathstart)
	ON_COMMAND(ID_IMPORT_TOOLPATHEOL, OnImportToolpatheol)
	ON_COMMAND(ID_IMPORT_TOOLPATHFINISH, OnImportToolpathfinish)
	ON_COMMAND(ID_IMPORT_REFPOINTSSTART, OnImportRefpointsstart)
	ON_COMMAND(ID_IMPORT_ANCILLARYPTSSTART, OnImportAncillaryptsstart)
	ON_EN_CHANGE(IDC_EDIT_ANCILLARY_PTS, OnChangeEditAncillaryPts)
	ON_EN_CHANGE(IDC_EDIT_FINISH_TOOLPATHS, OnChangeEditFinishToolpaths)
	ON_EN_CHANGE(IDC_EDIT_LINE_SEPERATOR, OnChangeEditLineSeperator)
	ON_EN_CHANGE(IDC_EDIT_REF_PTS, OnChangeEditRefPts)
	ON_EN_CHANGE(IDC_EDIT_START_TOOLPATHS, OnChangeEditStartToolpaths)
	ON_CBN_SELCHANGE(IDC_COMBO_DESTINATION, OnSelchangeComboDestination)
	ON_CBN_SELCHANGE(IDC_COMBO_UNITS, OnSelchangeComboUnits)
	ON_COMMAND(ID_IMPORT_COPYCOORDINATE, OnImportCopycoordinate)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_BUTTON_IMPORT, OnButtonImport)
	ON_COMMAND(ID_IMPORT_GOTO_TOOLPATHSTART, OnImportGotoToolpathstart)
	ON_COMMAND(ID_IMPORT_GOTO_TOOLPATHFINISH, OnImportGotoToolpathfinish)
	ON_COMMAND(ID_IMPORT_GOTO_REFPOINTS, OnImportGotoRefpoints)
	ON_COMMAND(ID_IMPORT_GOTO_ANCILLARYPOINTS, OnImportGotoAncillarypoints)
	ON_EN_CHANGE(IDC_EDIT_COORDINATE, OnChangeEditCoordinate)
	ON_COMMAND(ID_IMPORT_COORDINATE, OnImportCoordinate)
	ON_COMMAND(ID_IMPORT_ENVELOPESTART, OnImportEnvelopestart)
	ON_COMMAND(ID_IMPORT_ENVELOPEFINISH, OnImportEnvelopefinish)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, OnSelchangeComboDirection)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_COMMAND(ID_BUTTON_IMPORT_ASCII, OnButtonImportAscii)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_EOP, OnGetdispinfoListEop)
	ON_NOTIFY(NM_CLICK, IDC_LIST_EOP, OnClickListEop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EOP, OnDblclkListEop)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_EOP, OnRclickListEop)
	ON_COMMAND(ID_EOP_DELETE, OnEopDelete)
	ON_BN_CLICKED(IDC_CHECK_LINE_NUMBERS, OnCheckLineNumbers)
	ON_COMMAND(ID_BUTTON_IMPORT_APTSOURCE, OnButtonImportAptsource)
	ON_CBN_SELCHANGE(IDC_COMBO_EXTRACT_METHOD, OnSelchangeComboExtractMethod)
	ON_COMMAND(ID_BUTTON_IMPORT_CLOUDSET, OnButtonImportCloudset)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ENVELOPE, OnButtonImportEnvelope)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportToolPathsDlg message handlers
int CImportToolPathsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndImportToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndImportToolBar.LoadToolBar(IDR_IMPORT_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndImportToolBar);
	
	return 0;
}

void CImportToolPathsDlg::PostNcDestroy() 
{
	SAFE_DELETE( m_pFlag );
	
	CDialog::PostNcDestroy();
	delete this;
}

void CImportToolPathsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CImportToolPathsDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CImportToolPathsDlg::OnDestroy() 
{
	if (CImportToolPathsDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CImportToolPathsDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CImportToolPathsDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CRect rr;

	if (CImportToolPathsDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CImportToolPathsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}	

	m_spinSlowIncrement.SetRange(0,200);

	m_scrollVert.SetScrollRange(0.0,(double)m_nLength,FALSE);
	m_scrollVert.SetPageSize((double)m_nRowsL,FALSE);
	m_scrollVert.SetScrollPos((double)m_nTopRow, TRUE);

	m_comboDestination.AddString(_T("Taught"));
	m_comboDestination.AddString(_T("Scan"));

	m_comboUnits.AddString(_T("Metric"));
	m_comboUnits.AddString(_T("Imperial"));

	m_comboDirection.AddString(_T("Unidirectional"));
	m_comboDirection.AddString(_T("Zig Zag"));

	m_comboExtractionMethod.AddString(L"APT Version 1");
	m_comboExtractionMethod.AddString(L"APT Version 2");
	m_comboExtractionMethod.AddString(L"X Y Z i j k");
	m_comboExtractionMethod.AddString(L"X<tab>Y<tab>Z");
	m_comboExtractionMethod.AddString(L"X,Y,Z,i,j,k");

	
	CreateColumnsEOP();

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CImportToolPathsDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   switch(nID) {
   case ID_FILE_OPEN: strTipText.LoadString(IDS_Open);
	   break;
   case ID_BUTTON_IMPORT: strTipText.LoadString(IDS_Import);
	   break;
   case ID_BUTTON_IMPORT_ASCII: strTipText.LoadString(IDS_Import_Ascii);
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

void CImportToolPathsDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	int ii;
	CString Buff;

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_START].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_START].GetAt(ii) + L"\r\n");
	}
	m_editStartToolPath.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_EOL].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_EOL].GetAt(ii) + L"\r\n");
	}
	m_editLineSeperator.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_FINISH].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_FINISH].GetAt(ii) + L"\r\n");
	}
	m_editFinishToolPath.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_REF_PTS].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_REF_PTS].GetAt(ii) + L"\r\n");
	}
	m_editRefPoints.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_ANC_PTS].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_ANC_PTS].GetAt(ii) + L"\r\n");
	}
	m_editAncillaryPts.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_COORD].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_COORD].GetAt(ii) + L"\r\n");
	}
	m_editCoordinate.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_ENV_START].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_ENV_START].GetAt(ii) + L"\r\n");
	}
//	m_editStartEnvelope.SetWindowText(Buff);

	Buff.Empty();
	for(ii=0;ii<theApp.m_strToolPath[TP_ENV_FINISH].GetSize();ii++) {
		Buff += (theApp.m_strToolPath[TP_ENV_FINISH].GetAt(ii) + L"\r\n");
	}
//	m_editFinishEnvelope.SetWindowText(Buff);

	m_comboDestination.SetCurSel(theApp.m_LastSettings.nImportToolPathDest);
	m_comboUnits.SetCurSel(theApp.m_LastSettings.nImportAptUnits & 1);
	m_comboDirection.SetCurSel(theApp.m_LastSettings.nImportAptDirection);

	Buff.Format(_T("%.*f %s"),theApp.m_nUnitsPrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	m_checkLineNumbers.SetCheck(theApp.m_LastSettings.nImportDesignRulesMode & DISPLAY_LINE_NUMBERS);

	m_comboExtractionMethod.SetCurSel(theApp.m_LastSettings.nImportExtractionMethod);


	FillListEOP();
	SetToolBarCheckedState();

}

void CImportToolPathsDlg::SetWindowTitle()
{
	CString	Buff;
	int nPos;

	Buff = m_FileNamePath;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	SetWindowText(Buff);
}

void CImportToolPathsDlg::OnFileOpen() 
{
	int nPos;

	CFileDialog FileDlg(TRUE,_T(".AptSource"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All files (*.*)|*.*|TEXT (*.txt)|*.txt|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_AptSourcePath;
	if(FileDlg.DoModal()==IDOK) {
		m_FileNamePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = m_FileNamePath.ReverseFind('\\')) >0) {
				theApp.m_AptSourcePath = m_FileNamePath.Left(nPos);
			}
		}

		m_FileNamePath.Format(_T("%s"),FileDlg.GetPathName());
		theApp.ActivityLog(L"Import aptsource : " + m_FileNamePath);

		if(RetrieveFile(m_FileNamePath) == true) {
			SetFlags();
			SetWindowTitle();
			Invalidate(FALSE);
		}
	}
	
}




bool CImportToolPathsDlg::RetrieveFile(CString FileName)
{
	CUSLFile file;
	CFileException e;
	CString	Buff,Language;
	char* pText;
	char Array[256];
	int	ii,jj;
	int nPos;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary,&e)) {
		file.DecodeFileException(&e,&Buff,FileName);
		Language.LoadString(IDS_Error);
		MessageBox(Buff,Language,MB_ICONERROR);
		return FALSE;
	} else {
		m_str.RemoveAll();

		m_nIndex = 0;
		m_nTopRow = 0;
		int nFileLength = (int)file.GetLength();

		pText = new char[nFileLength+1];
		file.Read(pText,nFileLength);
		file.Close();

		m_ProgressBar.SetRange(0,100);
		m_ProgressBar.SetPos(nPos=0);
		for(ii=0,jj=0;ii<nFileLength;ii++,jj++) {

			if(nPos - MulDiv(ii,100,nFileLength)) {
				m_ProgressBar.SetPos(nPos = MulDiv(ii,100,nFileLength));
			}

			Array[jj]=pText[ii];
			if((pText[ii]==0x0a) || (pText[ii]==0x0d) || (ii==nFileLength-1)) {
				Array[jj]=0;
				if(strlen(Array)>1) {
					Buff.Format(_T("%s"), (CString) Array);
					Buff.TrimRight();
					m_str.Add(Buff);
				}
				jj=-1;
			}
		}
		m_ProgressBar.SetPos(0);
		delete pText;
		m_nLength = m_str.GetSize();
		SAFE_DELETE( m_pFlag );
		m_pFlag = new unsigned int[m_nLength];
		ZeroMemory(m_pFlag, m_nLength * sizeof (int));
		return TRUE;
	}
	return TRUE;
}


void CImportToolPathsDlg::OnPaint() 
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


	Buff = "Hello";
	m_size = dcMem.GetTextExtent(Buff);
	m_nRowsL = rr.Height() / m_size.cy;

	dcMem.SetBkMode(TRANSPARENT);
	for(int nRow=0;nRow<m_nRowsL && nRow<m_nLength;nRow++) {
		rrBK = rr;
		rrBK.top = nRow*m_size.cy;
		rrBK.bottom = rrBK.top + m_size.cy;


		if(m_pFlag[nRow + m_nTopRow] & START_TOOLPATH) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbStartToolPath));
		}

		if(m_pFlag[nRow + m_nTopRow] & FINISH_TOOLPATH) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbFinishToolPath));
			dcMem.SetTextColor(RGB(255,255,255));
		}

		if(m_pFlag[nRow + m_nTopRow] & EOL_TOOLPATH) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbEOLToolPath));
		}

		if(m_pFlag[nRow + m_nTopRow] & START_REF_PTS) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbReference));
		}

		if(m_pFlag[nRow + m_nTopRow] & START_ANC_PTS) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbAncillary));
		}
		
		if(m_pFlag[nRow + m_nTopRow] & START_ENVELOPE) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbStartEnvelope));
		}

		if(m_pFlag[nRow + m_nTopRow] & FINISH_ENVELOPE) {
			dcMem.FillRect(&rrBK,&CBrush(theApp.m_LastSettings.rgbFinishEnvelope));
		}

		if(m_pFlag[nRow + m_nTopRow] & HIGHLIGHTED) {
			dcMem.FillRect(&rrBK,&CBrush(RGB(0,0,255)));
			dcMem.SetTextColor(RGB(255,255,255));
		}



		if((nRow + m_nTopRow) < m_nLength) {
			dcMem.TextOut(1,nRow*m_size.cy,m_str.GetAt(nRow + m_nTopRow));
			dcMem.SetTextColor(RGB(0,0,0));
		}
	}

	dc.BitBlt(rrStatic.left,rrStatic.top,rrStatic.Width(),rrStatic.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;
	
	BlockFill(&dc,&m_staticStartTP,theApp.m_LastSettings.rgbStartToolPath);
	BlockFill(&dc,&m_staticFinishTP,theApp.m_LastSettings.rgbFinishToolPath);
	BlockFill(&dc,&m_staticEOL,theApp.m_LastSettings.rgbEOLToolPath);
	BlockFill(&dc,&m_staticREF,theApp.m_LastSettings.rgbReference);
	BlockFill(&dc,&m_staticAnc,theApp.m_LastSettings.rgbAncillary);
	BlockFill(&dc,&m_staticStartEnv,theApp.m_LastSettings.rgbStartEnvelope);
	BlockFill(&dc,&m_staticFinishEnv,theApp.m_LastSettings.rgbFinishEnvelope);


}


void CImportToolPathsDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}


void CImportToolPathsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImportToolPathsDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	int nn;

	if(rr.PtInRect(point) && (m_pFlag != NULL)) {

		if((m_bShiftKey == false) && (m_bCtrlKey == false)) {
			for(int ii=0;ii<m_nLength;ii++) {
				m_pFlag[ii] &= ~HIGHLIGHTED;
			}
		}

		m_nIndex = m_nTopRow + MulDiv(point.y-rr.top-m_size.cy/2,m_nRowsL,m_nRowsL*m_size.cy);
		m_pFlag[m_nIndex] |= HIGHLIGHTED;

		if(m_bShiftKey == true) {
			if(m_nIndex < m_nShiftIndex) {
				for(nn=m_nIndex ; nn<m_nShiftIndex; nn++) {
					m_pFlag[nn] |= HIGHLIGHTED;
				}
			} else {
				for(nn=m_nShiftIndex ; nn<m_nIndex; nn++) {
					m_pFlag[nn] |= HIGHLIGHTED;
				}
			}
		}

		Invalidate(FALSE);
	}


	m_staticStartTP.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbStartToolPath=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticFinishTP.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbFinishToolPath=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticEOL.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbEOLToolPath=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticREF.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbReference=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticAnc.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbAncillary=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticStartEnv.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbStartEnvelope=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticFinishEnv.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbFinishEnvelope=dlg.GetColor();
			Invalidate(FALSE);
		}
	}


	
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CImportToolPathsDlg::PreTranslateMessage(MSG* pMsg) 
{
	int	ii;

	switch(pMsg->message) {
	case WM_KEYDOWN:
		switch(pMsg->wParam) {
		case 0x10:
			m_bShiftKey = TRUE;
			m_nShiftIndex = m_nIndex;
			break;
		case 0x11:
			m_bCtrlKey = TRUE;
			break;
		case 27:
			for(ii=0;ii<m_nLength;ii) {
				m_pFlag[ii] &= ~HIGHLIGHTED;
			}
			Invalidate(FALSE);
			break;
		}
		break;
	case WM_KEYUP:
		m_bShiftKey = false;
		m_bCtrlKey = false;
		break;
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
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CImportToolPathsDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{

	CMenu* menu = (CMenu *) new CMenu;

	if (menu->LoadMenu(IDR_RB_IMPORT_MENU)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		ASSERT(pPopup != NULL);
				
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,point.y,this);

	};
	delete menu;
	
}

void CImportToolPathsDlg::OnImportToolpathstart() 
{
	int	ii;

	theApp.m_strToolPath[TP_START].RemoveAll();
	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_START].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
	
}

void CImportToolPathsDlg::OnImportToolpatheol() 
{
	int	ii;

	theApp.m_strToolPath[TP_EOL].RemoveAll();
	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_EOL].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnImportToolpathfinish() 
{
	int	ii;

	theApp.m_strToolPath[TP_FINISH].RemoveAll();
	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_FINISH].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnImportRefpointsstart() 
{
	int	ii;

	theApp.m_strToolPath[TP_REF_PTS].RemoveAll();
	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_REF_PTS].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnImportAncillaryptsstart() 
{
	int	ii;

	theApp.m_strToolPath[TP_ANC_PTS].RemoveAll();
	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_ANC_PTS].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnImportCoordinate() 
{
	int	ii;

	theApp.m_strToolPath[TP_COORD].RemoveAll();
	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_COORD].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnImportEnvelopestart() 
{
	int	ii;

	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_ENV_START].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnImportEnvelopefinish() 
{
	int	ii;

	for(ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & HIGHLIGHTED) {
			theApp.m_strToolPath[TP_ENV_FINISH].Add(m_str.GetAt(ii));
		}
	}
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnChangeEditAncillaryPts() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editAncillaryPts.GetWindowText(Buff);

	theApp.m_strToolPath[TP_ANC_PTS].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_ANC_PTS].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);

}

void CImportToolPathsDlg::OnChangeEditFinishToolpaths() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editFinishToolPath.GetWindowText(Buff);

	theApp.m_strToolPath[TP_FINISH].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_FINISH].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);
	
}

void CImportToolPathsDlg::OnChangeEditLineSeperator() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editLineSeperator.GetWindowText(Buff);

	theApp.m_strToolPath[TP_EOL].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_EOL].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);
	
}

void CImportToolPathsDlg::OnChangeEditRefPts() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editRefPoints.GetWindowText(Buff);

	theApp.m_strToolPath[TP_REF_PTS].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_REF_PTS].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);
	
}

void CImportToolPathsDlg::OnChangeEditStartToolpaths() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editStartToolPath.GetWindowText(Buff);

	theApp.m_strToolPath[TP_START].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_START].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);
	
}

void CImportToolPathsDlg::OnChangeEditStartEnvelope() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editStartEnvelope.GetWindowText(Buff);

	theApp.m_strToolPath[TP_ENV_START].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_ENV_START].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);
	
}

void CImportToolPathsDlg::OnChangeEditFinishEnvelope() 
{
	CString Buff;
	int nLength, nStart = 0;

	m_editFinishEnvelope.GetWindowText(Buff);

	theApp.m_strToolPath[TP_ENV_FINISH].RemoveAll();
	while((nLength=Buff.Find(_T("\r\n"),nStart)) > 0 ) {
		theApp.m_strToolPath[TP_ENV_FINISH].Add(Buff.Mid(nStart,nLength-nStart));
		nStart = nLength + 2;
	}
	SetFlags();
	Invalidate(FALSE);
	
}

void CImportToolPathsDlg::OnChangeEditCoordinate() 
{
	CString Buff;
	int nStart = 0;

	m_editCoordinate.GetWindowText(Buff);

	theApp.m_strToolPath[TP_COORD].RemoveAll();
	if((nStart = Buff.Find(_T("\r\n"))) > 0 ) {
		Buff.GetBufferSetLength(nStart);
	}
	theApp.m_strToolPath[TP_COORD].Add(Buff);
	SetFlags();
	Invalidate(FALSE);

}

void CImportToolPathsDlg::OnSelchangeComboDestination() 
{
	theApp.m_LastSettings.nImportToolPathDest = m_comboDestination.GetCurSel();
}

void CImportToolPathsDlg::OnSelchangeComboUnits() 
{
	theApp.m_LastSettings.nImportAptUnits = m_comboUnits.GetCurSel();
}

void CImportToolPathsDlg::OnImportCopycoordinate() 
{
	CString Buff;
	CCoord Cp;

	Buff = m_str.GetAt(m_nIndex);

	ExtractCoordinate(Buff, &Cp);
	theApp.m_CopyLine.Zero();
	theApp.m_CopyLine.Add(Cp);

}

void CImportToolPathsDlg::SetFlags()
{
	if(theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_ASCII) {
		SetFlagsAscii();
	}

	if(theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_APTSOURCE) {
		SetFlagsAptSource();
	}
}

void CImportToolPathsDlg::SetFlagsAptSource()
{
	int	ii,gg,kk;
	bool bStart = false;
	bool bFinish = false;
	bool bEOL = false;
	bool bRefStart = false;
	bool bAncStart = false;
	bool bEnvStart = false;
	bool bEnvFinish = false;
	int	nStart = 0;
	int	nFinish = m_nLength;
	int nRefStart = 0;
	int	nAncStart = 0;
	int	nEnvStart = 0;
	int	nEnvFinish = 0;

	ZeroMemory(m_pFlag, m_nLength * sizeof (int));

	//Find the start
	for(ii=0;ii<m_nLength;ii++) {
		for(gg=0,bStart = true; (gg<theApp.m_strToolPath[TP_START].GetSize()) && (bStart == true);gg++) {

			if(m_str.GetAt(ii+gg).Find(theApp.m_strToolPath[TP_START].GetAt(gg)) == 0) {
				bStart=true;
				nStart = ii+gg;
				m_pFlag[ii+gg] |= START_TOOLPATH;
			} else {
				bStart = false;
				for(kk=0;kk<=gg;kk++) 
					m_pFlag[ii+kk] &= ~START_TOOLPATH;
			}
		}
		if(bStart == true) {
			break;
		}
	}

	//Find the finish
	for(ii=0;ii<m_nLength;ii++) {
		for(gg=0,bFinish = true; (gg<theApp.m_strToolPath[TP_FINISH].GetSize()) && (bFinish == true);gg++) {

			if(m_str.GetAt(ii+gg).Find(theApp.m_strToolPath[TP_FINISH].GetAt(gg)) == 0) {
				bFinish=true;
				nFinish = ii+gg;
				m_pFlag[ii+gg] |= FINISH_TOOLPATH;
			} else {
				bFinish = false;
				for(kk=0;kk<=gg;kk++) 
					m_pFlag[ii+kk] &= ~FINISH_TOOLPATH;
			}
		}
		if(bFinish == true) {
			break;
		}
	}

	//find all the EOL
	m_nToolPathL = 0;
	if(bStart == true) {

		if(theApp.m_strToolPath[TP_EOL].GetSize() > 0) {
			for(ii=nStart;ii<nFinish;ii++) {

				for(gg=0,bEOL = true; (gg<theApp.m_strToolPath[TP_EOL].GetSize()) && (bEOL == true);gg++) {
					if(m_str.GetAt(ii+gg).Find(theApp.m_strToolPath[TP_EOL].GetAt(gg)) != 0) {
						bEOL = false;
					}
				}

				if(bEOL == true) {
					for(gg=0;gg<theApp.m_strToolPath[TP_EOL].GetSize();gg++) {
						m_pFlag[ii+gg] = EOL_TOOLPATH;
					}
					m_nToolPathL++;
				}
			}
		}
		m_nToolPathL++;
	}
	
	//find ref start pos
	for(ii=0;ii<m_nLength;ii++) {
		for(gg=0,bRefStart = true; (gg<theApp.m_strToolPath[TP_REF_PTS].GetSize()) && (bRefStart == true);gg++) {

			if(m_str.GetAt(ii+gg).Find(theApp.m_strToolPath[TP_REF_PTS].GetAt(gg)) == 0) {
				bRefStart=true;
				nRefStart = ii+gg;
				m_pFlag[ii+gg] |= START_REF_PTS;
			} else {
				bRefStart = false;
				for(kk=0;kk<=gg;kk++) 
					m_pFlag[ii+kk] &= ~START_REF_PTS;
			}
		}
		if(bRefStart == true) {
			break;
		}
	}

	//find ancillary start pos
	for(ii=0;ii<m_nLength;ii++) {
		for(gg=0,bAncStart = true; (gg<theApp.m_strToolPath[TP_ANC_PTS].GetSize()) && (bAncStart == true);gg++) {

			if(m_str.GetAt(ii+gg).Find(theApp.m_strToolPath[TP_ANC_PTS].GetAt(gg)) == 0) {
				bAncStart=true;
				nAncStart = ii+gg;
				m_pFlag[ii+gg] |= START_ANC_PTS;
			} else {
				bAncStart = false;
				for(kk=0;kk<=gg;kk++) 
					m_pFlag[ii+kk] &= ~START_ANC_PTS;
			}
		}
		if(bAncStart == true) {
			break;
		}
	}

	//find envelope start pos
	for(ii=0;ii<m_nLength;ii++) {
		for(gg=0; gg<theApp.m_strToolPath[TP_ENV_START].GetSize();gg++) {

			if(m_str.GetAt(ii).Find(theApp.m_strToolPath[TP_ENV_START].GetAt(gg)) == 0) {
				m_pFlag[ii] |= START_ENVELOPE;
			}
		}
	}

	//find envelope start pos
	for(ii=0;ii<m_nLength;ii++) {
		for(gg=0; gg<theApp.m_strToolPath[TP_ENV_FINISH].GetSize();gg++) {

			if(m_str.GetAt(ii).Find(theApp.m_strToolPath[TP_ENV_FINISH].GetAt(gg)) == 0) {
				m_pFlag[ii] |= FINISH_ENVELOPE;
			}
		}
	}
}


void CImportToolPathsDlg::SetFlagsAscii()
{
	bool bStart = false;
	bool bFinish = false;
	bool bEOL = false;
	bool bRefStart = false;
	bool bAncStart = false;
	bool bEnvStart = false;
	bool bEnvFinish = false;
	int	nStart = 0;
	int	nFinish = m_nLength;
	int nRefStart = 0;
	int	nAncStart = 0;
	int	nEnvStart = 0;
	int	nEnvFinish = 0;
	CCoord Cp[2];
	float fLength;
	int ii;

	if(m_nLength <= 0) return;

	ZeroMemory(m_pFlag, m_nLength * sizeof (int));

	m_pFlag[0] = START_REF_PTS;

	m_pFlag[nStart=5] |= START_TOOLPATH;
	m_pFlag[nFinish = (m_nLength-1)] |= FINISH_TOOLPATH;


	//FInd EOL
	m_nToolPathL = 0;
	ExtractCoordinate((CString)m_str.GetAt(nStart),&Cp[0]);

	for(ii=nStart+1;ii<nFinish;ii++) {

		if(ExtractCoordinate((CString)m_str.GetAt(ii),&Cp[1]) == true) {

			fLength = CCoord::Length(Cp[0],Cp[1],0);
			if(fLength>4.0f) {
				m_pFlag[ii] = EOL_TOOLPATH;
				m_nToolPathL++;
			}
			Cp[0] = Cp[1];
		}
	}
	m_nToolPathL++;

}

//////////////////////////////////////////////////////////
//// This needs extensive checking
//////////////////////////////////////////////////

bool CImportToolPathsDlg::ExtractCoordinate(CString &rxString, CCoord *pCp)
{
	bool bFlag = false;
	int	nLength,nStart=0;
	CString Buff = L"GOTO /";
	CString strSearch = Buff + L"%f,%f,%f,%f,%f,%f";

	if(theApp.m_strToolPath[TP_COORD].GetSize()>0) {
		Buff = theApp.m_strToolPath[TP_COORD][0];
		strSearch.Format(_T("%s%s"), theApp.m_strToolPath[TP_COORD][0],_T("%f,%f,%f,%f,%f,%f"));
	}

	switch(theApp.m_LastSettings.nImportExtractionMethod) {
	case 0:
		if(theApp.m_strToolPath[TP_COORD].GetSize()>0) {
			Buff = theApp.m_strToolPath[TP_COORD][0];
			strSearch.Format(_T("%s%s"), theApp.m_strToolPath[TP_COORD][0],_T("%f,%f,%f,%f,%f,%f"));
		}
		if((nLength = rxString.Find(Buff,nStart)) == 0) {
			pCp->Zero();

			nStart = rxString.GetLength();
			if((nLength = rxString.Find(_T(","),nStart)) > 0) {
				Buff = rxString.Mid(nStart,nLength-nStart);
				 _WTOF(Buff,pCp->Side0.fX);
			}
			nStart = nLength + 1;

			if((nLength = rxString.Find(_T(","),nStart)) > 0) {
				Buff = rxString.Mid(nStart,nLength-nStart);
				 _WTOF(Buff,pCp->Side0.fY);
			}
			nStart = nLength + 1;

			if((nLength = rxString.Find(_T(","),nStart)) > 0) {
				Buff = rxString.Mid(nStart,nLength-nStart);
				 _WTOF(Buff,pCp->Side0.fZ);
			}
			nStart = nLength + 1;

			if((nLength = rxString.Find(_T(","),nStart)) > 0) {
				Buff = rxString.Mid(nStart,nLength-nStart);
				 _WTOF(Buff,pCp->Side0.fI);
			}
			nStart = nLength + 1;

			if((nLength = rxString.Find(_T(","),nStart)) > 0) {
				Buff = rxString.Mid(nStart,nLength-nStart);
				 _WTOF(Buff,pCp->Side0.fJ);
			}
			nStart = nLength + 1;

			Buff = rxString.Right(rxString.GetLength()-nStart);
			_WTOF(Buff,pCp->Side0.fK);
			bFlag = true;
		}
		break;
	case 1:
		if(theApp.m_strToolPath[TP_COORD].GetSize()>0) {
			Buff = theApp.m_strToolPath[TP_COORD][0];
			strSearch.Format(_T("%s%s"), theApp.m_strToolPath[TP_COORD][0],_T("%f,%f,%f,%f,%f,%f"));
		}
		if((nLength = rxString.Find(Buff,nStart)) == 0) {

			pCp->Zero();

			swscanf_s(rxString.GetBuffer(100),strSearch,&pCp->Side0.fX,&pCp->Side0.fY,&pCp->Side0.fZ,&pCp->Side0.fI,&pCp->Side0.fJ,&pCp->Side0.fK);

			bFlag = true;
		}
		break;
	case 2:
		strSearch.Format(_T("%s"), _T("%f %f %f %f %f %f"));
		pCp->Zero();
		swscanf_s(rxString.GetBuffer(100),strSearch,&pCp->Side0.fX,&pCp->Side0.fY,&pCp->Side0.fZ,&pCp->Side0.fI,&pCp->Side0.fJ,&pCp->Side0.fK);
		bFlag = true;
		break;
	case 3:
		strSearch.Format(_T("%s"), _T("%f\t%f\t%f"));
		pCp->Zero();
		swscanf_s(rxString.GetBuffer(100),strSearch,&pCp->Side0.fX,&pCp->Side0.fY,&pCp->Side0.fZ);
		pCp->Side0.norm = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f);
		bFlag = true;
		break;
	case 4:
		strSearch.Format(_T("%s"), _T("%f,%f,%f,%f,%f,%f"));
		pCp->Zero();
		swscanf_s(rxString.GetBuffer(100),strSearch,&pCp->Side0.fX,&pCp->Side0.fY,&pCp->Side0.fZ,&pCp->Side0.fI,&pCp->Side0.fJ,&pCp->Side0.fK);
		bFlag = true;
		break;
	case 5:
		strSearch.Format(_T("%s"), _T("%f,%f,%f"));
		pCp->Zero();
		swscanf_s(rxString.GetBuffer(100),strSearch,&pCp->Side0.fX,&pCp->Side0.fY,&pCp->Side0.fZ);
		pCp->Side0.norm = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f);
		bFlag = true;
		break;
	}

	if(theApp.m_LastSettings.nImportAptUnits==1) {
		pCp->Side0.fX *= MMPERINCH;
		pCp->Side0.fY *= MMPERINCH;
		pCp->Side0.fZ *= MMPERINCH;
	}

	theApp.m_Kinematics.TipsFromNormals((CCoord*)pCp,PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals((CCoord*)pCp,STARBOARD);

	pCp->CopySides(1,0);
	pCp->InvertNorm(1);


	return bFlag;
}

void CImportToolPathsDlg::OnButtonImport() 
{
	if(theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_ASCII) {
		ImportAscii();
	}
	if(theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_APTSOURCE) {
		ImportAptSource();
	}
	if(theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_CLOUDSET) {
		ImportCloudSet();
	}


}

void CImportToolPathsDlg::ImportAptSource() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPolyCoord* ToolPath = NULL;
	CPolyCoord	Ancillary;
	CPolyCoord	Envelope;
	bool bToolPathStarted = false;
	bool bReferenceStarted = false;
	bool bAncillaryStarted = false;
	bool bEnvelopeStarted = false;
	int	nLine=0;
	int	nEOL=0;
	int	ii,nRef;
	CCoord Cp;
	CString Buff;


	m_ProgressBar.SetRange(0,m_nToolPathL);
	m_ProgressBar.SetPos(0);

	if(m_nToolPathL > 0) {
		ToolPath = new CPolyCoord[m_nToolPathL];

		for(ii=0;(ii<m_nLength) && (nLine<m_nToolPathL);ii++) {

			m_pFlag[ii] &= ~HIGHLIGHTED;

			switch(m_pFlag[ii]) {
			case 0:
				if(bToolPathStarted == true) {
					if(ExtractCoordinate((CString)m_str.GetAt(ii),&Cp) == true) {
						ToolPath[nLine] + (Cp);
					}
				}
				break;
			case START_TOOLPATH:	bToolPathStarted = true;
				break;
			case EOL_TOOLPATH: nEOL++;
				nLine = nEOL / theApp.m_strToolPath[TP_EOL].GetSize();
				m_ProgressBar.SetPos(nLine);
				break;
			default:	bToolPathStarted = false;
				break;
			}
		}
		m_ProgressBar.SetPos(0);

		PROFILE->New();

		switch(theApp.m_LastSettings.nImportToolPathDest) {
		case 0:
			PROFILE->m_TaughtLine = new CPolyCoord[m_nToolPathL];
			for(nLine=0;nLine<m_nToolPathL;nLine++) {
				PROFILE->m_TaughtLine[nLine] = &ToolPath[nLine];
			}
			PROFILE->m_nTaughtLineL = m_nToolPathL;
			if(theApp.m_LastSettings.nImportAptDirection == 1) PROFILE->ReverseLines(TAUGHTLINES,2);
			break;
		case 1:
			PROFILE->m_ScanLine = new CPolyCoord[m_nToolPathL];
			for(nLine=0;nLine<m_nToolPathL;nLine++) {
				PROFILE->m_ScanLine[nLine] = &ToolPath[nLine];
			}
			PROFILE->m_nScanLineL = m_nToolPathL;
			if(theApp.m_LastSettings.nImportAptDirection == 1) PROFILE->ReverseLines(SCANLINES,2);
			break;
		}

		//Find Reference pts
		
		for(ii=0,nRef=0;(ii<m_nLength) && (nRef<3);ii++) {

			m_pFlag[ii] &= ~HIGHLIGHTED;

			switch(m_pFlag[ii]) {
			case START_REF_PTS:	bReferenceStarted = true;
				break;
			case 0:
				if(bReferenceStarted == true) {
					if(ExtractCoordinate((CString)m_str.GetAt(ii),&Cp) == true) {
						PROFILE->m_CpCadHole[nRef] = Cp;
						nRef++;
					}
				}
				break;
			default:	bReferenceStarted = false;
				break;
			}
		}
		

		ImportEnvelopes();

		
		//Find Ancillary pts
		Ancillary.Zero();

		for(ii=0;ii<m_nLength;ii++) {

			m_pFlag[ii] &= ~HIGHLIGHTED;

			switch(m_pFlag[ii]) {
			case 0:
				if(bAncillaryStarted == true) {
					if(ExtractCoordinate((CString)m_str.GetAt(ii),&Cp) == true) {
						Ancillary + (Cp);
					}
				}
				break;
			case START_ANC_PTS:	bAncillaryStarted = true;
				break;
			default:
				if(bAncillaryStarted == true ) {
					ii=m_nLength;
				}
				bAncillaryStarted = false;
				break;
			}
		}

		if(Ancillary.m_nCoordL>0) {
			PROFILE->AddLine(ENVELOPELINES, &Ancillary,0);
		}
		
		
		pFrame->SendMessage(UI_UPDATE_PROFILE_DLG,(WPARAM)this, NULL);
	}

	SAFE_DELETE_ARRAY( ToolPath );
}




void CImportToolPathsDlg::OnImportGotoToolpathstart() 
{
	for(int ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & START_TOOLPATH) {
			m_nTopRow = ii;
			Invalidate(FALSE);
			return;
		}
	}
	MessageBox(_T("No start can be recognised"),_T("Error"),MB_ICONERROR);
}

void CImportToolPathsDlg::OnImportGotoToolpathfinish() 
{
	for(int ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & FINISH_TOOLPATH) {
			m_nTopRow = ii;
			Invalidate(FALSE);
			return;
		}
	}
	MessageBox(_T("No Finish can be recognised"),_T("Error"),MB_ICONERROR);
}

void CImportToolPathsDlg::OnImportGotoRefpoints() 
{
	for(int ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & START_REF_PTS) {
			m_nTopRow = ii;
			Invalidate(FALSE);
			return;
		}
	}
	MessageBox(_T("No Reference points can be recognised"),_T("Error"),MB_ICONERROR);
	
}

void CImportToolPathsDlg::OnImportGotoAncillarypoints() 
{
	for(int ii=0;ii<m_nLength;ii++) {
		if(m_pFlag[ii] & START_ANC_PTS) {
			m_nTopRow = ii;
			Invalidate(FALSE);
			return;
		}
	}
	MessageBox(_T("No Ancillary points can be recognised"),_T("Error"),MB_ICONERROR);
}





void CImportToolPathsDlg::OnSelchangeComboDirection() 
{
	theApp.m_LastSettings.nImportAptDirection = m_comboDirection.GetCurSel();
}

void CImportToolPathsDlg::OnChangeEditSlowIncrement() 
{
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
		UpdateAllControls();
		UpdateMainFrameTools();
	}
}

void CImportToolPathsDlg::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	UpdateMainFrameTools();
	
	*pResult = 0;
}

void CImportToolPathsDlg::UpdateMainFrameTools()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(	pFrame->m_pMotionToolsSheet ) {
		pFrame->m_pMotionToolsSheet->UpdateAllPages();
	}

}


void CImportToolPathsDlg::ImportAscii() 
{
	int nLine,nIndex,nDest,ii,nRef;
	bool bReferenceStarted;
	CCoord Cp;
	CPolyCoord Line;

	m_ProgressBar.SetRange(0,m_nToolPathL);

	Cp.Zero();

	PROFILE->New();

	switch(theApp.m_LastSettings.nImportToolPathDest) {
	case 0:
		PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=m_nToolPathL];
		break;
	case 1:
		PROFILE->m_ScanLine = new CPolyCoord[PROFILE->m_nScanLineL=m_nToolPathL];
		break;
	}


	nLine = 0;
	nDest = 0;
	Line.Zero();
	Line.AllocateCoords(10000);

	//Find start
	for(nIndex=0;nIndex<m_nLength;nIndex++) {
		if(m_pFlag[nIndex] == START_TOOLPATH) break;
	}

	for(++nIndex ; nIndex<m_nLength ;nIndex++) {

		if(ExtractCoordinate((CString)m_str.GetAt(nIndex),&Cp) == true) {

			theApp.m_Kinematics.TipsFromNormals(&Cp,PORTSIDE);
			Cp.CopySides(1,0);

			if((m_pFlag[nIndex] & EOL_TOOLPATH) || (m_pFlag[nIndex] & FINISH_TOOLPATH)) {
				Line.SetSizeToMaxAccessed();
				switch(theApp.m_LastSettings.nImportToolPathDest) {
				case 0:
					PROFILE->m_TaughtLine[nLine++]= &Line;
					break;
				case 1:
					PROFILE->m_ScanLine[nLine++] = &Line;
					break;
				}
				m_ProgressBar.SetPos(nLine);
				nDest = 0;
				Line.Zero();
				Line.AllocateCoords(10000);
			}
			if(m_pFlag[nIndex] & FINISH_TOOLPATH) break;

			Line.ModifyCoord(nDest++,Cp);
		}
	}
	m_ProgressBar.SetPos(0);

	//Find ref points
	for(ii=0,nRef=0;(ii<m_nLength) && (nRef<3);ii++) {

		m_pFlag[ii] &= ~HIGHLIGHTED;

		switch(m_pFlag[ii]) {
		case START_REF_PTS:	bReferenceStarted = true;
			break;
		case 0:
			if(bReferenceStarted == true) {
				if(ExtractCoordinate((CString)m_str.GetAt(ii),&Cp) == true) {
					PROFILE->m_CpCadHole[nRef] = Cp;
					nRef++;
				}
			}
			break;
		default:	bReferenceStarted = false;
			break;
		}
	}


}

void CImportToolPathsDlg::ImportCloudSet() 
{
	int nIndex,nDest;
	CCoord Cp;
	CPolyCoord Line;

	m_ProgressBar.SetRange(0,m_nLength);

	Cp.Zero();

	PROFILE->New();

	Line.Zero();
	Line.AllocateCoords(m_nLength);


	for(nIndex=0, nDest = 0 ; nIndex<m_nLength ;nIndex++) {

		if(ExtractCoordinate((CString)m_str.GetAt(nIndex),&Cp) == true) {
			Cp.CopySides(1,0);
			Line.ModifyCoord(nDest++,Cp);
		}
		m_ProgressBar.SetPos(nIndex);
	}

	switch(theApp.m_LastSettings.nImportToolPathDest) {
	case 0:
		PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=1];
		PROFILE->m_TaughtLine[0]= &Line;
		break;
	case 1:
		PROFILE->m_ScanLine = new CPolyCoord[PROFILE->m_nScanLineL=1];
		PROFILE->m_ScanLine[0] = &Line;
		break;
	}


}

void CImportToolPathsDlg::OnGetdispinfoListEop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn;

	nn = pDispInfo->item.iItem;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		if(nn<theApp.m_strToolPath[TP_ENV_START].GetSize()) {
			swprintf_s(str,100,_T("%s"), (LPCWSTR)theApp.m_strToolPath[TP_ENV_START].GetAt(nn));
		} else {
			str[0]=0;
		}
		break;
	case 2:
		if(nn<theApp.m_strToolPath[TP_ENV_FINISH].GetSize()) {
			swprintf_s(str,100,_T("%s"),(LPCWSTR)theApp.m_strToolPath[TP_ENV_FINISH].GetAt(nn));
		} else {
			str[0]=0;
		}
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CImportToolPathsDlg::OnClickListEop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CImportToolPathsDlg::OnDblclkListEop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CImportToolPathsDlg::CreateColumnsEOP()
{
	CRect rr;
	int ii;
	int ColumnWidth[10] = { 0,170,170};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Start);
	ColumnName[2].LoadString(IDS_Finish);

	m_listEOP.GetWindowRect(&rr);
	rr.right-=22;
	ColumnWidth[1] = rr.Width()/2;
	ColumnWidth[2] = rr.Width()/2;

	for (ii=0;ii<3;ii++) {
		m_listEOP.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listEOP.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CImportToolPathsDlg::FillListEOP()
{
	CString Buff;
	int nSize;

	nSize = theApp.m_strToolPath[TP_ENV_START].GetSize();
	if(nSize < theApp.m_strToolPath[TP_ENV_FINISH].GetSize()) {
		nSize = theApp.m_strToolPath[TP_ENV_FINISH].GetSize();
	}

	m_listEOP.DeleteAllItems();
	for (int ii=0;ii<nSize;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listEOP.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listEOP.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<nSize;ii++) 
		m_listEOP.SetItemState(ii , 0,LVIS_SELECTED);
	m_listEOP.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listEOP.SetFocus();


}

void CImportToolPathsDlg::OnRclickListEop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listEOP.GetFirstSelectedItemPosition();
	m_nIndex=m_listEOP.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_EOP_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	*pResult = 0;
}

void CImportToolPathsDlg::OnEopDelete() 
{
	if(m_nIndex<theApp.m_strToolPath[TP_ENV_START].GetSize()) theApp.m_strToolPath[TP_ENV_START].RemoveAt(m_nIndex,1);
	if(m_nIndex<theApp.m_strToolPath[TP_ENV_FINISH].GetSize()) theApp.m_strToolPath[TP_ENV_FINISH].RemoveAt(m_nIndex,1);
	FillListEOP();
}

void CImportToolPathsDlg::OnCheckLineNumbers() 
{
	m_checkLineNumbers.GetCheck() & 1 ?	theApp.m_LastSettings.nImportDesignRulesMode |= DISPLAY_LINE_NUMBERS : theApp.m_LastSettings.nImportDesignRulesMode &= ~DISPLAY_LINE_NUMBERS;
}

void CImportToolPathsDlg::OnButtonImportAptsource() 
{
	theApp.m_LastSettings.nImportDesignRulesMode &= ~(IMPORT_APTSOURCE | IMPORT_ASCII | IMPORT_CLOUDSET);
	theApp.m_LastSettings.nImportDesignRulesMode |= IMPORT_APTSOURCE;
	SetToolBarCheckedState();
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::OnButtonImportAscii() 
{
	theApp.m_LastSettings.nImportDesignRulesMode &= ~(IMPORT_APTSOURCE | IMPORT_ASCII | IMPORT_CLOUDSET);
	theApp.m_LastSettings.nImportDesignRulesMode |= IMPORT_ASCII;
	SetToolBarCheckedState();
	SetFlags();
	UpdateAllControls();

}

void CImportToolPathsDlg::OnButtonImportCloudset() 
{
	theApp.m_LastSettings.nImportDesignRulesMode &= ~(IMPORT_APTSOURCE | IMPORT_ASCII | IMPORT_CLOUDSET);
	theApp.m_LastSettings.nImportDesignRulesMode |= IMPORT_CLOUDSET;
	SetToolBarCheckedState();
	SetFlags();
	UpdateAllControls();
}

void CImportToolPathsDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndImportToolBar.CommandToIndex(ID_BUTTON_IMPORT_ASCII))>=0)
		theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_ASCII ? m_wndImportToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndImportToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndImportToolBar.CommandToIndex(ID_BUTTON_IMPORT_APTSOURCE))>=0)
		theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_APTSOURCE ? m_wndImportToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndImportToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndImportToolBar.CommandToIndex(ID_BUTTON_IMPORT_CLOUDSET))>=0)
		theApp.m_LastSettings.nImportDesignRulesMode & IMPORT_CLOUDSET ? m_wndImportToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndImportToolBar.SetButtonStyle(nIndex,0);


}

void CImportToolPathsDlg::OnSelchangeComboExtractMethod() 
{
	theApp.m_LastSettings.nImportExtractionMethod = m_comboExtractionMethod.GetCurSel();
}


void CImportToolPathsDlg::OnButtonImportEnvelope() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	ImportEnvelopes();
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG,(WPARAM)this,NULL);
	
}

void CImportToolPathsDlg::ImportEnvelopes()
{
	CPolyCoord	Envelope;
	bool bEnvelopeStarted = false;
	int	ii;
	CCoord Cp;
	CString Buff;

	int nExtraction_method = theApp.m_LastSettings.nImportExtractionMethod;
		//Find Envelope pts
		int nEe=0;
		int nNumberEnvelopes = theApp.m_strToolPath[TP_ENV_START].GetSize();
		if(nNumberEnvelopes>0) {
			PROFILE->m_EnvelopeLine = new CPolyCoord[ PROFILE->m_nEnvelopeLineL=nNumberEnvelopes  ];
			for(ii=0;ii<PROFILE->m_nEnvelopeLineL;ii++) PROFILE->m_EnvelopeLine[ii].Zero();
		} else {
			return;
		}

		bEnvelopeStarted = false;
		Envelope.Zero();

		for(ii=0 ;ii<m_nLength;ii++) {

			m_pFlag[ii] &= ~HIGHLIGHTED;

			switch(m_pFlag[ii]) {
			case 0:
				if(bEnvelopeStarted == true) {
					if(ExtractCoordinate((CString)m_str.GetAt(ii),&Cp) == true) {
						Envelope + (Cp);
					}
				}
				break;
			case START_ENVELOPE: 
				if(bEnvelopeStarted == true) {
					if(Envelope.m_nCoordL>0) {
						PROFILE->m_EnvelopeLine[nEe] = &Envelope;
						PROFILE->m_EnvelopeLine[nEe].SortNearestAdjacent();
						Buff.Format(L"%d",nEe + 1);
						PROFILE->m_EnvelopeLine[nEe].SetString(Buff);
						nEe++;
					}
					Envelope.Zero();
				}
				bEnvelopeStarted = true;
				break;
			default:
				if(bEnvelopeStarted == true) {
					if(Envelope.m_nCoordL>0) {
						PROFILE->m_EnvelopeLine[nEe] = &Envelope;
						PROFILE->m_EnvelopeLine[nEe].SortNearestAdjacent();
						Buff.Format(L"%d",nEe + 1);
						PROFILE->m_EnvelopeLine[nEe].SetString(Buff);
						nEe++;
					}
					Envelope.Zero();
				}
				bEnvelopeStarted = false;
				break;
			}
		}

	theApp.m_LastSettings.nImportExtractionMethod = nExtraction_method;

}
