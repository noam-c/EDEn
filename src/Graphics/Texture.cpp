#include "Texture.h"
#include <SDL.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "GraphicsUtil.h"
#include "Size.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

Texture::Texture() :
   valid(false)
{
   glGenTextures(1, &textureHandle);
}

Texture::Texture(const std::string& imagePath) :
   valid(false)
{
   // Create the texture
   DEBUG("Generating texture...");
   glGenTextures(1, &textureHandle);

   // Create storage space for the texture and load the image
   DEBUG("Loading image %s...", imagePath.c_str());
   SDL_Surface *image = IMG_Load(imagePath.c_str());

   if (!image)
   {
      // Problem loading the image; throw an exception
      DEBUG(std::string("Unable to load image: ") + IMG_GetError());
      return;
   }

   DEBUG("Image load successful!");

   size.width = image->w;
   size.height = image->h;

   DEBUG("Binding GL texture");

   // Bind this texture as the current texture OpenGL should work with
   bind();

   // Add Linear filtering for the texture
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   GLenum textureFormat = GL_RGB;
   if (image->format->BytesPerPixel == 4) // contains an alpha channel
   {
      textureFormat = image->format->Rmask == 0x000000ff ? GL_RGBA : GL_BGRA;
   }
   else if (image->format->BytesPerPixel == 3) // no alpha channel
   {
      textureFormat = image->format->Rmask == 0x000000ff ? GL_RGB : GL_BGR;
   }

   // Transfer the image data into the texture
   DEBUG("Transferring image data to GL texture");
   glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, size.width, size.height, 0, textureFormat, GL_UNSIGNED_BYTE, image->pixels);
   valid = true;

   DEBUG("Freeing image surface");
   SDL_FreeSurface(image);

   DEBUG("Texture creation complete.");
}

Texture::Texture(const void* imageData, GLenum imageFormat, const shapes::Size& imageSize, int bytesPerPixel) :
   valid(false),
   size(imageSize)
{
   // Create the texture
   DEBUG("Generating texture...");
   glGenTextures(1, &textureHandle);

   // Bind the texture to alter its parameters
   bind();

   // Add Linear filtering for the texture
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Transfer the image data into the texture
   DEBUG("Transferring image data to GL texture");
   glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, size.width, size.height, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
   valid = true;

   DEBUG("Texture creation complete.");
}

void Texture::bind()
{
   // Any texture ops on GL_TEXTURE_2D will become associated with this texture
   glBindTexture(GL_TEXTURE_2D, textureHandle);
}

bool Texture::isValid() const
{
   return valid;
}

const shapes::Size& Texture::getSize() const
{
   return size;
}

Texture::~Texture()
{
   glDeleteTextures(1, &textureHandle);
}
