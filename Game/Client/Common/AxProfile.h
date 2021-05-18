#ifndef _TENGDA_PROFILE_INCLUDE_
#define _TENGDA_PROFILE_INCLUDE_

#include "TDBasicType.h"

namespace AxProfile
{

void AxProfile_SamplingBegin(void);
void AxProfile_SamplingEnd(void);

void AxProfile_PushNode(const char* szName, const char* szParent=0);
void AxProfile_PopNode(const char* szName);

void AxProfile_Report(void);
}


struct AUTO_SAMPLING
{
	AUTO_SAMPLING()  { /*AxProfile::AxProfile_SamplingBegin();*/ }
	~AUTO_SAMPLING() { /*AxProfile::AxProfile_SamplingEnd();*/ }
};


#endif