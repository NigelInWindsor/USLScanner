// Stats.h: interface for the CStats class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATS_H__548ECFAB_BBCC_4F8F_83D5_64FE1D6C0E42__INCLUDED_)
#define AFX_STATS_H__548ECFAB_BBCC_4F8F_83D5_64FE1D6C0E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStats  
{
public:
	CStats();
	virtual ~CStats();

	virtual void Reset();
	virtual void Add(float val);
	virtual float Last(){return m_fLast;}
	virtual float Mean();
	virtual float STDev(){ return (float)sqrt(m_fVariance);}
	virtual float STDErr() { return STDev() / (float)sqrt(m_fCount); }
	virtual float Min(){ return m_fMin;}
	virtual float Max(){ return m_fMax;}
	virtual float Count(){ return m_fCount;}


protected:
	float m_fVal;
	float m_fLast;
	float m_fMean;
	float m_fM2;
	float m_fVariance;
	float m_fMin;
	float m_fMax;
	float m_fCount;
};

#endif // !defined(AFX_STATS_H__548ECFAB_BBCC_4F8F_83D5_64FE1D6C0E42__INCLUDED_)
