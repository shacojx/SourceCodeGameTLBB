#include ".\falagardresizetext.h"
#include "ceguiwindowmanager.h"
#include "CEGUIFontBase.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
namespace CEGUI
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	const utf8  FalagardResizeText::WidgetTypeName[] = "Falagard/ResizeText";


	FalagardResizeTextProperties::AimWindow     FalagardResizeText::d_AimWindowPropety;
	FalagardResizeTextProperties::AimPosition   FalagardResizeText::d_AimPositionPropety;


	FalagardResizeText::FalagardResizeText( const String& type, const String& name ):
		FalagardStaticText(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardResizeText )
		   CEGUI_ADD_STATICPROPERTY( &d_AimWindowPropety  );
		   CEGUI_ADD_STATICPROPERTY( &d_AimPositionPropety  );
		CEGUI_END_ADD_STATICPROPERTY
	}

	FalagardResizeText::~FalagardResizeText(void)
	{
	}

	void FalagardResizeText::SetAimWindow( const String& strWindowName )
	{
		m_pAimWindow = WindowManager::getSingleton().getWindow( strWindowName );
	}
	void FalagardResizeText::SetAimPosition( const String& strPositionName )
	{
		char szAim[ 128 ];
		char szSelf[ 128 ];
		char szLock[ 128 ];
		sscanf( strPositionName.c_str(),  "Aim:%s Self:%s Lock:%s", szAim, szSelf, szLock  ); 
		if( strcmp( szAim, "LeftTop" ) == 0 )
		{
			m_nAimPositionType = LeftTop;
		}
		else if( strcmp( szAim,"RightTop" )== 0 )
		{
			m_nAimPositionType = RightTop;
		}
		else if( strcmp( szAim, "RightBottom" ) == 0 )
		{
			m_nAimPositionType = RightBottom;
		}
		else if( strcmp( szAim,"LeftBottom" ) == 0)
		{
			m_nAimPositionType = LeftBottom;
		}
		else
			m_nAimPositionType = InvalidPosition;


		if( strcmp( szSelf, "LeftTop" ) == 0 )
		{
			m_nSelfPositionType = LeftTop;
		}
		else if( strcmp( szSelf, "RightTop" ) == 0 )
		{
			m_nSelfPositionType = RightTop;
		}
		else if( strcmp( szSelf, "RightBottom" ) == 0 )
		{
			m_nSelfPositionType = RightBottom;
		}
		else if( strcmp( szSelf, "LeftBottom" ) == 0)
		{
			m_nSelfPositionType = LeftBottom;
		}
		else 
			m_nSelfPositionType = InvalidPosition;


		if( strcmp( szLock, "Width" ) )
			m_nLockType = LockWidth;
		else if( strcmp( szLock, "Height" ) )
			m_nLockType = LockHeight;
	}
	void FalagardResizeText::positionSelf()
	{
		Point pos;
		if( m_pAimWindow ) // 如果右绑定的目标窗口,就更具目标窗口的位置计算
		{
			Size sz = getAbsoluteSize();
			Size szAim = m_pAimWindow->getAbsoluteSize();
			pos = m_pAimWindow->getAbsolutePosition();
			switch( m_nAimPositionType )
			{
			case LeftTop:
				{
					
					switch( m_nSelfPositionType )
					{
					case LeftTop: // 这个不能绑
						break;
					case RightTop: // 跟自己平行,再目标窗口的左边, 上边对齐
						pos.d_x -= sz.d_width;
						break;
					case RightBottom: // 再目标窗口左上,下方和目标窗口上边对齐
						pos.d_x -= sz.d_width;
						pos.d_y -= sz.d_height;
						break;
					case LeftBottom: // 再目标窗口正上方,左边对齐
						pos.d_y -= sz.d_height;
						break;
					default:
						break;
					}
				}
				break;
			case RightTop:
				{
					switch( m_nSelfPositionType )
					{
					case LeftTop: // 在目标窗口左边,上边对齐
						pos.d_x += sz.d_width;
						break;
					case RightTop: // 这个不能绑
						break;
					case RightBottom:// 在目标正上方, 右边对齐
						pos.d_y -= sz.d_height;
						pos.d_x += szAim.d_width - sz.d_width;
						break;
					case LeftBottom: // 目标右上方,左边和目标右边对齐
						pos.d_x += szAim.d_width;
						pos.d_y -= sz.d_height;
						break;
					default:
						break;
					}
				}
				break;
			case RightBottom:
				{
					Size sz = m_pAimWindow->getAbsoluteSize();
					switch( m_nSelfPositionType )
					{
					case LeftTop: //  目标窗口的右下方,左上角和目标右下角重合
						pos.d_x += sz.d_width;
						pos.d_y += sz.d_height;
						break;
					case RightTop:// 目标正下方,右边对齐
						pos.d_x += szAim.d_width - sz.d_width;
						pos.d_y += szAim.d_height;
						break;
					case RightBottom: // 不能绑
						break;
					case LeftBottom: // 正右方,坐下教对齐
						pos.d_x += szAim.d_width;
						pos.d_y += szAim.d_height - sz.d_height;
						break;
					default:
						break;
					}
				}
				break;
			case LeftBottom:
				{
					switch( m_nSelfPositionType )
					{
					case LeftTop: // 正下方,左边对齐
						pos.d_y += szAim.d_height; 
						break;
					case RightTop: // 目标窗口左下对齐
						pos.d_x -= sz.d_width;
						pos.d_y += szAim.d_height;
						break;
					case RightBottom: // 正左方,下边对齐
						pos.d_x -= sz.d_width;
						pos.d_y += szAim.d_height - sz.d_height;
						break;
					case LeftBottom: // 不能帮
						break;
					default:
						break;
					}
				}
				break;
			}
			
		} // end m_pAimWindow != NULL;
		else // 没有目标窗口就直接绑到父窗口上
		{
			Size sz = getAbsoluteSize();
			Size szParent = getParentSize();
			pos = getParent()->getAbsolutePosition();
			switch( m_nSelfPositionType )
			{
				case LeftTop: // 左上角
					break;
				case RightTop: // 
					pos.d_x += szParent.d_width - sz.d_width;
					break;
				case RightBottom: // 正左方,下边对齐
					pos.d_x += szParent.d_width - sz.d_width;
					pos.d_y += szParent.d_height - sz.d_height; 
					break;
				case LeftBottom: // 不能帮
					pos.d_y += szParent.d_height - sz.d_height; 
					break;
				default:
					break;
			}
		}
		setPosition( Absolute, pos );
	}
	void FalagardResizeText::onTextChanged( WindowEventArgs& e )
	{
		//setSize( getTextSize() );	
		//positionSelf();
	}
	// 如果尺寸改变了,就重新给自己更新尺寸
	void FalagardResizeText::onSized( WindowEventArgs& e )
	{
		//setSize( getTextSize() );	
		//positionSelf();
	}
	void	FalagardResizeText::ResizeWindow()
	{
		setHeight( Absolute, getTextHeight() );
		d_frame.setSize(getAbsoluteSize());
		requestRedraw();
		//positionSelf();
	}
	float FalagardResizeText::getTextHeight() const
	{
		const FontBase* fnt = getFont();
		Size sz;
        if (fnt)
        {
            Rect area = getAbsoluteRect();
			float height = fnt->getFormattedSize(d_text, area, CEGUI::WordWrapCentred).d_height;
			return height;
		}
		return 0;

	}
	Size FalagardResizeText::getTextSize() const // 得到字符的尺寸
	{
		const FontBase* fnt = getFont();
		Size sz;
		//const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

  //      Rect textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*this));
  //      Rect wndArea(getAbsoluteRect());
        if (fnt)
        {
            Rect area = getAbsoluteRect();
			 // get required size of the tool tip according to the text extents.
            // TODO: Add a proprty to allow specification of text formatting.
			float height = fnt->getFormattedSize(d_text, area, CEGUI::WordWrapCentred).d_height;
			//float width = fnt->getFormattedTextExtent(d_text, area, CEGUI::WordWrapCentred);

			sz =  Size(area.getWidth(), height);
        }
        else
        {
            sz = Size(0,0);
        }

		

        // get WidgetLookFeel for the assigned look.


      /*  sz.d_width  += wndArea.getWidth() - textArea.getWidth();
        sz.d_height += wndArea.getHeight() - textArea.getHeight();*/
	
		return sz;
	}
	void FalagardResizeText::populateRenderCache()
	{

		StaticText::populateRenderCache();
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	Window* FalagardResizeTextWindowFactory::createWindow(const String& name)
    {
        return new FalagardResizeText(d_type, name);
    }

    void FalagardResizeTextWindowFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	namespace FalagardResizeTextProperties 
	{
		////////////////////////////////////////////////////////////////////////		
		// 瞄准的窗口名字,必须是同级的
		String AimWindow::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardSuperToolTip*>(receiver)->isEmpty());
		}

		void AimWindow::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardResizeText*>(receiver)->SetAimWindow( value );
		}
		////////////////////////////////////////////////////////////////////////		
		// 瞄准的窗口的角,必须是同级的
		String AimPosition::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardSuperToolTip*>(receiver)->isEmpty());
		}

		void AimPosition::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardResizeText*>(receiver)->SetAimPosition( value );
		}
	}

};