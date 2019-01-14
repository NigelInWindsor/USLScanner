// DataBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DataBaseDlg.h"
#include "Token.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataBaseDlg dialog


CDataBaseDlg::CDataBaseDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CDataBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataBaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_nIndex = 0;
	m_nWhich = 0;
}


void CDataBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataBaseDlg)
	DDX_Control(pDX, IDC_COMBO_WHICH, m_comboWhich);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CDataBaseDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DATA, OnGetdispinfoListData)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATA, OnDblclkListData)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DATA, OnColumnclickListData)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH, OnSelchangeComboWhich)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataBaseDlg message handlers

void CDataBaseDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CDataBaseDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CDataBaseDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CDataBaseDlg::OnDestroy() 
{
	if (CDataBaseDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CDataBaseDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CDataBaseDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();

	m_StaticPosList.Initialize(this, m_listData, TRUE, TRUE, TRUE, TRUE);

	if (CDataBaseDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CDataBaseDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosList.SetNewPos();
	m_comboWhich.AddString(theApp.m_DataBasePathName);
	m_comboWhich.AddString(theApp.m_DataBaseFailedToSavePathName);
	m_comboWhich.SetCurSel(m_nWhich);

	RetrieveDataBase();

	

	CreateColumns();
	FillList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataBaseDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosList.SetNewPos();
	
}

void CDataBaseDlg::UpdateAllControls()
{

}

void CDataBaseDlg::CreateColumns()
{
	int ColumnWidth[3] = { 0,100,250};

	int nColumnCount = m_listData.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listData.DeleteColumn(0);
	}
	for (int ii=0;ii<m_nNumberColumns;ii++) {
		switch(ii) {
		case 0:
		case 1:
		case 2:	m_listData.InsertColumn(ii,m_Column[ii], LVCFMT_LEFT,ColumnWidth[ii]);
			break;
		default:	m_listData.InsertColumn(ii,m_Column[ii], LVCFMT_CENTER,120);
			break;
		}

	}
	m_listData.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CDataBaseDlg::FillList()
{
	CString Buff;

	m_listData.DeleteAllItems();
	for (int ii=0;ii<m_nDataL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listData.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listData.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<m_nDataL;ii++) 
		m_listData.SetItemState(ii , 0,LVIS_SELECTED);
	m_listData.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listData.SetFocus();

}

void CDataBaseDlg::OnGetdispinfoListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	if(m_nDataL<=0) return;

	int nIndex = m_nDataL-pDispInfo->item.iItem-1;
	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	default:
		swprintf_s(str,100,_T("%s"),(LPCWSTR)m_Data[pDispInfo->item.iSubItem].GetAt(pDispInfo->item.iItem));
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

bool CDataBaseDlg::RetrieveDataBase()
{
	CUSLFile File;
	CFileException e;
	CString Buff,FilePath,newtok;
	int nn,nCol;

	if(m_nWhich == 0) {
		FilePath = theApp.m_DataBasePathName;
	} else {
		FilePath = theApp.m_DataBaseFailedToSavePathName;
	}

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
		return FALSE;
	} else {
		CArchive* pArchFile;
		pArchFile = new CArchive(&File,CArchive::load);
		nn=0;
		m_nNumberColumns=0;
		m_nDataL=0;

		while(pArchFile->ReadString(Buff)) {
			if(nn==0) {
				CToken tok(Buff);
				tok.SetToken(L",");
				
				m_Column.Add(_T("#"));
				m_nNumberColumns=1;
				while(tok.MoreTokens())
				{
					newtok = tok.GetNextToken();
					m_Column.Add(newtok);
					m_nNumberColumns++;
				}
				nn++;
			} else {
				CToken tok(Buff);
				tok.SetToken(L",");
				
				newtok.Format(_T("%d"),nn);
				m_Data[0].Add(newtok);
				nCol=1;
				while(tok.MoreTokens() && (nCol < m_nNumberColumns))
				{
					newtok = tok.GetNextToken();
					m_Data[nCol].Add(newtok);
					nCol++;
				}
				for(nCol;nCol<m_nNumberColumns;nCol++) {
					m_Data[nCol].Add(L"");
				}
				m_nDataL++;
			}

		}

		pArchFile->Close();
		delete pArchFile;
		File.Close();
		Sort();

		return TRUE;
	}


}

void CDataBaseDlg::OnDblclkListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString FilePathName;

	m_nIndex=pDispInfo->item.mask;	

	FilePathName = m_Data[2].GetAt(m_nIndex);
	pFrame->FileView(FilePathName);

	*pResult = 0;
}

void CDataBaseDlg::OnColumnclickListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nColumn = pNMListView->iSubItem;
	if(nColumn == theApp.m_LastSettings.nDataBaseSortColumn) {
		theApp.m_LastSettings.nDataBaseSortDir > 0 ? theApp.m_LastSettings.nDataBaseSortDir = -1 : theApp.m_LastSettings.nDataBaseSortDir = 1;
	} else {
		theApp.m_LastSettings.nDataBaseSortColumn = nColumn;
	}

	Sort();

	Invalidate(FALSE);
	*pResult = 0;
}

void CDataBaseDlg::OnSelchangeComboWhich() 
{

	m_nWhich = m_comboWhich.GetCurSel();

	for(int ii=0;ii<100;ii++) {
		m_Data[ii].RemoveAll();
	}

	RetrieveDataBase();

	FillList();

}

void CDataBaseDlg::Sort()
{
	CString Src[100],Dest,Temp;
	int	ii,jj,gg;
	int nColumn = theApp.m_LastSettings.nDataBaseSortColumn;

	for(jj=1;jj<m_nDataL;jj++) {
		for(gg=0;gg<m_nNumberColumns;gg++)
			Src[gg] = m_Data[gg].GetAt(jj);
		for(ii=jj-1;ii>=0;ii--) {
			Dest = m_Data[nColumn].GetAt(ii);
			if(theApp.m_LastSettings.nDataBaseSortDir > 0) {
				if(Dest.CompareNoCase(Src[nColumn])>0) goto label_10;
			} else {
				if(Dest.CompareNoCase(Src[nColumn])<0) goto label_10;
			}
			for(gg=0;gg<m_nNumberColumns;gg++) 
				m_Data[gg].SetAt(ii+1,m_Data[gg].GetAt(ii));
		}
		ii=-1;
label_10:
		for(gg=0;gg<m_nNumberColumns;gg++) 
			m_Data[gg].SetAt(ii+1,Src[gg]);
	}

}
