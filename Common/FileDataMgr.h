// FileDataMgr.h
//
///////////////////////////////////////////////////////

#pragma once

#include "Type.h"
#include <stdio.h>

#define FILE_DATA_NAME_LEN		(128)
#define AUTO_ARRAY_MODE_MAX_LEN	(2048)


struct SFileData
{
public:
	SFileData( VOID );
	virtual ~SFileData( VOID );

	virtual BOOL Init( UINT uID, CHAR *pszFileName, FILE *fp );
	virtual VOID Term( VOID );

public:
	UINT GetID( VOID )const{
		return m_uID;
	}

	const CHAR *GetFileName( VOID )const{
		return m_szFileName;
	}

protected:
	UINT		m_uID;
	CHAR					m_szFileName[FILE_DATA_NAME_LEN];
};


class CFileDataMgr
{
public:
	CFileDataMgr( VOID );
	virtual ~CFileDataMgr( VOID );

	BOOL Init( const CHAR *pszPathFileName, BOOL bForceArrayMode );
	VOID Term( VOID );

	SFileData *GetFileData( UINT uID );
	const SFileData *GetConstFileData( UINT uID )const;

	BOOL IsInit( ){ return m_bInit ; }

protected:
	virtual BOOL OnNewFileData( SFileData **ppFileData );
	virtual VOID OnDeleteFileData( SFileData **ppFileData );

private:
	BOOL				m_bInit ;
	BOOL				m_bArrayMode;

	UINT	m_uMinID;
	UINT	m_uMaxID;

	UINT	m_uCount;
	UINT	m_uLength;
	SFileData			**m_apFileData;
};
