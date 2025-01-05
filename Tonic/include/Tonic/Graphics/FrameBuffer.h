#ifndef TONIC_GRAPHICS_FRAMEBUFFER_H
#define TONIC_GRAPHICS_FRAMEBUFFER_H

#include "Tonic/Graphics/Resource.h"
#include <Ethyl/Pointers.h>
#include <glm/vec2.hpp>

namespace Tonic::Graphics
{
class Texture;

struct FrameBufferDesc
{
    Ethyl::Shared<Texture> color, depth;
};

class FrameBuffer final : public Resource
{
public:
    FrameBuffer(Device &, const FrameBufferDesc &);
    ~FrameBuffer();

    auto GetID() const { return mID; }
    const glm::ivec2 GetViewportSize() const;
private:
    unsigned int mID;
    Ethyl::Shared<Texture> mColorAttachment, mDepthAttachment;
};
}

#endif // TONIC_GRAPHICS_FRAMEBUFFER_H