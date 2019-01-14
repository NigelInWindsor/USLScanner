#pragma once
class CPumpSpeedEdit : public CEdit
{
public:
	CPumpSpeedEdit();
	~CPumpSpeedEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

class CMFCPumpSpeedEdit : public CMFCToolBarEditBoxButton
{
public:
	CMFCPumpSpeedEdit();
	~CMFCPumpSpeedEdit();
};

