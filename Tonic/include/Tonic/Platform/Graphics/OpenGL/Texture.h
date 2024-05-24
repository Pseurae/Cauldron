#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_TEXTURE_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_TEXTURE_H

#include "Tonic/Graphics/Texture.h"

namespace Tonic::Graphics::OpenGL
{
class OGLTexture : public Texture
{
public:
    OGLTexture(Device &device, const TextureDesc &desc);
    ~OGLTexture();

    auto GetID() const { return m_TextureID; }
private:
    unsigned int m_TextureID;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_TEXTURE_H