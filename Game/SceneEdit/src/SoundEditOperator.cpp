#include ".\Soundeditoperator.h"
#include ".\Soundeditaction.h"

namespace WX 
{
	SoundEditOperator::SoundEditOperator(void)
	{
	}

	SoundEditOperator::~SoundEditOperator(void)
	{
	}

	const String& SoundEditOperator::getGroupName(void) const
	{
		static String strGroupName = "SoundEdit";
		return strGroupName;
	}

	String SoundEditOperator::getDescription(void) const
	{
		return String("SoundEdit");
	}

	String SoundEditOperator::getHelp(void) const
	{
		return String("SoundEdit");
	}

	void SoundEditOperator::undo(void)
	{

	}

	void SoundEditOperator::redo(void)
	{

	}
}
