#ifndef __FAIRYEDITORAPP_H__
#define __FAIRYEDITORAPP_H__

#include <wx/app.h>

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------
//声音系统初始化
typedef bool (__stdcall* FUNC_INITSOUND)(const char* szSndPath, const char* szDBCFile);
//查询声音列表
typedef bool (__stdcall* FUNC_ENUMSOUNDFILE)(int nIndex, char* szSoundFileBuf, int nBufSize, int& id);
//对外提供的声音播放接口,返回声音源HANDLE
typedef int (__stdcall*  FUNC_PLAYSOUND)(const char* szSoundFile, float* fvPos, bool bLoop);
//对外提供声音的停止接口
typedef void (__stdcall* FUNC_STOPSOUND)(int nHandle);
//对外提供声音系统的释放接口
typedef void (__stdcall* FUNC_DEINIT)(void);

typedef void (__stdcall* FUNC_SETSOUNDVOLUME)(int handle, float gain);

class FairyEditorFrame;

namespace WX {
    class SceneManipulator;
}

namespace Ogre
{
    class Exception;
}
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class FairyEditorApp : public wxApp
{
public:
	bool	mIsRenderEnable;
	bool	mRenderUserDXorGL;//use d3dx when true, use gl when false.

	FUNC_INITSOUND funcInit;
	FUNC_ENUMSOUNDFILE funcEnumSoundFile;
	FUNC_PLAYSOUND funcPlay;
	FUNC_STOPSOUND funcStop;
    FUNC_DEINIT funcDeInit;
    FUNC_SETSOUNDVOLUME funcSetSoundVolume;

protected:
    FairyEditorFrame* mMainFrame;
    WX::SceneManipulator* mSceneManipulator;

    wxLocale _locale;	

public:
    FairyEditorApp();
    ~FairyEditorApp();

    FairyEditorFrame* GetMainFrame(void) const
    {
        return mMainFrame;
    }

    WX::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

    // This is only called if OnInit() returned true so it's a good place to do
    // any cleanup matching the initializations done there.
    virtual int OnExit();

    virtual bool OnExceptionInMainLoop();
    virtual void OnFatalException();
    virtual void OnUnhandledException();

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnIdle(wxIdleEvent &e);

protected:
	void SetKeyboardShortcuts(void);

    /// 异常处理函数
    void _ExceptionHandler(PEXCEPTION_POINTERS pException);

    /// 初始化实现
    bool _InitImpl(void);

    void _displayExceptionMessageBox(const Ogre::Exception& e);

};

DECLARE_APP(FairyEditorApp)

#endif // 
