/************************************************************************* 
 文件名	    : AvoidOverlap.h 	
 版本号 	: 0.0.1
 功  能		: 挂在Human或Pet身上用来避免怪物很多时Obj的重叠问题
 修改记录	: 
*************************************************************************/
#ifndef _AVOID_OVERLAP_
#define _AVOID_OVERLAP_

#include "Type.h"
#include "AI_Character.h"
#include "GameUtil.h"

class AvoidOverlap
{
public:
	enum 
	{
		MAX_FRACTION = 16,
		MAX_MOVETIMES = 4,
		MAX_USEDTIMES = 255,
	};
	
	AvoidOverlap( VOID )
	{
		memset( m_isUsed, 0, sizeof(BYTE)*MAX_FRACTION );
		InitCosSin( );
	}
	
	virtual ~AvoidOverlap( VOID ) {}

	/** Desc				:得到怪物攻击玩家的目标坐标
	 *  @param rTar			:目标坐标
	 *  @param rMonsterPos	:怪物坐标
	 *  @param rPlayerPos	:玩家坐标
	 *  @param fMaxDist		:怪物最大攻击距离
	 *  @param fMinDist		:怪物最小攻击距离
	 *  @return				:
	 */
	VOID GetDirPos( WORLD_POS& rTar, 
					const WORLD_POS& rMonsterPos, 
					const WORLD_POS& rPlayerPos,
					FLOAT fMaxDist,
					FLOAT fMinDist )
	{
	__ENTER_FUNCTION
		/** 用怪物与玩家的位置确定要定位的范围 如L1～L2之间的方向 M到L1和L2之间的角度都是PI/4  */
		/**
		 *        L1|  /M
		 *			| /
		 *		   P|/________L2
		 */
		rTar = rPlayerPos; // 这样确保目标点是一个有效的点
		INT nStartIndex = 0,nEndIndex = 0,nIndex = 0;
		FLOAT vX = rPlayerPos.m_fX - rMonsterPos.m_fX;
		FLOAT vZ = rPlayerPos.m_fZ - rMonsterPos.m_fZ;
		FLOAT fDist = MySqrt( &rMonsterPos, &rPlayerPos );
		if (fDist < 0.001)
		{
			return;
		}
		FLOAT fCos = vX / fDist;
		FLOAT fSin = vZ / fDist;
		/** 确定开始,结束位置 */
		FLOAT fRet = ::asin( fSin );
		if ( fRet > 0 && vX < 0 )
		{
			fRet += __PI/2;
		}
		else if ( fRet < 0 && vX < 0 )
		{
			fRet -= __PI/2;
		}

		if ( fRet < 0 )
		{
			fRet += 2 * __PI;
		}

		FLOAT fStart = fRet - __PI / 4;
		fStart = fStart > 0 ? fStart : ( 2 * __PI + fStart );
		FLOAT fEnd = fRet + __PI / 4;
		fEnd = fEnd > 2 * __PI ? ( fEnd - 2*__PI ) : fEnd;
		nStartIndex = GetIndex( fStart );
		nEndIndex = GetIndex( fEnd );
		INT nUsedCount = 0;
		/** 必然是nStartIndex在第四相像，nEnd在第一相像 */
		nIndex = GetBestPos( nStartIndex, nEndIndex, nUsedCount );
		if (FALSE == HaveMonsterNearly(nIndex))
		{// 如果附近没有monster则原地不动
			rTar = rMonsterPos;
			return;
		}
		/** 如果改__PI/2范围内怪物超过3个 则偏移__PI/2重新筛选 直到找到最合理的或是轮寻满一圈 */
		if ( MAX_FRACTION / 4 - 2 < nUsedCount )
		{
			/** 避免所以怪物向一边倒的相像 */
			INT nDir = ( rand() % 2 ) > 0 ? 4 : -4 ;
			for ( INT i = 0; MAX_MOVETIMES > i; ++i )
			{
				nUsedCount = 0;
				nStartIndex += nDir;
				nStartIndex = ( nStartIndex + MAX_FRACTION ) % MAX_FRACTION;
				nEndIndex += nDir;
				nEndIndex = ( nEndIndex + MAX_FRACTION ) % MAX_FRACTION;
				
				nIndex = GetBestPos( nStartIndex, nEndIndex, nUsedCount );
				if ( MAX_FRACTION / 4 - 2 > nUsedCount )
				{
					break;
				}
			}
		}

		/** 确定与玩家的距离,距离为最大距离与最小距离之间的任意值 */
		rTar.m_fX = rPlayerPos.m_fX + fMinDist * GetCos( nIndex );
		rTar.m_fZ = rPlayerPos.m_fZ + fMinDist * GetSin( nIndex );
		
	__LEAVE_FUNCTION
	}
	
	VOID ResetUsedDir( VOID )
	{
		memset( m_isUsed, 0, MAX_FRACTION );
	}

protected:
	INT GetMinIndex( INT nStart, INT nEnd, INT& nMinIndex, INT& nUsedCount )
	{
	__ENTER_FUNCTION
		Assert( MAX_FRACTION > nStart && 0 <= nStart && MAX_FRACTION > nEnd && 0 <= nEnd );
		/** 奇偶分开增加效果 */
		for ( INT i = nStart; i <= nEnd; )
		{
			if ( m_isUsed[nMinIndex] > m_isUsed[i] ) 
			{
				nMinIndex = i;
            }
			if( m_isUsed[i] )
			{
				++nUsedCount;
			}
			i += 2;
		}
		for (INT i = nStart+1; i <= nEnd; )
		{
			if ( m_isUsed[nMinIndex] > m_isUsed[i] ) 
			{
				nMinIndex = i;
			}
			if ( m_isUsed[i] )
			{
				++nUsedCount;
			}
			i += 2;
		}

		++m_isUsed[nMinIndex];

		if ( MAX_USEDTIMES == m_isUsed[nMinIndex] ) 
		{
			m_isUsed[nMinIndex] = 0;
		}

		return nMinIndex;
	__LEAVE_FUNCTION
		return nMinIndex;
	}

	INT GetBestPos( INT nStartIndex, INT nEndIndex, INT& nUsedCount ) 
	{
	__ENTER_FUNCTION
		INT nIndex = 0;
		/** 找到最优的方向 */
		INT nMinIndex = nStartIndex;
		INT nStepLen = ( nEndIndex - nStartIndex ) > 0 ? ( nEndIndex - nStartIndex ) / 2 : ( nEndIndex - nStartIndex + MAX_FRACTION ) / 2;
		nMinIndex += nStepLen;
		if (nMinIndex >= MAX_FRACTION)
		{// 有效性检测
			nMinIndex -= MAX_FRACTION; 
		}
		if ( nStartIndex > nEndIndex )
		{
		/** nStartIndex必然落在第四象相 */
			INT Min1 = GetMinIndex( 0, nEndIndex, nMinIndex, nUsedCount );
			INT Min2 = GetMinIndex( nStartIndex, MAX_FRACTION - 1, nMinIndex, nUsedCount );
			if (m_isUsed[Min1] > m_isUsed[Min2])
			{
				nIndex = Min2;
				--m_isUsed[Min1];
			}
			else
			{
				nIndex = Min1;
				--m_isUsed[Min2];
			}
		}
		else 
		{
			nIndex = GetMinIndex( nStartIndex, nEndIndex, nMinIndex, nUsedCount );
		}
		return nIndex;
	__LEAVE_FUNCTION
		return 0;
	}

	INT GetPosIfNoMonster( FLOAT fvalue ) 
	{
	__ENTER_FUNCTION
		for ( INT i = 0; MAX_FRACTION > i; ++i ) 
		{
			if ( 2 * i * __PI / MAX_FRACTION >= fvalue ) 
			{
				FLOAT fStep1 = fabs( 2 * i * __PI / MAX_FRACTION - fvalue );
				INT j = i-1 < 0 ? MAX_FRACTION-1 : i-1;
				FLOAT fStep2 = fabs( 2  * j * __PI / MAX_FRACTION - fvalue );
				INT m = i+1 > MAX_FRACTION-1 ? MAX_FRACTION-i-1 : i+1;
				INT n = j-1 < 0 ? MAX_FRACTION-1 : j-1;
				if( 0 == m_isUsed[i] 
					&& 0 == m_isUsed[j]
					&& 0 == m_isUsed[m]
					&& 0 == m_isUsed[n] )
				{
					return fStep1 > fStep2 ? j : i;
				}
				else
				{
					return -1;
				}
			}
		}
		return -1 ;
	__LEAVE_FUNCTION
		return -1 ;
	}
	// 有效性检测
	VOID VerifyIndex(INT& nIndex)
	{
	__ENTER_FUNCTION
		if (nIndex >= MAX_FRACTION)
		{
			nIndex -= MAX_FRACTION; 
		}
		if (nIndex < 0)
		{
			nIndex += MAX_FRACTION;
		}
	__LEAVE_FUNCTION
	}

	BOOL HaveMonsterNearly(INT nIndex)
	{
	__ENTER_FUNCTION
		// 对nIndex附近的三个(包括nIndex)进行检测
		INT nTempIndex = nIndex-1;
		VerifyIndex(nTempIndex);
		if (m_isUsed[nTempIndex] > 0)
			return TRUE;

		nTempIndex = nIndex;
		VerifyIndex(nTempIndex);
		// 因为在此函数被调用以前已经被累加过了
		if (m_isUsed[nTempIndex] > 1)
			return TRUE;

		nTempIndex = nIndex+1;
		VerifyIndex(nTempIndex);
		if (m_isUsed[nTempIndex] > 0)
			return TRUE;

		return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
	}

	VOID InitCosSin( VOID ) 
	{
		for ( INT i = 0; MAX_FRACTION > i; ++i )
		{
			m_fSin[i] = sin( i * 2 * __PI / MAX_FRACTION );
			m_fCos[i] = sin( i * 2 * __PI / MAX_FRACTION + __HALF_PI);
		}
	}

	FLOAT GetCos( INT index ) const
	{
		if (index < 0 || index >= MAX_FRACTION)
		{
			Assert(NULL && "AvoidOverlap...GetCos()...index=invalid...");
			return 0.f;
		}
		return m_fCos[index];
	}

	FLOAT GetSin( INT index ) const
	{
		if (index < 0 || index >= MAX_FRACTION)
		{
			Assert(NULL && "AvoidOverlap...GetSin()...index=invalid...");
			return 0.f;
		}
		return m_fSin[index];
	}

	INT GetIndex( FLOAT fValue )
	{
		INT i;
		for(i = 0; MAX_FRACTION > i; ++i )
		{
			if ( i * 2 * __PI / MAX_FRACTION >= fValue ) 
			{
				return i;			
			}
		}
		if ( MAX_FRACTION == i )
		{
			return 0;
		}

		return -1;
	}

private:
	BYTE  m_isUsed[MAX_FRACTION];
	FLOAT m_fCos[MAX_FRACTION];
	FLOAT m_fSin[MAX_FRACTION];

};

#endif