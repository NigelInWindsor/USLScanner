// RMessageQueue.h: interface for the CRMessageQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMESSAGEQUEUE_H__B427F737_1AC4_4959_8F16_6B0F32724624__INCLUDED_)
#define AFX_RMESSAGEQUEUE_H__B427F737_1AC4_4959_8F16_6B0F32724624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConcurrentQueue.h"
#include "RMessage.h"

class CRMessageQueue : public CConcurrentQueue<CRMessage*,10>  
{
public:
	CRMessageQueue();
	virtual ~CRMessageQueue();

};

#endif // !defined(AFX_RMESSAGEQUEUE_H__B427F737_1AC4_4959_8F16_6B0F32724624__INCLUDED_)
