// EgismosBluetoothClient.h: interface for the CEgismosBluetoothClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EGISMOSBLUETOOTHCLIENT_H__8E6107ED_E706_4201_B8C6_E97A9F57DF63__INCLUDED_)
#define AFX_EGISMOSBLUETOOTHCLIENT_H__8E6107ED_E706_4201_B8C6_E97A9F57DF63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RBluetoothClient.h"

class CEgismosBluetoothClient : public CRBluetoothClient 
{
public:
	CEgismosBluetoothClient();
	virtual ~CEgismosBluetoothClient();

	virtual void InitialiseVariables();

};

#endif // !defined(AFX_EGISMOSBLUETOOTHCLIENT_H__8E6107ED_E706_4201_B8C6_E97A9F57DF63__INCLUDED_)
