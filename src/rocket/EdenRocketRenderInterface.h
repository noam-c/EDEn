/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
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

#include <RmlUi/Core/RenderInterface.h>

/**
 * OpenGL render interface for Rocket, using EDEn's texture management
 * utilities.
 *
 * @author Peter Curry
 * @author Noam Chitayat
 */
class EdenRocketRenderInterface final : public Rml::Core::RenderInterface
{
   public:
      /**
       * Called by Rocket when it wants to render geometry that it does not wish to optimise.
       */
      void RenderGeometry(Rml::Core::Vertex* vertices, int numVertices, int* indices, int numIndices, Rml::Core::TextureHandle texture, const Rml::Core::Vector2f& translation) override;

      /**
       * Called by Rocket when it wants to enable or disable scissoring to clip content.
       */
      void EnableScissorRegion(bool enable) override;

      /**
       * Called by Rocket when it wants to change the scissor region.
       */
      void SetScissorRegion(int x, int y, int width, int height) override;

      /**
       * Called by Rocket when a texture is required by the library.
       */
      bool LoadTexture(Rml::Core::TextureHandle& textureHandle, Rml::Core::Vector2i& textureDimensions, const Rml::Core::String& source) override;

      /**
       * Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
       */
      bool GenerateTexture(Rml::Core::TextureHandle& textureHandle, const Rml::Core::byte* source, const Rml::Core::Vector2i& sourceDimensions) override;

      /**
       * Called by Rocket when a loaded texture is no longer required.
       */
      void ReleaseTexture(Rml::Core::TextureHandle textureHandle) override;
};

#endif
