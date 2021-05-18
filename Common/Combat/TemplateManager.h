///////////////////////////////////////////////////////////////////////////////
// 文件名：TemplateManager.h
// 功能说明：数据表的通用管理器和读入器
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file TemplateManager.h
*		\brief 技能相关数据管理器的模板
*/
#ifndef TEMPLATEMANAGER_H
#define TEMPLATEMANAGER_H

#include "Type.h"
#include "TLBB_DBC.h"

namespace DBC
{
	class DBCFile;
}
using namespace DBC;

namespace Combat_Module
{
	namespace Skill_Module
	{
		template<typename U>
			class TemplateManager_T
		{
			public:
				enum
				{
					MAXSIZE = U::MAX_ID+1,
				};
				TemplateManager_T();
				~TemplateManager_T();
				BOOL InitFromDB(CHAR const* szFile);
				BOOL InitFromDBMemory(const DBCFile* pDB);
				U const * const GetInstanceByID(INT const nID)const;
				INT	Size(VOID) const {return m_nSize;};
			protected:
			private:
				BOOL m_bReady;
				INT m_nSize;
				U m_vTemplateList[MAXSIZE];
				DBCFile* m_pDB;
		};
		template<typename U>
		inline TemplateManager_T<U>::TemplateManager_T()
			: m_bReady(FALSE), m_nSize(0), m_pDB(NULL)
		{
		}
		template<typename U>
		inline TemplateManager_T<U>::~TemplateManager_T()
		{
			SAFE_DELETE(m_pDB);
		}
		template<typename U>
			inline BOOL TemplateManager_T<U>::InitFromDBMemory(const DBCFile* pDB)
		{
			__ENTER_FUNCTION
			m_nSize = 0;
			m_bReady = FALSE;

			if(NULL == pDB)
			{
				return FALSE;
			}

			m_nSize = pDB->GetRecordsNum();
			//init List
			//read data
			for(INT i=0; m_nSize >i; ++i)
			{
				INT nID = pDB->Search_Posistion(i,0)->iValue;
				if(0>nID||MAXSIZE<nID)
				{
					AssertEx(FALSE, "[TemplateManager_T<U>::InitFromDB: nID out of range!!abort input.");
					continue;
				}
				m_vTemplateList[nID].InitFromDB(pDB, i);
			}
			m_bReady = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}

		template<typename U>
			inline BOOL TemplateManager_T<U>::InitFromDB(CHAR const* szFile)
		{
			__ENTER_FUNCTION
				m_nSize = 0;
			m_bReady = FALSE;

			if(NULL == szFile)
			{
				return FALSE;
			}
			if (NULL != m_pDB)
			{
				SAFE_DELETE(m_pDB);
			}
			m_pDB = new DBCFile(0);
			if(NULL == m_pDB)
			{
				return FALSE;
			}
			if(FALSE == m_pDB->OpenFromTXT(szFile))
			{
				return FALSE;
			}
			m_nSize = m_pDB->GetRecordsNum();
			//init List
			//read data
			for(INT i=0; m_nSize >i; ++i)
			{
				INT nID = m_pDB->Search_Posistion(i,0)->iValue;
				if(0>nID||MAXSIZE<=nID)
				{
					AssertEx(FALSE, "[TemplateManager_T<U>::InitFromDB: nID out of range!!abort input.");
					continue;
				}
				m_vTemplateList[nID].InitFromDB(m_pDB, i);
			}
			m_bReady = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}

		template<typename U>
		inline U const* const TemplateManager_T<U>::GetInstanceByID(INT const nID)const
		{
			__ENTER_FUNCTION
			if(0>nID || MAXSIZE <=nID)
			{
				return NULL;
			}
			if (0 == m_nSize)
			{
				return NULL;
			}
			if (FALSE == m_bReady)
			{
				return NULL;
			}
			if(FALSE == m_vTemplateList[nID].IsInited())
			{
				return NULL;
			}
			return &m_vTemplateList[nID];
			__LEAVE_FUNCTION
			return NULL;
		}
	}
}
#endif //TEMPLATEMGR_H
