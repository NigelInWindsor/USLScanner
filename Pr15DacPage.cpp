// Pr15DacPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "Pr15DacPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPr15DacPage property page

IMPLEMENT_DYNCREATE(CPr15DacPage, CPropertyPage)

CPr15DacPage::CPr15DacPage() : CPropertyPage(CPr15DacPage::IDD)
{
	//{{AFX_DATA_INIT(CPr15DacPage)
	//}}AFX_DATA_INIT
	APP App = (APP) AfxGetApp();

	m_pPr = &App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Pr15;
	m_bDialogCreated=FALSE;
	m_pEditSpinItem=FALSE;
	m_pTS = &App->m_UtUser.m_TS[0];

}

CPr15DacPage::~CPr15DacPage()
{
}

void CPr15DacPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPr15DacPage)
	DDX_Control(pDX, IDC_LIST_DAC_TABLE, m_listDac);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE, m_comboTriggerMode);
	DDX_Control(pDX, IDC_SPIN_DAC_THRESHOLD, m_spinDacThreshold);
	DDX_Control(pDX, IDC_SPIN_DAC_BLANKING, m_spinDacBlanking);
	DDX_Control(pDX, IDC_EDIT_DAC_THRESHOLD, m_editDacThreshold);
	DDX_Control(pDX, IDC_EDIT_DAC_BLANKING, m_editDacBlanking);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPr15DacPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPr15DacPage)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, OnSelchangeComboAction)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_DAC_BLANKING, OnChangeEditDacBlanking)
	ON_EN_CHANGE(IDC_EDIT_DAC_THRESHOLD, OnChangeEditDacThreshold)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnSelchangeComboTriggerMode)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DAC_TABLE, OnDblclkListDac)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DAC_TABLE, OnClickListDac)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DAC_TABLE, OnGetdispinfoListDac)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ITEMCHANGED,DacTableChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPr15DacPage message handlers

BOOL CPr15DacPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboAction.AddString("Off");
	m_comboAction.AddString("Unipolar");
	m_comboAction.AddString("BiPolar");

	m_comboTriggerMode.AddString("MainBang");
	m_comboTriggerMode.AddString("Interface");


	m_spinDacBlanking.SetRange(0,255);
	m_spinDacThreshold.SetRange(0,255);

	CreateColumnList();
	m_bDialogCreated=TRUE;

	UpdateAllControls();
	SetAccessPrivelage();


	//This is the wrong place for this
//	App->
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPr15DacPage::UpdateAllControls()
{
	CString	Buff;

	if(m_bDialogCreated==FALSE) return;
	if(m_pPr->tvg_enable==0) {
		m_comboAction.SetCurSel(0);
	} else {
		if(m_pPr->tvg_slope==0) {
			m_comboAction.SetCurSel(1);
		} else {
			m_comboAction.SetCurSel(2);
		}
	}

	m_comboTriggerMode.SetCurSel(m_pPr->tvg_trig_mode);

	Buff.Format("%d \x0b5s",m_pPr->tvg_blanking);
	m_editDacBlanking.SetWindowText(Buff);
	m_spinDacBlanking.SetPos(m_pPr->tvg_blanking);

	Buff.Format("%d",m_pPr->tvg_interface_threshold);
	m_editDacThreshold.SetWindowText(Buff);
	m_spinDacThreshold.SetPos(m_pPr->tvg_interface_threshold);

	
	FillList();

}


void CPr15DacPage::OnSelchangeComboAction() 
{

	APP App = (APP) AfxGetApp();

	switch(m_comboAction.GetCurSel()){
	case 0:
		m_pPr->tvg_enable=0;
		break;
	case 1:
		m_pPr->tvg_enable=1;
		m_pPr->tvg_slope=0;
		break;
	case 2:
		m_pPr->tvg_enable=1;
		m_pPr->tvg_slope=1;
		break;
	};
	App->m_UtUser.Pr15TvgCommands(m_pPr);
	DacTableChanged();

}




void CPr15DacPage::CalculateDacHardware()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	pFrame->SetThreadPriorityIdle();
	double	dMaxTime=0;
	double	dRate;
	double dClkRate[6]={1.0,2.0,4.0,8.0,16.0,32.0};
	int	ii,jj,nMax,nStart,nFinish;
	double dConstant;
	int	nTemp;

//	pFrame->SetThreadPriorityIdle();

	for(ii=0,nMax=0;ii<16;ii++) {
		if(dMaxTime<m_pPr->dTvgOffset[ii]) {
			dMaxTime=m_pPr->dTvgOffset[ii];
			nMax=ii;
		};
	};
	if(dMaxTime<512.0) m_pPr->tvg_clk=3;
	if(dMaxTime<256.0) m_pPr->tvg_clk=2;
	if(dMaxTime<128.0) m_pPr->tvg_clk=1;
	if(dMaxTime<64.0) m_pPr->tvg_clk=0;


	switch(m_pPr->tvg_slope) {
	case 0: memset(&m_pPr->tvg,0,sizeof m_pPr->tvg);
		dConstant=20;
		break;
	case 1: memset(&m_pPr->tvg,128,sizeof m_pPr->tvg);
		dConstant=7;
		break;
	};
	dRate=dClkRate[m_pPr->tvg_clk];
	for(ii=0;ii<=nMax;ii++) {
		nStart=(int)(m_pPr->dTvgOffset[ii] / dRate);
		nFinish=(int)(m_pPr->dTvgOffset[ii+1] / dRate);
		if(nFinish>=nStart) {
			for(jj=nStart;(jj<nFinish)&&(jj<64);jj++) {
				nTemp=(int)(m_pPr->dTvgValue[ii+1] * dConstant);
				switch(m_pPr->tvg_slope) {
				case 0:
					if(nTemp>255) nTemp=255;
					m_pPr->tvg[jj]=nTemp;
					break;
				case 1:
					if(nTemp>127) nTemp=127;
					m_pPr->tvg[jj]=nTemp+128;
					break;
				};
			};
		};
	};

//	pFrame->SetThreadPriorityIdle();
}

void CPr15DacPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_DAC_THRESHOLD:
		m_pPr->tvg_interface_threshold=(pSpin->GetPos()&0xffff);
		Buff.Format("%d",m_pPr->tvg_interface_threshold);
		m_editDacThreshold.SetWindowText(Buff);
		App->m_UtUser.Pr15TvgCommands(m_pPr);
		break;
	case IDC_SPIN_DAC_BLANKING:
		m_pPr->tvg_blanking=(pSpin->GetPos()&0xffff);
		Buff.Format("%d \x0b5s",m_pPr->tvg_blanking);
		m_editDacBlanking.SetWindowText(Buff);
		App->m_UtUser.Pr15TvgCommands(m_pPr);
		break;
	}
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPr15DacPage::OnChangeEditDacBlanking() 
{
	APP App = (APP) AfxGetApp();
	int nTemp=m_pPr->tvg_blanking;
	CString	Buff;

	m_editDacBlanking.GetWindowText(Buff);
	m_pPr->tvg_blanking=atoi(Buff);
	if(nTemp-m_pPr->tvg_blanking) {
		App->m_UtUser.Pr15TvgCommands(m_pPr);
		m_spinDacBlanking.SetPos(m_pPr->tvg_blanking);
	};
}

void CPr15DacPage::OnChangeEditDacThreshold() 
{
	APP App = (APP) AfxGetApp();
	int nTemp=m_pPr->tvg_interface_threshold;
	CString	Buff;

	m_editDacThreshold.GetWindowText(Buff);
	m_pPr->tvg_interface_threshold=atoi(Buff);
	if(nTemp-m_pPr->tvg_interface_threshold) {
		App->m_UtUser.Pr15TvgCommands(m_pPr);
		m_spinDacThreshold.SetPos(m_pPr->tvg_interface_threshold);
	};	
}

void CPr15DacPage::OnSelchangeComboTriggerMode() 
{
	APP App = (APP) AfxGetApp();

	m_pPr->tvg_trig_mode=m_comboTriggerMode.GetCurSel();
	App->m_UtUser.Pr15TvgCommands(m_pPr);
	
}

void CPr15DacPage::SetAccessPrivelage()
{
	if(m_bDialogCreated != TRUE) return;	

	APP App = (APP) AfxGetApp();
	bool bFlag=FALSE;

	if(App->m_nAccessLevel>=SUPERVISOR) bFlag=TRUE;

	m_comboAction.EnableWindow(bFlag);
	m_comboTriggerMode.EnableWindow(bFlag);
	m_editDacBlanking.EnableWindow(bFlag);
	m_spinDacBlanking.EnableWindow(bFlag);
	m_editDacThreshold.EnableWindow(bFlag);
	m_spinDacThreshold.EnableWindow(bFlag);
}


void CPr15DacPage::CreateColumnList()
{
	APP App = (APP) AfxGetApp();
	CString ColumnName[3] = { "#","Delay mm", "Gain"};
	int ColumnWidth[3] = { 0,60,50 };
	CRect rr;

	m_listDac.GetWindowRect(&rr);

	// Delete all of the columns.
	int nColumnCount = m_listDac.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listDac.DeleteColumn(0);
	}

	m_listDac.InsertColumn(0,ColumnName[0], LVCFMT_LEFT,0);
	m_listDac.InsertColumn(1,ColumnName[1], LVCFMT_LEFT,rr.Width()/2-1);
	m_listDac.InsertColumn(2,ColumnName[2], LVCFMT_LEFT,rr.Width()/2-2);

	FillList();
	m_listDac.SetExtendedStyle( LVS_EX_FULLROWSELECT   );
}
void CPr15DacPage::FillList()
{
	CString	Buff;

	m_listDac.DeleteAllItems();
	for (int ii=0;ii<16;ii++) {
		Buff.Format("%d",ii+1);
		m_listDac.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listDac.EnsureVisible(0,FALSE);
	m_listDac.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listDac.SetFocus();
}

void CPr15DacPage::OnClickListDac(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}
//	m_nIndex=pDispInfo->item.mask;
	
	*pResult = 0;
}

void CPr15DacPage::OnDblclkListDac(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	APP App = (APP) AfxGetApp();
	CString	EOL[3]={" ","Scan","Moveto"};

	m_listDac.GetWindowRect(CtrlRect);
	m_listDac.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	if(m_pEditSpinItem!=NULL) delete m_pEditSpinItem;
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 1:	m_pEditSpinItem->Initialize(&m_pTS->Pr15.dTvgOffset[pDispInfo->item.mask],0.0,140.0,0.1,"%.01f");
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2:
		if(m_pTS->Pr15.tvg_slope==0) {
			m_pEditSpinItem->Initialize(&m_pTS->Pr15.dTvgValue[pDispInfo->item.mask],0.0,40.0,0.1,"%.01f");
		} else {
			m_pEditSpinItem->Initialize(&m_pTS->Pr15.dTvgValue[pDispInfo->item.mask],-12.0,40.0,0.1,"%.01f");
		}
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	*pResult = 0;
}

void CPr15DacPage::OnGetdispinfoListDac(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	char str[100];

	str[0]=0;
	switch(pDispInfo->item.iSubItem) {
	case 0:	sprintf(str,"%d",pDispInfo->item.iItem+1);
		break;
	case 1:	sprintf(str,"%.02f",m_pTS->Pr15.dTvgOffset[pDispInfo->item.iItem]);
		break;
	case 2:	sprintf(str,"%.02f",m_pTS->Pr15.dTvgValue[pDispInfo->item.iItem]);
		break;
	}
	pDispInfo->item.pszText=str;	
	*pResult = 0;
}

void CPr15DacPage::OldDacTableChanged()
{
	APP App = (APP) AfxGetApp();
	double	dMaxTime=0;
	double	dRate;
	double dClkRate[6]={1.0,2.0,4.0,8.0,16.0,32.0};
	int	ii,jj,nMax,nStart,nFinish;
	double dConstant;
	int	nTemp;

//	App->m_Thread.SetThreadPriorityIdle();
	for(ii=0,nMax=0;ii<16;ii++) {
		if(dMaxTime<m_pTS->Pr15.dTvgOffset[ii]) {
			dMaxTime=m_pTS->Pr15.dTvgOffset[ii];
			nMax=ii;
		};
	};
	if(dMaxTime<512.0) m_pTS->Pr15.tvg_clk=3;
	if(dMaxTime<256.0) m_pTS->Pr15.tvg_clk=2;
	if(dMaxTime<128.0) m_pTS->Pr15.tvg_clk=1;
	if(dMaxTime<64.0) m_pTS->Pr15.tvg_clk=0;


	switch(m_pTS->Pr15.tvg_slope) {
	case 0: memset(&m_pTS->Pr15.tvg,0,sizeof m_pTS->Pr15.tvg);
		dConstant=20;
		break;
	case 1: memset(&m_pTS->Pr15.tvg,128,sizeof m_pTS->Pr15.tvg);
		dConstant=7;
		break;
	};
	dRate=dClkRate[m_pTS->Pr15.tvg_clk];
	for(ii=0;ii<=nMax;ii++) {
		nStart=(int)(m_pTS->Pr15.dTvgOffset[ii] / dRate);
		nFinish=(int)(m_pTS->Pr15.dTvgOffset[ii+1] / dRate);
		if(nFinish>=nStart) {
			for(jj=nStart;(jj<nFinish)&&(jj<64);jj++) {
				nTemp=(int)(m_pTS->Pr15.dTvgValue[ii+1] * dConstant);
				switch(m_pTS->Pr15.tvg_slope) {
				case 0:
					if(nTemp>255) nTemp=255;
					m_pTS->Pr15.tvg[jj]=nTemp;
					break;
				case 1:
					if(nTemp>127) nTemp=127;
					m_pTS->Pr15.tvg[jj]=nTemp+128;
					break;
				};
			};
		};
	};
	App->m_UtUser.Pr15TvgCommands(&m_pTS->Pr15);
	App->m_UtUser.Pr15TvgData(&m_pTS->Pr15);
//	App->m_Thread.SetThreadPriorityNormal();

}



void CPr15DacPage::DacTableChanged()
{
	APP App = (APP) AfxGetApp();
	double	dMaxTime=0;
	double	dRate;
	double dClkRate[6]={1.0,2.0,4.0,8.0,16.0,32.0};
	int	ii,jj,nMax,nStart,nFinish;
	double dConstant;
	int	nTemp,nTotal;
	int nStep;
	double dTemp,dTotal;
	double dOffset[16];
	double dValue[16];
	CMainFrame* pFrame = (CMainFrame*)App->m_pMainWnd;

	switch(App->m_LastSettings.nDacUnits=1) {
	case 0:
		for(ii=0;ii<16;ii++) {
			dOffset[ii]=m_pTS->Pr15.dTvgOffset[ii];
			dValue[ii]=m_pTS->Pr15.dTvgValue[ii];
		}
		break;
	case 1:
		dOffset[0]=0.0;
		dValue[0]=0.0;
		if(m_pTS->Pr15.tvg_slope==0) {
			for(ii=1;ii<16;ii++) {
				dOffset[ii]=m_pTS->Pr15.dTvgOffset[ii]/(double)App->m_LastSettings.nMaterialVelocity*2e3;
				dValue[ii]=(m_pTS->Pr15.dTvgValue[ii]-m_pTS->Pr15.dTvgValue[ii-1])/(m_pTS->Pr15.dTvgOffset[ii]-m_pTS->Pr15.dTvgOffset[ii-1]);
				if(dValue[ii]<0.0) dValue[ii]=0.0;
				dValue[ii]*=1.0;
			}
		} else {
			for(ii=1;ii<16;ii++) {
				dOffset[ii]=m_pTS->Pr15.dTvgOffset[ii]/(double)App->m_LastSettings.nMaterialVelocity*2e3;
				dValue[ii]=(m_pTS->Pr15.dTvgValue[ii]-m_pTS->Pr15.dTvgValue[ii-1])/(m_pTS->Pr15.dTvgOffset[ii]-m_pTS->Pr15.dTvgOffset[ii-1]);
				dValue[ii]*=3.0;
			}
		}
		break;
	}

	//Force it 
//	m_pTS->Pr15.tvg_slope = 1;
//	m_pTS->Pr15.nTvgEnable = 1;
/*
	switch(m_pTS->Pr15.nTvgDisplayedTriggerMode) {
	case 0:
		for(ii=0;ii<16;ii++) {
			dOffset[ii]=0.0;
			dValue[ii]=0.;
		}
		break;
	case 1:	m_pTS->Pr15.nTvgTriggerMode=0;
		break;
	case 2:	m_pTS->Pr15.nTvgTriggerMode=1;
		break;
	}
*/

//	pFrame->SetThreadPriorityIdle();
	for(ii=0,nMax=0;ii<16;ii++) {
		if(dMaxTime<dOffset[ii]) {
			dMaxTime=dOffset[ii];
			nMax=ii;
		};
	};
	if(dMaxTime<512.0) m_pTS->Pr15.tvg_clk=3;
	if(dMaxTime<256.0) m_pTS->Pr15.tvg_clk=2;
	if(dMaxTime<128.0) m_pTS->Pr15.tvg_clk=1;
	if(dMaxTime<64.0) m_pTS->Pr15.tvg_clk=0;


	switch(m_pTS->Pr15.tvg_slope) {
	case 0: memset(&m_pTS->Pr15.tvg,0,sizeof m_pTS->Pr15.tvg);
		dConstant=120;
		break;
	case 1: 
		for(ii=0;ii<64;ii++) {
			m_pTS->Pr15.tvg[ii]=129;
		}
		dConstant=20;
		break;
	};
	dRate=dClkRate[m_pTS->Pr15.tvg_clk];
	for(ii=0;ii<=nMax;ii++) {
		nStart=(int)(dOffset[ii] / dRate);
		nFinish=(int)(dOffset[ii+1] / dRate);
		if(nFinish>=nStart) {
			nTemp=(int)(dValue[ii+1] * dConstant);
			dTemp=dValue[ii+1] * dConstant;
			if(dTemp>255) dTemp=255;
			nStep=(int)(1.0 / dTemp);
			if(nStep<1) nStep=1;
			if( (dTemp>0.0) && (nTemp==0) ) nTemp=1;
			nTotal=0;
			dTotal=dTemp;
			for(jj=nStart;(jj<nFinish)&&(jj<64);jj++) {
				switch(m_pTS->Pr15.tvg_slope) {
				case 0:
					m_pTS->Pr15.tvg[jj]=(int)(dTotal-(double)nTotal);
					dTotal+=dTemp;
					nTotal+=m_pTS->Pr15.tvg[jj];
					break;
				case 1:
					m_pTS->Pr15.tvg[jj]=(int)(dTotal-(double)nTotal)+129;
					dTotal+=dTemp;
					nTotal+=(m_pTS->Pr15.tvg[jj]-129);
					break;
				};
			};
		};
	};
	App->m_UtUser.Pr15TvgCommands(&m_pTS->Pr15);
	App->m_UtUser.Pr15TvgData(&m_pTS->Pr15);
//	pFrame->SetThreadPriorityNormal();

}

BOOL CPr15DacPage::OnKillActive() 
{
	if(m_pEditSpinItem!=NULL) delete m_pEditSpinItem;
	m_pEditSpinItem = NULL;
	
	return CPropertyPage::OnKillActive();
}
