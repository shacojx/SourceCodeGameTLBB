#include "StdAfx.h"
#include "HelperDocHostUIHandler.h"
#include "HelperSystem.h"

HRESULT HelperCommandDispatch::GetIDsOfNames( 
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID *rgDispId)
{
	WCHAR wszMethod_1[] = L"HelloWorld";

	if(memcmp(rgszNames[0], wszMethod_1, sizeof(wszMethod_1)) == 0)
	{
		*rgDispId = 1;
		return S_OK;
	}

	*rgDispId=0;
	return E_NOINTERFACE ;
}

HRESULT HelperCommandDispatch::Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr)
{
	if(dispIdMember==1)
	{
		MessageBox(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), "Hello World","Hello",0); //place your code here
	}

	return S_OK;
}

HRESULT HelperDocHostUIHandler::TranslateUrl(
	/* [in] */ DWORD dwTranslate,
	/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
	/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut) 
{
	CStringW strURLIn(pchURLIn);

	CStringW strProtol = strURLIn.Mid(0, 7);
	if(strProtol.CompareNoCase(L"anch://")==0)
	{
		CString strAnchor = CString(strURLIn.Mid(7));
		strAnchor.TrimRight("/\\");
		CHelperSystem::GetMe()->GotoAnchor((LPCTSTR)strAnchor);

		return S_OK;
	}

	return E_NOTIMPL;
}
