#include <math.h>
#include <windows.h>
#include "mex.h"

#define _TEST_STACK_
//The following preprocessor "_DEBUG" should be defined if and only if "UTStubMatlab.dll" is compiled with "_DEBUG".
//#define _DEBUG
#include "..\UTStubMatlab\UTStubMatlab.h"

//Note   MATLAB automatically frees up memory allocated with the MX Matrix Library allocation routines (mxCalloc, mxMalloc, mxRealloc) upon exiting your MEX-file. If you do not want to free this memory, use the API function mexMakeMemoryPersistent.
//Event Data
//addevent	Add event to timeseries object
//delevent	Remove tsdata.event objects from timeseries object
//gettsafteratevent	New timeseries object with samples occurring at or after event
//gettsafterevent	New timeseries object with samples occurring after event
//gettsatevent	New timeseries object with samples occurring at event
//gettsbeforeatevent	New timeseries object with samples occurring before or at event
//gettsbeforeevent	New timeseries object with samples occurring before event
//gettsbetweenevents	New timeseries object with samples occurring between events

#define MAX_PATH			260
#define DLL_COUNT			1

__declspec(dllexport) VS_FIXEDFILEINFO g_VS_FIXEDFILEINFO={	0xFEEF04BD,//DWORD dwSignature;
												0,//DWORD dwStrucVersion;
												0x00010001,//DWORD dwFileVersionMS;
												0x00050004,//DWORD dwFileVersionLS;
												0x00010001,//DWORD dwProductVersionMS;
												0x00050004,//DWORD dwProductVersionLS;
												VS_FF_DEBUG | VS_FF_PRERELEASE | VS_FF_PATCHED | VS_FF_PRIVATEBUILD | VS_FF_INFOINFERRED | VS_FF_SPECIALBUILD,//DWORD dwFileFlagsMask;
#ifdef _DEBUG
												VS_FF_DEBUG,//DWORD dwFileFlags;
#else _DEBUG
#ifdef _PRERELEASE
												VS_FF_PRERELEASE,//DWORD dwFileFlags;
#else _PRERELEASE
												0,//DWORD dwFileFlags;
#endif _PRERELEASE
#endif _DEBUG
												VOS__WINDOWS32,//DWORD dwFileOS;
												VFT_DLL,//DWORD dwFileType;
												0,//DWORD dwFileSubtype;
												0,//DWORD dwFileDateMS;
												0//DWORD dwFileDateLS;
											};
VS_FIXEDFILEINFO *g_paVS_FIXEDFILEINFO[DLL_COUNT]={NULL};

char g_pThisFileVersion[MAX_PATH]="";
char g_pThisProductVersion[MAX_PATH]="";
char g_pFileVersion[DLL_COUNT][MAX_PATH]={""};
char g_pProductVersion[DLL_COUNT][MAX_PATH]={""};
static HMODULE g_hModule[DLL_COUNT]={0};
DWORD g_dwProcessId=0;
CStubMatlab* g_pmxMain=NULL;

void ManageLibrary(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[] , const wchar_t *wcFunc, unsigned long pdwData3[DLL_COUNT]);

/* Input Arguments */

#define	ID_IN					prhs[0]
#define	FUNC_IN					prhs[1]
#define	STRING_OR_MATRIX_IN		prhs[2]
#define	NAME_IN					prhs[3]
#define	NAME5_IN				prhs[4]


/* Output Arguments */

#define	VALUE_OUT	plhs[0]

/* get_characteristics figures out the size, and category 
   of the input array_ptr, and then displays all this information. */ 
void get_characteristics(const mxArray *array_ptr)
{
	const char    *class_name;
	const mwSize  *dims;
	char          *shape_string;
	char          *temp_string;
	mwSize        c;
	mwSize        number_of_dimensions; 
	size_t        length_of_shape_string;

	/* Display a bottom banner. */
	mexPrintf("------------------------------------------------\n");

	/* Display the mxArray's Dimensions; for example, 5x7x3.  
	If the mxArray's Dimensions are too long to fit, then just
	display the number of dimensions; for example, 12-D. */ 
	number_of_dimensions = mxGetNumberOfDimensions(array_ptr);
	dims = mxGetDimensions(array_ptr);

	/* alloc memory for shape_string w.r.t thrice the number of dimensions */
	/* (so that we can also add the 'x')                                   */
	shape_string=(char *)mxCalloc(number_of_dimensions*3,sizeof(char));
	shape_string[0]='\0';
	temp_string=(char *)mxCalloc(64, sizeof(char));

	for (c=0; c<number_of_dimensions; c++) {
	sprintf(temp_string, "%"FMT_SIZE_T"dx", dims[c]);
	strcat(shape_string, temp_string);
	}

	length_of_shape_string = strlen(shape_string);
	/* replace the last 'x' with a space */
	shape_string[length_of_shape_string-1]='\0';
	if (length_of_shape_string > 16) {
	sprintf(shape_string, "%"FMT_SIZE_T"u-D", number_of_dimensions); 
	}
	mexPrintf("Dimensions: %s\n", shape_string);

	/* Display the mxArray's class (category). */
	class_name = mxGetClassName(array_ptr);
	mexPrintf("Class Name: %s%s\n", class_name,
	mxIsSparse(array_ptr) ? " (sparse)" : "");

	/* Display a bottom banner. */
	mexPrintf("------------------------------------------------\n");

	/* free up memory for shape_string */
	mxFree(shape_string);
}

bool IsInteger(mxClassID  category)
{
	switch (category)
	{
	case mxDOUBLE_CLASS:
	case mxINT8_CLASS:
	case mxUINT8_CLASS:
	case mxINT16_CLASS:
	case mxUINT16_CLASS:
	case mxINT32_CLASS:
	case mxUINT32_CLASS:
	case mxINT64_CLASS:
	case mxUINT64_CLASS:
		break;
	default: return false;
	}
	return true;
}

int iround(double x)
{
	int nx = (int)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}

void mexEnter();
void mexExit();

mxArray* mxCreateDummy()
{
	mwSize nd = 0;
    mwSize dims[] = { 0 };

	return mxCreateNumericArray(nd, dims, mxINT8_CLASS, mxREAL);
}

mxArray* mxCreateString2(const wchar_t *pIn)
{
	char pAux[MAX_PATH];
	mxArray *out;
	DWORD dwConvert=(DWORD)WideCharToMultiByte(CP_ACP, 0, pIn, -1, pAux, MAX_PATH, NULL, NULL);

	if(!dwConvert)
		return NULL;
	out = mxCreateString(pAux);
	if(!out)
		return NULL;
	return out;
}

mxArray* mxCreateByteResult0(BYTE byData2)
{
	mxArray *out;
	BYTE *pbyData;
	mwSize nd = 2;
    mwSize dims[] = { 1, 1 };

	out = mxCreateNumericArray(nd, dims, mxUINT8_CLASS, mxREAL);
	if(!out)
		return NULL;
	pbyData = (BYTE*)mxGetData(out);
	if(pbyData)
		*pbyData = byData2;
	return out;
}

mxArray* mxCreateByteResult1(int iSize,BYTE *pbyData2)
{
	mxArray *out;
	BYTE *pbyData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxUINT8_CLASS, mxREAL);
	if(!out)
		return NULL;
	pbyData = (BYTE*)mxGetData(out);
	if(pbyData && pbyData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pbyData[iIndex] = pbyData2[iIndex];
	}
	return out;
}

mxArray* mxCreateAscanResult2(int iRow, int iCol, int iByteSize, bool bSigned, BYTE** ppbyData2)
{
	mxArray *out;
	mwSize nd = 2;
    mwSize dims[] = { iRow, iCol };

	if(ppbyData2)
		*ppbyData2 = NULL;
	switch(iByteSize)
	{
	case 1:
		if(bSigned)
			out = mxCreateNumericArray(nd, dims, mxINT8_CLASS, mxREAL);
		else
			out = mxCreateNumericArray(nd, dims, mxUINT8_CLASS, mxREAL);
		break;
	case 2:
		if(bSigned)
			out = mxCreateNumericArray(nd, dims, mxINT16_CLASS, mxREAL);
		else
			out = mxCreateNumericArray(nd, dims, mxUINT16_CLASS, mxREAL);
		break;
	default:
		return NULL;
	}
	if(!out)
		return NULL;
	if(ppbyData2)
		*ppbyData2 = (BYTE*)mxGetData(out);
	return out;
}

mxArray* mxCreateCharResult1(int iSize,char *pbyData2)
{
	mxArray *out;
	char *pbyData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxINT8_CLASS, mxREAL);
	if(!out)
		return NULL;
	pbyData = (char*)mxGetData(out);
	if(pbyData && pbyData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pbyData[iIndex] = pbyData2[iIndex];
	}
	return out;
}

mxArray* mxCreateCharResult2(int iRow,int iCol,char *pData2)
{
	mxArray *out;
	char *pData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { iRow, iCol };

	out = mxCreateNumericArray(nd, dims, mxINT8_CLASS, mxREAL);
	if(!out)
		return NULL;
	pData = (char*)mxGetData(out);
	if(pData && pData2)
	{
		for(iIndex=0;iIndex<iRow*iCol;iIndex++)
			pData[iIndex] = pData2[iIndex];
	}
	return out;
}

mxArray* mxCreateWordResult1(int iSize,WORD *pwData2)
{
	mxArray *out;
	WORD *pwData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxUINT16_CLASS, mxREAL);
	if(!out)
		return NULL;
	pwData = (WORD*)mxGetData(out);
	if(pwData && pwData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pwData[iIndex] = pwData2[iIndex];
	}
	return out;
}

mxArray* mxCreateShortResult1(int iSize,short *psData2)
{
	mxArray *out;
	short *psData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxINT16_CLASS, mxREAL);
	if(!out)
		return NULL;
	psData = (short*)mxGetData(out);
	if(psData && psData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			psData[iIndex] = psData2[iIndex];
	}
	return out;
}

mxArray* mxCreateShortResult2(int iRow,int iCol,short *psData2)
{
	mxArray *out;
	short *psData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { iRow, iCol };

	out = mxCreateNumericArray(nd, dims, mxINT16_CLASS, mxREAL);
	if(!out)
		return NULL;
	psData = (short*)mxGetData(out);
	if(psData && psData2)
	{
		for(iIndex=0;iIndex<iRow*iCol;iIndex++)
			psData[iIndex] = psData2[iIndex];
	}
	return out;
}

mxArray* mxCreateLongResult1(int iSize,long *pdwData2)
{
	mxArray *out;
	long *pdwData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxINT32_CLASS, mxREAL);
	if(!out)
		return NULL;
	pdwData = (long*)mxGetData(out);
	if(pdwData && pdwData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pdwData[iIndex] = pdwData2[iIndex];
	}
	return out;
}

mxArray* mxCreateLongResult2(int iRow,int iCol,long *pdwData2)
{
	mxArray *out;
	long *pdwData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { iRow, iCol };

	out = mxCreateNumericArray(nd, dims, mxINT32_CLASS, mxREAL);
	if(!out)
		return NULL;
	pdwData = (long*)mxGetData(out);
	if(pdwData && pdwData2)
	{
		for(iIndex=0;iIndex<iRow*iCol;iIndex++)
			pdwData[iIndex] = pdwData2[iIndex];
	}
	return out;
}

mxArray* mxCreateLongLongResult1(int iSize,long long *pdwData2)
{
	mxArray *out;
	long long *pdwData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxINT64_CLASS, mxREAL);
	if(!out)
		return NULL;
	pdwData = (long long*)mxGetData(out);
	if(pdwData && pdwData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pdwData[iIndex] = pdwData2[iIndex];
	}
	return out;
}

mxArray* mxCreateLongLongResult2(int iRow,int iCol,long long *pdwData2)
{
	mxArray *out;
	long long *pdwData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { iRow,iCol };

	out = mxCreateNumericArray(nd, dims, mxINT64_CLASS, mxREAL);
	if(!out)
		return NULL;
	pdwData = (long long*)mxGetData(out);
	if(pdwData && pdwData2)
	{
		for(iIndex=0;iIndex<iRow*iCol;iIndex++)
			pdwData[iIndex] = pdwData2[iIndex];
	}
	return out;
}

mxArray* mxCreateDWORDResult1(int iSize,unsigned long *pdwData2)
{
	mxArray *out;
	unsigned long *pdwData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxUINT32_CLASS, mxREAL);
	if(!out)
		return NULL;
	pdwData = (unsigned long*)mxGetData(out);
	if(pdwData && pdwData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pdwData[iIndex] = pdwData2[iIndex];
	}
	return out;
}

mxArray* mxCreateIntegerResult2d(int iRow,int iCol,double *pdData2)
{
	mxArray *out;
	DWORD *pdwData;
	int i;
	mwSize nd = 2;
    mwSize dims[] = { iRow,iCol };

	out = mxCreateNumericArray(nd,dims,mxINT32_CLASS,mxREAL);
	if(!out)
	{
		mxDestroyArray(out);
		return NULL;
	}
	pdwData = (DWORD*)mxGetData(out);
	if(pdwData && pdData2)
	{
		for(i=0;i<iRow*iCol;i++)
			pdwData[i] = (DWORD)iround(pdData2[i]);
	}
	return out;
}

mxArray* mxCreateIntegerResult2(int iRow,int iCol,int *piData2)
{
	mxArray *out;
	DWORD *pdwData;
	int i;
	mwSize nd = 2;
    mwSize dims[] = { iRow,iCol };

	out = mxCreateNumericArray(nd,dims,mxINT32_CLASS,mxREAL);
	if(!out)
	{
		mxDestroyArray(out);
		return NULL;
	}
	pdwData = (DWORD*)mxGetData(out);
	if(pdwData && piData2)
	{
		for(i=0;i<iRow*iCol;i++)
			pdwData[i] = (DWORD)piData2[i];
	}
	return out;
}

mxArray* mxCreateDoubleResult0(double dData)
{
	mxArray *out;
	double *pdData;
	mwSize nd = 1;
    mwSize dims[] = { 1 };
    size_t rows=1, cols=1;

	out = mxCreateDoubleMatrix((mwSize)rows, (mwSize)cols, mxREAL);
	if(!out)
		return NULL;
	if(!mxIsDouble(out))
	{
		mxDestroyArray(out);
		return NULL;
	}
	pdData = mxGetPr(out);
	if(pdData)
		*pdData = dData;
	return out;
}

mxArray* mxCreateDoubleResult1(int iSize,double *pData2)
{
	mxArray *out;
	double *pData;
	int iIndex;
	mwSize nd = 2;
    mwSize dims[] = { 1, iSize };

	out = mxCreateNumericArray(nd, dims, mxDOUBLE_CLASS, mxREAL);
	if(!out)
		return NULL;
	pData = mxGetPr(out);
	if(pData && pData2)
	{
		for(iIndex=0;iIndex<iSize;iIndex++)
			pData[iIndex] = pData2[iIndex];
	}
	return out;
}

mxArray* mxCreateDoubleResult2(int iRow,int iCol,double *pdData2)
{
	mxArray *out;
	double *pdData;
	mwSize nd = 2;
    mwSize dims[2] = { iRow,iCol };

	dims[0] = iRow;
	dims[1] = iCol;
	out = mxCreateNumericArray(nd, dims, mxDOUBLE_CLASS, mxREAL);
	if(!out)
		return NULL;
	if(!mxIsDouble(out))
	{
		mxDestroyArray(out);
		return NULL;
	}
	pdData = mxGetPr(out);
	if(pdData && pdData2)
	{
		memcpy(pdData,pdData2,sizeof(double)*iRow*iCol);
	}
	return out;
}

void mxGetDimension(const mxArray *pm, structDimension *dim)
{
	const mwSize *dims;
	if(!dim)
		return;
	dim->number_of_dimensions = (int)mxGetNumberOfDimensions(pm);
	dims = mxGetDimensions(pm);
	dim->dims[0] = dims[0];
	dim->dims[1] = dims[1];
}
void *mxMalloc2(size_t size)
{
	return mxMalloc(size);
}
void *mxCalloc2(size_t n, size_t size)
{
	return mxCalloc(n, size);
}


//Local variables of functions "mexFunction"
wchar_t wcFunc[MAX_PATH];
wchar_t wcString[MAX_PATH];
wchar_t wcName[MAX_PATH];
wchar_t wcName5[MAX_PATH];
wchar_t wcAux[MAX_PATH];
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	mxClassID  category;
	int number_of_dimensions;
	const mwSize *dims;
	char pAux[MAX_PATH];
	mwSize c,size;
	double *pdData,*pdMatrix;
	void *pMatrix;
	int iMatrixSize;
	int iId,iId2;
	int iRet;
	DWORD dwConvert;
	int iCount;
	DWORD dwAux;
	unsigned long pdwData3[DLL_COUNT];
	int iError=0;
	int iDll;
	int iRowSize,iColSize;
#ifdef _TEST_STACK_
BYTE g_pBuffer1234[g_iBufferSize*1000];
if(g_pmxMain && g_pmxMain->g_pmxStackTest)
(*g_pmxMain->g_pmxStackTest)(g_pBuffer1234,g_iBufferSize*1000);
#endif _TEST_STACK_

	mexEnter();
	mexAtExit(mexExit);
	if(!g_dwProcessId)
		g_dwProcessId = GetCurrentProcessId();
	/* Check for proper number of arguments */
	pMatrix = NULL;
	pdMatrix = NULL;
	iMatrixSize = 0;
	wcString[0] = 0;
	wcName[0] = 0;
	wcName5[0] = 0;

	if((nrhs < 2) || (5 < nrhs))
		mexErrMsgTxt("bad input arguments count."); 

	if (mxIsSparse(ID_IN)) 
		mexErrMsgTxt("ID_IN is a spare matrix."); 
	category = mxGetClassID(ID_IN);
	if(!IsInteger(category))
		mexErrMsgTxt("ID_IN is not an integer 1."); 
	number_of_dimensions = (int)mxGetNumberOfDimensions(ID_IN);
	dims = mxGetDimensions(ID_IN);
	size = 1;
	for (c=0; c<number_of_dimensions; c++)
		size *= dims[c];
	if((size!=1) && (size!=2))
	{
		mexPrintf("number_of_dimensions: %d (%d x %d)\n", number_of_dimensions, dims[0], dims[1]);
		mexErrMsgTxt("ID_IN is not an integer 2."); 
	}
	if(!mxIsDouble(ID_IN))
		mexErrMsgTxt("ID_IN should be a double."); 
	pdData = mxGetPr(ID_IN);
	iId = (int)(pdData[0]);
	if(size==2)
	{
		iId2 = (int)pdData[1];
		if(iId==iId2)
			mexErrMsgTxt("Master and Slave Devices must have different ID");
	}

	if (mxIsSparse(FUNC_IN)) 
		mexErrMsgTxt("FUNC_IN is a spare matrix.");
	category = mxGetClassID(FUNC_IN);
	if(category!=mxCHAR_CLASS)
		mexErrMsgTxt("FUNC_IN is not a string 1.");
	dims = mxGetDimensions(FUNC_IN);
	if((number_of_dimensions!=2) || (dims[0]!=1))
		mexErrMsgTxt("FUNC_IN is not a string 2.");
	if (mxGetString(FUNC_IN, pAux, MAX_PATH) != 0)
		mexErrMsgTxt("Could not convert string data.");
	if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pAux, -1, (LPWSTR)wcFunc, MAX_PATH))
		mexErrMsgTxt("Could not convert to widechar function name string.");

	//FIRST PARAMETER = integer
	//SECOND PARAMETER = function name
	//THIRD PARAMETER (depend of the function name) = matrix (see "pdMatrix") OR string (see "wcString")
	//FOURTH PARAMETER = string (see "wcName")
	//FIFTH PARAMETER = string (see "wcName")
	if(nrhs == 5)
	{
		if (mxIsSparse(NAME5_IN))
			mexErrMsgTxt("NAME_IN is a spare matrix.");
		category = mxGetClassID(NAME5_IN);
		if(category!=mxCHAR_CLASS)
			mexErrMsgTxt("NAME5_IN is not a string 1.");
		number_of_dimensions = (int)mxGetNumberOfDimensions(NAME5_IN);
		dims = mxGetDimensions(NAME5_IN);
		if(number_of_dimensions>2)
			mexErrMsgTxt("dimension size of the input matrix is too many."); 
		if((dims[0]!=1) || (number_of_dimensions>2))
			mexErrMsgTxt("NAME5_IN is not a string 2.");
		if (mxGetString(NAME5_IN, pAux, MAX_PATH) != 0)
			mexErrMsgTxt("Could not convert string input.");
		if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pAux, -1, (LPWSTR)wcName5, MAX_PATH))
			mexErrMsgTxt("Could not convert to widechar input name.");
	}
	if(nrhs >= 4)
	{
		if (mxIsSparse(NAME_IN))
			mexErrMsgTxt("NAME_IN is a spare matrix.");
		category = mxGetClassID(NAME_IN);
		if(category!=mxCHAR_CLASS)
			mexErrMsgTxt("NAME_IN is not a string 1.");
		number_of_dimensions = (int)mxGetNumberOfDimensions(NAME_IN);
		dims = mxGetDimensions(NAME_IN);
		if(number_of_dimensions>2)
			mexErrMsgTxt("dimension size of the input matrix is too many."); 
		if((dims[0]!=1) || (number_of_dimensions>2))
			mexErrMsgTxt("NAME_IN is not a string 2.");
		if (mxGetString(NAME_IN, pAux, MAX_PATH) != 0)
			mexErrMsgTxt("Could not convert string input.");
		if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pAux, -1, (LPWSTR)wcName, MAX_PATH))
			mexErrMsgTxt("Could not convert to widechar input name.");
	}
	if(nrhs >= 3)
	{
		if (mxIsSparse(prhs[2])) 
			mexErrMsgTxt("third input parameter is a spare matrix."); 
		category = mxGetClassID(STRING_OR_MATRIX_IN);
		if(category==mxCHAR_CLASS)
		{
			if((dims[0]!=1) || (number_of_dimensions>2))
				mexErrMsgTxt("STRING_IN is not a string.");
			if (mxGetString(STRING_OR_MATRIX_IN, pAux, MAX_PATH) != 0)
				mexErrMsgTxt("STRING_IN could not convert string data.");
			if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pAux, -1, (LPWSTR)wcString, MAX_PATH))
				mexErrMsgTxt("Could not convert to widechar string.");
		}else{
			if(IsInteger(category))
			{
				number_of_dimensions = (int)mxGetNumberOfDimensions(STRING_OR_MATRIX_IN);
				dims = mxGetDimensions(STRING_OR_MATRIX_IN);
				if(number_of_dimensions>2)
					mexErrMsgTxt("STRING_OR_MATRIX_IN is not an array 1."); 
				iMatrixSize = 1;
				for (c=0; c<number_of_dimensions; c++)
					iMatrixSize *= dims[c];
				iRowSize = dims[0];
				iColSize = dims[1];
				if(mxIsDouble(STRING_OR_MATRIX_IN))
					pdMatrix = mxGetPr(STRING_OR_MATRIX_IN);
				else
					pMatrix = mxGetData(STRING_OR_MATRIX_IN);
				if((number_of_dimensions==0) || ((number_of_dimensions==1) && (dims[0]==0)) || ((number_of_dimensions==2) && (dims[0]==0) && (dims[1]==0)))
				{
					int iNlhs=1;
					while(iNlhs<=nlhs)
					{
						plhs[iNlhs-1] = mxCreateDummy();
						iNlhs++;
					}
					return;
				}
			}else
				mexErrMsgTxt("Error : bad third input parameter!");
		}
	}

	{
		int iNlhs=1;
		while(iNlhs<=nlhs)
		{
			plhs[iNlhs-1] = NULL;
			iNlhs++;
		}
	}

	if(g_pmxMain)
	{
		g_pmxMain->nlhs = nlhs;
		g_pmxMain->nrhs = nrhs;
		g_pmxMain->plhs = plhs;
		g_pmxMain->prhs = prhs;
		g_pmxMain->number_of_dimensions = number_of_dimensions;
		g_pmxMain->dims[0] = dims[0];
		g_pmxMain->dims[1] = dims[1];
		g_pmxMain->iId = iId;
		g_pmxMain->iId2 = iId2;
		g_pmxMain->wcFunc = &wcFunc[0];
		g_pmxMain->wcString = &wcString[0];
		g_pmxMain->wcName = &wcName[0];
		g_pmxMain->wcName5 = &wcName5[0];
		g_pmxMain->pdMatrix = pdMatrix;
		g_pmxMain->pMatrix = pMatrix;
		if(nrhs >= 3)
			g_pmxMain->iMatrixClass = (int)mxGetClassID(STRING_OR_MATRIX_IN);
	}

	if(iId==-1)
	{
//DRIVER MANAGEMENT START
		if((nrhs==2) && (nlhs==1) && !wcscmp(wcFunc,L"mxIsInit"))
		{
			for(iDll=0;iDll<DLL_COUNT;iDll++)
				pdwData3[iDll] = (DWORD)g_hModule[iDll];
			VALUE_OUT = mxCreateDWORDResult1(DLL_COUNT,pdwData3);
			return;
		}
		if((nrhs==3) && !wcscmp(wcFunc,L"LoadLibrary") && wcString[0])
		{
			ManageLibrary(nlhs, plhs, nrhs, prhs,wcFunc,pdwData3);
			if(nlhs==1)
			{
				VALUE_OUT = mxCreateDWORDResult1(DLL_COUNT,pdwData3);
			}
			return;
		}
		if((nrhs==2) && !wcscmp(wcFunc,L"FreeLibrary"))
		{
			ManageLibrary(nlhs, plhs, nrhs, prhs,wcFunc,pdwData3);
			if(nlhs==1)
				VALUE_OUT = mxCreateDWORDResult1(DLL_COUNT,pdwData3);
			return;
		}
		if((nrhs==2) && !wcscmp(wcFunc,L"version"))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateString(g_pFileVersion[0]);
			return;
		}
		if(g_pmxMain && (*g_pmxMain->g_pmxGeneralProcess)((void*)g_pmxMain))
			return;
//TOOLBOX MANAGEMENT STOP
	}else{
//DRIVER MANAGEMENT START
		if(g_pmxMain && (*g_pmxMain->g_pmxDeviceProcess)((void*)g_pmxMain))
			return;
		if(g_pmxMain && (*g_pmxMain->g_pmxDevice256Process)((void*)g_pmxMain))
			return;
	}
//DRIVER MANAGEMENT STOP
	/* Create a matrix for the return argument */ 
	{
		int iNlhs=1;
		while(iNlhs<=nlhs)
		{
			if(plhs[iNlhs-1]==NULL)
				plhs[iNlhs-1] = mxCreateDummy();
			iNlhs++;
		}
	}
	return;

}

bool myLoadLibrary(const char *pAux,char *pFileVersion,char *pProductVersion,VS_FIXEDFILEINFO **ppVS_FIXEDFILEINF,HMODULE *pModule)
{
	bool bRet=true;
	VS_FIXEDFILEINFO *pVS_FIXEDFILEINF;

	*pModule = LoadLibrary(pAux);
	if(*pModule)
	{
		pVS_FIXEDFILEINF = (VS_FIXEDFILEINFO*)GetProcAddress(*pModule,"g_VS_FIXEDFILEINFO");
		*ppVS_FIXEDFILEINF = pVS_FIXEDFILEINF;
		if(pVS_FIXEDFILEINF)
		{
			sprintf(pFileVersion,"%d.%d.%d.%d",
				(int)HIWORD(pVS_FIXEDFILEINF->dwFileVersionMS),(int)LOWORD(pVS_FIXEDFILEINF->dwFileVersionMS),
				(int)HIWORD(pVS_FIXEDFILEINF->dwFileVersionLS),(int)LOWORD(pVS_FIXEDFILEINF->dwFileVersionLS));
			sprintf(pProductVersion,"%d.%d.%d.%d",
				(int)HIWORD(pVS_FIXEDFILEINF->dwProductVersionMS),(int)LOWORD(pVS_FIXEDFILEINF->dwProductVersionMS),
				(int)HIWORD(pVS_FIXEDFILEINF->dwProductVersionLS),(int)LOWORD(pVS_FIXEDFILEINF->dwProductVersionLS));
			if(strcmp(g_pThisFileVersion,pFileVersion) || strcmp(g_pThisProductVersion,pProductVersion))
				mexPrintf("Version is different: \"%s.dll\" (FileVersion=%s ProductVersion=%s)\n",
					pAux,pFileVersion,pProductVersion);
			//else
			//	mexPrintf("\"%s\" OK 0x%.8lX\n",pName,*pModule);
		}else{
			mexPrintf("\"%s\" no version number\n",pAux);
			sprintf(pFileVersion,"0.0.0.0");
			sprintf(pProductVersion,"0.0.0.0");
			bRet = false;
		}
	}else{
		mexPrintf("LoadLibrary \"%s.dll\"\n\t=> failed (0x%.8lX)\n",pAux,*pModule);
		bRet = false;
	}
	Sleep(10);
	return bRet;
}

#define ConnectGlobalFunction(var_type) g_pmxMain->g_p##var_type = var_type;


//Local variables of functions "ManageLibrary"
char pAux[MAX_PATH];
char pRet[MAX_PATH];
char drive[_MAX_DRIVE];
char dir[_MAX_DIR];
char fname[_MAX_FNAME];
char ext[_MAX_EXT];
void ManageLibrary(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[] , const wchar_t *wcFunc, unsigned long pdwData3[DLL_COUNT])
{
	int iError;
	bool bCheck=true;
	bool bFree=false;
	int iDll;

	for(iDll=0;iDll<DLL_COUNT;iDll++)
		pdwData3[iDll] = (unsigned long)(0);
	if((nrhs==3) && !wcscmp(wcFunc,L"LoadLibrary"))
	{
		for(iDll=0;iDll<DLL_COUNT;iDll++)
		{
			if(g_hModule[iDll])
				bCheck = false;
		}
		if(bCheck)
		{
			sprintf(g_pThisFileVersion,"%d.%d.%d.%d",
				(int)HIWORD(g_VS_FIXEDFILEINFO.dwFileVersionMS),(int)LOWORD(g_VS_FIXEDFILEINFO.dwFileVersionMS),
				(int)HIWORD(g_VS_FIXEDFILEINFO.dwFileVersionLS),(int)LOWORD(g_VS_FIXEDFILEINFO.dwFileVersionLS));
			sprintf(g_pThisProductVersion,"%d.%d.%d.%d",
				(int)HIWORD(g_VS_FIXEDFILEINFO.dwProductVersionMS),(int)LOWORD(g_VS_FIXEDFILEINFO.dwProductVersionMS),
				(int)HIWORD(g_VS_FIXEDFILEINFO.dwProductVersionLS),(int)LOWORD(g_VS_FIXEDFILEINFO.dwProductVersionLS));
			mexPrintf("\"utCmd\" (FileVersion=%s ProductVersion=%s)\n",g_pThisFileVersion,g_pThisProductVersion);
			mxGetString(STRING_OR_MATRIX_IN, pAux, MAX_PATH);
			_splitpath(pAux,drive,dir,fname,ext);
			//load "UTStubMatlab.dll"
			strcpy(pRet,drive);
			strcat(pRet,dir);
			strcat(pRet,"UTStubMatlab");
			strcat(pRet,ext);
			if(!myLoadLibrary(pRet,g_pFileVersion[0],g_pProductVersion[0],&g_paVS_FIXEDFILEINFO[0],&g_hModule[0]))
				mexPrintf("error to load \"%s\"!\n",pAux);
		}
		bCheck = true;
		for(iDll=0;iDll<DLL_COUNT;iDll++)
		{
			pdwData3[iDll] = (unsigned long)(g_hModule[iDll]);
			if(!pdwData3[iDll])
				bCheck = false;
		}
		if(bCheck)
		{
			if(!g_hModule[0])
			{
				iError++;
				mexPrintf("error function \"%s\" unavailabled!\n","mxMain");
			}else{
				g_pmxMain = (CStubMatlab*)GetProcAddress(g_hModule[0],"mxMain");
				if(!g_pmxMain)
				{
					iError++;
					mexPrintf("error function \"%s\" not loaded!\n","mxMain");
				}else{
					//g_pmxMain->g_pmexPrintf = mexPrintf;
					ConnectGlobalFunction(mexErrMsgTxt)
					ConnectGlobalFunction(mexPrintf)
					ConnectGlobalFunction(mxGetClassID)
					ConnectGlobalFunction(mxIsSparse)
					ConnectGlobalFunction(mxGetPr)
					ConnectGlobalFunction(mxSetPr)
					ConnectGlobalFunction(mxGetData)
					ConnectGlobalFunction(mxSetData)
					ConnectGlobalFunction(mxGetDimension)
					ConnectGlobalFunction(mxMalloc2)
					ConnectGlobalFunction(mxCalloc2)
					ConnectGlobalFunction(mxFree)
					ConnectGlobalFunction(mexMakeMemoryPersistent)
					ConnectGlobalFunction(mexMakeArrayPersistent)
					ConnectGlobalFunction(mexFunctionName)
					ConnectGlobalFunction(mexGetVariable)
					ConnectGlobalFunction(mexPutVariable)
					ConnectGlobalFunction(mxDestroyArray)
					ConnectGlobalFunction(mxCreateDummy)
					ConnectGlobalFunction(mxCreateString2)
					ConnectGlobalFunction(mxCreateByteResult0)
					ConnectGlobalFunction(mxCreateByteResult1)
					ConnectGlobalFunction(mxCreateAscanResult2)
					ConnectGlobalFunction(mxCreateCharResult1)
					ConnectGlobalFunction(mxCreateCharResult2)
					ConnectGlobalFunction(mxCreateWordResult1)
					ConnectGlobalFunction(mxCreateShortResult1)
					ConnectGlobalFunction(mxCreateShortResult2)
					ConnectGlobalFunction(mxCreateLongResult1)
					ConnectGlobalFunction(mxCreateLongResult2)
					ConnectGlobalFunction(mxCreateLongLongResult1)
					ConnectGlobalFunction(mxCreateLongLongResult2)
					ConnectGlobalFunction(mxCreateDWORDResult1)
					ConnectGlobalFunction(mxCreateIntegerResult2d)
					ConnectGlobalFunction(mxCreateIntegerResult2)
					ConnectGlobalFunction(mxCreateDoubleResult0)
					ConnectGlobalFunction(mxCreateDoubleResult1)
					ConnectGlobalFunction(mxCreateDoubleResult2)
					/*mexPrintf("function \"%s\" ok!\n",#var_type);*/
					if(!g_pmxMain->g_pmxStubLibrary(true, pAux, g_pThisFileVersion, g_pThisProductVersion))
					{
						mexPrintf("error to load libraries!\n");
						bFree = true;
					}else{
#ifdef _DEBUG
						if(g_pmxMain && g_pmxMain->g_pmxCheckCompilation &&
							!(*g_pmxMain->g_pmxCheckCompilation)(true))
							bFree = true;
#else _DEBUG
						if(g_pmxMain && g_pmxMain->g_pmxCheckCompilation &&
							!(*g_pmxMain->g_pmxCheckCompilation)(false))
							bFree = true;
#endif _DEBUG
					}
				}
			}
		}
	}
	if(bFree || ((nrhs==2) && !wcscmp(wcFunc,L"FreeLibrary")))
	{
		if(g_pmxMain!=NULL)
		{
			g_pmxMain->g_pmxStubLibrary(false, pAux, NULL, NULL);
		}

		for(iDll=0;iDll<DLL_COUNT;iDll++)
		{
			pdwData3[iDll] = (unsigned long)(g_hModule[iDll]);
			if(g_hModule[iDll])
			{
				FreeLibrary(g_hModule[iDll]);
			}
			g_hModule[iDll] = 0;
		}

		g_pmxMain = NULL;
	}
}

void mexEnter()
{
	if(g_pmxMain)
		(*g_pmxMain->g_pmxEnter)();
}
void mexExit()
{
	if(g_pmxMain)
		(*g_pmxMain->g_pmxExit)();
}
