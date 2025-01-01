#ifndef TONIC_GRAPHICS_BUFFER_H
#define TONIC_GRAPHICS_BUFFER_H

#include "Tonic/Graphics/Resource.h"
#include <span>

namespace Tonic::Graphics
{
enum class BufferRole
{
    Index,
    Vertex,
    Uniform
};

class Buffer : public Resource 
{
public:
    Buffer(Device &device, std::span<const unsigned char> data, BufferRole bufferRole);
    Buffer(Device &device, unsigned int size, BufferRole bufferRole);
    ~Buffer();

    void SetData(std::span<const unsigned char> data);
    void SetSubData(std::span<const unsigned char> data, unsigned int offset);

    inline BufferRole GetRole() const { return m_BufferRole; }
    inline unsigned int GetID() const { return m_ID; }
private:
    BufferRole m_BufferRole;
    unsigned int m_ID;

};
}

#endif // TONIC_GRAPHICS_BUFFER_H