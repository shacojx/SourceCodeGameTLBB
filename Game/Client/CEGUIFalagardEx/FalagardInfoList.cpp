#include ".\falagardinfolist.h"
#include "ceguipropertyhelper.h"
#include "..\cegui\cegui\include\CEGUIWindowManager.h"
namespace CEGUI
{

	const utf8 FalagardInfoList::WidgetTypeName[] = "Falagard/InfoList";
	
	FalagardInfoListProperties::AddInfo		FalagardInfoList::d_AddInfoProperty;
	FalagardInfoListProperties::FadeSpeed	FalagardInfoList::d_FadeSpeedProperty;
	FalagardInfoListProperties::Life		FalagardInfoList::d_LifeProperty;
	FalagardInfoListProperties::NeedMove	FalagardInfoList::d_NeedMoveProperty;

	// 得意自动删除得列表框
	FalagardInfoItem::FalagardInfoItem(const String& text, uint item_id , void* item_data , bool disabled , bool auto_delete ):
		ListboxTextItem( text, item_id, item_data, disabled, auto_delete )
	{
		m_fCurAlpha = 1;
		m_Move_X  = 1;
		m_NeedMove = 0;
	}
	bool FalagardInfoItem::update( float elapsed )
	{
		m_nTimer -= int( elapsed * 1000 ); // 生存周期
		if( m_nTimer < 0 )
		{
				return false;
		}
		else if(m_NeedMove)
		{
			m_Move_X -= ((float)m_nFadeSpeed)/1000;
			char Temp[100] = {0};
			_snprintf(Temp, 100, "x:%f y:0", m_Move_X);
			CEGUI::WindowManager::getSingleton().getWindow("GongGaoBox_ListBox")->setProperty("Position", Temp);
		}
		
		return true;
	}
	void FalagardInfoItem::draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const
	{

		if (d_selected && d_selectBrush != 0)
		{
			cache.cacheImage(*d_selectBrush, targetRect, zBase, getModulateAlphaColourRect(d_selectCols, alpha), clipper);
		}

		const FontBase* font = getFont();

		if (font)
		{
			Rect finalPos(targetRect);
//			finalPos.d_top -= (FontBase::LineSpace) * 0.5f;
			cache.cacheText(getOwnerWindow(), d_itemText, font, Centred, finalPos, zBase, getModulateAlphaColourRect(d_textCols, alpha), clipper);
		}
	}

	FalagardInfoList::FalagardInfoList(const String& type, const String& name) :
		FalagardListbox(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardInfoList )
		   CEGUI_ADD_STATICPROPERTY( &d_AddInfoProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_FadeSpeedProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_LifeProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_NeedMoveProperty  );
		CEGUI_END_ADD_STATICPROPERTY

		m_nMaxInfoNumber = 3;
		m_NeedMove = 0;
	}

	FalagardInfoList::~FalagardInfoList(void)
	{
	}
	void FalagardInfoList::NormalizeItemNumber()
	{
		if( (int)getItemCount() < m_nMaxInfoNumber )
			return;
		for (size_t i = 0; i < getItemCount() - m_nMaxInfoNumber; i ++ ) 
			removeItem( d_listItems[ 0 ] );

	}

	void FalagardInfoList::add_Temp_Item(ListboxItem* item)
	{

		if (item != NULL)
		{
			d_Need_Add.push_back(item);
		}
	}
	void FalagardInfoList::updateSelf( float elapsed )
	{
		FalagardListbox::updateSelf( elapsed );
		if (m_NeedMove)
		{
			if (getItemCount() > 0)
			{
				if ( ((FalagardInfoItem*)d_listItems[0])->update( elapsed ) == false )
				{
					// clean up this item.
					removeItem( d_listItems[ 0 ] );	
				}
				setVisible(true);
			}else
			{
				setVisible(false);
			}
			if (d_listItems.size() <= 0 && d_Need_Add.size() > 0)
			{
				addItem(d_Need_Add.front());
				d_Need_Add.pop_front();
			}
		}else
		{
			for (size_t i = 0; i < getItemCount(); ++i ) 
			{
				// if item is supposed to be deleted by us
				if ( ((FalagardInfoItem*)d_listItems[i])->update( elapsed ) == false )
				{
					// clean up this item.
					removeItem( d_listItems[ i ] );
				}

			}
			int x=(int)getItemCount();
			if( x == 0)
				setVisible(false);
			else
				setVisible(true);
		}
		requestRedraw();
	}

	/*----------------------------------------------------------------------*/
	// FalagardWorldMapFactory
    Window* FalagardInfoListFactory::createWindow(const String& name)
    {
        return new FalagardInfoList(d_type, name);
    }

    void FalagardInfoListFactory::destroyWindow(Window* window)
    {
       delete window;
    }

	namespace FalagardInfoListProperties
	{
		String Life::get(const PropertyReceiver* receiver) const
		{
			return  PropertyHelper::intToString( static_cast< const FalagardInfoList*>(receiver)->getLife() );
		}

		void Life::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setLife( PropertyHelper::stringToInt( value ) );
		};

		String FadeSpeed::get(const PropertyReceiver* receiver) const
		{
			return  PropertyHelper::intToString( static_cast< const FalagardInfoList*>(receiver)->getFadeSpeed() );
		}

		void FadeSpeed::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setFadeSpeed( PropertyHelper::stringToInt( value ) );
		};

		//*----------------------------------------------------------------------
		String AddInfo::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void AddInfo::set(PropertyReceiver* receiver, const String& value)
		{
			FalagardInfoItem* pItem = new FalagardInfoItem(value);
			FalagardInfoList *pParent = static_cast<FalagardInfoList*>(receiver);
			if( pItem && pParent )
			{
				pItem->setMoveInfo( pParent->getMoveInfo());
				pItem->setFadeSpeed( pParent->getFadeSpeed());
				pItem->setTimer( pParent->getLife() );
				if (pParent->getMoveInfo())
				{
					pParent->add_Temp_Item( pItem );
				}else
				{
					pParent->addItem( pItem );
					pParent->NormalizeItemNumber();
				}
			}
		};

		//*----------------------------------------------------------------------
		String MaxInfo::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void MaxInfo::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setMaxInfo( PropertyHelper::stringToInt( value ) );
		};
		//*----------------------------------------------------------------------
		String NeedMove::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString( static_cast< const FalagardInfoList*>(receiver)->getMoveInfo() );
		}

		void NeedMove::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardInfoList*>(receiver)->setMoveInfo( PropertyHelper::stringToInt( value ) );
		};

	};
};