#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "WXEffectEditDialog.h"
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

#include "WXSkillEditDialog.h"
#include "WXMaterialEditorDialog.h"

#include "SceneManipulator.h"
#include "EffectSystem/WXEffectManager.h"
//#include "EffectSystem/WXEffect.h"
//#include "EffectSystem/WXEffectElement.h"
#include "EffectSystem/WXSkill.h"
#include "EffectSystem/WXAnimationEffectInfo.h"
#include "EffectSystem/WXAnimationSceneLightInfo.h"

#include "Core/WXSceneInfo.h"
#include "Core/WXEffectObject.h"

#include <OgreMeshManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>
#include <OgreAnimation.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

#include "WXColourProperty.h"
#include "PropertyManager.h"
#include "ActorAnimationSettingDialog.h"
#include "WXEditorApp.h"

#include "EffectSettingGlobalData.h"

//extern EffectSettingGlobalData gEffectSettingGlobalData;

IMPLEMENT_DYNAMIC_CLASS(FairySkillEditDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(FairySkillEditDialog, wxDialog)
	EVT_INIT_DIALOG(FairySkillEditDialog::OnInitDialog)
	EVT_PG_CHANGED(feID_PROPERTIES_VIEWER, FairySkillEditDialog::OnPropertyChanged)
	EVT_BUTTON(feID_BUTTON_ADD_EFFECTINFO, FairySkillEditDialog::OnAddEffectInfoButtonDown)
	EVT_BUTTON(feID_BUTTON_ADD_RIBBON, FairySkillEditDialog::OnAddRibbonButtonDown)
	EVT_BUTTON(feID_BUTTON_ADD_SOUND, FairySkillEditDialog::OnAddSoundButtonDown)
    EVT_BUTTON(feID_BUTTON_ADD_LIGHT, FairySkillEditDialog::OnAddLightButtonDown)

	EVT_BUTTON(feID_BUTTON_DEL, FairySkillEditDialog::OnDelButtonDown)
	EVT_BUTTON(feID_BUTTON_SAVE_AS_SKILL, FairySkillEditDialog::OnSaveAsButtonDown)
	EVT_BUTTON(feID_BUTTON_MATERIAL_SETTING, FairySkillEditDialog::OnMaterialSettingButtonDown)
END_EVENT_TABLE()

wxPGConstants * FairySkillEditDialog::PopulateConstants(const Ogre::String& name)
{
	size_t id = reinterpret_cast<size_t>(&name);
	wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

	if (constants)
	{
		for (std::list<wxPGConstants*>::iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
		{
			if (constants == *it)
			{
				if (constants->UnRef())
					delete constants;

				mConstantList.erase(it);

				break;
			}
		}
	}

//	if (!constants)
//	{
		constants = wxPropertyGrid::CreateConstantsArray(id);

		if ( name == "Animation" )
		{
			// 第一个为空，表示设置这个元素的材质是用原mesh的材质
			for ( unsigned short i = 0; i < mDObject->getSkeletonAnimationCount(); ++i )
			{
				Ogre::Animation *anim = mDObject->getSkeletonAnimation(i);

				if ( anim )
				{
					constants->Add(anim->getName().c_str());
				}
			}			
		}
		else if ( name == "EffectTemplateName" )
		{
			WX::EffectManager::EffectTemplateIterator it = 
				WX::EffectManager::getSingleton().getEffectTemplateIterator();

			while ( it.hasMoreElements() )
			{
				constants->Add(it.peekNextKey().c_str());

				it.moveNext();
			}
		}
		else if ( name == "AttachPoint" )
		{
			for ( unsigned short i=0; i<mDObject->getNumBones(); ++i )
			{
				constants->Add(mDObject->getBoneName(i).c_str());
			}	

			Ogre::StringVector locatorNames;
			mDObject->getLocatorNames(locatorNames);

			for ( size_t i=0; i<locatorNames.size(); ++i )
			{
				constants->Add(locatorNames[i].c_str());
			}			
		}
		else if ( name == "Material" )
		{
			typedef std::list<Ogre::String> MaterialFileNameList;
			MaterialFileNameList materialFileNameList;

			Ogre::FileInfoListPtr fileInfoList =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"*.material");
			for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
			{
				if ( it->archive->getName() == MATERIAL_PATH)
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
						constants->Add(matName.c_str());
						break;
					}
				}

				resourceMapIterator.moveNext();
			}
		}	
		else if ( name == "SoundName" )
		{
            constants->Add("NULL");
			//-----------------------------------
			//得到所有文件名
            if (wxGetApp().funcEnumSoundFile)
            {
			    int nIndex = 0;
			    do
			    {
				    char szTemp[100];

                    int id;

				    bool bHave = wxGetApp().funcEnumSoundFile(nIndex++, szTemp, 100, id);
				    if(!bHave) break;

				    constants->Add(szTemp);
			    }while(true);
            }
		}

		registerConstants(constants);
	//}

	return constants;
}

FairySkillEditDialog::~FairySkillEditDialog()
{
	// 删除材质对话框
	if (mMaterialEditorDialog)
	{
		mMaterialEditorDialog->Destroy();
		delete mMaterialEditorDialog;
		mMaterialEditorDialog = NULL;
	}

	for (std::list<wxPGConstants*>::const_iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
	{
		wxPGConstants* constants = *it;
		if (constants->UnRef())
			delete constants;
	}
	mConstantList.clear();
}

void FairySkillEditDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	if ( false == mInit )
	{
		wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 0, 0);
		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(0);

		mPropertiesViewer =
			new wxPropertyGridManager(this, feID_PROPERTIES_VIEWER,
			wxDefaultPosition, wxSize(300,300),
			wxCLIP_CHILDREN | wxCLIP_SIBLINGS |
			wxPG_BOLD_MODIFIED | // Modified values are drawn in bold font
			wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
			wxPG_TOOLTIPS | // Display tooltips for cell text that cannot be shown completely
			wxPG_TOOLBAR | // Include toolbar
			wxPG_DESCRIPTION | // Include description box
			wxFULL_REPAINT_ON_RESIZE |
			wxPGMAN_DEFAULT_STYLE); // Plus default style
		mPropertiesViewer->SetExtraStyle(wxPG_EX_CLASSIC_SPACING);
		sizer->Add(mPropertiesViewer, 0, wxFIXED_MINSIZE|wxGROW|wxALL, 2);	

		wxButton *materialButton = new wxButton( this, feID_BUTTON_MATERIAL_SETTING, wxT("Material Setting"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( materialButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *addEffectInfoButton = new wxButton( this, feID_BUTTON_ADD_EFFECTINFO, wxT("Add Effect Info"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( addEffectInfoButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *addRibbonButton = new wxButton( this, feID_BUTTON_ADD_RIBBON, wxT("Add Ribbon"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( addRibbonButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *addSoundButton = new wxButton( this, feID_BUTTON_ADD_SOUND, wxT("Add Sound"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( addSoundButton, 0, wxALIGN_CENTER|wxALL, 5 );

        wxButton *addLightButton = new wxButton( this, feID_BUTTON_ADD_LIGHT, wxT("Add Light"), wxDefaultPosition, wxDefaultSize, 0 );
        sizer->Add( addLightButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *delEffectInfoButton = new wxButton( this, feID_BUTTON_DEL, wxT("Del"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( delEffectInfoButton, 0, wxALIGN_CENTER|wxALL, 5 );

		wxButton *saveAsButton = new wxButton( this, feID_BUTTON_SAVE_AS_SKILL, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
		sizer->Add( saveAsButton, 0, wxALIGN_CENTER|wxALL, 5 );

		this->SetSizer(sizer);
		sizer->SetSizeHints(this);

		mInit = true;
	}	
}

wxPGProperty*
FairySkillEditDialog::CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value)
{
	wxPGProperty* property = NULL;
	wxString name = propertyDef.name.c_str();
	Ogre::ParameterType paraType = propertyDef.paramType;

	switch ( paraType )
	{
	case Ogre::PT_COLOURVALUE:
		property = wxFairyColourProperty(name, name, colorConverter(value));
		break;

	case Ogre::PT_STRING:
		{
			if (name == "HitTime")
			{
				Ogre::String newValue("");

				for (unsigned short i=0; i<mSkill->getNumHitTimes(); ++i)
				{
					float time = mSkill->getHitTime(i);

					newValue += Ogre::StringConverter::toString( (time * mDObject->getCurrentAnimationLength() * mFramePerSecond) );
					newValue += " ";
				}
				property = wxStringProperty(name, name, newValue.c_str());
			}
			else if (name == "BreakTime")
			{
				Ogre::String newValue("");

				for (unsigned short i=0; i<mSkill->getNumBreakTimes(); ++i)
				{
					float time = mSkill->getBreakTime(i);

					newValue += Ogre::StringConverter::toString( (time * mDObject->getCurrentAnimationLength() * mFramePerSecond) );
					newValue += " ";
				}
				property = wxStringProperty(name, name, newValue.c_str());
			}
            else if (name == "ShakeTime")
            {
                Ogre::String newValue("");

                for (unsigned short i=0; i<mSkill->getNumShakeTimes(); ++i)
                {
                    float time = mSkill->getShakeTime(i);

                    newValue += Ogre::StringConverter::toString( (time * mDObject->getCurrentAnimationLength() * mFramePerSecond) );
                    newValue += " ";
                }
                property = wxStringProperty(name, name, newValue.c_str());
            }
			else
			{
				wxPGConstants* constants = PopulateConstants(propertyDef.name);
				if (constants)
				{
					// 创建出combobox
					property = wxEnumProperty(name, name, *constants);
					// 设置成当前这个参数的值
					property->SetValueFromString(value.c_str());
				}
				else
				{				
					property = wxStringProperty(name, name, value.c_str());
				}
			}			

			break;
		}

	case Ogre::PT_REAL:
		{
			if (name == "AttachTime")
			{
				float inputValue = Ogre::StringConverter::parseReal(value);

				if (inputValue >= 0.0f && inputValue <= 1.0f)
					value = Ogre::StringConverter::toString( (inputValue * mDObject->getCurrentAnimationLength() * mFramePerSecond) );
			}

			property = wxFloatProperty(name, name, Ogre::StringConverter::parseReal(value));

			break;
		}

	case Ogre::PT_BOOL:
		{
			size_t id = reinterpret_cast<size_t>(&name);
			wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

			if (!constants)
			{
				constants = wxPropertyGrid::CreateConstantsArray(id);		

				constants->Add("true");
				constants->Add("false");

				registerConstants(constants);
			}

			if (constants)
			{
				// 创建出combobox
				property = wxEnumProperty(name, name, *constants);
				// 设置成当前这个参数的值
				property->SetValueFromString(value.c_str());
			}
			break;
		}

	default:
		property = wxStringProperty(name, name, value.c_str());
		break;
	}

	assert (property);

	return property;
}

void
FairySkillEditDialog::registerConstants(wxPGConstants* constants)
{
	assert(constants);
	constants->Ref();
	mConstantList.push_back(constants);
}

void
FairySkillEditDialog::OnPropertyChanged(wxPropertyGridEvent& e)
{
	unsigned short numOfEffectInfos = mSkill->getNumAnimationEffectInfos();
	unsigned short numOfRibbons = mSkill->getNumAnimationRibbons();
	unsigned short numOfSounds = mSkill->getNumAnimationSounds();
    unsigned short unmOfLights = mSkill->getNumAnimationSceneLightInfos();

	// Skip parent properties
	if (e.GetPropertyPtr()->GetParentingType() != 0)
		return;

	wxPGId id = e.GetPropertyPtr()->GetId();
	wxPGId parentId = mPropertiesViewer->GetPropertyParent(id);

	WX::String propertyName = e.GetPropertyName().c_str();
	WX::String propertyValue = e.GetPropertyValueAsString().c_str();

	if (propertyName == "AttachTime")
	{
		float inputValue = Ogre::StringConverter::parseReal(propertyValue);

		float outputTime = inputValue / ( mDObject->getCurrentAnimationLength() * mFramePerSecond );

		if ( outputTime >= 1.0f )
		{
			wxMessageBox(_("Wrong parameter!"));
			return;
		}
		propertyValue = Ogre::StringConverter::toString(outputTime);
	}
	else if (propertyName == "HitTime" || propertyName == "BreakTime" || propertyName == "ShakeTime")
	{
		Ogre::StringVector valueArray = Ogre::StringUtil::split(propertyValue);

		propertyValue.clear();

		for (size_t i=0; i<valueArray.size(); ++i)
		{
			float inputValue = Ogre::StringConverter::parseReal(valueArray[i]);

			float outputTime = inputValue / ( mDObject->getCurrentAnimationLength() * mFramePerSecond );

			if ( outputTime >= 1.0f )
			{
				wxMessageBox(_("Wrong parameter!"));
				return;
			}

			propertyValue += Ogre::StringConverter::toString(outputTime);
			propertyValue += " ";
		}
	}
	
	unsigned short index = mPropertiesViewer->GetPropertyIndex(parentId);

	if ( index == 0 )
	{
		mSkill->setParameter(propertyName, propertyValue);
	}
	else if ( index > 0 && index <= numOfEffectInfos )
	{
		mSkill->getAnimationEffectInfo(index-1)->setParameter(propertyName, propertyValue);
	}
	else if ( index > numOfEffectInfos && index <= (numOfEffectInfos + numOfRibbons) )
	{
		mSkill->getAnimationRibbon(index-numOfEffectInfos-1)->setParameter(propertyName, propertyValue);
	}
	else if ( index > numOfEffectInfos + numOfRibbons && index <= (numOfEffectInfos + numOfRibbons + numOfSounds) )
	{
		mSkill->getAnimationSound(index-numOfEffectInfos-numOfRibbons-1)->setParameter(propertyName, propertyValue);
	}
    else if ( index > numOfEffectInfos + numOfRibbons + numOfSounds
        && index <= (numOfEffectInfos + numOfRibbons + numOfSounds + unmOfLights) )
    {
        mSkill->getAnimationSceneLightInfo(index-numOfEffectInfos-numOfRibbons-numOfSounds-1)->setParameter(propertyName, propertyValue);
    }

	// 更新模板
	WX::Skill *skill = WX::EffectManager::getSingleton().getSkill(mSkill->getSkillName());

	*skill = *mSkill;
}

void FairySkillEditDialog::InitSkillDialog( WX::LogicModel *object, WX::SceneManipulator *sceneManipulator )
{
	mParentDialog = static_cast<ActorAnimationSettingDialog *>(this->GetParent());
	assert ( object && sceneManipulator );

	mDObject = object;
	mSkill = mDObject->getCurrentSkill();

	mSceneManipulator = sceneManipulator;

	if ( mSkill )
		InitPropertyEditor();

	this->SetTitle(mDObject->getCurrentSkill()->getSkillName());
}

void FairySkillEditDialog::InitPropertyEditor(void)
{
	// 更新模板
	WX::Skill *skill = WX::EffectManager::getSingleton().getSkill(mSkill->getSkillName());

	*skill = *mSkill;

	mPropertiesViewer->Freeze();
	mPropertiesViewer->GetGrid()->Clear();

	wxPGId id =	mPropertiesViewer->Append( wxCustomProperty("Basic Setting") );

	wxPGId itemId;

	if ( id.IsOk() )
	{
		mPropertiesViewer->DisableProperty(id);

		mPropertiesViewer->SetPropertyColour(id, *wxBLUE);

		const Ogre::ParameterList &paraList = mSkill->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			Ogre::String value = mSkill->getParameter(paraList[paraCount].name);

			itemId = mPropertiesViewer->AppendIn( id, CreateProperty(paraList[paraCount], value) );
			mPropertiesViewer->SetPropertyColourToDefault(itemId);
		}
	}

	// 设置effect属性
	for ( unsigned short i=0; i < mSkill->getNumAnimationEffectInfos(); ++i )
	{
		WX::AnimationEffectInfo *effectInfo = mSkill->getAnimationEffectInfo(i);

		assert (effectInfo);

		Ogre::String effectIndexStr = "Effect Info " + Ogre::StringConverter::toString(i);

		wxPGId id =	mPropertiesViewer->Append( wxCustomProperty(effectIndexStr.c_str()) );

		wxPGId itemId;

		if ( id.IsOk() )
		{
			mPropertiesViewer->DisableProperty(id);
			
			mPropertiesViewer->SetPropertyColour(id, *wxRED);

			const Ogre::ParameterList &paraList = effectInfo->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String value = effectInfo->getParameter(paraList[paraCount].name);

				itemId = mPropertiesViewer->AppendIn( id, CreateProperty(paraList[paraCount], value) );
				mPropertiesViewer->SetPropertyColourToDefault(itemId);
			}
		}		
	}

	for ( unsigned short i=0; i < mSkill->getNumAnimationRibbons(); ++i )
	{
		WX::AnimationRibbon *ribbon = mSkill->getAnimationRibbon(i);

		assert (ribbon);

		Ogre::String ribbonIndexStr = "Ribbon " + Ogre::StringConverter::toString(i);

		wxPGId id =	mPropertiesViewer->Append( wxCustomProperty(ribbonIndexStr.c_str()) );

		wxPGId itemId;

		if ( id.IsOk() )
		{
			mPropertiesViewer->DisableProperty(id);

			mPropertiesViewer->SetPropertyColour(id, *wxGREEN);

			const Ogre::ParameterList &paraList = ribbon->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String value = ribbon->getParameter(paraList[paraCount].name);

				itemId = mPropertiesViewer->AppendIn( id, CreateProperty(paraList[paraCount], value) );
				mPropertiesViewer->SetPropertyColourToDefault(itemId);
			}
		}		
	}

	for ( unsigned short i=0; i < mSkill->getNumAnimationSounds(); ++i )
	{
		WX::AnimationSound *sound = mSkill->getAnimationSound(i);

		assert (sound);

		Ogre::String soundIndexStr = "Sound " + Ogre::StringConverter::toString(i);

		wxPGId id =	mPropertiesViewer->Append( wxCustomProperty(soundIndexStr.c_str()) );

		wxPGId itemId;

		if ( id.IsOk() )
		{
			mPropertiesViewer->DisableProperty(id);

			mPropertiesViewer->SetPropertyColour(id, *wxCYAN);

			const Ogre::ParameterList &paraList = sound->getParameters();

			for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
			{
				Ogre::String value = sound->getParameter(paraList[paraCount].name);

				itemId = mPropertiesViewer->AppendIn( id, CreateProperty(paraList[paraCount], value) );
				mPropertiesViewer->SetPropertyColourToDefault(itemId);
			}
		}		
	}

    for ( unsigned short i=0; i < mSkill->getNumAnimationSceneLightInfos(); ++i )
    {
        WX::AnimationSceneLightInfo *light = mSkill->getAnimationSceneLightInfo(i);

        assert (light);

        Ogre::String lightIndexStr = "SceneLight " + Ogre::StringConverter::toString(i);

        wxPGId id =	mPropertiesViewer->Append( wxCustomProperty(lightIndexStr.c_str()) );

        wxPGId itemId;

        if ( id.IsOk() )
        {
            mPropertiesViewer->DisableProperty(id);

            mPropertiesViewer->SetPropertyColour(id, *wxCYAN);

            const Ogre::ParameterList &paraList = light->getParameters();

            for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
            {
                Ogre::String value = light->getParameter(paraList[paraCount].name);

                itemId = mPropertiesViewer->AppendIn( id, CreateProperty(paraList[paraCount], value) );
                mPropertiesViewer->SetPropertyColourToDefault(itemId);
            }
        }		
    }

	mPropertiesViewer->Thaw();
	mPropertiesViewer->ExpandAll();
	mPropertiesViewer->Refresh();
}

void FairySkillEditDialog::OnMaterialSettingButtonDown(wxCommandEvent &e)
{
	wxPGId id = mPropertiesViewer->GetSelectedProperty();

	if ( true == id.IsOk() && false == mPropertiesViewer->IsPropertyEnabled(id) )
	{		

		unsigned short index = mPropertiesViewer->GetPropertyIndex(id);

		if ( index > mSkill->getNumAnimationEffectInfos() &&
			index <= mSkill->getNumAnimationEffectInfos() + mSkill->getNumAnimationRibbons() )
		{
			if ( NULL == mMaterialEditorDialog )
			{
				mMaterialEditorDialog = new MaterialEditorDialog(this, wxID_ANY, _("Fairy Material Editor"));
			}
			if ( false == mMaterialEditorDialog->IsShown() )
				mMaterialEditorDialog->Show();	

			gEffectSettingGlobalData.mCurrentRibbonIndex = 
				index - mSkill->getNumAnimationEffectInfos() - 1;

			gEffectSettingGlobalData.mCurrentEditEffectType = CEFT_RIBBON;
			gEffectSettingGlobalData.mCurrentSkill = mSkill;

			mMaterialEditorDialog->InitDialog(mSceneManipulator);
		}
	}
}
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC_CLASS(AddEffectInfoDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddEffectInfoDialog, wxDialog)
EVT_INIT_DIALOG(AddEffectInfoDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddEffectInfoDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxComboBox *item2 = new wxComboBox( this, feID_COMBOCTRL_EFFECT, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxComboBox *item3 = new wxComboBox( this, feID_COMBOCTRL_LOCATOR, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, NULL, wxCB_DROPDOWN );
	item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item1, 0, wxALIGN_CENTER|wxALL, 10 );

	wxFlexGridSizer *item5 = new wxFlexGridSizer( 2, 0, 0 );

	wxButton *item6 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	item6->SetDefault();
	item5->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item7 = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	item5->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

	sizer->Add( item5, 0, wxALIGN_CENTER|wxALL, 10 );

	this->SetSizer( sizer );
	sizer->SetSizeHints( this );

	WX::EffectManager::EffectTemplateIterator it = 
				WX::EffectManager::getSingleton().getEffectTemplateIterator();

	while ( it.hasMoreElements() )
	{
		item2->AppendString(it.peekNextKey().c_str());

		it.moveNext();
	}

	item2->SetSelection(0);

	WX::LogicModel *object = static_cast<FairySkillEditDialog *>(this->GetParent())->mDObject;
	assert (object);

	for ( unsigned short i=0; i<object->getNumBones(); ++i )
	{
		item3->AppendString(object->getBoneName(i).c_str());
	}

	Ogre::StringVector locatorNames;
	object->getLocatorNames(locatorNames);

	for ( size_t i=0; i<locatorNames.size(); ++i )
	{
		item3->Append(locatorNames[i].c_str());
	}

	item3->SetSelection(0);
}

void FairySkillEditDialog::OnAddEffectInfoButtonDown(wxCommandEvent &e)
{
	if (mSkill)
	{
		AddEffectInfoDialog *dlg = new AddEffectInfoDialog(this, wxID_ANY,
			_("Animation Effect Info"));

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			Ogre::String effect = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_EFFECT),wxComboBox)->GetValue().c_str();
			Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_LOCATOR),wxComboBox)->GetValue().c_str();

			WX::AnimationEffectInfo *effectInfo = mSkill->addAnimationEffectInfo();
			assert (effectInfo);

			effectInfo->setEffectTemplateName(effect);
			effectInfo->setAttachPoint(locator1);

			InitPropertyEditor();
		}

		dlg->Destroy();
	}	
}

void FairySkillEditDialog::OnAddLightButtonDown(wxCommandEvent &e)
{
    if (mSkill)
    {
        WX::AnimationSceneLightInfo *light = mSkill->addAnimationSceneLightInfo();
        assert (light);

        InitPropertyEditor();
    }
}

void FairySkillEditDialog::OnDelButtonDown(wxCommandEvent &e)
{
	if (mSkill)
	{
		wxPGId id = mPropertiesViewer->GetSelectedProperty();

		if ( true == id.IsOk() && false == mPropertiesViewer->IsPropertyEnabled(id) )
		{
			unsigned short index = mPropertiesViewer->GetPropertyIndex(id);

			unsigned short numEffectInfos = mSkill->getNumAnimationEffectInfos();
			unsigned short numRibbons = mSkill->getNumAnimationRibbons();
			unsigned short numSounds = mSkill->getNumAnimationSounds();
            unsigned short numLights = mSkill->getNumAnimationSceneLightInfos();

			if (index > 0 && index <= numEffectInfos)
			{
				mSkill->removeAnimationEffectInfo(index-1);
			}
			else if ( index > numEffectInfos &&
				index <= numEffectInfos + numRibbons )
			{
				mSkill->removeAnimationRibbon(index - numEffectInfos - 1);
			}
			else if ( index > numEffectInfos + numRibbons && 
				index <= numEffectInfos + numRibbons + numSounds )
			{
				mSkill->removeAnimationSound(index - numEffectInfos - numRibbons - 1);
			}
            else if ( index > numEffectInfos + numRibbons  + numSounds&& 
                index <= numEffectInfos + numRibbons + numSounds + numLights)
            {
                mSkill->removeAnimationSceneLightInfo(index - numSounds - numEffectInfos - numRibbons - 1);
            }

			InitPropertyEditor();
		}
	}
}

void FairySkillEditDialog::OnSaveAsButtonDown(wxCommandEvent &e)
{
	if (mSkill)
	{
		Ogre::String pathFileName = EFFECT_PATH + mSkill->getSkillName() + ".skill";
		std::ofstream of;
		of.open ( pathFileName.c_str(), std::ios::out | std::ios::trunc ); // append to file

		if (!of.is_open())
		{
			return;
		}

		of << "skill " << mSkill->getSkillName() << "\n";
		of << "{\n";

		const Ogre::ParameterList &paraList = mSkill->getParameters();

		for ( size_t paraCount=0; paraCount < paraList.size(); ++paraCount )
		{
			of << "\t" << paraList[paraCount].name << " " << mSkill->getParameter(paraList[paraCount].name) << "\n";
		}

		for (unsigned short i=0; i<mSkill->getNumAnimationEffectInfos(); ++i)
		{
			of << "\tAnimEffect\n" << "\t{\n";

			WX::AnimationEffectInfo *effectInfo = mSkill->getAnimationEffectInfo(i);
			assert (effectInfo);

			const Ogre::ParameterList &effectInfoList = effectInfo->getParameters();

			for ( size_t paraCount = 0; paraCount < effectInfoList.size(); ++paraCount )
			{
				of << "\t\t" << effectInfoList[paraCount].name.c_str() 
					<< " " << effectInfo->getParameter( effectInfoList[paraCount].name.c_str() ) << "\n";
			}

			of << "\t}\n";
		}

		for (unsigned short i=0; i<mSkill->getNumAnimationRibbons(); ++i)
		{
			of << "\tRibbon\n" << "\t{\n";

			WX::AnimationRibbon *ribbon = mSkill->getAnimationRibbon(i);
			assert (ribbon);

			const Ogre::ParameterList &ribbonList = ribbon->getParameters();

			for ( size_t paraCount = 0; paraCount < ribbonList.size(); ++paraCount )
			{
				of << "\t\t" << ribbonList[paraCount].name.c_str() 
					<< " " << ribbon->getParameter( ribbonList[paraCount].name.c_str() ) << "\n";
			}

			of << "\t}\n";
		}

		for (unsigned short i=0; i<mSkill->getNumAnimationSounds(); ++i)
		{
			of << "\tSound\n" << "\t{\n";

			WX::AnimationSound *sound = mSkill->getAnimationSound(i);
			assert (sound);

			const Ogre::ParameterList &soundList = sound->getParameters();

			for ( size_t paraCount = 0; paraCount < soundList.size(); ++paraCount )
			{
				of << "\t\t" << soundList[paraCount].name.c_str() 
					<< " " << sound->getParameter( soundList[paraCount].name.c_str() ) << "\n";
			}

			of << "\t}\n";
		}

        for (unsigned short i=0; i<mSkill->getNumAnimationSceneLightInfos(); ++i)
        {
            of << "\tSceneLight\n" << "\t{\n";

            WX::AnimationSceneLightInfo *light = mSkill->getAnimationSceneLightInfo(i);
            assert (light);

            const Ogre::ParameterList &lightList = light->getParameters();

            for ( size_t paraCount = 0; paraCount < lightList.size(); ++paraCount )
            {
                of << "\t\t" << lightList[paraCount].name.c_str() 
                    << " " << light->getParameter( lightList[paraCount].name.c_str() ) << "\n";
            }

            of << "\t}\n";
        }

		of << "}";

		of.close();

		// 更新模板
		WX::Skill *skill = WX::EffectManager::getSingleton().getSkill(mSkill->getSkillName());

		*skill = *mSkill;

		wxMessageBox("Success Save!");
	}	
}

void FairySkillEditDialog::OnAddRibbonButtonDown(wxCommandEvent &e)
{
	if (mSkill)
	{
		AddEffectInfoDialog *dlg = new AddEffectInfoDialog(this, wxID_ANY,
			_("Animation Ribbon"));

		bool ok = (dlg->ShowModal() == wxID_OK);
		if(ok)
		{
			Ogre::String effect = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_EFFECT),wxComboBox)->GetValue().c_str();
			Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_LOCATOR),wxComboBox)->GetValue().c_str();

			WX::AnimationRibbon *ribbon = mSkill->addAnimationRibbon();
			assert (ribbon);
//			ribbon->setEffectTemplateName(effect);
			ribbon->setAttachPoint(locator1);

			InitPropertyEditor();
		}

		dlg->Destroy();
	}
}

void FairySkillEditDialog::OnAddSoundButtonDown(wxCommandEvent &e)
{
	if (mSkill)
	{
		//AddEffectInfoDialog *dlg = new AddEffectInfoDialog(this, wxID_ANY,
		//	_("Animation Ribbon"));

		//bool ok = (dlg->ShowModal() == wxID_OK);
		//if(ok)
		//{
		//	Ogre::String locator1 = wxDynamicCast(dlg->FindWindow(feID_COMBOCTRL_LOCATOR),wxComboBox)->GetValue().c_str();

			WX::AnimationSound *sound = mSkill->addAnimationSound();
			assert (sound);

			//ribbon->setAttachPoint(locator1);

			InitPropertyEditor();
		//}

		//dlg->Destroy();
	}
}