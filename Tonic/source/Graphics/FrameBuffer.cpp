#include "Tonic/Graphics/FrameBuffer.h"
#include "Tonic/Graphics/Texture.h"
#include "Tonic/Graphics/Texture.h"
#include <GL/gl3w.h>
#include <Ethyl/Assert.h>

namespace Tonic::Graphics
{
FrameBuffer::FrameBuffer(Device &device, const FrameBufferDesc &desc) : Resource(device), mColorAttachment(desc.color), mDepthAttachment(desc.depth)
{
    ETHYL_ASSERT(mColorAttachment, "Color attachment is a null pointer!");

    glGenFramebuffers(1, &mID);
    ETHYL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not create a FrameBuffer.");

    glBindFramebuffer(GL_FRAMEBUFFER, mID);

    auto color = std::static_pointer_cast<Texture>(mColorAttachment);
    auto depth = std::static_pointer_cast<Texture>(mDepthAttachment);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->GetID(), 0);
    if (depth != nullptr) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth->GetID(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &mID);
}

const glm::ivec2 FrameBuffer::GetViewportSize() const
{
    return std::static_pointer_cast<Texture>(mColorAttachment)->GetSize();
}
}