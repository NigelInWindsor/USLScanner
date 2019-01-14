// IGES.cpp: implementation of the CIGES class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "IGES.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIGES::CIGES()
{
	m_pEntity = NULL;
	m_nEntityL = 0;
	m_cParameterDelimiter = ',';
	m_cRecordDelimiter = ';';

	m_fMul = 1.0f;

	m_EntityType[100] = "Circular Arc";
	m_EntityType[102] = "Composite Curve";
	m_EntityType[104] = "Conic Arc";
	m_EntityType[106] = "Copious Data";
	m_EntityType[111] = "2D linear Path";
	m_EntityType[112] = "3D Linear Path";
	m_EntityType[163] = "Simple Closed Planar Curve";
	m_EntityType[108] = "Plane";
	m_EntityType[110] = "Line";
	m_EntityType[112] = "Spline Curve";
	m_EntityType[114] = "Spline Surface";
	m_EntityType[116] = "Point";
	m_EntityType[118] = "Ruled Surface";
	m_EntityType[120] = "Surface of Revolution";
	m_EntityType[122] = "Tabulated Cylinder";
	m_EntityType[124] = "Matrix";
	m_EntityType[125] = "Flash";
	m_EntityType[126] = "Rational B-Spline Curve";
	m_EntityType[128] = "Rational B-Spline Surface";
	m_EntityType[130] = "Offset Curve";
	m_EntityType[140] = "Offset Surface";
	m_EntityType[141] = "Boundary";
	m_EntityType[142] = "Curve on 'P' surface";
	m_EntityType[143] = "Bouded Surface";
	m_EntityType[144] = "Trimmed 'P' Surface";
	m_EntityType[190] = "Plane Surface";
	m_EntityType[192] = "Right circular cylindrical Surface";
	m_EntityType[194] = "Right circular conical surface";
	m_EntityType[196] = "Spherical Surface";
	m_EntityType[198] = "Toroidal surface";
	m_EntityType[202] = "Angular Dimension";
	m_EntityType[210] = "General Label";
	m_EntityType[212] = "General Note";
	m_EntityType[214] = "Leader (Arrow)";
	m_EntityType[216] = "Linear Dimension";
	m_EntityType[218] = "Dimension";
	m_EntityType[222] = "Radius Dimension";

	m_EntityType[404] = "Drawing Entity";
	m_EntityType[410] = "View Entity";


}

CIGES::~CIGES()
{
	if(m_pEntity) {
		for(int nn=0;nn<m_nEntityL;nn++) {
			SAFE_DELETE( m_pEntity[nn].fData );
		}
		delete m_pEntity;
	}
	m_pEntity = NULL;
	m_nEntityL = 0;

}

void CIGES::Zero()
{

	m_strStart.RemoveAll();
	m_strGlobal.RemoveAll();
	m_strDirectory.RemoveAll();
	m_strEntity.RemoveAll();
	m_strTerminate.RemoveAll();
	m_strGlobalFields.RemoveAll();

	if(m_pEntity) {
		for(int nn=0;nn<m_nEntityL;nn++) {
			SAFE_DELETE( m_pEntity[nn].fData );
		}
		delete m_pEntity;
	}
	m_pEntity = NULL;
	m_nEntityL = 0;

	m_cParameterDelimiter = ',';
	m_cRecordDelimiter = ';';

	m_fMul = 1.0f;

}

bool CIGES::Retrieve(CUSLFile *pFile)
{
	CString Buff;
	int nDCount=0;
	int nPCount=0;
	int nTCount=0;
	int nEntityPtr=0;
	char str[84],str1[84];
	int	nLineLength;
	CPolyCoord Line;

	Zero();

	pFile->Read(str,82);
	if((str[80] == '\r') || (str[80] == '\n')) nLineLength=81;
	if((str[81] == '\r') || (str[81] == '\n')) nLineLength=82;

	pFile->SeekToBegin();

	while(pFile->Read(str,nLineLength)==nLineLength) {

		str[80]=0;
		Buff = str;
		switch(str[72]) {
		case 'S':	m_strStart.Add(Buff);
			break;
		case 'G':	m_strGlobal.Add(Buff);
			break;
		case 'D':	m_strEntity.Add(Buff);
			pFile->Read(str1,nLineLength);
			Buff = str1;
			m_strEntity.Add(Buff);
			AddIGESEntity(str,str1);
			nDCount++;
			break;
		case 'P':	m_strDirectory.Add(Buff);
			AddParameterDataToEntity(nEntityPtr,str,pFile,nLineLength);
			Line.Zero();
			if(Line.AddEntity(&m_pEntity[nEntityPtr],nEntityPtr) == TRUE) {
//				AddTaughtLine(&Line);
			}
			nEntityPtr++;
			nPCount++;
			break;
		case 'T':
			nTCount++;
			break;
		}
	}

	GenerateGlobalFields();
	GenerateTransforms();

	return TRUE;

}

void CIGES::AddIGESEntity(char *Line0, char *Line1)
{
	int nn = 0;

	if(m_pEntity==NULL) {
		m_pEntity = new EntityStruct[m_nEntityL=1];
	} else {
		nn = m_nEntityL;
		m_nEntityL++;
		m_pEntity = (EntityStruct *)realloc(m_pEntity,m_nEntityL * sizeof EntityStruct);
	}

	//check for missing zeros replaced with blank spaces
	for(int ii=7;ii<65;ii+=8) {
		if(isalnum(Line0[ii]) == 0) Line0[ii]='0';
		if(isalnum(Line1[ii]) == 0) Line1[ii]='0';
	}

	ZeroMemory(&m_pEntity[nn],sizeof EntityStruct);
	sscanf_s(Line0,"%8d%8d%8d%8d%8d%8d%8d%8d%8d",
		&m_pEntity[nn].nEntityType,		&m_pEntity[nn].nParameterData,
		&m_pEntity[nn].nStructure,		&m_pEntity[nn].nLineFont,
		&m_pEntity[nn].nLevel,			&m_pEntity[nn].nView,
		&m_pEntity[nn].nMatrix,			&m_pEntity[nn].nLabel,
		&m_pEntity[nn].nStatus);
	sscanf_s(Line1,"%8d%8d%8d%8d%8d%8d%8d%8d%8d",
		&m_pEntity[nn].nEntityType,		&m_pEntity[nn].nLineWeight,
		&m_pEntity[nn].nColor,			&m_pEntity[nn].nParameterLineCount,
		&m_pEntity[nn].nFormNumber,		&m_pEntity[nn].nReserved0,
		&m_pEntity[nn].nReserved1,		&m_pEntity[nn].nEntityLabel,
		&m_pEntity[nn].nEntitySubscript);

}

void CIGES::AddParameterDataToEntity(int nEntityPtr, char *Line, CUSLFile *pFile,int nLineLength)
{
	float fData[100000];
	CString Buff;
	int	nLine,mm,nn,nEntityType,nDataPtr;
	char str[80];

	ZeroMemory(fData,sizeof fData);

	for(nLine=0,nDataPtr=0;nLine<m_pEntity[nEntityPtr].nParameterLineCount;nLine++) {
		for(nn=0,mm=0;(nn<64) && (Line[nn]!=',');nn++,mm++) {
			str[mm]=Line[nn];
		}
		str[mm]=0;
		if(nLine==0) {
			nEntityType = atoi(str);
			if(nEntityType != m_pEntity[nEntityPtr].nEntityType) {
				Buff.Format(_T("Entity %d and parameters dont correspond"),nEntityPtr + 1);
				MessageBox(NULL,Buff,_T("Error"),NULL);
			}
		} else {
			fData[nDataPtr++] = (float)atof(str);
		}
		while(nn<64) {
			if(Line[nn]==',') nn++;
			if(Line[nn]==';') nn=64;
			for(nn,mm=0;(nn<64) && (Line[nn]!=',') && (Line[nn]!=';');nn++,mm++) {
				str[mm]=Line[nn];
			}
			str[mm]=0;
			if(nn<64) {
				fData[nDataPtr++] = (float)atof(str);
			}
		}
		if(nLine<(m_pEntity[nEntityPtr].nParameterLineCount-1)) pFile->Read(Line,nLineLength);
		Line[80]=0;
		Buff = Line;
		m_strDirectory.Add(Buff);
	}
	m_pEntity[nEntityPtr].fData = new float[nDataPtr];
	m_pEntity[nEntityPtr].nDataL = nDataPtr;
	CopyMemory(m_pEntity[nEntityPtr].fData,fData,nDataPtr * sizeof (float));

}


//Section 2.2 ASCII file formats

void CIGES::GenerateGlobalFields()
{
	CString Buff,strGlobal;
	int nStart = 2;
	int nFinish,nArrayLength;
	CString strArrayLength;
	float fScale;

	for(int nLine=0;nLine<m_strGlobal.GetSize();nLine++) {
		Buff = m_strGlobal.GetAt(nLine);

		if(nLine==0) {
			nStart = 2;
			if(Buff.GetAt(0) != ',') {
				m_cParameterDelimiter = (char)Buff.GetAt(2);		//1H,
				m_cRecordDelimiter = (char)Buff.GetAt(6);			//1H;
				nStart=8;
			}
		}
		if(nLine == (m_strGlobal.GetSize() - 1)) {
			nFinish = Buff.ReverseFind(m_cRecordDelimiter);
		} else {
			nFinish = Buff.ReverseFind(m_cParameterDelimiter) + 1;
		}
		Buff.GetBufferSetLength(nFinish);
		strGlobal += Buff;
	}
		
	Buff.Format(L"%c",m_cParameterDelimiter);	m_strGlobalFields.Add(Buff);
	Buff.Format(L"%c",m_cRecordDelimiter);		m_strGlobalFields.Add(Buff);
	int nIndex = 3;	//starting from 1
	for(nStart;nStart<strGlobal.GetLength(); nIndex++) {

		switch(nIndex) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 12:
		case 15:
		case 18:
		case 21:
		case 22:
		case 25:
		case 26:
			nFinish=strGlobal.Find(L"H",nStart);
			strArrayLength = strGlobal.Mid(nStart,nFinish - nStart);
			nArrayLength = _ttoi(strArrayLength);
			nStart = nStart + strArrayLength.GetLength() + 1;
			Buff = strGlobal.Mid(nStart,nArrayLength);
			nStart += (nArrayLength + 1);
			break;
		default:
			nFinish = strGlobal.Find(m_cParameterDelimiter,nStart);
			if(nFinish == -1) nFinish = strGlobal.GetLength();
			Buff = strGlobal.Mid(nStart,nFinish - nStart);
			nStart = nFinish + 1;
			break;
		}

		m_strGlobalFields.Add(Buff);
	}

	_WTOF(m_strGlobalFields.GetAt(12),fScale);
	int nUnits = _ttoi(m_strGlobalFields.GetAt(13));
	switch(nUnits) {
	case 1:	fScale *= 25.4f;		//convert inches to mm
		break;
	case 2:						//already in mm
		break;
	case 3:						//ignore
		break;
	case 4: fScale *= 304.8f;	//convert feet to mm
		break;
	case 5: fScale *= 1609344.0f;	//convert miles to mm
		break;
	case 6: fScale *= 1000.0f;	//convert meters to mm
		break;
	case 7: fScale *= 1e6f;		//convert kilometers to mm
		break;
	case 8: fScale *= 0.0254f;	//convert 0.001" to mm
		break;
	case 9: fScale *= 0.001f;	//convert microns to mm
		break;
	case 10: fScale *= 10.0f;	//convert centimeters to mm
		break;
	case 11:					// i dont know what a micro inch is
		break;
	}
	m_fMul = fScale;

	_WTOF(m_strGlobalFields.GetAt(19),m_fMaxSize);


}

bool CIGES::Format(EntityStruct *pEntity, CString *pBuff)
{
	CString strTemp,strAll;
	int nIP,nN;

	pBuff->Empty();
	switch(pEntity->nEntityType) {
	case 100: strTemp.Format(L"Circular Arc Zt (%.02f) X1,Y1 (%.02f,%.02f) X2,Y2 (%.02f,%.02f) X3,Y3 (%.02f,%.02f)\r\n",pEntity->fData[0],pEntity->fData[1],pEntity->fData[2],pEntity->fData[3],pEntity->fData[4],pEntity->fData[5],pEntity->fData[6]);
		strAll += strTemp;
		strTemp.Format(L"Matrix %d",pEntity->nMatrix);
		strAll += strTemp;
		pBuff->Format(L"%s",strAll);
		break;
	case 104: 
		switch(pEntity->nFormNumber) {
		default: pBuff->Format(L"%s; Undefined",m_EntityType[pEntity->nEntityType]);
			break;
		case 1: pBuff->Format(L"%s; Ellipse %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f",m_EntityType[pEntity->nEntityType]
					,pEntity->fData[0],pEntity->fData[1],pEntity->fData[2],pEntity->fData[3],pEntity->fData[4],pEntity->fData[5]
					,pEntity->fData[6],pEntity->fData[7],pEntity->fData[8],pEntity->fData[9],pEntity->fData[10]);
			break;
		case 2: pBuff->Format(L"%s; Hyperbola",m_EntityType[pEntity->nEntityType]);
			break;
		case 3: pBuff->Format(L"%s; Parabola",m_EntityType[pEntity->nEntityType]);
			break;
		}
		break;
	case 106:
		nIP = (int)pEntity->fData[0];
		nN = (int)pEntity->fData[1];
		strTemp.Format(L"IP=%d N=%d\r\n",nIP,nN);
		strAll += strTemp;
		switch(pEntity->nFormNumber) {
		default: strTemp.Format(L"Form=%d\r\n",pEntity->nFormNumber);
			break;
		case 40: strTemp.Format(L"Form=%d: Witness Line Entity\r\n",pEntity->nFormNumber);
			break;
		}
		strAll += strTemp;
		pBuff->Format(L"%s",strAll);
		break;
	case 110: strAll.Format(L"%s X1,Y1,Z1 (%.02f,%.02f,%.02f) X2,Y2,Z2 (%.02f,%.02f,%.02f)",m_EntityType[pEntity->nEntityType],pEntity->fData[0],pEntity->fData[1],pEntity->fData[2],pEntity->fData[3],pEntity->fData[4],pEntity->fData[5]);
		if(pEntity->nMatrix) {
			strTemp.Format(L"\r\n%.02f %.02f %.02f %.02f",pEntity->mat._11,pEntity->mat._12,pEntity->mat._13,pEntity->mat._14);
			strAll += strTemp;
			strTemp.Format(L"\r\n%.02f %.02f %.02f %.02f",pEntity->mat._21,pEntity->mat._22,pEntity->mat._23,pEntity->mat._24);
			strAll += strTemp;
			strTemp.Format(L"\r\n%.02f %.02f %.02f %.02f",pEntity->mat._31,pEntity->mat._32,pEntity->mat._33,pEntity->mat._34);
			strAll += strTemp;
			strTemp.Format(L"\r\n%.02f %.02f %.02f %.02f",pEntity->mat._41,pEntity->mat._42,pEntity->mat._43,pEntity->mat._44);
			strAll += strTemp;
		};
		pBuff->Format(L"%s",strAll);
		break;
	case 116: pBuff->Format(L"%s X1,Y1,Z1 (%.02f,%.02f,%.02f)",m_EntityType[pEntity->nEntityType],pEntity->fData[0],pEntity->fData[1],pEntity->fData[2]);
		break;
	case 124:
		strTemp.Format(L"%s X0,Y0,Z0 T0 (%.02f,%.02f,%.02f,%.02f)\r\n",m_EntityType[pEntity->nEntityType],pEntity->fData[0],pEntity->fData[1],pEntity->fData[2],pEntity->fData[3]);
		strAll += strTemp;
		strTemp.Format(L"%s X1,Y1,Z1 T1 (%.02f,%.02f,%.02f,%.02f)\r\n",m_EntityType[pEntity->nEntityType],pEntity->fData[4],pEntity->fData[5],pEntity->fData[6],pEntity->fData[7]);
		strAll += strTemp;
		strTemp.Format(L"%s X2,Y2,Z2 T2 (%.02f,%.02f,%.02f,%.02f)\r\n",m_EntityType[pEntity->nEntityType],pEntity->fData[8],pEntity->fData[9],pEntity->fData[10],pEntity->fData[11]);
		strAll += strTemp;
		pBuff->Format(L"%s",strAll);
		break;
	case 202:
	case 210:
	case 212:
	case 214:
	case 216:
	case 218:
	case 222:
		pBuff->Format(L"%s",m_EntityType[pEntity->nEntityType]);
		break;

	}

	return true;
}


bool CIGES::CircleArcLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax)
{
	if(pEntity->nEntityType != 100) return false;
	struct ArcData {
		float fZt;
		float fXCentre;
		float fYCentre;
		float fX0;
		float fY0;
		float fX1;
		float fY1;
	} *pArc = (ArcData*)pEntity->fData;

	float fX = pArc->fX0-pArc->fXCentre;
	float fY = pArc->fY0-pArc->fYCentre;
	float fRadius = sqrtf((fX*fX) + (fY*fY));

	D3DXVECTOR3 vec[4];
	vec[0] = D3DXVECTOR3(pArc->fXCentre-fRadius,pArc->fYCentre,pArc->fZt);
	vec[1] = D3DXVECTOR3(pArc->fXCentre+fRadius,pArc->fYCentre,pArc->fZt);
	vec[2] = D3DXVECTOR3(pArc->fXCentre,pArc->fYCentre-fRadius,pArc->fZt);
	vec[3] = D3DXVECTOR3(pArc->fXCentre,pArc->fYCentre+fRadius,pArc->fZt);
	if(pEntity->nMatrix) {
		D3DXVec3TransformCoord(&vec[0],&vec[0],&pEntity->mat);
		D3DXVec3TransformCoord(&vec[1],&vec[1],&pEntity->mat);
		D3DXVec3TransformCoord(&vec[2],&vec[2],&pEntity->mat);
		D3DXVec3TransformCoord(&vec[3],&vec[3],&pEntity->mat);
	}
	for(int ii=0;ii<4;ii++) {
		if(vec[ii].x < pvecMin->x) pvecMin->x = vec[ii].x;
		if(vec[ii].y < pvecMin->y) pvecMin->y = vec[ii].y;
		if(vec[ii].x > pvecMax->x) pvecMax->x = vec[ii].x;
		if(vec[ii].y > pvecMax->y) pvecMax->y = vec[ii].y;
	}
	return true;

}

bool CIGES::LineLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax)
{
	if(pEntity->nEntityType != 110) return false;

	if(pEntity->fData[0] < pvecMin->x) pvecMin->x = pEntity->fData[0];
	if(pEntity->fData[1] < pvecMin->y) pvecMin->y = pEntity->fData[1];
	if(pEntity->fData[2] < pvecMin->z) pvecMin->z = pEntity->fData[2];

	if(pEntity->fData[0] > pvecMax->x) pvecMax->x = pEntity->fData[0];
	if(pEntity->fData[1] > pvecMax->y) pvecMax->y = pEntity->fData[1];
	if(pEntity->fData[2] > pvecMax->z) pvecMax->z = pEntity->fData[2];

	if(pEntity->fData[3] < pvecMin->x) pvecMin->x = pEntity->fData[3];
	if(pEntity->fData[4] < pvecMin->y) pvecMin->y = pEntity->fData[4];
	if(pEntity->fData[5] < pvecMin->z) pvecMin->z = pEntity->fData[5];

	if(pEntity->fData[3] > pvecMax->x) pvecMax->x = pEntity->fData[3];
	if(pEntity->fData[4] > pvecMax->y) pvecMax->y = pEntity->fData[4];
	if(pEntity->fData[5] > pvecMax->z) pvecMax->z = pEntity->fData[5];

	return true;
}
			

bool CIGES::PointLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax)
{
	if(pEntity->nEntityType != 116) return false;

	if(pEntity->fData[0] < pvecMin->x) pvecMin->x = pEntity->fData[0];
	if(pEntity->fData[1] < pvecMin->y) pvecMin->y = pEntity->fData[1];
	if(pEntity->fData[2] < pvecMin->z) pvecMin->z = pEntity->fData[2];

	if(pEntity->fData[0] > pvecMax->x) pvecMax->x = pEntity->fData[0];
	if(pEntity->fData[1] > pvecMax->y) pvecMax->y = pEntity->fData[1];
	if(pEntity->fData[2] > pvecMax->z) pvecMax->z = pEntity->fData[2];

	return true;

}

bool CIGES::ConicArcLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax)
{//this is completely wrong and needs doing
	if(pEntity->nEntityType != 104) return false;
	struct ArcData {
		float fZt;
		float fXCentre;
		float fYCentre;
		float fX0;
		float fY0;
		float fX1;
		float fY1;
	} *pArc = (ArcData*)pEntity->fData;

	float fX = pArc->fX0-pArc->fXCentre;
	float fY = pArc->fY0-pArc->fYCentre;
	float fRadius = sqrtf((fX*fX) + (fY*fY));

	if((pArc->fXCentre-fRadius) < pvecMin->x) pvecMin->x = pArc->fXCentre-fRadius;
	if((pArc->fYCentre-fRadius) < pvecMin->y) pvecMin->y = pArc->fYCentre-fRadius;
	if((pArc->fXCentre+fRadius) > pvecMax->x) pvecMax->x = pArc->fXCentre+fRadius;
	if((pArc->fYCentre+fRadius) > pvecMax->y) pvecMax->y = pArc->fYCentre+fRadius;

	return true;

}

void CIGES::GenerateTransforms()
{
	CString Buff,Temp;
	int nMatrixPointer,jj;

	for(int ii=0;ii<m_nEntityL;ii++) {

		if(m_pEntity[ii].nMatrix) {
			Buff=m_strEntity.GetAt(m_pEntity[ii].nMatrix-1);
			Temp.Empty();
			for(jj=73;jj<80;jj++) Temp += Buff.GetAt(jj);
			nMatrixPointer = (_ttoi(Temp) - 1)/2;

			D3DXMatrixIdentity(&m_pEntity[ii].mat);

			m_pEntity[ii].mat._11 = m_pEntity[nMatrixPointer].fData[0];
			m_pEntity[ii].mat._21 = m_pEntity[nMatrixPointer].fData[1];
			m_pEntity[ii].mat._31 = m_pEntity[nMatrixPointer].fData[2];
			m_pEntity[ii].mat._12 = m_pEntity[nMatrixPointer].fData[4];
			m_pEntity[ii].mat._22 = m_pEntity[nMatrixPointer].fData[5];
			m_pEntity[ii].mat._32 = m_pEntity[nMatrixPointer].fData[6];
			m_pEntity[ii].mat._13 = m_pEntity[nMatrixPointer].fData[8];
			m_pEntity[ii].mat._23 = m_pEntity[nMatrixPointer].fData[9];
			m_pEntity[ii].mat._33 = m_pEntity[nMatrixPointer].fData[10];

			m_pEntity[ii].mat._41 = m_pEntity[nMatrixPointer].fData[3];
			m_pEntity[ii].mat._42 = m_pEntity[nMatrixPointer].fData[7];
			m_pEntity[ii].mat._43 = m_pEntity[nMatrixPointer].fData[11];

		}

	}
}
