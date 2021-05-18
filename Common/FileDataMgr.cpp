#include "stdafx.h"
// FileDataMgr.cpp

#include <string.h>
#include <stdlib.h>

#include "FileDataMgr.h"

#define MGR_TXT	("MGR_TXT")
#define MGR_BIN	("MGR_BIN")

#define MGR_HEAD_SIZE	(7)

struct SFileDataLink
{
	SFileDataLink	*m_pPrev;
	SFileDataLink	*m_pNext;
	SFileData		*m_pData;
};

SFileData::SFileData( VOID )
{
	m_uID			= 0xFFFFFFFF;
	memset( m_szFileName, 0, sizeof ( m_szFileName ) );
}

SFileData::~SFileData( VOID )
{
}

BOOL SFileData::Init( UINT uID, CHAR *pszFileName, FILE *fp )
{
	m_uID			= uID;
	strcpy( m_szFileName, pszFileName );
	return TRUE;
}

VOID SFileData::Term( VOID )
{
	m_uID			= 0xFFFFFFFF;
	memset( m_szFileName, 0, sizeof ( m_szFileName ) );
}


CFileDataMgr::CFileDataMgr( VOID )
{
	m_bInit			= FALSE ;
	m_bArrayMode	= FALSE;

	m_uMinID		= 0xFFFFFFFF;
	m_uMaxID		= 0xFFFFFFFF;

	m_uCount		= 0;
	m_uLength		= 0;
	m_apFileData	= NULL;
}

CFileDataMgr::~CFileDataMgr( VOID )
{
	Term() ;
}

BOOL CFileDataMgr::Init( const CHAR *pszMgrPathFileName, BOOL bForceArrayMode )
{
	if ( pszMgrPathFileName == NULL )
		return FALSE;

	FILE *fp = fopen( pszMgrPathFileName, "rb" );
	if ( fp == NULL )
		return FALSE;

	BOOL bResult;
	CHAR szHead[8];	// MGR_TXT, MGR_BIN

	INT nFileSize, nFileCur;
	nFileCur = ftell( fp );
	fseek( fp, 0L, SEEK_END );
	nFileSize = ftell( fp );
	fseek( fp, nFileCur, SEEK_SET );

	nFileSize -= nFileCur;
	if ( nFileSize < MGR_HEAD_SIZE )
	{
		fclose( fp );
		return FALSE;
	}

	fread( szHead, MGR_HEAD_SIZE, 1, fp );
	szHead[7] = '\0';
	if ( 0 == tstricmp( szHead, MGR_TXT ) )
	{
		m_uMinID		= 0xFFFFFFFF;
		m_uMaxID		= 0;

		m_uCount		= 0;

		CHAR szMgrPath[260];
		CHAR szMgrFolderName[260];
		CHAR szItemPathFileName[260];
		{
			memset( szMgrPath, 0, sizeof ( szMgrPath ) );
			size_t nPathFileNameLen = strlen( pszMgrPathFileName );
			while ( nPathFileNameLen > 0 )
			{
				if ( pszMgrPathFileName[nPathFileNameLen] == '\\' || pszMgrPathFileName[nPathFileNameLen] == '/' )
				{
					memcpy( szMgrPath, pszMgrPathFileName, nPathFileNameLen+1 );
					szMgrPath[nPathFileNameLen] = '/';
					szMgrPath[nPathFileNameLen+1]='\0';

					strcpy( szMgrFolderName, &pszMgrPathFileName[nPathFileNameLen+1] );
					INT nFolderNameIndex = 0;
					while( TRUE )
					{
						if ( szMgrFolderName[nFolderNameIndex] == '.' || szMgrFolderName[nFolderNameIndex] == '\0' )
						{
							szMgrFolderName[nFolderNameIndex] = '/';
							szMgrFolderName[nFolderNameIndex+1] = '\0';
							break;
						}
						nFolderNameIndex++;
					}
					break;
				}
				nPathFileNameLen--;
			}
		}

		// 分析文件
		SFileDataLink *pHead;
		pHead = NULL;

		nFileSize -= MGR_HEAD_SIZE + 1;
		CHAR *szBuffer, *szTempBuffer;
		szBuffer = new CHAR [nFileSize+1];
		szTempBuffer = new CHAR [nFileSize+1];
		fread( szTempBuffer, nFileSize, 1, fp );
		szTempBuffer[nFileSize] = '\0';

		INT nFiltrateIndex = 0;
		INT nValidIndex = 0;
		while( szTempBuffer[nFiltrateIndex] )
		{
			if ( szTempBuffer[nFiltrateIndex] == ';' )
			{
				while( szTempBuffer[nFiltrateIndex] && szTempBuffer[nFiltrateIndex] != '\n' )
				{
					nFiltrateIndex++;
				}
				continue;
			}
			szBuffer[nValidIndex] = szTempBuffer[nFiltrateIndex];
			nFiltrateIndex++;
			nValidIndex++;
		}
		szBuffer[nValidIndex] = '\0';

		SAFE_DELETE_ARRAY(szTempBuffer) ;

		INT nValidSize = nValidIndex + 1;

		CHAR *szIDString;
		CHAR *szFileNameString;
		long nCurIndex;
		nCurIndex = 0;
		while( nCurIndex < nValidSize )
		{
			if ( szBuffer[nCurIndex] >= '0' && szBuffer[nCurIndex] <= '9' )
			{
				szIDString = &szBuffer[nCurIndex];
				BOOL bSucceeded = FALSE;
				BOOL bFailed	= FALSE;
				while( nCurIndex < nValidSize )
				{
					switch ( szBuffer[nCurIndex] )
					{
					case '=':
						bSucceeded = TRUE;
						nCurIndex++;
						while( nCurIndex < nValidSize )
						{
							if ( szBuffer[nCurIndex] != ' ' && szBuffer[nCurIndex] != '\t' )
							{
								break;
							}
							nCurIndex++;
						}
						break;
					case '\n':
						bFailed = TRUE;
						nCurIndex++;
						break;
					default:
						nCurIndex++;
						break;
					}

					if ( bSucceeded || bFailed )
						break;
				}

				if ( bSucceeded )
				{
					szFileNameString =  &szBuffer[nCurIndex];
					while( nCurIndex < nValidSize )
					{
						if ( szBuffer[nCurIndex] == '\t' || szBuffer[nCurIndex] == ' ' )
						{
							szBuffer[nCurIndex] = '\0';
						}

						if ( szBuffer[nCurIndex] == '\n' || szBuffer[nCurIndex] == '\r' )
						{
							szBuffer[nCurIndex] = '\0';
							break;
						}
						nCurIndex++;
					}

					for(INT i =0;i<260;i++)
					{
						if(szFileNameString[i]=='\\')
							szFileNameString[i]='/';
						if(szFileNameString[i]=='\0')
							break;
					}

					strcpy( szItemPathFileName, szMgrPath );
					strcat( szItemPathFileName, szMgrFolderName );
					strcat( szItemPathFileName, szFileNameString );
				
					SFileData *pFileData;
					bResult = OnNewFileData( &pFileData );
					if ( bResult )
					{
						FILE *fpItem = fopen( szItemPathFileName, "rb" );
						UINT uItemID = (UINT)(atoi( szIDString ));
						bResult = pFileData->Init( uItemID, szFileNameString, fpItem );
						if ( fpItem != NULL )
							fclose( fpItem );

						if ( bResult )
						{
							if ( pHead == NULL )
							{
								pHead = new SFileDataLink;
								pHead->m_pPrev = pHead->m_pNext = NULL;
								pHead->m_pData = pFileData;
							}
							else
							{
								SFileDataLink *pLink = new SFileDataLink;
								pLink->m_pPrev = NULL;
								pLink->m_pNext = pHead;
								pHead->m_pPrev = pLink;
								pLink->m_pData = pFileData;
								pHead = pLink;
							}

							if ( pFileData->GetID() < m_uMinID )
								m_uMinID = pFileData->GetID();

							if ( pFileData->GetID() > m_uMaxID )
								m_uMaxID = pFileData->GetID();

							m_uCount++;
						}
					}
				}
			}
			else if ( szBuffer[nCurIndex] != ' ' && szBuffer[nCurIndex] != '\t' )
			{
				while( nCurIndex < nValidSize )
				{
					if ( szBuffer[nCurIndex] == '\n' )
					{
						break;
					}
					nCurIndex++;
				}
			}
			nCurIndex++;
		}

		if ( bForceArrayMode )
		{
			m_bArrayMode = TRUE;
		}
		else
		{
			if ( m_uMaxID - m_uMinID < AUTO_ARRAY_MODE_MAX_LEN )
			{
				m_bArrayMode = TRUE;
			}
			else
			{
				m_bArrayMode = FALSE;
			}
		}

		if ( m_bArrayMode )
		{
			m_uLength = m_uMaxID - m_uMinID + 1;
			if ( m_uLength > 0 )
			{
				m_apFileData = new SFileData* [m_uLength];
				memset( m_apFileData, 0, sizeof ( SFileData* ) * m_uLength );

				while( pHead != NULL )
				{
					m_apFileData[pHead->m_pData->GetID()-m_uMinID] = pHead->m_pData;
					SFileDataLink *pPrevLink = pHead;
					pHead = pHead->m_pNext;
					delete pPrevLink;
				}
			}
		}
		else
		{
			m_uLength = m_uCount;
			if ( m_uLength > 0 )
			{
				m_apFileData = new SFileData* [m_uLength];
				memset( m_apFileData, 0, sizeof ( SFileData* ) * m_uLength );

				// 排序
				SFileDataLink *pCurLink = pHead;
				SFileDataLink *pNextLink;
				while( pCurLink != NULL )
				{
					pNextLink = pCurLink->m_pNext;
					if ( pCurLink->m_pPrev != NULL )
					{
						SFileDataLink *pSortPrev;
						SFileDataLink *pSortCur;
						pSortCur = pCurLink;
						pSortPrev = pCurLink->m_pPrev;
						while ( pSortPrev != NULL )
						{
							if ( pSortCur->m_pData->GetID() < pSortPrev->m_pData->GetID() )
							{
								pSortCur->m_pPrev = pSortPrev->m_pPrev;
								if ( pSortPrev->m_pPrev != NULL )
									pSortPrev->m_pPrev->m_pNext = pSortCur;

								pSortPrev->m_pNext = pSortCur->m_pNext;
								if ( pSortCur->m_pNext != NULL )
									pSortCur->m_pNext->m_pPrev = pSortPrev;

								pSortCur->m_pNext = pSortPrev;
								pSortPrev->m_pPrev = pSortCur;
							}
							else
							{
								break;
							}

							pSortPrev = pSortCur->m_pPrev;
							if ( pSortPrev == NULL )
								pHead = pSortCur;
						}
					}
					pCurLink = pNextLink;
				}

				UINT uIndex;
				uIndex = 0;
				while( pHead != NULL )
				{
					m_apFileData[uIndex++] = pHead->m_pData;
					SFileDataLink *pPrevLink = pHead;
					pHead = pHead->m_pNext;
					delete pPrevLink;
				}
			}
		}

		SAFE_DELETE_ARRAY( szBuffer );
		fclose( fp );
		m_bInit = TRUE ;
		return TRUE;
	}
	else if ( 0 == tstricmp( szHead, MGR_BIN ) )
	{
		UINT	uVersion;
		UINT	uReadItemCount;
		UINT	uItemID;
		CHAR				szItemFileName[FILE_DATA_NAME_LEN];

		fread( &uVersion, sizeof ( uVersion ), 1, fp );
		fread( &uReadItemCount, sizeof ( uReadItemCount ), 1, fp );
		fread( &m_uMinID, sizeof ( m_uMinID ), 1, fp );
		fread( &m_uMaxID, sizeof ( m_uMaxID ), 1, fp );

		if ( bForceArrayMode )
		{
			m_bArrayMode = TRUE;
		}
		else
		{
			if ( m_uMaxID - m_uMinID < AUTO_ARRAY_MODE_MAX_LEN )
			{
				m_bArrayMode = TRUE;
			}
			else
			{
				m_bArrayMode = FALSE;
			}
		}

		if ( m_bArrayMode )
		{
			m_uLength = m_uMaxID - m_uMinID + 1;
			if ( m_uLength > 0 )
			{
				m_apFileData = new SFileData* [m_uLength];
				memset( m_apFileData, 0, sizeof ( SFileData* ) * m_uLength );

				UINT  i;
				SFileData *pFileData;
				for ( i = 0; i < uReadItemCount; i++ )
				{
					bResult = OnNewFileData( &pFileData );
					if ( !bResult )
					{
						fclose( fp);
						return FALSE;
					}
					fread( &uItemID, sizeof ( uItemID ), 1, fp );
					fread( szItemFileName, sizeof ( szItemFileName ), 1, fp );
					bResult = pFileData->Init( uItemID, szItemFileName, fp );
					if ( !bResult )
					{
						fclose( fp);
						return FALSE;
					}
					if ( pFileData->GetID() < m_uMaxID )
					{
						m_apFileData[pFileData->GetID() - m_uMinID] = pFileData;
						m_uCount++;
					}
					else
					{
						pFileData->Term();
						delete pFileData;
					}
				}
			}
		}
		else
		{
		}
		fclose( fp );
		m_bInit = TRUE ;
		return TRUE;
	}
	else
	{
		fclose( fp );
		return FALSE;
	}
}

VOID CFileDataMgr::Term( VOID )
{
	m_bInit = FALSE ;
	m_bArrayMode	= FALSE;

	m_uMinID		= 0xFFFFFFFF;
	m_uMaxID		= 0xFFFFFFFF;

	m_uCount		= 0;
	if ( m_apFileData != NULL )
	{
		UINT i;
		for ( i = 0; i < m_uLength; i++ )
		{
			if ( m_apFileData[i] != NULL )
			{
				m_apFileData[i]->Term();
				delete m_apFileData[i];
			}
		}
		m_uLength		= 0;
		delete [] m_apFileData;
		m_apFileData = NULL;
	}
}

SFileData *CFileDataMgr::GetFileData( UINT uID )
{
	if ( m_bArrayMode )
	{
		return (uID - m_uMinID < m_uLength)?(m_apFileData[uID - m_uMinID]):(NULL);
	}
	else
	{
		if ( uID < m_uMinID || uID > m_uMaxID )
			return NULL;

		// 二分查找
		UINT uMinID, uMaxID;
		uMinID = m_uMinID;
		uMaxID = m_uMaxID;

		UINT uMinIndex, uMaxIndex, uCurIndex, uCurID;
		uMinIndex = 0;
		uMaxIndex = m_uLength - 1;
		uCurIndex = ((uMaxIndex - uMinIndex) >> 1) + uMinIndex;
		
		while ( TRUE )
		{
			uCurID = m_apFileData[uCurIndex]->GetID();
			if ( uCurID > uID )
			{
				uMaxIndex = uCurIndex;
			}
			else if ( uCurID < uID )
			{
				uMinIndex = uCurIndex;
			}
			else
			{
				break;
			}

			if ( uMaxIndex - uMinIndex > 1 )
			{
				uCurIndex = ((uMaxIndex - uMinIndex) >> 1) + uMinIndex;
			}
			else
			{
				if ( m_apFileData[uMinIndex]->GetID() == uID )
					uCurIndex = uMinIndex;
				else if ( m_apFileData[uMaxIndex]->GetID() == uID )
					uCurIndex = uMaxIndex;
				else
					uCurIndex = 0xFFFFFFFF;
				break;
			}
		}

		if ( uCurIndex != 0xFFFFFFFF )
			return m_apFileData[uCurIndex];
		else
			return NULL;
	}
}

const SFileData *CFileDataMgr::GetConstFileData( UINT uID )const
{
	return const_cast<CFileDataMgr*>(this)->GetFileData( uID );
}

BOOL CFileDataMgr::OnNewFileData( SFileData **ppFileData )
{
	if ( ppFileData != NULL )
	{
		*ppFileData = new SFileData;
		return TRUE;
	}
	return FALSE;
}

VOID CFileDataMgr::OnDeleteFileData( SFileData **ppFileData )
{
	if ( ppFileData != NULL )
	{
		delete *ppFileData;
		*ppFileData = NULL;
	}
}
