#include "Tonic/Platform/Graphics/OpenGL/FrameBuffer.h"
#include "Tonic/Graphics/Texture.h"
#include "Tonic/Platform/Graphics/OpenGL/Texture.h"
#include <GL/gl3w.h>
#include <Ethyl/Assert.h>

namespace Tonic::Graphics::OpenGL
{
OGLFrameBuffer::OGLFrameBuffer(Device &device, const FrameBufferDesc &desc) : FrameBuffer(device), m_ColorAttachment(desc.color), m_DepthAttachment(desc.depth)
{
    ETHYL_ASSERT(m_ColorAttachment != nullptr, "Color attachment is a null pointer!");

    glCreateFramebuffers(1, &m_ID);
    ETHYL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not create a FrameBuffer.");

    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

    auto color = std::static_pointer_cast<OGLTexture>(m_ColorAttachment);
    auto depth = std::static_pointer_cast<OGLTexture>(m_DepthAttachment);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->GetID(), 0);
    if (depth != nullptr) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth->GetID(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OGLFrameBuffer::~OGLFrameBuffer()
{
    glDeleteFramebuffers(1, &m_ID);
}

const glm::ivec2 OGLFrameBuffer::GetViewportSize() const
{
    return std::static_pointer_cast<OGLTexture>(m_ColorAttachment)->GetSize();
}
}