#include "StdAfx.h"
#include <string>
#include <map>
#include <assert.h>
#include "TLBB_DBC.h"

namespace DBC
{

INT	DBCFile::_ConvertStringToVector(const CHAR* strStrINTgSource, std::vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty)
{
	vRet.clear();
	
	std::string strSrc = strStrINTgSource;
	if(strSrc.empty())
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(TRUE)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (INT)vRet.size();
}

DBCFile::DBCFile(UINT id)
{
	m_ID = id;
	m_pStringBuf = NULL;
	m_nIndexColum = -1;
}

DBCFile::~DBCFile()
{
	if(m_pStringBuf) delete[] m_pStringBuf;
	m_pStringBuf = NULL;
}

BOOL DBCFile::OpenFromDBC(const CHAR* szFileName)
{
	assert(szFileName);

	//----------------------------------------------------
	//打开文件
	FILE* fp = fopen(szFileName, "rb");
	if(NULL == fp) return FALSE;

	//----------------------------------------------------
	//Read Head
	FILE_HEAD theHead;
	fread(&theHead, sizeof(FILE_HEAD), 1, fp);
	if(memcmp(theHead.m_Identify, "TDBC", 4) != 0) 
	{
		fclose(fp); return FALSE;
	}

	//----------------------------------------------------
	//Init 
	m_nRecordsNum = theHead.m_nRecordsNum;
	m_nFieldsNum  = theHead.m_nFieldsNum;
	m_nStringBufSize = theHead.m_nStringBlockSize;

	//---------------------------------------------
	//Create String Blok
	m_pStringBuf = new CHAR[theHead.m_nStringBlockSize];
	if(!m_pStringBuf) return FALSE;

	//------------------------------------------------------
	// Read Field Types
	std::vector< UINT > vFieldType;
	vFieldType.resize(theHead.m_nFieldsNum);
	fread(&(vFieldType[0]), sizeof(UINT), theHead.m_nFieldsNum, fp);

	//Check it!
	m_theType.resize(theHead.m_nFieldsNum);
	for(INT i=0; i<(INT)theHead.m_nFieldsNum; i++)
	{
		switch(vFieldType[i])
		{
		case T_INT:
		case T_FLOAT:
		case T_STRING:
			m_theType[i] = (FIELD_TYPE)vFieldType[i];
			break;

		default:
			delete[] (m_pStringBuf);
			fclose(fp); return FALSE;
		}
	}

	//------------------------------------------------------
	// Read All Field
	m_vDataBuf.resize(theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	fread(&(m_vDataBuf[0]), sizeof(FIELD), theHead.m_nRecordsNum * theHead.m_nFieldsNum, fp);

	//------------------------------------------------------
	// Read String Block
	fread(m_pStringBuf, m_nStringBufSize, 1, fp);

	fclose(fp); fp = NULL;
	return TRUE;
}

BOOL DBCFile::OpenFromTXT(const CHAR* szFileName)
{
	assert(szFileName);

	//----------------------------------------------------
	//打开文件
	FILE* fp = fopen(szFileName, "r");
	if(NULL == fp) return FALSE;

	//----------------------------------------------------
	//分析列数和类型
	CHAR szLine[1024*10];
	//读第一行
	fgets(szLine, 1024*10, fp);
	size_t nLen;
	if((nLen=strlen(szLine)) != 0)
		if(szLine[nLen-1] == '\r' || szLine[nLen-1] == '\n') szLine[nLen-1] = '\0';
	if((nLen=strlen(szLine)) != 0) 
		if(szLine[nLen-1] == '\r' || szLine[nLen-1] == '\n') szLine[nLen-1] = '\0';

	//分解
	std::vector< std::string > vRet;
	_ConvertStringToVector(szLine, vRet, "\t", TRUE, TRUE);
	if(vRet.empty()) return FALSE;
	//生成Field Types
	FILEDS_TYPE vFieldsType;
	vFieldsType.resize(vRet.size());

	for(INT i=0; i<(INT)vRet.size(); i++) 
	{
		if(vRet[i] == "INT") vFieldsType[i] = T_INT;
		else if(vRet[i] == "FLOAT") vFieldsType[i] = T_FLOAT;
		else if(vRet[i] == "STRING") vFieldsType[i] = T_STRING;
		else
		{
			fclose(fp);
			return FALSE;
		}
	}

	//--------------------------------------------------------------
	//初始化
	INT nRecordsNum	= 0;
	INT nFieldsNum	= (INT)vFieldsType.size();

	//临时字符串区
	std::vector< std::pair< std::string, INT > >	vStringBuf;
	//检索表
	std::map< std::string, INT >					mapStringBuf;

	//--------------------------------------------------------------
	//开始读取
	fgets(szLine, 1024*10, fp);		//空读一行

	INT nStringBufSize = 0;
	do
	{
		//读取一行
		if(NULL == fgets(szLine, 1024*10, fp)) break;
		if((nLen=strlen(szLine)) == 0) continue; 
		if(szLine[nLen-1] == '\r' || szLine[nLen-1] == '\n') szLine[nLen-1] = '\0';
		if((nLen=strlen(szLine)) == 0) continue; 
		if(szLine[nLen-1] == '\r' || szLine[nLen-1] == '\n') szLine[nLen-1] = '\0';

		//分解
		_ConvertStringToVector(szLine, vRet, "\t", TRUE, FALSE);

		//列数不对
		if(vRet.empty()) continue;
        if(vRet.size() != nFieldsNum) 
		{
			//补上空格
			if((INT)vRet.size() < nFieldsNum)
			{
				INT nSubNum = nFieldsNum-(INT)vRet.size();
				for(INT i=0; i<nSubNum; i++)
				{
					vRet.push_back("");
				}
			}
		}

		//第一列不能为空
		if(vRet[0].empty()) continue;

		for(register INT i=0; i<(INT)vRet.size(); i++)
		{
			FIELD newField;
			switch(vFieldsType[i])
			{
			case T_INT:
				newField.iValue = atoi(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_FLOAT:
				newField.fValue = (FLOAT)atof(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_STRING:
				if(vRet[i].empty())
				{
					newField.iValue = 0;
				}
				else
				{
					const CHAR * p = vRet[i].c_str();
					std::map< std::string, INT >::iterator it = mapStringBuf.find(vRet[i]);
					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[i], nStringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[i], (INT)vStringBuf.size()-1));
						newField.iValue = nStringBufSize + 1; // first CHAR is '\0' for blank string
	
						nStringBufSize += (INT)strlen(vRet[i].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				m_vDataBuf.push_back(newField);
				break;
			}
		}

		nRecordsNum++;
	}while(TRUE);
	fclose(fp);

	//--------------------------------------------------------
	//生成正式数据库
	m_nRecordsNum = nRecordsNum;
	m_nFieldsNum  = nFieldsNum;
	m_nStringBufSize = nStringBufSize+1;

	//Create String Blok
	m_pStringBuf = new CHAR[m_nStringBufSize];
	if(!m_pStringBuf) return FALSE;

	//------------------------------------------------------
	// Create Field Types
	m_theType = vFieldsType;

	//------------------------------------------------------
	// Create String Block
	UCHAR byBlank = '\0';
	m_pStringBuf[0] = '\0';

	register CHAR* p = m_pStringBuf + 1;
	for(INT i=0; i<(INT)vStringBuf.size(); i++)
	{
		memcpy(p, vStringBuf[i].first.c_str(), vStringBuf[i].first.size());	
		p += vStringBuf[i].first.size();

		*(p++) = '\0';
	}

	//------------------------------------------------------
	// Relocate String Block
	for(register INT i=0; i<nFieldsNum; i++)
	{
		if(vFieldsType[i] != T_STRING) continue;

		for(register INT j=0; j<nRecordsNum; j++)
		{
			FIELD& theField = m_vDataBuf[j*nFieldsNum+i];
			theField.pString = m_pStringBuf + theField.iValue;
		}
	}

	//------------------------------------------------------
	//生成索引列
	CreateIndex(szFileName, 0);

	return TRUE;
}

VOID DBCFile::CreateIndex(LPCTSTR szFileName, INT nColum)
{
	if(nColum <0 || nColum >= m_nFieldsNum || m_nIndexColum==nColum) return;

	m_hashIndex.clear();

	for(INT i=0; i<(INT)m_nRecordsNum; i++)
	{
		FIELD* p = &(m_vDataBuf[i*m_nFieldsNum]);

		stdext::hash_map< INT, FIELD* >::iterator itFind = m_hashIndex.find(p->iValue);
		if(itFind != m_hashIndex.end())
		{
			CHAR szTemp[256];
			_snprintf(szTemp, 256, "DBC[%s] Multi index at line %d", szFileName, i+1);
			throw std::exception(szTemp);
			return;
		}
		m_hashIndex.insert(std::make_pair(p->iValue, p));
	}
}

const DBCFile::FIELD* DBCFile::Search_Index_EQU(INT iIndex) const
{
	stdext::hash_map< INT, FIELD* >::const_iterator itFind = m_hashIndex.find(iIndex);
	if(itFind == m_hashIndex.end()) return NULL;

	return itFind->second;
}

const DBCFile::FIELD* DBCFile::Search_Posistion(INT nRecordLine, INT nColumNum) const
{
	INT nPosition = nRecordLine*GetFieldsNum() + nColumNum;

	if(nPosition <0 || nColumNum >= (INT)m_vDataBuf.size()) return NULL;

	return &(m_vDataBuf[nPosition]);
}


}