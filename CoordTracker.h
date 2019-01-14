#pragma once
//Abstract base class for a tracking device, e.g. optical tracker.
class CCoordTracker
{
public:
	CCoordTracker();
	~CCoordTracker();

	virtual bool GetSurfacePos(CCoord * pCpSurface) = 0;
	virtual bool GetScanLine(int* val) = 0;
};

