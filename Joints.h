#pragma once
class CJoints
{
public:
	CJoints();
	~CJoints();
	void Zero()
	{
		ZeroMemory(wName, sizeof wName);
		ZeroMemory(fCartesian, sizeof fCartesian);
		ZeroMemory(fAngle, sizeof fAngle);
		ZeroMemory(vRobotToolTip, sizeof vRobotToolTip);
		ZeroMemory(qRobotToolTip, sizeof qRobotToolTip);
		ZeroMemory(nSpare, sizeof nSpare);
	}

public:
	WCHAR	wName[100];
	union {
		struct {
			D3DXVECTOR3 vCartesian[2];
		};
		float fCartesian[6];
	};
	float fAngle[12];
	D3DXVECTOR3 vRobotToolTip[2];
	D3DXVECTOR4 qRobotToolTip[2];
	int	nSpare[20];
};

