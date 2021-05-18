#include "StdAfx.h"
#include "UIGlobal.h"
#include "UIString.h"
#include "TDKernel.h"
#include "TDObjectSystem.h"
#include "TDException.h"
#include "TDDataBase.h"
#include "TDDBC_Struct.h"
#include "Core/utf.h"

struct AUTO_LOCALE_CONVERT
{
	AUTO_LOCALE_CONVERT()
	{
		std::locale langLocale("");
		old = std::locale::global(langLocale);
	}
	~AUTO_LOCALE_CONVERT()
	{
		std::locale::global(old);
	}

	std::locale old;
};

CUIStringSystem* CUIStringSystem::s_pMe = NULL;
CUIStringSystem::CUIStringSystem()
{
	s_pMe = this;
}

CUIStringSystem::~CUIStringSystem()
{
}

VOID CUIStringSystem::Init(LPCTSTR szStringDBC)
{
	//----------------------------------------------------
	//字符串字典
	tDataBaseSystem* pUIDBC = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(pUIDBC);
	const tDataBase* pDBCDic = pUIDBC->GetDataBase(DBC_STRING_DICT);
	for(INT i=0; i<(INT)pDBCDic->GetRecordsNum(); i++)
	{
		const _DBC_STRING_DICT* pLine = (const _DBC_STRING_DICT*)((tDataBase*)pDBCDic)->Search_LineNum_EQU(i);
		m_theDirectionary[pLine->szKey] = pLine->szString;
	}

	//系统关键字
	//m_theDirectionary[UKS_PLAYERNAME] = 
	
	//---------------------------------
	//字符串字典id转化
	DIRECTIONARY::iterator it;
	for(it=m_theDirectionary.begin(); it!=m_theDirectionary.end(); it++)
	{
		STRING strOut;
		ParserString_Prebuild(it->second, strOut);

		it->second = strOut;
	}
	//------------------------------------------------

	//聊天模板创建
	GenerateTalkTemplateTable();

	// 表情映射
	//加载所有ui
	const tDataBase* pDBC = pUIDBC->GetDataBase(DBC_CODE_CONVERT);
	for(INT i=0; i<(INT)pDBC->GetRecordsNum(); i++)
	{
		const _DBC_CODE_CONVERT* pLine = (const _DBC_CODE_CONVERT*)((tDataBase*)pDBC)->Search_LineNum_EQU(i);
		CEGUI::utf32 dwValue = 0;
		sscanf( pLine->szCode, "%8X", &dwValue );
		m_mapCodeConvertTable[ pLine->nID ] = dwValue ;
	}

	//------------------------------------------------
	// 过滤词数组构造
	const tDataBase* pDBCfl = pUIDBC->GetDataBase(DBC_TALK_FILTER);
	for(INT i=0; i<(INT)pDBCfl->GetRecordsNum(); i++)
	{
		const _DBC_TALK_FILTER* pLine = (const _DBC_TALK_FILTER*)((tDataBase*)pDBCfl)->Search_LineNum_EQU(i);
		m_vecFilterTable.push_back(pLine->szString);
	}

	//------------------------------------------------
	// 完全匹配过滤表构造
	GenerateFullCompareTable();
}

std::pair< BOOL, CUIStringSystem::DIRECTIONARY::iterator > CUIStringSystem::_IsValidDictionKey(const STRING& strKey)
{
	DIRECTIONARY::iterator it = m_theDirectionary.find(strKey);
	if(it == m_theDirectionary.end()) return std::make_pair(false, it);
	else return std::make_pair(true, it);
}

// Prebuild转化：主要是从字典和系统级字符串中查找
VOID CUIStringSystem::ParserString_Prebuild(const STRING& strSource, STRING& strOut)
{
	const CHAR KeyParserBegin	= '{';
	const CHAR KeyParserMiddle	= '=';
	const CHAR KeyParserEnd		= '}';

	STRING::size_type nValidBegin = 0;
	strOut = "";

	do
	{
		if(nValidBegin >= strSource.size()) break;

		STRING::size_type nValidEnd = strSource.find(KeyParserBegin, nValidBegin);

		//最后一段正常值
		if(nValidEnd == STRING::npos) 
		{
			strOut += strSource.substr(nValidBegin);

			break;
		}

		//加入正常值
		if(nValidEnd != nValidBegin) 
		{
			strOut += strSource.substr(nValidBegin, nValidEnd-nValidBegin);
		}

		STRING::size_type nKeyStart = nValidEnd+1;									//"{"
		STRING::size_type nKeyMiddle = strSource.find(KeyParserMiddle, nKeyStart);	//"="
		STRING::size_type nKeyEnd = strSource.find(KeyParserEnd, nKeyStart);		//"}"
		
		//合法的Key描述
		if(nKeyEnd != STRING::npos && nKeyMiddle != STRING::npos && nKeyEnd > nKeyMiddle)
		{
			STRING strKey_Left = strSource.substr(nKeyStart, nKeyMiddle-nKeyStart);
			STRING strKey_Right = strSource.substr(nKeyMiddle+1, nKeyEnd-nKeyMiddle-1);

			if(strKey_Left == "ID")
			{
				std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strKey_Right);
				//找到key
				if(isValidKey.first)
				{
					//加入从key转化过来的值
					strOut += isValidKey.second->second;

					nValidBegin = nKeyEnd+1;
					continue;
				}
			}
		}
		strOut += strSource.substr(nValidEnd, nKeyEnd-nValidEnd);
		nValidBegin = nKeyEnd;
	}while(true);
}

//对MBCS码字符串进行合法检测，剔除非法字符，例如半个汉字
/*
|
|	使用GBK标准
|
|	Windows95及后续版本中文版支持GBK。
|	GB2312的EUC编码范围是第一字节0xA1~0xFE(实际只用到0xF7)，第
|	二字节0xA1~0xFE。GBK对此进行扩展。第一字节为0x81~0xFE，第二
|	字节分两部分，一是0x40~0x7E，二是0x80~0xFE。其中和GB2312相
|	同的区域，字完全相同。扩展部分大概是按部件(部首)和笔顺(笔画)
|	从GB13000中取出再排列入GBK中。因此GBK并不是GB13000，虽然两者
|	字汇相同，但编码体系不同。一个是ISO2022系列不等长编码，一个
|	是等长编码，并且编码区域也不同。注意到GBK实际上不是国家标准。
|	在此之前有一个GB2312基本集，在它之上是一个技术更先进的GB13000。
|	GBK只是一种过渡和扩展规范。所以在Unicode里有GB2312->Unicode,
|	GB12345->Unicode的转换表格，而没有GBK->Unicode转换表格。只有
|	Microsoft制作的Code Page 936(CP936.TXT)可以算作GBK->Unicode
|	转换表格。但要注意这是一个商业公司制作的文件，而不是国家或
|	国际标准组织制作的，有可能与标准有不一致的地方。
|
|
*/
BOOL CUIStringSystem::_CheckStringCode(const STRING& strIn, STRING& strOut)
{
	const BYTE byANSIBegin		= 0X20;
	const BYTE byANSIEnd		= 0X80;

	const BYTE by1GBKBegin	= 0X81;
	const BYTE by1GBKEnd	= 0XFE;

	const BYTE by2GBKBegin1	= 0X40;
	const BYTE by2GBKEnd1	= 0X7E;
	const BYTE by2GBKBegin2	= 0X80;
	const BYTE by2GBKEnd2	= 0XFE;

	const CHAR * const szERROR = "<ERROR>";

	//预先分配好内存
	INT nSourSize = (INT)strIn.size();
	strOut.reserve(nSourSize+1);
	strOut = "";

	for(INT i=0; i<nSourSize; )
	{
		BYTE byChar = strIn[i];

		//标准ANSI英文字符
		if(byChar >= byANSIBegin && byChar <= byANSIEnd || byChar == '\r' || byChar == '\n' || byChar == '\t')
		{
			strOut += byChar;
			i++; 
			continue;
		}
		//汉字第一个字节
		else if(byChar >= by1GBKBegin && byChar <= by1GBKEnd)
		{
			//半个汉字
			if(i+1 == nSourSize) goto CHECKSTRING_ERROR;

			//取出第二个字节
			BYTE byNext = strIn[++i];
			if(	!( byNext >= by2GBKBegin1 && byNext <= by2GBKEnd1 || byNext >= by2GBKBegin2 && byNext <= by2GBKEnd2) )
			{
				//非法的汉字
				goto CHECKSTRING_ERROR;
			}

			//加入完整的汉字
			CHAR szTemp[8];
			szTemp[0] = byChar;
			szTemp[1] = byNext;
			szTemp[2] = 0;
			strOut += szTemp;

			i++;
			continue;
		}
		else goto CHECKSTRING_ERROR;
	}
	return TRUE;

CHECKSTRING_ERROR:
	strOut += szERROR;
	return FALSE;
}

//对文字串中出现的非法字符进行过滤
BOOL CUIStringSystem::_CheckStringFilter(const STRING& strIn)
{
	size_t allsize = m_vecFilterTable.size();
	for(INT i = 0; i < (INT)allsize; ++i)
	{
		if(STRING::npos != strIn.find(m_vecFilterTable[i]))	return FALSE; //发现非法字符串
	}

	return TRUE;
}

//完全匹配过滤
BOOL CUIStringSystem::_CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType, BOOL bUseAllTable)
{
	BOOL bFind = FALSE;

	FULLCMP_MAP::iterator it = m_fullCompareTable.find("all");
	if(bUseAllTable && it != m_fullCompareTable.end())
	{
		UI_FULLCMP_TEMPLATE& strVec = it->second;
		if(strVec.find(strIn) != strVec.end()) bFind = TRUE;
	}

	it = m_fullCompareTable.find(strFilterType);
	if(!bFind && it != m_fullCompareTable.end())
	{
		UI_FULLCMP_TEMPLATE& strVec = it->second;
		if(strVec.find(strIn) != strVec.end()) bFind = TRUE;
	}

	return !bFind;
}

// 运行时转化
// 特殊表情符转化，把后面的字符转化成utf32，并且把string也转化成utf32
VOID CUIStringSystem::ParserString_Runtime(const STRING& strSourceNotCheck, CEGUI::String& strOutUtf32)
{

	return ParserString_RuntimeNew(strSourceNotCheck, strOutUtf32);

	const CHAR KeyParserBegin	= '{';
	const CHAR KeyParserMiddle	= '=';
	const CHAR KeyParserEnd		= '}';

	STRING::size_type nValidBegin = 0;
	STRING strOutMBCS = "";

	//检查字符串合法性
	STRING strSource;
	_CheckStringCode(strSourceNotCheck, strSource);
	
	//过滤聊天信息中的非法信息
	if(FALSE == _CheckStringFilter(strSource))
		strSource = "#R请注意你的言辞！！";

	do
	{
		if(nValidBegin >= strSource.size()) break;

		STRING::size_type nValidEnd = strSource.find(KeyParserBegin, nValidBegin);

		//最后一段正常值
		if(nValidEnd == STRING::npos) 
		{
			strOutMBCS = strSource.substr(nValidBegin);

			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
			break;
		}

		//加入正常值
		if(nValidEnd != nValidBegin) 
		{
			strOutMBCS = strSource.substr(nValidBegin, nValidEnd-nValidBegin);

			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
		}

		STRING::size_type nKeyStart = nValidEnd+1;									//"{"
		STRING::size_type nKeyMiddle = strSource.find(KeyParserMiddle, nKeyStart);	//"="
		STRING::size_type nKeyEnd = strSource.find(KeyParserEnd, nKeyStart);		//"}"
		
		//合法的Key描述
		if(nKeyEnd != STRING::npos && nKeyMiddle != STRING::npos && nKeyEnd > nKeyMiddle)
		{
			STRING strKey_Left = strSource.substr(nKeyStart, nKeyMiddle-nKeyStart);
			STRING strKey_Right = strSource.substr(nKeyMiddle+1, nKeyEnd-nKeyMiddle-1);

			if(strKey_Left == "ID")
			{
				std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strKey_Right);
				//找到key
				if(isValidKey.first)
				{
					//加入从key转化过来的值
					strOutMBCS += isValidKey.second->second;

					nValidBegin = nKeyEnd+1;

					std::string strTemp;
					CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
					strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());

					strOutMBCS = "";
					continue;
				}
			}
			else if(strKey_Left == "PIC") // 表情动画
			{
				// 如果已经定义了
			/*	CODE_CONVERT::iterator  pos = m_mapCodeConvertTable.find( strKey_Right );
//				if( pos != m_mapCodeConvertTable.end() )
				{
					CEGUI::utf32 val = atoi(strKey_Right.c_str());
					val |= 0xFA000000;
					strOutUtf32 += val;
					nValidBegin = nKeyEnd + 1;
					continue;
				}*/
			}
			else if( strKey_Left == "COL" ) // 字体颜色
			{
				CEGUI::utf32 val = 0;
				sscanf( strKey_Right.c_str(), " %8X", &val);
				val = 0xFB000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nValidBegin = nKeyEnd + 1;
				continue;
			}
			else if( strKey_Left == "BAK" ) // 字体背景色
			{
				CEGUI::utf32 val = 0;
				sscanf( strKey_Right.c_str(), " %8X", &val);
				val = 0xFC000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nValidBegin = nKeyEnd + 1;
				continue;
			}
		}
		strOutMBCS = strSource.substr(nValidEnd, nKeyEnd-nValidEnd);

		std::string strTemp;
		CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
		strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());

		nValidBegin = nKeyEnd;
	}while(true);
	
}

VOID CUIStringSystem::ParserString_RuntimeNew(const STRING& strSourceNotCheck, CEGUI::String& strOutUtf32)
{
	/*
	#R		表示后面的字体为红色(red)
	#G		表示后面的字体为绿色(green)
	#B		表示后面的字体为蓝色(blue)
	#K		表示后面的字体为黑色(black)
	#Y		表示后面的字体为黄色(yellow)
	#W		表示后面的字体为白色(white)
	#b		表示后面的字体为闪烁(blink)
	#c		+ 六个数字或者A-F字母 自定义颜色，例如：c008000=暗绿色
	#e		+ 六个数字或者A-F字母 自定义颜色，例如：e008000=暗绿色,表示字体扩展边底色
	#u		+ 文字 + #u 文字有下划线。
	#n		所有文字状态恢复正常。
	#r		文字换行。
	##		输出一个#号。
	#aB		表示后面跟着一个超链接，这是超链接部分的开始
	#aE		表示后面跟着一个超链接，这是超链接部分的结束

	#{IDSTR}  查表替换字符串

	#0~99	  表情
	#-01~-99	仅程序使用，不许玩家输入的动画
	*/

	const CHAR			KeyParser			= '#';
	const CHAR			KeyParserIDBegin	= '{';
	const CHAR			KeyParserIDEnd		= '}';
	STRING::size_type	nValidBegin			= 0;
	STRING				strOutMBCS			= "";

	BOOL				bBlink				= FALSE;

	//检查字符串合法性
	STRING strSource;
	_CheckStringCode(strSourceNotCheck, strSource);

	//过滤聊天信息中的非法信息
	if(FALSE == _CheckStringFilter(strSource))
		strSource = "#R请注意你的言辞！！";

	do
	{
		if(nValidBegin >= strSource.size()) break;

		STRING::size_type nValidEnd = strSource.find(KeyParser, nValidBegin);

		//最后一段正常值
		if(nValidEnd == STRING::npos) 
		{
			strOutMBCS = strSource.substr(nValidBegin);
			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
			break;
		}

		//加入正常值
		if(nValidEnd != nValidBegin) 
		{
			strOutMBCS = strSource.substr(nValidBegin, nValidEnd-nValidBegin);
			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
		}

		STRING::size_type nKeyStart = nValidEnd+1;	//"#"后面那个字符的位置
		STRING::size_type nKeyEnd;					//下一个非指令字符的位置							

		//合法的Key描述
		if(nKeyStart != STRING::npos )
		{
			STRING strKey_Opt	= strSource.substr(nKeyStart, 1);
			CEGUI::utf32 val = 0;
			if( strKey_Opt == "R" )
			{//#R红色
				val = 0xFBFF0000;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "G" )
			{//#G绿色
				val = 0xFB00FF00;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "B" )
			{//#B蓝色
				val = 0xFB0000FF;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "K" )
			{//#K黑色
				val = 0xFB000000;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "Y" )
			{//#Y黄色
				val = 0xFBFFFF00;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "W" )
			{//#W白色
				val = 0xFBFFFFFF;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "b" )
			{//#b闪烁
				val = 0xE0000000;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;

				bBlink = TRUE;
			}
			else if( strKey_Opt == "c" )
			{//#c自定义颜色
				STRING strColor = strSource.substr(nKeyStart+1, 6);
				CEGUI::utf32 val = 0;
				sscanf( strColor.c_str(), " %6X", &val);
				val = 0xFB000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nKeyEnd = nKeyStart + 7;
			}
			else if( strKey_Opt == "e" )
			{//#e自定义扩展边颜色
				STRING strColor = strSource.substr(nKeyStart+1, 6);
				CEGUI::utf32 val = 0;
				sscanf( strColor.c_str(), " %6X", &val);
				val = 0xFC000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nKeyEnd = nKeyStart + 7;
			}
			else if( strKey_Opt == "u" )
			{//#u下划线
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "n" )
			{//#n回复状态
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "r" )
			{//#r回车换行
				strOutMBCS = "\n";
				std::string strTemp;
				CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
				strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "#" )
			{//##
				strOutMBCS = "#";
				std::string strTemp;
				CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
				strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "a")
			{//HyperLink #aB{linktxt}showtxt#aE
				STRING strManipulate = strSource.substr(nKeyStart+1, 1);
				if(strManipulate == "B")//#aB
				{
					val = 0xFD000000;
					strOutUtf32 += val;
					nKeyEnd = nKeyStart+2;
				}
				else if(strManipulate == "E")//#aE
				{
					val = 0xFE000000;
					strOutUtf32 += val;
					nKeyEnd = nKeyStart+2;
				}
				else
				{
					//非法HyperLink#a字串
					nKeyEnd = nKeyStart+2;
				}
			}
			else if( strKey_Opt.c_str()[0] == KeyParserIDBegin )
			{//#{IDSTR} == 大哥
				STRING::size_type nIDBegin	= nKeyStart+1;
				STRING::size_type nIDEnd	= strSource.find(KeyParserIDEnd, nIDBegin);

				if(nIDEnd != STRING::npos)
				{
					STRING strID = strSource.substr(nIDBegin, nIDEnd-nIDBegin);

					if(strID.substr(0,5) == "_ITEM")
					{//#{_ITEM10100005}
						STRING szTBindex = strID.substr(5,8);
						UINT	TBindex = atoi(szTBindex.c_str());
						strOutMBCS	=	g_pObjectSystem->ItemNameByTBIndex(TBindex);
						std::string strTemp;
						CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
						strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
					}
					if(strID.substr(0,5) == "_TIME")
					{//#{_TIME0507211233}
					 //例如：0507211233 表示 "2005年07月21日 12:33"
						STRING szTime	= strID.substr(5,10);
						UINT	uTime	= atoi(szTime.c_str());

						INT min,hour,day,month,year;

						min = uTime % 100;
						uTime /= 100;

						hour = uTime % 100;
						uTime /= 100;

						day = uTime % 100;
						uTime /= 100;

						month = uTime % 100;
						uTime /= 100;

						year = 2000 + uTime % 100;
						//uTime /= 100;

						CHAR szFormat[] = "%d年%d月%d日 %d:%d";
						CHAR szDate[32] = {0};
						_snprintf(szDate,sizeof(szDate), szFormat, year,month,day,hour,min);
						strOutMBCS = szDate;

						std::string strTemp;
						CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
						strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
					}
					//#{_MONEY8583045}
					if(strID.substr(0,6) == "_MONEY")
					{
						INT nMoney;
						sscanf(strID.c_str(),"_MONEY%d",&nMoney);

						INT nGoldCoin = nMoney / 10000;
						INT nSilverCoin = (nMoney % 10000) / 100;
						INT nCopperCoin = nMoney % 100;
						
						CHAR szData[128] = {0};

						if(nGoldCoin != 0)
						{
							_snprintf(szData, sizeof(szData), "%d#-02", nGoldCoin);
						}
						if(nSilverCoin != 0)
						{
							_snprintf(szData, sizeof(szData), "%s%d#-03", szData, nSilverCoin);
						}
						if(nCopperCoin != 0)
						{
							_snprintf(szData, sizeof(szData), "%s%d#-04", szData, nCopperCoin);
						}

						strOutMBCS = szData;

						CEGUI::String strTemp;
						ParserString_RuntimeNew(strOutMBCS, strTemp);

						strOutUtf32 += strTemp;
					}
					else
					{//不是特殊命令查表
						std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strID);
						//找到key
						if(isValidKey.first)
						{
							//加入从key转化过来的值
							strOutMBCS = isValidKey.second->second;
							CEGUI::String strOutTemp;
							//递归分析
							ParserString_RuntimeNew(strOutMBCS, strOutTemp);
							strOutUtf32 += strOutTemp;
						}
					}

					nKeyEnd = nIDEnd+1;
				}
				else
				{
					//非法#{需要过滤掉
					nKeyEnd = nKeyStart + 2; 
				}
			}
			else if( strKey_Opt == "-")
			{//#-01~-99(必须是两位数值，一位数值算非法值) 仅程序使用的表情符，玩家输入的字符串不能使用。
				STRING strNum = strSource.substr(nKeyStart+1,2);
				if((strNum.size() == 2) &&
				   (strNum.at(0) >= 48 && strNum.at(0) <= 57) &&
				   (strNum.at(1) >= 48 && strNum.at(1) <= 57))
				{
					UINT nNum;
					sscanf(strNum.c_str(), " %u", &nNum);
					
					if(1 != nNum)
					{
						CEGUI::utf32 val = nNum;
						val |= 0xE3000000;
						strOutUtf32 += val;

						nKeyEnd = nKeyStart + 3;
					}
					else
					{
						// -1是非法动画标识，必须从-2开始 [4/24/2006]
						//忽略掉#-
						nKeyEnd = nKeyStart + 1;
					}
				}
				else
				{//非法命令
					//忽略掉#-
					nKeyEnd = nKeyStart + 1;
				}
			}
			else if( strKey_Opt.c_str()[0] >= 48 && strKey_Opt.c_str()[0] <= 57 )
			{//#0~99 表情
				UINT	nDigital	= strKey_Opt.c_str()[0] - 48;
				STRING strDigital	= strSource.substr(nKeyStart+1, 1);
			
				if(strDigital.c_str()[0] >= 48  && strDigital.c_str()[0] <= 57 )
				{//两位整数
					nDigital = nDigital*10 + (strDigital.c_str()[0] - 48);
					nKeyEnd = nKeyStart + 2;
				}
				else
				{//一位整数
					nKeyEnd = nKeyStart + 1;
				}

//				CODE_CONVERT::iterator  pos = m_mapCodeConvertTable.find( nDigital );
//				if( pos != m_mapCodeConvertTable.end() )
				{
					CEGUI::utf32 val = nDigital;
					val |= 0xFA000000;
					strOutUtf32 += val;
				}
			}
			else
			{//非法命令
				//忽略掉#
				nKeyEnd = nKeyStart;
			}
		}
		nValidBegin = nKeyEnd;
	}while(true);

	if(bBlink)
	{
		CEGUI::utf32 val = 0xE1000000;
		strOutUtf32 += val;
	}

}

#define	SELFMSG_MAX_LENGTH 2048
//根据传入的系统字串ID，在字典里查找格式字符串
STRING CUIStringSystem::ParserString_VarParam(const STRING &strID, va_list ptr)
{
	STRING strOut = "";

	std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strID);
	if(isValidKey.first)
	{
		if(ptr)
		{
			STRING strFormat = isValidKey.second->second;
			CHAR  result[SELFMSG_MAX_LENGTH+1];
			memset(result,0,SELFMSG_MAX_LENGTH+1);

			//格式化结果字符串
			_vsnprintf(result, SELFMSG_MAX_LENGTH, strFormat.c_str(), ptr);

			strOut = result;
		}
	}

	return strOut;
}

STRING CUIStringSystem::ParserString_NoVarParam(const STRING& strID)
{
	STRING strOut = "";

	std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strID);
	if(isValidKey.first)
	{
		strOut = isValidKey.second->second;
	}

	return strOut;
}

#undef SELFMSG_MAX_LENGTH

VOID CUIStringSystem::ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl)
{
	const CHAR KeyStart = '#';
	strOut.clear();
	
	STRING::size_type sB = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

	do
	{	
		if(sE == STRING::npos)
		{
			//copy last
			strOut += strSrc.substr(sB);
			break;
		}
		
		//copy normal
		strOut += strSrc.substr(sB, sE-sB);

		//get op
		STRING strOp = strSrc.substr(sE+1, 1);

		//skip color
		if(strOp == "R" || strOp == "G" || strOp == "B" ||
		   strOp == "K" || strOp == "Y" || strOp == "W")
		{
			sE += 2;
		}
		else if(strOp == "c" || strOp == "e")
		{
			sE += 8;
		}
		else if(bControl && (strOp == "b" || strOp == "u" || strOp == "r" || strOp == "n"))
		{
			sE += 2;
		}
		else
		{
			//not color means
			strOut += strSrc.at(sE);	//same as --> strOut += KeyStart;
			sE += 1;
		}

		if(sE >= sLen) break;

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);
}

STRING CUIStringSystem::getTalkTemplate(const STRING& strKey, UINT index)
{
	STRING strOut = "";

	if(!(strKey.empty()))
	{
		TALK_TEMPLATE_MAP::iterator itFind = m_talkTemplateTable.find(strKey);
		if(itFind != m_talkTemplateTable.end() && index < (itFind->second).size())
		{
			strOut = (itFind->second)[index];
		}
	}

	return strOut;
}

STRING CUIStringSystem::getTalkRandHelpMsg()
{
	TALK_TEMPLATE_MAP::iterator itFind = m_talkTemplateTable.find(TALK_HELPSTRING_KEY);
	if(itFind != m_talkTemplateTable.end())
	{
		UINT idx = abs(rand()%(INT)((itFind->second).size()));
		return (itFind->second)[idx];
	}
	return "";
}

void CUIStringSystem::mbcs_to_utf8(const std::string& strIn, std::string& strOut)
{
	//设置语言环境
	AUTO_LOCALE_CONVERT theAutoConvert;
	strOut = utf::mbcs_to_utf8(strIn);
}

void CUIStringSystem::utf8_to_mbcs(const std::string& strIn, std::string& strOut)
{
	//设置语言环境
	AUTO_LOCALE_CONVERT theAutoConvert;
	strOut = utf::utf8_to_mbcs(strIn);
}

VOID CUIStringSystem::GenerateFullCompareTable()
{
	tDataBaseSystem* pUIDBC = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(pUIDBC);
	const tDataBase* pDBCFullCmp = pUIDBC->GetDataBase(DBC_FULLCMP_FILTER);
	TDAssert(pDBCFullCmp);
	for(INT i=0; i<(INT)pDBCFullCmp->GetRecordsNum(); i++)
	{
		const _DBC_FULLCMP_FILTER* pLine = (const _DBC_FULLCMP_FILTER*)((tDataBase*)pDBCFullCmp)->Search_LineNum_EQU(i);
		FULLCMP_MAP::iterator itFind = m_fullCompareTable.find(pLine->szFilterType);

		//创建新的类型
		if(itFind == m_fullCompareTable.end())
		{
			UI_FULLCMP_TEMPLATE	talkTemplate;

			m_fullCompareTable.insert(std::make_pair(pLine->szFilterType, talkTemplate));
			itFind = m_fullCompareTable.find(pLine->szFilterType);
		}

		//添加字串
		UI_FULLCMP_TEMPLATE& strVec = itFind->second;
		strVec.insert(pLine->szFullTxt);
	}
}

VOID CUIStringSystem::GenerateTalkTemplateTable()
{
	DIRECTIONARY::iterator it = m_theDirectionary.begin();

	UI_TALK_TEMPLATE	talkTemplate;
	STRING				talkKey;

	do
	{
		//聊天快捷模板发现
		if("TALK_" == (it->first).substr(0,5))
		{
			talkKey = (it->first).substr(5, (it->first).find("_", 6)-(it->first).find("_") -1);

			TALK_TEMPLATE_MAP::iterator itTemplate = m_talkTemplateTable.find(talkKey);
			if(itTemplate == m_talkTemplateTable.end())
			{
				INT idmax = (TALK_HELPSTRING_KEY == talkKey)?TALK_HELPSTRING_MAX:TALK_TEMPLATE_MAX;
				for(INT id = 0; id < idmax; ++id)
				{
					CHAR num[4] = {0};
					_snprintf(num,4,"%03d",id+1);

					STRING strFind = "TALK_" + talkKey + "_" + num;

					DIRECTIONARY::iterator itFind = m_theDirectionary.find(strFind);
		
					if(itFind != m_theDirectionary.end())
					{
						talkTemplate.push_back(itFind->second);
                		m_theDirectionary.erase(itFind);
					}
					else
					{
						if(idmax == TALK_TEMPLATE_MAX)
							talkTemplate.push_back("");	//not found insert an empty string.
					}
				}

				m_talkTemplateTable.insert(std::make_pair(talkKey, talkTemplate));

				talkKey.clear();
				talkTemplate.clear();
				it = m_theDirectionary.begin();
			}
			else
				m_theDirectionary.erase(it++); //说明聊天模板的书写方式错误，删除错误记录
		}
		else
			it++;
	}
	while(it != m_theDirectionary.end());
}

STRING CUIStringSystem::ModifyChatTemplate(const STRING& strSrc, const STRING& strTalker, const STRING& strTarget)
{
	const CHAR KeyStart = '$';

	STRING::size_type sB = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

	STRING strOut;
	strOut.clear();

	do
	{	
		if(sE == STRING::npos)
		{
			//copy last
			strOut += strSrc.substr(sB);
			break;
		}
			
		//copy normal
		strOut += strSrc.substr(sB, sE-sB);

		//get op
		STRING strOp = strSrc.substr(sE+1, 1);

		//replace talker
		if(strOp == "N")
		{
			strOut += strTalker;
			sE += 2;
		}
		else if(strOp == "n") //replace target
		{
			strOut += strTarget;
			sE += 2;
		}
		else
		{
			//ignore wrong '$'
			//strOut += strSrc.at(sE);
			sE += 1;
		}

		if(sE >= sLen) break;

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);

	return strOut;
}
