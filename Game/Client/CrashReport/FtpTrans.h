#ifndef __FTPTRANC_H__
#define __FTPTRANC_H__

#include <afxinet.h>
class CFtpTrans
{
public:
	CFtpTrans();
	virtual ~CFtpTrans();

	bool SetAccessRight(CString userName,CString userPass);

	bool OpenConnection(CString server);

	bool GetFile(CString remoteFile, CString localFile);
	int GetMultipleFile(CStringArray *remoteArray, CStringArray *localArray, int number_file);

	bool PutFile(CString localFile, CString remoteFile);

	bool CloseConnection();
private:
	CInternetSession *m_pInternetSession;
	CFtpConnection *m_pFtpConnection;
	CString strServerName;
	CString strPass;
	CString strUser;
public:
	CString strAppName;
	bool missed[100];

};

#endif 
