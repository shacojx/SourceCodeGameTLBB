/***********************************\
*									*
*		宠物征友板接口				*
*		2006-03-10					*
*									*
\***********************************/
#include "Type.h"
#include "../DataPool/GMDP_Struct_Pet.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

struct _PET_DETAIL_ATTRIB;
struct SDATA_PET;
struct _PET_PLACARD_ITEM;
class CObject_PlayerNPC;

namespace SCRIPT_SANDBOX
{
	class PetInviteFriend
	{
	public:
		struct PET_FRIEND
		{
			SDATA_PET			m_Pet;
			CObject_PlayerNPC*	m_Avatar;

			PET_FRIEND()
			{
				m_Avatar = NULL;
			}

			virtual ~PET_FRIEND()
			{
				m_Pet.CleanUp();
				m_Avatar = NULL;
			}
		};

	//LuaPlus界面相关接口
	public:
		//获得客户端的征友信息的数量
		INT GetInviteNum(LuaPlus::LuaState* state);

		//获得征友宠主的详细信息（名称、等级之类）
		INT GetHumanINFO(LuaPlus::LuaState* state);

		//获得宠主对宠物的介绍信息
		INT GetInviteMsg(LuaPlus::LuaState* state);

		//设置界面上显示的模型
		INT	SetPetModel(LuaPlus::LuaState* state);

		//显示相应宠物的详细信息，需要调用TargetPet界面来显示
		INT	ShowTargetPet(LuaPlus::LuaState* state);

		//获得宠物板宠物的信息
		INT	GetPetINFO(LuaPlus::LuaState* state);

	//网络处理相关接口
	public:
		VOID							ConvertPlaceCard2PetFriend();

	//公用函数接口
	public:
		VOID							PET_DETAIL_ATTRIB2SDATA_PAT(const _PET_DETAIL_ATTRIB* pIn, SDATA_PET* pOut);
		VOID							InitPetList();
		VOID							CleanUpPetList();
		VOID							DestoryPetList();
		VOID							SetPetListFakeObj(INT idx, INT rdx);	//idx 设置位置， rdx 模型编号
		SDATA_PET*						GetPetListData(INT idx);
		const _PET_PLACARD_ITEM*		GetPlaceCardItem(INT idx);

	//数据区，用以存放转换回来的数据
	protected:
		std::vector< PET_FRIEND* >		m_PetList;

	public:
		PetInviteFriend();
		virtual ~PetInviteFriend();

		static PetInviteFriend s_PetInviteFriend;
		static LuaPlus::LuaObject* s_pMetaTable;
	};
};