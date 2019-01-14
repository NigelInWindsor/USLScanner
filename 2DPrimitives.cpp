// 2DPrimitives.cpp: implementation of the C2DPrimitives class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "2DPrimitives.h"
#include "D2d1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2DPrimitives::C2DPrimitives()
{

}

C2DPrimitives::~C2DPrimitives()
{

}


void C2DPrimitives::CreateGradientBackGround()
{
	ID2D1GradientStopCollection *pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::ForestGreen, 1);
	gradientStops[1].position = 1.0f;
	// Create the ID2D1GradientStopCollection from a previously
	// declared array of D2D1_GRADIENT_STOP structs.
	hr = m_pRenderTarget->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
		);
}
