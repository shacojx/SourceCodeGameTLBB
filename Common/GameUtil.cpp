#include "stdafx.h"
#include "Type.h"
#include "GameUtil.h"

#ifdef __LINUX__
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <setjmp.h>
#endif

INT					g_CmdArgv;
INT					g_LockTimes=0 ;
BOOL				g_LockTimeOutEnable=FALSE;

WORLD_TIME			g_WorldTime = WT_IND_1;

VOID	sm_lock(CHAR& flag,CHAR type)
{
	bgsm_loop:
	if(flag==SM_FREE)
	{
		flag=type;
		if(flag!=type)
		{
			MySleep(1);
#ifdef __LINUX__
			g_LockTimes ++ ;
			printf("lock fail %s,%d,%s",__FILE__,__LINE__,__PRETTY_FUNCTION__);
#endif
			if((g_LockTimes>100)&&g_LockTimeOutEnable)
			{
				g_LockTimes =0;
				return;
			}
			goto bgsm_loop;
		}
		return;
	}
	else
	{
		MySleep(1);
		goto	bgsm_loop;
	}
}



VOID	sm_unlock(CHAR& flag,CHAR type)
{
	edsm_loop:
		
		if(flag==SM_FREE)
		{
			return;
		}
		
		flag=SM_FREE;
	
		if(flag!=SM_FREE)
		{
			MySleep(1);
#ifdef __LINUX__
			printf("unlock fail %s,%s,%s",__FILE__,__LINE__,__PRETTY_FUNCTION__);
			if((g_LockTimes>100)&&g_LockTimeOutEnable)
			{
				g_LockTimes =0;
				return;
			}
#endif
			goto edsm_loop;
		}
		return;
	
}

BOOL sm_trylock(CHAR& flag,CHAR type)
{
	INT LockTimes;

bgsm_loop:

	if(flag==SM_FREE)
	{
		flag=type;
		if(flag!=type)
		{
			LockTimes++;
			MySleep(1);
			if(LockTimes>10)
			{
				return FALSE;
			}
			goto bgsm_loop;
		}
		return TRUE;
	}
	else
	{
		LockTimes++;
		MySleep(1);
		if(LockTimes>10)
		{
			return FALSE;
		}
		goto	bgsm_loop;
	}
}




VOID MySleep( UINT millionseconds )
{
#if defined(__WINDOWS__)
	Sleep( millionseconds ) ;
#elif defined(__LINUX__)
	usleep( millionseconds*1000 ) ;
#endif
}

TID MyGetCurrentThreadID( )
{
#if defined(__WINDOWS__)
	return GetCurrentThreadId( ) ;
#elif defined(__LINUX__)
	return pthread_self();
#endif
}

FLOAT MySqrt( const WORLD_POS* pCur, const WORLD_POS* pTar )
{
__ENTER_FUNCTION
	Assert(pCur);
	Assert(pTar);
	return (FLOAT)sqrt( ((double)pCur->m_fX-(double)pTar->m_fX)*((double)pCur->m_fX-(double)pTar->m_fX)+
						((double)pCur->m_fZ-(double)pTar->m_fZ)*((double)pCur->m_fZ-(double)pTar->m_fZ) ) ;
__LEAVE_FUNCTION
	return 0.0f;
}

FLOAT MyLengthSq( const WORLD_POS* pCur, const WORLD_POS* pTar )
{
	return (FLOAT)(((double)pCur->m_fX-(double)pTar->m_fX)*((double)pCur->m_fX-(double)pTar->m_fX)+
						((double)pCur->m_fZ-(double)pTar->m_fZ)*((double)pCur->m_fZ-(double)pTar->m_fZ) ) ;
}

FLOAT MyAngle( const WORLD_POS* pCur, const WORLD_POS* pTar )
{
	//double Sqrt = sqrt( (pCur->m_fX-pTar->m_fX)*(pCur->m_fX-pTar->m_fX)+
	//					(pCur->m_fZ-pTar->m_fZ)*(pCur->m_fZ-pTar->m_fZ) ) ;
	//double Acos = acos((pTar->m_fZ-pCur->m_fZ)/ Sqrt);
	//return (FLOAT)Acos ;

	double Sqrt = (double)sqrt( (pCur->m_fX-pTar->m_fX)*(pCur->m_fX-pTar->m_fX)+
						(pCur->m_fZ-pTar->m_fZ)*(pCur->m_fZ-pTar->m_fZ) );

	if(Sqrt <= 0.0f) return 0.0f;

	double fACos = (pTar->m_fZ-pCur->m_fZ)/ Sqrt;
	if( fACos >  1.0) fACos = 0.0;
	if( fACos < -1.0) fACos = __PI;

	fACos = ::acos(fACos);
 
	// [0~180]
	if(pTar->m_fX >= pCur->m_fX)
		return (FLOAT)fACos;
	//(180, 360)
	else
		return (FLOAT)(2*__PI - fACos);
}

BOOL MyPosEqual( const WORLD_POS* pA, const WORLD_POS* pB )
{
#ifndef ZERO_VALUE
#define ZERO_VALUE 0.001
#endif
	if( pA->m_fX>pB->m_fX ) 
	{
		if( pA->m_fX-pB->m_fX > ZERO_VALUE ) return FALSE ;
	}
	else
	{
		if( pB->m_fX-pA->m_fX > ZERO_VALUE ) return FALSE ;
	}

	if( pA->m_fZ>pB->m_fZ ) 
	{
		if( pA->m_fZ-pB->m_fZ > ZERO_VALUE ) return FALSE ;
	}
	else
	{
		if( pB->m_fZ-pA->m_fZ > ZERO_VALUE ) return FALSE ;
	}

	return TRUE ;
}

VOID MyRandPos( WORLD_POS* pPos, const WORLD_POS* pDir, FLOAT fRadioMax, FLOAT fRadioMin )
{
// 最大，最小角度值取的是 COS值
#define COS_ANGLE_MIN	0.7071f
#define COS_ANGLE_MAX   1.0f	

#define ANGLE_MIN	0
#define ANGLE_MAX   __PI / 4	

	FLOAT fR1=fRadioMin ;
	FLOAT fR2=fRadioMax ;
	FLOAT fAx=pPos->m_fX ;
	FLOAT fAz=pPos->m_fZ ;
	FLOAT fBx=pDir->m_fX ;
	FLOAT fBz=pDir->m_fZ ;
	FLOAT fPx, fPz ;
		
	if((fR1 < 0)||(fR2 < 0))
	{
		// 半径是负数。  
		return ;
	}

	FLOAT fVec1X = 0;		// 向量AB
	FLOAT fVec1Y = 0;		// 
	
	FLOAT fRandThi    = 0; // 随机角度
	FLOAT fRandLength = 0; // 随机长度

	// 求整数半径
	INT   iR1 = 0;
	INT   iR2 = 0;
	iR1 = (INT)(fR1 * 100);
	iR2 = (INT)(fR2 * 100);
	if(iR1 > iR2)
	{
		INT iTmep = iR1;
		iR1 = iR2;
		iR2 = iTmep;
	}

	// 求随机半径长度。
	INT iLenDeta    = iR2 - iR1;
	INT iRandLength = 0;
	if(0 == iR1)
	{
		iRandLength = rand() % (iR2 + 1);
	}
	else
	{
		iRandLength = rand() % iR1 + iLenDeta;
		
	}

	fRandLength = (FLOAT)(iRandLength * 1.0) / 100;

	// 求随机角度值。
	INT iThiMin  = (INT)(ANGLE_MIN * 100);
	INT iThiMax  = (INT)(ANGLE_MAX * 100);
	INT iThiDeta = iThiMax - iThiMin;
	INT iRandThi = 0;
	if(0 == iThiMin )
	{
		iRandThi = rand() % (iThiMax + 1);
	}
	else
	{
		iRandThi = rand() % iThiMin + iThiDeta;
	}
	fRandThi     = (FLOAT)(iRandThi * 1.0) / 100;

	// 求随机角度方向
   	INT iDir = rand() % 2;
	if(0 == iDir)
	{
		fRandThi = -fRandThi;
	}
	
	fVec1X = fBx - fAx;
	fVec1Y = fBz - fAz;
	FLOAT fVec1Len = ::sqrtf(fVec1X * fVec1X + fVec1Y * fVec1Y);

	if(fabs(FLOAT(fVec1Len)) < 0.00001f)
	{
		// A点， B点重合.
		fPx = fAx;
		fPz = fAz;
		return ;
	}

	FLOAT fThi = ::asin(FLOAT(fVec1X / fVec1Len));
	fThi += fRandThi;

	FLOAT x = ::cos(fThi) * fRandLength;
	FLOAT y = ::sin(fThi) * fRandLength;

	fPx = x + fAx;
	fPz = y + fAz;

	pPos->m_fX = fPx ;
	pPos->m_fZ = fPz ;
}

CHAR* MySocketError( )
{
#ifndef _ESIZE
#define _ESIZE 256
#endif

extern CHAR Error[_ESIZE] ;
	return Error ;
}


UINT g_aCrc32Table[256] =
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};

UINT MyCRC( const CHAR* szString )
{
	if(szString==NULL || szString[0]==0) return 0;

	UINT dwCrc32 = 0xFFFFFFFF;
	INT nSize = (INT)strlen(szString);
	for(INT i=0; i<nSize; i++)
	{
		dwCrc32 = dwCrc32*33 + (UCHAR)szString[i] ;
//		dwCrc32 = ((dwCrc32)>>8)^g_aCrc32Table[(szString[i])^((dwCrc32)&0x000000FF)];
	}

	return dwCrc32;
}

/*
BOOL	Binary2String(const CHAR* pIn,UINT InLength,CHAR* pOut)
{
	__ENTER_FUNCTION

	if(InLength==0)
	{
		return FALSE;
	}
	UINT iOut = 0;
	for(UINT i = 0 ;i<InLength;i++)
	{
		switch(pIn[i]) 
		{
		case '\0':
			{
				pOut[iOut] = '\\';
			    iOut++;
				pOut[iOut] = '0';
				iOut++;
			}
			break;
		case '\'':
			{
				pOut[iOut] = '\'';
				iOut++;
				pOut[iOut] = '\'';
				iOut++;
			}
			break;
		case '\"':
			{
				pOut[iOut] = '\\';
				iOut++;
				pOut[iOut] = '\"';
				iOut++;
			}
			break;
		case '\\':
			{
				pOut[iOut] = '\\';
				iOut++;
				pOut[iOut] = '\\';
				iOut++;
			}
			break;
		default:
			{
				pOut[iOut] = pIn[i];
				iOut++;
			}
			break;
		}
		
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
*/

CHAR		Value2Ascii(CHAR in)
{
	__ENTER_FUNCTION
		switch(in) 
		{
			case 0:
				return '0';
				break;
			case 1:
				return '1';
			case 2:
				return '2';
				break;
			case 3:
				return '3';
				break;
			case 4:
				return '4';
				break;
			case 5:
				return '5';
				break;
			case 6:
				return '6';
				break;
			case 7:
				return '7';
				break;
			case 8:
				return '8';
				break;
			case 9:
				return '9';
				break;
			case 10:
				return 'A';
				break;
			case 11:
				return 'B';
				break;
			case 12:
				return 'C';
				break;
			case 13:
				return 'D';
				break;
			case 14:
				return 'E';
				break;
			case 15:
				return 'F';
				break;
			default:
				Assert(FALSE);
				return '?';
				break;
		}

	__LEAVE_FUNCTION

		return '?';
}

CHAR Ascii2Value(CHAR in)
{
	__ENTER_FUNCTION
		switch(in) 
	{
		case '0':
			return 0;
			break;
		case '1':
			return 1;
		case '2':
			return 2;
			break;
		case '3':
			return 3;
			break;
		case '4':
			return 4;
			break;
		case '5':
			return 5;
			break;
		case '6':
			return 6;
			break;
		case '7':
			return 7;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 9;
			break;
		case 'A':
			return 10;
			break;
		case 'B':
			return 11;
			break;
		case 'C':
			return 12;
			break;
		case 'D':
			return 13;
			break;
		case 'E':
			return 14;
			break;
		case 'F':
			return 15;
			break;
		default:
			Assert(FALSE);
			return '?';
			break;
	}

	__LEAVE_FUNCTION

		return '?';
}


BOOL	Binary2String(const CHAR* pIn,UINT InLength,CHAR* pOut)
{
	__ENTER_FUNCTION

		if(InLength==0)
		{
			return FALSE;
		}
		UINT iOut = 0;

		
		for(UINT i = 0 ;i<InLength;i++)
		{
			
			pOut[iOut] = Value2Ascii(((unsigned char)pIn[i]&0xF0)>>4);
			iOut++;
			pOut[iOut] = Value2Ascii(pIn[i]&0x0F);
			iOut++;
		

		}
		return TRUE;

		__LEAVE_FUNCTION

			return FALSE;
}

/*
BOOL DBStr2Binary(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength)
{
	__ENTER_FUNCTION

	if(InLength==0)
	{
		return FALSE;
	}

	UINT iOut = 0;
	UINT i;
	for( i = 0 ;i<InLength-1;)
	{
			if((pIn[i]=='\\') && (pIn[i+1]=='0'))
			{
				pOut[iOut]	=	'\0';
				iOut++;
				i+=2;
				continue;
			}
			else if((pIn[i]=='\\') && (pIn[i+1]=='\"'))
			{
				pOut[iOut]	=	'\"';
				iOut++;
				i+=2;
				continue;
			}
			else if((pIn[i]=='\\') && (pIn[i+1]=='\\'))
			{
				pOut[iOut]	=	'\\';
				iOut++;
				i+=2;
				continue;
			}
			else if(pIn[i]=='\0')
			{
				break;
			}

		pOut[iOut] = pIn[i];
		iOut++;
		i++;

		if(iOut>=OutLimit)
			break;
	}
	OutLength = iOut;
	return TRUE;

	__LEAVE_FUNCTION
}
*/

BOOL DBStr2Binary(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength)
{
	__ENTER_FUNCTION

		if(InLength==0)
		{
			return FALSE;
		}

		UINT iOut = 0;
		UINT i;
		for( i = 0 ;i<InLength-1;)
		{
			if(pIn[i]=='\0'||pIn[i+1]=='\0')
			{
				break;
			}
			
			pOut[iOut]	=	(Ascii2Value(pIn[i])<<4) + Ascii2Value(pIn[i+1]);
			iOut++;
			i+=2;
			if(iOut>=OutLimit)
				break;
		}
		OutLength = iOut;
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
}


BOOL String2Binary(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength)
{
	__ENTER_FUNCTION

		if(InLength==0)
		{
			return FALSE;
		}

		UINT iOut = 0;
		UINT i;
		for( i = 0 ;i<InLength-1;)
		{
			if((pIn[i]=='\\') && (pIn[i+1]=='0'))
			{
				pOut[iOut]	=	'\0';
				iOut++;
				i+=2;
				continue;
			}
			else if((pIn[i]=='\'') && (pIn[i+1]=='\''))
			{
			pOut[iOut]	=	'\'';
			iOut++;
			i+=2;
			continue;
			}
			else if((pIn[i]=='\\') && (pIn[i+1]=='\"'))
			{
				pOut[iOut]	=	'\"';
				iOut++;
				i+=2;
				continue;
			}
			else if((pIn[i]=='\\') && (pIn[i+1]=='\\'))
			{
				pOut[iOut]	=	'\\';
				iOut++;
				i+=2;
				continue;
			}
			else if(pIn[i]=='\0')
			{
				break;
			}

			pOut[iOut] = pIn[i];
			iOut++;
			i++;

			if(iOut>=OutLimit)
				break;
		}
		OutLength = iOut;
		return TRUE;

		__LEAVE_FUNCTION
}

BOOL		StrSafeCheck(const CHAR* pIn,UINT InLength)
{
	if(InLength==0)
	{
		return FALSE;
	}
	for(UINT i = 0 ;i<InLength;i++)
	{
		switch(pIn[i]) 
		{
			case '\0':
				{
					return TRUE;
					break;
				}
			case '\'':
			case '\"':
			case ')':
			case '(':
			case '=':
			case '%':
				{
					return FALSE;
				}
		}
	}

	return TRUE;

}

BOOL		CommandSafeCheck(const CHAR* pIn,UINT InLength)
{
	if(InLength==0)
	{
		return FALSE;
	}
	for(UINT i = 0 ;i<InLength;i++)
	{
		switch(pIn[i]) 
		{
		case '\0':
			{
				return TRUE;
				break;
			}
		case '%':
			{
				return FALSE;
			}
		}
	}

	return TRUE;

}

UINT GetMailSize( const MAIL& mail )
{
	if( mail.m_uFlag==MAIL_TYPE_SCRIPT )
	{
		return	sizeof(GUID_t)+
				sizeof(BYTE)+
				sizeof(CHAR)*mail.m_SourSize+
				sizeof(INT)+
				sizeof(BYTE)+
				sizeof(CHAR)*mail.m_DestSize+
				sizeof(WORD)+
				sizeof(CHAR)*mail.m_ContexSize+
				sizeof(UINT)+
				sizeof(UINT)+
				sizeof(CampID_t)+
				sizeof(UINT)+
				sizeof(UINT)+
				sizeof(UINT)+
				sizeof(UINT);
	}
	else
	{
		return	sizeof(GUID_t)+
				sizeof(BYTE)+
				sizeof(CHAR)*mail.m_SourSize+
				sizeof(INT)+
				sizeof(BYTE)+
				sizeof(CHAR)*mail.m_DestSize+
				sizeof(WORD)+
				sizeof(CHAR)*mail.m_ContexSize+
				sizeof(UINT)+
				sizeof(UINT)+
				sizeof(CampID_t);
	}
}

UINT GetMailListSize( const MAIL_LIST& maillist )
{
	UINT uSize = sizeof(BYTE)+sizeof(BYTE) ;
	INT i=0; 
	for( i=0; i<maillist.m_Count; i++ )
	{
		uSize += GetMailSize(maillist.m_aMail[i]) ;
	}
	return uSize ;
}

CHAR* ReplaceIllegalString( CHAR* strText, INT nLength, INT nLevel )
{
	if( strText == NULL ) return NULL;
	INT i;
	for( i = 0; i < nLength; i ++ )
	{
		if( strText[i] == '%' ) strText[i] = '?';
		if( strText[i] == '\0' ) break;
	}

	return strText;
}

BOOL CheckIllegalString( const CHAR* strText, INT nLength, INT nLevel )
{
	if( strText == NULL ) return FALSE;
	INT i;
	if( nLevel == CHECK_STRING_NORMAL )
	{
		for( i = 0; i < nLength; i ++ )
		{
			if( strText[i] == '%' ) return TRUE;
			if( strText[i] == '\0' ) return FALSE;
		}
	}
	else if( nLevel == CHECK_STRING_CHARNAME )
	{
		for( i = 0; i < nLength; i ++ )
		{
			switch( strText[i] )
			{
			case ' ':
			case '#':
			case '\\':
			case '/':
			case '`':
			case '~':
			case '!':
			case '@':
			case '$':
			case '%':
			case '^':
			case '&':
			case '*':
			case '(':
			case ')':
			case '-':
			case '_':
			case '+':
			case '|':
			case '{':
			case '}':
			case '[':
			case ']':
			case ';':
			case ':':
			case '\'':
			case '\"':
			case '<':
			case '>':
			case ',':
			case '.':
			case '?':
			case '0xA1':
				return TRUE;
			}
			if( strText[i] == '\0' ) return FALSE;
		}
	}
	return FALSE;
}

#if defined (_FOX_CLIENT) && defined (_FOX_LOGIN)  && defined (_FOX_WORLD)
#define DUMP_LOG	"./Log/login_dump.log"
#endif

#if defined (_FOX_CLIENT) && defined (_FOX_SERVER) && defined (_FOX_WORLD)
#define DUMP_LOG	"./Log/server_dump.log"
#endif

#if defined (_FOX_SERVER) && defined (_FOX_WORLD) && defined(_FOX_LOGIN)
#define DUMP_LOG	"./Log/world_dump.log"
#endif

#if (!defined(_FOX_SERVER)) && (!defined (_FOX_WORLD)) && (!defined(_FOX_LOGIN))
#define DUMP_LOG	"./Log/shm_dump.log"
#endif

#if defined(_FOX_BILLING) && defined(_FOX_LOGIN) &&(!defined(_FOX_WORLD))
#define DUMP_LOG	"./Log/billing_dump.log"
#endif

VOID  DumpStack(const CHAR* type)
{
#ifdef __LINUX__
	void *	DumpArray[25];
	int	Size =	backtrace(DumpArray,25);
	char ** symbols = backtrace_symbols(DumpArray, Size);
	if(symbols)
	{
		if(Size>10) Size= 10;
		if(Size>0)
		{
			FILE* f = fopen( DUMP_LOG, "a" ) ;
			char threadinfo[256] = {0};
			sprintf(threadinfo,"threadid = %d cause dump\r\n",MyGetCurrentThreadID());
			fwrite(threadinfo,1,strlen(threadinfo),f);
			fwrite(type,1,strlen(type),f);
			for(int	i=0;i<Size;i++)
			{
				printf("%s\r\n",symbols[i]);
				fwrite(symbols[i],1,strlen(symbols[i]),f);
				fwrite("\r\n",1,2,f);
			}
			fclose(f) ;
		}
		free(symbols);
	}
	else
	{
		FILE* f = fopen( DUMP_LOG, "a" ) ;
		char	buffer[256] = {0};
		char threadinfo[256] = {0};
		sprintf(threadinfo,"threadid = %d cause dump\r\n",MyGetCurrentThreadID());
		fwrite(threadinfo,1,strlen(threadinfo),f);
		fwrite(type,1,strlen(type),f);
		fclose(f);
	}
#endif
}

