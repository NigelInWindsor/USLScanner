// TaughtToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TaughtToolsDlg.h"
#include "MainFrm.h"
#include "TaughtLinesEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaughtToolsDlg dialog


CTaughtToolsDlg::CTaughtToolsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CTaughtToolsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaughtToolsDlg)
	//}}AFX_DATA_INIT
	
	m_pDlg = pDlg;
	
	m_fExtrapolateValue = 0.0f;
	m_fSliceOffValue = 0.0f;

	m_nStartLine = 0;
	m_nFinishLine = PROFILE->m_nScanLineL-1;
	m_fWaterPath = theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE];

	m_pCopyLine = NULL;
	m_nCopyLineL = 0;
}

CTaughtToolsDlg::~CTaughtToolsDlg()
{
	SAFE_DELETE_ARRAY( m_pCopyLine );

}

void CTaughtToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaughtToolsDlg)
	DDX_Control(pDX, IDC_BUTTON_DOUBLE_SCAN_LINES, m_buttonDoubleScanLines);
	DDX_Control(pDX, IDC_COMBO_UNTWIST_DIRECTION, m_comboUntwistDirection);
	DDX_Control(pDX, IDC_COMBO_ROTATE_WHAT, m_comboRotateWhat);
	DDX_Control(pDX, IDC_EDIT_ROTATE_ANGLE, m_editRotateAngle);
	DDX_Control(pDX, IDC_COMBO_ROTATE_AXIS, m_comboRotateAxis);
	DDX_Control(pDX, IDC_SPIN_WP, m_spinWaterPath);
	DDX_Control(pDX, IDC_EDIT_WP, m_editWaterPath);
	DDX_Control(pDX, IDC_EDIT_SET_ALL_NORMALS_TO, m_editSetNormalTo);
	DDX_Control(pDX, IDC_COMBO_NORMAL_CHOICE, m_comboNormalChoice);
	DDX_Control(pDX, IDC_COMBO_UNTWIST_MODE, m_comboUnTwistMode);
	DDX_Control(pDX, IDC_EDIT_DELETE_FINISH_LINE, m_editDeleteFinishLine);
	DDX_Control(pDX, IDC_EDIT_DELETE_START_LINE, m_editDeleteStartLine);
	DDX_Control(pDX, IDC_COMBO_SPLIT_METHOD, m_comboSplitMethod);
	DDX_Control(pDX, IDC_EDIT_SLICE_OFF_POS, m_editSliceOffPos);
	DDX_Control(pDX, IDC_EDIT_EXTRAPOLATE_DISTANCE, m_editExtrapolateDistance);
	DDX_Control(pDX, IDC_COMBO_SLICE_OFF_MODE, m_comboSliceOffMode);
	DDX_Control(pDX, IDC_COMBO_EXTRAPOLATE_MODE, m_comboExtrapolateMode);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_RESOLUTION, m_editResolution);
	DDX_Control(pDX, IDC_COMBO_PERPENDICULAR_STYLE, m_comboPerpendicularStyle);
	DDX_Control(pDX, IDC_COMBO_COMPUTE_SCAN_AXIS, m_comboComputeScanAxis);
	DDX_Control(pDX, IDC_COMBO_ALOGORITHM, m_comboAlgorithm);
	DDX_Control(pDX, IDC_COMBO_COPUTE_FACE, m_comboComputeFace);
	DDX_Control(pDX, IDC_COMBO_SORT_AXIS, m_comboSortAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaughtToolsDlg, CDialog)
	//{{AFX_MSG_MAP(CTaughtToolsDlg)
	ON_BN_CLICKED(IDC_BUTTON_SORT_ALL_LINES, OnButtonSortAllLines)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_SCANLINES, OnButtonGenerateScanlines)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE_FACE, OnButtonComputeFace)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPUTE_SCAN_AXIS, OnSelchangeComboComputeScanAxis)
	ON_CBN_SELCHANGE(IDC_COMBO_ALOGORITHM, OnSelchangeComboAlogorithm)
	ON_CBN_SELCHANGE(IDC_COMBO_PERPENDICULAR_STYLE, OnSelchangeComboPerpendicularStyle)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT_AXIS, OnSelchangeComboSortAxis)
	ON_EN_CHANGE(IDC_EDIT_RESOLUTION, OnChangeEditResolution)
	ON_BN_CLICKED(IDC_BUTTON_EXTRAPOLATE_MIN_EDGE, OnButtonExtrapolateMinEdge)
	ON_BN_CLICKED(IDC_BUTTON_EXTRAPOLATE_MAX_EDGE, OnButtonExtrapolateMaxEdge)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE_NORMALS, OnButtonComputeNormals)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_CBN_SELCHANGE(IDC_COMBO_EXTRAPOLATE_MODE, OnSelchangeComboExtrapolateMode)
	ON_CBN_SELCHANGE(IDC_COMBO_SLICE_OFF_MODE, OnSelchangeComboSliceOffMode)
	ON_BN_CLICKED(IDC_BUTTON_SLICE_OFF_MIN, OnButtonSliceOffMin)
	ON_BN_CLICKED(IDC_BUTTON_SLICE_OFF_MAX, OnButtonSliceOffMax)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_GREATER_THAN, OnButtonDeleteGreaterThan)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_TO_RIGHT, OnButtonLeftToRight)
	ON_BN_CLICKED(IDC_BUTTON_SPLIT, OnButtonSplit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_UNTWIST_MODE, OnSelchangeComboUntwistMode)
	ON_BN_CLICKED(IDC_BUTTON_UNTWIST, OnButtonUntwist)
	ON_BN_CLICKED(IDC_BUTTON_SET_ALL_NORMALS, OnButtonSetAllNormals)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_START_LINE, OnButtonEnterStartLine)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_FINISH_LINE, OnButtonEnterFinishLine)
	ON_BN_CLICKED(IDC_BUTTON_SET_WP, OnButtonSetWp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WP, OnDeltaposSpinWp)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_NEG_K, OnButtonDeleteNegK)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POS_K, OnButtonDeletePosK)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, OnButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_MULTIPLY, OnButtonMultiply)
	ON_BN_CLICKED(IDC_BUTTON_SET_R_POS, OnButtonSetRPos)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_45_DEGREES, OnButtonDelete45Degrees)
	ON_BN_CLICKED(IDC_BUTTON_BLADE_EDGE_CLIP, OnButtonBladeEdgeClip)
	ON_BN_CLICKED(IDC_BUTTON_KEEP_COPY, OnButtonKeepCopy)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_COPY, OnButtonReplaceCopy)
	ON_BN_CLICKED(IDC_BUTTON_SCANLINES_TO_OUTSIDE, OnButtonScanlinesToOutside)
	ON_BN_CLICKED(IDC_BUTTON_SCANLINES_TO_INSIDE, OnButtonScanlinesToInside)
	ON_CBN_SELCHANGE(IDC_COMBO_UNTWIST_DIRECTION, OnSelchangeComboUntwistDirection)
	ON_BN_CLICKED(IDC_BUTTON_ADD_R_POS, OnButtonAddRPos)
	ON_BN_CLICKED(IDC_BUTTON_DOUBLE_SCAN_LINES, OnButtonDoubleScanLines)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_BLADE, OnButtonRotateBlade)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_NEG_I, OnButtonDeleteNegI)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POS_I, OnButtonDeletePosI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaughtToolsDlg message handlers

void CTaughtToolsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CTaughtToolsDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CTaughtToolsDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}

void CTaughtToolsDlg::OnDestroy() 
{
	

	if (CTaughtToolsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CTaughtToolsDlg::IDD]);
	}

	CDialog::OnDestroy();
	if(m_pDlg) *m_pDlg = NULL;			
	
}

BOOL CTaughtToolsDlg::OnInitDialog() 
{
	
	CDialog::OnInitDialog();
	CString Buff;
	int nSide = 0;
	int nNotSide = 1;

	switch(theApp.m_nSide0Orientation) {
	case 0:
		break;
	case 1:
		nSide = 1;
		nNotSide = 0;
		break;
	};

	if (CTaughtToolsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CTaughtToolsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	m_spinSlowIncrement.SetRange(0,200);
	m_spinWaterPath.SetRange(0,200);


	Buff.Format(L"X %s",theApp.m_DlgSideName[nSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Y %s",theApp.m_DlgSideName[nSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Z %s",theApp.m_DlgSideName[nSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Xt %s",theApp.m_DlgSideName[nSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Yt %s",theApp.m_DlgSideName[nSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"X %s",theApp.m_DlgSideName[nNotSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Y %s",theApp.m_DlgSideName[nNotSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Z %s",theApp.m_DlgSideName[nNotSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Xt %s",theApp.m_DlgSideName[nNotSide]); m_comboSortAxis.AddString(Buff);	
	Buff.Format(L"Yt %s",theApp.m_DlgSideName[nNotSide]); m_comboSortAxis.AddString(Buff);	

	Buff.Format(L"%s side from %s side",theApp.m_DlgSideName[nNotSide], theApp.m_DlgSideName[nSide]); m_comboComputeFace.AddString(Buff);
	Buff.Format(L"%s side from %s side",theApp.m_DlgSideName[nSide], theApp.m_DlgSideName[nNotSide]); m_comboComputeFace.AddString(Buff);
	m_comboComputeFace.SetCurSel(0);

	m_comboComputeScanAxis.AddString(_T("X"));
	m_comboComputeScanAxis.AddString(_T("Y"));
	m_comboComputeScanAxis.AddString(_T("Z"));

	Buff.LoadString(IDS_J_And_D_Nose);				m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Taught_Along_Scan_Axis_V1); m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Taught_Perpendicular_Scan_Axis); m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Roto_Symetrical);			m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Asymetric_D_Nose);			m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Taught_Along_Scan_Axis_V2); m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_D_Nose_with_straight_LE);	m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Interpolate_surface);		m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Arc_about_TT_centre);		m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Arc_about_circumcentre);	m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_RR_Raft);					m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Simple_interpolate);		m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Single_line_bscan);			m_comboAlgorithm.AddString(Buff);


	Buff.LoadString(IDS_Linear); m_comboPerpendicularStyle.AddString(Buff);
	Buff.LoadString(IDS_Spline); m_comboPerpendicularStyle.AddString(Buff);

	Buff.LoadString(IDS_Relative_distance);			m_comboExtrapolateMode.AddString(Buff);
	Buff.LoadString(IDS_To_Y_Plane);				m_comboExtrapolateMode.AddString(Buff);
	Buff.LoadString(IDS_Relative_distance_spline);	m_comboExtrapolateMode.AddString(Buff);

	Buff.LoadString(IDS_At_X_Plane);		m_comboSliceOffMode.AddString(Buff);
	Buff.LoadString(IDS_At_Y_Plane);		m_comboSliceOffMode.AddString(Buff);
	Buff.LoadString(IDS_At_Z_Plane);		m_comboSliceOffMode.AddString(Buff);

	Buff.LoadString(IDS_Change_in_X_direction);	m_comboSplitMethod.AddString(Buff);
	Buff.LoadString(IDS_Change_in_X_value);		m_comboSplitMethod.AddString(Buff);
	Buff.LoadString(IDS_Change_in_Y_value);		m_comboSplitMethod.AddString(Buff);
	Buff.LoadString(IDS_Change_in_Z_value);		m_comboSplitMethod.AddString(Buff);

	Buff.LoadString(IDS_Set_X_Equal);			m_comboUnTwistMode.AddString(Buff);
	Buff.LoadString(IDS_Set_Y_Equal);			m_comboUnTwistMode.AddString(Buff);
	Buff.LoadString(IDS_Set_Z_Equal);			m_comboUnTwistMode.AddString(Buff);

	m_comboNormalChoice.AddString(L"i");
	m_comboNormalChoice.AddString(L"j");
	m_comboNormalChoice.AddString(L"k");

	m_editRotateAngle.SetWindowText(L"0.0");

	m_comboRotateAxis.AddString(L"X");
	m_comboRotateAxis.AddString(L"Y");
	m_comboRotateAxis.AddString(L"Z");
	m_comboRotateAxis.SetCurSel(0);

	m_comboRotateWhat.AddString(L"Everything");
	m_comboRotateWhat.AddString(L"Outside Blade");
	m_comboRotateWhat.AddString(L"Inside Blade");
	m_comboRotateWhat.SetCurSel(0);

	Buff.LoadString(IDS_Clockwise);		m_comboUntwistDirection.AddString(Buff);
	Buff.LoadString(IDS_Anticlockwise);	m_comboUntwistDirection.AddString(Buff);

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaughtToolsDlg::UpdateAllControls()
{
	
	CString Buff;

	m_comboComputeScanAxis.SetCurSel(PROFILE->m_nFastAxis);
	m_comboAlgorithm.SetCurSel(PROFILE->m_nComputeAlgorithm);
	m_comboPerpendicularStyle.SetCurSel(PROFILE->m_nPerpendicularStyle);


	m_comboSortAxis.SetCurSel(theApp.m_LastSettings.nTaughtSortSelection);


	Buff.Format(L"%.01f mm",m_fExtrapolateValue);
	m_editExtrapolateDistance.SetWindowText(Buff);

	Buff.Format(L"%.01f mm",m_fSliceOffValue);
	m_editSliceOffPos.SetWindowText(Buff);

	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fSurfaceResolution);
	m_editResolution.SetWindowText(Buff);

	if((theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) || (theApp.m_LSA.m_nActualLawsL <= 1)) {
		m_editSlowIncrement.EnableWindow(true);
	} else {
		PROFILE->m_fSlowIncrement = theApp.m_PhasedArray[PORTSIDE].getTxFocalLawPitch();
		m_editSlowIncrement.EnableWindow(false);
	}
	Buff.Format(_T("%.*f %s"),2,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	m_comboExtrapolateMode.SetCurSel( theApp.m_LastSettings.nExtrapolateMode );
	m_comboSliceOffMode.SetCurSel( theApp.m_LastSettings.nSliceOffMode );

	m_comboSplitMethod.SetCurSel(0);

	Buff.Format(L"%d",m_nStartLine+1);
	m_editDeleteStartLine.SetWindowText(Buff);

	Buff.Format(L"%d",m_nFinishLine+1);
	m_editDeleteFinishLine.SetWindowText(Buff);

	m_comboUnTwistMode.SetCurSel(theApp.m_LastSettings.nUnTwistMode);

	m_comboNormalChoice.SetCurSel(0);

	m_editSetNormalTo.SetWindowText(L"0.000");

	Buff.Format(L"%.01f mm",m_fWaterPath);
	m_editWaterPath.SetWindowText(Buff);

	m_comboUntwistDirection.SetCurSel(theApp.m_LastSettings.nUntwistDirection);


}

void CTaughtToolsDlg::OnButtonSortAllLines() 
{
	

	int nAxis = theApp.m_LastSettings.nTaughtSortSelection = m_comboSortAxis.GetCurSel();
	if(nAxis>=5) nAxis += 4;

	PROFILE->SortEachLine(TAUGHTLINES,nAxis);


}

void CTaughtToolsDlg::OnButtonGenerateScanlines() 
{
	

	PROFILE->GenerateScanLines();
	UpdateProfileSheet();
	
}


void CTaughtToolsDlg::OnButtonComputeFace() 
{
	

	switch(m_comboComputeFace.GetCurSel()) {
	case 0: PROFILE->CalculateOppositeTaughtSide(STARBOARD);
		break;
	case 1: PROFILE->CalculateOppositeTaughtSide(PORTSIDE);
		break;
	}
}

void CTaughtToolsDlg::OnSelchangeComboComputeScanAxis() 
{
	
	int nFastAxis = PROFILE->m_nFastAxis;
	
	PROFILE->m_nFastAxis = m_comboComputeScanAxis.GetCurSel();

	if(PROFILE->m_nFastAxis==PROFILE->m_nSlowAxis)
		PROFILE->m_nSlowAxis=nFastAxis;
}

void CTaughtToolsDlg::OnSelchangeComboAlogorithm() 
{
	
	
	PROFILE->m_nComputeAlgorithm = m_comboAlgorithm.GetCurSel();
}

void CTaughtToolsDlg::OnSelchangeComboPerpendicularStyle() 
{
	
	
	PROFILE->m_nPerpendicularStyle = m_comboPerpendicularStyle.GetCurSel();
	
}

void CTaughtToolsDlg::OnSelchangeComboSortAxis() 
{
	
	theApp.m_LastSettings.nTaughtSortSelection = m_comboSortAxis.GetCurSel();	
}

void CTaughtToolsDlg::OnChangeEditResolution() 
{
	
	CString Buff;

	m_editResolution.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fSurfaceResolution);
}


void CTaughtToolsDlg::OnButtonComputeNormals() 
{
	
	
	CWaitCursor Wait;
	PROFILE->CalculateMathematicalNormals();
	Wait.Restore();
	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnChangeEditSlowIncrement() 
{
	
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
		UpdateMainFrameTools();
	}
}

void CTaughtToolsDlg::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	
	float fStep = 1.0f / (float)pow(10, theApp.m_LastSettings.nStepSizePrecision);

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	} else {
		fStep *= 25.4f;
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	}
	UpdateAllControls();
	UpdateMainFrameTools();
	
	*pResult = 0;
}

void CTaughtToolsDlg::UpdateMainFrameTools()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->PostMessage(UI_UPDATE_MOTION_TOOLS_SHEET);

}

void CTaughtToolsDlg::UpdateProfileSheet()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame->m_pProfileSheet!=NULL) {
		pFrame->m_pProfileSheet->UpdateAllPages();
	};
}

void CTaughtToolsDlg::OnSelchangeComboExtrapolateMode() 
{

	

	theApp.m_LastSettings.nExtrapolateMode = m_comboExtrapolateMode.GetCurSel();
}

void CTaughtToolsDlg::OnButtonExtrapolateMinEdge() 
{
	
	CString Buff;

	m_editExtrapolateDistance.GetWindowText(Buff);
	_WTOF(Buff,m_fExtrapolateValue);

	PROFILE->ExtrapolateScanLines(0,m_fExtrapolateValue);
	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnButtonExtrapolateMaxEdge() 
{
	
	CString Buff;

	m_editExtrapolateDistance.GetWindowText(Buff);
	_WTOF(Buff,m_fExtrapolateValue);

	PROFILE->ExtrapolateScanLines(1,m_fExtrapolateValue);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnSelchangeComboSliceOffMode() 
{
	

	theApp.m_LastSettings.nSliceOffMode = m_comboSliceOffMode.GetCurSel();
}

void CTaughtToolsDlg::OnButtonSliceOffMin() 
{
	
	CString Buff;

	m_editSliceOffPos.GetWindowText(Buff);
	_WTOF(Buff,m_fSliceOffValue);

	PROFILE->CropScanLines(0,m_fSliceOffValue,theApp.m_LastSettings.nSliceOffMode);
	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnButtonSliceOffMax() 
{
	
	CString Buff;

	m_editSliceOffPos.GetWindowText(Buff);
	_WTOF(Buff,m_fSliceOffValue);

	PROFILE->CropScanLines(1,m_fSliceOffValue,theApp.m_LastSettings.nSliceOffMode);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonDeleteGreaterThan() 
{
	
	CString Buff;

	m_editSliceOffPos.GetWindowText(Buff);
	_WTOF(Buff,m_fSliceOffValue);

	PROFILE->DeleteScanLinesThatAreCompletelyOneSideOfThePlane(1,m_fSliceOffValue,theApp.m_LastSettings.nSliceOffMode);
	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnButtonLeftToRight() 
{
	

	PROFILE->ConvertLeftHandedToRightHanded(TAUGHTLINES);
	PROFILE->ConvertLeftHandedToRightHanded(SCANLINES);
	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnButtonSplit() 
{
	

	switch(m_comboSplitMethod.GetCurSel()) {
	case 0:	PROFILE->SplitScanLinesDueToXDirChange();
		break;
	case 1:	PROFILE->SplitScanLinesDueToValueChange(0);
		break;
	case 2:	PROFILE->SplitScanLinesDueToValueChange(1);
		break;
	case 3:	PROFILE->SplitScanLinesDueToValueChange(2);
		break;
	};

	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnButtonDelete() 
{
	
	CString Buff;

	InvalidateStartAndFinishValue();

	PROFILE->DeleteLines(SCANLINES,m_nStartLine,m_nFinishLine);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnSelchangeComboUntwistMode() 
{
	
	
	theApp.m_LastSettings.nUnTwistMode = m_comboUnTwistMode.GetCurSel();
}

void CTaughtToolsDlg::OnButtonUntwist() 
{
	

	PROFILE->Untwist();
	UpdateProfileSheet();
	
}



void CTaughtToolsDlg::OnButtonEnterStartLine() 
{
	m_nStartLine = 0;
	UpdateAllControls();	
	
}

void CTaughtToolsDlg::OnButtonEnterFinishLine() 
{
	
	m_nFinishLine = PROFILE->m_nScanLineL-1;
	UpdateAllControls();	
}

void CTaughtToolsDlg::OnButtonSetWp() 
{
	
	CString Buff;

	m_editWaterPath.GetWindowText(Buff);
	_WTOF(Buff,m_fWaterPath);

	InvalidateStartAndFinishValue();

	PROFILE->SetScanLineWaterPaths(m_nStartLine, m_nFinishLine, m_fWaterPath);
	UpdateProfileSheet();

}

void CTaughtToolsDlg::OnDeltaposSpinWp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	m_fWaterPath += (float)(5 * pNMUpDown->iDelta);
	if(m_fWaterPath<0.0f) m_fWaterPath=0.0f;
	Buff.Format(L"%.01f mm",m_fWaterPath);
	m_editWaterPath.SetWindowText(Buff);

	*pResult = 0;
}

void CTaughtToolsDlg::OnButtonDeleteNegK() 
{
	
	
	InvalidateStartAndFinishValue();
	PROFILE->DeleteScanCoordWith(NEGATIVE,'k',m_nStartLine,m_nFinishLine);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonDeletePosK() 
{
	
	
	InvalidateStartAndFinishValue();
	PROFILE->DeleteScanCoordWith(POSITIVE,'k',m_nStartLine,m_nFinishLine);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::InvalidateStartAndFinishValue()
{
	CString Buff;

	m_editDeleteStartLine.GetWindowText(Buff);
	m_nStartLine = _ttoi(Buff) - 1;
	m_editDeleteFinishLine.GetWindowText(Buff);
	m_nFinishLine = _ttoi(Buff) - 1;

}

void CTaughtToolsDlg::OnButtonRotate() 
{
	
	CString Buff;
	float fAngle;
	int nAxis;

	m_editRotateAngle.GetWindowText(Buff);
	_WTOF(Buff,fAngle);

	nAxis = m_comboRotateAxis.GetCurSel();

	switch(m_comboRotateWhat.GetCurSel()) {
	case 0:	PROFILE->RotateComponent(nAxis,fAngle);
		break;
	case 1:	PROFILE->RotateBlade(nAxis,fAngle,OUTSIDE);
		break;
	case 2:	PROFILE->RotateBlade(nAxis,fAngle,INSIDE);
		break;
	}
	UpdateProfileSheet();

}

void CTaughtToolsDlg::OnButtonSetAllNormals() 
{
	
	float fNormal;
	CString Buff;
	int nNormalChoice = m_comboNormalChoice.GetCurSel();

	m_editSetNormalTo.GetWindowText(Buff);
	_WTOF(Buff,fNormal);

	PROFILE->RotateAllNormalsToIEqualsZero();

	UpdateProfileSheet();

}
void CTaughtToolsDlg::OnButtonMultiply() 
{
	// TODO: Add your control notification handler code here
	
}

void CTaughtToolsDlg::OnButtonSetRPos() 
{
	
	CString Buff;
	float fAngle;

	m_editWaterPath.GetWindowText(Buff);
	_WTOF(Buff,fAngle);

	InvalidateStartAndFinishValue();

	PROFILE->SetScanLineRAngle(m_nStartLine, m_nFinishLine, fAngle);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonDelete45Degrees() 
{
	
	
	PROFILE->DeleteAnythingGreater45();
	UpdateProfileSheet();

}

void CTaughtToolsDlg::OnButtonBladeEdgeClip() 
{
	
	
	PROFILE->BladeEdgeClip();
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonKeepCopy() 
{
	
	
	SAFE_DELETE_ARRAY( m_pCopyLine );
	if(PROFILE->m_nScanLineL > 0) {
		m_pCopyLine = new CPolyCoord[m_nCopyLineL = PROFILE->m_nScanLineL];
		for(int nLine=0;nLine<m_nCopyLineL;nLine++) m_pCopyLine[nLine] = &PROFILE->m_ScanLine[nLine];
	}
}

void CTaughtToolsDlg::OnButtonReplaceCopy() 
{
	
	
	SAFE_DELETE_ARRAY( PROFILE->m_ScanLine );
	PROFILE->m_nScanLineL = 0;

	if(m_nCopyLineL > 0) {
		PROFILE->m_ScanLine = new CPolyCoord[ PROFILE->m_nScanLineL = m_nCopyLineL];
		for(int nLine=0;nLine<m_nCopyLineL;nLine++) PROFILE->m_ScanLine[nLine] = &m_pCopyLine[nLine];
	}
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonScanlinesToOutside() 
{
	
	
	PROFILE->CopyAllLines(OUTSIDELINES,SCANLINES,false);
}

void CTaughtToolsDlg::OnButtonScanlinesToInside() 
{
	
	
	PROFILE->CopyAllLines(INSIDELINES,SCANLINES,false);
}

void CTaughtToolsDlg::OnSelchangeComboUntwistDirection() 
{
	
	
	theApp.m_LastSettings.nUntwistDirection = m_comboUntwistDirection.GetCurSel();
}

void CTaughtToolsDlg::OnButtonAddRPos() 
{
	
	CString Buff;
	float fAngle;

	m_editWaterPath.GetWindowText(Buff);
	_WTOF(Buff,fAngle);

	InvalidateStartAndFinishValue();

	PROFILE->AddScanLineRAngle(m_nStartLine, m_nFinishLine, fAngle);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonDoubleScanLines() 
{
	
	PROFILE->InsertExtraScanLines(1);
	UpdateProfileSheet();

}

void CTaughtToolsDlg::OnButtonRotateBlade() 
{
	
	CString Buff;
	float fAngle;
	int nAxis;

	InvalidateStartAndFinishValue();

	m_editRotateAngle.GetWindowText(Buff);
	_WTOF(Buff,fAngle);

	nAxis = 1;

	PROFILE->RotateComponent(nAxis,fAngle);
	PROFILE->SortEachLine(SCANLINES,0);
	PROFILE->AddScanLineRAngle(m_nStartLine, m_nFinishLine, fAngle);

	UpdateProfileSheet();
	
}

void CTaughtToolsDlg::OnButtonDeleteNegI() 
{
	
	
	InvalidateStartAndFinishValue();
	PROFILE->DeleteScanCoordWith(NEGATIVE,'i',m_nStartLine,m_nFinishLine);
	UpdateProfileSheet();
}

void CTaughtToolsDlg::OnButtonDeletePosI() 
{
	
	
	InvalidateStartAndFinishValue();
	PROFILE->DeleteScanCoordWith(POSITIVE,'i',m_nStartLine,m_nFinishLine);
	UpdateProfileSheet();
}
