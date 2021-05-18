#ifndef __FAIRYRESOURCESOLVER_H__
#define __FAIRYRESOURCESOLVER_H__

#include "WXPrerequisites.h"

#include <OgreSharedPtr.h>
#include <OgreStringVector.h>

namespace WX {

    typedef Ogre::StringVector StringVector;
    typedef Ogre::StringVectorPtr StringVectorPtr;
    typedef std::set<String> StringSet;
    typedef std::pair<String, String> ResourceLocation;
    typedef std::set<ResourceLocation> ResourceLocationSet;

    class UnsolvedResource
    {
    public:
        UnsolvedResource(void) {}
        virtual ~UnsolvedResource() {}

        virtual const String& getOrigin(void) const = 0;
        virtual const String& getDescription(void) const = 0;
        virtual const StringVector* getRecommendValues(void) const = 0;
        virtual const StringVector* getPossibleValues(void) const = 0;
        virtual bool setResolvedValue(const String& value) = 0;
    };

    typedef Ogre::SharedPtr<UnsolvedResource> UnsolvedResourcePtr;
    typedef std::vector<UnsolvedResourcePtr> UnsolvedResourceList;

    class ResourceSolver
    {
    public:
        ResourceSolver(void) {}
        virtual ~ResourceSolver() {}

        virtual const String& getName(void) const = 0;
        virtual const UnsolvedResourceList& getUnsolvedResources(void) const = 0;
        virtual bool commitPermanent(void) = 0;
    };

    typedef Ogre::SharedPtr<ResourceSolver> ResourceSolverPtr;
    typedef std::vector<ResourceSolverPtr> ResourceSolverList;

}

#endif 
