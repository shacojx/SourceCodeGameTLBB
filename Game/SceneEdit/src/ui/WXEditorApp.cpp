// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXEditorApp.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/intl.h>
    #include <wx/menu.h>
#endif

#include <wx/config.h>

// ----------------------------------------------------------------------------
// Local headers
// ----------------------------------------------------------------------------

#include "WXEditorApp.h"
#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include "RenderSystemConfigDialog.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreMaterialManager.h>

#include "SceneManipulator.h"

#include "Core/WXMaterialScriptSolver.h"
#include "Core/WXTerrainSolver.h"
#include "Core/WXSceneSolver.h"
#include "Core/WXResourceSolverHelper.h"
#include "Core/TerrainData.h"
#include "Core/Terrain.h"

#include "PostFilter/WXPostFilterManager.h"
#include "PostFilter/WXPostFilters.h"

// 音效系统
#include "../../SndShell/SndShell.h"

// 异常处理
#include <DbgHelp.h>
#include <excpt.h>

// ----------------------------------------------------------------------------
// Resource relocation processing
// ----------------------------------------------------------------------------
static bool
RelocationResources(const WX::ResourceSolverList& solvers)
{
    // TODO: UI support

    bool modified = false;
    for (WX::ResourceSolverList::const_iterator itSolver = solvers.begin(); itSolver != solvers.end(); ++itSolver)
    {
        WX::ResourceSolver* solver = itSolver->get();
        if (!solver)
            continue;

        const WX::String& solverName = solver->getName();

        for (WX::UnsolvedResourceList::const_iterator itItem = solver->getUnsolvedResources().begin(); itItem != solver->getUnsolvedResources().end(); ++itItem)
        {
            WX::UnsolvedResource* unsolved = (*itItem).get();
            const WX::String& origin = unsolved->getOrigin();
            const WX::String& description = unsolved->getDescription();
            const Ogre::StringVector* recommends = unsolved->getRecommendValues();
            if (!recommends || recommends->empty())
            {
                wxLogMessage(/*_("Can't found resource '%s' (%s) for '%s'.")*/wxT("子类 '%s' 在资源 '%s'中不存在, 无法子类化!"),
                    origin.c_str(), description.c_str(), solverName.c_str());
            }
            else
            {
                const WX::String& resolved = recommends->front();
                unsolved->setResolvedValue(resolved);
                wxLogMessage(/*_("Can't found resource '%s' (%s) for '%s', use '%s' instead.")*/wxT("没找到条目"),
                    origin.c_str(), description.c_str(), solverName.c_str(), resolved.c_str());
            }
        }

        modified |= solver->commitPermanent();
    }

    return modified;
}

// ----------------------------------------------------------------------------
// the local scene manipulator class for process resource relocation
// ----------------------------------------------------------------------------

class wxSceneManipulator : public WX::SceneManipulator
{
protected:
    void _postprocessScene(void);
};

void
wxSceneManipulator::_postprocessScene(void)
{
    WX::ResourceSolverList solvers;
    solvers.push_back(WX::findUnsolvedTerrainBrushes(getTerrain()));
    solvers.push_back(WX::findUnsolvedSceneItems(getSceneInfo()));

    bool modified = RelocationResources(solvers);

    SceneManipulator::_postprocessScene();
}

// ----------------------------------------------------------------------------
// the listener used to found detail error when parsing scripts
// ----------------------------------------------------------------------------

class ScriptParsingListener : public Ogre::ResourceGroupListener
{
public:
    Ogre::String _groupName;
    size_t _scriptCount;
    size_t _scriptIndex;
    Ogre::String _scriptName;

    ScriptParsingListener(void)
        : _groupName()
        , _scriptCount()
        , _scriptIndex()
        , _scriptName()
    {
        Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);
    }

    ~ScriptParsingListener()
    {
        Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
    }

    void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
    {
        _groupName = groupName;
        _scriptCount = scriptCount;
        _scriptIndex = 0;
        _scriptName.clear();
        wxStaticCast(wxTheApp->GetTopWindow(), wxFrame)->SetStatusText(
            wxString::Format(/*_("Parsing resource group: %s")*/wxT("%s 不是位图资源."), groupName.c_str()));
        wxYield();
    }

    void scriptParseStarted(const Ogre::String& scriptName)
    {
        _scriptName = scriptName;
        wxStaticCast(wxTheApp->GetTopWindow(), wxFrame)->SetStatusText(
            wxString::Format(/*_("Parsing script: %s")*/wxT("正在解析脚本文件：%s"), scriptName.c_str()));
        wxYield();
    }

#if OGRE_VERSION >= 0x010300
    void scriptParseEnded(const Ogre::String& scriptName)
#else
    void scriptParseEnded(void)
#endif
    {
        _scriptName.clear();
        ++_scriptIndex;
        wxStaticCast(wxTheApp->GetTopWindow(), wxFrame)->SetStatusText(
            wxEmptyString);
        wxYield();
    }

    void resourceGroupScriptingEnded(const Ogre::String& groupName)
    {
        _groupName.clear();
        _scriptCount = 0;
        wxStaticCast(wxTheApp->GetTopWindow(), wxFrame)->SetStatusText(
            wxEmptyString);
        wxYield();
    }

    ////////////////////////////////////////////////////////////////////////////

    void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount)
    {
    }

    void resourceLoadStarted(const Ogre::ResourcePtr& resource)
    {
    }

    void resourceLoadEnded(void)
    {
    }

    void worldGeometryStageStarted(const Ogre::String& description)
    {
    }

    void worldGeometryStageEnded(void)
    {
    }

    void resourceGroupLoadEnded(const Ogre::String& groupName)
    {
    }
};

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(FairyEditorApp)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(FairyEditorApp, wxApp)
    EVT_IDLE(FairyEditorApp::OnIdle)
END_EVENT_TABLE()

FairyEditorApp::FairyEditorApp()
    : wxApp()
    , mMainFrame(NULL)
    , mSceneManipulator(NULL)
	, mIsRenderEnable(true)
	, mRenderUserDXorGL(false)
{
	funcInit	= NULL;
	funcEnumSoundFile = NULL;
	funcPlay	= NULL;
	funcStop    = NULL;
    funcDeInit  = NULL;
    funcSetSoundVolume = NULL;
}

FairyEditorApp::~FairyEditorApp()
{
    if (funcDeInit)
    {
        funcDeInit();
    }

    delete mSceneManipulator;

    // Must delete the global wxConfig here because if OnInit() failed,
    // it will never delete'ed by wxWidgets
    delete wxConfigBase::Set(NULL);
}

void
FairyEditorApp::OnIdle(wxIdleEvent &e)
{
    wxApp::OnIdle(e);	

    if ( mSceneManipulator && mSceneManipulator->getRenderWindow() && mSceneManipulator->getRenderWindow()->isActive()
		&& mIsRenderEnable && mMainFrame->GetCanvas()->doUpdate(true) )
    {
        e.RequestMore(IsActive());
    }//
}

// 'Main program' equivalent: the program execution "starts" here
bool FairyEditorApp::_InitImpl()
{
    // 只有调用完这个之后,OnFatalException才会被调用
    wxHandleFatalExceptions(true);

	// Initialize locale
	wxLocale::AddCatalogLookupPathPrefix(wxT("../intl"));
	_locale.Init();
	_locale.AddCatalog(wxT("FairyEditor"));

	// Setup application description
	SetVendorName(wxT("HopeCool"));
	SetClassName(wxT("FairyEditor"));
	SetAppName(_("Fairy Editor"));

	// Create the default config object
	wxConfigBase::Get()->SetRecordDefaults(true);


    ///////////////////////////////////////////////////////////////////////////////////////////////////

	funcInit	= (FUNC_INITSOUND)(&SSInit);
	funcEnumSoundFile = (FUNC_ENUMSOUNDFILE)(&SSEnumSoundFile);
	funcPlay	= (FUNC_PLAYSOUND)(&SSPlaySoundFunc);
	funcStop	= (FUNC_STOPSOUND)(&SSStopSoundFunc);		
    funcDeInit  = (FUNC_DEINIT)(&SSDeInit);
    funcSetSoundVolume = (FUNC_SETSOUNDVOLUME)(&SSSetSoundVolume);
	// 声音系统的初始化
	if ( NULL == funcInit || false == funcInit("..\\Sound\\", "..\\config\\SoundInfo.txt") )
    {
        wxMessageBox(_("Sound System Init Failed!"));
    }

    mMainFrame = new FairyEditorFrame(NULL, wxID_ANY, GetAppName(),
        wxDefaultPosition, wxSize(800, 600));

	// 设置快捷键
	SetKeyboardShortcuts();

    wxMenuBar *menu_bar = mMainFrame->GetMenuBar();
    if (menu_bar)
    {
        //// Make help menu
        wxMenu *help_menu = new wxMenu;
        help_menu->Append(wxID_ABOUT,/* _("&About\tF1")*/wxT("[&A]关于"));

        //// Add help menu to menu bar
        menu_bar->Append(help_menu,/* _("&Help")*/wxT("[&H]帮助"));
    }

    mMainFrame->Centre();
#ifndef __WXMAC__
    mMainFrame->Show(true);
#endif
    mMainFrame->Update();
    SetTopWindow(mMainFrame);

    //////////////////////////////////////////////////////////////////////////

    mSceneManipulator = new wxSceneManipulator;

    mSceneManipulator->init(
        "Resources.cfg",
        //"Plugins.cfg",
		"",
        "Fairy.cfg",
        "Fairy.log");

	Ogre::Root* pTheRoot = &(Ogre::Root::getSingleton());
	if( mRenderUserDXorGL )
	{
		//使用D3D渲染
		pTheRoot->loadRenderPlugins_D3D9();
	}
	else
	{
		//使用OpenGL渲染
		pTheRoot->loadRenderPlugins_GL();
	}
	//加载其他插件
	pTheRoot->loadInnerPlugins();

#if 1
    wxDialog *dlg = new RenderSystemConfigDialog(mMainFrame, wxID_ANY,
        GetAppName() + /*_(" Rendering Setup")*/wxT("  渲染系统配置"));
    bool ok = dlg->ShowModal() == wxID_OK;
    dlg->Destroy();
#else
    bool ok = Ogre::Root::getSingleton().showConfigDialog();
#endif

    if (!ok)
    {
        return false;
    }

    ScriptParsingListener listener;
    try
    {
        mMainFrame->InitScene(mSceneManipulator);
    }	
    catch (const Ogre::Exception& e)
    {
        bool rethrow = true;

        if (e.getNumber() == Ogre::Exception::ERR_DUPLICATE_ITEM)
        {
            if (e.getSource() == "ResourceManager::add")
            {
                if (Ogre::StringUtil::endsWith(listener._scriptName, ".material", true))
                {
                    char materialName[256];
                    if (sscanf(e.getDescription().c_str(), "Resource with the name %255s already exists.", materialName) == 1)
                    {
                        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
                        if (!material.isNull())
                        {
                            wxString message;

                            WX::StringVectorPtr currentNames = WX::findResourceFilenames(listener._groupName, listener._scriptName);
                            const Ogre::String& currentName = currentNames.isNull() || currentNames->empty() ? listener._scriptName : currentNames->front();
                            if (material->getOrigin().empty())
                            {
                                message.Printf(_("Predefined material \"%s\" duplicated in file \"%s\", the application will exit."),
                                        material->getName().c_str(), currentName.c_str());
                            }
                            else
                            {
                                WX::StringVectorPtr originNames = WX::findResourceFilenames(material->getGroup(), material->getOrigin());
                                const Ogre::String& originName = originNames.isNull() || originNames->empty() ? material->getOrigin() : originNames->front();
                                if (currentName == originName)
                                {
                                    message.Printf(/*_("The material \"%s\" occuring in the file \"%s\" more than once, the application will exit.")*/wxT("材质 \"%s\" 在文件 \"%s\" 中重复，应用程序退出。"),
                                            material->getName().c_str(), currentName.c_str());
                                }
                                else
                                {
                                    message.Printf(/*_("Duplicate material \"%s\" in files \"%s\" and \"%s\", the application will exit.")*/wxT("材质重复 \"%s\" 在文件中 \"%s\" 和 \"%s\"，应用程序退出。"),
                                            material->getName().c_str(), currentName.c_str(), originName.c_str());
                                }
                            }

                            wxMessageBox(message,
                                wxString::Format(/*_("%s Fatal")*/wxT("%s 致命错误"), GetAppName().c_str()),
                                wxICON_ERROR | wxOK, GetTopWindow());

                            return false;
                        }
                    }
                }
            }
        }

        if (rethrow)
            throw;
    }

	TCHAR tcsQuality[5] = "";
	GetPrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("High"), tcsQuality, 5, _T("Startup.ini"));
	if (0 == strcmp(tcsQuality, _T("None")))
	{		
		mSceneManipulator->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_NONE);
	}
	else if (0 == strcmp(tcsQuality, _T("Low")))
	{
		mSceneManipulator->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_LOW);
	}
	else
	{		
		mSceneManipulator->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_HIGH);
	}

    WX::ResourceSolverList solvers;
    solvers.push_back(WX::findUnsolvedMaterialTextures());

    bool modified = RelocationResources(solvers);

    WX::PostFilterManager* pfm = mSceneManipulator->getPostFilterManager();
    registerAllPostFilterFactories(pfm);
    pfm->createPostFilter("HeatVision", mSceneManipulator->getViewport());
    pfm->createPostFilter("Bloom", mSceneManipulator->getViewport());
    pfm->createPostFilter("Floodlighting", mSceneManipulator->getViewport());
    pfm->createPostFilter("MotionBlur", mSceneManipulator->getViewport());
    pfm->createPostFilter("Fade", mSceneManipulator->getViewport());

    Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();
    sceneManager->setShadowColour(Ogre::ColourValue(0.75, 0.75, 0.75));
    sceneManager->setShadowTextureSettings(1024, 2);

    Ogre::ShadowTechnique shadowTechnique = Ogre::SHADOWTYPE_NONE;
    //shadowTechnique = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
    //shadowTechnique = Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
    //shadowTechnique = Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
    //shadowTechnique = Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
    sceneManager->setShadowTechnique(shadowTechnique);

    //sceneManager->setShadowFarDistance(1500);
    //mSceneManipulator->getViewport()->setShadowsEnabled(false);

    return true;
}

int FairyEditorApp::OnExit()
{
    return wxApp::OnExit();
}

void 
FairyEditorApp::SetKeyboardShortcuts(void)
{
	mMainFrame->SetKeyboardShortcuts();
}

void 
FairyEditorApp::_ExceptionHandler(PEXCEPTION_POINTERS pException)
{
    typedef	BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
        IN HANDLE			hProcess,
        IN DWORD			ProcessId,
        IN HANDLE			hFile,
        IN MINIDUMP_TYPE	DumpType,
        IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
        IN PMINIDUMP_USER_STREAM_INFORMATION		UserStreamParam, OPTIONAL
        IN PMINIDUMP_CALLBACK_INFORMATION			CallbackParam OPTIONAL
        );

    HANDLE	hDump_File;
    CHAR	Dump_Path[MAX_PATH];

    GetModuleFileName(NULL, Dump_Path, sizeof(Dump_Path));	//path of current process
    // Try to get MiniDumpWriteDump() address.
    HMODULE hDbgHelp = LoadLibrary("DBGHELP.DLL");
    MINIDUMP_WRITE_DUMP MiniDumpWriteDump_ = (MINIDUMP_WRITE_DUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

    if (MiniDumpWriteDump_)
    {
        MINIDUMP_EXCEPTION_INFORMATION	M;

        M.ThreadId = GetCurrentThreadId();
        M.ExceptionPointers = pException;
        M.ClientPointers = 0;

        lstrcpy(Dump_Path + lstrlen(Dump_Path) - 3, "dmp");

        hDump_File = CreateFile(Dump_Path,
            GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        MiniDumpWriteDump_(GetCurrentProcess(), GetCurrentProcessId(), hDump_File,
            MiniDumpNormal, (pException) ? &M : NULL, NULL, NULL);

        CloseHandle(hDump_File);

        FreeLibrary(hDbgHelp);
    }

    wxMessageBox("The program is crash!\n"
        "Check the SceneEdit.dmp for crash info!", "Crash", wxICON_ERROR);
}

bool 
FairyEditorApp::OnInit()
{
    // 捕获初始化阶段所产生的异常
    try
    {
        return _InitImpl();
    }
    catch(Ogre::Exception& e)
    {
        _displayExceptionMessageBox(e);
    }
    catch (std::exception& e)
    {
        wxMessageBox(e.what(), "std Exception", wxICON_ERROR);
    }
    catch(...)
    {
        throw;
    }

    return false;
}

bool  
FairyEditorApp::OnExceptionInMainLoop()
{
    // 处理运行过程中碰到的异常,如果是Ogre相关的异常,就弹出对话框,其他异常继续抛出,被OnUnhandledException截获
    try
    {
        throw;
    }
    catch (Ogre::Exception& e)
    {
        _displayExceptionMessageBox(e);
    }
    catch (std::exception& e)
    {
        wxMessageBox(e.what(), "std Exception", wxICON_ERROR);
    }
    catch(...)
    {
        throw;
    }

    return true;
}

void FairyEditorApp::OnUnhandledException()
{
    wxMessageBox("Unhandled Exception", "OnUnhandledException", wxICON_ERROR);
}

void 
FairyEditorApp::OnFatalException()
{
    // 处理crash时的异常,记录下异常信息,写dmp文件
    __try
    {
        throw;
    }
    __except(_ExceptionHandler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

void 
FairyEditorApp::_displayExceptionMessageBox(const Ogre::Exception& e)
{
    // for the foolish people that didn't want to study English at all,we have to translate it to Chinese. F**K!
    Ogre::String fullDesc = e.getFullDescription();

    if ( Ogre::String::npos != fullDesc.find("ParticleSystem template with name") &&
        Ogre::String::npos != fullDesc.find("already exists") )
        fullDesc += "\n粒子系统模板重复,请删除重复的粒子系统模板\n";

    wxMessageBox(fullDesc.c_str(), "Ogre Exception", wxICON_ERROR);
}