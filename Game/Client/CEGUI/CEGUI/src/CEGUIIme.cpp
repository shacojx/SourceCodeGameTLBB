
#define UNICODE

#include "CEGUIString.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIIme.h"
#include "elements/CEGUIEditboxInterface.h"
#include "elements/CEGUIEditbox.h"
#include "elements/CEGUIMultiLineEditbox.h"


//#include "AxTrace.h"

#include <usp10.h>
#include <dimm.h>

#include <string>
#include <vector>
#include <stack>
#include <list>
#include <set>
#include <assert.h>
#include "windows.h"
#include "tchar.h"

// Start of Ime namespace section
namespace Ime
{

#define IMM32_DLLNAME "\\imm32.dll"
#define VER_DLLNAME "\\version.dll"

#define MAX_COMPSTRING_SIZE 256

//////////////////////////////////////////////////////////////////////
// Empty implementation of the IMM32 API
INPUTCONTEXT* WINAPI Dummy_ImmLockIMC( HIMC ) { return NULL; }
BOOL WINAPI Dummy_ImmUnlockIMC( HIMC ) { return FALSE; }
LPVOID WINAPI Dummy_ImmLockIMCC( HIMCC ) { return NULL; }
BOOL WINAPI Dummy_ImmUnlockIMCC( HIMCC ) { return FALSE; }
BOOL WINAPI Dummy_ImmDisableTextFrameService( DWORD ) { return TRUE; }
LONG WINAPI Dummy_ImmGetCompositionStringW( HIMC, DWORD, LPVOID, DWORD ) { return IMM_ERROR_GENERAL; }
DWORD WINAPI Dummy_ImmGetCandidateListW( HIMC, DWORD, LPCANDIDATELIST, DWORD ) { return 0; }
HIMC WINAPI Dummy_ImmGetContext( HWND ) { return NULL; }
BOOL WINAPI Dummy_ImmReleaseContext( HWND, HIMC ) { return FALSE; }
HIMC WINAPI Dummy_ImmAssociateContext( HWND, HIMC ) { return NULL; }
BOOL WINAPI Dummy_ImmGetOpenStatus( HIMC ) { return 0; }
BOOL WINAPI Dummy_ImmSetOpenStatus( HIMC, BOOL ) { return 0; }
BOOL WINAPI Dummy_ImmGetConversionStatus( HIMC, LPDWORD, LPDWORD ) { return 0; }
HWND WINAPI Dummy_ImmGetDefaultIMEWnd( HWND ) { return NULL; }
UINT WINAPI Dummy_ImmGetIMEFileNameA( HKL, LPSTR, UINT ) { return 0; }
UINT WINAPI Dummy_ImmGetVirtualKey( HWND ) { return 0; }
BOOL WINAPI Dummy_ImmNotifyIME( HIMC, DWORD, DWORD, DWORD ) { return FALSE; }
BOOL WINAPI Dummy_ImmSetConversionStatus( HIMC, DWORD, DWORD ) { return FALSE; }
BOOL WINAPI Dummy_ImmSimulateHotKey( HWND, DWORD ) { return FALSE; }
BOOL WINAPI Dummy_ImmIsIME( HKL ) { return FALSE; }
BOOL WINAPI Dummy_ImmSetCompositionWindow(HIMC, LPCOMPOSITIONFORM) { return FALSE; }

// Traditional Chinese IME
UINT WINAPI Dummy_GetReadingString( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) { return 0; }
BOOL WINAPI Dummy_ShowReadingWindow( HIMC, BOOL ) { return FALSE; }

// Verion library imports
BOOL APIENTRY Dummy_VerQueryValueA( const LPVOID, LPSTR, LPVOID *, PUINT ) { return 0; }
BOOL APIENTRY Dummy_GetFileVersionInfoA( LPSTR, DWORD, DWORD, LPVOID ) { return 0; }
DWORD APIENTRY Dummy_GetFileVersionInfoSizeA( LPSTR, LPDWORD ) { return 0; }

//////////////////////////////////////////////////////////////////////
//Function pointers: IMM32
INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC )			= Dummy_ImmLockIMC;
BOOL (WINAPI * _ImmUnlockIMC)( HIMC )					= Dummy_ImmUnlockIMC;
LPVOID (WINAPI * _ImmLockIMCC)( HIMCC )					= Dummy_ImmLockIMCC;
BOOL (WINAPI * _ImmUnlockIMCC)( HIMCC )					= Dummy_ImmUnlockIMCC;
BOOL (WINAPI * _ImmDisableTextFrameService)( DWORD )	= Dummy_ImmDisableTextFrameService;
LONG (WINAPI * _ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD ) = Dummy_ImmGetCompositionStringW;
DWORD (WINAPI * _ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD ) = Dummy_ImmGetCandidateListW;
HIMC (WINAPI * _ImmGetContext)( HWND )					= Dummy_ImmGetContext;
BOOL (WINAPI * _ImmReleaseContext)( HWND, HIMC )		= Dummy_ImmReleaseContext;
HIMC (WINAPI * _ImmAssociateContext)( HWND, HIMC )		= Dummy_ImmAssociateContext;
BOOL (WINAPI * _ImmGetOpenStatus)( HIMC )				= Dummy_ImmGetOpenStatus;
BOOL (WINAPI * _ImmSetOpenStatus)( HIMC, BOOL )			= Dummy_ImmSetOpenStatus;
BOOL (WINAPI * _ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD ) = Dummy_ImmGetConversionStatus;
HWND (WINAPI * _ImmGetDefaultIMEWnd)( HWND )			= Dummy_ImmGetDefaultIMEWnd;
UINT (WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT ) = Dummy_ImmGetIMEFileNameA;
UINT (WINAPI * _ImmGetVirtualKey)( HWND )				= Dummy_ImmGetVirtualKey;
BOOL (WINAPI * _ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD ) = Dummy_ImmNotifyIME;
BOOL (WINAPI * _ImmSetConversionStatus)( HIMC, DWORD, DWORD ) = Dummy_ImmSetConversionStatus;
BOOL (WINAPI * _ImmSimulateHotKey)( HWND, DWORD )		= Dummy_ImmSimulateHotKey;
BOOL (WINAPI * _ImmIsIME)( HKL )						= Dummy_ImmIsIME;
BOOL (WINAPI * _ImmSetCompositionWindow)(HIMC, LPCOMPOSITIONFORM) = Dummy_ImmSetCompositionWindow;

// Function pointers: Traditional Chinese IME
UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) = Dummy_GetReadingString;
BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL )		= Dummy_ShowReadingWindow;

// Function pointers: Verion library imports
BOOL (APIENTRY * _VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT )	= Dummy_VerQueryValueA;
BOOL (APIENTRY * _GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID )		= Dummy_GetFileVersionInfoA;
DWORD (APIENTRY * _GetFileVersionInfoSizeA)( LPSTR, LPDWORD )				= Dummy_GetFileVersionInfoSizeA;

////////////////////////////////////////////////////////////////////

HWND      s_hWnd;			// Main Wiondow Handle
HINSTANCE s_hDllImm32;      // IMM32 DLL handle
HINSTANCE s_hDllVer;        // Version DLL handle
HKL       s_hklCurrent;     // Current keyboard layout of the process
bool      s_bVerticalCand;  // Indicates that the candidates are listed vertically
HIMC      s_hImcDef;           // Default input context
bool      s_bChineseIME;
IMESTATE  s_ImeState = IMEUI_STATE_OFF;
HINSTANCE s_hDllIme;           // Instance handle of the current IME module
std::multiset< WCHAR > s_hIMEHasSend;

//Active IME EditBox
std::list< CEGUI::Window* > s_activeExitBox;

inline HWND GetHWND() { return s_hWnd; }
inline WORD GetLanguage() { return LOWORD( s_hklCurrent ); }
inline WORD GetPrimaryLanguage() { return PRIMARYLANGID( LOWORD( s_hklCurrent ) ); }
inline WORD GetSubLanguage() { return SUBLANGID( LOWORD( s_hklCurrent ) ); }
CEGUI::EditboxInterface* GetActiveIMEEditBox_Interface(void);

////////////////////////////////////////////////////////////////
//
void CheckToggleState(void);
void SetupImeApi(void);
void CheckInputLocale(void);

//[4/21/2006] prepare for ime wnd pos adjust.
void SetImeWindowPos(void);

/*******************************************
			Initialize/Uninitialize
********************************************/
void InitializeImm(unsigned int hWnd)
{
	#define GETPROCADDRESS( Module, APIName, Temp ) \
		Temp = GetProcAddress( Module, #APIName ); \
		if( Temp ) \
			*(FARPROC*)&_##APIName = Temp

    FARPROC Temp;

	s_hWnd = (HWND)(UINT_PTR)hWnd;

    CHAR szPath[MAX_PATH+1];
    if( !::GetSystemDirectoryA( szPath, MAX_PATH+1 ) )
        return;
    lstrcatA( szPath, IMM32_DLLNAME );
    s_hDllImm32 = LoadLibraryA( szPath );
    if( s_hDllImm32 )
    {
        GETPROCADDRESS( s_hDllImm32, ImmLockIMC, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmUnlockIMC, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmLockIMCC, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmUnlockIMCC, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmDisableTextFrameService, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetCompositionStringW, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetCandidateListW, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetContext, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmReleaseContext, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmAssociateContext, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetOpenStatus, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmSetOpenStatus, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetConversionStatus, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetDefaultIMEWnd, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetIMEFileNameA, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmGetVirtualKey, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmNotifyIME, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmSetConversionStatus, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmSimulateHotKey, Temp );
        GETPROCADDRESS( s_hDllImm32, ImmIsIME, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmSetCompositionWindow, Temp);
    }

    if( !::GetSystemDirectoryA( szPath, MAX_PATH+1 ) )
        return;
    lstrcatA( szPath, VER_DLLNAME );
    s_hDllVer = LoadLibraryA( szPath );
    if( s_hDllVer )
    {
        GETPROCADDRESS( s_hDllVer, VerQueryValueA, Temp );
        GETPROCADDRESS( s_hDllVer, GetFileVersionInfoA, Temp );
        GETPROCADDRESS( s_hDllVer, GetFileVersionInfoSizeA, Temp );
    }

	// initialize its default input context associated with the app window.
    // Save the default input context
    s_hImcDef = _ImmGetContext( GetHWND() );
    _ImmReleaseContext( GetHWND(), s_hImcDef );
}

//--------------------------------------------------------------------------------------
void UninitializeImm(void)
{
	#define PLACEHOLDERPROC( APIName ) \
		_##APIName = Dummy_##APIName

    if( s_hDllImm32 )
    {
        PLACEHOLDERPROC( ImmLockIMC );
        PLACEHOLDERPROC( ImmUnlockIMC );
        PLACEHOLDERPROC( ImmLockIMCC );
        PLACEHOLDERPROC( ImmUnlockIMCC );
        PLACEHOLDERPROC( ImmDisableTextFrameService );
        PLACEHOLDERPROC( ImmGetCompositionStringW );
        PLACEHOLDERPROC( ImmGetCandidateListW );
        PLACEHOLDERPROC( ImmGetContext );
        PLACEHOLDERPROC( ImmReleaseContext );
        PLACEHOLDERPROC( ImmAssociateContext );
        PLACEHOLDERPROC( ImmGetOpenStatus );
        PLACEHOLDERPROC( ImmSetOpenStatus );
        PLACEHOLDERPROC( ImmGetConversionStatus );
        PLACEHOLDERPROC( ImmGetDefaultIMEWnd );
        PLACEHOLDERPROC( ImmGetIMEFileNameA );
        PLACEHOLDERPROC( ImmGetVirtualKey );
        PLACEHOLDERPROC( ImmNotifyIME );
        PLACEHOLDERPROC( ImmSetConversionStatus );
        PLACEHOLDERPROC( ImmSimulateHotKey );
        PLACEHOLDERPROC( ImmIsIME );
		PLACEHOLDERPROC( ImmSetCompositionWindow);

        FreeLibrary( s_hDllImm32 );
        s_hDllImm32 = NULL;
    }
    if( s_hDllIme )
    {
        PLACEHOLDERPROC( GetReadingString );
        PLACEHOLDERPROC( ShowReadingWindow );

        FreeLibrary( s_hDllIme );
        s_hDllIme = NULL;
    }
    if( s_hDllVer )
    {
        PLACEHOLDERPROC( VerQueryValueA );
        PLACEHOLDERPROC( GetFileVersionInfoA );
        PLACEHOLDERPROC( GetFileVersionInfoSizeA );

        FreeLibrary( s_hDllVer );
        s_hDllVer = NULL;
    }
}


bool ImeMsgProc(unsigned int uMsg, unsigned int _wParam, unsigned int _lParam)
{
	WPARAM wParam = _wParam;
	LPARAM lParam = _lParam;
    HIMC hImc;

    switch( uMsg )
    {
		case WM_CHAR:
            switch( (WCHAR)wParam )
            {
			//Tab
			case VK_TAB:
				CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Tab);
				break;

			// Backspace
			case VK_BACK:
				CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Backspace);
				break;

			case VK_CANCEL: // Ctrl-C Copy
			case 24:        // Ctrl-X Cut
				{
					CEGUI::EditboxInterface* pEdit = GetActiveIMEEditBox_Interface();
					if(pEdit)
					{
						if(0 != pEdit->getSelectionLength() && !(pEdit->isTextMasked()))
						{
							//utf32 selection string
							CEGUI::String szSel = (GetActiveIMEEditBox())->getText().substr(pEdit->getSelectionStartIndex(), pEdit->getSelectionLength());

							size_t length = szSel.length();
							if(length > 0)
							{
								size_t pos = 0;
								wchar_t* pWtxt = new wchar_t[length+1];
								pWtxt[length] = 0;
								//utf32 -> utf16
								while(length--)
								{
									pWtxt[pos] = szSel.at(pos) & 0xFFFF;
									pos++;
								}

								//paste to clipboard
								if(::OpenClipboard(::GetTopWindow(NULL)))
								{
									::EmptyClipboard();
									
									int size = ::WideCharToMultiByte(CP_ACP,0,pWtxt,wcslen(pWtxt),NULL,0,NULL,FALSE);
									//alloc mem
									HGLOBAL hglb = ::GlobalAlloc(GMEM_MOVEABLE,(size_t)size+1);
									char* ptxt = (char*)::GlobalLock(hglb);
									ptxt[size] = 0;
									//utf16 -> ansi char
									::WideCharToMultiByte(CP_ACP,0,pWtxt,wcslen(pWtxt),ptxt,size,NULL,FALSE);
									::GlobalUnlock(hglb);
									//copy to clipboard
									::SetClipboardData(CF_TEXT, hglb);
									::CloseClipboard();
									//free mem
									::GlobalFree(hglb);
								}

								delete [] pWtxt;

								if((WCHAR)wParam == 24) /*Ctrl-X need clear selection*/
								{
									CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Delete);
								}
							}//end of if(length > 0)
						}
					}// end of if(pWin)
				}
				break;

			// Ctrl-V Paste
			case 22:
				{
					CEGUI::EditboxInterface* pWin = GetActiveIMEEditBox_Interface();
					if(pWin)
					{
						if(::IsClipboardFormatAvailable(CF_TEXT) && ::OpenClipboard(::GetTopWindow(NULL)))
						{
							HGLOBAL hglb = ::GetClipboardData(CF_TEXT);
							if(NULL != hglb)
							{
								char* ptxt = (char*)::GlobalLock(hglb);

								int size = ::MultiByteToWideChar(CP_ACP,0,ptxt,(int)strlen(ptxt),NULL,0);
								wchar_t* pWtxt = new wchar_t[size+1];
								pWtxt[size] = 0;

								::MultiByteToWideChar(CP_ACP,0,ptxt,(int)strlen(ptxt),pWtxt,size);

								::GlobalUnlock(hglb);
								::CloseClipboard();

								int pos = 0;
								while(size--)
								{
									CEGUI::System::getSingleton().injectChar((CEGUI::utf32)pWtxt[pos]);
									pos++;
								}
							}// end of if(NULL != hglb)
						}
					}// end of if(pWin)
				}
				break;

				// Ctrl-A Select All
			case 1:
				{
					CEGUI::EditboxInterface* pEdit = GetActiveIMEEditBox_Interface();
					if(pEdit)
					{
						pEdit->setSelection(0, pEdit->getMaxTextLength());
						pEdit->setCaratIndex(pEdit->getMaxTextLength());
					}
				}
				break;

			case VK_RETURN:
				CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Return);
				break;

				// Junk characters we don't want in the string
			case 26:  // Ctrl Z
			case 2:   // Ctrl B
			case 14:  // Ctrl N
			case 19:  // Ctrl S
			case 4:   // Ctrl D
			case 6:   // Ctrl F
			case 7:   // Ctrl G
			case 10:  // Ctrl J
			case 11:  // Ctrl K
			case 12:  // Ctrl L
			case 17:  // Ctrl Q
			case 23:  // Ctrl W
			case 5:   // Ctrl E
			case 18:  // Ctrl R
			case 20:  // Ctrl T
			case 25:  // Ctrl Y
			case 21:  // Ctrl U
//			case 9:   // Ctrl I
			case 15:  // Ctrl O
			case 16:  // Ctrl P
			case 27:  // Ctrl [
			case 29:  // Ctrl ]
			case 28:  // Ctrl \ 
				break;
			default:
				{
					WCHAR szCharW;
					CHAR szChar = (CHAR)LOWORD(wParam);
					if(szChar >= 32 && szChar <= 128 )
					{
						::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)&szChar, 1, &szCharW, 1);

						//AxTrace(0, 1, "Char:%02X", szCharW);
						CEGUI::System::getSingleton().injectChar((CEGUI::utf32)szCharW);
					}
				}
			}
			return true;

        case WM_IME_COMPOSITION:
            OutputDebugStringW( L"WM_IME_COMPOSITION\n" );
            {
                LONG lRet;  // Returned count in CHARACTERS
                WCHAR wszCompStr[MAX_COMPSTRING_SIZE];
				bool bRet = false;

                //*trapped = true;
                if( NULL == ( hImc = _ImmGetContext( GetHWND() ) ) )
                {
                    break;
                }

                if ( lParam & GCS_RESULTSTR )
                {
                    OutputDebugStringW( L"  GCS_RESULTSTR\n" );
                    lRet = _ImmGetCompositionStringW( hImc, GCS_RESULTSTR, wszCompStr, sizeof( wszCompStr ) );
                    if( lRet > 0 )
                    {
                        lRet /= sizeof(WCHAR);
                        wszCompStr[lRet] = 0;  // Force terminate
						for(int i = 0; i <lRet; ++i )
						{
							//AxTrace(0, 0, "Result:%02X", (WCHAR)wszCompStr[i]);
							CEGUI::System::getSingleton().injectChar((WCHAR)wszCompStr[i]);
						}
                    }
					bRet = true;
                }

                _ImmReleaseContext( GetHWND(), hImc );

				// Adjust Ime Window Pos. [4/21/2006]
				//SetImeWindowPos();
				return bRet;
			}
        case WM_INPUTLANGCHANGE:
            OutputDebugStringW( L"WM_INPUTLANGCHANGE\n" );
            {
                UINT uLang = GetPrimaryLanguage();
                CheckToggleState();

                // IME changed.  Setup the new IME.
                SetupImeApi();
            }
            break;

		case WM_IME_NOTIFY:
			//OutputDebugStringW( L"WM_IME_NOTIFY\n" );
			{
				if(wParam == IMN_SETOPENSTATUS || wParam == IMN_SETCONVERSIONMODE)
				{
					CheckToggleState();
				}

			}
			break;
		case WM_KEYDOWN:
			{
				switch ( wParam )
				{
				case VK_SHIFT:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::LeftShift);
					break;

				case VK_CONTROL:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::LeftControl);
					break;

				case VK_DELETE:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Delete);
					break;

				case VK_LEFT:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowLeft);
					break;

				case VK_RIGHT:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowRight);
					break;

				case VK_UP:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowUp);
					break;

				case VK_DOWN:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::ArrowDown);
					break;

				case VK_HOME:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::Home);
					break;

				case VK_END:
					CEGUI::System::getSingleton().injectKeyDown(CEGUI::Key::End);
					break;
				}
			}
			break;

		case WM_KEYUP:
			{
				switch ( wParam )
				{
				case VK_SHIFT:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::LeftShift);
					break;

				case VK_CONTROL:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::LeftControl);
					break;

				case VK_DELETE:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::Delete);
					break;

				case VK_LEFT:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::ArrowLeft);
					break;

				case VK_RIGHT:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::ArrowRight);
					break;

				case VK_UP:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::ArrowUp);
					break;

				case VK_DOWN:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::ArrowDown);
					break;

				case VK_HOME:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::Home);
					break;

				case VK_END:
					CEGUI::System::getSingleton().injectKeyUp(CEGUI::Key::End);
					break;
				}
			}
			break;

		default:
			break;
	}

	return false;
}

void CheckToggleState(void)
{
    CheckInputLocale();
    bool bIme = _ImmIsIME( s_hklCurrent ) != 0;
    s_bChineseIME = ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

    HIMC hImc;
    if( NULL != ( hImc = _ImmGetContext( GetHWND() ) ) )
    {
        if( s_bChineseIME )
        {
            DWORD dwConvMode, dwSentMode;
            _ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
            s_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;
        }
        else
        {
            s_ImeState = ( bIme && _ImmGetOpenStatus( hImc ) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
        }
        _ImmReleaseContext( GetHWND(), hImc );
    }
    else
        s_ImeState = IMEUI_STATE_OFF;
}

//--------------------------------------------------------------------------------------
// Sets up IME-specific APIs for the IME edit controls.  This is called every time
// the input locale changes.
void SetupImeApi(void)
{
    char szImeFile[MAX_PATH + 1];

    _GetReadingString = NULL;
    _ShowReadingWindow = NULL;
    if( _ImmGetIMEFileNameA( s_hklCurrent, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) == 0 )
        return;

    if( s_hDllIme ) FreeLibrary( s_hDllIme );
    s_hDllIme = LoadLibraryA( szImeFile );
    if ( !s_hDllIme )
        return;
    _GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
        ( GetProcAddress( s_hDllIme, "GetReadingString" ) );
    _ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
        ( GetProcAddress( s_hDllIme, "ShowReadingWindow" ) );
}

void CheckInputLocale(void)
{
    s_hklCurrent = GetKeyboardLayout( 0 );
}

void SetImeWindowPos(void)
{
	//Set Ime Wnd Position.
	CEGUI::EditboxInterface* pEdit = GetActiveIMEEditBox_Interface();
	if(NULL == pEdit) return;

	CEGUI::Window* pWnd = GetActiveIMEEditBox();
	if(NULL == pWnd) return;

	CEGUI::Rect ceguiRect = pWnd->getPixelRect();
	POINT pt;
	pt.x = (long)ceguiRect.d_left;
	pt.y = (long)ceguiRect.d_bottom;

	HIMC hImc = _ImmGetContext(GetHWND());
	if(NULL == hImc) return;

	COMPOSITIONFORM imeForm;
	imeForm.dwStyle = CFS_POINT;
	imeForm.ptCurrentPos = pt;

	_ImmSetCompositionWindow(hImc,&imeForm);
	_ImmReleaseContext(GetHWND(),hImc);
}
////////////////////////////////////////////////////////////////////////
/*******************************************
				CEGUI Input
********************************************/
void SetActiveIMEEditBox(CEGUI::Window* imeEditBox, bool set)
{
	if(!imeEditBox) return;

	if(set)
	{
		if(GetActiveIMEEditBox() != imeEditBox)
			s_activeExitBox.push_front(imeEditBox);
	}
	else
	{
		s_activeExitBox.remove(imeEditBox);
		//if(s_activeExitBox.empty())
		//{
		//	s_activeExitBox.push_front(imeEditBox);
		//}
	}
}

CEGUI::Window* GetActiveIMEEditBox(void)
{
	if(s_activeExitBox.empty()) return 0;

	return *(s_activeExitBox.begin());
}

CEGUI::EditboxInterface* GetActiveIMEEditBox_Interface(void)
{
	CEGUI::Window* pWin = GetActiveIMEEditBox();

	if(pWin && pWin->testClassName((CEGUI::utf8*)"MultiLineEditbox"))
	{
		return (CEGUI::EditboxInterface*)(CEGUI::MultiLineEditbox*)pWin;
	}
	else if(pWin && pWin->testClassName((CEGUI::utf8*)"Editbox"))
	{
		return (CEGUI::EditboxInterface*)(CEGUI::Editbox*)pWin;
	}
	return 0;
}

unsigned int GetCaretBlinkTime(void)
{
	return ::GetCaretBlinkTime();
}

IMESTATE GetState(void)
{
	return s_ImeState;
}

std::list< CEGUI::Window* >* GetActiveExitBox(void)
{
	return &s_activeExitBox;
}
}