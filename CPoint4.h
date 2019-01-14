#pragma once
class CPoint4
{
public:
	CPoint4();
	CPoint4(float x, float y, float z, float w);
	~CPoint4();

	float           operator[](int idx) const { return mData[idx]; }
	const float*    Data() const { return mData; }

private:
	float           mData[4];
};

