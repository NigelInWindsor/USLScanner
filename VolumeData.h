// VolumeData.h: interface for the CVolumeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLUMEDATA_H__1735ACC6_445B_4E85_82C6_E771A8EE9BED__INCLUDED_)
#define AFX_VOLUMEDATA_H__1735ACC6_445B_4E85_82C6_E771A8EE9BED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "USLFile.h"
#include "GenericScanner.h"

#define	GENERIC_VOLUME_VERSION_DOWTY	1
#define DOWTY_AEROSPACE_DOS				2

#pragma pack(push, 2)

struct	header_data	{
	float		f_stepsize;		// f == fast axis
	float		s_stepsize;		// s == slow axis
	WORD		f_increment;
	WORD		s_increment;
	int			f_scan_size;
	int			s_scan_size;
	WORD		samplesline;
	WORD		numberlines;
	int			gate_addrs;
/*	struct	pr15_data	pr15;
	struct	adc100_data	adc100;
	struct	timebase_data	tb;
	struct	dsp200_data	dsp;
	*/
	char	Dummy[782];
	WORD	scan_axes;
	int		x_start;
	int		y_start;
	int		z_start;
	int		xt_start;
	int		yt_start;
	int		r_start;
	
	WORD		data_size;
	WORD		interp_type[6];
	WORD		coord_l;						//number of coordinate points
	int		coord_offset;				//point at which the profile coords start
	int		interp_l;					//number of interpolation points
	int		interpolated_offset;		//point at which interpolated starts
	char	Dummy2[6];						//struct	dosdate_t test_date;
	char		Operator[20];
	WORD		no_str;
	WORD		data_type;
	WORD		no_gates;
	char		coord_name[14];
	char		text_str[2][80];
	WORD		max_line;
	WORD		orientation;
	WORD		ascan_time;
	char		Dummy3[24];					//struct	pr2050_data	pr20;
	WORD		interp_index;
	int		y_start_pos;
	int		x_start_pos;
	int		y_finish_pos;
	int		x_finish_pos;
	WORD		bscan_type;
	WORD		eol_mode;
	char		spare[148];
	
};

struct	bmp_data {
	char id[2];
	int	filesize;
	WORD	reserved[2];
	int	headersize;
	int	infoSize;
	int	width;
	int	depth;
	WORD 	biPlanes;
	WORD	bits;
	int 	biCompression;
	int	biSizeImage;
	int 	biXPelsPerMeter;	
	int	biYPelsPerMeter;
	int	biClrUsed;
	int	biClrImportant;
};

#pragma pack(pop)



class CVolumeData  
{
public:
	int m_nSamplePeriod;
	void Rename(LPCTSTR lpszFileName);
	void ChangeVelocityDepth(int nVelocity);
	int m_nMaterialVelocity;
	void SaveScanData(int nScanLine);
	bool OpenForWriting(CString FilePath);
	bool m_bCollectAscans;
	void CalculateNumberOfImages();
	void CheckAllBScanVariables();
	float m_fDepth;
	float m_fScanLength;
	void BuildBscanArray(int nWhich);
	int CheckFileType(CString FilePath);
	bool RetrieveDowtyAeroSpaceDos(CString FilePath);
	bool RetrieveDowtyAeroSpaceWindows(CString FilePath);
	bool Retrieve(CString FilePath);
	CUSLFile m_File;
	void Zero();

	bool m_bDataInMemory;
	int m_ullDataOffset;
	int m_ullDataSize;
	int m_nNumberBscans;
	int m_nAscanLength;
	int m_nNumberLines;
	int m_nSamplesLine;
	int	m_nFileType;
	int	m_nScanAxes;
	char* m_pData;
	char* m_pBscanArray;
	struct	TimeSlotData	m_TS[8];
	struct	UtGlobalData	m_Global;
	CString m_TestDate;
	CString	m_FileName;
	CString	m_FilePath;
	CoordStructGeneric  m_CpStart;
	CoordStructGeneric m_CpFinish;
	double	m_dFastScanSize;
	double	m_dSlowScanSize;
	double	m_dFastIncrement;
	double	m_dSlowIncrement;
	int	m_nSentence;
	int	m_nNumberImages;
	int m_nNumberAmpImages;
	int m_nNumberTimeImages;
	bool m_bModifiedFlag;


	CVolumeData();
	virtual ~CVolumeData();

};

#endif // !defined(AFX_VOLUMEDATA_H__1735ACC6_445B_4E85_82C6_E771A8EE9BED__INCLUDED_)
