#ifndef __FAIRYRESOURCESOLVERHELPER_H__
#define __FAIRYRESOURCESOLVERHELPER_H__

#include "WXResourceSolver.h"

namespace WX {

    class UnsolvedResourceBase : public UnsolvedResource
    {
    public:
        String origin;
        String description;
        StringVectorPtr recommends;
        StringVectorPtr possibles;

    public:
        UnsolvedResourceBase(void);
        ~UnsolvedResourceBase();

        const String& getOrigin(void) const;
        const String& getDescription(void) const;
        const Ogre::StringVector* getRecommendValues(void) const;
        const Ogre::StringVector* getPossibleValues(void) const;
    };

    class UnsolvedResourceSimple : public UnsolvedResourceBase
    {
    public:
        std::vector<String*> references;

    public:
        UnsolvedResourceSimple(void);
        ~UnsolvedResourceSimple();

        bool setResolvedValue(const String& value);
    };

    class ResourceSolverBase : public ResourceSolver
    {
    public:
        UnsolvedResourceList unsolvedResources;

    public:
        ResourceSolverBase(void);
        ~ResourceSolverBase();

        virtual const UnsolvedResourceList& getUnsolvedResources(void) const;
    };

    extern StringVectorPtr findResourceFilenames(const String& group, const String& filename);
    extern StringVectorPtr findImageResources(const String& group);
    extern void moveMatchToFront(StringVector& names, const String& name);

    template <class Container>
    StringVectorPtr toStringVectorPtr(const Container& container)
    {
        if (container.empty())
            return StringVectorPtr();

        return StringVectorPtr(new StringVector(container.begin(), container.end()));
    }

}

#endif 
