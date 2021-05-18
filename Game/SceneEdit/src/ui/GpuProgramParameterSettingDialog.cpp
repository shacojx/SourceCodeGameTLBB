// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "GpuProgramParameterSettingDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "GpuProgramParameterSettingDialog.h"
#include "res/GpuProgramParameterEditor_wdr.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include "WXColourProperty.h"
#include "PropertyManager.h"

#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreGpuProgram.h>
#include <OgreStringConverter.h>

IMPLEMENT_DYNAMIC_CLASS(GpuProgramParameterSettingDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(GpuProgramParameterSettingDialog, wxDialog)

	EVT_INIT_DIALOG(GpuProgramParameterSettingDialog::OnInitDialog)
	EVT_PG_CHANGED(ID_PARAMETER_EDITOR, GpuProgramParameterSettingDialog::OnPropertyChanged)
	//EVT_COMBOBOX(ID_COMBO_MATERIAL, GpuProgramParameterSettingDialog::OnComboMaterial)
	EVT_TREE_ITEM_ACTIVATED(ID_TREECTRL_MATERIAL, GpuProgramParameterSettingDialog::OnMaterialSelected)
	EVT_BUTTON(ID_BUTTON_APPLY_OTHER_MATERIAL, GpuProgramParameterSettingDialog::OnApplyOtherMaterialButtonDown)
	EVT_BUTTON(ID_BUTTON_SAVE, GpuProgramParameterSettingDialog::OnSaveMaterial)
	EVT_CLOSE(GpuProgramParameterSettingDialog::OnClose)

END_EVENT_TABLE()

void
GpuProgramParameterSettingDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	if (!mInit)
	{
		wxSizer* sizer = GpuProgramParameterEditor(this, true, true);

		mMaterialTreeCtrl = wxDynamicCast(this->FindWindow(ID_TREECTRL_MATERIAL), wxTreeCtrl);

		mPropertyGridManager = wxDynamicCast(this->FindWindow(ID_PARAMETER_EDITOR), wxPropertyGridManager);

		mMaterialFilesMap.clear();

		InitMaterialFilesMap();

		InitMaterialTreeCtrl();

		mChangedMaterialNames.Clear();

		mInit = true;
	}	
}

void 
GpuProgramParameterSettingDialog::InitMaterialTreeCtrl(void)
{
	CatalogMap catalogMap;

	if (!Ogre::ResourceGroupManager::getSingletonPtr())
		return;

	mMaterialTreeCtrl->Freeze();

	mMaterialTreeCtrl->DeleteAllItems();

	wxTreeItemId rootId = mMaterialTreeCtrl->AddRoot(_("Materials"));

	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"*.material");
	for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
	{
		Ogre::String pathStr = it->archive->getName();
		
		Ogre::String pathName(pathStr);

		Ogre::String fileName = it->filename;

		size_t pos = pathStr.find_first_of('/');

		if (pos != Ogre::String::npos)
		{
			pathStr.erase(0,pos);
		}

		pos = pathStr.find_last_of('/');

		// 在文件夹中
		if (pos != Ogre::String::npos)
		{	
			wxTreeItemId itemId = GetParentId(pathStr, catalogMap);
			MaterialFilesMap::iterator it = mMaterialFilesMap.find(fileName);

			if ( it != mMaterialFilesMap.end() )
			{
				const MaterialNames &nameList = it->second.nameList;

				it->second.pathName = pathName;

				for (MaterialNames::const_iterator i = nameList.begin();
					i != nameList.end(); ++i)
				{
					mMaterialTreeCtrl->AppendItem( itemId, wxT(*i) );
				}
			}

		}
		else
		{
			// 如果是在根目录下，就直接用画刷名称来作为路径名
			MaterialFilesMap::iterator it = mMaterialFilesMap.find(fileName);

			if ( it != mMaterialFilesMap.end() )
			{
				const MaterialNames &nameList = it->second.nameList;

				it->second.pathName = pathName;

				for (MaterialNames::const_iterator i = nameList.begin();
					i != nameList.end(); ++i)
				{
					mMaterialTreeCtrl->AppendItem( mMaterialTreeCtrl->GetRootItem(), wxT(*i) );
				}
			}
		}	
	}

	mMaterialTreeCtrl->Thaw();
}

void GpuProgramParameterSettingDialog::OnMaterialSelected(wxTreeEvent& e)
{
	wxTreeItemId itemId = e.GetItem();

	if (itemId.IsOk())
	{
		Ogre::String matName = mMaterialTreeCtrl->GetItemText(itemId).c_str();

		mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(matName);

		if ( false == mCurrentMaterial.isNull() )
		{
			mPropertyGridManager->Freeze();
			mPropertyGridManager->GetGrid()->Clear();

			wxPGId materialId = mPropertyGridManager->Append( wxCustomProperty(mCurrentMaterial->getName().c_str()) );

			if (materialId.IsOk())
			{
				mPropertyGridManager->DisableProperty(materialId);

				unsigned short numTechniques = mCurrentMaterial->getNumTechniques();

				for (unsigned short it=0; it<numTechniques; ++it)
				{
					Ogre::Technique *tech = mCurrentMaterial->getTechnique(it);

					wxPGId techId =	mPropertyGridManager->AppendIn( materialId, wxCustomProperty(tech->getName().c_str()) );

					if ( techId.IsOk() )
					{
						mPropertyGridManager->DisableProperty(techId);

						mPropertyGridManager->SetPropertyColour(techId, *wxRED);

						unsigned short numPasses = tech->getNumPasses();

						for (unsigned short ip=0; ip<numPasses; ++ip)
						{
							Ogre::Pass *pass = tech->getPass(ip);

					//		if ( pass->isProgrammable() )
					//		{
								wxPGId passId = mPropertyGridManager->AppendIn( techId, wxCustomProperty(pass->getName().c_str()) );

								if (passId.IsOk())
								{
									wxPGId programId;

									mPropertyGridManager->DisableProperty(passId);

									mPropertyGridManager->SetPropertyColour(passId, *wxGREEN);

									if ( pass->hasVertexProgram() )
									{
										// 先获取verterx program的参数
										Ogre::GpuProgramParametersSharedPtr VPParameters = pass->getVertexProgramParameters();

										if ( false == VPParameters.isNull() )
										{
											programId = mPropertyGridManager->AppendIn( passId, wxCustomProperty("Vertex Program") );

											size_t numContants = VPParameters->getNumConstantDefinitions();

											for ( size_t ic=0; ic<numContants; ++ic )
											{
												const Ogre::GpuProgramParameters::ConstantDefinition *definition = 
													VPParameters->getConstantDefinition(ic);

												if (definition && (false == definition->isAuto))
												{
													Ogre::String valueStr;

													Ogre::String paraName = definition->name;

													Ogre::GpuProgramParameters::RealConstantEntry* realEntry = 
														VPParameters->getNamedRealConstantEntry(paraName);

													if (realEntry)
													{
														for (int indexVal=0; indexVal<4; ++indexVal)
														{
															valueStr += Ogre::StringConverter::toString(realEntry->val[indexVal]);
															valueStr += " ";
														}
													}
													else
													{
														Ogre::GpuProgramParameters::IntConstantEntry* intEntry =
															VPParameters->getNamedIntConstantEntry(paraName);

														if (intEntry)
														{
															for (int indexVal=0; indexVal<4; ++indexVal)
															{
																valueStr += Ogre::StringConverter::toString(intEntry->val[indexVal]);
																valueStr += " ";
															}
														}
													}

													wxPGId itemId = mPropertyGridManager->AppendIn( programId, CreateProperty( paraName.c_str(),Ogre::String(valueStr.c_str()) ) );

													mPropertyGridManager->SetPropertyColourToDefault(itemId);
												}
											} // for ( size_t ic=0; ic<numContants; ++ic )
										}

									}

									if ( pass->hasFragmentProgram() )
									{
										Ogre::GpuProgramParametersSharedPtr FPParameters = pass->getFragmentProgramParameters();

										if ( false == FPParameters.isNull() )
										{
											programId = mPropertyGridManager->AppendIn( passId, wxCustomProperty("Fragment Program") );

											size_t numContants = FPParameters->getNumConstantDefinitions();

											for ( size_t ic=0; ic<numContants; ++ic )
											{
												const Ogre::GpuProgramParameters::ConstantDefinition *definition = 
													FPParameters->getConstantDefinition(ic);

												if (definition && (false == definition->isAuto))
												{
													Ogre::String valueStr;

													Ogre::String paraName = definition->name;

													Ogre::GpuProgramParameters::RealConstantEntry* realEntry = 
														FPParameters->getNamedRealConstantEntry(paraName);

													if (realEntry)
													{
														for (int indexVal=0; indexVal<4; ++indexVal)
														{
															valueStr += Ogre::StringConverter::toString(realEntry->val[indexVal]);
															valueStr += " ";
														}
													}
													else
													{
														Ogre::GpuProgramParameters::IntConstantEntry* intEntry =
															FPParameters->getNamedIntConstantEntry(paraName);

														if (intEntry)
														{
															for (int indexVal=0; indexVal<4; ++indexVal)
															{
																valueStr += Ogre::StringConverter::toString(intEntry->val[indexVal]);
																valueStr += " ";
															}
														}
													}

													wxPGId itemId = mPropertyGridManager->AppendIn( programId, CreateProperty( paraName.c_str(),Ogre::String(valueStr.c_str()) ) );

													mPropertyGridManager->SetPropertyColourToDefault(itemId);
												}
											} // for ( size_t ic=0; ic<numContants; ++ic )
										}

									}
									
									
								} // if (passId.IsOk())

							//} // if ( pass->isProgrammable() )
						} // for (unsigned short ip=0; ip<numPasses; ++ip)
					} // if ( id.IsOk() )			
				} // for (unsigned short it=0; it<numTechniques; ++it)
			}			

			mPropertyGridManager->Thaw();
			mPropertyGridManager->ExpandAll();
			mPropertyGridManager->Refresh();
		}
	}	
}

wxTreeItemId  
GpuProgramParameterSettingDialog::GetParentId( Ogre::String &catalog, CatalogMap &map )
{
	// 如果送入的相对路径为空，说明这个纹理放在资源目录的根目录下
	if ( catalog == "" )
		// 所以，它的节点应该被建在根目录下
		return mMaterialTreeCtrl->GetRootItem();
	else
	{
		// 否则，先试图找它的上一级目录，如果有的话（说明这个目录在树中已经建好，可能是因为有同个目录下的纹理
		// 被处理过了），就返回这个目录
		CatalogMap::iterator i = map.find(catalog);

		if ( i != map.end() )
			return i->second;
		// 如果没有，就得递归往上找，并建树
		else
		{
			Ogre::String path, baseName;
			Ogre::StringUtil::splitFilename(catalog, baseName, path);
			if (!path.empty() && path[path.length()-1] == '/')
				path.erase(path.length() - 1);

			wxTreeItemId id = mMaterialTreeCtrl->AppendItem( GetParentId(path, map), wxT(baseName) );

			map.insert( CatalogMap::value_type( catalog, id ) );

			return id;
		}
	}
}

void 
GpuProgramParameterSettingDialog::dealWithSpecialParameterOnChanged( const Ogre::String &name, Ogre::String &value )
{
	if ( name == "diffuseMat" || name == "ambientMat" || name == "specularMat" )
	{
		Ogre::StringVector valueVector = Ogre::StringUtil::split(value);

		if (valueVector.size() == 1)
		{
			Ogre::String originValue = valueVector[0];

			value.clear();

			value += originValue;
			value += " ";
			value += originValue;
			value += " ";
			value += originValue;
			value += " ";
			value += "1.0";
		}
		else if (valueVector.size() == 2)
		{
			Ogre::String originValue = valueVector[0];

			value.clear();

			value += originValue;
			value += " ";
			value += originValue;
			value += " ";
			value += originValue;
			value += " ";
			value += valueVector[1];
		}
	}
}

void 
GpuProgramParameterSettingDialog::dealWithSpecialParameterOnCreated( const Ogre::String &name, Ogre::String &value )
{
	if ( name == "diffuseMat" || name == "ambientMat" || name == "specularMat" )
	{
		Ogre::StringVector valueVector = Ogre::StringUtil::split(value);

		if (valueVector.size() == 4)
		{
			value.clear();

			if ( ( valueVector[0] == valueVector[1] ) && ( valueVector[1] == valueVector[2] ) )
			{
				value += valueVector[0];
				
				float alphaValue = Ogre::StringConverter::parseReal(valueVector[3]);

				if (alphaValue != 1.0f)
				{
					value += " ";
					value += valueVector[3];
				}
			}
		}
	}
}

void
GpuProgramParameterSettingDialog::OnPropertyChanged(wxPropertyGridEvent& e)
{
	// Skip parent properties
	if (e.GetPropertyPtr()->GetParentingType() != 0)
		return;

	wxPGId id = e.GetPropertyPtr()->GetId();
	wxPGId programId = mPropertyGridManager->GetPropertyParent(id);

	wxPGId passId = mPropertyGridManager->GetPropertyParent(programId);
	wxPGId techId = mPropertyGridManager->GetPropertyParent(passId);

	const wxString &programLabel = mPropertyGridManager->GetPropertyLabel(programId);

	unsigned short passIndex = mPropertyGridManager->GetPropertyIndex(passId);

	unsigned short techIndex = mPropertyGridManager->GetPropertyIndex(techId);

	Ogre::String propertyName = e.GetPropertyName().c_str();
	Ogre::String propertyValue = e.GetPropertyValueAsString().c_str();

	// 有某些参数进行特殊处理
	dealWithSpecialParameterOnChanged( propertyName, propertyValue );

	assert (false == mCurrentMaterial.isNull());

	Ogre::Technique *currentTechnique = mCurrentMaterial->getTechnique(techIndex);

	if (currentTechnique)
	{
		Ogre::Pass *currentPass = currentTechnique->getPass(passIndex);

		if (currentPass)
		{
			Ogre::StringVector valueArray = Ogre::StringUtil::split(propertyValue);

			// 判断是int型参数还是float型参数
			if (programLabel == "Vertex Program")
			{
				Ogre::GpuProgramParametersSharedPtr VPParameters = currentPass->getVertexProgramParameters();

				assert ( false == VPParameters.isNull() );

				Ogre::GpuProgramParameters::RealConstantEntry* realEntry = 
					VPParameters->getNamedRealConstantEntry(propertyName);

				if (realEntry)
				{
					float tempValue[4] = {0.0f};

					for (size_t i=0; i<valueArray.size(); ++i)
					{
						tempValue[i] = Ogre::StringConverter::parseReal(valueArray[i]);
					}

					VPParameters->setNamedConstant(propertyName, tempValue, 1);
				}
				else
				{
					Ogre::GpuProgramParameters::IntConstantEntry* intEntry = 
						VPParameters->getNamedIntConstantEntry(propertyName);

					if (intEntry)
					{
						int tempValue[4] = {0};

						for (size_t i=0; i<valueArray.size(); ++i)
						{
							tempValue[i] = Ogre::StringConverter::parseInt(valueArray[i]);
						}

						VPParameters->setNamedConstant(propertyName, tempValue, 1);
					}
				}
			}
			else if (programLabel == "Fragment Program")
			{
				Ogre::GpuProgramParametersSharedPtr FPParameters = currentPass->getFragmentProgramParameters();

				assert ( false == FPParameters.isNull() );

				Ogre::GpuProgramParameters::RealConstantEntry* realEntry = 
					FPParameters->getNamedRealConstantEntry(propertyName);

				if (realEntry)
				{
					float tempValue[4] = {0.0f};

					for (size_t i=0; i<valueArray.size(); ++i)
					{
						tempValue[i] = Ogre::StringConverter::parseReal(valueArray[i]);
					}

					FPParameters->setNamedConstant(propertyName, tempValue, 1);
				}
				else
				{
					Ogre::GpuProgramParameters::IntConstantEntry* intEntry = 
						FPParameters->getNamedIntConstantEntry(propertyName);

					if (intEntry)
					{
						int tempValue[4] = {0};

						for (size_t i=0; i<valueArray.size(); ++i)
						{
							tempValue[i] = Ogre::StringConverter::parseInt(valueArray[i]);
						}

						FPParameters->setNamedConstant(propertyName, tempValue, 1);
					}
				}
			}
		}
	}	
}

wxPGProperty*
GpuProgramParameterSettingDialog::CreateProperty(const Ogre::String &paraName, Ogre::String &paraValue)
{
	wxPGProperty* property = NULL;

	dealWithSpecialParameterOnCreated(paraName, paraValue);

	if (paraName.find("Color") != Ogre::String::npos)
	{
		property = wxFairyColourProperty(paraName, paraName, colorConverter(paraValue));
	}
	else
	{
		property = wxStringProperty(paraName, paraName, paraValue.c_str());
	}

	assert (property);

	return  property;
}

void GpuProgramParameterSettingDialog::OnApplyOtherMaterialButtonDown(wxCommandEvent &e)
{
	wxArrayTreeItemIds ids;

	size_t numIds = mMaterialTreeCtrl->GetSelections(ids);

	for (size_t inumIds = 0; inumIds < numIds; ++inumIds)
	{
		Ogre::String matName = mMaterialTreeCtrl->GetItemText( ids[inumIds] ).c_str();

		if (matName == mCurrentMaterial->getName())
			continue;

		Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().getByName(matName);

		if ( false == matPtr.isNull() )
		{
			for (unsigned short techIndex=0; techIndex<matPtr->getNumTechniques(); ++techIndex)
			{
				Ogre::Technique *currentTech = mCurrentMaterial->getTechnique(techIndex);
				Ogre::Technique *destTech = matPtr->getTechnique(techIndex);

				if ( currentTech && destTech )
				{
					for (unsigned short passIndex=0; passIndex<destTech->getNumPasses(); ++passIndex)
					{
						Ogre::Pass *currentPass = currentTech->getPass(passIndex);
						Ogre::Pass *destPass = destTech->getPass(passIndex);

						if (currentPass && destPass)
						{
							if ( currentPass->hasVertexProgram() && destPass->hasVertexProgram() &&
								currentPass->getVertexProgramName() == destPass->getVertexProgramName() )
							{
								destPass->setVertexProgram("");
								destPass->setVertexProgram(currentPass->getVertexProgramName());
								destPass->setVertexProgramParameters
									( Ogre::GpuProgramParametersSharedPtr(new Ogre::GpuProgramParameters( *(currentPass->getVertexProgramParameters()) )) );
							}

							if ( currentPass->hasFragmentProgram() && destPass->hasFragmentProgram() &&
								currentPass->getFragmentProgramName() == destPass->getFragmentProgramName() )
							{
								destPass->setFragmentProgram("");
								destPass->setFragmentProgram(currentPass->getFragmentProgramName());
								destPass->setFragmentProgramParameters
									( Ogre::GpuProgramParametersSharedPtr(new Ogre::GpuProgramParameters( *(currentPass->getFragmentProgramParameters()) )) );
							}

						}
					}
				}
			}

			bool exist = false;
			for (size_t count = 0; count < mChangedMaterialNames.Count(); ++count)
			{
				if (mChangedMaterialNames[count] == matName)
					exist = true;
			}

			if ( false == exist )
				mChangedMaterialNames.push_back(matName);
		} // if ( false == matPtr.isNull() )
	}
}

void GpuProgramParameterSettingDialog::InitMaterialFilesMap(void)
{
	Ogre::ResourceManager::ResourceMapIterator resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

	while ( resourceMapIterator.hasMoreElements() )
	{				
		Ogre::String matName = resourceMapIterator.peekNextValue()->getName();
		Ogre::String fileName = resourceMapIterator.peekNextValue()->getOrigin();

		MaterialFilesMap::iterator i = mMaterialFilesMap.find(fileName);

		if (i != mMaterialFilesMap.end())
		{
			i->second.nameList.push_back(matName);
		}
		else
		{
			MaterialFileInfo fileInfo;

			fileInfo.nameList.push_back(matName);

			std::pair<MaterialFilesMap::iterator, bool> inserted = 
				mMaterialFilesMap.insert(MaterialFilesMap::value_type(fileName, fileInfo));

			assert (inserted.second);
		}

		resourceMapIterator.moveNext();
	}
}

void GpuProgramParameterSettingDialog::OnClose(wxCloseEvent& event)
{
	if ( event.CanVeto() )
	{
		this->Show(false);

		event.Veto();
	}
}

void GpuProgramParameterSettingDialog::OnSaveMaterial(wxCommandEvent &e)
{
	wxString message;

	message += _("Changed materials\n");

	for (size_t count=0; count<mChangedMaterialNames.Count(); ++count)
	{
		message += mChangedMaterialNames[count];
		message += "\n";
	}
	message += mCurrentMaterial->getName();
	message += "\n";

	message += _("Hit YES to save all changed materials,Hit NO to save the current material.");

	int answer = wxMessageBox(message, _("Save materials"), wxYES_NO | wxCANCEL);

	if (answer == wxYES)
	{
		for (size_t count = 0; count < mChangedMaterialNames.Count(); ++count)
		{
			Ogre::String matName = mChangedMaterialNames[count].c_str();

			Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().getByName(matName);

			if (false == matPtr.isNull())
			{
				MaterialFilesMap::iterator it = mMaterialFilesMap.find(matPtr->getOrigin());

				if (it != mMaterialFilesMap.end())
				{
					Ogre::String path = it->second.pathName;
					if (path[ path.length()-1 ] != '/')
						path += '/';

					Ogre::String pathFileName = path + matPtr->getName() + ".material";

					Ogre::MaterialSerializer ser;
					ser.queueForExport(matPtr);
					ser.exportQueued(pathFileName);
				}
			}
		}

		mChangedMaterialNames.Clear();

		if (false == mCurrentMaterial.isNull())
		{
			MaterialFilesMap::iterator it = mMaterialFilesMap.find(mCurrentMaterial->getOrigin());

			if (it != mMaterialFilesMap.end())
			{
				Ogre::String path = it->second.pathName;
				if (path[ path.length()-1 ] != '/')
					path += '/';

				Ogre::String pathFileName = path + mCurrentMaterial->getName() + ".material";

				Ogre::MaterialSerializer ser;
				ser.queueForExport(mCurrentMaterial);
				ser.exportQueued(pathFileName);
			}		
		}

		wxMessageBox("All changed materials Saved!");
	}
	else if (answer == wxNO)
	{
		if (false == mCurrentMaterial.isNull())
		{
			MaterialFilesMap::iterator it = mMaterialFilesMap.find(mCurrentMaterial->getOrigin());

			if (it != mMaterialFilesMap.end())
			{
				Ogre::String path = it->second.pathName;
				if (path[ path.length()-1 ] != '/')
					path += '/';

				Ogre::String pathFileName = path + mCurrentMaterial->getName() + ".material";

				Ogre::MaterialSerializer ser;
				ser.queueForExport(mCurrentMaterial);
				ser.exportQueued(pathFileName);

				wxMessageBox("Current material Saved!");
			}		
		}
	}	
}
