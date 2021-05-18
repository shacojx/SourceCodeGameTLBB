#pragma once
#include "WXOperator.h"
#include <OgreVector3.h>
#include "GrowPointEditAction.h"
namespace WX 
{

	class CGrowPointOperator: public Operator
	{
	public:
		CGrowPointOperator(void);
		~CGrowPointOperator(void);

	public:

	    virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
        virtual String getHelp(void) const;
	    virtual void undo(void);
        virtual void redo(void);



		// 怪物实例id
		unsigned long m_ulInstanceId;

		// 怪物外形点
        unsigned long m_ulTypeId;

		// 怪物名字
		String m_strName;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CGrowPointEditAction* m_pAction;

		// 设置区域编辑操作的指针.
		void SetAction(CGrowPointEditAction* pAction);

		// 怪物实例id
		void SetInstanceId(unsigned long ulInstanceId);

		// 设置怪物外形id
		void SetTypeId(unsigned long ulTypeId);

		// 设置undo， redo 操作需要的位置.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

		// 怪物名字.
		void SetName(String& strName);


	};




	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// 添加一个新怪
	//
	class CGrowPointAddOperator : public CGrowPointOperator
	{
	public:
		CGrowPointAddOperator(void);
		~CGrowPointAddOperator(void);

	public:
		

		
		virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
	    virtual String getHelp(void) const;


		virtual void undo(void);
        virtual void redo(void);

	};



	/////////////////////////////////////////////////////////////////////////////////
	//
	// 删除一个怪物的操作.
	//
	//
	class CGrowPointDelOperator : public CGrowPointOperator
	{
	public:
		CGrowPointDelOperator(void);
		~CGrowPointDelOperator(void);

	public:
		

		
		virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
	    virtual String getHelp(void) const;

      	virtual void undo(void);
		virtual void redo(void);

	};




















}
















