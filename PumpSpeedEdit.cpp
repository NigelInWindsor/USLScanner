#include "stdafx.h"
#include "PumpSpeedEdit.h"


CPumpSpeedEdit::CPumpSpeedEdit()
	: CEdit()
{
}


CPumpSpeedEdit::~CPumpSpeedEdit()
{
}


BOOL CPumpSpeedEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CEdit::PreTranslateMessage(pMsg);
}

CMFCPumpSpeedEdit::CMFCPumpSpeedEdit()
	: CMFCToolBarEditBoxButton()
{
}

CMFCPumpSpeedEdit::~CMFCPumpSpeedEdit()
{}
