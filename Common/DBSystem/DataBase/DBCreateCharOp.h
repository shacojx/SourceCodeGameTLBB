#ifndef _DB_CREATECHAR_OP_H_
#define _DB_CREATECHAR_OP_H_

#include "Type.h"
#include "ODBCBase.h"



class DBCreateCharOp:public ODBCBase
{
	UINT			m_AID;
	CHAR			m_Account[MAX_ACCOUNT+1];		//用户名称
	GUID_t			m_CharGuid;
	CHAR			m_CharName[MAX_CHARACTER_NAME];
	BOOL			m_Sex;
	BYTE			m_HairColor;					//新角色头发颜色	
	BYTE			m_FaceColor;					//新角色脸形颜色
	BYTE			m_HairModel;					//新角色头发模型
	BYTE			m_FaceModel;					//新角色脸形模型
	BYTE			m_HeadID;						//头部编号
	CampID_t		m_Camp	;						//阵营
	MenPaiID_t		m_MenPai;						//门派
public:
	DBCreateCharOp(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	AddNew();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	const	CHAR*	GetAccount()	const;
	VOID			SetAccount(const CHAR*	pAccount);

	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();

	VOID			SetCharName(const CHAR* pName);
	const CHAR*		GetCharName();
	
	VOID			SetSex(BOOL bSex);
	BOOL			GetSex();

	VOID			SetHairColor(BYTE Color);
	BYTE			GetHairColor();

	VOID			SetFaceColor(BYTE Color);
	BYTE			GetFaceColor();

	VOID			SetHairModel(BYTE Model);
	BYTE			GetHairModel();

	VOID			SetFaceModel(BYTE Model);
	BYTE			GetFaceModel();


	VOID			SetHeadID(BYTE Head);
	BYTE			GetHeadID();

	VOID			SetCampID(CampID_t Camp);
	CampID_t		GetCampID();

	VOID			SetMenPaiID(MenPaiID_t MenPai);
	MenPaiID_t		GetMenPaiID();
};


#endif