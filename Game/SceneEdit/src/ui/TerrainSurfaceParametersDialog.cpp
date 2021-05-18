/********************************************************************
filename:   TerrainSurfaceParametersDialog.cpp

purpose:    用于设置地形材质的对话框
*********************************************************************/

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "TerrainSurfaceParametersDialog.h"
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

#include <wx/colordlg.h>

#include "TerrainSurfaceParametersDialog.h"
#include "SceneManipulator.h"
#include "UIColourConverter.h"

#include "Core/Terrain.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/TerrainSurfaceParameters_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>

IMPLEMENT_DYNAMIC_CLASS(TerrainSurfaceParametersDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(TerrainSurfaceParametersDialog, wxDialog)
    EVT_INIT_DIALOG(TerrainSurfaceParametersDialog::OnInitDialog)

    EVT_TEXT_ENTER(ID_TEXTCTRL_AMBIENT, TerrainSurfaceParametersDialog::OnAmbientTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_DIFFUSE, TerrainSurfaceParametersDialog::OnDiffuseTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_SPECULAR, TerrainSurfaceParametersDialog::OnSpecularTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_EMISSIVE, TerrainSurfaceParametersDialog::OnEmissiveTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_SHININESS, TerrainSurfaceParametersDialog::OnShininessTextChanged)

    EVT_BUTTON(ID_BUTTON_COLOUR_AMBIENT, TerrainSurfaceParametersDialog::OnAmbientColourButtonDown)
    EVT_BUTTON(ID_BUTTON_COLOUR_DIFFUSE, TerrainSurfaceParametersDialog::OnDiffuseColourButtonDown)
    EVT_BUTTON(ID_BUTTON_COLOUR_SPECULAR, TerrainSurfaceParametersDialog::OnSpecularColourButtonDown)
    EVT_BUTTON(ID_BUTTON_COLOUR_EMISSIVE, TerrainSurfaceParametersDialog::OnEmissiveColourButtonDown)

    EVT_BUTTON(ID_BUTTON_OK, TerrainSurfaceParametersDialog::OnOK)
    EVT_BUTTON(ID_BUTTON_CANCEL, TerrainSurfaceParametersDialog::OnCancel)

END_EVENT_TABLE()

void
TerrainSurfaceParametersDialog::OnInitDialog(wxInitDialogEvent &e)
{
    assert (mManipulator);
    mTerrainData = mManipulator->getTerrainData();
    assert (mTerrainData);

    mTerrain = mManipulator->getTerrain();
    assert (mTerrain);

    mOriginSurfaceParams = mTerrainData->mSurfaceParams;

    wxDialog::OnInitDialog(e);
    wxSizer* sizer = TerrainSurfaceParams(this, true, true);

    mAmbientText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_AMBIENT), wxTextCtrl);
    mDiffuseText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_DIFFUSE), wxTextCtrl);
    mSpecularText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_SPECULAR), wxTextCtrl);
    mEmissiveText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_EMISSIVE), wxTextCtrl);
    mShininessText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_SHININESS), wxTextCtrl);

    mAmbientButton = wxDynamicCast(this->FindWindow(ID_BUTTON_COLOUR_AMBIENT), wxButton);
    mDiffuseButton = wxDynamicCast(this->FindWindow(ID_BUTTON_COLOUR_DIFFUSE), wxButton);
    mSpecularButton = wxDynamicCast(this->FindWindow(ID_BUTTON_COLOUR_SPECULAR), wxButton);
    mEmissiveButton = wxDynamicCast(this->FindWindow(ID_BUTTON_COLOUR_EMISSIVE), wxButton);

    assert ( mAmbientText && mDiffuseText && mSpecularText && mEmissiveText && mShininessText
         && mAmbientButton && mDiffuseButton && mSpecularButton && mEmissiveButton );

    setColourText();
    setButtonColour();    
}

void TerrainSurfaceParametersDialog::setColourText(void)
{
    mAmbientText->SetValue( Ogre::StringConverter::toString(mTerrainData->mSurfaceParams.ambient) );
    mDiffuseText->SetValue( Ogre::StringConverter::toString(mTerrainData->mSurfaceParams.diffuse) );
    mSpecularText->SetValue( Ogre::StringConverter::toString(mTerrainData->mSurfaceParams.specular) );
    mEmissiveText->SetValue( Ogre::StringConverter::toString(mTerrainData->mSurfaceParams.emissive) );
    mShininessText->SetValue( Ogre::StringConverter::toString(mTerrainData->mSurfaceParams.shininess) );
}

void TerrainSurfaceParametersDialog::setButtonColour(void)
{
    unsigned char r, g, b;

    FColourToIColour(mTerrainData->mSurfaceParams.ambient.r,
        mTerrainData->mSurfaceParams.ambient.g,
        mTerrainData->mSurfaceParams.ambient.b,
        r, g, b);

    mAmbientButton->SetBackgroundColour(wxColour(r,g,b));

    FColourToIColour(mTerrainData->mSurfaceParams.diffuse.r,
        mTerrainData->mSurfaceParams.diffuse.g,
        mTerrainData->mSurfaceParams.diffuse.b,
        r, g, b);

    mDiffuseButton->SetBackgroundColour(wxColour(r,g,b));

    FColourToIColour(mTerrainData->mSurfaceParams.specular.r,
        mTerrainData->mSurfaceParams.specular.g,
        mTerrainData->mSurfaceParams.specular.b,
        r, g, b);

    mSpecularButton->SetBackgroundColour(wxColour(r,g,b));

    FColourToIColour(mTerrainData->mSurfaceParams.emissive.r,
        mTerrainData->mSurfaceParams.emissive.g,
        mTerrainData->mSurfaceParams.emissive.b,
        r, g, b);

    mEmissiveButton->SetBackgroundColour(wxColour(r,g,b));
}

void TerrainSurfaceParametersDialog::OnAmbientTextChanged(wxCommandEvent &e)
{
    Ogre::ColourValue colour = Ogre::StringConverter::parseColourValue(mAmbientText->GetValue().c_str());
    mTerrainData->mSurfaceParams.ambient = colour;

    mTerrain->resetSurfaceParams();

    setButtonColour();
}

void TerrainSurfaceParametersDialog::OnDiffuseTextChanged(wxCommandEvent &e)
{
    Ogre::ColourValue colour = Ogre::StringConverter::parseColourValue(mDiffuseText->GetValue().c_str());
    mTerrainData->mSurfaceParams.diffuse = colour;

    mTerrain->resetSurfaceParams();

    setButtonColour();
}

void TerrainSurfaceParametersDialog::OnSpecularTextChanged(wxCommandEvent &e)
{
    Ogre::ColourValue colour = Ogre::StringConverter::parseColourValue(mSpecularText->GetValue().c_str());
    mTerrainData->mSurfaceParams.specular = colour;

    mTerrain->resetSurfaceParams();

    setButtonColour();
}

void TerrainSurfaceParametersDialog::OnEmissiveTextChanged(wxCommandEvent &e)
{
    Ogre::ColourValue colour = Ogre::StringConverter::parseColourValue(mEmissiveText->GetValue().c_str());
    mTerrainData->mSurfaceParams.emissive = colour;

    mTerrain->resetSurfaceParams();

    setButtonColour();
}

void TerrainSurfaceParametersDialog::OnShininessTextChanged(wxCommandEvent &e)
{
    Ogre::Real shininess = Ogre::StringConverter::parseReal(mShininessText->GetValue().c_str());
    mTerrainData->mSurfaceParams.shininess = shininess;

    mTerrain->resetSurfaceParams();
}

void TerrainSurfaceParametersDialog::OnAmbientColourButtonDown(wxCommandEvent &e)
{
    wxColour colour;
    if ( getColourFromDialog(colour) )
    {
        Ogre::ColourValue colourValue;

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            colourValue.r, colourValue.g, colourValue.b);

        mTerrainData->mSurfaceParams.ambient = colourValue;

        mTerrain->resetSurfaceParams();

        setColourText();
        setButtonColour();
    }
}

void TerrainSurfaceParametersDialog::OnDiffuseColourButtonDown(wxCommandEvent &e)
{
    wxColour colour;
    if ( getColourFromDialog(colour) )
    {
        Ogre::ColourValue colourValue;

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            colourValue.r, colourValue.g, colourValue.b);

        mTerrainData->mSurfaceParams.diffuse = colourValue;

        mTerrain->resetSurfaceParams();

        setColourText();
        setButtonColour();
    }    
}

void TerrainSurfaceParametersDialog::OnSpecularColourButtonDown(wxCommandEvent &e)
{
    wxColour colour;
    if ( getColourFromDialog(colour) )
    {
        Ogre::ColourValue colourValue;

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            colourValue.r, colourValue.g, colourValue.b);

        mTerrainData->mSurfaceParams.specular = colourValue;

        mTerrain->resetSurfaceParams();

        setColourText();
        setButtonColour();
    }    
}

void TerrainSurfaceParametersDialog::OnEmissiveColourButtonDown(wxCommandEvent &e)
{
    wxColour colour;
    if ( getColourFromDialog(colour) )
    {
        Ogre::ColourValue colourValue;

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            colourValue.r, colourValue.g, colourValue.b);

        mTerrainData->mSurfaceParams.emissive = colourValue;

        mTerrain->resetSurfaceParams();

        setColourText();
        setButtonColour();
    }        
}

bool TerrainSurfaceParametersDialog::getColourFromDialog(wxColour& col)
{
    wxColourDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxColourData retData = dialog.GetColourData();
        col = retData.GetColour();
        return true;
    }
    else
        return false;
}

void TerrainSurfaceParametersDialog::OnOK(wxCommandEvent& event)
{ 
    wxDialog::OnOK(event);
}

void TerrainSurfaceParametersDialog::OnCancel(wxCommandEvent& event)
{
    // 取消之前调整的材质，使用初始材质
    mTerrainData->mSurfaceParams = mOriginSurfaceParams;
    mTerrain->resetSurfaceParams();

    wxDialog::OnCancel(event);
}