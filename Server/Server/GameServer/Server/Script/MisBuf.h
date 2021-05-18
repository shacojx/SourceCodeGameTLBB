//-----------------------------------------------------------------------------
// 文件名 : MisBuf.h
// 模块	:	Script
// 功能	 :	任务系统用的场景生存期的数据块
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __MISBUF__H_
#define __MISBUF__H_


typedef struct _MisBuf
{
	enum 
	{
		MISSIONBUFLEN = 64,
		MISSIONCHARBUFLEN = MISSIONBUFLEN * 4 * 8,//因为客户端的bug,所以放大buf长度
	};

	typedef struct _MisItemBuf
	{
		//这个buf存储了发送给客户端的字符信息串,客户端需要注意字符解析问题
		_MisItemBuf()
		{
			ResetBuf();
		}
		
		CHAR buf[MISSIONCHARBUFLEN];
		enum BufType
		{
			BufType_Int,
			BufType_Str,
			BufType_IntStr,
			BufType_Money,
			BufType_Item,
			BufType_RandItem,
			BufType_RadioItem,
			BufType_Skill
		}mType;	
		INT m_BufUse;
		VOID ResetBuf(void)
		{

			m_BufUse = 0;
			memset(buf,0,MISSIONCHARBUFLEN);
		}
		VOID SetBufUse(INT NewBufUse){
			m_BufUse += NewBufUse;
		}
		INT GetBufUse(void){
			return m_BufUse;
		}
		INT GetBufMaxSize(){
			return MISSIONCHARBUFLEN;
		}
		INT AddBuf(VOID* vpNewBuf, INT nNewBufLen){
			if (m_BufUse+nNewBufLen > MISSIONCHARBUFLEN)
			{
				return 0;
			}
			memcpy(buf+m_BufUse, (VOID*)(vpNewBuf), nNewBufLen);
			m_BufUse += nNewBufLen;
			return m_BufUse;
		}
	}MisItemBuf[MISSIONBUFLEN];

	MisItemBuf Buff;
	INT mUseNum;

	VOID ResetMisBuf()
	{
		//因为采用Q_strncyz,所以不用再清0
		//memset(&Buff,0,sizeof(Buff));

		mUseNum = 0;
		for (UINT i = 0; i < MISSIONBUFLEN; i++)
		{
			Buff[i].ResetBuf();
		}
	}
}MisBuf;

#endif