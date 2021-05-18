#pragma once
#include "FalagardSelfFitWindow.h"
#include "IFalagardBeHitBoard.h"
// 角色头上冒的字,比如减的血和信息
namespace CEGUI
{
	namespace FalagardBeHitBoardProperties
	{
		class UseTempPos: public Property	
		{
			public:
			UseTempPos() : Property(
				"UseTempPos",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class TempPoint: public Property	
		{
			public:
			TempPoint() : Property(
				"TempPoint",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class DoubleTime: public Property	
		{
			public:
			DoubleTime() : Property(
				"DoubleTime",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class DoubleEndSize: public Property	
		{
			public:
			DoubleEndSize() : Property(
				"DoubleEndSize",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class DoubleSize: public Property	
		{
			public:
			DoubleSize() : Property(
				"DoubleSize",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class NormalSize: public Property	
		{
			public:
			NormalSize() : Property(
				"NormalSize",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class MoveMode: public Property	
		{
			public:
			MoveMode() : Property(
				"MoveMode",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class XPos: public Property	
		{
			public:
			XPos() : Property(
				"XPos",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class YPos: public Property	
		{
			public:
			YPos() : Property(
				"YPos",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class XSpeed: public Property	
		{
			public:
			XSpeed() : Property(
				"XSpeed",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class YSpeed: public Property	
		{
			public:
			YSpeed() : Property(
				"YSpeed",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class XAcceleration: public Property	
		{
			public:
			XAcceleration() : Property(
				"XAcceleration",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class YAcceleration: public Property	
		{
			public:
			YAcceleration() : Property(
				"YAcceleration",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class MaxLife: public Property	
		{
			public:
			MaxLife() : Property(
				"MaxLife",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class AlphaMode: public Property	
		{
			public:
			AlphaMode() : Property(
				"AlphaMode",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*8***********************************************************************************
		class ColorType:public Property
		{
			public:
			ColorType() : Property(
				"ColorType",
				"Property to get/set Minimap name status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		//*8***********************************************************************************
		class BackColorType:public Property
		{
			public:
			BackColorType() : Property(
				"BackColorType",
				"",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	};
	
	class FalagardBeHitBoard :	public FalagardSelfFitWindow, public IFalagardBeHitBoard
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
	public:
		FalagardBeHitBoard(const String& type, const String& name);
		~FalagardBeHitBoard(void);

		
	protected:
		int  m_nXSpeed[ MAX_MOVE_TYPE_NUMBER ];
		int  m_nYSpeed[ MAX_MOVE_TYPE_NUMBER ];
		int  m_nXAcceleration[ MAX_MOVE_TYPE_NUMBER ];
		int  m_nYAcceleration[ MAX_MOVE_TYPE_NUMBER ];
		int  m_nLife[ MAX_MOVE_TYPE_NUMBER ];
		int  m_nXPos[ MAX_MOVE_TYPE_NUMBER ];
		int  m_nYPos[ MAX_MOVE_TYPE_NUMBER ];
		int  m_UseTempPos[ MAX_MOVE_TYPE_NUMBER ];

		bool m_bAlpha[ MAX_MOVE_TYPE_NUMBER ];
		colour  m_ColorType[ MAX_COLOR_TYPE_NUMBER ];
		colour  m_BackColorType[ MAX_COLOR_TYPE_NUMBER ];

		int  m_nMoveMode[ MAX_MOVE_TYPE_NUMBER ];

		Point m_ptTmpPos[ MAX_TEMP_TYPE_NUMBER ];


		float m_fDoubleSize;
		float m_fDoubleEndSize;
		float m_fNormalSize;
		float m_fDoubleTime;
	public:
		virtual int getTempPosUse( int nType ) const { return m_UseTempPos[ nType ]; };
		void setTempPosUse( int nType, int value ) { m_UseTempPos[ nType ] = value; };

		virtual float getDoubleEndSize()const { return m_fDoubleEndSize; };
		void setDoubleEndSize( float fValue ) { m_fDoubleEndSize = fValue; };

		virtual float getDoubleTime()const { return m_fDoubleTime; };
		void setDoubleTime( float fValue ) { m_fDoubleTime = fValue; };

		virtual float getDoubleSize() const { return m_fDoubleSize; };
		void  setDoubleSize( float fValue ){ m_fDoubleSize = fValue; };

		virtual float getNormalSize() const{ return m_fNormalSize; };
		void  setNormalSize( float fValue ){ m_fNormalSize = fValue; };

		virtual int  getMoveMode( int nType ) const { return m_nMoveMode[ nType ] ; };
		void setMoveMode( int nType,int nValue ) { m_nMoveMode[ nType ] = nValue; };
		

		virtual int  getXPos( int nType ) const { return m_nXPos[ nType ] ; };
		void setXPos( int nType,int nValue ) { m_nXPos[ nType ] = nValue; };
		
		virtual int  getYPos( int nType ) const { return m_nYPos[ nType ] ; };
		void setYPos( int nType,int nValue ) { m_nYPos[ nType ] = nValue; };

		virtual bool getAlpha( int nType ) const { return m_bAlpha[ nType ];};
		void setAlpha( int nType,bool bAlpha ) { m_bAlpha[ nType ] = bAlpha; };

		virtual int  getXSpeed( int nType ) const { return m_nXSpeed[ nType ] ; };
		void setXSpeed( int nType,int nValue ) { m_nXSpeed[ nType ] = nValue; };
		
		virtual int  getYSpeed( int nType )const { return m_nYSpeed[ nType ] ; };
		void setYSpeed( int nType,int nValue){ m_nYSpeed[ nType ] = nValue; };

		virtual int  getXAcceleration( int nType )const { return m_nXAcceleration[ nType ] ; };
		void setXAcceleration( int nType,int nValue ){ m_nXAcceleration[ nType ] = nValue; };

		virtual int  getYAcceleration( int nType )const { return m_nYAcceleration[ nType ] ; };
		void setYAcceleration( int nType,int nValue ){ m_nYAcceleration[ nType ] = nValue; };

		virtual int  getLife( int nType )const { return m_nLife[ nType ] ; };
		void setLife( int nType, int nValue ){ m_nLife[ nType ] = nValue; };

		virtual colour getColorType( int nType ) { return m_ColorType[ nType ]; };
		void setColorType( int nType, colour col ) { m_ColorType[ nType ] = col; };

		virtual colour getBackColorType( int nType ) { return m_BackColorType[ nType ]; };
		void setBackColorType( int nType, colour col ) { m_BackColorType[ nType ] = col; };

		virtual Point getTempPos( int nType ) const { return m_ptTmpPos[ nType ]; };
		void setTempPos( int nType, Point point ) { m_ptTmpPos[ nType ] = point; };

		virtual void setDisplayCenter( float fX, float fY );

		virtual void setTextScale( float fScaleX, float fScaleY);
		virtual float getTextScaleX();
		virtual float getTextScaleY();
		virtual void  SetTextColor( const colour& col );
	private:
		static FalagardBeHitBoardProperties::XPos			d_XPosProperty;
        static FalagardBeHitBoardProperties::YPos			d_YPosProperty;
		static FalagardBeHitBoardProperties::XSpeed			d_XSpeedProperty;
        static FalagardBeHitBoardProperties::YSpeed			d_YSpeedProperty;
		static FalagardBeHitBoardProperties::XAcceleration	d_XAccelerationproperty;
		static FalagardBeHitBoardProperties::YAcceleration	d_YAccelerationProperty;
        static FalagardBeHitBoardProperties::MaxLife		d_MaxLifeProperty;
		static FalagardBeHitBoardProperties::AlphaMode		d_AlphaModeProperty;
		static FalagardBeHitBoardProperties::ColorType		d_ColorTypeProperty;
		static FalagardBeHitBoardProperties::BackColorType	d_BackColorTypeProperty;
		static FalagardBeHitBoardProperties::NormalSize     d_NormalSizeProperty;
		static FalagardBeHitBoardProperties::DoubleSize     d_DoubleSizeProperty;
		static FalagardBeHitBoardProperties::DoubleTime		d_DoubleTimeProperty;
		static FalagardBeHitBoardProperties::DoubleEndSize  d_DoubleEndSizeProperty;
		static FalagardBeHitBoardProperties::MoveMode		d_MoveModeProperty;
		static FalagardBeHitBoardProperties::TempPoint		d_TempPointProperty;
		static FalagardBeHitBoardProperties::UseTempPos		d_UseTempPosProperty;

	};

	class FALAGARDBASE_API FalagardBeHitFactory : public WindowFactory
    {
    public:
        FalagardBeHitFactory(void) : WindowFactory(FalagardBeHitBoard::WidgetTypeName) { }
        ~FalagardBeHitFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}