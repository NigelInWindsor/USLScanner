// EgismosBluetoothClient.cpp: implementation of the CEgismosBluetoothClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "EgismosBluetoothClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEgismosBluetoothClient::CEgismosBluetoothClient() : CRBluetoothClient()
{
	
}

CEgismosBluetoothClient::~CEgismosBluetoothClient()
{

}


void CEgismosBluetoothClient::InitialiseVariables(){
	SetRemoteName(L"bde spp dev");
//	SetRemoteName(L"Galaxy Tab A (2016)");
	NameToBthAddr();
}
