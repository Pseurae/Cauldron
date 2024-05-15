#ifndef TONIC_GRAPHICS_BUFFER_H
#define TONIC_GRAPHICS_BUFFER_H

#include "Tonic/Graphics/Resource.h"
#include <span>

namespace Tonic::Graphics
{
enum class BufferRole
{
    Index,
    Vertex
};

class Buffer : public Resource 
{
public:
    Buffer(Device &device, BufferRole bufferRole) : Resource(device), m_BufferRole(bufferRole) {}

    virtual void SetData(std::span<const unsigned char> data) = 0;
    virtual void SetSubData(std::span<const unsigned char> data, unsigned int offset) = 0;

    BufferRole GetRole() const { return m_BufferRole; }
private:
    BufferRole m_BufferRole;
};
}

#endif // TONIC_GRAPHICS_BUFFER_H