

#include "SkeletonMeshUtil.h"
#include "SkeletonMesh.h"
#include "OgreException.h"
#include "OgreDataStream.h"
#include "OgreResourceGroupManager.h"
#include "OgreSceneManager.h"
#include "OgreStringConverter.h"
#include "OgreBone.h"

#define TIXML_USE_STL
#include <tinyxml.h>
#define TiXML

bool	SkeletonMeshUtil::Load(	DObject_NT* pMesh, 
								const DString strObjGroup, 
								const DString& strXmlFile, 
								const StringList& attribNames,
								const StringList& attribValues)
{
	if(!pMesh)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, 
			"Error Loading SkeletonMesh FileName ="+strXmlFile+" Group ="+strObjGroup,
			"SkeletonMeshUtil.Load");
	}
	
	const char* pTemp = NULL;
	TiXML::TiXmlDocument xmldoc;
	TiXML::TiXmlElement* pObject;
	TiXML::TiXmlElement* pAttribs;
	TiXML::TiXmlElement* pEntityList;
	TiXML::TiXmlElement* pLocators;

	StringList tempAttribNames, tempAttribValues;

	long attribidx;

	pMesh->SetGroupName(strObjGroup);

	pMesh->SetObjFileName(strXmlFile);

	if(strObjGroup.size() == 0)
	{
		if(!xmldoc.LoadFile(strXmlFile.c_str())) return false;
	}
	else
	{
		Ogre::DataStreamPtr dataptr = Ogre::ResourceGroupManager::getSingleton().openResource(strXmlFile, strObjGroup);
		xmldoc.Parse(dataptr->getAsString().c_str());
		if(xmldoc.Error())
			return false;
	}

	pObject = xmldoc.RootElement();
		if(!pObject) return false;
	
	pTemp = pObject->Attribute("name");
	
	//if(pTemp) pMesh->SetObjFileName(pTemp);

	
	pMesh->SetObjectNode(pMesh->GetSceneMgr()->createSceneNode());	

	pMesh->ReSet();

	//load object attributes
	pAttribs = pObject->FirstChildElement("Attributes");
	if(pAttribs)
	{
		TiXML::TiXmlElement* pAttrib;
		pAttrib = pAttribs->FirstChildElement("Attribute");
		while(pAttrib)
		{
			DString			name, value, hints;
			//load attribute name
			pTemp = pAttrib->Attribute("name");
			if(!pTemp)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> One Attribute haven't name, skipped!", 
						pMesh->GetObjFileName().c_str(), 
						pMesh->GetObjName().c_str());

				pAttrib = pAttrib->NextSiblingElement("Attribute");
				continue;
			}
			name = pTemp;
			//load attribute value
			pTemp = pAttrib->Attribute("value");
			if(!pTemp) value = "";
			else value = pTemp;
			//load attribute hints
			pTemp = pAttrib->Attribute("hints");
			if(!pTemp) hints = "";
			else hints = pTemp;
			//add attribute to attribute map
			if(!pMesh->_AddAttrib(name, value, hints))
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load attribute \'%s\', skipped!", 
						pMesh->GetObjFileName().c_str(),
						pMesh->GetObjName().c_str(),
						name.c_str());
			}
			tempAttribNames.push_back(name);
			tempAttribValues.push_back(value);
			pAttrib = pAttrib->NextSiblingElement("Attribute");
		}
	}

	//update object attributes
	for(attribidx = 0; attribidx < (long)tempAttribNames.size(); attribidx ++)
	{
		pMesh->_SetAttribValue(tempAttribNames[attribidx], tempAttribValues[attribidx]);
	}

	//load object mesh list
	pEntityList = pObject->FirstChildElement("EntityList");
	if(pEntityList)
	{
		TiXML::TiXmlElement* pEntity;
		pEntity = pEntityList->FirstChildElement("Entity");
		while(pEntity)
		{
			DString name, entfile, mat, condition;
			pTemp = pEntity->Attribute("name");
			if(pTemp) name = pTemp; else name = "";
			pTemp = pEntity->Attribute("file");
			if(pTemp) entfile = pTemp; else entfile = "";
			pTemp = pEntity->Attribute("material");
			if(pTemp) mat = pTemp; else mat = "";
			pTemp = pEntity->Attribute("condition");
			if(pTemp) condition = pTemp; else condition = "";
			pMesh->_AddEntity(name, entfile, mat, condition);
			pEntity = pEntity->NextSiblingElement("Entity");
		}
		_LoadEntity(pMesh);
	}

	//load locator info list
	pLocators = pObject->FirstChildElement("Locators");
	if(pLocators)
	{
		TiXML::TiXmlElement* pLocator;
		pLocator = pLocators->FirstChildElement("Locator");
		while(pLocator)
		{
			DString LocatorName, BoneName;
			Ogre::Vector3 LocatorOffPos;
			Ogre::Quaternion LocatorOffRot(Ogre::Quaternion::IDENTITY);
			float qx, qy, qz, qw;

			pTemp = pLocator->Attribute("name");
			if(pTemp) LocatorName = pTemp;
			else
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> The locator have not name, skipped!", 
						pMesh->GetObjFileName().c_str(),
						pMesh->GetObjName().c_str());
				pLocator = pLocator->NextSiblingElement("Locator");
				continue;
			}

			pTemp = pLocator->Attribute("bonename");
			if(pTemp) BoneName = pTemp;
			else BoneName = "";

			LocatorOffPos.x = (float)atof(pLocator->Attribute("x"));
			LocatorOffPos.y = (float)atof(pLocator->Attribute("y"));
			LocatorOffPos.z = (float)atof(pLocator->Attribute("z"));

			qx = (float)atof(pLocator->Attribute("qx"));
			qy = (float)atof(pLocator->Attribute("qy"));
			qz = (float)atof(pLocator->Attribute("qz"));
			qw = (float)atof(pLocator->Attribute("qw"));
			LocatorOffRot.FromAngleAxis(Ogre::Radian(qw), Ogre::Vector3(qx, qy, qz));
			

			if(!pMesh->_AddLocator(LocatorName, BoneName, LocatorOffPos, LocatorOffRot))
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add locator \'%s\', skipped", 
					pMesh->GetObjFileName().c_str(),
					pMesh->GetObjName().c_str(), 
					LocatorName.c_str());
				pLocator = pLocator->NextSiblingElement("Locator");
				continue;
			}

			TiXML::TiXmlElement* pSlot;
			pSlot = pLocator->FirstChildElement("Slot");
			while(pSlot)
			{
				DString SlotName, SlotObj;
				DStringList InitAttribs;

				pTemp = pSlot->Attribute("name");
				if(pTemp) SlotName = pTemp;
				else
				{
					MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add slot because there is no name, skipped!",
							pMesh->GetObjFileName().c_str(),
							pMesh->GetObjName().c_str());
					pSlot = pSlot->NextSiblingElement("Slot");
					continue;
				}

				pTemp = pSlot->Attribute("object");
				if(pTemp) SlotObj = pTemp;
				else SlotObj = "";

				pTemp = pSlot->Attribute("attribs");
				if(pTemp)
				{
					char* pBuf = new char[strlen(pTemp) + 1];
					char* pTok;
					strcpy(pBuf, pTemp);
					pTok = strtok(pBuf, ";");
					while(pTok)
					{
						if(pTok[0] == '$')
							InitAttribs.push_back(pTok);
						pTok = strtok(NULL, ";");
					}
					delete [] pBuf;
				}

				pMesh->_AddSlot(LocatorName, SlotName, SlotObj, InitAttribs);

				pSlot = pSlot->NextSiblingElement("Slot");
			}
			pLocator = pLocator->NextSiblingElement("Locator");
		}
		
	}

	pMesh->UpdateObjAttribute(AH_ALL);

	pMesh->getBoundingBox();

	return true;
}



void	SkeletonMeshUtil::_LoadEntity(DObject_NT* pMesh)
{
	pMesh->_LoadEntity();
}

void	SkeletonMeshUtil::_LoadChildEntity(DObject_NT* pMesh)
{
	pMesh->_LoadChildEntity();
}


bool  SkeletonMeshUtil::Save(DObject_NT* pMesh,
							 DString& StrFileName,
							 bool isBinary /* = false */)
{
	if(!pMesh)
	{
		
	}
	if(isBinary)
	{

	}
	return SaveToXml(pMesh,StrFileName);

}

bool SkeletonMeshUtil::SaveToXml(DObject_NT* pMesh,DString StrFileName)
{
	long i = 0, j = 0;
	FILE* fout;
	fout = fopen(StrFileName.c_str(), "w");
	if(!fout) return false;
	
	fprintf(fout, "<Object name=\"%s\">\n", pMesh->GetObjName().c_str());

	//output attributes
	{
		DObject_NT::_AttribMap::iterator it;
		fprintf(fout, "\t<Attributes>\n");
		for(it = pMesh->m_AttribMap.begin(); it != pMesh->m_AttribMap.end(); it ++)
		{
			DString hints;
			hints = pMesh->Hint2String(it->second.Hints);
			fprintf(fout, "\t\t<Attribute name=\"%s\" value=\"%s\" hints=\"%s\"/>\n", it->first.c_str(), it->second.Value.c_str(), hints.c_str());
		}
		fprintf(fout, "\t</Attributes>\n");
	}

	//output entitys
	{
		DObject_NT::_EntityList::iterator it;
		fprintf(fout, "\t<EntityList>\n");
		for(it = pMesh->m_EntityList.begin(); it != pMesh->m_EntityList.end(); it ++)
		{
			fprintf(fout, "\t\t<Entity name=\"%s\" file=\"%s\" material=\"%s\" condition=\"%s\"/>\n", it->Name.c_str(), it->File.c_str(), it->Material.c_str(), it->Condition.c_str());
		}
		fprintf(fout, "\t</EntityList>\n");
	}

	//output locators
	{
		DObject_NT::_LocatorList::iterator it;
		fprintf(fout, "\t<Locators>\n");
		for(it = pMesh->m_LocatorList.begin(); it != pMesh->m_LocatorList.end(); it ++)
		{
			DObject_NT::_SlotList::iterator sit;
			Ogre::Vector3 qv;
			Ogre::Radian qw;
			it->OffRot.ToAngleAxis(qw, qv);
			fprintf(fout, "\t\t<Locator name=\"%s\" bonename=\"%s\"  x=\"%f\" y=\"%f\" z=\"%f\" qx=\"%f\" qy=\"%f\" qz=\"%f\" qw=\"%f\">\n", 
				it->Name.c_str(), it->BoneName.c_str(), it->OffPos.x, it->OffPos.y, it->OffPos.z, qv.x, qv.y, qv.z, qw.valueRadians());
			for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
			{
				DString attribs;
				if(sit->InitAttribs.size() >= 1)
					attribs = sit->InitAttribs[0];
				for(i = 1; i < (long)sit->InitAttribs.size(); i ++)
					attribs += ";" + sit->InitAttribs[i];

				fprintf(fout, "\t\t\t<Slot name=\"%s\" object=\"%s\" attribs=\"%s\"/>\n", 
					sit->Name.c_str(), sit->ObjFile.c_str(), attribs.c_str());
			}
			fprintf(fout, "\t\t</Locator>\n");
		}
		fprintf(fout, "\t</Locators>\n");
	}	

	fprintf(fout, "</Object>\n");

	fclose(fout);
	return true;
}

bool	SkeletonMeshUtil::LoadChild(DObject_NT* pMesh, 
							   const DString strObjGroup, 
							   const DString& strXmlFile, 
							   const StringList& attribNames,
							   const StringList& attribValues,
							   Ogre::SkeletonInstance*	pParentSkeleton,
							   Ogre::Bone*				pParentBone
							   )
							   
{
	if(!pMesh)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, 
			"Error Loading SkeletonMesh FileName ="+strXmlFile+" Group ="+strObjGroup,
			"SkeletonMeshUtil.Load");
	}

	const char* pTemp = NULL;
	TiXML::TiXmlDocument xmldoc;
	TiXML::TiXmlElement* pObject;
	TiXML::TiXmlElement* pAttribs;
	TiXML::TiXmlElement* pEntityList;
	TiXML::TiXmlElement* pLocators;

	StringList tempAttribNames, tempAttribValues;

	long attribidx;

	pMesh->SetGroupName(strObjGroup);

	pMesh->SetObjFileName(strXmlFile);

	pMesh->SetIsChildObject(true);



	if(strObjGroup.size() == 0)
	{
		if(!xmldoc.LoadFile(strXmlFile.c_str())) return false;
	}
	else
	{
		Ogre::DataStreamPtr dataptr = Ogre::ResourceGroupManager::getSingleton().openResource(strXmlFile, strObjGroup);
		xmldoc.Parse(dataptr->getAsString().c_str());
		if(xmldoc.Error())
			return false;
	}

	pObject = xmldoc.RootElement();
	if(!pObject) return false;

	pTemp = pObject->Attribute("name");
	
	if(pTemp) pMesh->SetObjFileName(pTemp);
	

	pMesh->SetObjectNode(pMesh->GetSceneMgr()->createSceneNode());

	pMesh->ReSet();
	
	assert(pParentSkeleton);
	pMesh->m_pObjParentSkeleton	=	pParentSkeleton;

	//load object attributes
	pAttribs = pObject->FirstChildElement("Attributes");
	if(pAttribs)
	{
		TiXML::TiXmlElement* pAttrib;
		pAttrib = pAttribs->FirstChildElement("Attribute");
		while(pAttrib)
		{
			DString			name, value, hints;
			//load attribute name
			pTemp = pAttrib->Attribute("name");
			if(!pTemp)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> One Attribute haven't name, skipped!", 
					pMesh->GetObjFileName().c_str(), 
					pMesh->GetObjName().c_str());

				pAttrib = pAttrib->NextSiblingElement("Attribute");
				continue;
			}
			name = pTemp;
			//load attribute value
			pTemp = pAttrib->Attribute("value");
			if(!pTemp) value = "";
			else value = pTemp;
			//load attribute hints
			pTemp = pAttrib->Attribute("hints");
			if(!pTemp) hints = "";
			else hints = pTemp;
			//add attribute to attribute map
			if(!pMesh->_AddAttrib(name, value, hints))
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load attribute \'%s\', skipped!", 
					pMesh->GetObjFileName().c_str(),
					pMesh->GetObjName().c_str(),
					name.c_str());
			}

			tempAttribNames.push_back(name);
			tempAttribValues.push_back(value);

			pAttrib = pAttrib->NextSiblingElement("Attribute");
		}
	}

	//update object attributes
	for(attribidx = 0; attribidx < (long)tempAttribNames.size(); attribidx ++)
	{
		pMesh->_SetAttribValue(tempAttribNames[attribidx], tempAttribValues[attribidx]);
	}

	//load object mesh list
	pEntityList = pObject->FirstChildElement("EntityList");
	if(pEntityList)
	{
		TiXML::TiXmlElement* pEntity;
		pEntity = pEntityList->FirstChildElement("Entity");
		while(pEntity)
		{
			DString name, entfile, mat, condition;
			pTemp = pEntity->Attribute("name");
			if(pTemp) name = pTemp; else name = "";
			pTemp = pEntity->Attribute("file");
			if(pTemp) entfile = pTemp; else entfile = "";
			pTemp = pEntity->Attribute("material");
			if(pTemp) mat = pTemp; else mat = "";
			pTemp = pEntity->Attribute("condition");
			if(pTemp) condition = pTemp; else condition = "";
			pMesh->_AddEntity(name, entfile, mat, condition);
			pEntity = pEntity->NextSiblingElement("Entity");
		}
		_LoadChildEntity(pMesh);
	}

	//load locator info list
	pLocators = pObject->FirstChildElement("Locators");
	if(pLocators)
	{
		TiXML::TiXmlElement* pLocator;
		pLocator = pLocators->FirstChildElement("Locator");
		while(pLocator)
		{
			DString LocatorName, BoneName;
			Ogre::Vector3 LocatorOffPos;
            Ogre::Quaternion LocatorOffRot(Ogre::Quaternion::IDENTITY);
			float qx, qy, qz, qw;

			pTemp = pLocator->Attribute("name");
			if(pTemp) LocatorName = pTemp;
			else
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> The locator have not name, skipped!", 
					pMesh->GetObjFileName().c_str(),
					pMesh->GetObjName().c_str());
				pLocator = pLocator->NextSiblingElement("Locator");
				continue;
			}

			pTemp = pLocator->Attribute("bonename");
			if(pTemp) BoneName = pTemp;
			else BoneName = "";

			LocatorOffPos.x = (float)atof(pLocator->Attribute("x"));
			LocatorOffPos.y = (float)atof(pLocator->Attribute("y"));
			LocatorOffPos.z = (float)atof(pLocator->Attribute("z"));

			qx = (float)atof(pLocator->Attribute("qx"));
			qy = (float)atof(pLocator->Attribute("qy"));
			qz = (float)atof(pLocator->Attribute("qz"));
			qw = (float)atof(pLocator->Attribute("qw"));
			LocatorOffRot.FromAngleAxis(Ogre::Radian(qw), Ogre::Vector3(qx, qy, qz));

			
			BoneName	=	pParentBone->getName();

			if(!pMesh->_AddChildLocator(LocatorName, BoneName, LocatorOffPos, LocatorOffRot,pParentSkeleton,pParentBone))
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add locator \'%s\', skipped", 
					pMesh->GetObjFileName().c_str(),
					pMesh->GetObjName().c_str(), 
					LocatorName.c_str());
				pLocator = pLocator->NextSiblingElement("Locator");
				continue;
			}

			TiXML::TiXmlElement* pSlot;
			pSlot = pLocator->FirstChildElement("Slot");
			while(pSlot)
			{
				DString SlotName, SlotObj;
				DStringList InitAttribs;

				pTemp = pSlot->Attribute("name");
				if(pTemp) SlotName = pTemp;
				else
				{
					MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add slot because there is no name, skipped!",
						pMesh->GetObjFileName().c_str(),
						pMesh->GetObjName().c_str());
					pSlot = pSlot->NextSiblingElement("Slot");
					continue;
				}

				pTemp = pSlot->Attribute("object");
				if(pTemp) SlotObj = pTemp;
				else SlotObj = "";

				pTemp = pSlot->Attribute("attribs");
				if(pTemp)
				{
					char* pBuf = new char[strlen(pTemp) + 1];
					char* pTok;
					strcpy(pBuf, pTemp);
					pTok = strtok(pBuf, ";");
					while(pTok)
					{
						if(pTok[0] == '$')
							InitAttribs.push_back(pTok);
						pTok = strtok(NULL, ";");
					}
					delete [] pBuf;
				}

				pMesh->_AddSlot(LocatorName, SlotName, SlotObj, InitAttribs);

				pSlot = pSlot->NextSiblingElement("Slot");
			}
			pLocator = pLocator->NextSiblingElement("Locator");
		}
		//pMesh->_UpdateLocatorPos();
	}	

	pMesh->UpdateObjAttribute(AH_ALL);


	pMesh->getBoundingBox();
	return true;
}
