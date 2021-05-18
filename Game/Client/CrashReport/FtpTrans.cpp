#include "stdafx.h"
#include "FtpTrans.h"

CFtpTrans::CFtpTrans()
{
	strAppName.SetString("CreshReport1.0");
	m_pInternetSession = new CInternetSession(strAppName, INTERNET_OPEN_TYPE_PRECONFIG);
}

CFtpTrans::~CFtpTrans()
{
	if( m_pInternetSession )
	{
		m_pInternetSession->Close();
		delete m_pInternetSession;
	}
}


bool CFtpTrans::SetAccessRight(CString userName, CString userPass)
{
	strPass = userPass;
	strUser = userName;
	return true;
}

bool CFtpTrans::OpenConnection(CString server)
{
	if(server == "") return 0;
	strServerName = server;

	try
	{
		m_pFtpConnection = m_pInternetSession->GetFtpConnection(strServerName, strUser, strPass);
	}
	catch(...) 
	{
		return false;
	}
	return true;
}

bool CFtpTrans::GetFile(CString remoteFile, CString localFile)
{
	BOOL bGotFile = m_pFtpConnection->GetFile(remoteFile,
		localFile,
		FALSE,
		FILE_ATTRIBUTE_NORMAL,	
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE);

	if( bGotFile ) return true;
	return false;
}

int CFtpTrans::GetMultipleFile(CStringArray *remoteArray, CStringArray *localArray, int number_file)
{
	// init some var
	BOOL goodfile;
	int x=0;
	int nb_lost_file =0;

	while(x<number_file)
	{
		goodfile = m_pFtpConnection->GetFile(remoteArray->GetAt(x),
			localArray->GetAt(x),
			FALSE,
			FILE_ATTRIBUTE_NORMAL,
			FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE);

		missed[x] = goodfile ? 0 : 1;
		if(missed[x])
			nb_lost_file++;
		x++;
	}
	return nb_lost_file;
}

bool CFtpTrans::PutFile(CString localFile, CString remoteFile)
{
	BOOL bPutFile = m_pFtpConnection->PutFile(localFile,
		remoteFile);

	if( bPutFile ) return true;
	return false;
}

bool CFtpTrans::CloseConnection()
{
	if(m_pFtpConnection == NULL) return false;

	try
	{
		m_pFtpConnection->Close();
	}
	catch(...)
	{
		return false;
	}

	delete m_pFtpConnection;
	return true;
}