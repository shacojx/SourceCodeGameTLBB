#ifndef __SOUNDEDITOPERATOR_H__
#define __SOUNDEDITOPERATOR_H__
#pragma once
#include "WXOperator.h"
#include <OgreVector3.h>

namespace WX 
{
	
	class SoundEditOperator: public Operator
	{
	public:
		SoundEditOperator(void);
		~SoundEditOperator(void);

		
	public:
			virtual const String& getGroupName(void) const;
			virtual String getDescription(void) const;
			virtual String getHelp(void) const;
			virtual void undo(void);
			virtual void redo(void);
	};
}

#endif //__SOUNDEDITOPERATOR_H__