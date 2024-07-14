#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_FRAMEBUFFER_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_FRAMEBUFFER_H

#include "Tonic/Graphics/FrameBuffer.h"
#include <glm/vec2.hpp>

namespace Tonic::Graphics::OpenGL
{
class OGLTexture;

class OGLFrameBuffer final : public FrameBuffer
{
public:
    OGLFrameBuffer(Device &, const FrameBufferDesc &);
    ~OGLFrameBuffer();

    auto GetID() const { return m_ID; }
    const glm::ivec2 GetViewportSize() const;
private:
    unsigned int m_ID;
    Ethyl::Shared<Texture> m_ColorAttachment, m_DepthAttachment;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_FRAMEBUFFER_H