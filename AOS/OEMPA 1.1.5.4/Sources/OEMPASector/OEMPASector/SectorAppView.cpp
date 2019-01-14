
// SectorAppView.cpp : implementation of the CSectorAppView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SectorApp.h"
#endif

#include "SectorAppDoc.h"
#include "SectorAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSectorAppView

IMPLEMENT_DYNCREATE(CSectorAppView, CView)

BEGIN_MESSAGE_MAP(CSectorAppView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSectorAppView construction/destruction

CSectorAppView::CSectorAppView()
{
	// TODO: add construction code here

}

CSectorAppView::~CSectorAppView()
{
}

BOOL CSectorAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSectorAppView drawing

void CSectorAppView::OnDraw(CDC* /*pDC*/)
{
	CSectorAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CSectorAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSectorAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}


// CSectorAppView diagnostics

#ifdef _DEBUG
void CSectorAppView::AssertValid() const
{
	CView::AssertValid();
}

void CSectorAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSectorAppDoc* CSectorAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSectorAppDoc)));
	return (CSectorAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CSectorAppView message handlers
