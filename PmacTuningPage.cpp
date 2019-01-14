// PmacTuningPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacTuningPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacTuningPage property page

IMPLEMENT_DYNCREATE(CPmacTuningPage, CResizablePage)

CPmacTuningPage::CPmacTuningPage() : CResizablePage(CPmacTuningPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacTuningPage)
	//}}AFX_DATA_INIT
	
	m_nAxisNumber = 1;
	m_pEditSpinItem=NULL;

	m_nDistance	= 1000;
	m_nTest=0;
	m_fStepSize = 0.01f;
	ZeroMemory(m_fPos,sizeof m_fPos);

	m_pnX[0] = m_pnX[1] = 0;
	m_bLBDown = false;
	m_nMoveWhich= 0;

	m_pAxis = &theApp.m_Axes[0];

	m_strDecodeDescription[0] = "Clk/Dir";
	m_strDecodeDescription[1] = "x1 CW";
	m_strDecodeDescription[2] = "x2 CW";
	m_strDecodeDescription[3] = "x4 CW";
	m_strDecodeDescription[4] = "!Clk/Dir";
	m_strDecodeDescription[5] = "x1 CCW";
	m_strDecodeDescription[6] = "x2 CCW";
	m_strDecodeDescription[7] = "x4 CCW";

	for(int nAxis = 0;nAxis<16; nAxis++) {
		switch(theApp.m_PmacUser.m_nPmacType) {
		case PMAC1:
		case PMAC2:
		case PMAC_Ultralite:
			m_nEncoderAddrs[nAxis] = 900 + (m_nAxisNumber-1)*5;
			break;
		case Turbo_PMAC1:
		case Turbo_PMAC2:
		case Turbo_PMAC_Ultralite:
		case Turbo_PMAC2_Ultralite:
			m_nEncoderAddrs[nAxis] = 7000 + ((nAxis/4)*10 + ((nAxis%4)+1)) * 10;
			break;
		}
		theApp.m_PmacUser.RequestVariable(_T("I"),m_nEncoderAddrs[nAxis],&m_strEncoderDecode[nAxis]);
		swscanf_s(m_strEncoderDecode[nAxis],_T("%d"),&m_nEncoderDecode[nAxis]);

	}

}

CPmacTuningPage::~CPmacTuningPage()
{
}

void CPmacTuningPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacTuningPage)
	DDX_Control(pDX, IDC_LIST_ENCODERS, m_listEncoders);
	DDX_Control(pDX, IDC_CHECK_VELOCITY, m_checkVelocity);
	DDX_Control(pDX, IDC_CHECK_DAC_VOLTAGE, m_checkDACVoltage);
	DDX_Control(pDX, IDC_CHECK_FOLLOWING_ERROR, m_checkFollowingError);
	DDX_Control(pDX, IDC_EDIT_IX30GAIN, m_editIX30Gain);
	DDX_Control(pDX, IDC_EDIT_BANDPASS_DAMPING, m_editBandPassDamping);
	DDX_Control(pDX, IDC_EDIT_BANDPASS_FREQUENCY, m_editBandPassFrequency);
	DDX_Control(pDX, IDC_EDIT_NOTCH_DAMPING, m_editNotchDamping);
	DDX_Control(pDX, IDC_EDIT_NOTCH_FREQUENCY, m_editNotchFrequency);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editPosition);
	DDX_Control(pDX, IDC_EDIT_ENCODER_COUNT, m_editEncoderCount);
	DDX_Control(pDX, IDC_EDIT_POS2, m_editPos2);
	DDX_Control(pDX, IDC_EDIT_POS1, m_editPos1);
	DDX_Control(pDX, IDC_EDIT_POS0, m_editPos0);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE, m_editStepSize);
	DDX_Control(pDX, IDC_EDITMULTIPLY, m_editInPosMultiplier);
	DDX_Control(pDX, IDC_EDIT_TA, m_editTA);
	DDX_Control(pDX, IDC_EDIT_TS, m_editTS);
	DDX_Control(pDX, IDC_EDIT_DISTANCE, m_editDistance);
	DDX_Control(pDX, IDC_COMBO_TEST, m_comboTest);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_LIST_VARIABLE, m_listVariables);
	DDX_Control(pDX, IDC_COMBO_AXIS_NUMBER, m_comboAxisNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacTuningPage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacTuningPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS_NUMBER, OnSelchangeComboAxisNumber)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_VARIABLE, OnGetdispinfoListVariable)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VARIABLE, OnDblclkListVariable)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VARIABLE, OnClickListVariable)
	ON_BN_CLICKED(IDC_BUTTON_DO_IT, OnButtonDoIt)
	ON_EN_CHANGE(IDC_EDIT_TA, OnChangeEditTa)
	ON_EN_CHANGE(IDC_EDIT_TS, OnChangeEditTs)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_2, OnButtonGoto2)
	ON_EN_CHANGE(IDC_EDITMULTIPLY, OnChangeEditmultiply)
	ON_EN_CHANGE(IDC_EDIT_POS0, OnChangeEditPos0)
	ON_EN_CHANGE(IDC_EDIT_POS1, OnChangeEditPos1)
	ON_EN_CHANGE(IDC_EDIT_POS2, OnChangeEditPos2)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE, OnChangeEditStepsize)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_NOTCH_FILTER, OnButtonNotchFilter)
	ON_BN_CLICKED(IDC_BUTTON_FACTORY, OnButtonFactory)
	ON_BN_CLICKED(IDC_BUTTON_STORE, OnButtonStore)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_DAC_VOLTAGE, OnCheckDacVoltage)
	ON_BN_CLICKED(IDC_CHECK_FOLLOWING_ERROR, OnCheckFollowingError)
	ON_BN_CLICKED(IDC_CHECK_VELOCITY, OnCheckVelocity)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ENCODERS, OnDblclkListEncoders)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ENCODERS, OnGetdispinfoListEncoders)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENCODERS, OnClickListEncoders)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(UI_ITEMCHANGED,UpdatePmacFromList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacTuningPage message handlers

BOOL CPmacTuningPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	CString Buff;
	int	ii;
	CRect rr;

	GetClientRect(m_rrClient);
	AddAnchor(&m_staticView);

	rr = m_rrClient;
//	ScreenToClient(rr);

	m_pnX[0] = m_pnX[1] = rr.right;

	for(ii=0;ii<16;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_comboAxisNumber.AddString(Buff);
	}

	m_comboTest.AddString(_T("Move"));

	

	UpdateAllControls();
	CreateColumns();
	FillList();
	EnableToolTips(true);
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPmacTuningPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPmacTuningPage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	if (pCtrl->GetSafeHwnd()) {
		pCtrl->GetWindowRect(&rr);
		ScreenToClient(&rr);

		int nID = pCtrl->GetDlgCtrlID();
		CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left, 100, m_rrClient.Width()), MulDiv(rr.top, 100, m_rrClient.Height())), CSize(MulDiv(rr.right, 100, m_rrClient.Width()), MulDiv(rr.bottom, 100, m_rrClient.Height())), CSize(0, 0), true, false);
	}
}

void CPmacTuningPage::UpdateAllControls()
{
	CString Buff;
	CString Unit[3] = {L" mm",L"\x020",L"\x0b0"};

	for(int ii=0;ii<16;ii++) {
		if((theApp.m_Axes[ii].nPhysicalAxis == m_nAxisNumber) && (theApp.m_Axes[ii].bAvailable==TRUE)) {
			m_fStepSize = theApp.m_Axes[ii].fStepSize;
			m_pAxis = &theApp.m_Axes[ii];
		}
	}
	Buff.Format(_T("%.7f%s"),m_fStepSize,Unit[m_pAxis->nUnitSelected]);
	m_editStepSize.SetWindowText(Buff);


	m_comboAxisNumber.SetCurSel(m_nAxisNumber-1);

	Buff.Format(_T("%d cts"),m_nDistance);
	m_editDistance.SetWindowText(Buff);

	m_comboTest.SetCurSel(m_nTest);

	Buff.Format(_T("%d ms"),theApp.m_Tank.nTA);
	m_editTA.SetWindowText(Buff);
	Buff.Format(_T("%d ms"),theApp.m_Tank.nTS);
	m_editTS.SetWindowText(Buff);

	Buff.Format(_T("%.03f"),m_fPos[0][m_nAxisNumber-1]);
	m_editPos0.SetWindowText(Buff);
	Buff.Format(_T("%.03f"),m_fPos[1][m_nAxisNumber-1]);
	m_editPos1.SetWindowText(Buff);
	Buff.Format(_T("%.03f"),m_fPos[2][m_nAxisNumber-1]);
	m_editPos2.SetWindowText(Buff);


	Buff.Format(_T("%d"),theApp.m_LastSettings.nInPosGainMultiplier);
	m_editInPosMultiplier.SetWindowText(Buff);

	Buff.Format(L"%.01fHz",m_pAxis->fNotchFrequency);
	m_editNotchFrequency.SetWindowText(Buff);
	Buff.Format(L"%.01fHz",m_pAxis->fNotchDamping);
	m_editNotchDamping.SetWindowText(Buff);

	Buff.Format(L"%.01fHz",m_pAxis->fBandPassFrequency);
	m_editBandPassFrequency.SetWindowText(Buff);
	Buff.Format(L"%.01fHz",m_pAxis->fBandPassDamping);
	m_editBandPassDamping.SetWindowText(Buff);

	CalculateNotchFilter(false);

	m_checkDACVoltage.SetCheck(theApp.m_LastSettings.nTunningRenderDacVoltage);
	m_checkFollowingError.SetCheck(theApp.m_LastSettings.nTunningRenderFollowingError);
	m_checkVelocity.SetCheck(theApp.m_LastSettings.nTunningVelocity);

}

void CPmacTuningPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	CDC	dcMem;
	CBitmap bmp;

	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 12;
	CFont Font;
	CFont* pOldFont;
	Font.CreateFontIndirect(&lf);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	CRect rrArea = rr;
	rrArea.OffsetRect(-rr.left,-rr.top);

	COLORREF	*pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pOldFont = dcMem.SelectObject(&Font);
	dcMem.SetBkMode(TRANSPARENT);

	dcMem.FillRect(&rrArea,&CBrush(RGB(255,255,255)));

	DrawMoveTrace((CPaintDC*)&dcMem,rrArea);

	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);
	SAFE_DELETE( pArray );
}

void CPmacTuningPage::DrawMoveTrace(CPaintDC* pDC,CRect rr)
{
	if(theApp.m_Thread.m_CpTunning.m_nCoordL<=0) return;

	int nSamples = theApp.m_Thread.m_nTuningPtr;
	int	nn,xx,yy,nPos,nX=0;
	int	nMinError,nMaxError;
	CString Buff;
	int	nMaxVelocitySample=0;
	CPen* pOldPen;
	float fTemp,fMax,fMaxVelocity,fMinVelocity;
	CString Unit[3] = {L" mm",L"\x020",L"\x0b0"};

	CPen penBlue(PS_SOLID, 1 , RGB(0,0,255));
	CPen penRed(PS_SOLID, 1 , RGB(255,0,0));
	CPen penGreen(PS_SOLID, 1 , RGB(0,0,255));
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	pOldPen = pDC->SelectObject(&penGrey);

	Buff = "888.8";
	CSize size = pDC->GetTextExtent(Buff);
	rr.DeflateRect(size.cx,size.cy*2);
	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.left,rr.bottom);
	pDC->LineTo(rr.BottomRight());
	int	nYCentre=rr.CenterPoint().y;

	CPoint *pt = new CPoint[nSamples];

	//Draw the basic trajectory
	pDC->SelectObject(&penBlack);
	nMinError = 200000;
	nMaxError = -200000;
	fMaxVelocity = 0.0f;
	fMinVelocity = 0.0f;
	pDC->MoveTo(rr.left,rr.bottom);
	for(nn=0;nn<nSamples;nn++) {
		xx=rr.left + MulDiv(nn,rr.Width(),nSamples-1);
		nPos = (int)((theApp.m_Thread.m_CpTunning.m_pCp[nn].fTunningPos - theApp.m_Thread.m_CpTunning.m_pCp[0].fTunningPos) / theApp.m_Axes[m_nAxisNumber-1].fStepSize);
		yy=rr.bottom - MulDiv(nPos,rr.Height(),m_nDistance);
		pDC->LineTo(xx,yy);
		if(theApp.m_Thread.m_CpTunning.m_pCp[nn].nTunningFollowingError < nMinError) nMinError = theApp.m_Thread.m_CpTunning.m_pCp[nn].nTunningFollowingError;
		if(theApp.m_Thread.m_CpTunning.m_pCp[nn].nTunningFollowingError > nMaxError) nMaxError = theApp.m_Thread.m_CpTunning.m_pCp[nn].nTunningFollowingError;
//		if((yy==rr.CenterPoint().y) && (nMaxVelocitySample ==0)) {
//			nMaxVelocitySample = nn;
//		}

		if(fMaxVelocity < theApp.m_Thread.m_CpTunning.m_pCp[nn].fVelocity) fMaxVelocity = theApp.m_Thread.m_CpTunning.m_pCp[nn].fVelocity;
		if(fMinVelocity > theApp.m_Thread.m_CpTunning.m_pCp[nn].fVelocity) fMinVelocity = theApp.m_Thread.m_CpTunning.m_pCp[nn].fVelocity;
	}

	//Draw the error position
	if(theApp.m_LastSettings.nTunningRenderFollowingError) {
		nMinError/=3072;
		nMaxError/=3072;

		int nMinTemp = (int)(1.0f / theApp.m_Axes[m_nAxisNumber-1].fStepSize);
		int nMaxTemp = (int)(4.0f / theApp.m_Axes[m_nAxisNumber-1].fStepSize);
		if(nMaxError < nMinTemp) nMaxError = nMinTemp;
		if(nMinError > -nMinTemp) nMinError = -nMinTemp;
		if(nMaxError > nMaxTemp) nMaxError = nMaxTemp;
		if(nMinError < -nMaxTemp) nMinError = -nMaxTemp;

		if(abs(nMaxError) > abs(nMinError)) {
			nMinError = -nMaxError;
		} else {
			nMaxError = -nMinError;
		}

		pDC->MoveTo(rr.right,nYCentre);
		pDC->LineTo(rr.left,nYCentre);

		pDC->SelectObject(&penGrey);
		fMax = (float)nMaxError * theApp.m_Axes[m_nAxisNumber-1].fStepSize;
		for(fTemp = 0.2f;fTemp < fMax; fTemp += 0.2f) {
			yy = (int)(fTemp * (float)(rr.Height()/2) / fMax);
			pDC->MoveTo(rr.left,nYCentre-yy);
			pDC->LineTo(rr.right,nYCentre-yy);
			pDC->MoveTo(rr.left,nYCentre+yy);
			pDC->LineTo(rr.right,nYCentre+yy);
			Buff.Format(L"%.01f%s",fTemp,Unit[theApp.m_Axes[m_nAxisNumber-1].nUnitSelected]);
			pDC->TextOut(rr.left,nYCentre+yy,Buff);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(rr.left,nYCentre-yy-size.cy,Buff);

		}

		pDC->SelectObject(&penBlack);

		for(nn=0;nn<nSamples;nn++) {
			pt[nn].x=rr.left + MulDiv(nn,rr.Width(),nSamples-1);
			pt[nn].y=nYCentre - MulDiv(theApp.m_Thread.m_CpTunning.m_pCp[nn].nTunningFollowingError/3072,(rr.Height()/2),nMaxError);
		}
		pDC->Polyline(pt,nSamples);

		Buff.Format(_T("%.02f%s"),(float)nMaxError * theApp.m_Axes[m_nAxisNumber-1].fStepSize,Unit[theApp.m_Axes[m_nAxisNumber-1].nUnitSelected]);
		pDC->TextOut(rr.left-10,rr.top-10,Buff);
		Buff.Format(_T("%.02f%s"),(float)nMinError * theApp.m_Axes[m_nAxisNumber-1].fStepSize,Unit[theApp.m_Axes[m_nAxisNumber-1].nUnitSelected]);
		pDC->TextOut(rr.left-10,rr.bottom,Buff);
	}

	if(theApp.m_LastSettings.nTunningRenderDacVoltage) {

		pDC->SelectObject(&penGreen);

		for(nn=0;nn<nSamples;nn++) {
			pt[nn].x=rr.left + MulDiv(nn,rr.Width(),nSamples-1);
			pt[nn].y=nYCentre - MulDiv(theApp.m_Thread.m_CpTunning.m_pCp[nn].nTunningDAC,(rr.Height()/2),20480);
		}
		pDC->Polyline(pt,nSamples);


	}

	if(theApp.m_LastSettings.nTunningVelocity) {

		pDC->SelectObject(&penBlue);
		pDC->SetTextColor(RGB(0,0,255));

		if(fabs(fMinVelocity) > fabs(fMaxVelocity)) fMaxVelocity = fabs(fMinVelocity);

		for(nn=0;nn<nSamples;nn++) {
			pt[nn].x=rr.left + MulDiv(nn,rr.Width(),nSamples-1);
			pt[nn].y=nYCentre - (int)(theApp.m_Thread.m_CpTunning.m_pCp[nn].fVelocity * (float)(rr.Height()/2) / fMaxVelocity);
		}
		pDC->Polyline(pt,nSamples);
		
		Buff.Format(L"%.0fmm/s",fMaxVelocity);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.right-size.cx,rr.top,Buff);
		Buff.Format(L"%.0fmm/s",-fMaxVelocity);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.right-size.cx,rr.bottom-size.cy,Buff);

	}
	
	//Draw rulers

	pDC->SelectObject(&penRed);
	pDC->MoveTo(m_pnX[0],rr.bottom);
	pDC->LineTo(m_pnX[0],rr.top);

	pDC->SelectObject(&penGreen);
	pDC->MoveTo(m_pnX[1],rr.bottom);
	pDC->LineTo(m_pnX[1],rr.top);


	float fFrequency = 0.0f;
	float fMaxTime = (float)nSamples / 2.2e3f;
	float fTime = (float)abs(m_pnX[1] - m_pnX[0]) * fMaxTime / (float)rr.Width();
	if(fTime!= 0.0f) fFrequency = 1.0f / fTime;

	Buff.Format(L"t:%.02fs",fTime);
	size = pDC->GetTextExtent(Buff);
	if(m_pnX[1]>m_pnX[0]) nX = m_pnX[1];
	pDC->TextOut(nX,rr.top-size.cy,Buff);

	Buff.Format(L"Fq:%.02fHz",fFrequency);
	if(m_pnX[1]>m_pnX[0]) nX = m_pnX[0];
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(nX-size.cx,rr.top-size.cy,Buff);

	Buff.Format(L"Total t:%.01fs",fMaxTime);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.right-size.cx,rr.top-size.cy,Buff);

	pDC->SelectObject(pOldPen);
	
	delete pt;

}


void CPmacTuningPage::OnSelchangeComboAxisNumber() 
{
	m_nAxisNumber = m_comboAxisNumber.GetCurSel() + 1;
	theApp.m_Thread.m_CpTunning.Zero();
	FillList();
	Invalidate(FALSE);
	UpdateAllControls();
	
}

void CPmacTuningPage::CreateColumns()
{
	CRect rr;
	CString ColumnName[12] = { L"I",L"Description", L"Value"};
	int ColumnWidth[12] = { 36,180,80};

	m_listVariables.GetWindowRect(rr);
	ColumnWidth[2] = rr.Width() - ColumnWidth[0] - ColumnWidth[1] - 4;
	for (int ii=0;ii<3;ii++) {
		m_listVariables.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}

	m_listEncoders.GetWindowRect(rr);
	ColumnWidth[0] = 22;
	ColumnWidth[1] = (rr.Width() - ColumnWidth[0]) / 2 - 4;
	ColumnWidth[2] = rr.Width() - ColumnWidth[0] - ColumnWidth[1] - 4;

	ColumnName[0] = "#";
	ColumnName[1] = "I";
	ColumnName[2] = "Decode";
	for (int ii=0;ii<3;ii++) {
		m_listEncoders.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}

}

void CPmacTuningPage::FillList()
{
	CString	Buff;
	int	ii;

	InitializeStrings();

	m_listVariables.DeleteAllItems();
	for (ii=0;ii<8;ii++) {
		Buff.Format(_T("%d"),m_nOffset[ii]);
		m_listVariables.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	
	m_listVariables.EnsureVisible(0,FALSE);
	m_listVariables.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listVariables.SetFocus();
	m_listVariables.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


	m_listEncoders.DeleteAllItems();
	for(ii=0;ii<16;ii++) {
		Buff.Format(L"%d",ii+1);
		m_listEncoders.InsertItem(ii, Buff.GetBuffer(255), ii);
	}
	m_listEncoders.EnsureVisible(0,FALSE);
	m_listEncoders.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listEncoders.SetFocus();
	m_listEncoders.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


}


void CPmacTuningPage::InitializeStrings()
{
	int nn=0;

	m_strDescription[nn++]="DAC Bias";
	m_strDescription[nn++]="Proportional Gain";
	m_strDescription[nn++]="Derivitive Gain";
	m_strDescription[nn++]="Velocity FeedForward";
	m_strDescription[nn++]="Integral Gain";
	m_strDescription[nn++]="Integration Mode";
	m_strDescription[nn++]="Acceleration Feed Forward";
	m_strDescription[nn++]="Friction Feed Forward";
	m_strDescription[nn++]="Encoder Decode";

	nn=0;
	m_nOffset[nn++]=29 + m_nAxisNumber*100;
	m_nOffset[nn++]=30 + m_nAxisNumber*100;
	m_nOffset[nn++]=31 + m_nAxisNumber*100;
	m_nOffset[nn++]=32 + m_nAxisNumber*100;
	m_nOffset[nn++]=33 + m_nAxisNumber*100;
	m_nOffset[nn++]=34 + m_nAxisNumber*100;
	m_nOffset[nn++]=35 + m_nAxisNumber*100;
	m_nOffset[nn++]=68 + m_nAxisNumber*100;
	m_nOffset[nn++]=m_nEncoderAddrs[m_nAxisNumber - 1];

	for(nn=0;nn<9;nn++) {
		theApp.m_PmacUser.RequestVariable(_T("I"),m_nOffset[nn],&m_strValue[nn]);
	}

}


void CPmacTuningPage::OnGetdispinfoListVariable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		str[0]=0;
		swprintf_s(str,100,_T("%s"),(LPCWSTR)m_strDescription[pDispInfo->item.iItem]);
		break;
	case 2:
		swprintf_s(str,100,_T("%s"), (LPCWSTR)m_strValue[pDispInfo->item.iItem]);
		break;
	}
	
	pDispInfo->item.pszText=str;

	
	*pResult = 0;
}

void CPmacTuningPage::OnDblclkListVariable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listVariables.GetWindowRect(CtrlRect);
	m_listVariables.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	
		break;
	case 2: m_nListEdit  = 0;
		m_pEditSpinItem->Initialize(&m_strValue[m_nIndex=pDispInfo->item.mask]);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	}
	*pResult = 0;
}

void CPmacTuningPage::OnClickListVariable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CPmacTuningPage::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}



BOOL CPmacTuningPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);

	return CResizablePage::OnSetActive();
}

void CPmacTuningPage::OnButtonDoIt() 
{
	CString Buff;

	m_nTest = m_comboTest.GetCurSel();
	m_editDistance.GetWindowText(Buff);

	theApp.m_PmacUser.m_nTunningDistance = m_nDistance = _ttoi(Buff);;
	theApp.m_PmacUser.m_nTunningAxis = m_comboAxisNumber.GetCurSel();

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=TUNING_MOVE;


}

BOOL CPmacTuningPage::OnKillActive() 
{
	ClearEditSpin();
	KillTimer(1);
	
	return CResizablePage::OnKillActive();
}

void CPmacTuningPage::OnChangeEditTa() 
{
	CString	Buff;

	m_editTA.GetWindowText(Buff);
	theApp.m_Tank.nTA = _ttoi(Buff);
}

void CPmacTuningPage::OnChangeEditTs() 
{
	CString	Buff;

	m_editTS.GetWindowText(Buff);
	theApp.m_Tank.nTS = _ttoi(Buff);
}


void CPmacTuningPage::OnButtonGoto0() 
{
	theApp.m_PmacUser.m_nTunningAxis = m_comboAxisNumber.GetCurSel() + 1;
	theApp.m_PmacUser.m_nTunningPos = (int)(m_fPos[0][m_nAxisNumber-1] / m_fStepSize);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=MOVETO_HIGH_GAIN;


}

void CPmacTuningPage::OnButtonGoto1() 
{
	theApp.m_PmacUser.m_nTunningAxis = m_comboAxisNumber.GetCurSel() + 1;
	theApp.m_PmacUser.m_nTunningPos = (int)(m_fPos[1][m_nAxisNumber-1] / m_fStepSize);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=MOVETO_HIGH_GAIN;
}

void CPmacTuningPage::OnButtonGoto2() 
{
	theApp.m_PmacUser.m_nTunningAxis = m_comboAxisNumber.GetCurSel() + 1;
	theApp.m_PmacUser.m_nTunningPos = (int)(m_fPos[2][m_nAxisNumber-1] / m_fStepSize);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=MOVETO_HIGH_GAIN;
}

void CPmacTuningPage::OnButtonStore() 
{
	CString Buff;

	m_editPosition.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[1][m_nAxisNumber-1]);

	Buff.Format(_T("%.03f"),m_fPos[1][m_nAxisNumber-1]);
	m_editPos1.SetWindowText(Buff);
	
}

void CPmacTuningPage::OnChangeEditmultiply() 
{
	CString Buff;

	m_editInPosMultiplier.GetWindowText(Buff);
	theApp.m_LastSettings.nInPosGainMultiplier = _ttoi(Buff);

}

void CPmacTuningPage::OnChangeEditPos0() 
{
	CString Buff;

	m_editPos0.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[0][m_nAxisNumber-1]);
}

void CPmacTuningPage::OnChangeEditPos1() 
{
	CString Buff;

	m_editPos1.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[1][m_nAxisNumber-1]);
}

void CPmacTuningPage::OnChangeEditPos2() 
{
	CString Buff;

	m_editPos2.GetWindowText(Buff);
	_WTOF(Buff,m_fPos[2][m_nAxisNumber-1]);
}

void CPmacTuningPage::OnChangeEditStepsize() 
{
	CMainFrame* pFrame = (CMainFrame*)theApp.m_pMainWnd;
	CString Buff;
	float fTemp;

	m_editStepSize.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	if(fTemp - m_fStepSize) {
		m_fStepSize = fTemp;
		for(int ii=0;ii<16;ii++) {
			if((theApp.m_Axes[ii].nPhysicalAxis == m_nAxisNumber) && (theApp.m_Axes[ii].bAvailable==TRUE)) {
				theApp.m_Axes[ii].fStepSize = m_fStepSize;
				break;
			}
		}

		pFrame->SendMessage(UI_UPDATE_AXES, NULL, NULL);
	}

}

void CPmacTuningPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	int nAxis = m_nAxisNumber - 1;
//	int nPos = theApp.m_PmacUser.m_nPosition[nAxis];
	int nPos = theApp.m_Axes[nAxis].nPos+theApp.m_Axes[nAxis].nOffset;
	static float fOldStepSize;
	CString Unit[3] = {L" mm",L"\x020",L"\x0b0"};
	int ii;

	for(ii=0;ii<16;ii++) {
		if((theApp.m_Axes[ii].nPhysicalAxis == m_nAxisNumber) && (theApp.m_Axes[ii].bAvailable==TRUE)) {
			if(m_fStepSize - theApp.m_Axes[ii].fStepSize) {
				m_fStepSize = theApp.m_Axes[ii].fStepSize;
				Buff.Format(_T("%.7f %s"),m_fStepSize,Unit[theApp.m_Axes[ii].nUnitSelected]);
				m_editStepSize.SetWindowText(Buff);
			}
			break;
		}
	}

	if((nPos - m_nOldPos) || (fOldStepSize - m_fStepSize)) {
		m_nOldPos = nPos;
		fOldStepSize = m_fStepSize;
		Buff.Format(_T("%d"), m_nOldPos);
		m_editEncoderCount.SetWindowText(Buff);

		Buff.Format(_T("%.03f%s"), (float)m_nOldPos * m_fStepSize,Unit[theApp.m_Axes[ii].nUnitSelected]);
		m_editPosition.SetWindowText(Buff);
	}
	
	CResizablePage::OnTimer(nIDEvent);
}

void CPmacTuningPage::OnButtonNotchFilter() 
{

	CalculateNotchFilter(true);
}

void CPmacTuningPage::CalculateNotchFilter(bool bDownLoad) 
{
	double	dNotchFrequency;
	double	dNotchDamping;
	double	dBandPassFrequency;
	double	dBandPassDamping;
	CString Buff;

	m_editNotchFrequency.GetWindowText(Buff);
	_WTOD(Buff, dNotchFrequency);
	m_editNotchDamping.GetWindowText(Buff);
	_WTOD(Buff, dNotchDamping);
	m_editBandPassFrequency.GetWindowText(Buff);
	_WTOD(Buff, dBandPassFrequency);
	m_editBandPassDamping.GetWindowText(Buff);
	_WTOD(Buff, dBandPassDamping);


	double dSReal = -2.0 * PI * dNotchFrequency * dNotchDamping;
	double dSImag = 2.0 * PI * dNotchFrequency * sqrt(1.0 - (dNotchDamping * dNotchDamping));

	double dZReal = exp(dSReal * 0.000442) * cos(dSImag * 0.000442);	//servo update rate 442 microseconds
	double dZImag = exp(dSReal * 0.000442) * sin(dSImag * 0.000442);	//servo update rate 442 microseconds

	double dN1 = -2.0 * dZReal;
	double dN2 = (dZReal*dZReal) + (dZImag*dZImag);

	double dNDCGain = 1 + dN1 + dN2;

	dSReal = -2.0 * PI * dBandPassFrequency * dBandPassDamping;
	dSImag = 2.0 * PI * dBandPassFrequency * sqrt(1.0 - (dBandPassDamping * dBandPassDamping));

	dZReal = exp(dSReal * 0.000442) * cos(dSImag * 0.000442);	//servo update rate 442 microseconds
	dZImag = exp(dSReal * 0.000442) * sin(dSImag * 0.000442);	//servo update rate 442 microseconds

	double dD1 = -2.0 * dZReal;
	double dD2 = (dZReal*dZReal) + (dZImag*dZImag);

	double dDDCGain = 1.0 / (1 + dD1 + dD2);

	double dDCGainI330;
	if((dD1 + dD2) != -1.0) {
		dDCGainI330 = 1.0 / (dNDCGain * dDDCGain);
	} else {
		dDCGainI330 = 1.0;
	}

	Buff.Format(_T("%.3f"),dDCGainI330);
	m_editIX30Gain.SetWindowText(Buff);

	if(bDownLoad == true) {
		theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+36,(float)dN1);
		theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+37,(float)dN2);
		theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+38,(float)dD1);
		theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+39,(float)dD2);

		m_pAxis->fNotchFrequency = (float)dNotchFrequency;
		m_pAxis->fNotchDamping = (float)dNotchDamping;
		m_pAxis->fBandPassFrequency = (float)dBandPassFrequency;
		m_pAxis->fBandPassDamping = (float)dBandPassDamping;
	}
}

void CPmacTuningPage::OnButtonFactory() 
{
	CString Buff;
	float fDefault = 0.0f;

	m_pAxis->fNotchFrequency = 0.0f;
	m_pAxis->fNotchDamping = 0.0f;
	m_pAxis->fBandPassFrequency = 0.0f;
	m_pAxis->fBandPassDamping = 0.0f;
	theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+36,fDefault);
	theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+37,fDefault);
	theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+38,fDefault);
	theApp.m_PmacUser.SetVariable(_T("I"),m_nAxisNumber*100+39,fDefault);

	Buff = "";
	m_editNotchFrequency.SetWindowText(Buff);
	m_editNotchDamping.SetWindowText(Buff);
	m_editBandPassFrequency.SetWindowText(Buff);
	m_editBandPassDamping.SetWindowText(Buff);

	Buff.Format(_T("%.3f"),1.0f);
	m_editIX30Gain.SetWindowText(Buff);
}

BOOL CPmacTuningPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case IDC_EDIT_NOTCH_FREQUENCY:  strTipText = "Generaly about 50Hz";
		break;
	case IDC_EDIT_NOTCH_DAMPING: strTipText = "Lightly damped = 0.2";
		break;
	case IDC_EDIT_BANDPASS_FREQUENCY:  strTipText = "Generaly about 80Hz";
		break;
	case IDC_EDIT_BANDPASS_DAMPING: strTipText = "Heavily damped = 0.8";
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}



void CPmacTuningPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr;
	
	if(GetSafeHwnd()) {
		GetClientRect(m_rrClient);
		AddAnchor(&m_staticView);

		rr = m_rrClient;
	//	ScreenToClient(rr);

		m_pnX[0] = m_pnX[1] = rr.right;
		Invalidate(false);
	}
}

void CPmacTuningPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(m_bLBDown != true) {
		if(rr.PtInRect(point)) {
			if(abs(m_pnX[0] - point.x) < abs(m_pnX[1] - point.x)) {
				m_nMoveWhich = 0;
			} else {
				m_nMoveWhich = 1;
			}
			m_pnX[m_nMoveWhich] = point.x - rr.left;
			m_bLBDown = true;
			Invalidate(false);
		}
	}
	
	CResizablePage::OnLButtonDown(nFlags, point);
}

void CPmacTuningPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBDown = false;
	
	CResizablePage::OnLButtonUp(nFlags, point);
}

void CPmacTuningPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(m_bLBDown == true) {
		if(rr.PtInRect(point)) {
			m_pnX[m_nMoveWhich] = point.x - rr.left;
			Invalidate(false);
		}

	}
	CResizablePage::OnMouseMove(nFlags, point);
}

void CPmacTuningPage::OnCheckDacVoltage() 
{
	theApp.m_LastSettings.nTunningRenderDacVoltage = m_checkDACVoltage.GetCheck();
	Invalidate(false);
}

void CPmacTuningPage::OnCheckFollowingError() 
{
	theApp.m_LastSettings.nTunningRenderFollowingError = m_checkFollowingError.GetCheck();
	Invalidate(false);
}

void CPmacTuningPage::OnCheckVelocity() 
{
	theApp.m_LastSettings.nTunningVelocity = m_checkVelocity.GetCheck();
	Invalidate(false);
}



void CPmacTuningPage::OnGetdispinfoListEncoders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nPtr;

	str[0]=0;

	if(pDispInfo->item.iItem<16) {
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			str[0]=0;
			swprintf_s(str,100,_T("%d"),m_nEncoderAddrs[pDispInfo->item.iItem]);
			break;
		case 2:
			nPtr = m_nEncoderDecode[pDispInfo->item.iItem] & 0x7;
			swprintf_s(str,100,_T("%s"),(LPCWSTR)m_strDecodeDescription[nPtr]);
			break;
		}
	}
	
	pDispInfo->item.pszText=str;

	
	*pResult = 0;
}

void CPmacTuningPage::OnDblclkListEncoders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listEncoders.GetWindowRect(CtrlRect);
	m_listEncoders.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	
		break;
	case 2:	m_nListEdit = 1;
		m_pEditSpinItem->Initialize(&m_nEncoderDecode[m_nIndex=pDispInfo->item.mask],m_strDecodeDescription,8);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	}
	*pResult = 0;
}

void CPmacTuningPage::OnClickListEncoders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

long CPmacTuningPage::UpdatePmacFromList(unsigned int nTemp,long nlTemp)
{
	CString Buff;

	switch(m_nListEdit) {
	case 0:
		Buff.Format(_T("%s%d=%s"),_T("I"),m_nOffset[m_nIndex],m_strValue[m_nIndex]);
		theApp.m_PmacUser.SendStr(Buff);
		break;
	case 1:
		Buff.Format(_T("%s%d=%d"),_T("I"),m_nEncoderAddrs[m_nIndex],m_nEncoderDecode[m_nIndex]);
		theApp.m_PmacUser.SendStr(Buff);
		break;
	};

	return 0;
}

