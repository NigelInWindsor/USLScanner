// EditCoordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EditCoordDlg.h"
#include "3DViewEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCoordDlg dialog


CEditCoordDlg::CEditCoordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCoordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCoordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Text = "Set All";
	m_Action = "To";
	m_pParent = pParent;
}


void CEditCoordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCoordDlg)
	DDX_Control(pDX, IDC_BUTTON_TRANSLATE, m_buttonTranslate);
	DDX_Control(pDX, IDC_STATIC_ACTION, m_editAction);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_editText);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_editValue);
	DDX_Control(pDX, IDC_COMBO_WHICH_AXIS, m_comboWhichAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditCoordDlg, CDialog)
	//{{AFX_MSG_MAP(CEditCoordDlg)
	ON_BN_CLICKED(IDC_BUTTON_TRANSLATE, OnButtonTranslate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCoordDlg message handlers

BOOL CEditCoordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	
	m_comboWhichAxis.AddString(_T("X"));
	m_comboWhichAxis.AddString(_T("Y"));
	m_comboWhichAxis.AddString(_T("Z"));
	m_comboWhichAxis.AddString(_T("I"));
	m_comboWhichAxis.AddString(_T("J"));
	m_comboWhichAxis.AddString(_T("K"));
	m_comboWhichAxis.SetCurSel(0);

	m_editText.SetWindowText(m_Text);
	m_editAction.SetWindowText(m_Action);
	
	Buff.Format(_T("Translate so that index %d is at water jet"),theApp.m_nIndex+1);
	m_buttonTranslate.SetWindowText(Buff);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditCoordDlg::SetText(CString Text,CString Action)
{

	m_Text=Text;
	m_Action=Action;
}

void CEditCoordDlg::OnOK() 
{
	CString Buff;
	int nAxis[12] = {0,1,2,6,7,8,10,11,12,16,17,18};	//quad done

	m_editValue.GetWindowText(Buff);
	_WTOF(Buff,m_fValue);
	m_nAxis = nAxis[m_comboWhichAxis.GetCurSel()];

	CDialog::OnOK();
}

void CEditCoordDlg::OnButtonTranslate() 
{
	CCoord CpSurface,CpHead;

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		break;
	}

	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;

	PROFILE->m_fRotateX = 0.0;
	PROFILE->m_fRotateY = 0.0;
	PROFILE->m_fRotateZ = 0.0;
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
		PROFILE->m_fTranslateX = CpSurface.Side0.fX - PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fX;
		PROFILE->m_fTranslateY = CpSurface.Side0.fY - PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fY;
		PROFILE->m_fTranslateZ = CpSurface.Side0.fZ - PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fZ;
		break;
	case SCANLINES:
		PROFILE->m_fTranslateX = CpSurface.Side0.fX - PROFILE->m_ScanLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fX;
		PROFILE->m_fTranslateY = CpSurface.Side0.fY - PROFILE->m_ScanLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fY;
		PROFILE->m_fTranslateZ = CpSurface.Side0.fZ - PROFILE->m_ScanLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fZ;
		break;
	case BRAINLINES:
		PROFILE->m_fTranslateX = CpSurface.Side0.fX - PROFILE->m_BrainLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fX;
		PROFILE->m_fTranslateY = CpSurface.Side0.fY - PROFILE->m_BrainLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fY;
		PROFILE->m_fTranslateZ = CpSurface.Side0.fZ - PROFILE->m_BrainLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fZ;
		break;
	case ENVELOPELINES:
		PROFILE->m_fTranslateX = CpSurface.Side0.fX - PROFILE->m_EnvelopeLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fX;
		PROFILE->m_fTranslateY = CpSurface.Side0.fY - PROFILE->m_EnvelopeLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fY;
		PROFILE->m_fTranslateZ = CpSurface.Side0.fZ - PROFILE->m_EnvelopeLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fZ;
		break;
	}
	PROFILE->AcceptTransform();


	m_fValue=0.0f;
	m_nAxis=0;

	CDialog::OnOK();

}
