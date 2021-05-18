// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ExceptionInfoDialog.h"
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

#include "ExceptionInfoDialog.h"
#include "SceneManipulator.h"

// ----------------------------------------------------------------------------
// Other wxWidgets headers
// ----------------------------------------------------------------------------

#include <wx/listbox.h>

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/ExceptionInfoList_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreResourceGroupManager.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

IMPLEMENT_DYNAMIC_CLASS(ExceptionInfoDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ExceptionInfoDialog, wxDialog)
	EVT_INIT_DIALOG(ExceptionInfoDialog::OnInitDialog)
    EVT_LISTBOX_DCLICK(ID_EXCEPTION_LISTBOX, ExceptionInfoDialog::OnListBoxDoubleClick)
END_EVENT_TABLE()

void
ExceptionInfoDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);
	wxSizer* sizer = ExceptionInfo(this, true, true);

	// 获取各控件的引用
	mListBox = wxDynamicCast(this->FindWindow(ID_EXCEPTION_LISTBOX),wxListBox);

	assert (mListBox);

	mListBox->InsertItems(mDescArrayString,0);
}

void 
ExceptionInfoDialog::fillExceptionInfo(void)
{
	assert (mSceneManipulator);

    const WX::System::ExceptionInfoMap &infoMap = mSceneManipulator->getExceptionInfoMap();

	assert ( infoMap.size() > 0 );

	WX::System::ExceptionInfoMap::const_iterator i = infoMap.begin();

	while ( i != infoMap.end() )
	{	
		// 添加异常描述，这些描述都将显示在listbox中
		mDescArrayString.Add((i->first).c_str());		
		++i;
	}	
}

void
ExceptionInfoDialog::OnListBoxDoubleClick( wxCommandEvent &event )
{
	ExceptionHandleDialog *dlg = new ExceptionHandleDialog( this, wxID_ANY,
		/*_("ExceptionHandle Dialog")*/wxT("异常处理对话框"), event.GetSelection() );
		
	assert (mSceneManipulator);

	dlg->ShowModal();	

	dlg->Destroy();

	// 如果异常描述列表为空，说明全部错误都已经修正，可以关闭自己了
	if (mDescArrayString.IsEmpty())
		Close();
}

////////////////////////////////
IMPLEMENT_DYNAMIC_CLASS(ExceptionHandleDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ExceptionHandleDialog, wxDialog)
	EVT_INIT_DIALOG(ExceptionHandleDialog::OnInitDialog)
	EVT_BUTTON(ID_BUTTON_DELETE_FROM_SCENE, ExceptionHandleDialog::OnDeleteFromFileButtonDown)
	EVT_BUTTON(ID_BUTTON_RELOCATE, ExceptionHandleDialog::OnRelocateButtonDown)
END_EVENT_TABLE()

void
ExceptionHandleDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);
	wxSizer* sizer = ExceptionHandle(this, true, true);	
}

void 
ExceptionHandleDialog::OnDeleteFromFileButtonDown(wxCommandEvent &e)
{
	ExceptionInfoDialog *dlg = static_cast<ExceptionInfoDialog *>( GetParent() );
	assert (dlg);

	assert ( selectIndex >= 0 && selectIndex < dlg->mDescArrayString.Count() );
	Ogre::String descName = dlg->mDescArrayString[selectIndex].c_str();
	
    const WX::System::ExceptionInfoMap &infoMap = dlg->mSceneManipulator->getExceptionInfoMap();

	WX::System::ExceptionInfoMap::const_iterator i = infoMap.find(descName);

	assert (i != infoMap.end() );

	const WX::System::ExceptionInfoList &list = i->second;

	assert (list.size() > 0);

	wxString resultStr;

	resultStr << /*_("Success deleting objects from scene:\n")*/wxT("成功地把物体 %s 从场景中删除，请重新保存场景使该改动生效！");

	for ( size_t j=0; j<list.size(); ++j )
	{
		// 删除与这个异常相关的object
		dlg->mSceneManipulator->removeObject(list[j].objectName);
		resultStr << list[j].objectName << '\n';
	}	

	wxMessageBox(resultStr);

    // 删除listbox中的对应项
	dlg->mListBox->Delete(selectIndex);
	dlg->mDescArrayString.RemoveAt(selectIndex);

	Close();
}

void 
ExceptionHandleDialog::OnRelocateButtonDown(wxCommandEvent &e)
{
	/*wxFileDialog fileDialog(this,
        _("Relocate the missing file"));

    if (fileDialog.ShowModal() == wxID_OK)
    {		
       
    }*/
}