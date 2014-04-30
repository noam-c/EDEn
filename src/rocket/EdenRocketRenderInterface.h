/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

/*
 * This source file is based on the ShellRenderInterfaceOpenGL class.
 * ShellRenderInterfaceOpenGL.h and ShellRenderInterfaceOpenGL.cpp are
 * part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 */

#ifndef EDEN_ROCKET_RENDER_INTERFACE_H
#define EDEN_ROCKET_RENDER_INTERFACE_H

#include "Rocket/Core/RenderInterface.h"
#include "SDL_opengl.h"

/**
 * OpenGL render interface for Rocket, using EDEn's texture management
 * utilities.
 *
 * @author Peter Curry
 * @author Noam Chitayat
 */
class EdenRocketRenderInterface : public Rocket::Core::RenderInterface
{
   public:
      /**
       * Called by Rocket when it wants to render geometry that it does not wish to optimise.
       */
      virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices, int* indices, int numIndices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

      /**
       * Called by Rocket when it wants to enable or disable scissoring to clip content.
       */
      virtual void EnableScissorRegion(bool enable);

      /**
       * Called by Rocket when it wants to change the scissor region.
       */
      virtual void SetScissorRegion(int x, int y, int width, int height);

      /**
       * Called by Rocket when a texture is required by the library.
       */
      virtual bool LoadTexture(Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, const Rocket::Core::String& source);

      /**
       * Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
       */
      virtual bool GenerateTexture(Rocket::Core::TextureHandle& textureHandle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& sourceDimensions);

      /**
       * Called by Rocket when a loaded texture is no longer required.
       */
      virtual void ReleaseTexture(Rocket::Core::TextureHandle textureHandle);
};

#endif
