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

// OEMPAWizardExampleDoc.cpp : implementation of the COEMPAWizardExampleDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OEMPAWizardExample.h"
#endif

#include "OEMPAWizardExampleDoc.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COEMPAWizardExampleDoc

IMPLEMENT_DYNCREATE(COEMPAWizardExampleDoc, CDocument)

BEGIN_MESSAGE_MAP(COEMPAWizardExampleDoc, CDocument)
END_MESSAGE_MAP()


// COEMPAWizardExampleDoc construction/destruction

COEMPAWizardExampleDoc::COEMPAWizardExampleDoc()
{
	// TODO: add one-time construction code here

}

COEMPAWizardExampleDoc::~COEMPAWizardExampleDoc()
{
}

BOOL COEMPAWizardExampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// COEMPAWizardExampleDoc serialization

void COEMPAWizardExampleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void COEMPAWizardExampleDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void COEMPAWizardExampleDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COEMPAWizardExampleDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COEMPAWizardExampleDoc diagnostics

#ifdef _DEBUG
void COEMPAWizardExampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COEMPAWizardExampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COEMPAWizardExampleDoc commands
BOOL COEMPAWizardExampleDoc::SaveDocument(LPCTSTR lpszPathName)
{
	CWnd *pWnd=AfxGetMainWnd();
	CMainFrame *pMainFrame;

	pMainFrame = dynamic_cast<CMainFrame*>(pWnd);
	if(pMainFrame && !pMainFrame->FileSave((wchar_t*)lpszPathName))
		return FALSE;
	return TRUE;
}

BOOL COEMPAWizardExampleDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CWnd *pWnd=AfxGetMainWnd();
	CMainFrame *pMainFrame;

	pMainFrame = dynamic_cast<CMainFrame*>(pWnd);
	if(pMainFrame)
		pMainFrame->FileSaveAs((wchar_t*)lpszPathName);
	return TRUE;
}

BOOL COEMPAWizardExampleDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	bool bSaveAs;
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{//Save As
		bSaveAs = true;
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);
 
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			//if(!pTemplate->GetDocString(newName,CDocTemplate::DocStringIndex::filterExt))
			newName = L"*.txt";
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);
 
			//ljr
			//// append the default suffix if there is one
			//CString strExt;
			//if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			//  !strExt.IsEmpty())
			//{
			//	ASSERT(strExt[0] == '.');
			//	newName += strExt;
			//}
			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
				!strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');

				int nSemi;                       //added
				if(nSemi = strExt.Find(';')){}    //added
				strExt = strExt.Left(nSemi);     //added

				//newName += strExt;
			}
		}
 
		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
		//first save the current file
		SaveDocument(m_strPathName);
	}else{//Save
		bSaveAs = false;
		//return TRUE;        // success
	}
 
	CWaitCursor wait;
 
	if ((bSaveAs && !OnSaveDocument(newName)) || (!bSaveAs && !SaveDocument(newName)))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				//ljr
				//TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				//DELETE_EXCEPTION(e);
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				e->Delete();     //modified
			}
			END_CATCH_ALL
		}
		return FALSE;
	}
 
	// reset the title and change the document name
	//if (bReplace)
	//	SetPathName(newName);
 
	return TRUE;        // success
}
