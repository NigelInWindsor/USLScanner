
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "UTKernelLayoutFile.h"
#include "SectorApp.h"
#include "DynamicView.h"
#include "DynamicNCView.h"
#include "MainFrm.h"
#include "AscopeToolbar.h"
#include "DataFile.h"
#include "DynamicWnd.h"
#include "DynamicNCWnd.h"
#include "DynamicCscopeView.h"
#include "HardwareCscopeView.h"
#include "AscopeView.h"
#include "BscopeView.h"
#include "CscopeView.h"
#include "DscopeView.h"
#include "DscopeCorrectedView.h"
#include "win_stub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

__declspec(dllimport) const wchar_t* UTKernel_GetFolderDataFiles(const wchar_t *pSubFolder);//integration
__declspec(dllimport) void UTKernel_SetFolderDataFiles(const wchar_t *pSubFolder,const wchar_t *pFolder);//integration
CAnalysisToolsDlg* g_pAnalysisDlg=NULL;//integration
UINT g_uiUTEventMessage=0;//integration
bool g_bBackwallRebound=true;//integration

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_LOAD_DEMO_FILE, &CMainFrame::OnFileDemoLoad)
	ON_COMMAND(ID_FILE_LOAD_DATA, &CMainFrame::OnFileLoad)
	ON_COMMAND(ID_FILE_SAVE_DATA, &CMainFrame::OnFileSave)

	ON_COMMAND(ID_VIEWS_DYNAMIC, &CMainFrame::OnDynamicView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_DYNAMIC, &CMainFrame::OnUpdateDynamicView)

	ON_COMMAND(ID_VIEWS_DYNAMIC_NC, &CMainFrame::OnDynamicNCView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_DYNAMIC_NC, &CMainFrame::OnUpdateDynamicNCView)

	ON_COMMAND(ID_VIEWS_DYNAMIC_CSCOPE, &CMainFrame::OnDynamicCscopeView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_DYNAMIC_CSCOPE, &CMainFrame::OnUpdateDynamicCscopeView)

	ON_COMMAND(ID_VIEWS_ASCOPE, &CMainFrame::OnAscopeView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_ASCOPE, &CMainFrame::OnUpdateAscopeView)

	ON_COMMAND(ID_VIEWS_BSCOPE, &CMainFrame::OnBscopeView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_BSCOPE, &CMainFrame::OnUpdateBscopeView)

	ON_COMMAND(ID_VIEWS_CSCOPE, &CMainFrame::OnCscopeView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_CSCOPE, &CMainFrame::OnUpdateCscopeView)

	ON_COMMAND(ID_VIEWS_DSCOPE, &CMainFrame::OnDscopeView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_DSCOPE, &CMainFrame::OnUpdateDscopeView)

	ON_COMMAND(ID_VIEWS_DSCOPE_CORRECTED, &CMainFrame::OnDscopeCorrectedView)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_DSCOPE_CORRECTED, &CMainFrame::OnUpdateDscopeCorrectedView)

	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_AMP_0, &CMainFrame::OnViewsHardwareCscopeAmp0)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_AMP_0, &CMainFrame::OnUpdateViewsHardwareCscopeAmp0)
	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_AMP_1, &CMainFrame::OnViewsHardwareCscopeAmp1)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_AMP_1, &CMainFrame::OnUpdateViewsHardwareCscopeAmp1)
	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_AMP_2, &CMainFrame::OnViewsHardwareCscopeAmp2)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_AMP_2, &CMainFrame::OnUpdateViewsHardwareCscopeAmp2)
	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_AMP_3, &CMainFrame::OnViewsHardwareCscopeAmp3)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_AMP_3, &CMainFrame::OnUpdateViewsHardwareCscopeAmp3)

	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_TOF_0, &CMainFrame::OnViewsHardwareCscopeTof0)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_TOF_0, &CMainFrame::OnUpdateViewsHardwareCscopeTof0)
	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_TOF_1, &CMainFrame::OnViewsHardwareCscopeTof1)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_TOF_1, &CMainFrame::OnUpdateViewsHardwareCscopeTof1)
	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_TOF_2, &CMainFrame::OnViewsHardwareCscopeTof2)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_TOF_2, &CMainFrame::OnUpdateViewsHardwareCscopeTof2)
	ON_COMMAND(ID_VIEWS_HARDWARE_CSCOPE_TOF_3, &CMainFrame::OnViewsHardwareCscopeTof3)
	ON_UPDATE_COMMAND_UI(ID_VIEWS_HARDWARE_CSCOPE_TOF_3, &CMainFrame::OnUpdateViewsHardwareCscopeTof3)

	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CMainFrame::OnEventMessage)//integration
END_MESSAGE_MAP()

// CMainFrame construction/destruction
CMainFrame *CMainFrame::g_pMainFrame=NULL;
CMainFrame::CMainFrame()
{
	m_iIndexLayoutFile = -1;
	g_pMainFrame = this;
}

CMainFrame::~CMainFrame()
{
	if (m_pAnalysisDlg)
		delete m_pAnalysisDlg;
}

CMainFrame* CMainFrame::GetMainFrame()
{
	return g_pMainFrame;
}

void CMainFrame::InitPlacement()
{
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	if(m_iIndexLayoutFile<0)
	if(UTKernel_LayoutFile()->RegisterMainWnd(m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
	{
		if(UTKernel_LayoutFile()->ReadPositionMainWnd(m_iIndexLayoutFile,sysTime,iResHorz,iResVert,wndPlacement,iData))
			SetWindowPlacement(&wndPlacement);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CString string;

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	//if (!m_SettingDlg.Create(this, IDD_DIALOG_DYNAMIC_SETTING, CBRS_LEFT, IDD_DIALOG_DYNAMIC_SETTING))
	//{
	//	TRACE(_T("Failed to create dynamic setting"));
	//	return -1;
	//}

	m_pAnalysisDlg = new CAnalysisToolsDlg(this);
	
	if (!m_pAnalysisDlg->Create(CAnalysisToolsDlg::IDD, this))
		TRACE(_T("Failed to create analysis tools dialog"));

	m_pAnalysisDlg->ShowWindow(SW_HIDE);//integration SW_SHOW);

	g_pAnalysisDlg = m_pAnalysisDlg;//integration
	DemoHWDialog(this,g_uiUTEventMessage,g_dPieceThickness,g_dEncoderAxisLength,g_bAscanDisplayLastButOne);//integration

	if(IsWow64())
		string.Format(L"OEMPASector (x86, PID %d)",GetCurrentProcessId());
	else
		string.Format(L"OEMPASector (PID %d)",GetCurrentProcessId());
	SetWindowText(string);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG

bool g_bCorrectedViewEnable=false;
void AllocDisplay2(bool bAlloc);//integration
void AllocDisplay(bool bAlloc)//integration
{
	AllocDisplay2(bAlloc);//integration
	if(g_pDynamicCscope)
		g_pDynamicCscope->ResetDisplayProbe1();//integration
	for(int iGate=0;iGate<4;iGate++)
	{
		if(g_pHardwareCscope[iGate])
			g_pHardwareCscope[iGate]->ResetDisplayProbe1();//integration
	}
	EnterCriticalSection(&CRT_UPDATE_SETTING);

	if(!bAlloc)
	{
		DeleteAscanMemory(0);
		DeleteAscanMemory(1);
		DeleteCscanMemory(0);
		DeleteCscanMemory(1);
		LeaveCriticalSection(&CRT_UPDATE_SETTING);
		return;
	}

	DemoReset();

	if (GetSystem(System) == true)
	{
		g_dVelocity = System.dVelocity;
		g_dSamplingPeriod = System.dDigitizingPeriod;
		g_dEncoderAxisStep = System.dEncoderStep;
		g_dEncoderAxisLength = System.dEncoderLength;
	}
	
	g_bCorrectedViewEnable = true;
	if (GetInfo(0, InfoProbe1) == true)
	{
		g_uiCycleQuantity[0] = InfoProbe1.iCycleCount;
		g_uiPointQuantity[0] = InfoProbe1.PointQuantity;
		g_dMinimumAngle[0] = InfoProbe1.pdAngle[0];
		g_dStepAngle[0] = InfoProbe1.pdAngle[1] - InfoProbe1.pdAngle[0];

		if(InfoProbe1.iCycleCount<3)
			g_bCorrectedViewEnable = false;
		for(int iCycle=0;iCycle<InfoProbe1.iCycleCount;iCycle++)
		{
			if(iCycle && (InfoProbe1.pdAngle[iCycle]<=InfoProbe1.pdAngle[iCycle-1]))
				g_bCorrectedViewEnable = false;
			//if(InfoProbe1.pdX[iCycle]!=InfoProbe1.pdX[0])
			if(iCycle && (InfoProbe1.pdX[iCycle]<InfoProbe1.pdX[iCycle-1]))
				g_bCorrectedViewEnable = false;
		}
	}

	if (GetInfo(1, InfoProbe2) == true)
	{
		g_uiCycleQuantity[1] = InfoProbe2.iCycleCount;
		g_uiPointQuantity[1] = InfoProbe2.PointQuantity;
		g_dMinimumAngle[1] = InfoProbe2.pdAngle[0];
		g_dStepAngle[1] = InfoProbe2.pdAngle[1] - InfoProbe2.pdAngle[0];
	}

	AllocateAscanMemory(0);
	AllocateAscanMemory(1);
	AllocateCscanMemory(0);
	AllocateCscanMemory(1);

	if (g_pDynamicView)
		g_pDynamicView->m_bResetConfiguration = true;

	if (g_pDynamicNCView)
		g_pDynamicNCView->m_bResetConfiguration = true;

	if (g_pAscopeToolbar)
		g_pAscopeToolbar->FillCycleCombo();

	LeaveCriticalSection(&CRT_UPDATE_SETTING);

	::UpdateAxisLimitation(0);

	//integration
	//if (g_pAnalysisDlg)
	//	g_pAnalysisDlg->UpdateLimitation();

}

bool CMainFrame::SetCursor(double dX,double dZ)
{
	if(!m_pAnalysisDlg)
		return false;
	m_pAnalysisDlg->m_bCursor = true;
	m_pAnalysisDlg->m_dX = dX;
	m_pAnalysisDlg->m_dZ = dZ;
	return true;
}

bool CMainFrame::ResetCursor()
{
	if(!m_pAnalysisDlg)
		return false;
	m_pAnalysisDlg->m_bCursor = false;
	m_pAnalysisDlg->m_dX = 0.0;
	m_pAnalysisDlg->m_dZ = 0.0;
	return true;
}

LRESULT CMainFrame::OnEventMessage(WPARAM wParam,LPARAM lParam)//integration
{
	if(!wParam && !lParam)
	{
		AllocDisplay(false);//integration
		AllocDisplay(true);//integration
	}
	if(!wParam && lParam==1)
	{
		OnDynamicView();
	}
	return 0;
}

void CMainFrame::OnFileDemoLoad()
{
DemoHWDialog(this,g_uiUTEventMessage,g_dPieceThickness,g_dEncoderAxisLength,g_bAscanDisplayLastButOne);//integration
if(m_pAnalysisDlg)
	m_pAnalysisDlg->ShowWindow(SW_SHOW);//integration
return;//integration
	CString strFileFullPathName=_T("*.bin");
	_TCHAR BASED_CODE szFilter[] = _T("TestDemo Data Files (*.bin)|*.bin|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.bin"), strFileFullPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this, 0);
	
	if (dlg.DoModal() == IDOK)
	{
		CString FileName = dlg.GetPathName();
		
		_TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
		_wsplitpath_s(FileName, drive, dir, fname, ext);
		CString csDataFile, csTextFile;

		csDataFile = drive;
		csDataFile += dir;
		csDataFile += fname;
		csDataFile += _T(".bin");

		csTextFile = drive;
		csTextFile += dir;
		csTextFile += fname;
		csTextFile += _T(".txt");
		
		bool error = DemoLoad(0, csDataFile, csTextFile);

		if (error == false)
		{
			TRACE(_T("Failed to load the file"));
			return;
		}
		
		AllocDisplay(false);//integration
		AllocDisplay(true);//integration
#ifdef POUR_MEMOIRE
		EnterCriticalSection(&CRT_UPDATE_SETTING);

		DeleteAscanMemory(0);
		DeleteAscanMemory(1);

		DemoReset();

		if (GetSystem(System) == true)
		{
			g_dVelocity = System.dVelocity;
			g_dSamplingPeriod = System.dDigitizingPeriod;
			g_dEncoderAxisStep = System.dEncoderStep;
		}
			
		if (GetInfo(0, InfoProbe1) == true)
		{
			g_uiCycleQuantity[0] = InfoProbe1.iCycleCount;
			g_uiPointQuantity[0] = InfoProbe1.PointQuantity;
			g_dMinimumAngle[0] = InfoProbe1.pdAngle[0];
			g_dStepAngle[0] = InfoProbe1.pdAngle[1] - InfoProbe1.pdAngle[0];
		}

		if (GetInfo(1, InfoProbe2) == true)
		{
			g_uiCycleQuantity[1] = InfoProbe2.iCycleCount;
			g_uiPointQuantity[1] = InfoProbe2.PointQuantity;
			g_dMinimumAngle[1] = InfoProbe2.pdAngle[0];
			g_dStepAngle[1] = InfoProbe2.pdAngle[1] - InfoProbe2.pdAngle[0];
		}

		AllocateAscanMemory(0);
		AllocateAscanMemory(1);

		if (g_pDynamicView)
			g_pDynamicView->m_bResetConfiguration = true;

		if (g_pDynamicNCView)
			g_pDynamicNCView->m_bResetConfiguration = true;

		if (g_pAscopeToolbar)
			g_pAscopeToolbar->FillCycleCombo();

		LeaveCriticalSection(&CRT_UPDATE_SETTING);

		::UpdateAxisLimitation(0);

		if (m_pAnalysisDlg)
			m_pAnalysisDlg->UpdateLimitation();

#endif
		error = DemoRun(true, 1.0, NULL);

		if (error == false)
			TRACE(_T("Failed to start the acquisition"));
	}
}

void CMainFrame::OnFileLoad()
{
	CString strFileFullPathName=_T("*.wld");
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPASector");//integration
	strFileFullPathName += L"*.txt";
	_TCHAR BASED_CODE szFilter[] = _T("OEMPA Sector Data Files (*.wld)|*.wld|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.wld"), strFileFullPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this, 0);
	
	if (dlg.DoModal() == IDOK)
	{
		CString FileName = dlg.GetPathName();

		CDataFile cFile;
		cFile.FileName = FileName;

		if (cFile.Open(false))
		{
			BOOL bError = cFile.Read();

			if (bError == TRUE)
			{
				DeleteAscanAnalysisMemory(0, cFile.EncoderSize);
				DeleteAscanAnalysisMemory(1, cFile.EncoderSize);

				g_dPieceThicknessAnalysis = cFile.PieceThickness;
				g_dEncoderAxisLengthAnalysis = cFile.EncoderAxisLength;

				SystemAnalysis.dEncoderStep = cFile.EncoderAxisStep;
				SystemAnalysis.dVelocity = cFile.Velocity;
				SystemAnalysis.dDigitizingPeriod = cFile.SamplingTime;

				InfoProbe1Analysis.iCycleCount = cFile.CycleQuantity[0];
				InfoProbe1Analysis.PointQuantity = cFile.PointQuantity[0];
				
				InfoProbe2Analysis.iCycleCount = cFile.CycleQuantity[1];
				InfoProbe2Analysis.PointQuantity = cFile.PointQuantity[1];

				for (int i = 0 ; i < InfoProbe1Analysis.iCycleCount ; i++)
				{
					InfoProbe1Analysis.pdAngle[i] = cFile.Angle[0][i];
					InfoProbe1Analysis.pdX[i] = cFile.Emergence[0][i];
				}

				for (int i = 0 ; i < InfoProbe2Analysis.iCycleCount ; i++)
				{
					InfoProbe2Analysis.pdAngle[i] = cFile.Angle[1][i];
					InfoProbe2Analysis.pdX[i] = cFile.Emergence[1][i];
				}

				AllocateAscanAnalysisMemory(0, cFile.EncoderSize, cFile.SliceSize[0]);
				AllocateAscanAnalysisMemory(1, cFile.EncoderSize, cFile.SliceSize[1]);

				EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);
				
				for (int i = 0 ; i < (int)cFile.EncoderSize ; i++)
				{
					for (int j = 0 ; j < (int)cFile.SliceSize[0] ; j++)
						AscanAnalysisMemory1[i][j] = cFile.AscanMemory1[i][j];
				}

				LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);

				EnterCriticalSection(&CRT_ANALYSIS_MEMORY_2);

				for (int i = 0 ; i < (int)cFile.EncoderSize ; i++)
					memcpy(AscanAnalysisMemory2[i], cFile.AscanMemory2[i], cFile.SliceSize[1] * sizeof(char));

				LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_2);




				g_uiProbeAnalysis = 0;
				g_uiMode = 0;

				::UpdateAxisLimitation(1);

				if (m_pAnalysisDlg)
					m_pAnalysisDlg->UpdateLimitation();
				UTKernel_SetFolderDataFiles(L"OEMPASector",FileName.GetBuffer());//integration
			}

			cFile.Close();
		}
	}
}

void CMainFrame::OnFileSave()
{
	CString strFileFullPathName=_T("*.wld");
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPASector");//integration
	strFileFullPathName += L"*.txt";
	_TCHAR BASED_CODE szFilter[] = _T("OEMPA Sector Data Files (*.wld)|*.wld|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("*.wld"), strFileFullPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this, 0);
	
	if (dlg.DoModal() == IDOK)
	{
		CString FileName = dlg.GetPathName();

		CDataFile cFile;
		cFile.FileName = FileName;

		if (cFile.Open(true))
		{
			cFile.EncoderSize = ::GetEncoderPositionQuantity();
			cFile.SliceSize[0] = ::GetOneSliceSize(0);
			cFile.SliceSize[1] = ::GetOneSliceSize(1);
			cFile.CycleQuantity[0] = InfoProbe1.iCycleCount;
			cFile.CycleQuantity[1] = InfoProbe2.iCycleCount;
			cFile.PointQuantity[0] = InfoProbe1.PointQuantity;
			cFile.PointQuantity[1] = InfoProbe2.PointQuantity;
			cFile.Angle[0] = new double[InfoProbe1.iCycleCount];
			cFile.Emergence[0] = new double[InfoProbe1.iCycleCount];
			cFile.Angle[1] = new double[InfoProbe2.iCycleCount];
			cFile.Emergence[1] = new double[InfoProbe2.iCycleCount];

			for (int i = 0 ; i < InfoProbe1.iCycleCount ; i++)
			{
				cFile.Angle[0][i] = InfoProbe1.pdAngle[i];
				cFile.Emergence[0][i] = InfoProbe1.pdX[i];
			}

			for (int i = 0 ; i < InfoProbe2.iCycleCount ; i++)
			{
				cFile.Angle[1][i] = InfoProbe2.pdAngle[i];
				cFile.Emergence[1][i] = InfoProbe2.pdX[i];
			}

			cFile.EncoderAxisLength = g_dEncoderAxisLength;
			cFile.EncoderAxisStep = System.dEncoderStep;
			cFile.SamplingTime = System.dDigitizingPeriod;
			cFile.Velocity = System.dVelocity;
			cFile.PieceThickness = g_dPieceThickness;

			cFile.AllocateMemory();

			EnterCriticalSection(&CRT_MEMORY_1);

			for (int i = 0 ; i < (int)cFile.EncoderSize ; i++)
				memcpy(cFile.AscanMemory1[i], AscanMemory1[i], cFile.SliceSize[0] * sizeof(char));

			LeaveCriticalSection(&CRT_MEMORY_1);

			EnterCriticalSection(&CRT_MEMORY_2);
			
			for (int i = 0 ; i < (int)cFile.EncoderSize ; i++)
				memcpy(cFile.AscanMemory2[i], AscanMemory2[i], cFile.SliceSize[1] * sizeof(char));

			LeaveCriticalSection(&CRT_MEMORY_2);

			if (cFile.Write() == FALSE)
				TRACE(_T("CDataFile::Write() returned an error"));

			cFile.Close();
			UTKernel_SetFolderDataFiles(L"OEMPASector",FileName.GetBuffer());//integration
		}
	}
}

void CMainFrame::OnDynamicView()
{
	if (g_pDynamicView == NULL)
		theApp.m_pDynamicViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pDynamicView->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateDynamicView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pDynamicView == NULL) && AscanMemory1);//integration
	if(AscanMemory1 && g_bCorrectedViewEnable)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDynamicNCView()
{
	if (g_pDynamicNCView == NULL)
		theApp.m_pDynamicNCViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pDynamicNCView->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateDynamicNCView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pDynamicNCView == NULL) && AscanMemory1);//integration
	if(AscanMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDynamicCscopeView()
{
	if (g_pDynamicCscope == NULL)
		theApp.m_pDynamicCscopeViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pDynamicCscope->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateDynamicCscopeView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pDynamicCscope == NULL) && AscanMemory1);//integration
	if(AscanMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnViewsHardwareCscopeAmp0()
{
	OnViewsHardwareCscope(true,0);
}

void CMainFrame::OnViewsHardwareCscopeAmp1()
{
	OnViewsHardwareCscope(true,1);
}

void CMainFrame::OnViewsHardwareCscopeAmp2()
{
	OnViewsHardwareCscope(true,2);
}

void CMainFrame::OnViewsHardwareCscopeAmp3()
{
	OnViewsHardwareCscope(true,3);
}

void CMainFrame::OnViewsHardwareCscopeTof0()
{
	OnViewsHardwareCscope(false,0);
}

void CMainFrame::OnViewsHardwareCscopeTof1()
{
	OnViewsHardwareCscope(false,1);
}

void CMainFrame::OnViewsHardwareCscopeTof2()
{
	OnViewsHardwareCscope(false,2);
}

void CMainFrame::OnViewsHardwareCscopeTof3()
{
	OnViewsHardwareCscope(false,3);
}

int g_iGateId=0;
void CMainFrame::OnViewsHardwareCscope(bool bAmp,int iGate)
{
	if(!bAmp)
		iGate+= 4;
	g_iGateId = iGate;
	if (g_pHardwareCscope[iGate] == NULL)
		theApp.m_pHardwareCscopeViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pHardwareCscope[iGate]->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateViewsHardwareCscopeAmp0(CCmdUI *pCmdUI)
{
	if(CscanAmpMemory1[0] && System.bGateEnable[0])//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeAmp1(CCmdUI *pCmdUI)
{
	if(CscanAmpMemory1[1] && System.bGateEnable[1])//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeAmp2(CCmdUI *pCmdUI)
{
	if(CscanAmpMemory1[2] && System.bGateEnable[2])//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeAmp3(CCmdUI *pCmdUI)
{
	if(CscanAmpMemory1[3] && System.bGateEnable[3])//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeTof0(CCmdUI *pCmdUI)
{
	if(CscanTofMemory1[0] && System.bGateEnable[0] && System.bCscanTofEnable)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeTof1(CCmdUI *pCmdUI)
{
	if(CscanTofMemory1[1] && System.bGateEnable[1] && System.bCscanTofEnable)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeTof2(CCmdUI *pCmdUI)
{
	if(CscanTofMemory1[2] && System.bGateEnable[2] && System.bCscanTofEnable)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewsHardwareCscopeTof3(CCmdUI *pCmdUI)
{
	if(CscanTofMemory1[3] && System.bGateEnable[3] && System.bCscanTofEnable)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnAscopeView()
{
	if (g_pAscope == NULL)
		theApp.m_pAscopeViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pAscope->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateAscopeView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pAscope == NULL) && AscanAnalysisMemory1);//integration
	//if(AscanAnalysisMemory1)//integration
	if(AscanMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnBscopeView()
{
	if (g_pBscope == NULL)
		theApp.m_pBscopeViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pBscope->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateBscopeView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pBscope == NULL) && AscanAnalysisMemory1);//integration
	if(AscanAnalysisMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnCscopeView()
{
	if (g_pCscope == NULL)
		theApp.m_pCscopeViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pCscope->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateCscopeView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pCscope == NULL) && AscanAnalysisMemory1);//integration
	if(AscanAnalysisMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDscopeView()
{
	if (g_pDscope == NULL)
		theApp.m_pDscopeViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pDscope->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateDscopeView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pDscope == NULL) && AscanAnalysisMemory1);//integration
	if(AscanAnalysisMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDscopeCorrectedView()
{
	if (g_pDscopeCorrected == NULL)
		theApp.m_pDscopeCorrectedViewTemplate->OpenDocumentFile(NULL);
	else{
		CWnd *pWnd;
		pWnd = g_pDscopeCorrected->GetParent();
		if(pWnd)
			MDIActivate(pWnd);
	}
}

void CMainFrame::OnUpdateDscopeCorrectedView(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable((g_pDscopeCorrected == NULL) && AscanAnalysisMemory1);//integration
	if(AscanAnalysisMemory1)//integration
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMove(int x, int y)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CFrameWnd::OnMove(x, y);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemMainWnd(GetSafeHwnd(),iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionMainWnd(m_iIndexLayoutFile,iResHorz,iResVert,wndPlacement,0);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;

	CFrameWnd::OnSize(nType, cx, cy);

	//integration
	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemMainWnd(GetSafeHwnd(),iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionMainWnd(m_iIndexLayoutFile,iResHorz,iResVert,wndPlacement,0);
}

void CMainFrame::OnClose() 
{
	DemoDestroy();//integration
	DestroyWindow();
}
