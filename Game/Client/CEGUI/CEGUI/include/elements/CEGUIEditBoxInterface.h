/************************************************************************
	filename: 	CEGUIEditboxInterface.h
	created:	30/3/2006
	author:		Jean
	
	purpose:	Interface to base class for Editbox interface
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUIEditboxInterface_h_
#define _CEGUIEditboxInterface_h_

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Base class for an Editbox interface
*/
class CEGUIEXPORT EditboxInterface
{
public:
	/*!
	\brief
		Define the current selection for the Editbox

	\param start_pos
		Index of the starting point for the selection.  If this value is greater than the number of characters in the Editbox, the
		selection start will be set to the end of the text.

	\param end_pos
		Index of the ending point for the selection.  If this value is greater than the number of characters in the Editbox, the
		selection end will be set to the end of the text.

	\return
		Nothing.
	*/
	virtual void	setSelection(size_t start_pos, size_t end_pos) = 0;
	
	/*!
	\brief
		return the length of the current selection (in code points / characters).

	\return
		Number of code points (or characters) contained within the currently defined selection.
	*/
	virtual size_t	getSelectionLength(void) const = 0;

	/*!
	\brief
		return the current selection start point.

	\return
		Index of the selection start point relative to the start of the text.  If no selection is defined this function returns
		the position of the carat.
	*/
	virtual size_t	getSelectionStartIndex(void) const = 0;


	/*!
	\brief
		return the current selection end point.

	\return
		Index of the selection end point relative to the start of the text.  If no selection is defined this function returns
		the position of the carat.
	*/
	virtual size_t	getSelectionEndIndex(void) const = 0;

	/*!
	\brief
		return true if the text for the Editbox will be rendered masked.

	\return
		true if the Editbox text will be rendered masked using the currently set mask code point, false if the Editbox
		text will be rendered as plain text.
	*/
	virtual bool	isTextMasked(void) const = 0;
	/*!
	\brief
		return the maximum text length set for this Editbox.

	\return
		The maximum number of code points (characters) that can be entered into this Editbox.

	\note
		Depending on the validation string set, the actual length of text that can be entered may be less than the value
		returned here (it will never be more).
	*/
	virtual size_t	getMaxTextLength(void) const = 0;

	/*!
	\brief
		Set the current position of the carat.

	\param carat_pos
		New index for the insert carat relative to the start of the text.  If the value specified is greater than the
		number of characters in the Editbox, the carat is positioned at the end of the text.

	\return
		Nothing.
	*/
	virtual void	setCaratIndex(size_t carat_pos) = 0;
};


}

#endif
