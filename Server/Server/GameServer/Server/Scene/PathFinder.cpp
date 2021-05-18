#include "stdafx.h"
//-----------------------------------------------------------------------------
// Game Server , Route Module
//-----------------------------------------------------------------------------
#include "PathFinder.h"

#ifndef __MAP_H__
#include "Map.h"
#endif

//这个宏对性能有影响
//#ifdef _DEBUG
#define CAN_GO_EDGE
//#endif

#define RECURETIMES 16 //这个值与MAX_CHAR_PATH_NODE_NUMBER一致
#define OFFSETZ 128


PathFinder::PathFinder(Map* owner,const CHAR* filename,uint& mx,uint& mz)
{
	__ENTER_FUNCTION
#ifdef _DEBUG
	strcpy(mFileName,filename);
	mOwner = owner;
#endif
    ReadNavMap(filename,mx,mz);

	InitEightDirections();

	__LEAVE_FUNCTION
}

//读取导航信息，文件由编辑器生成
VOID PathFinder::ReadNavMap(const CHAR* filename,uint& mx,uint& mz)
{
	__ENTER_FUNCTION

	FILE* fp = fopen(filename, "rb");
	if(NULL == fp) 
	{
		Assert(fp);
		return ;
	}

	_NAVMAP_HEAD head;
	fread(&head, sizeof(_NAVMAP_HEAD), 1, fp);

	mWidth = head.width;
	mHeight = head.height;
	mGridSize = 0.5;
	mInvGridSize = 1 / mGridSize;

	mx = (uint)(mWidth * mGridSize);
	mz = (uint)(mHeight * mGridSize);

	mLeftTopx = 0;
	mLeftTopz = 0;

	mMaxNode = mWidth * mHeight;

	mWorld = new _WORLD[mMaxNode];
	mWorkWorld = new _WORLD[mMaxNode];
	mNodes = new _NODES[mMaxNode + 1];

	INT size = sizeof(struct _NAVMAP_HEAD);
	for(INT j=0;j<mHeight;j++)
	{
		for(INT i=0;i<mWidth;i++)
		{
			INT info;
			fread(&info,size,1,fp);

			_WORLD* pWorld = mWorld + j * mWidth + i;
			pWorld->state = info;			
		}
	}

	mNodes[0].zx=0;
	mNodes[0].f=0;
	mNodes[0].g=0;

	curStep=0;

	__LEAVE_FUNCTION
}

//-----------------------------------------------------------------------------
/*
 *	              
 规律如下:
					5	2   6

					1	*   3			
                    
					4   0   7


					4 5 6 7		0 1 2 3		     
					6 7 4 5     2 3 0 1
 */
//-----------------------------------------------------------------------------
VOID PathFinder::InitEightDirections()
{
	__ENTER_FUNCTION

	INT n;
	for(n=0;n<4;n++)
	{
		DZX[n].costmultiplier = 10;
	}
	for(n=4;n<8;n++)
	{
		DZX[n].costmultiplier = 14;
	}

	DZX[0].zx = -mWidth;
	DZX[0].route = 2;

	DZX[1].zx = 1;
	DZX[1].route = 3;

	DZX[2].zx =mWidth;
	DZX[2].route = 0;

	DZX[3].zx = -1;
	DZX[3].route = 1;


	DZX[4].zx = -mWidth + 1;
	DZX[4].route = 6;

	DZX[5].zx = mWidth + 1;
	DZX[5].route = 7;

	DZX[6].zx = mWidth - 1;
	DZX[6].route = 4;

	DZX[7].zx = -mWidth - 1;
	DZX[7].route = 5;
	
	__LEAVE_FUNCTION
}

PathFinder::~PathFinder()
{
	__ENTER_FUNCTION
	
	SAFE_DELETE_ARRAY(mWorld);
	SAFE_DELETE_ARRAY(mWorkWorld);
	SAFE_DELETE_ARRAY(mNodes);

	__LEAVE_FUNCTION
}

VOID PathFinder::Reset(INT startz,INT endz)
{
	__ENTER_FUNCTION
	
	Assert( startz >= 0 && endz < mHeight);

	//256*256的mempcy大约花费0.040毫秒
	memcpy((VOID*)(mWorkWorld + (startz * mWidth)),(VOID*)(mWorld + (startz * mWidth)),(endz - startz) * mWidth * sizeof(_WORLD));
	//memcpy((VOID*)mWorkWorld,(VOID*)(mWorld),mMaxNode * sizeof(_WORLD));

	mBestFNode = 1;
	mNodes[mBestFNode].zx = mStartzx;
	mNodes[mBestFNode].g = 0;
	mNodes[mBestFNode].f = mNodes[mBestFNode].g + Distance(mStartzx);

	mWorkWorld[mStartzx].route = NO_ROUTE;
	
	mFreeNode = 1;
	
	mHeap[0] = EMPTY_NODE;
	mLastHeapLeaf = 1;
	mHeap[mLastHeapLeaf] = mBestFNode;	

	__LEAVE_FUNCTION
}

//-----------------------------------------------------------------------------
//二叉堆A*
//-----------------------------------------------------------------------------
VOID PathFinder::RemoveRootFromHeap()
{
	__ENTER_FUNCTION

	mHeap[ROOT_HEAP] = mHeap[mLastHeapLeaf--];
	
	INT k=ROOT_HEAP;
	while(NOTEMPTY_DOWN(k))
	{
		INT leftk=LEFT(k);
		INT rightk=RIGHT(k);
		INT bestk;
		if(NOTEMPTY_DOWN(leftk) && NOTEMPTY_DOWN(rightk) )
		{
			if(mNodes[mHeap[leftk]].f < mNodes[mHeap[rightk]].f)
				bestk=leftk;
			else
				bestk=rightk;
		}
		else if(NOTEMPTY_DOWN(leftk))
			bestk=leftk;
		else
			break;
		
		if(mNodes[mHeap[bestk]].f < mNodes[mHeap[k]].f)
		{
			SwapHeap(k,bestk);
			k=bestk;
		}
		else
			break;
	}
	__LEAVE_FUNCTION
}

VOID PathFinder::InsertNodeToHeap(WORD node)
{
	__ENTER_FUNCTION

	if(mLastHeapLeaf < MAX_HEAP_LEAFS)
		mLastHeapLeaf++;
	
	mHeap[mLastHeapLeaf] = node;
	
	INT k = mLastHeapLeaf;
	while(NOTEMPTY_UP(k))
	{
		INT parentk=PARENT(k);
		if(NOTEMPTY_UP(parentk))
		{
			if(mNodes[mHeap[k]].f < mNodes[mHeap[parentk]].f)
			{
				SwapHeap(k,parentk);
				k=parentk;
			}
			else
				break;
		}
		else
			break;
	}

	__LEAVE_FUNCTION
}

//-----------------------------------------------------------------------------
//辅助函数
//-----------------------------------------------------------------------------
inline INT PathFinder::LEFT(INT k)
{
	__ENTER_FUNCTION

	return k << 1;

	__LEAVE_FUNCTION
}
inline INT PathFinder::RIGHT(INT k)
{
	__ENTER_FUNCTION

	return (k << 1) + 1;

	__LEAVE_FUNCTION
}
inline INT PathFinder::PARENT(INT k)
{
	__ENTER_FUNCTION

	return (k >> 1);

	__LEAVE_FUNCTION
}
inline BOOL PathFinder::NOTEMPTY_UP(INT k)
{
	__ENTER_FUNCTION

	return k != 0;

	__LEAVE_FUNCTION
}
inline BOOL PathFinder::NOTEMPTY_DOWN(INT k)
{
	__ENTER_FUNCTION

	return k <= mLastHeapLeaf;

	__LEAVE_FUNCTION
}

inline VOID PathFinder::SwapHeap(const INT k1, const INT k2)
{
	__ENTER_FUNCTION

	WORD tmp = mHeap[k1];
	mHeap[k1] = mHeap[k2];
	mHeap[k2] = tmp;

	__LEAVE_FUNCTION
}

//-----------------------------------------------------------------------------
//导航图格子的大小目前暂定为0.5m
//寻路核心函数
//-----------------------------------------------------------------------------
BOOL PathFinder::FindPath(WORLD_POS* startPt,WORLD_POS* endPt,WORLD_POS* posNode,INT& numNode,INT nLevel,BOOL bLine,INT maxRoute)
{
__ENTER_FUNCTION	
	
	Assert( startPt->m_fX >= 0 && startPt->m_fX <mOwner->CX() && startPt->m_fZ >= 0 && startPt->m_fZ <mOwner->CZ() );
	Assert( endPt->m_fX >= 0 &&endPt->m_fX < mOwner->CX() && endPt->m_fZ >= 0 && endPt->m_fZ < mOwner->CZ() );

#ifdef _DEBUG
	if(startPt->m_fX >= 0 && startPt->m_fZ <mOwner->CZ() && endPt->m_fX >= 0 && endPt->m_fZ < mOwner->CZ() && 
		startPt->m_fZ >= 0 && startPt->m_fX <mOwner->CX() && endPt->m_fZ >= 0 && endPt->m_fX < mOwner->CX())
	{
		;
	}
	else
		return FALSE;
#endif

	mfStartX = startPt->m_fX;
	mfStartZ = startPt->m_fZ;
	mfEndX = endPt->m_fX;
	mfEndZ = endPt->m_fZ;
	
	//起始点在哪个格子上
	mStartx = (WORD) ( ( mfStartX - mLeftTopx ) * mInvGridSize);
	mStartz = (WORD) ( ( mfStartZ - mLeftTopz ) * mInvGridSize);
	//结束点在那个格子上
	mEndx = (WORD)( ( mfEndX - mLeftTopx ) * mInvGridSize);
	mEndz = (WORD)( ( mfEndZ - mLeftTopz ) * mInvGridSize);
	//起始点和结束点
	mStartzx = mStartz * mWidth + mStartx;
	mEndzx = mEndz * mWidth + mEndx;

	numNode = 0;
	
	mDistance = 0;
	//如果需要直线寻路
	if( bLine)
	{
		//if( mWorld[mStartzx].state != IMPASSABLE)
		if( IsCanGo(mWorld[mStartzx].state, nLevel) )
		{
			if(IsStraightLine(mfStartX,mfStartZ,mfEndX,mfEndZ,FALSE,nLevel))
			{
				posNode[numNode++] = WORLD_POS(mfEndX,mfEndZ);
				mDistance = RealDistance(mStartzx,mEndzx);
				return TRUE;
			}
			else
			{
				posNode[numNode++] = mFirstCanGoPos;
				mDistance = RealDistance(mStartzx,(WORD) ( ( mFirstCanGoPos.m_fZ - mLeftTopz ) * mInvGridSize) * mWidth + (WORD) ( ( mFirstCanGoPos.m_fX - mLeftTopx ) * mInvGridSize) );
				return TRUE;
			}
		}
		else
			return FALSE;
	}

	//做A*寻路
	//如果起始点和目标点皆不可行走
	//if(mWorld[mStartzx].state == IMPASSABLE || mWorld[mEndzx].state == IMPASSABLE)
	if( !IsCanGo(mWorld[mStartzx].state, nLevel) || !IsCanGo(mWorld[mEndzx].state, nLevel) )
	{
		return FALSE;
	}
	else
	{
		INT deltax = abs(mEndx - mStartx);
		INT deltaz = abs(mEndz - mStartz);

		//太远
		if( (deltax + deltaz) > maxRoute)
		{
			mDistance = RealDistance(mStartzx,(WORD) ( ( mFirstCanGoPos.m_fZ - mLeftTopz ) * mInvGridSize) * mWidth + (WORD) ( ( mFirstCanGoPos.m_fX - mLeftTopx ) * mInvGridSize) );
			return FALSE;
		}

		if(IsStraightLine(mfStartX,mfStartZ,mfEndX,mfEndZ,FALSE,nLevel))
		{
			posNode[numNode++] = WORLD_POS(mfEndX,mfEndZ);
			mDistance = RealDistance(mStartzx,mEndzx);
			return TRUE;
		}
		
		INT minz = min(mStartz,mEndz);
		INT maxz = max(mStartz,mEndz);
		minz = max(minz - OFFSETZ,0);
		maxz = min(maxz + OFFSETZ,mHeight - 1);

		Reset(minz,maxz);
		INT count = maxRoute;

		do
		{
			mBestFNode = mHeap[ROOT_HEAP];

			//关闭节点
			_NODES *pparent_node = mNodes + mBestFNode;

			//相等,则完成了寻路
			if(pparent_node->zx == mEndzx) 
			{				
				PackRoute(posNode,numNode,nLevel);
				return TRUE;
			}

			mWorkWorld[pparent_node->zx].state = CLOSED;
			RemoveRootFromHeap(); 

			
			//打开节点
			for( BYTE d = 0;d < EIGHT_DIRECTIONS ; d++)
			{
			//边界过滤,示意
			//运行版本,因地图的四周皆不可走,故而可以不用
			/*
			 *	
			 0     1      2      3                                           127
			 
		127	 128   129    130
			
			 256   257    258
			 */
			
#ifdef CAN_GO_EDGE //release版本不用
				INT zx = (INT)( pparent_node->zx);
				if( ( zx % mWidth == 0) && ( (d == 3) || (d == 6) || (d == 7) ) )
					continue;
				if( ( zx % mWidth == (mWidth -1) ) && ( ( d == 1) || ( d == 4 ) || (d == 5 ) ) )
					continue;

				zx += DZX[d].zx;
				if(zx < 0 || zx>= mWidth * mHeight)
					continue;

#else
				WORD zx = pparent_node->zx + DZX[d].zx;
#endif
				
				_WORLD *pworld = mWorkWorld + zx;

				if(pworld->state == UNKNOWN) 
				{
					pworld->state = OPEN; 
					pworld->route = d; 

					mFreeNode++; 

					_NODES *pfree_node = mNodes + mFreeNode;
					pfree_node->zx = zx;
					pfree_node->g = pparent_node->g + DZX[d].costmultiplier;
					pfree_node->f = pfree_node->g + Distance(zx);

					InsertNodeToHeap(mFreeNode);
				}
			}

			if(mLastHeapLeaf <= 0)
			{
				posNode[numNode++] = mFirstCanGoPos;
				mDistance = RealDistance(mStartzx,(WORD) ( ( mFirstCanGoPos.m_fZ - mLeftTopz ) * mInvGridSize) * mWidth + (WORD) ( ( mFirstCanGoPos.m_fX - mLeftTopx ) * mInvGridSize) );
				return TRUE;
			}
		} 
		while( --count>0 );
#ifdef _DEBUG
		count;
#endif
	}

__LEAVE_FUNCTION

	return FALSE;

}

inline WORD PathFinder::Distance(const UINT zx)
{
	__ENTER_FUNCTION
	//mWidth不能保证为2^
	return (WORD)( ( abs( (INT) (zx & (mWidth-1)) - (INT) mEndx ) + abs( (INT)(zx / mWidth) - (INT) mEndz ) ) * 10);

	__LEAVE_FUNCTION
}

inline WORD PathFinder::RealDistance(const UINT zxFirst,const UINT zxSecond)
{
	__ENTER_FUNCTION
		//mWidth不能保证为2^
		return (WORD)( ( abs( (INT) (zxFirst & (mWidth-1)) - (INT) (zxSecond & (mWidth-1)) ) + abs( (INT)(zxFirst / mWidth) - (INT) (zxSecond / mWidth) ) ));

	__LEAVE_FUNCTION
}

VOID PathFinder::PackRoute(WORLD_POS* posNode,INT& numNode, INT nLevel)
{		
	__ENTER_FUNCTION
	
	AIROUTE airoute;
	memset(airoute.route,0,MAX_ROUTES); 
	
	UINT zx = mEndzx;
	INT start = MAX_ROUTES - 1;
	BYTE route = NO_ROUTE;

	mGridNum = 0;
	//优化后代码	
	WORLD_POS firstPos;
	mGrids[mGridNum++] = zx;
	mDistance = 0;
	while(zx != mStartzx)
	{
		//反解
		route = mWorkWorld[zx].route;
		zx += DZX[DZX[route].route].zx;

		INT x = zx % mWidth;
		INT z = zx / mWidth;

		WORLD_POS goPos;
		goPos.m_fX = mLeftTopx + x * mGridSize + mGridSize/2;
		goPos.m_fZ = mLeftTopz + z * mGridSize + mGridSize/2;

		if(IsStraightLine(mfStartX,mfStartZ,goPos.m_fX,goPos.m_fZ,TRUE,nLevel))
		{
			firstPos.m_fX = mLeftTopx +  x * mGridSize + mGridSize/2;
			firstPos.m_fZ = mLeftTopz +  z * mGridSize + mGridSize/2;

			posNode[numNode++] = firstPos;
			mDistance += RealDistance(zx,mGrids[mGridNum-1]);
			break;
		}
		else
		{
			mGrids[mGridNum++] = zx;
			mDistance += RealDistance(zx,mGrids[mGridNum-1]);
		}
	}

	if(mGridNum)
	{
		mCallTimes = 0;
		curStep = mGridNum;

		EditAStarPathOpt(firstPos.m_fX,firstPos.m_fZ,posNode,numNode, nLevel);
	}

	__LEAVE_FUNCTION
}

//这个函数必须在FindPath后调用
INT PathFinder::GetDistance()
{
	return mDistance;
}

//这是3d意义上的直线判断
BOOL PathFinder::IsStraightLine(FLOAT mAStarBeginPtx,FLOAT mAStarBeginPty,FLOAT mAStarEndPtx,FLOAT mAStarEndPty,BOOL edit, INT nLevel)
{
__ENTER_FUNCTION

	INT intersecttime  = 0;

	WORLD_POS startPt(mAStarBeginPtx,mAStarBeginPty);
	WORLD_POS endPt(mAStarEndPtx,mAStarEndPty);
	
	INT xinc1,yinc1;

	//起始点和结束点肯定在内
	INT x1,y1,x2,y2;
	x1 = (INT)( ( startPt.m_fX - mLeftTopx ) * mInvGridSize);
	y1 = (INT)( ( startPt.m_fZ - mLeftTopz ) * mInvGridSize);
	x2 = (INT)( ( endPt.m_fX - mLeftTopx ) * mInvGridSize);
	y2 = (INT)( ( endPt.m_fZ - mLeftTopz ) * mInvGridSize);	

	FLOAT deltax,deltay;
	deltax = (endPt.m_fX-startPt.m_fX) * mInvGridSize;        
	deltay = (endPt.m_fZ-startPt.m_fZ) * mInvGridSize;

	mFirstCanGoPos = startPt;
	//斜率小于1
	if (fabs(deltax) >= fabs(deltay))
	{
		FLOAT slerp = deltay/deltax;

		if (x2 >= x1)                 
		{
			xinc1 = 1;

			for(INT x =  x1 + xinc1; x <= x2; x += xinc1)
			{
				INT y = (INT)( ( ( mLeftTopx + x * mGridSize - startPt.m_fX) * slerp + startPt.m_fZ - mLeftTopz) * mInvGridSize);

				INT index = x + y * mWidth; 
				
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state,nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;
				
				mFirstCanGoPos = WORLD_POS(mLeftTopx +  x * mGridSize + mGridSize/2,
                mLeftTopz +  y * mGridSize + mGridSize/2);

				index = x - xinc1 + y * mWidth;
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state, nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  ( x - xinc1) * mGridSize + mGridSize/2,
					mLeftTopz +  y * mGridSize + mGridSize/2);
			}
		}
		else                          
		{
			xinc1 = -1;
			for(INT x = x1;x >= x2 - xinc1;x += xinc1)
			{
				INT y =  (INT) ( ( startPt.m_fZ + slerp * ( (mLeftTopx + x*mGridSize) - startPt.m_fX) - mLeftTopz  ) * mInvGridSize);

				INT index = x + y * mWidth; 
				//if(mWorld[x + y * mWidth].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[x + y * mWidth].state, nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  x * mGridSize + mGridSize/2,
					mLeftTopz +  y * mGridSize + mGridSize/2);

				index = x + xinc1 + y * mWidth; 
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state,nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  ( x + xinc1) * mGridSize + mGridSize/2,
					mLeftTopz +  y * mGridSize + mGridSize/2);

			}
		}	
	}
	else//斜率大于1                          
	{
		FLOAT slerpInv = deltax/deltay;

		if (y2 >= y1)                 
		{
			yinc1 = 1;		
			for(INT y = y1 + yinc1;y <= y2;y +=yinc1)
			{
				INT x = (INT) ( ( (( mLeftTopz + y * mGridSize) - startPt.m_fZ) * slerpInv + startPt.m_fX - mLeftTopx) * mInvGridSize);
				INT index = x + y * mWidth; 
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state, nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  x * mGridSize + mGridSize/2,
					mLeftTopz +  y * mGridSize + mGridSize/2);

				index = x + (y - yinc1) * mWidth;
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state,nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  x * mGridSize + mGridSize/2,
					mLeftTopz +  (y - yinc1) * mGridSize + mGridSize/2);
			}
		}
		else                          
		{
			yinc1 = -1;
			for(INT y = y1;y >= y2 -yinc1;y += yinc1)
			{
				INT x =  (INT)( ( startPt.m_fX + slerpInv * ( (mLeftTopz + y*mGridSize) - startPt.m_fZ) - mLeftTopx  ) * mInvGridSize);

				INT index = x + y * mWidth; 
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state, nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  x * mGridSize + mGridSize/2,
					mLeftTopz +  y * mGridSize + mGridSize/2);

				index = x + (y + yinc1) * mWidth;
				//if(mWorld[index].state == IMPASSABLE && (edit ? intersecttime++ : 1 ) )
				if( !IsCanGo(mWorld[index].state, nLevel) && (edit ? intersecttime++ : 1 ) )
					return FALSE;

				mFirstCanGoPos = WORLD_POS(mLeftTopx +  x * mGridSize + mGridSize/2,
					mLeftTopz +  (y + yinc1) * mGridSize + mGridSize/2);
			}
		}


	}

__LEAVE_FUNCTION

	return TRUE;
}

VOID PathFinder::EditAStarPathOpt(FLOAT startPtx,FLOAT startPty,WORLD_POS* posNode,INT& numNode,INT nLevel)
{
	__ENTER_FUNCTION

	if(mCallTimes++ >= RECURETIMES)
		return;
	
	for(INT i=0; i<curStep; i++)
	{
		INT x = mGrids[i] % mWidth;
		INT y = mGrids[i] / mWidth;

		WORLD_POS goPos;
		if(i != 0)
		{
			goPos.m_fX = mLeftTopx + x * mGridSize + mGridSize/2;
			goPos.m_fZ = mLeftTopz +  y * mGridSize + mGridSize/2;
		}
		else
		{
			goPos = WORLD_POS(mfEndX,mfEndZ);
		}

		if(IsStraightLine(startPtx,startPty,goPos.m_fX,goPos.m_fZ,TRUE,nLevel))
		{
			posNode[ numNode++ ] = goPos;
			curStep = i;

			break;
		}
	}

	if(curStep)
	{
		INT x = mGrids[curStep] % mWidth;
		INT y = mGrids[curStep] / mWidth;

		WORLD_POS startPos;
		startPos.m_fX = mLeftTopx +  x * mGridSize + mGridSize/2;
		startPos.m_fZ = mLeftTopz +  y * mGridSize + mGridSize/2;

		EditAStarPathOpt(startPos.m_fX,startPos.m_fZ,posNode,numNode,nLevel);
	}
	else
	{
		numNode;
		numNode;
	}

	__LEAVE_FUNCTION
}

//这个函数并不递归
//VOID PathFinder::EditAStarPath(FLOAT startPtx,FLOAT startPty,WORLD_POS* posNode,INT& numNode)
//{	
//	__ENTER_FUNCTION
//
//	if(mCallTimes++ >= RECURETIMES)
//		return;
//	
//	INT curGrid = 0;
//
//	for(INT i= mGridNum - 1;i >= curStep; i--)
//	{
//		INT x = mGrids[i] % mWidth;
//		INT y = mGrids[i] / mWidth;
//
//		WORLD_POS goPos;
//		goPos.m_fX = mLeftTopx + x * mGridSize + mGridSize/2;
//		goPos.m_fZ = mLeftTopz +  y * mGridSize + mGridSize/2;
//
//		if(IsStraightLine(startPtx,startPty,goPos.m_fX,goPos.m_fZ,TRUE))
//		{
//			curStep = i;
//			posNode[ numNode++ ] = goPos;
//
//			curGrid = mGrids[i];			
//
//			break;
//		}
//	}
//
//	if(curGrid != mGrids[mGridNum-1])
//	{
//		INT x = curGrid % mWidth;
//		INT y = curGrid / mWidth;
//
//		WORLD_POS startPos;
//		startPos.m_fX = mLeftTopx +  x * mGridSize + mGridSize/2;
//		startPos.m_fZ = mLeftTopz +  y * mGridSize + mGridSize/2;
//
//		EditAStarPath(startPos.m_fX,startPos.m_fZ,posNode,numNode);
//	}
//
//	__LEAVE_FUNCTION
//}

BOOL PathFinder::IsCanGo(const WORLD_POS& pos, const INT nLevel)
{
__ENTER_FUNCTION
	
	INT gridx = (WORD) ( ( pos.m_fX - mLeftTopx ) * mInvGridSize);
	INT gridz = (WORD) ( ( pos.m_fZ - mLeftTopz ) * mInvGridSize);

	if ( (gridx >= 0 && gridx < mWidth) && (gridz >= 0 && gridz < mHeight) )
		//return mWorld[ gridz * mWidth + gridx].state != IMPASSABLE;
		return IsCanGo(mWorld[ gridz * mWidth + gridx].state,nLevel);

	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL PathFinder::IsCanGo( const INT state, const INT nDriverLevel )
{
	return nDriverLevel >= state;
}
