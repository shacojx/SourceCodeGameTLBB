#pragma once
#include "FalModule.h"
#include "CEGUIRenderableImage.h"
#include "CEGUIWindowFactory.h"
#include "fallistbox.h"
#include "falimagelistexproperties.h"
#include "elements/ceguilistboxtextitem.h"
#include <vector>
namespace CEGUI
{
	class FALAGARDBASE_API FalagardImageListBoxItemEx : public ListboxTextItem
	{
		int					d_State;  // 当前选项的状态
		const Image*		d_Normal;
		const Image*		d_HorverImage;
		const Image*		d_IconImage;
		bool				d_bHorver;
	public:
		bool IsHorver(){ return d_bHorver; };
		void SetHorver( bool horver = true ){ d_bHorver = horver; };
		void SetHorverImage( const Image* pImage );
		void SetNormalImage( const Image* pImage );
		void SetIconImage( const Image* pImage );
		void SetState( int nState );
		int  GetState() { return d_State; };

		virtual void draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const;

		FalagardImageListBoxItemEx(const String& text, uint item_id = 0, void* item_data = NULL, bool disabled = false, bool auto_delete = true);

		~FalagardImageListBoxItemEx(){};

	};

	class FALAGARDBASE_API FalagardImageListBoxEx :	public FalagardListbox
	{

		int							d_nStateNumber; // 有多少种状态
		FalagardImageListBoxItemEx* d_pLastHorver;
	public:
		static const utf8  WidgetTypeName[];       //!< type name for this widget.
	
		//virtual void drawSelf(float z);
		//virtual void updateSelf(float elapsed);
		virtual void onMouseMove( MouseEventArgs& e );
		virtual void onMouseButtonDown( MouseEventArgs& e );

		void setMaxStateNumber( int nIndex );
		void setStateImage( const Image* image, int nIndex = -1);

		const Image* getStateImage( int nState ) const;
		
		void setSelectImage( const Image* image );	
		const Image* getSelectImage() const;//{ return d_SelectImage; } const;

		void setHorverImage( const Image* image );
		const Image* getHorverImage() const;

		void SetNormalImage( const Image* pImage );
		const Image* getNormalImage() const;
	protected:
		const Image*				d_SelectImage;
		const Image*				d_NormalImage;
		const Image*				d_HorverImage;
		typedef std::list< Image* >  StateImage;
		StateImage			d_stateImage;		//!< RenderableImage used when rendering an image in the normal state.
		static FalagardImageListBoxExProperties::SetStateImage    d_SetStateImageProperty;
		static FalagardImageListBoxExProperties::AddItem		  d_AddItemProperty;
		static FalagardImageListBoxExProperties::SelectImage	  d_SelectImageProperty;
		static FalagardImageListBoxExProperties::HorverImage	  d_HorverImageProperty;
		static FalagardImageListBoxExProperties::NormalImage	  d_NormalImageProperty;

	public:
		FalagardImageListBoxEx(const String& type, const String& name);
		~FalagardImageListBoxEx(void);
	};

	class FALAGARDBASE_API FalagardImageListBoxExFactory : public WindowFactory
    {
    public:
        FalagardImageListBoxExFactory(void) : WindowFactory(FalagardImageListBoxEx::WidgetTypeName) { }
        ~FalagardImageListBoxExFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}
