#include "stdafx.h"
#include "Type.h"
#include "GameStruct.h"

#include "SocketInputStream.h"
#include "SocketOutputStream.h"


BYTE		GetSerialType(UINT Serial)
{

	return (BYTE)((Serial%100000)/1000);
}


BYTE		GetSerialClass(UINT Serial)
{
	return (BYTE)(Serial/10000000);
}
BYTE		GetSerialQual(UINT Serial)
{
	return	(BYTE)((Serial%10000000)/100000);
}
UINT		GetSerialIndex(UINT Serial)
{
	return (Serial%1000);
}

_ITEM_TYPE	ConvertSerial2ItemType(UINT Serial)
{
	_ITEM_TYPE it;
	it.m_Class		=	GetSerialClass(Serial);
	it.m_Quality	=	GetSerialQual(Serial);
	it.m_Type		=	GetSerialType(Serial);
	it.m_Index		=	GetSerialIndex(Serial);

	return it;
}

//获得物品有效数据大小
UINT	GetItemRealSize(const _ITEM& it)
{
	INT CreatorSize = it.GetCreatorVar()?(sizeof(CHAR)*MAX_ITEM_CREATOR_NAME):0;
	if(it.ItemClass()==ICLASS_EQUIP)
	{
		
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			//////////////////////////////////////////////////////////////////////////
			sizeof(SHORT)*2		+
			sizeof(UINT)		+
			sizeof(_ITEM_ATTR)*it.m_Equip.m_AttrCount	+
			sizeof(_ITEM_GEMINFO)*it.m_Equip.m_StoneCount	+
			sizeof(BYTE)*9		+
			CreatorSize;

	}
	else if(it.ItemClass()==ICLASS_GEM)
	{
		return	sizeof(_ITEM_GUID)	+
				sizeof(UINT)		+
				sizeof(CHAR)		+
				sizeof(CHAR)		+
				sizeof(INT)*MAX_ITEM_PARAM+
				CreatorSize			+
				sizeof(GEM_INFO);
	}
	else if(it.ItemClass()==ICLASS_COMITEM)
	{
		
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			CreatorSize			+
			sizeof(MEDIC_INFO);
	}
	else if(it.ItemClass()==ICLASS_MATERIAL)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			CreatorSize			+
			sizeof(MEDIC_INFO);
	}
	else if(it.ItemClass()==ICLASS_TASKITEM)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			CreatorSize			+
			sizeof(MEDIC_INFO);
	}
	else if(it.ItemClass()==ICLASS_STOREMAP)
	{
		return 	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			CreatorSize			+
			sizeof(STORE_MAP_INFO);

	}
	else
		return	sizeof(_ITEM_GUID)	+
				sizeof(UINT)		+
				sizeof(CHAR)		+
				sizeof(INT)*MAX_ITEM_PARAM+
				CreatorSize			+
				sizeof(CHAR);
}


VOID	SwapItem(_ITEM* pItemA,_ITEM* pItemB)
{
	Assert(pItemA);
	Assert(pItemB);

	_ITEM tempItem;
	memcpy(&tempItem,pItemA,sizeof(_ITEM));
	memcpy(pItemA,pItemB,sizeof(_ITEM));
	memcpy(pItemB,&tempItem,sizeof(_ITEM));
}

VOID	OverWriteItem(_ITEM* pItemA,_ITEM* pItemB)
{
	Assert(pItemA);
	Assert(pItemB);

	memcpy(pItemB,pItemA,sizeof(_ITEM));
	memset(pItemA,0,sizeof(_ITEM));
}


VOID	_ITEM::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_ItemGUID),sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)(&m_ItemIndex),sizeof(UINT));
	iStream.Read((CHAR*)(&m_RulerID),sizeof(CHAR));
	iStream.Read((CHAR*)(&m_nsBind),sizeof(CHAR));
	iStream.Read((CHAR*)(m_Param),sizeof(INT)*MAX_ITEM_PARAM);

	if(GetCreatorVar())
	{
		iStream.Read(m_Creator,sizeof(CHAR)*MAX_ITEM_CREATOR_NAME);
	}
	if(ItemClass()==ICLASS_EQUIP)
	{
		iStream.Read((CHAR*)(&GetEquipData()->m_CurDurPoint),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_CurDamagePoint),sizeof(USHORT));
		iStream.Read((CHAR*)(&GetEquipData()->m_MaxDurPoint),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_BasePrice),sizeof(UINT));
		iStream.Read((CHAR*)(&GetEquipData()->m_EquipPoint),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_AttrCount),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_SetNum),sizeof(SHORT));
		iStream.Read((CHAR*)(&GetEquipData()->m_MaxNum),sizeof(CHAR));
		if(GetEquipData()->m_AttrCount>MAX_ITEM_ATTR)		
			GetEquipData()->m_AttrCount	= MAX_ITEM_ATTR;

		for(INT j = 0;j<GetEquipData()->m_AttrCount;j++)
		{
			iStream.Read((CHAR*)(&GetEquipData()->m_pAttr[j]),sizeof(_ITEM_ATTR));
		}

		iStream.Read((CHAR*)(&GetEquipData()->m_StoneCount),sizeof(BYTE));

		if(GetEquipData()->m_StoneCount>MAX_ITEM_GEM)		
			GetEquipData()->m_StoneCount	= MAX_ITEM_GEM;

		for(INT j = 0;j<GetEquipData()->m_StoneCount;j++)
		{
			iStream.Read((CHAR*)(&GetEquipData()->m_pGemInfo[j]),sizeof(_ITEM_GEMINFO));
		}

		iStream.Read((CHAR*)(&GetEquipData()->m_NeedLevel),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_GemMax),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_FaileTimes),sizeof(BYTE));
	}
	else if(ItemClass()==ICLASS_GEM)
	{
		iStream.Read((CHAR*)(GetGemData()),sizeof(GEM_INFO));
	}
	else if(ItemClass()==ICLASS_COMITEM)
	{
		iStream.Read((CHAR*)(GetMedicData()),sizeof(MEDIC_INFO));
	}
	else if(ItemClass()==ICLASS_MATERIAL)
	{
		iStream.Read((CHAR*)(GetMedicData()),sizeof(MEDIC_INFO));
	}
	else if(ItemClass()==ICLASS_TASKITEM)
	{
		iStream.Read((CHAR*)(GetMedicData()),sizeof(MEDIC_INFO));
	}
	else if(ItemClass()==ICLASS_STOREMAP)
	{
		iStream.Read((CHAR*)(GetStoreMapData()),sizeof(STORE_MAP_INFO));
	}
}

VOID	_ITEM::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_ItemGUID),sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)(&m_ItemIndex),sizeof(UINT));
	oStream.Write((CHAR*)(&m_RulerID),sizeof(CHAR));	
	oStream.Write((CHAR*)(&m_nsBind),sizeof(CHAR));
	oStream.Write((CHAR*)(m_Param),sizeof(INT)*MAX_ITEM_PARAM);

	if(GetCreatorVar())
	{
		oStream.Write(m_Creator,sizeof(CHAR)*MAX_ITEM_CREATOR_NAME);
	}

	BYTE bClass = ItemClass();

	if(bClass==ICLASS_EQUIP)
	{
		oStream.Write((CHAR*)(&GetEquipData()->m_CurDurPoint),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_CurDamagePoint),sizeof(USHORT));
		oStream.Write((CHAR*)(&GetEquipData()->m_MaxDurPoint),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_BasePrice),sizeof(UINT));
		oStream.Write((CHAR*)(&GetEquipData()->m_EquipPoint),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_AttrCount),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_SetNum),sizeof(SHORT));
		oStream.Write((CHAR*)(&GetEquipData()->m_MaxNum),sizeof(BYTE));

		for(INT j = 0;j<GetEquipData()->m_AttrCount;j++)
		{
			oStream.Write((CHAR*)(&GetEquipData()->m_pAttr[j]),sizeof(_ITEM_ATTR));
		}

		oStream.Write((CHAR*)(&GetEquipData()->m_StoneCount),sizeof(BYTE));

		for(INT j = 0;j<GetEquipData()->m_StoneCount;j++)
		{
			oStream.Write((CHAR*)(&GetEquipData()->m_pGemInfo[j]),sizeof(_ITEM_GEMINFO));
		}

		oStream.Write((CHAR*)(&GetEquipData()->m_NeedLevel),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_GemMax),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_FaileTimes),sizeof(BYTE));
	}
	else if(bClass==ICLASS_GEM)
	{	
		oStream.Write((CHAR*)(GetGemData()),sizeof(GEM_INFO));
	}
	else if(bClass	== ICLASS_COMITEM)
	{
		oStream.Write((CHAR*)(GetMedicData()),sizeof(MEDIC_INFO));
	}
	else if(bClass == ICLASS_MATERIAL)
	{
		oStream.Write((CHAR*)(GetMedicData()),sizeof(MEDIC_INFO));
	}
	else if(bClass == ICLASS_TASKITEM)
	{
		oStream.Write((CHAR*)(GetMedicData()),sizeof(MEDIC_INFO));
	}
	else if(bClass	== ICLASS_STOREMAP)
	{
		oStream.Write((CHAR*)(GetStoreMapData()),sizeof(STORE_MAP_INFO));
	}
}
BOOL	_ITEM::IsFixAttr(BYTE attrType)
{
	switch(attrType) 
	{

	case IATTRIBUTE_BASE_ATTACK_P:
	case IATTRIBUTE_BASE_ATTACK_M:
	case IATTRIBUTE_BASE_ATTACK_TIME:
	case IATTRIBUTE_BASE_DEFENCE_P:
	case IATTRIBUTE_BASE_DEFENCE_M:
	case IATTRIBUTE_BASE_MISS:
		{
			return TRUE;
		}
		break;
	default:
		return FALSE;
	}
}

BYTE	_ITEM::GetFixAttrCount()
{
	INT iAttrCount =	GetEquipData()->m_AttrCount - GetEquipData()->m_StoneCount;
	
	INT i = 0;
	INT FixAttrCount	=	0;
	for(; i<iAttrCount;i++)
	{
		 if( IsFixAttr(GetEquipData()->m_pAttr[i].m_AttrType)
			 && GetEquipData()->m_pAttr[i].m_Value.m_Value>0)
			 FixAttrCount++;
		 
	 }
	 return  FixAttrCount;

}

BOOL	_ITEM::GetItemBind() const
{
	if( m_nsBind & IEI_BIND_INFO)
		return TRUE;
	return FALSE;

	
}
VOID	_ITEM::SetItemBind(BOOL bBind)
{
	if(bBind)
		m_nsBind |= IEI_BIND_INFO;
	else
		m_nsBind &= (~(IEI_BIND_INFO));//这个操作一般不会执行
}

BOOL	_ITEM::GetItemIdent() const
{
	if( m_nsBind & IEI_IDEN_INFO)
		return TRUE;
	return FALSE;
}
VOID	_ITEM::SetItemIdent(BOOL bIdent)
{
	if(bIdent)
		m_nsBind |= IEI_IDEN_INFO;
	else
		m_nsBind &= (~(IEI_IDEN_INFO));//这个操作一般不会执行
}

BOOL	_ITEM::GetCreatorVar()	const
{
	if( m_nsBind & IEL_CREATOR)
		return TRUE;
	return FALSE;
}

VOID	_ITEM::SetCreatorVar(BOOL bCreator)	
{
	if(bCreator)
		m_nsBind |= IEL_CREATOR;
	else
		m_nsBind &= (~(IEL_CREATOR));//这个操作一般不会执行
}

BOOL	_ITEM::GetItemPLock() const
{
	if( m_nsBind & IEI_PLOCK_INFO)
		return TRUE;
	return FALSE;
}
VOID	_ITEM::SetItemPLock(BOOL bLock)
{
	if(bLock)
		m_nsBind |= IEI_PLOCK_INFO;
	else
		m_nsBind &= (~(IEI_PLOCK_INFO));//这个操作一般不会执行
}

INT		_ITEM::GetItemFailTimes() const
{
	return m_Equip.m_FaileTimes;
}
VOID	_ITEM::SetItemFailTimes(INT nTimes)
{
	m_Equip.m_FaileTimes = nTimes;
}

INT		_ITEM::GetItemParamValue(UINT Start,const ItemParamValue& ipv)
{
	INT TotalSize = sizeof(INT)*MAX_ITEM_PARAM;
	INT nSize;
	switch(ipv)
	{
	case IPV_CHAR:
		{
			nSize = 1;
			if( Start > ((UINT)TotalSize - (UINT)nSize) )
			{
				Assert(FALSE);
			}
			return *((CHAR*)m_Param+Start);
		}
		break;
	case IPV_SHORT:
		{
			nSize = 2;
			if( Start > ( (UINT)TotalSize - (UINT)nSize ) )
			{
				Assert(FALSE);
			}
			return *((SHORT*)((CHAR*)m_Param+Start));
		}
		break;
	case IPV_INT:
		{
			nSize = 4;
			if( Start > ( (UINT)TotalSize - (UINT)nSize ) )
			{
				Assert(FALSE);
			}
			return *((INT*)((CHAR*)m_Param+Start));
		}
		break;
	default:
		Assert(FALSE);
	}

	return -1;
}

VOID	_ITEM::SetItemParam(UINT start,ItemParamValue& ipv,INT value)
{
	INT TotalSize = sizeof(INT)*MAX_ITEM_PARAM;
	INT size;

	switch(ipv)
	{
	case IPV_CHAR:
		{
			size = 1;
			if( start > ( (UINT)TotalSize - (UINT)size ) )
			{
				Assert(FALSE);
			}
			*((CHAR*)m_Param+start) = (CHAR)value;
		}
		break;
	case IPV_SHORT:
		{
			size = 2;
			if(start>((UINT)TotalSize - (UINT)size))
			{
				Assert(FALSE);
			}
			*((SHORT*)((CHAR*)m_Param+start)) = (SHORT)value;
		}
		break;
	case IPV_INT:
		{
			size = 4;
			if(start>((UINT)TotalSize - (UINT)size))
			{
				Assert(FALSE);
			}
			*((INT*)((CHAR*)m_Param+start)) = (INT)value;
		}
		break;
	default:
		Assert(FALSE);
	}
}


BOOL	_ITEM::WriteFixAttr(CHAR* pIn,INT BuffLength)
{
	Assert(pIn);
	
	BYTE bClass = ItemClass();
	
	CHAR*	pBuff = pIn;
	m_RulerID	= *pBuff;
	pBuff++;
	
	BYTE	FixAttrCount = *((BYTE*)pBuff);
	pBuff+= sizeof(BYTE);
	
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			
			
			GetEquipData()->m_SetNum	=	*((SHORT*)pBuff);					//套装编号
			pBuff		+=	sizeof(SHORT);

			GetEquipData()->m_BasePrice	=	*((UINT*)pBuff);					//售出价格
			pBuff		+=	sizeof(UINT);

			GetEquipData()->m_MaxNum	=	*pBuff;								//套装
			pBuff		+=   sizeof(CHAR);


			GetEquipData()->m_EquipPoint	=	*((BYTE*)pBuff);				//物品装佩点
			pBuff		+=	sizeof(BYTE);

			GetEquipData()->m_MaxDurPoint	=	*((BYTE*)pBuff);				//最大耐久值
			pBuff		+=	sizeof(BYTE);

			GetEquipData()->m_NeedLevel =	*((BYTE*)pBuff);					//需求等级
			pBuff		+=	sizeof(BYTE);

			//GetEquipData()->m_GemMax	=	*((BYTE*)pBuff);					//最大宝石个数
			//pBuff		+=	sizeof(BYTE);

			//固定属性在前面
            for(INT i = 0 ; i<FixAttrCount;i++)
			{
				GetEquipData()->m_pAttr[i].m_AttrType	=	*((BYTE*)pBuff);
				pBuff	+= sizeof(BYTE);

				GetEquipData()->m_pAttr[i].m_Value.m_Value	=	*((SHORT*)pBuff);
				pBuff	+= sizeof(SHORT);
			}

			GetEquipData()->m_AttrCount	=	FixAttrCount; //先保存FixAttrCount;

		}
		break;
	case ICLASS_GEM:
		{
			GetGemData()->m_nPrice	=	*((INT*)pBuff);
			pBuff				+=	sizeof(INT);

			GetGemData()->m_Attr.m_AttrType	=	*((BYTE*)pBuff);	
			pBuff				+= sizeof(BYTE);

			GetGemData()->m_Attr.m_Value.m_Value = *((SHORT*)pBuff) ;
			pBuff				+= sizeof(SHORT);


		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
		{
			GetMedicData()->m_bCosSelf   =		*((BOOL*)pBuff);
			pBuff					    +=		sizeof(BOOL);
			GetMedicData()->m_nBasePrice =		*((UINT*)pBuff);
			pBuff					    +=		sizeof(UINT);
			GetMedicData()->m_nLayedNum =		*((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_nLevel	=		*((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_nReqSkill	=		*((INT*)pBuff);
			pBuff						+=		sizeof(INT);
			GetMedicData()->m_nReqSkillLevel	= *((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_nScriptID	=		*((INT*)pBuff);
			pBuff						+=		sizeof(INT);
			GetMedicData()->m_nSkillID	=		*((INT*)pBuff);
			pBuff						+=		sizeof(INT);
			GetMedicData()->m_TargetType =		*((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
		}
		break;
	case ICLASS_STOREMAP:
		{
			GetStoreMapData()->m_GrowType	= *((INT*)pBuff);	
			pBuff						+= sizeof(INT);
			GetStoreMapData()->m_nLevel	= *((INT*)pBuff);
			pBuff						+= sizeof(INT);
			GetStoreMapData()->m_SceneID= *((INT*)pBuff);
			pBuff						+= sizeof(INT);
			GetStoreMapData()->m_xPos	= *((FLOAT*)pBuff);
			pBuff						+= sizeof(FLOAT);
			GetStoreMapData()->m_zPos	= *((FLOAT*)pBuff);
			pBuff						+= sizeof(FLOAT);
		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}

	Assert(BuffLength>=(pBuff-pIn));

	return TRUE;
}

BOOL	_ITEM::ReadFixAttr(CHAR* pOut,INT& OutLength,INT BuffLength)
{
	Assert(pOut);

	BYTE bClass = ItemClass();

	CHAR*	pBuff = pOut;
	*pBuff	=	m_RulerID;
	pBuff++;

	BYTE	FixAttrCount = GetFixAttrCount();

	*((BYTE*)pBuff)	= FixAttrCount;
	pBuff++;
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{


			*((SHORT*)pBuff)  =	GetEquipData()->m_SetNum;					//套装编号
			pBuff		+=	sizeof(SHORT);
			*((UINT*)pBuff) = GetEquipData()->m_BasePrice;				//售出价格
			pBuff		+=	sizeof(UINT);
			*pBuff			=	GetEquipData()->m_MaxNum;					//套装
			pBuff		+=   sizeof(CHAR);
			*((BYTE*)pBuff)  = GetEquipData()->m_EquipPoint;				//物品装佩点
			pBuff		+=	sizeof(BYTE);
			*((BYTE*)pBuff)  = GetEquipData()->m_MaxDurPoint;				//最大耐久值
			pBuff		+=	sizeof(BYTE);
			*((BYTE*)pBuff)  = GetEquipData()->m_NeedLevel;					//需求等级
			pBuff		+=	sizeof(BYTE);
			//*((BYTE*)pBuff)  =	GetEquipData()->m_GemMax;					//最大宝石个数
			//pBuff		+=	sizeof(BYTE);

			//固定属性在前面
			for(INT i = 0 ; i<FixAttrCount;i++)
			{
				*((BYTE*)pBuff)	=	GetEquipData()->m_pAttr[i].m_AttrType;
				pBuff	+= sizeof(BYTE);

				*((SHORT*)pBuff)	=	GetEquipData()->m_pAttr[i].m_Value.m_Value	;
				pBuff	+= sizeof(SHORT);
			}

		}
		break;
	case ICLASS_GEM:
		{
			*((INT*)pBuff)   =  GetGemData()->m_nPrice	;
			pBuff				+=	sizeof(INT);
			*((BYTE*)pBuff)	 =  GetGemData()->m_Attr.m_AttrType;	
			pBuff				+= sizeof(BYTE);
			*((SHORT*)pBuff) =	GetGemData()->m_Attr.m_Value.m_Value ;
			pBuff				+= sizeof(SHORT);


		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
		{
			*((BOOL*)pBuff)   = GetMedicData()->m_bCosSelf;
			pBuff					    +=	sizeof(BOOL);
			*((UINT*)pBuff) =	GetMedicData()->m_nBasePrice;
			pBuff					    +=	sizeof(UINT);
			*((BYTE*)pBuff)	=	GetMedicData()->m_nLayedNum;
			pBuff						+=  sizeof(BYTE);
			*((BYTE*)pBuff)	=	GetMedicData()->m_nLevel;
			pBuff						+=  sizeof(BYTE);
			*((INT*)pBuff)	=	GetMedicData()->m_nReqSkill;
			pBuff						+=  sizeof(INT);
			*((BYTE*)pBuff)	=	GetMedicData()->m_nReqSkillLevel;
			pBuff						+=  sizeof(BYTE);
			*((INT*)pBuff)	=	GetMedicData()->m_nScriptID;
			pBuff						+= sizeof(INT);
			*((INT*)pBuff)	=	GetMedicData()->m_nSkillID;
			pBuff						+= sizeof(INT);
			*((BYTE*)pBuff)	=	GetMedicData()->m_TargetType;
			pBuff						+= sizeof(BYTE);
		}
		break;
	case ICLASS_STOREMAP:
		{
			*((INT*)pBuff)	=	GetStoreMapData()->m_GrowType;	
			pBuff						+= sizeof(INT);
			*((INT*)pBuff)	=	GetStoreMapData()->m_nLevel;
			pBuff						+= sizeof(INT);
			*((INT*)pBuff)	=	GetStoreMapData()->m_SceneID;
			pBuff						+= sizeof(INT);
			*((FLOAT*)pBuff)	=	GetStoreMapData()->m_xPos;
			pBuff						+= sizeof(FLOAT);
			*((FLOAT*)pBuff)	=	GetStoreMapData()->m_zPos;
			pBuff						+= sizeof(FLOAT);
		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}
	OutLength = (INT)(pBuff-pOut);
	Assert(BuffLength>=OutLength);

	return TRUE;
}

//一定要先序列化固定属性
//CHAR* pIn 实际上是整数序列
BOOL	_ITEM::WriteVarAttr(CHAR* pIn,INT BuffLength)
{
	Assert(pIn);

	BYTE bClass = ItemClass();

	CHAR*	pBuff = pIn;
	m_nsBind	=	*pBuff;
	pBuff++;
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			GetEquipData()->m_CurDurPoint	=	*((BYTE*)pBuff);
			pBuff	+= sizeof(BYTE);
			GetEquipData()->m_FaileTimes	=	*((BYTE*)pBuff);
			pBuff	+= sizeof(BYTE);
			
			BYTE FixAttrCount = GetEquipData()->m_AttrCount;
			GetEquipData()->m_AttrCount	=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(BYTE);
			//第一个整数完毕

			Assert(GetEquipData()->m_AttrCount<=MAX_ITEM_ATTR);
			//每个随机属性一个INT
			for(INT i = FixAttrCount; i<GetEquipData()->m_AttrCount;i++)
			{
				GetEquipData()->m_pAttr[i].m_AttrType	=	(BYTE)(*((SHORT*)pBuff));	//类型占一个Short
				pBuff	+= sizeof(SHORT);

				GetEquipData()->m_pAttr[i].m_Value.m_Value	=	*((SHORT*)pBuff);//类型值占一个Short
				pBuff	+= sizeof(SHORT);
			}
			
			GetEquipData()->m_StoneCount	=	(BYTE)(*((BYTE*)pBuff)); //宝石个数一个SHORT
			pBuff	+= sizeof(SHORT);

			GetEquipData()->m_GemMax		=	(BYTE)(*((BYTE*)pBuff)); //宝石个数一个SHORT
			pBuff	+= sizeof(BYTE);

			GetEquipData()->m_CurDamagePoint	=	*((USHORT*)pBuff); //损伤度一个SHORT
			pBuff	+= sizeof(USHORT);

			Assert(GetEquipData()->m_StoneCount<=MAX_ITEM_GEM);
			
			//每个宝石类型一个INT
			for( int i = 0;i<MAX_ITEM_GEM;i++)
			{
				GetEquipData()->m_pGemInfo[i].m_GemType	=	*((UINT*)pBuff);
				pBuff += sizeof(UINT);
			}

		}
		break;
	case ICLASS_GEM:
		{
			
		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
		{
			GetMedicData()->m_nCount = *((BYTE*)pBuff);
		}
		break;
	case ICLASS_STOREMAP:
		{

		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}

	Assert(BuffLength>=(pBuff-pIn));

	return TRUE;
}

BOOL	_ITEM::ReadVarAttr(CHAR* pOut,INT& OutLength,INT BuffLength)
{
	Assert(pOut);

	BYTE bClass = ItemClass();

	CHAR*	pBuff = pOut;
	*pBuff	=	m_nsBind;
	pBuff++;
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			*((BYTE*)pBuff)	=	GetEquipData()->m_CurDurPoint;
			pBuff	+= sizeof(BYTE);

			*((BYTE*)pBuff)	=	GetEquipData()->m_FaileTimes;
			pBuff	+= sizeof(BYTE);

			*((BYTE*)pBuff)	=	GetEquipData()->m_AttrCount;
			pBuff	+=	sizeof(BYTE);
			
			BYTE FixAttrCount = GetFixAttrCount();

			Assert(GetEquipData()->m_AttrCount<=MAX_ITEM_ATTR);
			for(INT i = FixAttrCount; i<GetEquipData()->m_AttrCount;i++)
			{
				*((SHORT*)pBuff)	=	GetEquipData()->m_pAttr[i].m_AttrType;
				pBuff	+= sizeof(SHORT);

				*((SHORT*)pBuff)	=	GetEquipData()->m_pAttr[i].m_Value.m_Value;
				pBuff	+= sizeof(SHORT);
			}
			
			*((SHORT*)pBuff)	=	GetEquipData()->m_StoneCount;
			pBuff	+= sizeof(SHORT);

			*((BYTE*)pBuff)	=	GetEquipData()->m_GemMax;
			pBuff	+= sizeof(BYTE);

			*((USHORT*)pBuff)	=	GetEquipData()->m_CurDamagePoint;
			pBuff	+= sizeof(USHORT);

			Assert(GetEquipData()->m_StoneCount<=MAX_ITEM_GEM);

			for( int i = 0;i<MAX_ITEM_GEM;i++)
			{
				*((UINT*)pBuff)	=	GetEquipData()->m_pGemInfo[i].m_GemType;
				pBuff += sizeof(UINT);
			}

		}
		break;
	case ICLASS_GEM:
		{

		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
		{
			*((BYTE*)pBuff)	=	GetMedicData()->m_nCount;
			pBuff	+= sizeof(BYTE);
		}
		break;
	case ICLASS_STOREMAP:
		{

		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}

	OutLength = (INT)(pBuff-pOut);
	Assert(BuffLength>=OutLength);
	return TRUE;
}


BYTE	_ITEM::GetItemCount() const
{
	BYTE bClass = ItemClass();

	if(bClass==ICLASS_EQUIP)
	{
		return 1;

	}
	else if(bClass==ICLASS_GEM)
	{
		return 1;
	}
	else if(bClass==ICLASS_COMITEM)
	{

		return GetMedicData()->m_nCount;	
	}
	else if(bClass==ICLASS_MATERIAL)
	{

		return GetMedicData()->m_nCount;	
	}
	else if(bClass == ICLASS_TASKITEM)
	{
		return GetMedicData()->m_nCount;	
	}
	else
		return 0;
}

VOID	_ITEM::SetItemCount(INT nCount)	
{
	UCHAR bClass = ItemClass();

	if(bClass==ICLASS_COMITEM)
	{
		GetMedicData()->m_nCount	=	nCount;	
	}
	else if(bClass==ICLASS_MATERIAL)
	{

		GetMedicData()->m_nCount	=	nCount;	
	}
	else if(bClass==ICLASS_TASKITEM)
	{
		GetMedicData()->m_nCount	=	nCount;	
	}
	else
	{
		Assert(FALSE);
		return;
	}
}

BYTE	_ITEM::GetItemTileMax() const
{
	BYTE	bClass = ItemClass();


	switch(ItemClass()) 
	{
	case ICLASS_EQUIP:
		return 1;
		break;
	case ICLASS_GEM:
		return 1;
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
		{
			return GetMedicData()->m_nLayedNum;
		}
		break;
	case ICLASS_STOREMAP:
		{
			return 1;
		}
	default:
		{
			AssertEx(FALSE,"not implement class");
			return 0;
		}
	}
}




//是否达到单叠最大个数
BOOL	_ITEM::isFullTile() const
{
	return GetItemCount()>=GetItemTileMax();
}

//增加个数
BOOL	_ITEM::IncCount(UINT nCount /* =1 */)
{
	
	const	BYTE	bClass = ItemClass();

	switch(bClass) {
	case ICLASS_EQUIP:
		{
			return FALSE;
		}
		break;
	case ICLASS_GEM:
		{
			return FALSE;
		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
		{
			Assert(!isFullTile());
			GetMedicData()->m_nCount+=nCount;	
			return TRUE;
		}
		break;
	default:
		return FALSE;
		break;
	}
	
	
}

//减少个数
BOOL	_ITEM::DecCount(UINT nCount /* =1 */)
{
	const	BYTE	bClass = ItemClass();
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			return FALSE;
		}
		break;
	case ICLASS_GEM:
		{
			return FALSE;
		}
		break;
	case ICLASS_TASKITEM:
	case ICLASS_MATERIAL:
	case ICLASS_COMITEM:
		{
			Assert(GetItemCount()>0);
			GetMedicData()->m_nCount-=nCount;	
			return TRUE;
         }
		break;
	default:
		return FALSE;
		break;
	}
}

BOOL Item2String(_ITEM* pItem,CHAR* pOut,INT& OutLength,INT BuffLength)
{

		Assert(pItem);
		Assert(pOut);
		
		CHAR*	pBuff = pOut;
		*((UINT*)pBuff)	=		pItem->m_ItemIndex;
		pBuff+=sizeof(UINT);
		pItem->ReadVarAttr(pBuff,OutLength,BuffLength-sizeof(UINT));
		
		OutLength	+=	sizeof(UINT);
		CHAR		p[255];
		memset(p,0,255);
		Binary2String(pOut,OutLength,p);
		strcpy(pOut,p);
		OutLength = (INT)strlen(p);
		return TRUE;
}

BOOL String2Item(CHAR* pIn,INT BuffLength,_ITEM* pItem)
{
	Assert(pItem);
	Assert(pIn);
	
	
	CHAR	p[255];
	memset(p,0,255);
	UINT	iOut;
	String2Binary(pIn,BuffLength,p,255,iOut);
	CHAR*	pBuff = p;
	pItem->m_ItemIndex =*((UINT*)pBuff);
	pBuff+=	sizeof(UINT);
	pItem->WriteVarAttr(pBuff,iOut);
	return TRUE;
}


VOID GUILD_CGW_ASKLIST::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_SortType), sizeof(m_SortType));
	iStream.Read((CHAR*)(&m_uStart), sizeof(m_uStart));
	iStream.Read((CHAR*)(&m_Camp), sizeof(m_Camp));
}

VOID GUILD_CGW_ASKLIST::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_SortType), sizeof(m_SortType));
	oStream.Write((CHAR*)(&m_uStart), sizeof(m_uStart));
	oStream.Write((CHAR*)(&m_Camp), sizeof(m_Camp));
}

VOID GUILD_CGW_CREATE::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_NameSize), sizeof(m_NameSize));

	if ( m_NameSize > 0 )
	{
		iStream.Read((CHAR*)m_szGuildName, m_NameSize);
	}

	iStream.Read((CHAR*)(&m_DescSize), sizeof(m_DescSize));

	if ( m_DescSize > 0 )
	{
		iStream.Read((CHAR*)m_szGuildDesc, m_DescSize);
	}

	iStream.Read((CHAR*)(&m_Camp), sizeof(m_Camp));
}

VOID GUILD_CGW_CREATE::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_NameSize), sizeof(m_NameSize));

	if ( m_NameSize > 0 )
	{
		oStream.Write((CHAR*)m_szGuildName, m_NameSize);
	}

	oStream.Write((CHAR*)(&m_DescSize), sizeof(m_DescSize));

	if ( m_DescSize > 0 )
	{
		oStream.Write((CHAR*)m_szGuildDesc, m_DescSize);
	}

	oStream.Write((CHAR*)(&m_Camp), sizeof(m_Camp));
}

VOID GUILD_CGW_JOIN::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_Camp), sizeof(m_Camp));
}

VOID GUILD_CGW_JOIN::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_Camp), sizeof(m_Camp));
}

VOID GUILD_CGW_ASKINFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_Type), sizeof(m_Type));
}

VOID GUILD_CGW_ASKINFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_Type), sizeof(m_Type));
}

VOID GUILD_CGW_APPOINT::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	iStream.Read((CHAR*)(&m_NewPosition), sizeof(m_NewPosition));
}

VOID GUILD_CGW_APPOINT::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	oStream.Write((CHAR*)(&m_NewPosition), sizeof(m_NewPosition));
}

VOID GUILD_CGW_ADJUSTAUTH::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	iStream.Read((CHAR*)(&m_NewAuthority), sizeof(m_NewAuthority));
}

VOID GUILD_CGW_ADJUSTAUTH::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	oStream.Write((CHAR*)(&m_NewAuthority), sizeof(m_NewAuthority));
}

VOID GUILD_CGW_RECRUIT::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_ProposerGUID), sizeof(m_ProposerGUID));
}

VOID GUILD_CGW_RECRUIT::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_ProposerGUID), sizeof(m_ProposerGUID));
}

VOID GUILD_CGW_EXPEL::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildUserGUID), sizeof(m_GuildUserGUID));
}

VOID GUILD_CGW_EXPEL::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildUserGUID), sizeof(m_GuildUserGUID));
}

VOID GUILD_CGW_WITHDRAW::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_WITHDRAW::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_DEPOSIT::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_DEPOSIT::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_CHANG_DESC::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildDescLength), sizeof(BYTE));
	if(m_GuildDescLength>0)
	{
		iStream.Read((CHAR*)(m_GuildDesc), m_GuildDescLength*sizeof(CHAR));
	}
}

VOID GUILD_CGW_CHANG_DESC::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildDescLength), sizeof(BYTE));
	if(m_GuildDescLength>0)
	{
		oStream.Write((CHAR*)(m_GuildDesc), m_GuildDescLength*sizeof(CHAR));
	}
}

VOID GUILD_WGC_APPOINT_INFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&iPosNum), sizeof(iPosNum));
	iStream.Read((CHAR*)(m_PosList), sizeof(s)*iPosNum);
}

VOID GUILD_WGC_APPOINT_INFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&iPosNum), sizeof(iPosNum));
	oStream.Write((CHAR*)(m_PosList), sizeof(s)*iPosNum);
}

VOID GUILD_WGC_GUILD_INFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);
	iStream.Read((CHAR*)(m_GuildCreator), MAX_CHARACTER_NAME);
	iStream.Read((CHAR*)(m_GuildChairMan), MAX_CHARACTER_NAME);
	iStream.Read((CHAR*)(m_CityName), MAX_CITY_NAME_SIZE);
	iStream.Read((CHAR*)(&m_nLevel), sizeof(m_nLevel));
	iStream.Read((CHAR*)(&m_nPortSceneID), sizeof(m_nPortSceneID));
	iStream.Read((CHAR*)(&m_MemNum), sizeof(m_MemNum));
	iStream.Read((CHAR*)(&m_Longevity), sizeof(m_Longevity));
	iStream.Read((CHAR*)(&m_Contribute), sizeof(m_Contribute));
	iStream.Read((CHAR*)(&m_Honor), sizeof(m_Honor));
	iStream.Read((CHAR*)(&m_FoundedMoney), sizeof(m_FoundedMoney));
	iStream.Read((CHAR*)(&m_nIndustryLevel), sizeof(m_nIndustryLevel));
	iStream.Read((CHAR*)(&m_nAgrLevel), sizeof(m_nAgrLevel));
	iStream.Read((CHAR*)(&m_nComLevel), sizeof(m_nComLevel));
	iStream.Read((CHAR*)(&m_nDefLevel), sizeof(m_nDefLevel));
	iStream.Read((CHAR*)(&m_nTechLevel), sizeof(m_nTechLevel));
	iStream.Read((CHAR*)(&m_nAmbiLevel), sizeof(m_nAmbiLevel));
	iStream.Read((CHAR*)(&m_bAccess), sizeof(m_bAccess));
}

VOID GUILD_WGC_GUILD_INFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);
	oStream.Write((CHAR*)(m_GuildCreator), MAX_CHARACTER_NAME);
	oStream.Write((CHAR*)(m_GuildChairMan), MAX_CHARACTER_NAME);
	oStream.Write((CHAR*)(m_CityName), MAX_CITY_NAME_SIZE);
	oStream.Write((CHAR*)(&m_nLevel), sizeof(m_nLevel));
	oStream.Write((CHAR*)(&m_nPortSceneID), sizeof(m_nPortSceneID));
	oStream.Write((CHAR*)(&m_MemNum), sizeof(m_MemNum));
	oStream.Write((CHAR*)(&m_Longevity), sizeof(m_Longevity));
	oStream.Write((CHAR*)(&m_Contribute), sizeof(m_Contribute));
	oStream.Write((CHAR*)(&m_Honor), sizeof(m_Honor));
	oStream.Write((CHAR*)(&m_FoundedMoney), sizeof(m_FoundedMoney));
	oStream.Write((CHAR*)(&m_nIndustryLevel), sizeof(m_nIndustryLevel));
	oStream.Write((CHAR*)(&m_nAgrLevel), sizeof(m_nAgrLevel));
	oStream.Write((CHAR*)(&m_nComLevel), sizeof(m_nComLevel));
	oStream.Write((CHAR*)(&m_nDefLevel), sizeof(m_nDefLevel));
	oStream.Write((CHAR*)(&m_nTechLevel), sizeof(m_nTechLevel));
	oStream.Write((CHAR*)(&m_nAmbiLevel), sizeof(m_nAmbiLevel));
	oStream.Write((CHAR*)(&m_bAccess), sizeof(m_bAccess));
}

VOID GUILD_WGC_MEMBER_LIST::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_uValidMemberCount), sizeof(m_uValidMemberCount));
	iStream.Read((CHAR*)(&m_uMemberCount), sizeof(m_uMemberCount));
	iStream.Read((CHAR*)(&m_uMemberMax), sizeof(m_uMemberMax));
	iStream.Read((CHAR*)(&m_bPosition), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_bAccess), sizeof(BYTE));
	iStream.Read((CHAR*)(m_GuildDesc), MAX_GUILD_DESC_SIZE);
	iStream.Read((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);
	if( m_uMemberCount > 0 )
	{
		iStream.Read((CHAR*)m_GuildMemberData, sizeof(s) * m_uMemberCount);
	}
}

VOID GUILD_WGC_MEMBER_LIST::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_uValidMemberCount), sizeof(m_uValidMemberCount));
	oStream.Write((CHAR*)(&m_uMemberCount), sizeof(m_uMemberCount));
	oStream.Write((CHAR*)(&m_uMemberMax), sizeof(m_uMemberMax));
	oStream.Write((CHAR*)(&m_bPosition), sizeof(BYTE));
	oStream.Write((CHAR*)(&m_bAccess), sizeof(BYTE));
	oStream.Write((CHAR*)(m_GuildDesc), MAX_GUILD_DESC_SIZE);
	oStream.Write((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);

	if( m_uMemberCount > 0 )
	{
		oStream.Write((CHAR*)m_GuildMemberData, sizeof(s) * m_uMemberCount);
	}
}

VOID GUILD_WGC_LIST::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_uStartIndex), sizeof(m_uStartIndex));
	iStream.Read((CHAR*)(&m_uGuildCount), sizeof(m_uGuildCount));
	iStream.Read((CHAR*)(&m_uGuildListCount), sizeof(m_uGuildListCount));
	iStream.Read((CHAR*)(&m_Camp), sizeof(m_Camp));

	if( m_uGuildListCount > 0 )
	{
		iStream.Read((CHAR*)m_uGuild, sizeof(s) * m_uGuildListCount);
	}
}

VOID GUILD_WGC_LIST::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_uStartIndex), sizeof(m_uStartIndex));
	oStream.Write((CHAR*)(&m_uGuildCount), sizeof(m_uGuildCount));
	oStream.Write((CHAR*)(&m_uGuildListCount), sizeof(m_uGuildListCount));
	oStream.Write((CHAR*)(&m_Camp), sizeof(m_Camp));

	if( m_uGuildListCount > 0 )
	{
		oStream.Write((CHAR*)m_uGuild, sizeof(s) * m_uGuildListCount);
	}
}

VOID GUILD_WGC_SELF_GUILD_INFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildNameSize), sizeof(BYTE));
	
	if( m_GuildNameSize > 0 )
	{
		iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
	}

	iStream.Read((CHAR*)(&m_bAccess), sizeof(BYTE));
}

VOID GUILD_WGC_SELF_GUILD_INFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildNameSize), sizeof(BYTE));

	if( m_GuildNameSize > 0 )
	{
		oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
	}

	oStream.Write((CHAR*)(&m_bAccess), sizeof(BYTE));
}

VOID _GUILD_PACKET::Read(SocketInputStream& iStream)
{
	GUILD_PACKET*		pGuildPacket;

	iStream.Read( (CHAR*)(&m_uPacketType), sizeof(m_uPacketType) );

	pGuildPacket = GetPacket(m_uPacketType);
	if( pGuildPacket == NULL )
	{
		Assert( FALSE );
		return;
	}

	pGuildPacket->Read(iStream);
}

VOID _GUILD_PACKET::Write(SocketOutputStream& oStream) const
{
	GUILD_PACKET*		pGuildPacket;

	oStream.Write( (CHAR*)(&m_uPacketType), sizeof(m_uPacketType) );

	pGuildPacket = GetPacket(m_uPacketType);
	if( pGuildPacket == NULL )
	{
		Assert( FALSE );
		return;
	}

	pGuildPacket->Write(oStream);
}

VOID _GUILD_RETURN::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)&m_ReturnType, sizeof(m_ReturnType));

	switch( m_ReturnType )
	{
	case GUILD_RETURN_CREATE:
	case GUILD_RETURN_JOIN:
		{
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}
		}
		break;
	case GUILD_RETURN_RESPONSE:
		{
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}
		}
		break;
	case GUILD_RETURN_EXPEL:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_REJECT:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_RECRUIT:
		{
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}

		}
		break;
	case GUILD_RETURN_LEAVE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

		}
		break;
	case GUILD_RETURN_FOUND:
	case GUILD_RETURN_DISMISS:
		{
		}
		break;
	case GUILD_RETURN_PROMOTE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_PosID, sizeof(m_PosID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_DEMOTE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_PosID, sizeof(m_PosID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_AUTHORIZE:
	case GUILD_RETURN_DEPRIVE_AUTHORITY:
		{
		}
		break;
	case GUILD_RETURN_DEMISE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));
			iStream.Read((CHAR*)&m_ChangedPositionNameSize, sizeof(m_ChangedPositionName));
			
			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}

			if( m_ChangedPositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_ChangedPositionName, m_ChangedPositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_WITHDRAW:
	case GUILD_RETURN_DEPOSIT:
		{
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID _GUILD_RETURN::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)&m_ReturnType, sizeof(m_ReturnType));

	switch( m_ReturnType )
	{
	case GUILD_RETURN_CREATE:
	case GUILD_RETURN_JOIN:
		{
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}
		}
		break;
	case GUILD_RETURN_RESPONSE:
		{
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}
		}
		break;
	case GUILD_RETURN_EXPEL:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_REJECT:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_RECRUIT:
		{
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}

		}
		break;
	case GUILD_RETURN_LEAVE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_FOUND:
	case GUILD_RETURN_DISMISS:
		{
		}
		break;
	case GUILD_RETURN_PROMOTE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_PosID, sizeof(m_PosID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_DEMOTE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_PosID, sizeof(m_PosID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_AUTHORIZE:
	case GUILD_RETURN_DEPRIVE_AUTHORITY:
		{
		}
		break;
	case GUILD_RETURN_DEMISE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));
			oStream.Write((CHAR*)&m_ChangedPositionNameSize, sizeof(m_ChangedPositionName));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}

			if( m_ChangedPositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_ChangedPositionName, m_ChangedPositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_WITHDRAW:
	case GUILD_RETURN_DEPOSIT:
		{
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID MAIL::Read( SocketInputStream& iStream )
{
	iStream.Read((CHAR*)(&m_GUID),sizeof(m_GUID));
	iStream.Read((CHAR*)(&m_SourSize),sizeof(BYTE));
	if( m_SourSize>=MAX_CHARACTER_NAME )
	{
		Assert(FALSE) ;
		m_SourSize = MAX_CHARACTER_NAME ;
	}
	iStream.Read((CHAR*)(&m_szSourName),sizeof(CHAR)*m_SourSize);
	m_szSourName[MAX_CHARACTER_NAME-1] = 0 ;

	iStream.Read((CHAR*)(&m_nPortrait),sizeof(m_nPortrait));

	iStream.Read((CHAR*)(&m_DestSize),sizeof(BYTE));
	if( m_DestSize>=MAX_CHARACTER_NAME )
	{
		Assert(FALSE) ;
		m_DestSize = MAX_CHARACTER_NAME ;
	}
	iStream.Read((CHAR*)(&m_szDestName),sizeof(CHAR)*m_DestSize);
	m_szDestName[MAX_CHARACTER_NAME-1] = 0 ;

	iStream.Read((CHAR*)(&m_ContexSize),sizeof(WORD));
	if( m_ContexSize >= MAX_MAIL_CONTEX )
	{
		Assert(FALSE) ;
		m_ContexSize = MAX_MAIL_CONTEX ;
	}
	iStream.Read((CHAR*)(&m_szContex),sizeof(CHAR)*m_ContexSize);
	m_szContex[MAX_MAIL_CONTEX-1] = 0 ;

	iStream.Read((CHAR*)(&m_uFlag),sizeof(UINT));
	iStream.Read((CHAR*)(&m_uCreateTime),sizeof(m_uCreateTime));
	iStream.Read((CHAR*)(&m_Camp),sizeof(CampID_t));

	if( m_uFlag==MAIL_TYPE_SCRIPT )
	{
		iStream.Read((CHAR*)(&m_uParam0),sizeof(UINT));
		iStream.Read((CHAR*)(&m_uParam1),sizeof(UINT));
		iStream.Read((CHAR*)(&m_uParam2),sizeof(UINT));
		iStream.Read((CHAR*)(&m_uParam3),sizeof(UINT));
	}
}

VOID MAIL::Write( SocketOutputStream& oStream ) const
{
	oStream.Write((CHAR*)(&m_GUID),sizeof(m_GUID));
	Assert( m_SourSize<MAX_CHARACTER_NAME ) ;
	oStream.Write((CHAR*)(&m_SourSize),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_szSourName),sizeof(CHAR)*m_SourSize);

	oStream.Write((CHAR*)(&m_nPortrait),sizeof(m_nPortrait));

	Assert( m_DestSize<MAX_CHARACTER_NAME ) ;
	oStream.Write((CHAR*)(&m_DestSize),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_szDestName),sizeof(CHAR)*m_DestSize);

	Assert( m_ContexSize<MAX_MAIL_CONTEX ) ;
	oStream.Write((CHAR*)(&m_ContexSize),sizeof(WORD));
	oStream.Write((CHAR*)(&m_szContex),sizeof(CHAR)*m_ContexSize);

	oStream.Write((CHAR*)(&m_uFlag),sizeof(UINT));
	oStream.Write((CHAR*)(&m_uCreateTime),sizeof(m_uCreateTime));
	oStream.Write((CHAR*)(&m_Camp),sizeof(CampID_t));

	if( m_uFlag==MAIL_TYPE_SCRIPT )
	{
		oStream.Write((CHAR*)(&m_uParam0),sizeof(UINT));
		oStream.Write((CHAR*)(&m_uParam1),sizeof(UINT));
		oStream.Write((CHAR*)(&m_uParam2),sizeof(UINT));
		oStream.Write((CHAR*)(&m_uParam3),sizeof(UINT));
	}
}


VOID MAIL_LIST::Read( SocketInputStream& iStream )
{
	iStream.Read((CHAR*)(&m_Count),sizeof(BYTE));
	if( m_Count>MAX_MAIL_SIZE )
	{
		Assert(FALSE) ;
		m_Count = MAX_MAIL_SIZE ;
	}
	INT i=0;
	for( i=0; i<m_Count; i++ )
	{
		m_aMail[i].Read( iStream ) ;
	}
	iStream.Read((CHAR*)(&m_TotalLeft),sizeof(BYTE));
}

VOID MAIL_LIST::Write( SocketOutputStream& oStream ) const
{
	Assert( m_Count<=MAX_MAIL_SIZE ) ;
	oStream.Write((CHAR*)(&m_Count),sizeof(BYTE));
	INT i=0;
	for( i=0; i<m_Count; i++ )
	{
		m_aMail[i].Write( oStream ) ;
	}
	oStream.Write((CHAR*)(&m_TotalLeft),sizeof(BYTE));
}

VOID BATCH_MAIL::CleanUp()
{
	m_GUID = INVALID_ID;

	m_SourSize = 0;
	memset( m_szSourName, 0, sizeof(m_szSourName) );

	m_ReceiverCount = 0;
	memset( m_Receivers, 0, sizeof(m_Receivers) );

	m_ContentSize = 0;
	memset( m_szContent, 0, sizeof(m_szContent) );

	m_uFlag = MAIL_TYPE_NORMAL2;
	m_uCreateTime = 0;

	m_Camp = -1;
}

UINT BATCH_MAIL::GetSize() const
{
	UINT uSize;

	uSize = sizeof(m_GUID) + sizeof(m_SourSize) + m_SourSize + sizeof(m_ReceiverCount);

	if ( m_ReceiverCount > 0 )
	{
		for( INT i=0; i<m_ReceiverCount; ++i )
		{
			uSize += sizeof(m_Receivers[i].m_DestSize) + m_Receivers[i].m_DestSize;
		}
	}

	uSize += sizeof(m_ContentSize) + m_ContentSize;
	uSize += sizeof(m_uFlag) + sizeof(m_uCreateTime);
	uSize += sizeof(m_Camp);

	return uSize;
}

VOID BATCH_MAIL::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	iStream.Read( (CHAR*)(&m_SourSize), sizeof(m_SourSize) );
	if( m_SourSize > 0 && m_SourSize < MAX_CHARACTER_NAME )
	{
		iStream.Read( (CHAR*)(&m_szSourName), m_SourSize );
	}
	else
	{
		m_SourSize = 0;
	}

	iStream.Read( (CHAR*)(&m_ReceiverCount), sizeof(m_ReceiverCount) );
	if( m_ReceiverCount > 0 && m_ReceiverCount < MAX_RECEIVER )
	{
		for( INT i=0; i<m_ReceiverCount; ++i )
		{
			iStream.Read( (CHAR*)(&m_Receivers[i].m_DestSize), sizeof(m_Receivers[i].m_DestSize) );

			if( m_Receivers[i].m_DestSize > 0 && m_Receivers[i].m_DestSize < MAX_CHARACTER_NAME )
			{
				iStream.Read( (CHAR*)(&m_Receivers[i].m_szDestName), m_Receivers[i].m_DestSize );
			}
			else
			{
				m_Receivers[i].m_DestSize = 0;
			}
		}
	}
	else
	{
		m_ReceiverCount = 0;
	}

	iStream.Read( (CHAR*)(&m_ContentSize), sizeof(m_ContentSize) );
	if( m_ContentSize > 0 && m_ContentSize < MAX_MAIL_CONTEX )
	{
		iStream.Read( (CHAR*)(&m_szContent), m_ContentSize );
	}
	else
	{
		m_ContentSize = 0;
	}

	iStream.Read( (CHAR*)(&m_uFlag), sizeof(m_uFlag) );
	iStream.Read( (CHAR*)(&m_uCreateTime), sizeof(m_uCreateTime) );
	iStream.Read( (CHAR*)(&m_Camp), sizeof(m_Camp) );
}

VOID BATCH_MAIL::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	oStream.Write( (CHAR*)(&m_SourSize), sizeof(m_SourSize) );
	if( m_SourSize > 0 && m_SourSize < MAX_CHARACTER_NAME )
	{
		oStream.Write( (CHAR*)(&m_szSourName), m_SourSize );
	}

	oStream.Write( (CHAR*)(&m_ReceiverCount), sizeof(m_ReceiverCount) );
	if( m_ReceiverCount > 0 && m_ReceiverCount < MAX_RECEIVER )
	{
		for( INT i=0; i<m_ReceiverCount; ++i )
		{
			oStream.Write( (CHAR*)(&m_Receivers[i].m_DestSize), sizeof(m_Receivers[i].m_DestSize) );

			if( m_Receivers[i].m_DestSize > 0 && m_Receivers[i].m_DestSize < MAX_CHARACTER_NAME )
			{
				oStream.Write( (CHAR*)(&m_Receivers[i].m_szDestName), m_Receivers[i].m_DestSize );
			}
		}
	}

	oStream.Write( (CHAR*)(&m_ContentSize), sizeof(m_ContentSize) );
	if( m_ContentSize > 0 && m_ContentSize < MAX_MAIL_CONTEX )
	{
		oStream.Write( (CHAR*)(&m_szContent), m_ContentSize );
	}

	oStream.Write( (CHAR*)(&m_uFlag), sizeof(m_uFlag) );
	oStream.Write( (CHAR*)(&m_uCreateTime), sizeof(m_uCreateTime) );
	oStream.Write( (CHAR*)(&m_Camp), sizeof(m_Camp) );
}


VOID	DB_QUERY::Parse(const CHAR* pTemplate,...)
{
	va_list argptr;
	va_start(argptr, pTemplate);
	int nchars  = tvsnprintf((char*)m_SqlStr, MAX_SQL_LENGTH, pTemplate, argptr );
	va_end(argptr);

	if (nchars == -1 || nchars > MAX_SQL_LENGTH )
	{
		Assert(FALSE);
		return;
	}

}

VOID	LONG_DB_QUERY::Parse(const CHAR* pTemplate,...)
{
	va_list argptr;
	va_start(argptr, pTemplate);
	int nchars  = tvsnprintf((char*)m_SqlStr, MAX_LONG_SQL_LENGTH, pTemplate, argptr );
	va_end(argptr);
	if (nchars == -1 || nchars > MAX_LONG_SQL_LENGTH )
	{
		Assert(FALSE);
		return;
	}

}


UINT TEAM_LIST_ENTRY::GetSize() const
{
	return sizeof(m_GUID) + sizeof(m_SceneID) + sizeof(m_ExtraID) + sizeof(m_NameSize)
		 + sizeof(CHAR) * m_NameSize + sizeof(m_nPortrait) + sizeof(m_uDataID);
}

VOID TEAM_LIST_ENTRY::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_SceneID, sizeof(m_SceneID) );
	iStream.Read( (CHAR*)&m_ExtraID, sizeof(m_ExtraID) );
	iStream.Read( (CHAR*)&m_NameSize, sizeof(m_NameSize) );

	if ( m_NameSize>0 )
	{
		iStream.Read( (CHAR*)m_Name, sizeof(CHAR) * m_NameSize );
	}

	iStream.Read( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
	iStream.Read( (CHAR*)&m_uDataID, sizeof(m_uDataID) );
}

VOID TEAM_LIST_ENTRY::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_SceneID, sizeof(m_SceneID) );
	oStream.Write( (CHAR*)&m_ExtraID, sizeof(m_ExtraID) );
	oStream.Write( (CHAR*)&m_NameSize, sizeof(m_NameSize) );

	if ( m_NameSize>0 )
	{
		oStream.Write( (CHAR*)m_Name, sizeof(CHAR) * m_NameSize );
	}

	oStream.Write( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
	oStream.Write( (CHAR*)&m_uDataID, sizeof(m_uDataID) );
}
