#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_TEXTURE_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_TEXTURE_H

#include "Tonic/Platform/Graphics/OpenGL/Device.h"
#include "Tonic/Graphics/Texture.h"
#include <glm/vec2.hpp>

namespace Tonic::Graphics::OpenGL
{
class OGLTexture : public Texture
{
public:
    OGLTexture(OGLDevice &device, const TextureDesc &desc);
    ~OGLTexture();

    const auto GetID() const { return m_TextureID; }
    const auto GetSize() const { return m_Size; }
private:
    glm::ivec2 m_Size;
    unsigned int m_TextureID;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_TEXTURE_H