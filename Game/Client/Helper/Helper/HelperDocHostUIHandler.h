//
// HelperDocHostUIHandler
//			ref: http://siney.nease.net/article/newintf.htm
//			ref: http://msdn.microsoft.com/library/default.asp?url=/workshop/browser/hosting/wbcustomization.asp
//


#pragma once


class HelperCommandDispatch : public IDispatch
{
public:
	HelperCommandDispatch() :m_refcount(1){ }
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
		/* [out] */ UINT *pctinfo)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ ITypeInfo **ppTInfo)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR *rgszNames,
		/* [in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID *rgDispId);

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID classid, void** intf) 
	{
		if (classid == IID_IDispatch)
		{
			*intf = (IDispatch*)this;
			AddRef();
		}
		else
			return E_NOINTERFACE;
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef() 
	{
		InterlockedIncrement(&m_refcount);
		return m_refcount;
	}

	virtual ULONG STDMETHODCALLTYPE Release() 
	{
		InterlockedDecrement(&m_refcount);
		if (m_refcount == 0)
			delete this;
		return m_refcount;
	}

private:
	long m_refcount;
};

///////////////////////////////////////////////////////////////////
//
class HelperDocHostUIHandler : public IDocHostUIHandler
{
public:
	HelperDocHostUIHandler() :m_refcount(1){ }

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID classid, void** intf) 
	{
		if (classid == IID_IUnknown)
		{
			*intf = (IUnknown*)this;
			AddRef();
		}
		else if (classid == IID_IDocHostUIHandler)
		{
			*intf = (IDocHostUIHandler*)this;
			AddRef();
		}
		else if (classid == IID_IDispatch)
		{
			*intf = (IDispatch*)this;
			AddRef();
		}
		else
			return E_NOINTERFACE;
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef() 
	{
		InterlockedIncrement(&m_refcount);
		return m_refcount;
	}

	virtual ULONG STDMETHODCALLTYPE Release() 
	{
		InterlockedDecrement(&m_refcount);
		if (m_refcount == 0)
			delete this;
		return m_refcount;
	}

	//·µ»ØS_OK£¬ÆÁ±ÎµôÓÒ¼ü²Ëµ¥
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo) 
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ShowUI(
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
		/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE HideUI(void)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI(void) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableModeless(
		/* [in] */ BOOL fEnable) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(
		/* [in] */ BOOL fActivate) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(
		/* [in] */ BOOL fActivate) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder(
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
		/* [in] */ BOOL fRameWindow)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
		/* [in] */ DWORD nCmdID) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(
		/* [out] */ LPOLESTR __RPC_FAR *pchKey,
		/* [in] */ DWORD dw) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget(
		/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
		/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget) 
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetExternal(
		/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch) 
	{
		*ppDispatch = new HelperCommandDispatch();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl(
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
		/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject(
		/* [in] */ IDataObject __RPC_FAR *pDO,
		/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet) 
	{
		return E_NOTIMPL;
	}

private:
	long	m_refcount;
};
