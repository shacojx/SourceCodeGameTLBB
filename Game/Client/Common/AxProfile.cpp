#include "AxProfile.h"
#include <windows.h>
#include <string>
#include <list>
#include <hash_map>
#include <assert.h>

//////////////////////////////////////////
//
//#define ENABLE_AXPROFILE
//
//////////////////////////////////////////


#define HashMap ::stdext::hash_map

namespace AxProfile
{

////////////////////////////////////////////////////////////////

//效率节点
struct NodeDefine
{
	STRING			strName;
	NodeDefine		*pParent;
};

//节点存储结构
typedef HashMap< STRING, NodeDefine >	NodeDefineMap;

//一次采样中的节点
struct NodeInSampling
{
	NodeDefine*			pNodeDefine;		//在Buf中的指针
	LARGE_INTEGER		nStartTime;			//开始时间
	LARGE_INTEGER		nEndTime;			//结束时间
};

//采样节点存储结构
typedef HashMap< NodeDefine*, NodeInSampling > NodeInSamplingMap;

//一次采样
struct Sampling
{
	bool					bInvalid;			//是否有效
												/*
												| 以下几种情况本次采样为无效;
												|	1. 某节点没有被封闭;
												|
												|
												*/
	LARGE_INTEGER			nStartTime;			//开始时间
	LARGE_INTEGER			nEndTime;			//结束时间
	NodeInSamplingMap		mapSamplingNode;	//采集的效率节点
};

//采样存贮结构
typedef std::list< Sampling >	SamplingList;


////////////////////////////////////////////////////////////////

//所有的效率节点列表
NodeDefineMap		g_AllNode;
//采样数据存储
SamplingList		g_SamplingList;
//当前的采样
Sampling*			g_pCurrentSampling = NULL;

////////////////////////////////////////////////////////////////

//关闭当前节点
void	_TryCloseCurrentSampling(void);

////////////////////////////////////////////////////////////////
void AxProfile_SamplingBegin(void)
{
#ifdef ENABLE_AXPROFILE
	//当前时间
	LARGE_INTEGER tNow;
	::QueryPerformanceCounter(&tNow);

	//关闭当前采样
	if(g_pCurrentSampling) _TryCloseCurrentSampling();

	//开始新采样
	Sampling newSampling;
	newSampling.bInvalid = false;
	newSampling.nStartTime = tNow;
	newSampling.nEndTime.QuadPart = 0;

	g_SamplingList.push_back(newSampling);

	//设置当前采样
	g_pCurrentSampling = &(*(g_SamplingList.rbegin()));
#endif
}

void AxProfile_SamplingEnd(void)
{
#ifdef ENABLE_AXPROFILE
	//关闭当前采样
	if(g_pCurrentSampling) _TryCloseCurrentSampling();
#endif
}

void AxProfile_PushNode(const char* szName, const char* szParent)
{
#ifdef ENABLE_AXPROFILE
	//检测是否是无效的名字
	if(!szName || szName[0] == '\0') 
	{
		assert(false && "AxProfile_PushNode: Invalid Node Name!");
		return;
	}

	//当前尚未开始采样
	if(!g_pCurrentSampling)
	{
		assert(false && "AxProfile_PushNode: Must in the Pair of StartSampling and EndSampling!");
		return;
	}

	//当前时间
	LARGE_INTEGER tNow;
	::QueryPerformanceCounter(&tNow);

	//搜索已有节点
	NodeDefineMap::iterator itFind = g_AllNode.find(szName);
	if(itFind == g_AllNode.end())
	{
		//创建新节点定义
		NodeDefine newNode;
		newNode.strName = szName;

		//搜索父节点
		if(!szParent || szParent[0] == '\0')
		{
			newNode.pParent = NULL;
		}
		else
		{
			NodeDefineMap::iterator itParent = g_AllNode.find(szName);
			if(itParent == g_AllNode.end())
			{
				assert(false && "AxProfile_PushNode: Invalid Parent Name!");
				return;
			}

			newNode.pParent = &(itParent->second);
		}

		//加入定义
		g_AllNode.insert(std::make_pair(newNode.strName, newNode));

		itFind = g_AllNode.find(szName);
	}

	//寻找该节点是否在采样中
	NodeInSamplingMap::iterator itNodeInSampling = 
		g_pCurrentSampling->mapSamplingNode.find(&(itFind->second));

	if(itNodeInSampling == g_pCurrentSampling->mapSamplingNode.end())
	{
		//创建新节点
		NodeInSampling newNode;
		newNode.nStartTime = tNow;
		newNode.nEndTime.QuadPart = 0;
		newNode.pNodeDefine = &(itFind->second);

		g_pCurrentSampling->mapSamplingNode.insert(std::make_pair(newNode.pNodeDefine, newNode));
	}	
	else
	{
		//对于已经正在采样中的节点，不再重复计时
		//...
	}
#endif
}

void AxProfile_PopNode(const char* szName)
{
#ifdef ENABLE_AXPROFILE
	//检测是否是无效的名字
	if(!szName || szName[0] == '\0') 
	{
		assert(false && "AxProfile_PopNode: Invalid Node Name!");
		return;
	}

	//当前尚未开始采样
	if(!g_pCurrentSampling)
	{
		assert(false && "AxProfile_PopNode: Must in the Pair of StartSampling and EndSampling!");
		return;
	}

	//当前时间
	LARGE_INTEGER tNow;
	::QueryPerformanceCounter(&tNow);

	//搜索已有节点
	NodeDefineMap::iterator itFind = g_AllNode.find(szName);
	if(itFind == g_AllNode.end())
	{
		//没有找到节点定义
		assert(false && "AxProfile_PopNode: Muse Push Node First!");
		return;
	}

	//寻找该节点是否在采样中
	NodeInSamplingMap::iterator itNodeInSampling = 
		g_pCurrentSampling->mapSamplingNode.find(&(itFind->second));
	if(itNodeInSampling == g_pCurrentSampling->mapSamplingNode.end())
	{
		//没有找到节点定义
		assert(false && "AxProfile_PopNode: Muse Push Node First!");
		return;
	}

	//设定停止时间
	itNodeInSampling->second.nEndTime = tNow;

	//是否达到最大限制
	if(g_SamplingList.size() >= 1000)
	{
		g_SamplingList.erase(g_SamplingList.begin());
	}
#endif
}

////////////////////////////////////////////////////////////////
void _TryCloseCurrentSampling(void)
{
#ifdef ENABLE_AXPROFILE

	if(!g_pCurrentSampling) return;

	//当前时间
	LARGE_INTEGER tNow;
	::QueryPerformanceCounter(&tNow);

	g_pCurrentSampling->nEndTime = tNow;

	//检查是否所有效率点已经封闭
	register HashMap< NodeDefine*, NodeInSampling >::iterator it = 
		g_pCurrentSampling->mapSamplingNode.begin();
	HashMap< NodeDefine*, NodeInSampling >::iterator itEnd = g_pCurrentSampling->mapSamplingNode.end();

	g_pCurrentSampling->bInvalid = true;
	for(;it!=itEnd; ++it)
	{
		register NodeInSampling& theNode = it->second;

		if(theNode.nEndTime.QuadPart == 0)
		{
			//有节点未封闭
			g_pCurrentSampling->bInvalid = false;
			break;
		}
	}

	//清空当前节点
	g_pCurrentSampling = NULL;
#endif
}

void AxProfile_Report(void)
{
#ifdef ENABLE_AXPROFILE
	FILE* fp = fopen("d:\\out.txt", "w");
	if(!fp) return;

	LARGE_INTEGER gPer;
	QueryPerformanceFrequency(&gPer);

	//printf title
	fprintf(fp, "#\t_TOTAL_\t");
	NodeDefineMap::iterator itNodeDefine = g_AllNode.begin();
	for(;itNodeDefine!=g_AllNode.end(); itNodeDefine++)
	{
		fprintf(fp, "%s\t", itNodeDefine->second.strName.c_str());
	}
	fprintf(fp, "\n");

	int nIndex=0;
	std::list< Sampling >::iterator itSampling = g_SamplingList.begin();
	for(;itSampling!=g_SamplingList.end(); itSampling++)
	{
		fprintf(fp, "%d\t", nIndex);
		Sampling& theSampling = *itSampling;
		if(!theSampling.bInvalid) continue;

		//printf total
		fprintf(fp, "%f\t", 
			(theSampling.nEndTime.QuadPart-theSampling.nStartTime.QuadPart)*1000.0/gPer.QuadPart);

		itNodeDefine = g_AllNode.begin();
		for(;itNodeDefine!=g_AllNode.end(); itNodeDefine++)
		{
			NodeDefine* pNodeDefine = &(itNodeDefine->second);
			
			NodeInSamplingMap::iterator itNodeInSampling = theSampling.mapSamplingNode.find(pNodeDefine);
			if(itNodeInSampling == theSampling.mapSamplingNode.end())
			{
				fprintf(fp, "0\t");
			}
			else
			{
				NodeInSampling& theSamplingNode = itNodeInSampling->second;
                
				fprintf(fp, "%f\t", 
					(theSamplingNode.nEndTime.QuadPart-theSamplingNode.nStartTime.QuadPart)*1000.0/gPer.QuadPart);
			}
		}

		fprintf(fp, "\n");

		nIndex++;
	}

	fclose(fp);


	g_SamplingList.clear();
	g_pCurrentSampling = 0;
#endif
}

}