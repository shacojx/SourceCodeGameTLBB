/********************************************************************
filename:   FairyEffectManager.h

purpose:    effect manager manage all effects, skills and path files.
*********************************************************************/

#ifndef __EffectManager_H__
#define __EffectManager_H__

// ogre header
#include <OgreSingleton.h>
#include <OgreScriptLoader.h>
#include <OgreStringVector.h>
#include <OgreDataStream.h>
#include "OgreIteratorWrappers.h"

// fairy header
#include "Core/WXPrerequisites.h"

// stl header
#include <map>

// 前向声明
namespace Ogre	{
	class SceneNode;
}

namespace WX	{
	class Effect;
	class EffectElementFactory;
	class EffectElement;
	class System;
	class EffectBasic;
	class Skill;
	class AnimationEffectInfo;
	class AnimationRibbon;
	class AnimationSound;
    class AnimationSceneLightInfo;
    class MainSceneLight;
}
namespace WX	{	

    /// 特效级别，默认为HIGH
    enum EffectLevel
    {
        EL_LOW,
        EL_MID,
        EL_HIGH,
    };

	class EffectManager : public Ogre::Singleton<EffectManager>, Ogre::ScriptLoader
	{
	public:

		typedef std::map<String, Effect *> EffectTemplateMap;
		//typedef std::map<String, Effect *> EffectMap;
		typedef std::list<Effect *> ActiveEffectList;

		typedef std::list<Effect *> FreeEffectList;
		typedef std::map<String, FreeEffectList *> FreeEffectMap;

		typedef std::map<String, EffectElementFactory *> ElementFactoryMap;

		typedef std::map<String, String> EffectTemplateScriptFileMap;

		typedef std::vector<String> LoadedScripts;

		typedef std::map<String, Skill *> SkillMap;

	public:

		EffectManager( System *system );
		~EffectManager();

		/// @copydoc ScriptLoader::getScriptPatterns
		const Ogre::StringVector& getScriptPatterns(void) const;
        /// @copydoc ScriptLoader::parseScript
		void parseScript(Ogre::DataStreamPtr& stream, const String& groupName);
        /// @copydoc ScriptLoader::getLoadingOrder
        Real getLoadingOrder(void) const;
		
        static EffectManager& getSingleton(void);       
        static EffectManager* getSingletonPtr(void);
		
		/// debug
		void dump(const String &fileName);

		/// 根据特效模板来创建一个新的特效
		Effect *createEffect( const String &templateName );

		Skill *getSkill( const String &skillName );

		/// 根据特效元素的类型来创建一个特效元素
		EffectElement * createElement(const String &type);

		/// 真正地删除一个特效
		ActiveEffectList::iterator destroyEffect( Effect *effect );

		/** 暂时地删除一个特效
			并不是真正地删除特效，而是把它从活动的特效链表中删除，放入
			空闲的特效的链表，以备后用
		@param removeParentNode 是否要把该effect的节点从他的父节点上卸下来，
			如果创建effect的节点调用的是createSceneNode，这个参数为false，
			如果创建effect的节点调用的是createSceneNode( Ogre::SceneNode *parentNode )，这个参数为true
        @param removeNow 是否马上就删除这个effect，如果为false，那么这个effect会在它完全播放完才
                         被删除

        @remarks 如果removeNow为false，不会马上删除effect，这时要保证effect的SceneNode不会先于effect删除，
                 所以如果需要removeNow为false，创建effect的scnenode最好使用createSceneNode()，这样，scenenode保存
                 在effect内部，可以保证scenenode的删除时机
		*/
		void removeEffect( Effect *effect, bool removeParentNode = true, bool removeNow = true );

		/**	创建一个控制器，用于每帧更新effect	*/
		void createController(void);

		/// 由控制器调用，每帧更新活动的特效
		void updateActiveEffects(Real time);

		size_t getNumEffectTemplates(void);

		typedef Ogre::MapIterator<EffectTemplateMap> EffectTemplateIterator;
		EffectTemplateIterator getEffectTemplateIterator(void);

		typedef Ogre::MapIterator<ElementFactoryMap> ElementFactoryIterator;
		ElementFactoryIterator getElementFactoryIterator(void);

		typedef Ogre::MapIterator<SkillMap> SkillIterator;
		SkillIterator getSkillIterator(void);

		const String & getFileNameByTemplateName( const String &templateName );

		void getTemplatesFromScriptFile( const String &fileName, std::vector<String> &templates );

		void getLoadedScripts( std::vector<String> &files )	{	files = mLoadedScripts;	}

		/// 根据模板名称来获取模板
		Effect * getTemplate(const String& name);

		Skill * createSkillTemplate(const String &name);

		/** 创建一个新的effect模板
		@param name 特效名称
		*/
		Effect * createEffectTemplate(const String &name);

		void addToEffectTemplateScriptFileMap( const String &templateName, const String &fileName );

		Skill * createSkill(const String &skillName);
		void removeSkill(Skill *skill);

		/** 设置每条空闲的effect链中最多可保存的空闲effect个数
		@remarks effect在removeEffect时会被回收，放入一个map中，map的key是effect的模板名称，
				 value是一条空闲的effect链，这个方法设置的就是这条链的最大个数，也就是说，在
				 空闲effect池中，同个模板最多可以保存mMaxNumFreeEffectPerList个空闲的effect
		*/
		void setMaxNumFreeEffectPerList(unsigned short num)
		{
			mMaxNumFreeEffectPerList = num;
		}
		unsigned short getMaxNumFreeEffectPerList(void)
		{
			return mMaxNumFreeEffectPerList;
		}

        MainSceneLight* getMainSceneLight(void)
        {
            return mMainSceneLight;
        }

        /// 清空特效池中的特效
        void clearAllFreeEffects(void);

        /// 设置特效级别
        void setEffectLevel(EffectLevel level);
        EffectLevel getEffectLevel(void)
        {
            return mEffectLevel;
        }

        void _destroyEffectTemplates(void);

        void _destroySkillTemplates(void);

	protected:

		/** 注册新的特效元素工厂
			每增加一种新类型的特效元素，都得在这个函数中添加相应的代码
		*/
		void registerElementFactory(void);

		void addElementFactory( EffectElementFactory *factory );

		void skipToNextCloseBrace(Ogre::DataStreamPtr& chunk);
        void skipToNextOpenBrace(Ogre::DataStreamPtr& chunk);

		/// 解析脚本中的新的特效元素
		void parseNewElement(const String &type, Ogre::DataStreamPtr &stream, Effect *effectParent);

		/// 解析特效元素的属性 
		void parseElementAttrib(const String& line, EffectElement *element);

		/// 解析.effect文件
		void parseEffectFile( Ogre::DataStreamPtr &stream );
		void parseEffectAttrib(const String& line, Effect *effect);

		/// 解析.skill文件
		void parseSkillFile( Ogre::DataStreamPtr &stream );
		void parseSkillAttrib(const String& line, Skill *skill);
        void parseAnimEffectInfo(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimEffectInfoAttrib(const String& line, AnimationEffectInfo *effectInfo);
		void parseAnimRibbon(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimRibbonAttrib(const String& line, AnimationRibbon *ribbon);
		void parseAnimSound(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimSoundAttrib(const String& line, AnimationSound *sound);
        void parseAnimSceneLightInfo(Ogre::DataStreamPtr &stream, Skill *skill);	
        void parseAnimSceneLightInfoAttrib(const String& line, AnimationSceneLightInfo *sceneLightInfo);

		/** 从空闲的特效链表中获取一个所指定模板的特效
			一般来说，在游戏运行时如果要删除一个特效（比如说一个挂接
			特效的动作完成了），都不会直接删除特效的，而是把它经过一些处理后
			（重新初始化）放入到一个空闲的链表中，下次如果要求创建同种类型的特效，
			就先从空闲链表中找，如果没有，再重新创建一个全新的
		*/
		Effect * getFromFreeMap( const String &templateName );

		/// 把一个特效加入到空闲链表中
		void addToFreeEffectMap( Effect *effect );	

		void addToLoadedScripts( const String &fileName )	{	mLoadedScripts.push_back(fileName);	}

        /// 输出错误信息
        void _logErrorInfo(const String& errorInfo, const String& lineContent, 
            const String& functionName);
		
	private:

		ElementFactoryMap mElementFactoryMap;
		EffectTemplateMap mEffectTemplateMap;
		FreeEffectMap mFreeEffectMap;
		SkillMap mSkillMap;

		EffectTemplateScriptFileMap mEffectTemplateScriptFileMap;

		LoadedScripts mLoadedScripts;

		ActiveEffectList mActiveEffectList;

		Ogre::StringVector mScriptPatterns;

		System *mSystem;

		Ogre::Controller<Real> *mController;

		unsigned short mMaxNumFreeEffectPerList;

        /// 每次进行getLine的时候就递增行号，作为错误的输出信息
        int mWrongLineNum;

        /// 当前解析的文件名称
        String mParsingFileName;

        /// 当前场景上的灯光变化信息
        MainSceneLight* mMainSceneLight;

        /// 特效级别
        EffectLevel mEffectLevel;
	};
}

#endif