// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// OEMPAWizardExampleView.cpp : implementation of the COEMPAWizardExampleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OEMPAWizardExample.h"
#endif
#include "CustomizedDriverAPI.h"
#include "OEMPAWizardExampleDoc.h"
#include "OEMPAWizardExampleView.h"
#include "HWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool WizardDelete();
bool WizardNew();
bool DeviceOEMPADelete();
bool DeviceOEMPANew(bool bHW);
const wchar_t *GetOEMPAWizardExamplePath();
bool ReadWizardWriteTemplate(bool bOverwrite);
bool ReadTemplateWriteWizard();
structCorrectionOEMPA* WINAPI CallbackCustomizedWizardAPI(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedWizardAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
bool OEMPA_ReadWizardWriteFile(const wchar_t *pFileName);
void EditFile(const wchar_t *pFile,bool bCloseWaiting);
bool DeviceOEMPADelete();
bool DeviceOEMPANew(bool bHW);

extern CHWDeviceOEMPA *g_pHWDeviceOEMPA;
extern wchar_t g_wcFolderData[MAX_PATH];
extern wchar_t g_wcFolderKernel[MAX_PATH];
extern wchar_t g_wcFolderEmuMon[MAX_PATH];

// COEMPAWizardExampleView

CHWDlg *COEMPAWizardExampleView::g_pDlg=NULL;

IMPLEMENT_DYNCREATE(COEMPAWizardExampleView, CEditView)

BEGIN_MESSAGE_MAP(COEMPAWizardExampleView, CEditView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_HW, &COEMPAWizardExampleView::OnButtonHw)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_HW, &COEMPAWizardExampleView::OnUpdateButtonHw)
	ON_COMMAND(ID_BUTTON_FLASH, &COEMPAWizardExampleView::OnButtonFlash)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_FLASH, &COEMPAWizardExampleView::OnUpdateButtonFlash)
END_MESSAGE_MAP()

// COEMPAWizardExampleView construction/destruction

extern structCorrectionOEMPA *g_pCorrection;
COEMPAWizardExampleView::COEMPAWizardExampleView()
{
	// TODO: add construction code here
	g_pCorrection = new structCorrectionOEMPA;
	if(g_pCorrection)
		OEMPA_New(*g_pCorrection);
	OEMPA_SetCallbackCustomizedWizardAPI(CallbackCustomizedWizardAPI);
	WizardNew();
	DeviceOEMPANew(false);
	ReadWizardWriteTemplate(false);
	g_pDlg = NULL;
}

COEMPAWizardExampleView::~COEMPAWizardExampleView()
{
	WizardDelete();
	DeviceOEMPADelete();
	if(g_pCorrection)
		delete [] g_pCorrection;
	g_pCorrection = NULL;
}

BOOL COEMPAWizardExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CEditView::PreCreateWindow(cs);
}

// COEMPAWizardExampleView drawing

void COEMPAWizardExampleView::OnDraw(CDC* /*pDC*/)
{
	COEMPAWizardExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void COEMPAWizardExampleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COEMPAWizardExampleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COEMPAWizardExampleView diagnostics

#ifdef _DEBUG
void COEMPAWizardExampleView::AssertValid() const
{
	CEditView::AssertValid();
}

void COEMPAWizardExampleView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

COEMPAWizardExampleDoc* COEMPAWizardExampleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COEMPAWizardExampleDoc)));
	return (COEMPAWizardExampleDoc*)m_pDocument;
}
#endif //_DEBUG


// COEMPAWizardExampleView message handlers

int COEMPAWizardExampleView::OpenTemplate(CString strFile)
{
	CString string,strText;
	CEdit& theEdit = GetEditCtrl();
	int iLine=0;

	strText = L"";
	try{
		CStdioFile file(strFile,CFile::typeText | CFile::modeRead);
		while(file.ReadString(string))
		{
			strText = strText + string;
			strText = strText + L"\r\n";
			iLine++;
			if(iLine>1000)
			{
				iLine = 0;//error
				break;
			}
		}
		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	theEdit.SetWindowText(strText);
	return iLine;
}

bool COEMPAWizardExampleView::SaveTemplate(CString strFileName)
{
	CString string,strText;
	CEdit& theEdit = GetEditCtrl();
	int iLine=0,iFindStart=0,iFindStop=-1,iLength;

	iLine=0;iFindStart=-1;iFindStop=-1;
	theEdit.GetWindowText(strText);
	iLength = strText.GetLength();
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
		do
		{
			if(iFindStart==-1)
			{
				iFindStart = 0;
			}else{
				iFindStart = iFindStop+2;
			}
			if(iFindStart==-1)
				break;
			if(iFindStart>=iLength)
				break;
			iFindStop = strText.Find(L"\r\n",iFindStart);
			if(iFindStop<0)
				iFindStop = strText.GetLength();
			string = strText.Mid(iFindStart,iFindStop-iFindStart+1);
			file.WriteString(string);
			file.WriteString(L"\n");
			iLine++;
			if(iLine>1000)
				break;
		}while(1);
		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if(iLine)
		return true;
	return false;
}

bool COEMPAWizardExampleView::EditTemplate()
{
	CString strFileName;
	int iCount=0;
	COEMPAWizardExampleDoc *pDoc;

	strFileName = g_wcFolderKernel;
	strFileName += GetOEMPAWizardExamplePath();
	pDoc = GetDocument();
	if(pDoc)
		pDoc->SetPathName(strFileName);
test:
	if(!OpenTemplate(strFileName))
	{
		//file is empty
		DeleteFile(strFileName);
		ReadWizardWriteTemplate(true);
		iCount++;
		if(iCount>1)
			return false;
		goto test;
	}
	return true;
}

void COEMPAWizardExampleView::OnButtonHw()
{
	if(!g_pDlg)
		g_pDlg = new CHWDlg(this);
	else
		g_pDlg->ShowWindow(SW_SHOW);
}

void COEMPAWizardExampleView::OnButtonFlash()
{
	CString strFileName;

	strFileName = g_wcFolderEmuMon;
	strFileName += L"WizardTemplateFlashFile.txt";
	if(g_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
		return;
	if(AfxMessageBox(L"The device will be reallocated?",MB_OKCANCEL)==IDCANCEL)
		return;
	DeviceOEMPADelete();
	EditFile(strFileName,true);
	DeviceOEMPANew(false);
}

void COEMPAWizardExampleView::OnUpdateButtonHw(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void COEMPAWizardExampleView::OnUpdateButtonFlash(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_pHWDeviceOEMPA?(g_pHWDeviceOEMPA->GetSWDevice()->IsConnected()?FALSE:TRUE):FALSE);
}
