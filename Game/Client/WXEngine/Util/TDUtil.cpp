#include "StdAfx.h"
#include "TDUtil.h"
#include "TDException.h"
#include <stdio.h>
#include <math.h>


//====================================
//字符串相关
void TDU_Log(LPCSTR pszFmt, ...)
{
	FILE* fp = fopen(".\\WXClient.log", "a" );
	if(!fp) fp = fopen(".\\WXClient.log", "w");
	if(!fp) return;

	CHAR szTemp[1024] = {0};
	va_list ptr; va_start(ptr, pszFmt);
	_vsnprintf((char *)szTemp, 1024, pszFmt, ptr);
	va_end(ptr);

	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);

	fprintf(fp, "[%02d_%02d:%02d:%02d]: %s\n", 
		timeNow.wDay, timeNow.wHour, timeNow.wMinute, timeNow.wSecond, szTemp);
	fclose(fp);
}

//====================================
//字符串相关
INT	TDU_ConvertStringToVector(LPCTSTR strStrintgSource, std::vector< STRING >& vRet, LPCTSTR szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty)
{
	vRet.clear();

	//------------------------------------------------------------
	//合法性
	if(!strStrintgSource || strStrintgSource[0] == '\0') return 0;
	
	STRING strSrc = strStrintgSource;

	//------------------------------------------------------------
	//查找第一个分割点
	STRING::size_type nLeft = 0;
	STRING::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(TRUE)
	{
		STRING strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : _tcslen(szKey));
		
		if(bOneOfKey)
		{
			STRING strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != STRING::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (INT)vRet.size();
}

//====================================
//数学相关
FLOAT TDU_GetDist(const fVector2& fvPos1, const fVector2& fvPos2)
{
	return sqrtf( (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y));
}

FLOAT TDU_GetDist(const fVector3& fvPos1, const fVector3& fvPos2)
{
	return sqrtf( (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y) +  (fvPos1.z-fvPos2.z)*(fvPos1.z-fvPos2.z));
}

FLOAT TDU_GetDistSq(const fVector2& fvPos1, const fVector2& fvPos2)
{
	return (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y);
}

FLOAT TDU_GetDistSq(const fVector3& fvPos1, const fVector3& fvPos2)
{
	return (fvPos1.x-fvPos2.x)*(fvPos1.x-fvPos2.x) + (fvPos1.y-fvPos2.y)*(fvPos1.y-fvPos2.y) +  (fvPos1.z-fvPos2.z)*(fvPos1.z-fvPos2.z);
}

FLOAT TDU_GetYAngle(const fVector2& fvPos1, const fVector2& fvPos2)	
{
	DOUBLE dDistance = (DOUBLE)TDU_GetDist(fvPos1, fvPos2 );
	if(dDistance <= 0.0f) return 0.0f;

	DOUBLE fACos = (fvPos2.y - fvPos1.y ) / dDistance;
	if( fACos >  1.0) fACos = 0.0;
	if( fACos < -1.0) fACos = TDU_PI;

	if( fACos >-TDU_MINFLOAT && fACos <TDU_MINFLOAT)
	{
		if(fvPos2.x > fvPos1.x )	return  (FLOAT)TDU_PI/2.0f;
		else						return -(FLOAT)TDU_PI/2.0f;
	}

	fACos = ::acos(fACos);
 
	// [0~180]
	if(fvPos2.x >= fvPos1.x)
		return (FLOAT)fACos;
	//(180, 360)
	else
		return (FLOAT)(2*TDU_PI - fACos);
}

fVector3 TDU_GetCenter(const fVector3& fvPos1, const fVector3& fvPos2)
{
	fVector3 fvRet;

	fvRet.x = (fvPos1.x + fvPos2.x)/2.0f;
	fvRet.y = (fvPos1.y + fvPos2.y)/2.0f;
	fvRet.z = (fvPos1.z + fvPos2.z)/2.0f;

	return fvRet;
}

fVector2 TDU_Normalise(const fVector2& vec)
{
	fVector2 fvRet = vec;
	float fLength = sqrt(fvRet.x * fvRet.x + fvRet.y * fvRet.y);

	// Will also work for zero-sized vectors, but will change nothing
	if ( fLength > 1e-08 )
	{
		float fInvLength = 1.0f / fLength;
		fvRet.x *= fInvLength;
		fvRet.y *= fInvLength;
	}
	return fvRet;
}

fVector2 TDU_GetReflect(const fVector2& fvPos1, const fVector2& fvPos2, const fVector2& v1)
{
	// pos1 -> pos2
	fVector2 fvNormal = fvPos2 - fvPos1;
	fvNormal.normalise();
	// pos1 -> v1
	fVector2 fvLine1 = v1 - fvPos1;
	// pos1 -> v2
	fVector2 fvLine2 = 2 * TDU_DotProduct(fvLine1, fvNormal)* fvNormal - fvLine1;
	//return v2
	return fvLine2 + fvPos1;
}

//从内存中字符串读取一行文本(按照换行符)
const char* TDU_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd)
{
	//寻找下一个换行符
	register const char* pMem = pMemory;
	if(pMem >= pDeadEnd || *pMem==0) return 0;

	while(pMem < pDeadEnd && pMem-pMemory+1<nBufSize && 
			*pMem != 0 && *pMem != '\r' && *pMem != '\n') *(pStringBuf++) = *(pMem++);
	//add 'null' end
	*pStringBuf = 0;

	//skip all next \r and \n
	while(pMem < pDeadEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) pMem++;

	return pMem;
}

BOOL TDU_CheckStringValid(const char* pStr)
{
	const char* szInvalid = " #\\/`~!@~$%^&*()-_+=|{}[];:'\"<>,.?";
	const char* szHZBlank = "　"; //中文空格

	if(NULL == pStr) return FALSE;
	std::string strName(pStr);

	if(strName.empty()) return FALSE;

	//查找是否有非法ascii字符
	STRING::size_type nInvalidPos = strName.find_first_of(szInvalid);
	if(nInvalidPos == STRING::npos)
	{
		//中文空格
		nInvalidPos = strName.find(szHZBlank);
	}

	if(nInvalidPos != STRING::npos)
	{
		return FALSE;
	}
	return TRUE;
}
