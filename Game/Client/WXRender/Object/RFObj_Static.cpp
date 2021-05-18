#include "StdAfx.h"
#include "RFObj_Static.h"
#include "TDException.h"

#define		FOBJ_MESH_FILE			_T("mesh name")				//静态物体网格文件

CRFObj_Static::CRFObj_Static()
	: CWXObj_Object("StaticEntity")
{
}

CRFObj_Static::~CRFObj_Static()
{
}

VOID CRFObj_Static::StaObj_SetMeshFile(LPCTSTR szMeshFile)
{
	TDAssert(szMeshFile && m_pWXObject);

	m_pWXObject->setProperty(FOBJ_MESH_FILE, WX::String(szMeshFile));
}

VOID CRFObj_Static::SetVisible(BOOL bVisible)
{
	TDThrow("(Hide static object)Not Support yet!");

}
