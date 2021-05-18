// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WXMaterialEditorDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include "WXMaterialEditorDialog.h"
#include "WXMaterialPreviewDialog.h"
#include "SceneManipulator.h"

#include "EffectSettingGlobalData.h"

#include "Core/WXSceneInfo.h"
#include "Core/WXParticleSystemObject.h"
#include "Core/WXEffectObject.h"
#include "Core/WXProjector.h"

#include "EffectSystem/WXEffect.h"
#include "EffectSystem/WXEffectElement.h"
#include "EffectSystem/WXParticleElement.h"
#include "EffectSystem/WXBeamElement.h"
#include "EffectSystem/WXSkill.h"
#include "EffectSystem/WXAnimationRibbon.h"
#include "EffectSystem/WXAnimationEffectInfo.h"
#include "EffectSystem/WXProjectorElement.h"
#include "EffectSystem/WXBillboardSetElement.h"

#include "OgreExt/OgreMeshParticleRenderer.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/MaterialSetting_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreMaterialManager.h>
#include <OgreMaterialSerializer.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreTextureUnitState.h>
#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>
#include <OgreStringVector.h>
#include <OgreParticleSystem.h>
#include <OgreBlendMode.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// ParticleEditorDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(MaterialEditorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MaterialEditorDialog, wxDialog)
	EVT_INIT_DIALOG(MaterialEditorDialog::OnInitDialog)
	EVT_BUTTON(ID_BUTTON_SELECT, MaterialEditorDialog::OnSelectButtonDown)
	EVT_COMBOBOX(ID_COMBO_DRAW_STYLE, MaterialEditorDialog::OnComboDrawStyle)
	EVT_COMBOBOX(ID_COMBO_STAGE_STYLE, MaterialEditorDialog::OnComboStageStyle)
	EVT_CHECKBOX(ID_CHECKBOX_TWO_SIDE, MaterialEditorDialog::OnCheckBoxTwoSide)
	EVT_BUTTON(ID_BUTTON_OPEN, MaterialEditorDialog::OnOpenButtonDown)
	EVT_TEXT_ENTER(ID_TEXTCTRL_DURATION, MaterialEditorDialog::OnTextCtrlDuration)
	EVT_COMBOBOX(ID_COMBO_MATERIAL, MaterialEditorDialog::OnComboMaterial)
	EVT_BUTTON(ID_BUTTON_SAVE, MaterialEditorDialog::OnSaveButtonDown)
	EVT_COMBOBOX(ID_COMBO_TEX_ADD_MODE, MaterialEditorDialog::OnComboTexAddMode)
	EVT_TEXT_ENTER(ID_TEXTCTRL_SCALE, MaterialEditorDialog::OnTextCtrlScale)
	EVT_TEXT_ENTER(ID_TEXTCTRL_ROTATE_ANIM, MaterialEditorDialog::OnTextCtrlRotateAnim)
//	EVT_BUTTON(ID_BUTTON_EMISSIVE, MaterialEditorDialog::OnButtonEmissive)

END_EVENT_TABLE()


void
MaterialEditorDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = MaterialSetting(this, true, true);

	mDrawStyleComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_DRAW_STYLE),wxComboBox);
	mStageStyleComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_STAGE_STYLE),wxComboBox);
	mMaterialComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_MATERIAL),wxComboBox);
	mTexNameTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TEXTURE_NAME),wxTextCtrl);
	mTwoSideCheckBox = wxDynamicCast(this->FindWindow(ID_CHECKBOX_TWO_SIDE),wxCheckBox);
	mDurationTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_DURATION),wxTextCtrl);
	mTexAddModeComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_TEX_ADD_MODE),wxComboBox);
	mTexScaleTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_SCALE), wxTextCtrl);
	mTexRotateAnimTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ROTATE_ANIM), wxTextCtrl);
}

MaterialEditorDialog::~MaterialEditorDialog()
{
	if (mMaterialPreviewDialog)
	{
		mMaterialPreviewDialog->Destroy();
		delete mMaterialPreviewDialog;
		mMaterialPreviewDialog = NULL;
	}
}

void MaterialEditorDialog::OnSelectButtonDown(wxCommandEvent &e)
{
	if ( NULL == mMaterialPreviewDialog )
	{
		mMaterialPreviewDialog = new MaterialPreviewDialog(this, wxID_ANY, _("Fairy Material Preview"));
	}
	if ( false == mMaterialPreviewDialog->IsShown() )
		mMaterialPreviewDialog->Show();			
	mMaterialPreviewDialog->InitPreviewDialog(mSceneManipulator);
}

void MaterialEditorDialog::InitMaterialInfo( Ogre::String &matName )
{
	mSelectedMatName = matName;

	mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(mSelectedMatName);

	mMaterialComboBox->SetValue(mSelectedMatName);


	Ogre::SceneBlendFactor sourceFactor;
	Ogre::SceneBlendFactor destFactor;

	Ogre::LayerBlendOperationEx layerBlendOpEx;

	Ogre::TextureUnitState::UVWAddressingMode addressMode;

	if (mCurrentMaterial.isNull())
	{
		wxMessageBox("Current Material is NULL : " + matName);
	}
	else
	{
		// mesh粒子和projector都不需要关闭光照
		if ( (gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh") ||
			(gEffectSettingGlobalData.mCurrentEditEffectType == CEFT_EFFECT_PROJECTOR) ) 
		{
			mCurrentMaterial->setLightingEnabled(true);
			mCurrentMaterial->setDiffuse(0.0f, 0.0f, 0.0f, 1.0f);
			mCurrentMaterial->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
			mCurrentMaterial->setAmbient(0.0f, 0.0f, 0.0f);
		}
		else
		{
			mCurrentMaterial->setLightingEnabled(false);
		}

		mCurrentMaterial->setDepthWriteEnabled(false);
		mCurrentMaterial->setFog(true);

		size_t techNum = mCurrentMaterial->getNumTechniques();

		if (techNum > 0)
		{
			size_t passNum = mCurrentMaterial->getTechnique(0)->getNumPasses();

			if (passNum > 0)
			{
				mCurrentFirstPass = mCurrentMaterial->getTechnique(0)->getPass(0);
				assert (mCurrentFirstPass);

				

				sourceFactor = mCurrentFirstPass->getSourceBlendFactor();
				destFactor = mCurrentFirstPass->getDestBlendFactor();

				if ( sourceFactor == Ogre::SBF_ONE && destFactor == Ogre::SBF_ZERO )
				{
					mDrawStyleComboBox->SetValue("Regular");
				}
				else if ( sourceFactor == Ogre::SBF_ONE && destFactor == Ogre::SBF_ONE )
				{
					mDrawStyleComboBox->SetValue("Translucent");
				}
				else if ( sourceFactor == Ogre::SBF_ZERO && destFactor == Ogre::SBF_ONE_MINUS_SOURCE_COLOUR )
				{
					mDrawStyleComboBox->SetValue("Darken");
				}
				else if ( sourceFactor == Ogre::SBF_SOURCE_ALPHA && destFactor == Ogre::SBF_ONE_MINUS_SOURCE_ALPHA )
				{
					mDrawStyleComboBox->SetValue("AlphaBlend");
				}
				else if ( sourceFactor == Ogre::SBF_SOURCE_COLOUR && destFactor == Ogre::SBF_ONE_MINUS_SOURCE_COLOUR )
				{
					mDrawStyleComboBox->SetValue("ColourBlend");
				}
				else if ( sourceFactor == Ogre::SBF_DEST_COLOUR && destFactor == Ogre::SBF_ZERO )
				{
					mDrawStyleComboBox->SetValue("Modulate");
				}

				if (mCurrentFirstPass->getCullingMode() == Ogre::CULL_NONE)
				{
					mTwoSideCheckBox->SetValue(true);
				}
				else
				{
					mTwoSideCheckBox->SetValue(false);
				}

				size_t texNum = mCurrentFirstPass->getNumTextureUnitStates();

				if (texNum == 0)
				{
					mCurrentFirstTexUnit = mCurrentFirstPass->createTextureUnitState();
				}
				else
					mCurrentFirstTexUnit = mCurrentFirstPass->getTextureUnitState(0);

				assert (mCurrentFirstTexUnit);
	
				Ogre::String texName = mCurrentFirstTexUnit->getTextureName();

				mTexNameTextCtrl->SetValue(texName.c_str());

				mDurationTextCtrl->SetValue( Ogre::StringConverter::toString(mCurrentFirstTexUnit->getAnimationDuration()) );

				layerBlendOpEx = mCurrentFirstTexUnit->getColourBlendMode().operation;

				if (layerBlendOpEx == Ogre::LBX_MODULATE)
				{
					mStageStyleComboBox->SetValue("Modulated");
				}
				else if (layerBlendOpEx == Ogre::LBX_MODULATE_X2)
				{
					mStageStyleComboBox->SetValue("TransModulate2X");
				}
				else if (layerBlendOpEx == Ogre::LBX_MODULATE_X4)
				{
					mStageStyleComboBox->SetValue("TransModulate4X");
				}

				addressMode = mCurrentFirstTexUnit->getTextureAddressingMode();

				if (addressMode.u == Ogre::TextureUnitState::TAM_CLAMP)
				{
					mTexAddModeComboBox->SetValue("Clamp");
				}
				else if (addressMode.u == Ogre::TextureUnitState::TAM_WRAP)
				{
					mTexAddModeComboBox->SetValue("Wrap");
				}
				else if (addressMode.u == Ogre::TextureUnitState::TAM_MIRROR)
				{
					mTexAddModeComboBox->SetValue("Mirror");
				}		

				float uScale = mCurrentFirstTexUnit->getTextureUScale();
				float vScale = mCurrentFirstTexUnit->getTextureVScale();

				Ogre::String scaleStr = Ogre::StringConverter::toString(uScale) + " " + Ogre::StringConverter::toString(vScale);

				mTexScaleTextCtrl->SetValue(scaleStr);				
			}
		}	
	}
}

void MaterialEditorDialog::OnComboDrawStyle(wxCommandEvent &e)
{
	Ogre::String drawStyle = e.GetString().c_str();

	if (mCurrentFirstPass)
	{
		if ( drawStyle == "Regular" )
		{
			mCurrentFirstPass->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ZERO);
		}
		else if ( drawStyle == "Translucent" )
		{
			mCurrentFirstPass->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE);
		}
		else if ( drawStyle == "Darken" )
		{
			mCurrentFirstPass->setSceneBlending(Ogre::SBF_ZERO, Ogre::SBF_ONE_MINUS_SOURCE_COLOUR);
		}
		else if ( drawStyle == "AlphaBlend" )
		{
			mCurrentFirstPass->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
		}
		else if ( drawStyle == "ColourBlend" )
		{
			mCurrentFirstPass->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_COLOUR);

		}
		else if ( drawStyle == "Modulate" )
		{
			mCurrentFirstPass->setSceneBlending(Ogre::SBF_DEST_COLOUR, Ogre::SBF_ZERO);
		}

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}
	}	
}

void MaterialEditorDialog::OnCheckBoxTwoSide(wxCommandEvent &e)
{
	if (mCurrentFirstPass)
	{
		if (e.IsChecked())
		{
			mCurrentFirstPass->setCullingMode(Ogre::CULL_NONE);
		}
		else
		{
			mCurrentFirstPass->setCullingMode(Ogre::CULL_CLOCKWISE);
		}

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::InitMaterialCombo(void)
{
	typedef std::list<Ogre::String> MaterialFileNameList;
	MaterialFileNameList materialFileNameList;

	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		"*.material");
	for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
	{
		if ( it->archive->getName() == EFFECT_PATH)
		{
			materialFileNameList.push_back(it->filename);
		}
	}

	Ogre::ResourceManager::ResourceMapIterator resourceMapIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();

	while ( resourceMapIterator.hasMoreElements() )
	{				
		Ogre::String matName = resourceMapIterator.peekNextValue()->getName();

		for ( MaterialFileNameList::iterator i = materialFileNameList.begin();
			i != materialFileNameList.end(); ++i )
		{
			if ( *i == resourceMapIterator.peekNextValue()->getOrigin() )
			{
				mMaterialComboBox->Append(matName);

				break;

			}
		}

		resourceMapIterator.moveNext();
	}

}

void MaterialEditorDialog::InitDialog( WX::SceneManipulator *manipulator )
{
	assert (manipulator);
	mSceneManipulator = manipulator;

	//InitMaterialCombo();

	// 根据当前所编辑的东西自动地选择或创建材质

	// 如果当前选择了粒子系统，就编辑该粒子系统的材质
	CurrentEditEffectType currentEditItem = gEffectSettingGlobalData.mCurrentEditEffectType;

	switch (currentEditItem)
	{
	case CEFT_PARTICLE:
		{
			Ogre::ParticleSystem *currentSystem = gEffectSettingGlobalData.mCurrentParticleSystem;
			Ogre::String currentTemplateName = gEffectSettingGlobalData.mCurrentParticleTemplateName;

			assert (currentSystem);

			if (currentSystem->getMaterialName() != currentSystem->getName())
			{
				mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(currentSystem->getName());

				if (mCurrentMaterial.isNull())
				{
					mCurrentMaterial = Ogre::MaterialManager::getSingleton().create(currentSystem->getName(),"General");
				}

				WX::Scene::ObjectsByTypeRange particles =
					mSceneManipulator->getSceneInfo()->findObjectsByType(WX::ParticleSystemObject::msType);

				for (WX::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
				{
					const WX::ObjectPtr& object = *it;
					WX::ParticleSystemObject* particleObject = static_cast<WX::ParticleSystemObject *>(object.get());

					Ogre::ParticleSystem *system = particleObject->getParticleSystem();
					Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

					// 现在做的是直接对模板的修改，所以要修改全部的同个模板的特效
					if ( system && templateName == currentTemplateName )
					{
						system->setMaterialName(mCurrentMaterial->getName());
					}
				}

				// 已经创建的effect中的粒子也要相应修改
				WX::Scene::ObjectsByTypeRange effects =
					mSceneManipulator->getSceneInfo()->findObjectsByType(WX::EffectObject::msType);

				for (WX::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
				{
					const WX::ObjectPtr& object = *it;
					const WX::EffectObject* effectObject = static_cast<WX::EffectObject *>(object.get());

					WX::Effect *effect = effectObject->getEffect();

					assert (effect);

					for ( size_t i=0; i<effect->getNumElements(); ++i )
					{
						WX::EffectElement *element = effect->getElement(i);

						if ( element->getType() == "Particle" )
						{
							Ogre::String name = static_cast<WX::ParticleElement *>(element)->getParticleSystemName();
							if (name == currentTemplateName)
							{
								Ogre::ParticleSystem *system = static_cast<WX::ParticleElement *>(element)->getParticleSystem();

								system->setMaterialName(mCurrentMaterial->getName());
							}
						}
					}		
				}

				currentSystem->setMaterialName(mCurrentMaterial->getName());	
			}
			else
			{
				mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(currentSystem->getName());

				assert ( false == mCurrentMaterial.isNull() );
			}
			InitMaterialInfo( Ogre::String(mCurrentMaterial->getName()) );

			break;
		}

	case CEFT_RIBBON:
		{
			unsigned short currentRibbonIndex = gEffectSettingGlobalData.mCurrentRibbonIndex;

			WX::AnimationRibbon *ribbon = gEffectSettingGlobalData.mCurrentSkill->getAnimationRibbon(currentRibbonIndex);

			assert (ribbon);

			Ogre::String currRibbonMatName = ribbon->getMaterialName();

			// ribbon该有的名称
			Ogre::String ribbonMatName = gEffectSettingGlobalData.mCurrentSkill->getSkillName() +
				"ribbon" + Ogre::StringConverter::toString(currentRibbonIndex);

			if ( currRibbonMatName != ribbonMatName )
			{
				mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(ribbonMatName);

				if (mCurrentMaterial.isNull())
				{
					mCurrentMaterial = Ogre::MaterialManager::getSingleton().create(ribbonMatName,"General");
				}

				ribbon->setMaterial(ribbonMatName);
			}
			else
			{
				mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(ribbonMatName);

				assert ( false == mCurrentMaterial.isNull() );
			}

			InitMaterialInfo( Ogre::String(mCurrentMaterial->getName()) );

			break;
		}

	case CEFT_EFFECT_BEAM:
		{
			WX::BeamElement *beam = static_cast<WX::BeamElement *>
				( gEffectSettingGlobalData.mCurrentEffect->getElement(gEffectSettingGlobalData.mCurrentEffectElementIndex) );

			assert (beam);

			Ogre::String currBeamMatName = beam->getMaterialName();

			// ribbon该有的名称
			Ogre::String beamMatName = gEffectSettingGlobalData.mCurrentEffect->getTemplateName() +
				"beam" + Ogre::StringConverter::toString(gEffectSettingGlobalData.mCurrentEffectElementIndex);

			if ( currBeamMatName != beamMatName )
			{
				mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(beamMatName);

				if (mCurrentMaterial.isNull())
				{
					mCurrentMaterial = Ogre::MaterialManager::getSingleton().create(beamMatName,"General");
				}

				beam->setMaterialName(beamMatName);
			}
			else
			{
				mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(beamMatName);

				assert ( false == mCurrentMaterial.isNull() );
			}

			InitMaterialInfo( Ogre::String(mCurrentMaterial->getName()) );

			break;
		}

	case CEFT_EFFECT_PROJECTOR:
		{
			WX::ProjectorElement *projectorElement = static_cast<WX::ProjectorElement *>
				( gEffectSettingGlobalData.mCurrentEffect->getElement(gEffectSettingGlobalData.mCurrentEffectElementIndex) );

			assert (projectorElement);

			gEffectSettingGlobalData.mOriginProjectorMaterial = projectorElement->getMaterialName();

			mCurrentMaterial = projectorElement->getProjector()->_getPatchMaterial();

			if ( false == mCurrentMaterial.isNull() )
				InitMaterialInfo( Ogre::String(mCurrentMaterial->getName()) );
			else
			{
				wxMessageBox(_("Please select the material in effect editor first!"));
			}
		}

    case CEFT_EFFECT_BILLBOARDSET:
        {
            WX::BillboardSetElement *billboardSet = static_cast<WX::BillboardSetElement *>
                ( gEffectSettingGlobalData.mCurrentEffect->getElement(gEffectSettingGlobalData.mCurrentEffectElementIndex) );

            assert (billboardSet);

            Ogre::String currBillboardMatName = billboardSet->getBillboardMaterial();

            // ribbon该有的名称
            Ogre::String billboardMatName = gEffectSettingGlobalData.mCurrentEffect->getTemplateName() +
                "billboardset" + Ogre::StringConverter::toString(gEffectSettingGlobalData.mCurrentEffectElementIndex);

            if ( currBillboardMatName != billboardMatName )
            {
                mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(billboardMatName);

                if (mCurrentMaterial.isNull())
                {
                    mCurrentMaterial = Ogre::MaterialManager::getSingleton().create(billboardMatName,"General");
                }

                billboardSet->setBillboardMaterial(billboardMatName);
            }
            else
            {
                mCurrentMaterial = Ogre::MaterialManager::getSingleton().getByName(billboardMatName);

                assert ( false == mCurrentMaterial.isNull() );
            }

            InitMaterialInfo( Ogre::String(mCurrentMaterial->getName()) );

            break;
        }

	}
}

void MaterialEditorDialog::OnComboStageStyle(wxCommandEvent &e)
{
	Ogre::String stageStyle = e.GetString().c_str();

	if (mCurrentFirstTexUnit)
	{
		if ( stageStyle == "Modulated" )
		{
			mCurrentFirstTexUnit->setColourOperationEx(Ogre::LBX_MODULATE);
		}
		else if ( stageStyle == "TransModulate2X" )
		{
			mCurrentFirstTexUnit->setColourOperationEx(Ogre::LBX_MODULATE_X2);
		}
		else if ( stageStyle == "TransModulate4X" )
		{
			mCurrentFirstTexUnit->setColourOperationEx(Ogre::LBX_MODULATE_X4);
		}	

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::OnOpenButtonDown(wxCommandEvent &e)
{
	wxFileDialog dialog(this, _T("Choose a texture file"), EFFECT_PATH, wxT(""),
		"All texture files (*.dds;*.bmp;*.gif;*.png;*.jpg;*.tga)|*.dds;*.bmp;*.gif;*.png;*.jpg;*.tga", wxMULTIPLE); 

	if (dialog.ShowModal() == wxID_OK)
	{		
		mSelectedFileNames.clear();

		dialog.GetFilenames(mSelectedFileNames);

		if ( mSelectedFileNames.size() > 1 )
		{
			if (mCurrentFirstTexUnit)
			{
				Ogre::String firstTexName = mSelectedFileNames[0].c_str();

				Ogre::StringVector splitedName = Ogre::StringUtil::split( firstTexName, "_" );

				Ogre::StringVector splitedName1 = Ogre::StringUtil::split( splitedName[1], "." );

				mCurrentTexName = splitedName[0] + "." + splitedName1[1];

				mTexNameTextCtrl->SetValue(mCurrentTexName);

				float duration = Ogre::StringConverter::parseReal(mDurationTextCtrl->GetValue().c_str());

				mCurrentFirstTexUnit->setAnimatedTextureName(mCurrentTexName, mSelectedFileNames.size(), duration);
			}
		}
		else
		{
			if (mCurrentFirstTexUnit)
			{
				mCurrentTexName = mSelectedFileNames[0].c_str();
				mCurrentFirstTexUnit->setTextureName(mCurrentTexName);
				mTexNameTextCtrl->SetValue(mCurrentTexName);
			}
		}

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::OnTextCtrlDuration(wxCommandEvent &e)
{
	if ( mSelectedFileNames.size() > 1 )
	{
		if (mCurrentFirstTexUnit)
		{
			float duration = Ogre::StringConverter::parseReal(mDurationTextCtrl->GetValue().c_str());

			mCurrentFirstTexUnit->setAnimatedTextureName(mCurrentTexName, mSelectedFileNames.size(), duration);
		}

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::OnNewButtonDown(wxCommandEvent &e)
{
}

void MaterialEditorDialog::OnComboMaterial(wxCommandEvent &e)
{
	Ogre::String matName = e.GetString().c_str();

	InitMaterialInfo(matName);
}

void MaterialEditorDialog::OnSaveButtonDown(wxCommandEvent &e)
{
	if (mCurrentMaterial.isNull()==false)
	{
		Ogre::String matName;
		switch (gEffectSettingGlobalData.mCurrentEditEffectType)
		{
		case CEFT_EFFECT_PROJECTOR:
			{
				matName = gEffectSettingGlobalData.mOriginProjectorMaterial;

				Ogre::MaterialPtr newMat = Ogre::MaterialManager::getSingleton().getByName(matName);

				mCurrentMaterial->copyDetailsTo(newMat);
				newMat->setSelfIllumination(0.0f, 0.0f, 0.0f);

				Ogre::String pathFileName = EFFECT_PATH + matName + ".material";

				Ogre::MaterialSerializer ser;
				ser.queueForExport(newMat);
				ser.exportQueued(pathFileName);

				break;
			}

		default:
			{
				matName = mCurrentMaterial->getName();

				Ogre::String pathFileName = EFFECT_PATH + matName + ".material";

				Ogre::MaterialSerializer ser;
				ser.queueForExport(mCurrentMaterial);
				ser.exportQueued(pathFileName);

				break;
			}
		}		

		wxMessageBox("Success Save!");

		Hide();
	}
}

void MaterialEditorDialog::OnComboTexAddMode(wxCommandEvent &e)
{
	Ogre::String addMode = e.GetString().c_str();

	if (mCurrentFirstTexUnit)
	{
		if ( addMode == "Clamp" )
		{
			mCurrentFirstTexUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		}
		else if ( addMode == "Wrap" )
		{
			mCurrentFirstTexUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}
		else if ( addMode == "Mirror" )
		{
			mCurrentFirstTexUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_MIRROR);
		}	

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::OnTextCtrlScale(wxCommandEvent &e)
{
	if (mCurrentFirstTexUnit)
	{
		Ogre::StringVector scales = Ogre::StringUtil::split(mTexScaleTextCtrl->GetValue().c_str());

		mCurrentFirstTexUnit->setTextureUScale( Ogre::StringConverter::parseReal(scales[0]) );
		mCurrentFirstTexUnit->setTextureVScale( Ogre::StringConverter::parseReal(scales[1]) );

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::OnTextCtrlRotateAnim(wxCommandEvent &e)
{
	if (mCurrentFirstTexUnit)
	{
		mCurrentFirstTexUnit->setRotateAnimation( Ogre::StringConverter::parseReal(mTexRotateAnimTextCtrl->GetValue().c_str()) );

		if ( gEffectSettingGlobalData.mCurrentParticleSystem &&
			gEffectSettingGlobalData.mCurrentParticleSystem->getRendererName() == "mesh" )
		{
			setMeshMaterial();
		}

	}
}

void MaterialEditorDialog::setMeshMaterial(void)
{
	WX::Scene::ObjectsByTypeRange particles =
		mSceneManipulator->getSceneInfo()->findObjectsByType(WX::ParticleSystemObject::msType);

	for (WX::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
	{
		const WX::ObjectPtr& object = *it;
		WX::ParticleSystemObject* particleObject = static_cast<WX::ParticleSystemObject *>(object.get());

		Ogre::ParticleSystem *system = particleObject->getParticleSystem();
		Ogre::String templateName = particleObject->getPropertyAsString("particle system name");

		// 现在做的是直接对模板的修改，所以要修改全部的同个模板的特效
		if ( system && templateName == gEffectSettingGlobalData.mCurrentParticleTemplateName )
		{
			Ogre::MeshParticleRenderer *renderer = static_cast<Ogre::MeshParticleRenderer *>
				(system->getRenderer());

			assert (renderer->getType() == "mesh");

			renderer->setMaterialName(mCurrentMaterial->getName());
		}
	}

	// 已经创建的effect中的粒子也要相应修改
	WX::Scene::ObjectsByTypeRange effects =
		mSceneManipulator->getSceneInfo()->findObjectsByType(WX::EffectObject::msType);

	for (WX::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
	{
		const WX::ObjectPtr& object = *it;
		const WX::EffectObject* effectObject = static_cast<WX::EffectObject *>(object.get());

		WX::Effect *effect = effectObject->getEffect();

		assert (effect);

		for ( size_t i=0; i<effect->getNumElements(); ++i )
		{
			WX::EffectElement *element = effect->getElement(i);

			if ( element->getType() == "Particle" )
			{
				Ogre::String name = static_cast<WX::ParticleElement *>(element)->getParticleSystemName();
				if (name == gEffectSettingGlobalData.mCurrentParticleTemplateName)
				{
					Ogre::ParticleSystem *system = static_cast<WX::ParticleElement *>(element)->getParticleSystem();

					Ogre::MeshParticleRenderer *renderer = static_cast<Ogre::MeshParticleRenderer *>
						(system->getRenderer());

					assert (renderer->getType() == "mesh");

					renderer->setMaterialName(mCurrentMaterial->getName());
				}
			}
		}		
	}

	if (gEffectSettingGlobalData.mCurrentSkill)
	{
		for ( unsigned short i = 0; i < gEffectSettingGlobalData.mCurrentSkill->getNumAnimationEffectInfos(); ++i )
		{
			WX::AnimationEffectInfo *effectInfo = gEffectSettingGlobalData.mCurrentSkill->getAnimationEffectInfo(i);
			assert (effectInfo);

			WX::Effect *effect = effectInfo->getEffect();

			assert (effect);

			for ( size_t i=0; i<effect->getNumElements(); ++i )
			{
				WX::EffectElement *element = effect->getElement(i);

				if ( element->getType() == "Particle" )
				{
					Ogre::String name = static_cast<WX::ParticleElement *>(element)->getParticleSystemName();
					if (name == gEffectSettingGlobalData.mCurrentParticleTemplateName)
					{
						Ogre::ParticleSystem *system = static_cast<WX::ParticleElement *>(element)->getParticleSystem();

						Ogre::MeshParticleRenderer *renderer = static_cast<Ogre::MeshParticleRenderer *>
							(system->getRenderer());

						assert (renderer->getType() == "mesh");

						renderer->setMaterialName(mCurrentMaterial->getName());
					}
				}
			}		
		}
	}

	Ogre::MeshParticleRenderer *renderer = static_cast<Ogre::MeshParticleRenderer *>
		(gEffectSettingGlobalData.mCurrentParticleSystem->getRenderer());

	assert (renderer->getType() == "mesh");

	renderer->setMaterialName(mCurrentMaterial->getName());
}