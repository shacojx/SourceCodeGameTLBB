#include "falagardimagelistitem.h"
#include "FalImageListProperties.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>

namespace CEGUI
{
	namespace FalagardImageListBoxProperties
	{
	//////////////////////////////////////////////////////////////////////////////////////
		String ImageNormal::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardImageListBox*>(receiver)->setNormalImage());
		}

		void ImageNormal::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBox*>(receiver)->setNormalImage(PropertyHelper::stringToImage(value));
		}
		//////////////////////////////////////////////////////////////////////////////////////
		String ImageHorver::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardImageListBox*>(receiver)->setHorverImage());
		}

		void ImageHorver::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBox*>(receiver)->setHorverImage(PropertyHelper::stringToImage(value));
		}
		//////////////////////////////////////////////////////////////////////////////////////
		String ImageChecked::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardImageListBox*>(receiver)->isEmpty());
		}

		void ImageChecked::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBox*>(receiver)->setCheckedImage(PropertyHelper::stringToImage(value));
		}
		//////////////////////////////////////////////////////////////////////////////////////

		String ImageDisable::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardImageListBox*>(receiver)->isEmpty());
		}

		void ImageDisable::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardImageListBox*>(receiver)->setDisabledImage(PropertyHelper::stringToImage(value));
		}

	////////////////////////////////////////////////////////////////////////////////////////
		String AddItem::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void AddItem::set(PropertyReceiver* receiver, const String& value)
		{
			char	text[128];
			int		nID = 0;
			sscanf(value.c_str(), "id=%d text=%s", &nID, text );
			FalagardImageListBoxItem* pItem = new FalagardImageListBoxItem( text, nID );
			FalagardImageListBox *pParent = static_cast<FalagardImageListBox*>(receiver);
			if( pItem && pParent )
			{
				pItem->SetNormalImage( (Image*)pParent->getNormalImage() );
				pItem->SetHorverImage( (Image*)pParent->getHorverImage() );
				pItem->SetCheckedImage( (Image*)pParent->getCheckedImage() );
				pItem->SetDisableImage( (Image*)pParent->getDisabledImage() );
				pParent->addItem( pItem );
			}
		}

	}
}
