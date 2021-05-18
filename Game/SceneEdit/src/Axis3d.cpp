#include "Axis3d.h"

#include "Core/WXUtils.h"

#include <OgreQuaternion.h>
#include <OgreSimpleRenderable.h>
#include <OgreVector3.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareBuffer.h>
#include <OgreRenderQueue.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreMatrix4.h>
#include <OgreSceneNode.h>
#include <OgrePlane.h>
#include <OgreRay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreVector2.h>
#include <OgreStringConverter.h>
#include <OgreMaterialManager.h>

namespace WX
{
   const int AxisTextSize = 32;

   static Ogre::String axisText[3] =
   {
       Ogre::String("X"),
       Ogre::String("Y"),
       Ogre::String("Z")
   };


   AxisRenderable::AxisRenderable(int lineCount,Ogre::Camera* camera,Ogre::Viewport* viewport) 
   {
        // Disable cast shadows by default
        mCastShadows = false;

      mPrevAxisGizmoSelAxis = -1;
      mCamera = camera;
      mViewport = viewport;

      m_max_line_count = m_line_count  = lineCount;
      m_line_count = 0;

      mLength = 200;
      mProjectDistance = mCamera->getFarClipDistance();
      m_local = false;
      m_locked_data = 0;
      //m_locked_buffer = 0;
      mRenderOp.vertexData = new Ogre::VertexData();
      m_radius = 0;

      mRenderOp.indexData = 0;
      mRenderOp.vertexData->vertexCount = m_line_count*2;
      mRenderOp.vertexData->vertexStart = 0;

      mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST; 
      mRenderOp.useIndexes = false; 

      Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
      Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

      decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
      decl->addElement(0, 3*sizeof(Ogre::Real), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

      Ogre::HardwareVertexBufferSharedPtr vbuf = 
         Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
         decl->getVertexSize(0),
         2*m_max_line_count,
         Ogre::HardwareBuffer::HBU_WRITE_ONLY);

      bind->setBinding(0, vbuf);

      // Obtain the pure colour material
      Ogre::MaterialPtr pureColourMaterial = createPureColourMaterial(Ogre::ColourValue(1,1,0));

      Ogre::String realName = pureColourMaterial->getName() + "_NoDepthBuffer";
      Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(realName);
      if (material.isNull())
      {
          // Clone to avoid confuse somewhere else that using the same material
          material = pureColourMaterial->clone(realName);
          material->setDepthCheckEnabled(false);
          material->setDepthWriteEnabled(false);
      }

      setMaterial(material->getName());

      mBox.setExtents(-10,-10,-10,10,10,10);
      setQueryFlags(0);
   }

   AxisRenderable::~AxisRenderable() 
   {
      delete mRenderOp.vertexData;
      destroyElements();
   }

   void AxisRenderable::getWorldTransforms( Ogre::Matrix4* xform ) const
   {
      // return identity matrix to prevent parent transforms
      if(m_local)
      {
          *xform = Ogre::Matrix4::IDENTITY;
      }
      else
      {
          Ogre::SimpleRenderable::getWorldTransforms(xform );
      }
   }

   const Ogre::Quaternion& AxisRenderable::getWorldOrientation(void) const
   {
      if(m_local)
      {
         return Ogre::Quaternion::IDENTITY;
      }
      else
      {
         return Ogre::SimpleRenderable::getWorldOrientation();
      }
   }

   const Ogre::Vector3& AxisRenderable::getWorldPosition(void) const
   {
      if(m_local)
      {
         return Ogre::Vector3::ZERO;
      }
      else
      {
         return Ogre::SimpleRenderable::getWorldPosition();
      }
   }

   void AxisRenderable::setLocal(bool local)
   {
      m_local = local;
   }

   bool AxisRenderable::getLocal()
   {
      return m_local;
   }

   void AxisRenderable::lock()
   {
      if(!m_locked_data)
      {
         m_locked_buffer = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
         m_locked_data = static_cast<Ogre::Real*>(m_locked_buffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
      }
   }

   void AxisRenderable::unlock()
   {
      m_locked_buffer->unlock();
      m_locked_data = NULL;

	  m_radius = Ogre::Vector3((mBox.getMinimum() - mBox.getMaximum())*0.5).length();
   }

   void AxisRenderable::drawLine(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::ColourValue &c1, const Ogre::ColourValue &c2,unsigned int lineCount)
   {
      assert(lineCount >= 0 && lineCount < m_max_line_count && m_locked_data);
      if(lineCount<0 || lineCount >= m_max_line_count || !m_locked_data)
      {
         return;
      }

      Ogre::Real *pPos = m_locked_data;

      pPos = (Ogre::Real *)((unsigned int)(pPos) + 6*lineCount*sizeof(Ogre::Real) + 2*lineCount*sizeof(unsigned int));
      *pPos++ = v1.x;
      *pPos++ = v1.y;
      *pPos++ = v1.z;
      Ogre::Root::getSingleton().convertColourValue(c1,((Ogre::uint32 *)pPos));
      pPos = (Ogre::Real *)((unsigned int)(pPos) + sizeof(unsigned int));
      *pPos++ = v2.x;
      *pPos++ = v2.y;
      *pPos++ = v2.z;
      Ogre::Root::getSingleton().convertColourValue(c2,((Ogre::uint32 *)pPos));

      mBox.merge(v1);
      mBox.merge(v2);

      mRenderOp.vertexData->vertexCount = 3 * 2;
      mRenderOp.vertexData->vertexStart = 0;
  
      setBoundingBox(mBox);
   }

   void AxisRenderable::clear()
   {
      m_line_count = 0;
      mRenderOp.vertexData->vertexCount = m_line_count;
      mBox.setExtents(-100000,-100000,-100000,100000,100000,100000);
   }

   Ogre::Real AxisRenderable::getSquaredViewDepth(const Ogre::Camera* cam) const
   {
      Ogre::Vector3 min, max, mid, dist;
      min = mBox.getMinimum();
      max = mBox.getMaximum();
      mid = ((min - max) * 0.5) + min;
      dist = cam->getDerivedPosition() - mid;
      return dist.squaredLength();
   }

   //helper function
   inline Ogre::Real
   intersect(Ogre::Plane* plane,const Ogre::Vector3 &p1, const Ogre::Vector3 &p2)
   {
       Ogre::Vector3 dir = p2 - p1;
       Ogre::Real den = dir.dotProduct(plane->normal);
       if(den == 0)
           return 1e20f;
       return -plane->getDistance(p1) / den;
   }

   inline Ogre::Vector3
   interpolate(const Ogre::Vector3& from, const Ogre::Vector3& to, const Ogre::Real factor)
   {
       assert(factor >= 0.0f && factor <= 1.0f);
       
       Ogre::Vector3 result;

       Ogre::Real inverse = 1.0f - factor;
       result[0] = from[0] * inverse + to[0] * factor;
       result[1] = from[1] * inverse + to[1] * factor;
       result[2] = from[2] * inverse + to[2] * factor;

       return result;
   }

   void AxisRenderable::calcAxisInfo(Ogre::Vector3& pos,int length)
   {
       mAxisGizmoCenter = pos;
       mLength = length;

       Ogre::Vector3 axisVector[3] = {
           Ogre::Vector3(1,0,0),
           Ogre::Vector3(0,1,0),
           Ogre::Vector3(0,0,1)
       };
     
       for(unsigned int  i = 0; i < 3; i++)
           mAxisGizmoVector[i] = axisVector[i];  
      
       createGuiElements();
   } 

   void 
   AxisRenderable::renderAxisText()
   {
       for(int i=0;i<3;i++)
       {
           // Project the sphere onto the camera
           Ogre::Real left, right, top, bottom;

           Ogre::Vector3 offPos(0,0,0);
           offPos[i] = mLength;
           Ogre::Vector3 textPos = getWorldPosition() + offPos;
           Ogre::Sphere sphere(textPos,AxisTextSize);

           Ogre::Vector2 mCenterPosSC;
           if (mCamera->projectSphere(sphere, &left, &top, &right, &bottom))
           {
               int iLeft, iTop, iWidth, iHeight;
               mViewport->getActualDimensions(iLeft, iTop, iWidth, iHeight);
               size_t szLeft, szRight, szTop, szBottom;

               szLeft = (size_t)(iLeft + ((left + 1) * 0.5 * iWidth));
               szRight = (size_t)(iLeft + ((right + 1) * 0.5 * iWidth));
               szTop = (size_t)(iTop + ((-top + 1) * 0.5 * iHeight));
               szBottom = (size_t)(iTop + ((-bottom + 1) * 0.5 * iHeight));

               mCenterPosSC.x = ( szLeft + szRight ) / 2;
               mCenterPosSC.y = ( szTop + szBottom ) / 2; 
           }

           int nTop = mCenterPosSC.y  - AxisTextSize/2;
           int nLeft = mCenterPosSC.x  - AxisTextSize/2;
           
           mTextRect[i]->setLeft(nLeft);
           mTextRect[i]->setTop(nTop); 

           mTextRect[i]->setWidth(AxisTextSize);
           mTextRect[i]->setHeight(AxisTextSize);
           mTextRect[i]->setCaption(axisText[i]);

   
       }

       for(int i=0;i<3;i++)
       {
           mTextRect[i]->setParameter("colour_top", "0.5 0.7 0.5");
           mTextRect[i]->setParameter("colour_bottom", "0.5 0.7 0.5");
       }
       if(mAxisGizmoSelAxis != 1 && IsOK())
       {
           mTextRect[mAxisGizmoSelAxis]->setParameter("colour_top", "1 1 1");
           mTextRect[mAxisGizmoSelAxis]->setParameter("colour_bottom", "1 1 1");
       }
   }

   bool AxisRenderable::IsOK()
   {
       return (mAxisGizmoSelAxis>=0 && mAxisGizmoSelAxis<=2) ||
           (mPrevAxisGizmoSelAxis>=0 && mPrevAxisGizmoSelAxis<=2);
   }
   void 
   AxisRenderable::renderAxis()
   {
       if(mAxisGizmoSelAxis != mPrevAxisGizmoSelAxis && IsOK())
       {    
           if(mPrevAxisGizmoSelAxis == -1)
           {
               Ogre::Vector3 now(0.f,0.f,0.f);
               now[mAxisGizmoSelAxis] = mLength;

               lock();
               drawLine(Ogre::Vector3(0,0,0), now, Ogre::ColourValue::White,   Ogre::ColourValue::White,mAxisGizmoSelAxis);
               unlock();
           }
           else if(mAxisGizmoSelAxis == -1)
           { 
               Ogre::Vector3 prev(0.f,0.f,0.f);
               prev[mPrevAxisGizmoSelAxis] = mLength;
               Ogre::ColourValue prevColour(0.f,0.f,0.f);
               unsigned int val32 = 0xff << ((3-mPrevAxisGizmoSelAxis)*8);
               prevColour.setAsRGBA(val32);

               lock();
               drawLine(Ogre::Vector3(0,0,0), prev, prevColour,  prevColour,mPrevAxisGizmoSelAxis);
               unlock();
           }
           else
           {   
               Ogre::Vector3 now(0.f,0.f,0.f);
               now[mAxisGizmoSelAxis] = mLength;

               lock();
               drawLine(Ogre::Vector3(0,0,0), Ogre::Vector3(mLength,0,0), Ogre::ColourValue::Red,   Ogre::ColourValue::Red,0);
               drawLine(Ogre::Vector3(0,0,0), Ogre::Vector3(0,mLength,0), Ogre::ColourValue::Green, Ogre::ColourValue::Green,1);
               drawLine(Ogre::Vector3(0,0,0), Ogre::Vector3(0,0,mLength), Ogre::ColourValue::Blue,  Ogre::ColourValue::Blue,2);
               drawLine(Ogre::Vector3(0,0,0), now, Ogre::ColourValue::White,   Ogre::ColourValue::White,mAxisGizmoSelAxis);
               unlock();
           }

           mPrevAxisGizmoSelAxis = mAxisGizmoSelAxis;
       }     
   }



    bool  
    AxisRenderable::collideAxis(Ogre::Ray& ray)
    {   
        Ogre::Vector3 dir = getWorldPosition() - ray.getOrigin();
        Ogre::Real mAxisGizmoProjLen = mLength / mViewport->getActualWidth() * dir.length() * Ogre::Math::Tan(mCamera->getFOVy());

        dir.normalise();
        mAxisGizmoSelAxis = -1;

        // find axis to use...
        for(unsigned int  i = 0; i < 3; i++)
        {
            Ogre::Vector3 up, normal;

            up = dir.crossProduct(mAxisGizmoVector[i]);
            normal = up.crossProduct(mAxisGizmoVector[i]);

            if(normal.isZeroLength())
                break;

            Ogre::Plane plane(normal,getWorldPosition());


            // width of the axis poly is 1/10 the run   
            Ogre::Vector3 a = up * mAxisGizmoProjLen / 10;
            Ogre::Vector3 b = mAxisGizmoVector[i] * mAxisGizmoProjLen;

            Ogre::Vector3 poly [] = 
            {
                Ogre::Vector3(getWorldPosition() + a),
                    Ogre::Vector3(getWorldPosition() + a + b),
                    Ogre::Vector3(getWorldPosition() - a + b),
                    Ogre::Vector3(getWorldPosition() - a)
            };

            Ogre::Vector3 end = ray.getPoint(mProjectDistance);
            Ogre::Real t = intersect(&plane,ray.getOrigin(), end);

            if(t >= 0 && t <= 1)
            {
                Ogre::Vector3 pos = interpolate(ray.getOrigin(), end, t);

                // check if inside our 'poly' of this axis vector...
                bool inside = true;
                for(unsigned int  j = 0; inside && (j < 4); j++)
                {
                    unsigned int k = (j+1) % 4;
                    Ogre::Vector3 vec1 = poly[k] - poly[j];
                    Ogre::Vector3 vec2 = pos - poly[k];

                    if(vec1.dotProduct(vec2) >0.f)
                        inside = false;   
                }

                //
                if(inside)
                {
                    mAxisGizmoSelAxis = i;
                    return(true);
                }
            }
        }      

        return(false);
    }

    void
    AxisRenderable::_updateRenderQueue(Ogre::RenderQueue* queue)
    {   
        renderAxis();
        renderAxisText();

		queue->addRenderable( this, Ogre::RENDER_QUEUE_OVERLAY, 500); 
    }

    void
    AxisRenderable::createGuiElements(void)
    {
        mTextCenterPointOverlay = Ogre::OverlayManager::getSingleton().create("AxisOverlay");
        mTextCenterPointOverlay->setZOrder(500);
        // this panel will be the main container for our selecting rectangle
        
        mOverlayContainer = static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement("BorderPanel", "AxisOverlayContainer"));
        mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
        mOverlayContainer->setMaterialName("Core/StatsBlockCenter");
        mOverlayContainer->setParameter("border_size", "1 1 1 1");
        mOverlayContainer->setParameter("border_material", "Core/StatsBlockBorder");
        mOverlayContainer->setParameter("border_topleft_uv", "0.0000 1.0000 0.0039 0.9961");
        mOverlayContainer->setParameter("border_top_uv", "0.0039 1.0000 0.9961 0.9961");
        mOverlayContainer->setParameter("border_topright_uv", "0.9961 1.0000 1.0000 0.9961");
        mOverlayContainer->setParameter("border_left_uv","0.0000 0.9961 0.0039 0.0039");
        mOverlayContainer->setParameter("border_right_uv","0.9961 0.9961 1.0000 0.0039");
        mOverlayContainer->setParameter("border_bottomleft_uv","0.0000 0.0039 0.0039 0.0000");
        mOverlayContainer->setParameter("border_bottom_uv","0.0039 0.0039 0.9961 0.0000");
        mOverlayContainer->setParameter("border_bottomright_uv","0.9961 0.0039 1.0000 0.0000");

        mOverlayContainer->setParameter("font_name", "TrebuchetMSBold");
        mOverlayContainer->setParameter("char_height", Ogre::StringConverter::toString(AxisTextSize));
        mOverlayContainer->setParameter("colour_top", "0.5 0.7 0.5");
        mOverlayContainer->setParameter("colour_bottom", "0.5 0.7 0.5");

        mTextCenterPointOverlay->add2D(mOverlayContainer);

        for(int i=0;i<3;i++)
        {
            mTextRect[i] = 
                Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", axisText[i]);
            mTextRect[i]->setMetricsMode(Ogre::GMM_PIXELS);      
            mTextRect[i]->setParameter("font_name", "TrebuchetMSBold");
            mTextRect[i]->setParameter("char_height", Ogre::StringConverter::toString(AxisTextSize));
            mTextRect[i]->setParameter("colour_top", "0.5 0.7 0.5");
            mTextRect[i]->setParameter("colour_bottom", "0.5 0.7 0.5");

            mOverlayContainer->addChild(mTextRect[i]);
            mTextCenterPointOverlay->show();

        }
    }

    void AxisRenderable::destroyElements()
    {
        for(int i=0;i<3;i++)
        {
            if (mTextRect[i])
            {
                mOverlayContainer->removeChild(mTextRect[i]->getName());
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(mTextRect[i]->getName());
                mTextRect[i] = NULL;
            }
        }

        if(mOverlayContainer)
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(mOverlayContainer->getName());
            mOverlayContainer = NULL;
        }

        if(mTextCenterPointOverlay)
        {
            Ogre::OverlayManager::getSingleton().destroy(mTextCenterPointOverlay);
            mTextCenterPointOverlay = NULL;
        }

    }

	void
	AxisRenderable::showElement(bool visible)
	{
		if(visible)
			mTextCenterPointOverlay->show();
		else
			mTextCenterPointOverlay->hide();
	}
}
