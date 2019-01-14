// IGESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "IGESDlg.h"
#include "NURBS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIGESDlg dialog


CIGESDlg::CIGESDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CIGESDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIGESDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_nIndex=0;
	m_nPaintOption=0;

}


void CIGESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIGESDlg)
	DDX_Control(pDX, IDC_EDIT_PARAM, m_editParam);
	DDX_Control(pDX, IDC_EDIT_DATA, m_editData);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_LIST_ENTITY, m_listEntity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIGESDlg, CDialog)
	//{{AFX_MSG_MAP(CIGESDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ENTITY, OnGetdispinfoListEntity)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENTITY, OnClickListEntity)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_DRAWING, OnButtonDrawing)
	ON_COMMAND(ID_BUTTON_GLOBAL, OnButtonGlobal)
	ON_COMMAND(ID_BUTTON_START, OnButtonStart)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_ENTITY, OnKeydownListEntity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIGESDlg message handlers
int CIGESDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_IGES_TOOLBAR)) {
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

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

void CIGESDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CIGESDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CIGESDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CIGESDlg::OnDestroy() 
{
	if (CIGESDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CIGESDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CIGESDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();

	if (this->IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[this->IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}


	
	UpdateAllControls();

	CreateColumns();
	FillList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIGESDlg::UpdateAllControls()
{
	CString Buff,Temp,Line;
	CSize size;
	CDC* pDC = GetDC();
	CRect rr;
	EntityStruct* pEntity;
	int nn,A,N,K,M,K1,M1,nPtr,N1,B,C;

	m_editData.GetWindowRect(&rr);

	if (GetSafeHwnd() == NULL) return;

	if(m_nIndex<m_IGES.m_nEntityL) {
		pEntity = &m_IGES.m_pEntity[m_nIndex];

		switch(pEntity->nEntityType) {
		default:
			for(nn=0;nn<m_IGES.m_pEntity->nDataL;nn++) {
				Temp.Format(_T("%f "),m_IGES.m_pEntity->fData[nn]);
				size = pDC->GetTextExtent(Line+Temp);
				if(size.cx > rr.Width()) {
					Line += "\r\n";
					Buff += Line;
					Line.Empty();
				}
				Line += Temp;
			}
			Buff += Line;
			break;
		case 100:
		case 104:
		case 106:
		case 110:
		case 116:
		case 124:
		case 202:
		case 210:
		case 212:
		case 214:
		case 218:
		case 222:
			m_IGES.Format(pEntity,&Buff);
			break;
		case 126:	//Rational B-Spline curve
			switch(pEntity->nFormNumber) {
			case 0: Line = "Rational B Spline\r\n";
				break;
			case 1: Line = "Line\r\n";
				break;
			case 2: Line = "Circular arc\r\n";
				break;
			case 3: Line = "Elliptical arc \r\n";
				break;
			case 4: Line = "Parabolic arc\r\n";
				break;
			case 5: Line = "Hyperbolic arc\r\n";
				break;
			}
			Buff += Line;
			K = (int)pEntity->fData[0];
			M = (int)pEntity->fData[1];
			Line.Format(_T("K:%d M:%d P1:%d P2:%d P3:%d P4:%d\r\n"),K,M,(int)pEntity->fData[2],(int)pEntity->fData[3],(int)pEntity->fData[4],(int)pEntity->fData[5]);
			Buff += Line;
			N = 1 + K - M;
			A = N + 2 * M;
			Buff += "Knots ";
			for(nn=0,nPtr=6;nn<=A;nn++,nPtr++) {
				Temp.Format(_T("%f "),pEntity->fData[nPtr]);
				Buff += Temp;
			}
			Buff+= "\r\n";
			Buff += "Weights ";
			for(nn=0;nn<=K;nn++,nPtr++) {
				Temp.Format(_T("%f "),pEntity->fData[nPtr]);
				Buff += Temp;
			}
			Buff+= "\r\n";

			for(nn=0;nn<=K;nn++,nPtr+=3) {
				Temp.Format(_T("%f %f %f \r\n"),pEntity->fData[nPtr],pEntity->fData[nPtr+1],pEntity->fData[nPtr+2]);
				Buff += Temp;
			}
			Temp.Format(_T("V0:%f V1:%f\r\n"),pEntity->fData[nPtr],pEntity->fData[nPtr+1]);
			Buff+=Temp;
			nPtr+=2;
			Temp.Format(_T("X:%f Y:%f Z:%f\r\n"),pEntity->fData[nPtr],pEntity->fData[nPtr+1],pEntity->fData[nPtr+2]);
			Buff+=Temp;
			nPtr+=3;
			break;
		case 128:
			K = (int)pEntity->fData[0];
			K1 = (int)pEntity->fData[1];
			M = (int)pEntity->fData[2];
			M1 = (int)pEntity->fData[3];
			Line.Format(_T("K1:%d K2:%d M1:%d M2:%d P1:%d P2:%d P3:%d P4:%d P5:%d\r\n"),K,K1,M,M1,(int)pEntity->fData[4],(int)pEntity->fData[5],(int)pEntity->fData[6],(int)pEntity->fData[7],(int)pEntity->fData[8]);
			Buff += Line;
			N = 1 + K - M;
			A = N + 2 * M;
			N1 = 1 + K1 - M1;
			B = N1 + 2 * M1;
			C = (1+K) * (1+K1);
			Buff += "Knots ";
			for(nn=0,nPtr=9;nn<=A;nn++,nPtr++) {
				Temp.Format(_T("%f "),pEntity->fData[nPtr]);
				Buff += Temp;
			}
			Buff+= "\r\n";
			Buff += "Knots ";
			for(nn=0;nn<=B;nn++,nPtr++) {
				Temp.Format(_T("%f "),pEntity->fData[nPtr]);
				Buff += Temp;
			}
			Buff+= "\r\n";
			Buff += "Weights ";
			for(nn=0;nn<C;nn++,nPtr++) {
				Temp.Format(_T("%f "),pEntity->fData[nPtr]);
				Buff += Temp;
			}
			Buff+= "\r\n";
			for(nn=0;nn<C;nn++,nPtr+=3) {
				Temp.Format(_T("%f %f %f \r\n"),pEntity->fData[nPtr],pEntity->fData[nPtr+1],pEntity->fData[nPtr+2]);
				Buff += Temp;
			}
			Temp.Format(_T("U0:%f U1:%f\r\n"),pEntity->fData[nPtr],pEntity->fData[nPtr+1]);
			Buff+=Temp;
			nPtr+=2;
			Temp.Format(_T("V0:%f V1:%f\r\n"),pEntity->fData[nPtr],pEntity->fData[nPtr+1]);
			Buff+=Temp;
			nPtr+=2;

		}
		m_editData.SetWindowText(Buff);
	}


	if(m_IGES.m_pEntity != NULL) {
		Line.Format(_T("Form %d"),m_IGES.m_pEntity[m_nIndex].nFormNumber);
		Buff = Line;
		m_editParam.SetWindowText(Buff);
	}

	ReleaseDC(pDC);

	SetToolBarCheckedState();
}

void CIGESDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
}

void CIGESDlg::CreateColumns()
{
	int ColumnWidth[10] = { 20,170};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1]= "Type";

	int nColumnCount = m_listEntity.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listEntity.DeleteColumn(0);
	}
	for (int ii=0;ii<2;ii++) {
		m_listEntity.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}
	m_listEntity.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


}

void CIGESDlg::FillList()
{
	CString Buff;

	m_listEntity.DeleteAllItems();
	for (int ii=0;ii<m_IGES.m_nEntityL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listEntity.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
/*
	m_listEntity.EnsureVisible(m_nIndex,FALSE);
	for(ii=0;ii<m_IGES.m_nEntityL;ii++) 
		m_listEntity.SetItemState(ii , 0,LVIS_SELECTED);
	m_listEntity.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listEntity.SetFocus();
*/
}

void CIGESDlg::OnGetdispinfoListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	if(m_IGES.m_nEntityL<=0) return;
	str[0]=0;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,100,_T("%d %s"),m_IGES.m_pEntity[pDispInfo->item.iItem].nEntityType,(LPCWSTR)m_IGES.m_EntityType[m_IGES.m_pEntity[pDispInfo->item.iItem].nEntityType]);
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	
	*pResult = 0;
}

void CIGESDlg::OnClickListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	m_nIndex=pDispInfo->item.mask;

	Invalidate(false);
	UpdateAllControls();
	*pResult = 0;
}

void CIGESDlg::OnKeydownListEntity(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	POSITION pos = m_listEntity.GetFirstSelectedItemPosition();
	m_nIndex = m_listEntity.GetNextSelectedItem(pos);
	if(pLVKeyDow->wVKey==40) m_nIndex++;
	if(pLVKeyDow->wVKey==38) m_nIndex--;
	UpdateAllControls();
	Invalidate(false);

	*pResult = 0;
}

void CIGESDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int	ii;
	CRect rr;
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penRed(PS_SOLID,4,RGB(255,0,0));
	CPen *pOldPen = dc.SelectObject(&penBlack);
	LOGFONT lf;
	CFont Font;

	CopyMemory(&lf,&theApp.m_LastSettings.lfViewGraticule,sizeof lf);
	lf.lfEscapement=0;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = dc.SelectObject(&Font);
	

	m_staticView.GetWindowRect(rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

	CBrush Brush(RGB(255,255,255));
	CBrush * pOldBrush = dc.SelectObject(&Brush);
	dc.FillRect(rr,&Brush);
	dc.SelectObject(pOldBrush);

	rr.DeflateRect(2,2,2,2);

	vecMin = D3DXVECTOR3(10e6f,10e6f,10e6f);
	vecMax = D3DXVECTOR3(-10e6f,-10e6f,-10e6f);

	switch(m_nPaintOption) {
	case 0:
		for(ii=0;ii<m_IGES.m_nEntityL;ii++) {
			switch(m_IGES.m_pEntity[ii].nEntityType) {
			case 100: m_IGES.CircleArcLimits(&m_IGES.m_pEntity[ii],&vecMin,&vecMax);
				break;
			case 110: m_IGES.LineLimits(&m_IGES.m_pEntity[ii],&vecMin,&vecMax);
				break;
			case 116: m_IGES.PointLimits(&m_IGES.m_pEntity[ii],&vecMin,&vecMax);
				break;
			}
		}
	
		for(ii=0;ii<m_IGES.m_nEntityL;ii++) {
			if(ii==m_nIndex) {
				dc.SelectObject(&penRed);
			} else {
				dc.SelectObject(&penBlack);
			}
			switch(m_IGES.m_pEntity[ii].nEntityType) {
			case 100: RenderEntityCircleArc(&dc,rr,&m_IGES.m_pEntity[ii]);
				break;
			case 104: RenderEntityConicArc(&dc,rr,&m_IGES.m_pEntity[ii]);
				break;
			case 110: RenderEntityLine(&dc,rr,&m_IGES.m_pEntity[ii]);
				break;
			case 116: RenderEntityPoint(&dc,rr,&m_IGES.m_pEntity[ii]);
				break;
			case 126: RenderEntityRationalBSpline(&dc,rr,&m_IGES.m_pEntity[ii]);
				break;
			}
		}
		break;
	case 1:	RenderHeader(&dc,rr,'S');
		break;
	case 2: RenderHeader(&dc,rr,'G');
		break;
	}

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);
}


void CIGESDlg::OnButtonDrawing() 
{
	m_nPaintOption = 0;
	SetToolBarCheckedState();
	Invalidate(false);
	
}

void CIGESDlg::OnButtonGlobal() 
{
	m_nPaintOption = 2;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CIGESDlg::OnButtonStart() 
{
	m_nPaintOption = 1;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CIGESDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_DRAWING))>=0)
		m_nPaintOption==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_START))>=0)
		m_nPaintOption==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_GLOBAL))>=0)
		m_nPaintOption==2 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


}

void CIGESDlg::RenderHeader(CPaintDC *pDC, CRect rr, char cHeaderType)
{
	CString Buff = L"88888.8";
	CSize size = pDC->GetTextExtent(Buff);
	int nX,nY,ii;
	int nLength = 0;

	nX = rr.left + 2;
	nY = rr.top + 2;
	switch(cHeaderType) {
	default:
		for(ii=0;ii<m_IGES.m_strStart.GetSize();ii++) {
			pDC->TextOut(nX,nY,m_IGES.m_strStart.GetAt(ii));
			nY+=size.cy;
		}
		break;
	case 'G':
		for(ii=0;ii<m_IGES.m_strGlobalFields.GetSize();ii++) {
			pDC->TextOut(nX,nY,Buff=m_IGES.m_strGlobalFields.GetAt(ii));
			size = pDC->GetTextExtent(Buff);
			if(size.cx > nLength) nLength = size.cx;
			nY += size.cy;
			if((nY + size.cy) >= rr.bottom) {
				nY = rr.top + 2;
				nX += (nLength+20);
				nLength = 0;
			}
		}
		break;
	}
}

void CIGESDlg::OnFileOpen() 
{
	CString Buff,PathName;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".iges"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.igs)|*.igs|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		PathName=m_IGES.m_PathName=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = PathName.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = PathName.Left(nPos);
			}
		}

		if(!File.Open(m_IGES.m_PathName,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,m_IGES.m_PathName);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			m_IGES.Retrieve(&File);
			File.Close();
			Wait.Restore();
		}
	}
	FillList();
	Invalidate(false);
}

void CIGESDlg::WorldToClient(float fX,float fY,float fZ,CRect rr,CPoint *pt)
{

	pt->x = rr.left + (int)(((fX-vecMin.x) * (float)rr.Width()) / (vecMax.x-vecMin.x));
	pt->y = rr.bottom - (int)(((fY-vecMin.y) * (float)rr.Height()) / (vecMax.y-vecMin.y));
}

bool CIGESDlg::RenderEntityLine(CPaintDC *pDC, CRect rr, EntityStruct *pEntity)
{
	CPoint pt;
	if(pEntity->nEntityType != 110) return false;
	D3DXVECTOR3 vec;

	vec = D3DXVECTOR3(pEntity->fData[0],pEntity->fData[1],pEntity->fData[2]);
	if(pEntity->nMatrix) 
		D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);
	WorldToClient(vec.x,vec.y,0.0f, rr,&pt);
	pDC->MoveTo(pt);

	vec = D3DXVECTOR3(pEntity->fData[3],pEntity->fData[4],pEntity->fData[5]);
	if(pEntity->nMatrix) 
		D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);
	WorldToClient(vec.x,vec.y,0.0f, rr,&pt);
	pDC->LineTo(pt);
	return true;

}

bool CIGESDlg::RenderEntityPoint(CPaintDC *pDC, CRect rr, EntityStruct *pEntity)
{
	CPoint pt;
	if(pEntity->nEntityType != 116) return false;
	WorldToClient(pEntity->fData[0],pEntity->fData[1],pEntity->fData[2], rr,&pt);
	pDC->SetPixel(pt,RGB(0,0,0));
	return true;

}

bool CIGESDlg::RenderEntityCircleArc(CPaintDC *pDC, CRect rr, EntityStruct *pEntity)
{
	if(pEntity->nEntityType != 100) return false;

	CPoint pt,pt0,pt1;
	CRect rrCircle;
	struct ArcData {
		float fZt;
		float fXCentre;
		float fYCentre;
		float fX0;
		float fY0;
		float fX1;
		float fY1;
	} *pArc = (ArcData*)pEntity->fData;

	D3DXVECTOR3 vec;

	float fX = pArc->fX0-pArc->fXCentre;
	float fY = pArc->fY0-pArc->fYCentre;
	float fRadius = sqrtf((fX*fX) + (fY*fY));

	float fAngle0 = atan2(pArc->fY0-pArc->fYCentre,pArc->fX0-pArc->fXCentre);
	float fAngle1 = atan2(pArc->fY1-pArc->fYCentre,pArc->fX1-pArc->fXCentre);

	if(fAngle1<fAngle0) {
		fAngle1+=(2.0f*PIf);
	}
	if(fAngle0 == fAngle1) {
		fAngle0=0.0;
		fAngle1=2.0f * PIf;
	}

	int nn=0;
	for(float fAngle = fAngle0; fAngle < fAngle1; fAngle += 0.017f,nn++) {
		vec.x = fRadius * cosf(fAngle) + pArc->fXCentre;
		vec.y = fRadius * sinf(fAngle) + pArc->fYCentre;
		vec.z = pArc->fZt;
		if(pEntity->nMatrix) 
			D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);

		WorldToClient(vec.x,vec.y,0.0f, rr,&pt);

		if(nn==0) {
			pDC->MoveTo(pt);
		} else {
			pDC->LineTo(pt);
		}
	}
	vec.x = fRadius * cosf(fAngle1) + pArc->fXCentre;
	vec.y = fRadius * sinf(fAngle1) + pArc->fYCentre;
	vec.z = pArc->fZt;
	if(pEntity->nMatrix) 
		D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);
	WorldToClient(vec.x,vec.y,0.0f, rr,&pt);
	pDC->LineTo(pt);

	return true;

}

bool CIGESDlg::RenderEntityConicArc(CPaintDC *pDC, CRect rr, EntityStruct *pEntity)
{
	if(pEntity->nEntityType != 104) return false;

	CPoint pt;
	struct ConicArcData {
		float fA;
		float fB;
		float fC;
		float fD;
		float fE;
		float fF;
		float fZt;
		float fX0;
		float fY0;
		float fX1;
		float fY1;
	} *pArc = (ConicArcData*)pEntity->fData;

	float a = sqrtf( -pArc->fF/pArc->fA );
	float b = sqrtf( -pArc->fF/pArc->fC );

	D3DXVECTOR3 vec;
	float fAngle0 = 0.0f;
	float fAngle1 = 2.0f * PIf;

	if((pArc->fX0-pArc->fX1) || (pArc->fY0-pArc->fY1)) {
//		fAngle0 = acosf(pArc->fX0 / a);
		fAngle0 = asinf(pArc->fY0 / b);
//		fAngle1 = acosf(pArc->fX1 / a);
		fAngle1 = asinf(pArc->fY1 / b);
	}
	if(fAngle1 < fAngle0) fAngle1 += (2.0f * PIf);

	int nn=0;
	for(float fAngle = fAngle0; fAngle < fAngle1; fAngle += 0.017f,nn++) {
		vec.x = a * cosf(fAngle);									//consider swapping b and a or mat._12 with mat._21
		vec.y = b * sinf(fAngle);
		vec.z = pArc->fZt;
		if(pEntity->nMatrix) 
			D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);

		WorldToClient(vec.x,vec.y,0.0f, rr,&pt);

		if(nn==0) {
			pDC->MoveTo(pt);
		} else {
			pDC->LineTo(pt);
		}
	}
/*
	vec = D3DXVECTOR3(pArc->fX0,pArc->fY0,pArc->fZt);
	if(pEntity->nMatrix) 
		D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);
	WorldToClient(vec.x,vec.y,0.0f, rr,&pt);
	pDC->MoveTo(pt.x-3,pt.y-3);
	pDC->LineTo(pt.x+4,pt.y+4);
	pDC->MoveTo(pt.x-3,pt.y+3);
	pDC->LineTo(pt.x+4,pt.y-4);

	vec = D3DXVECTOR3(pArc->fX1,pArc->fY1,pArc->fZt);
	if(pEntity->nMatrix) 
		D3DXVec3TransformCoord(&vec,&vec,&pEntity->mat);
	WorldToClient(vec.x,vec.y,0.0f, rr,&pt);
	pDC->MoveTo(pt.x-3,pt.y-3);
	pDC->LineTo(pt.x+4,pt.y+4);
	pDC->MoveTo(pt.x-3,pt.y+3);
	pDC->LineTo(pt.x+4,pt.y-4);
*/
	return true;

}

bool CIGESDlg::RenderEntityRationalBSpline(CPaintDC *pDC, CRect rr, EntityStruct *pEntity)
{
	if(pEntity->nEntityType != 126) return false;
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPoint pt;
	int ii,nPtr;
	D3DXVECTOR3 vec;
	D3DXVECTOR3 vecCtrlPts[6];
	D3DXVECTOR3 vecCurvePts[300];
	struct SplineData {
		float fK;
		float fM;
		float fProp1;
		float fProp2;
		float fProp3;
		float fProp4;
	} *pSpline = (SplineData*)pEntity->fData;

	ZeroMemory(vecCtrlPts,sizeof vecCtrlPts);
	ZeroMemory(vecCurvePts,sizeof vecCurvePts);

	int N = 1 + (int)pSpline->fK - (int)pSpline->fM;
	int A = N + 2 * (int)pSpline->fM;
	int K = (int)pSpline->fK;

	float *fKnot = new float[A+1];
	float *fWeight = new float[K+1];
	D3DXVECTOR3 *vecPt = new D3DXVECTOR3[K+1];

	for(ii=0,nPtr=6;ii<=A;ii++,nPtr++) 
		fKnot[ii] = pEntity->fData[nPtr];

	for(ii=0;ii<=K;ii++,nPtr++) 
		fWeight[ii] = pEntity->fData[nPtr];

	for(ii=0;ii<=K;ii++,nPtr+=3) 
		vecPt[ii] = D3DXVECTOR3(pEntity->fData[nPtr],pEntity->fData[nPtr+1],pEntity->fData[nPtr+2]);

	float	yp1=pEntity->fData[nPtr++];
	float	ypn=pEntity->fData[nPtr++];
	D3DXVECTOR3 vecNorm = D3DXVECTOR3(pEntity->fData[nPtr],pEntity->fData[nPtr+1],pEntity->fData[nPtr+2]);

	
	for(ii=0;ii<=K;ii++) {
		if(pEntity->nMatrix) {
			D3DXVec3TransformCoord(&vec,&vecPt[ii],&pEntity->mat);
		} else {
			vec = vecPt[ii];
		}
		vecCtrlPts[ii] = vec;
		WorldToClient(vec.x,vec.y,0.0f, rr,&pt);

		ii==0 ?	pDC->MoveTo(pt) : pDC->LineTo(pt);
	}



	int nCtrlPts = K + 1;
	int nCurvePts = 200;
	int nOrder = 3;

	float *fKnots = new float[nCtrlPts + nOrder + 1];
	CNURBS	Nurb;

	Nurb.knot(nCtrlPts,nOrder,fKnots);
	Nurb.bspline(nCtrlPts,nOrder,nCurvePts,vecCtrlPts,fKnots,vecCurvePts);
	CPen* pOldPen = pDC->SelectObject(&penBlue);
	for(ii=0;ii<nCurvePts;ii++) {
		WorldToClient(vecCurvePts[ii].x,vecCurvePts[ii].y,0.0f, rr,&pt);
		ii==0 ?	pDC->MoveTo(pt) : pDC->LineTo(pt);
	}

	float fHomogeneous[6] = {1.0f, 10.0f, 6.0f, 10.0f, 4.0f, 1.0f};
	Nurb.rationalbspline(nCtrlPts,nOrder,nCurvePts,vecCtrlPts,fKnots,fHomogeneous,vecCurvePts);

	pDC->SelectObject(&penRed);
	for(ii=0;ii<nCurvePts;ii++) {
		WorldToClient(vecCurvePts[ii].x,vecCurvePts[ii].y,0.0f, rr,&pt);
		ii==0 ?	pDC->MoveTo(pt) : pDC->LineTo(pt);
	}

	pDC->SelectObject(pOldPen);
	delete fKnots;
	delete vecPt;
	delete fWeight;
	delete fKnot;

	return true;
}


