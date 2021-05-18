#ifndef AXIS_RENDERABLE_H
#define AXIS_RENDERABLE_H

#include <OgrePrerequisites.h>
#include <OgreSimpleRenderable.h>
#include <OgreHardwareVertexBuffer.h>

namespace Ogre
{
    class TextAreaOverlayElement;
}
namespace WX
{
    class AxisRenderable : public Ogre::SimpleRenderable
   {
   protected:
      void getWorldTransforms( Ogre::Matrix4* xform ) const;
      const Ogre::Quaternion& getWorldOrientation(void) const;
      const Ogre::Vector3& getWorldPosition(void) const;

      Ogre::Real m_radius;
      unsigned int m_line_count;
      unsigned int m_max_line_count;
      bool m_local;
      Ogre::Real* m_locked_data;
      Ogre::HardwareVertexBufferSharedPtr m_locked_buffer;
      
      float mLength;
      Ogre::Vector3 mAxisGizmoCenter;
      Ogre::Vector3 mAxisGizmoVector[3];
      int mAxisGizmoSelAxis;
      int mPrevAxisGizmoSelAxis;
      int mProjectDistance;

      Ogre::Viewport* mViewport;
      Ogre::Camera* mCamera;


      Ogre::Overlay* mTextCenterPointOverlay;
      Ogre::OverlayContainer* mOverlayContainer;
      Ogre::OverlayElement* mTextRect[3];
            
   public:
       AxisRenderable(int lineCount,Ogre::Camera* camera,Ogre::Viewport* viewport);
      ~AxisRenderable();
      Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
      Ogre::Real getBoundingRadius(void) const { return m_radius; }

      // Delete all lines in the AxisRenderable. Since lines can't be modified after added, use this function to
      // clear the AxisRenderable so you can re-add the lines with new positions/colours.
      void clear();

      // Lock the AxisRenderable, ready for drawLine() calls.
      void lock();

      // Unlock the AxisRenderable.
      void unlock();

      // Draw a line between v1 and v2, with the colours c1 and c2 at the corresponding ends.
      // Lines are preserved (except after a clear()), if they don't change then you only need to add them once.
      // Lines can only be drawn while the AxisRenderable is locked.
      void drawLine(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::ColourValue &c1 = Ogre::ColourValue::White, const Ogre::ColourValue &c2 = Ogre::ColourValue::White,unsigned int lineCount =0);

      // Set the coordinate system to use for the AxisRenderable. If true, all line coordinates will be local to the
      // scene node the AxisRenderable is attached to. If false, all line coordinates will use world coordinates.
      // (So when true, lines rotate and move with the node they belong to)
      void setLocal(bool local);
      bool getLocal();

      // Add a local axis display to a scenenode. This causes the scenenode to display a red line along it's x axis, 
      // a green line along it's y axis and a blue line along it's z axis. Length determines how long each axis line is.
      // The AxisRenderable pointer returned can be discarded, it's already been attached to the scenenode.
      
      int getSelAxis()
      {
          return mAxisGizmoSelAxis;
      }
      void createGuiElements();
      void destroyElements();
      bool collideAxis(Ogre::Ray& ray);
      void calcAxisInfo(Ogre::Vector3& pos,int length);
	  void showElement(bool visible);

   protected:
       void _updateRenderQueue(Ogre::RenderQueue* queue);
       void renderAxisText();
       void renderAxis();
       bool IsOK();
   };
}

#endif

