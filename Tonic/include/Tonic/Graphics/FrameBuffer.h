#ifndef TONIC_GRAPHICS_FRAMEBUFFER_H
#define TONIC_GRAPHICS_FRAMEBUFFER_H

#include "Tonic/Graphics/Resource.h"
#include <Ethyl/Pointers.h>
#include <glm/vec4.hpp>

namespace Tonic::Graphics
{
class Texture;

struct FrameBufferDesc
{
    Ethyl::Shared<Texture> color, depth;
};

class FrameBuffer : public Resource
{
public:
    FrameBuffer(Device &device) : Resource(device) {}
};
}

#endif // TONIC_GRAPHICS_FRAMEBUFFER_H