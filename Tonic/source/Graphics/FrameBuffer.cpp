#include "Tonic/Graphics/FrameBuffer.h"
#include "Tonic/Graphics/Texture.h"
#include "Tonic/Graphics/Texture.h"
#include <GL/gl3w.h>
#include <Ethyl/Assert.h>

namespace Tonic::Graphics
{
FrameBuffer::FrameBuffer(Device &device, const FrameBufferDesc &desc) : Resource(device), m_ColorAttachment(desc.color), m_DepthAttachment(desc.depth)
{
    ETHYL_ASSERT(m_ColorAttachment, "Color attachment is a null pointer!");

    glGenFramebuffers(1, &m_ID);
    ETHYL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not create a FrameBuffer.");

    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

    auto color = std::static_pointer_cast<Texture>(m_ColorAttachment);
    auto depth = std::static_pointer_cast<Texture>(m_DepthAttachment);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->GetID(), 0);
    if (depth != nullptr) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth->GetID(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_ID);
}

const glm::ivec2 FrameBuffer::GetViewportSize() const
{
    return std::static_pointer_cast<Texture>(m_ColorAttachment)->GetSize();
}
}