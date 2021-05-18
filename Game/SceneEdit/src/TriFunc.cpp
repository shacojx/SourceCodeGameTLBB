#include "TriFunc.h"

// 是否选中一个三角型
bool IsSelTri(
				Ogre::Vector3 dir, 
				Ogre::Vector3 orig, 
				Ogre::Vector3 v1, 
				Ogre::Vector3 v2, 
				Ogre::Vector3 v3,
				float& t, 
				float& u, 
				float& v 
				)
{

	
	float    fu = 0;
	float    fv = 0;
	float    fDist = 0;
	float    fDet  = 0;
	
	Ogre::Vector3 edge1 = v2 - v1;
	Ogre::Vector3 edge2 = v3 - v1;

	Ogre::Vector3 pvec;
	pvec = dir.crossProduct(edge2);
	fDet = edge1.dotProduct(pvec);
  
    Ogre::Vector3 tvec;
    if( fDet > 0 )
    {
        tvec = orig - v1;
    }
    else
    {
        tvec = v1 - orig;
        fDet = -fDet;
    }

    if( fDet < 0.0001f )
        return false;

    fu = tvec.dotProduct(pvec);
    if( fu < 0.0f || fu > fDet )
	{
        return false;
	}

    // Prepare to test V parameter
    Ogre::Vector3 qvec;
	qvec = tvec.crossProduct(edge1);

    // Calculate V parameter and test bounds
	fv = dir.dotProduct(qvec);
    if( fv < 0.0f || fu + fv > fDet )
	{
        return false;
	}

	fDist = edge2.dotProduct(qvec);
    float fInvDet = 1.0f / fDet;
    fDist *= fInvDet;
    fu *= fInvDet;
    fv *= fInvDet;

	t = fDist;
	u = fu; 
	v = fv;

  return true;

}
