#include "falagardimagelistex.h"
#include "FalImageListExProperties.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>

namespace CEGUI
{
	namespace FalagardImageListBoxExProperties
	{
		//////////////////////////////////////////////////////////////////////////////////////
		String NormalImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardImageListBox*>(receiver)->setNormalImage());
		}

		void NormalImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBoxEx*>(receiver)->SetNormalImage(PropertyHelper::stringToImage(value));
		}
	//////////////////////////////////////////////////////////////////////////////////////
		String SetStateImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardImageListBox*>(receiver)->setNormalImage());
		}

		void SetStateImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBoxEx*>(receiver)->setStateImage(PropertyHelper::stringToImage(value));
		}
		//////////////////////////////////////////////////////////////////////////////////////
		String AddItem::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		void AddItem::set(PropertyReceiver* receiver, const String& value)
		{
			char	text[128];
			int		nID = 0;
			int		nState = 0;
			
			sscanf(value.c_str(), "id=%d state=%d text=%s ", &nID, &nState, text );
			std::string  str = text;
			FalagardImageListBoxItemEx* pItem = new FalagardImageListBoxItemEx( (String)(utf8*)(str.c_str() ), nID );
			FalagardImageListBoxEx *pParent = static_cast<FalagardImageListBoxEx*>(receiver);
			if( pItem && pParent )
			{
				pItem->SetHorverImage( pParent->getHorverImage() );
				pItem->setSelectionBrushImage( pParent->getSelectImage() );
				pItem->SetNormalImage( pParent->getNormalImage() );
				pItem->SetIconImage( (Image*)pParent->getStateImage( nState ) );
				pItem->SetState( nState );
				pParent->addItem( pItem );
				//pParent->setHeight( pItem->getPixelSize().d_height * pParent->getItemCount() );
			}
		}
	
		//////////////////////////////////////////////////////////////////////////////////////
		
		String SelectImage::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		
		void SelectImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBoxEx*>(receiver)->setSelectImage( PropertyHelper::stringToImage( value ) );
		}

		//////////////////////////////////////////////////////////////////////////////////////
		
		String HorverImage::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		
		void HorverImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBoxEx*>(receiver)->setHorverImage( PropertyHelper::stringToImage( value ) );
		}

	}
}
