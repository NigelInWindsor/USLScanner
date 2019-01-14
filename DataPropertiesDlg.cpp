// DataPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "DataPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	_LIST_TABLE			0x01
#define	_CLASSIC			0x02
#define	_LANGUAGE0			0x04
#define	_LANGUAGE1			0x08

#define	ID_EDIT		40000

/////////////////////////////////////////////////////////////////////////////
// CDataPropertiesDlg dialog


CDataPropertiesDlg::CDataPropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, CData *pData)
	: CDialog(CDataPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataPropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pData = pData;
	
	m_nIndex = 0;
	m_pEditSpinItem = NULL;
	m_pFont = NULL;
	m_pStatic = NULL;
	m_pEdit = NULL;
	m_nTableFormat = 0;


	m_nFontHeight[0]=8;
	m_nFontHeight[1]=9;
	m_nFontHeight[2]=10;
	m_nFontHeight[3]=11;
	m_nFontHeight[4]=12;
	m_nFontHeight[5]=14;
	m_nFontHeight[6]=16;
	m_nFontHeight[7]=18;
	m_nFontHeight[8]=20;
	m_nFontHeight[9]=22;
	m_nFontHeight[10]=24;
	m_nFontHeight[11]=26;
	m_nFontHeight[12]=28;
	m_nFontHeight[13]=30;
	m_nFontHeight[14]=34;
	m_nFontHeight[15]=38;
	m_nFontHeight[16]=42;
}


void CDataPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataPropertiesDlg)
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Control(pDX, IDC_STATIC_FONT_SIZE, m_staticFontSize);
	DDX_Control(pDX, IDC_COMBO_FONT_SIZE, m_comboFontSize);
	DDX_Control(pDX, IDC_LIST_DETAILS, m_listDetails);
	DDX_Control(pDX, IDC_EDIT_DATE, m_editDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CDataPropertiesDlg)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DETAILS, OnGetdispinfoListDetails)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DETAILS, OnClickListDetails)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DETAILS, OnDblclkListDetails)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DETAILS, OnRclickListDetails)
	ON_COMMAND(ID_DATA_ADD, OnDataAdd)
	ON_COMMAND(ID_DATA_DELETE, OnDataDelete)
	ON_COMMAND(ID_DATA_INSERT, OnDataInsert)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_SIZE, OnSelchangeComboFontSize)
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_LANGUAGE_0, OnButtonLanguage0)
	ON_COMMAND(ID_BUTTON_LANGUAGE_1, OnButtonLanguage1)
	ON_COMMAND(ID_BUTTON_CLASSIC, OnButtonClassic)
	ON_COMMAND(ID_BUTTON_TABLE, OnButtonTable)
	ON_COMMAND(ID_DATA_AUTOOPERATOR, OnDataAutooperator)
	ON_COMMAND(ID_DATA_AUTOINSPECTIONDATE, OnDataAutoinspectiondate)
	ON_COMMAND(ID_DATA_AUTOFASTSTEPSIZE, OnDataAutofaststepsize)
	ON_COMMAND(ID_DATA_AUTOSLOWSTEPSIZE, OnDataAutoslowstepsize)
	ON_COMMAND(ID_DATA_AUTOCLEAR, OnDataAutoclear)
	ON_COMMAND(ID_BUTTON_ERASE, OnButtonErase)
	ON_COMMAND(ID_BUTTON_FILL, OnButtonFill)
	ON_COMMAND(ID_BUTTON_EXPORT_TO_WORD, OnButtonExportToWord)
	ON_COMMAND(ID_BUTTON_EXPORT_TEXT, OnButtonExportText)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
int CDataPropertiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndFileToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndFileToolBar.LoadToolBar(IDR_DATA_PROPERTIES_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create porosity ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndFileToolBar);
	
	return 0;
}

// CDataPropertiesDlg message handlers
void CDataPropertiesDlg::PostNcDestroy() 
{
	SAFE_DELETE(m_pFont);
	SAFE_DELETE_ARRAY( m_pStatic );
	SAFE_DELETE_ARRAY( m_pEdit );
	
	CDialog::PostNcDestroy();
	delete this;
}

void CDataPropertiesDlg::OnClose() 
{
	if(theApp.m_LastSettings.nScanMask & _PROPERTY_PAGE_MUST_BE_COMPLETE) {
		if(theApp.m_pScanData) {
			if(theApp.m_pScanData->m_DataProperties.IsAllEntriesComplete() == false) {
				MessageBox(L"Not all entries are complete\r\nWindow can not be closed",L"Error",MB_ICONERROR);
				return;
			}
		}
	}

	theApp.m_LastSettings.bWindowOpenOnLastExit[CDataPropertiesDlg::IDD % MAXIMUMWINDOWSIDNUMBER] = FALSE;
	
	SaveColumnWidths();
	CDialog::OnClose();
	DestroyWindow();
}

void CDataPropertiesDlg::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CDataPropertiesDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();

	*m_pDlg	= NULL;	
	
}

BOOL CDataPropertiesDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	
	CreateFont();
	m_StaticPosList.Initialize(this, m_listDetails, TRUE, TRUE, TRUE, TRUE);

	CRect rect = theApp.m_LastSettings.RectWindowPos[CDataPropertiesDlg::IDD % MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
	SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);

	m_StaticPosList.SetNewPos();

	for(int ii=0;ii<17;ii++) {
		Buff.Format(L"%d",m_nFontHeight[ii]);
		m_comboFontSize.AddString(Buff);
	}

	SetDlgLayout();

	m_listDetails.SetFont(m_pFont);

	CreateColumns();
	
	UpdateAllControls();

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataPropertiesDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(theApp.m_LastSettings.nDataPropertiesMask & _LIST_TABLE) {
		m_StaticPosList.SetNewPos();
	}
}

void CDataPropertiesDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	int nn;

	CTime time(m_pData->m_osTestDateTime);
	Buff = time.Format(_T("%d %b %Y %Hh%M"));
	m_editDate.SetWindowText(Buff);

	m_comboFontSize.SetCurSel(theApp.m_LastSettings.nDataPropertiesFontHeight);

	if(	theApp.m_LastSettings.nDataPropertiesMask	& _LIST_TABLE) {
		FillList();
	}

	if(theApp.m_LastSettings.nDataPropertiesMask & _CLASSIC) {
		for(nn=0;nn<m_pData->m_DataProperties.GetSize();nn++) {
			if(m_pEdit[nn]) {
				if(m_pEdit[nn].m_hWnd) {
					Buff = m_pData->m_DataProperties.GetValueAt(nn);
					m_pEdit[nn].SetWindowText(Buff);
				}
			}
		}



	}

	FillList();
	SetToolBarCheckedState();
	SetAccessPrivelages();
}



void CDataPropertiesDlg::SetDlgLayout()
{
	CString Buff;
	CRect rr;
	CSize size;
	int nMaxWidth = 0;
	int nTop,nn;
	int nBoarder = 6;

	SAFE_DELETE_ARRAY( m_pStatic );
	SAFE_DELETE_ARRAY( m_pEdit );

	if(theApp.m_LastSettings.nDataPropertiesMask & _LIST_TABLE) {
		m_staticDate.ShowWindow(true);
		m_staticFontSize.ShowWindow(true);
		m_editDate.ShowWindow(true);
		m_comboFontSize.ShowWindow(true);
		m_listDetails.ShowWindow(true);
		m_listDetails.SetFont(m_pFont);

		CreateColumns();
		FillList();

		return;
	}

	if(theApp.m_LastSettings.nDataPropertiesMask & _CLASSIC) {
		if(m_pData->m_DataProperties.GetSize() <= 0) return;

		m_staticDate.ShowWindow(false);
		m_staticFontSize.ShowWindow(false);
		m_editDate.ShowWindow(false);
		m_comboFontSize.ShowWindow(false);
		m_listDetails.ShowWindow(false);

		CDC *pDC = GetDC();
		CFont *pOldFont = pDC->SelectObject(m_pFont);

		Buff = "HELLO";
		size = pDC->GetTextExtent(Buff);
		size.cx = 0;

		m_pStatic = new CStatic[m_pData->m_DataProperties.GetSize()];
		m_pEdit = new CEdit[m_pData->m_DataProperties.GetSize()];
		rr.left=10;
		nTop = rr.top=32;


		for(nn=0;nn<m_pData->m_DataProperties.GetSize();nn++) {
			if(m_pStatic[nn].m_hWnd==NULL) {
				Buff.Empty();
				if((theApp.m_LastSettings.nDataPropertiesMask & (_LANGUAGE0 | _LANGUAGE1)) == (_LANGUAGE0 | _LANGUAGE1)) {
					Buff.Format(L"%s / %s", m_pData->m_DataProperties.GetCaptionAt(0,nn) ,  m_pData->m_DataProperties.GetCaptionAt(1,nn));
				} else {
					if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) {
						Buff = m_pData->m_DataProperties.GetCaptionAt(0,nn);
					}
					if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1) {
						Buff = m_pData->m_DataProperties.GetCaptionAt(1,nn);
					}
				}

				if(Buff.GetLength()>0) {
					size = pDC->GetTextExtent(Buff);
					if(size.cx > nMaxWidth) nMaxWidth = size.cx;
				}
				rr.right = rr.left + size.cx;
				rr.bottom = rr.top + size.cy + nBoarder;

				m_pStatic[nn].Create(Buff.GetBuffer(200),WS_CHILD|WS_VISIBLE,rr,this);
				m_pStatic[nn].SetFont(m_pFont);

				rr.top += size.cy + nBoarder;
			}
		}

		CRect WindowsRect; 
		GetClientRect(WindowsRect);

		int nExStyle = WS_EX_CLIENTEDGE;
		int nStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | BS_CENTER;
		rr.top = nTop;
		rr.left += (nMaxWidth + 4);
		rr.right = WindowsRect.right - 10;
		for(nn=0;nn<m_pData->m_DataProperties.GetSize();nn++) {
			if(m_pEdit[nn].m_hWnd==NULL) {
				Buff = m_pData->m_DataProperties.GetValueAt(nn);
				rr.bottom = rr.top + size.cy + nBoarder;

				m_pEdit[nn].CreateEx( nExStyle,L"EDIT",Buff.GetBuffer(200),nStyle,rr, this, ID_EDIT+nn);
				m_pEdit[nn].SetFont(m_pFont);

				rr.top += size.cy + nBoarder;
			}
		}

		GetWindowRect(WindowsRect);
		WindowsRect.bottom = WindowsRect.top + rr.top + 44;
		SetWindowPos( &wndTop , WindowsRect.left, WindowsRect.top, WindowsRect.Width(), WindowsRect.Height(), NULL);


		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
		SetAccessPrivelages();
		return;
	}
}

void CDataPropertiesDlg::CreateColumns()
{
	int ColumnWidth[10] = { 0,170,100,100,100,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0]= "#";
	CRect rr;
	int ii,nColumn;

	if((theApp.m_LastSettings.nDataPropertiesMask & (_LANGUAGE0 + _LANGUAGE1)) == 0)							m_nTableFormat = 0;
	if((theApp.m_LastSettings.nDataPropertiesMask & (_LANGUAGE0 + _LANGUAGE1)) == _LANGUAGE0)					m_nTableFormat = 1;
	if((theApp.m_LastSettings.nDataPropertiesMask & (_LANGUAGE0 + _LANGUAGE1)) == _LANGUAGE1)					m_nTableFormat = 2;
	if((theApp.m_LastSettings.nDataPropertiesMask & (_LANGUAGE0 + _LANGUAGE1)) == (_LANGUAGE0 | _LANGUAGE1))	m_nTableFormat = 3;


	for(ii=1;ii<4;ii++) {
		ColumnWidth[ii] = MinMax(&theApp.m_LastSettings.nParameterColumnWidth[ii],10,1000);
	}

	int nColumnCount = m_listDetails.GetHeaderCtrl()->GetItemCount();
	for (ii=0;ii < nColumnCount;ii++) m_listDetails.DeleteColumn(0);

	m_listDetails.GetWindowRect(rr);

	nColumn=0;
	m_listDetails.InsertColumn(nColumn,ColumnName[nColumn], LVCFMT_LEFT,ColumnWidth[nColumn]);
	nColumn++;
	if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) {
		ColumnName[nColumn].LoadString(IDS_Item);
		m_listDetails.InsertColumn(nColumn,ColumnName[nColumn], LVCFMT_LEFT,ColumnWidth[nColumn]);
		nColumn++;
	}
	if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1) {
		ColumnName[nColumn].LoadString(IDS_Item);
		m_listDetails.InsertColumn(nColumn,ColumnName[nColumn], LVCFMT_LEFT,ColumnWidth[nColumn]);
		nColumn++;
	}
	ColumnName[nColumn].LoadString(IDS_Value);
	m_listDetails.InsertColumn(nColumn,ColumnName[nColumn], LVCFMT_LEFT,ColumnWidth[nColumn]);
	nColumn++;

	m_listDetails.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


}

void CDataPropertiesDlg::FillList()
{
	COLORREF rgbRow[2] = {RGB(220,220,220), RGB(240, 240,240)};

	CString Buff;

	m_listDetails.Zero();
	m_listDetails.DeleteAllItems();
	for (int ii=0;ii<m_pData->m_DataProperties.GetSize();ii++) {
		Buff.Format(L"%d",ii+1);
		m_listDetails.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listDetails.EnsureVisible(m_nIndex,TRUE);
	m_listDetails.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listDetails.SetFocus();


	for (int ii=0;ii<m_pData->m_DataProperties.GetSize();ii++) {
		m_listDetails.SetBkRowColor(ii,rgbRow[ii&1]);
	};
}

void CDataPropertiesDlg::OnGetdispinfoListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn;

	if(m_pData->m_DataProperties.GetSize()<=0) return;
	nn = pDispInfo->item.iItem;

	switch(m_nTableFormat) {
	case 0:
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,100,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetValueAt(nn));
			break;
		}
		break;
	case 1:
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,100,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetCaptionAt(0,nn));
			break;
		case 2:
			swprintf_s(str,100,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetValueAt(nn));
			break;
		}
		break;
	case 2:
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetCaptionAt(1,nn));
			break;
		case 2:
			swprintf_s(str,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetValueAt(nn));
			break;
		}
		break;
	case 3:
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,L"%s",(LPCWSTR)m_pData->m_DataProperties.GetCaptionAt(0,nn));
			break;
		case 2:
			swprintf_s(str,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetCaptionAt(1,nn));
			break;
		case 3:
			swprintf_s(str,L"%s", (LPCWSTR)m_pData->m_DataProperties.GetValueAt(nn));
			break;
		}
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CDataPropertiesDlg::OnClickListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	
	*pResult = 0;
}

void CDataPropertiesDlg::OnDblclkListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;


	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex=pDispInfo->item.mask;

	if((m_nIndex >= m_pData->m_DataProperties.GetSize()) || (m_nIndex==-1)) {
		m_pData->m_DataProperties.Add(-1);
		m_nIndex = m_pData->m_DataProperties.GetSize() - 1;
		FillList();
	}

	if((m_nIndex>=0) && (m_nIndex<m_pData->m_DataProperties.GetSize())) {
		m_listDetails.GetWindowRect(CtrlRect);
		m_listDetails.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect,&m_nFontHeight[theApp.m_LastSettings.nDataPropertiesFontHeight]);

		switch(m_nTableFormat) {
		case 0:
			switch(m_nColumn = pDispInfo->item.iItem) {
			case 0:
				break;
			case 1:
				m_strBuff = m_pData->m_DataProperties.GetValueAt(m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
			break;
		case 1:
			switch(m_nColumn = pDispInfo->item.iItem) {
			case 0:
				break;
			case 1:
				m_strBuff = m_pData->m_DataProperties.GetCaptionAt(0,m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 2:
				m_strBuff = m_pData->m_DataProperties.GetValueAt(m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
			break;
		case 2:
			switch(m_nColumn = pDispInfo->item.iItem) {
			case 0:
				break;
			case 1:
				m_strBuff = m_pData->m_DataProperties.GetCaptionAt(1,m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 2:
				m_strBuff = m_pData->m_DataProperties.GetValueAt(m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
			break;
		case 3:
			switch(m_nColumn = pDispInfo->item.iItem) {
			case 0:
				break;
			case 1:
			case 2:
				m_strBuff = m_pData->m_DataProperties.GetCaptionAt(m_nColumn-1,m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 3:
				m_strBuff = m_pData->m_DataProperties.GetValueAt(m_nIndex);
				m_pEditSpinItem->Initialize(&m_strBuff);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
			break;
		}
	}
	*pResult = 0;
}

void CDataPropertiesDlg::OnRclickListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listDetails.GetFirstSelectedItemPosition();
	m_nIndex=m_listDetails.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_DATA_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}


void CDataPropertiesDlg::OnDataAdd() 
{
	m_pData->m_DataProperties.Add(-1);
	FillList();
}

void CDataPropertiesDlg::OnDataDelete() 
{
	// TODO: Add your command handler code here
	
}

void CDataPropertiesDlg::OnDataInsert() 
{
	m_pData->m_DataProperties.Add(m_nIndex);
	FillList();
}

HRESULT CDataPropertiesDlg::ItemChanged(WPARAM,LPARAM)
{
	switch(m_nTableFormat) {
	case 0:
		switch(m_nColumn) {
		case 0:
			break;
		case 1:	m_pData->m_DataProperties.SetValueAt(m_nIndex,m_strBuff);
			break;
		}
		break;
	case 1:
		switch(m_nColumn) {
		case 0:
			break;
		case 1:	m_pData->m_DataProperties.SetCaptionAt(0,m_nIndex,m_strBuff);
			break;
		case 2:	m_pData->m_DataProperties.SetValueAt(m_nIndex,m_strBuff);
			break;
		}
		break;
	case 2:
		switch(m_nColumn) {
		case 0:
			break;
		case 1:	m_pData->m_DataProperties.SetCaptionAt(1,m_nIndex,m_strBuff);
			break;
		case 2:	m_pData->m_DataProperties.SetValueAt(m_nIndex,m_strBuff);
			break;
		}
		break;
	case 3:
		switch(m_nColumn) {
		case 0:
			break;
		case 1:
		case 2:
			m_pData->m_DataProperties.SetCaptionAt(m_nColumn-1,m_nIndex,m_strBuff);
			break;
		case 3:	m_pData->m_DataProperties.SetValueAt(m_nIndex,m_strBuff);
			break;
		}
		break;
	}
	return NULL;
}




void CDataPropertiesDlg::OnFileOpen() 
{
	CString FilePath,Buff;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".pal"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data Templates (*.DataTemplate)|*.DataTemplate|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			m_pData->m_DataProperties.Retrieve(&File);
			if(&theApp.m_Data.m_DataProperties != &m_pData->m_DataProperties)
				theApp.m_Data.m_DataProperties = &m_pData->m_DataProperties;
			File.Close();
			SetDlgLayout();
			UpdateAllControls();
			Invalidate(false);
		}
	}

}

void CDataPropertiesDlg::OnFileSaveas() 
{
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;

	CFileDialog FileDlg(FALSE,_T(".pal"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data Templates (*.DataTemplate)|*.DataTemplate|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			m_pData->m_DataProperties.Save(&File);
			if(&theApp.m_Data.m_DataProperties != &m_pData->m_DataProperties)
				theApp.m_Data.m_DataProperties = &m_pData->m_DataProperties;

			File.Close();
		}
	}		
}

BOOL CDataPropertiesDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save_template);
	   break;
   case ID_FILE_OPEN: strTipText.LoadString(IDS_Open_template);
	   break;
   case ID_BUTTON_LANGUAGE_0:  strTipText.LoadString(IDS_Language_1);
	   break;
   case ID_BUTTON_LANGUAGE_1:  strTipText.LoadString(IDS_Language_2);
	   break;
   case ID_BUTTON_TABLE:  strTipText.LoadString(IDS_Editable_table);
	   break;
   case ID_BUTTON_CLASSIC:  strTipText.LoadString(IDS_Operator_classic_list);
	   break;
   case ID_BUTTON_ERASE:  strTipText.LoadString(IDS_Erase_all_values);
	   break;
   case ID_BUTTON_FILL:  strTipText.LoadString(IDS_Fill_automatic_values);
	   break;
   case ID_BUTTON_EXPORT_TO_WORD:  strTipText.LoadString(IDS_Export_to_Word);
	   break;
   case ID_BUTTON_EXPORT_TEXT:  strTipText.LoadString(IDS_Export_text_to_Word);
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

void CDataPropertiesDlg::OnSelchangeComboFontSize() 
{
	SaveColumnWidths();

	theApp.m_LastSettings.nDataPropertiesFontHeight = m_comboFontSize.GetCurSel();
	CreateFont();
	SetDlgLayout();
}

void CDataPropertiesDlg::CreateFont()
{
	SAFE_DELETE( m_pFont );

	MinMax(&theApp.m_LastSettings.nDataPropertiesFontHeight,0,16);

	CFont *pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	LOGFONT lf = m_LogFont;
	lf.lfHeight = m_nFontHeight[theApp.m_LastSettings.nDataPropertiesFontHeight];
	theApp.SetFontLanguage(&lf);
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&lf);

}


void CDataPropertiesDlg::SaveColumnWidths()
{
	int nColumnCount = m_listDetails.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		theApp.m_LastSettings.nParameterColumnWidth[ii] = m_listDetails.GetColumnWidth(ii);
	}

}


BOOL CDataPropertiesDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CString Buff;

	switch(nCode) {
	case 0x400:	//Text entered
		GetDlgItemText(nID,Buff);
		if((nID>=ID_EDIT) && (nID<(ID_EDIT + 100))) {
			m_pData->m_DataProperties.SetValueAt(nID - ID_EDIT,Buff);
			return TRUE;
		}
		break;
	case 0x000: //Button pressed and released
		break;

	}
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CDataPropertiesDlg::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:

			return TRUE;
			break;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDataPropertiesDlg::OnButtonLanguage0() 
{
	theApp.m_LastSettings.nDataPropertiesMask ^= _LANGUAGE0;
	SetToolBarCheckedState();
	SetDlgLayout();
}

void CDataPropertiesDlg::OnButtonLanguage1() 
{
	theApp.m_LastSettings.nDataPropertiesMask ^= _LANGUAGE1;
	SetToolBarCheckedState();
	SetDlgLayout();
	
}

void CDataPropertiesDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_LANGUAGE_0))>=0)
		theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0 ? m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndFileToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_LANGUAGE_1))>=0)
		theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1 ? m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndFileToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_TABLE))>=0)
		theApp.m_LastSettings.nDataPropertiesMask & _LIST_TABLE ? m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndFileToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_CLASSIC))>=0)
		theApp.m_LastSettings.nDataPropertiesMask & _CLASSIC ? m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndFileToolBar.SetButtonStyle(nIndex,0);

}

void CDataPropertiesDlg::OnButtonClassic() 
{
	SaveColumnWidths();

	theApp.m_LastSettings.nDataPropertiesMask	&= ~_LIST_TABLE;
	theApp.m_LastSettings.nDataPropertiesMask |= _CLASSIC;
	SetToolBarCheckedState();
	SetDlgLayout();
}

void CDataPropertiesDlg::OnButtonTable() 
{
	theApp.m_LastSettings.nDataPropertiesMask	&= ~_CLASSIC;
	theApp.m_LastSettings.nDataPropertiesMask |= _LIST_TABLE;
	SetToolBarCheckedState();
	SetDlgLayout();
}

void CDataPropertiesDlg::OnDataAutooperator() 
{
	m_pData->m_DataProperties.SetModeAt(m_nIndex, AUTO_OPERATOR);
	AutoFillAll();
	FillList();
}

void CDataPropertiesDlg::OnDataAutoinspectiondate() 
{
	m_pData->m_DataProperties.SetModeAt(m_nIndex, AUTO_INSPECTION_DATE);
	AutoFillAll();
	FillList();
}

void CDataPropertiesDlg::OnDataAutofaststepsize() 
{
	m_pData->m_DataProperties.SetModeAt(m_nIndex, AUTO_FAST_STEPSIZE);
	AutoFillAll();
	FillList();
}

void CDataPropertiesDlg::OnDataAutoslowstepsize() 
{
	m_pData->m_DataProperties.SetModeAt(m_nIndex, AUTO_SLOW_STEPSIZE);
	AutoFillAll();
	FillList();
}

void CDataPropertiesDlg::OnDataAutoclear() 
{
	CString Buff;

	Buff.Empty();
	m_pData->m_DataProperties.SetModeAt(m_nIndex, 0);
	m_pData->m_DataProperties.SetValueAt(m_nIndex, Buff);
	AutoFillAll();
	FillList();
}


void CDataPropertiesDlg::AutoFillAll()
{

	m_pData->AutoFillDataProperties();

}

void CDataPropertiesDlg::OnButtonErase() 
{
	CString Buff;

	Buff.Empty();
	for(int ii=0;ii<m_pData->m_DataProperties.GetSize();ii++) {
		m_pData->m_DataProperties.SetValueAt(ii,Buff);
	};
	UpdateAllControls();
}

void CDataPropertiesDlg::OnButtonFill() 
{
	AutoFillAll();
	UpdateAllControls();
	
}

void CDataPropertiesDlg::OnButtonExportToWord() 
{
	CRect rr;
	CDC  dcMem;
	CBitmap bmpScreen;

	GetWindowRect(rr);
	rr.DeflateRect(1,1);
	ScreenToClient(rr);
	CDC *pDCScreen = GetDC();

	bmpScreen.CreateCompatibleBitmap(pDCScreen,rr.Width(),rr.Height());
	dcMem.CreateCompatibleDC(pDCScreen);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmpScreen);

	dcMem.BitBlt(0,0,rr.Width(),rr.Height(),pDCScreen,rr.left,rr.top,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


	if ( OpenClipboard() )
	{
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,bmpScreen.m_hObject);
		CloseClipboard();
	}

	ReleaseDC(pDCScreen);

}

void CDataPropertiesDlg::OnButtonExportText() 
{
	HGLOBAL hClipboardData;
	WCHAR * pchData;
	CString Buff,Temp;
	int ii;

	Buff.Empty();

	for (ii=0;ii<m_pData->m_DataProperties.GetSize();ii++) {

		if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) {
			Temp = m_pData->m_DataProperties.GetCaptionAt(0,ii);
			Buff += Temp;
		}
		if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1) {
			Temp = m_pData->m_DataProperties.GetCaptionAt(1,ii);
			if(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) {
				Buff += (L" / " + Temp);
			} else {
				Buff += Temp;
			}
		}
		Temp = L"\t" + m_pData->m_DataProperties.GetValueAt(ii);
		Buff += Temp;
		Buff += "\r\n";
	}

	if ( OpenClipboard() )
	{
		EmptyClipboard();
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, (Buff.GetLength()+1) * sizeof WCHAR);
		pchData = (WCHAR*)GlobalLock(hClipboardData);
		for(ii=0;ii<Buff.GetLength();ii++) {
			pchData[ii] = Buff.GetAt(ii);
		}
		GlobalUnlock(hClipboardData);
		SetClipboardData(CF_UNICODETEXT,hClipboardData);
		CloseClipboard();
	}
	
}

void CDataPropertiesDlg::SetAccessPrivelages()
{
	int nn,nIndex;
	
	if((theApp.m_LastSettings.nScanMask & _LOCK_FILE_ON_SENTENCE) && (m_pData->m_nSentence>0)) {
		
		for(nn=0;nn<m_pData->m_DataProperties.GetSize();nn++) {
			if(m_pEdit[nn].m_hWnd!=NULL) {
				m_pEdit[nn].SetReadOnly(true);
			}
		}
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_FILE_SAVEAS))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_FILE_OPEN))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_ERASE))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_FILL))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,TBBS_DISABLED);

	} else {

		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_FILE_SAVEAS))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_FILE_OPEN))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_ERASE))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndFileToolBar.CommandToIndex(ID_BUTTON_FILL))>=0) m_wndFileToolBar.SetButtonStyle(nIndex,0);
	}
}
