#include "StdAfx.h"
#include "RFObj_Model.h"
#include "TDException.h"

#define		FOBJ_MODEL_FILE			_T("model name")	//动态模型资源属性

CRFObj_Model::CRFObj_Model()
	: CWXObj_Object("Model")
{
}

CRFObj_Model::~CRFObj_Model()
{
}

VOID CRFObj_Model::StaObj_SetMeshFile(LPCTSTR szMeshFile)
{
	TDAssert(szMeshFile && m_pWXObject);

	m_pWXObject->setProperty(FOBJ_MODEL_FILE, WX::String(szMeshFile));
}

VOID CRFObj_Model::SetVisible(BOOL bVisible)
{
	TDThrow("(Hide static object)Not Support yet!");

}
