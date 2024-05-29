#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_BUFFER_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_BUFFER_H

#include "Tonic/Graphics/Buffer.h"
#include <span>

namespace Tonic::Graphics::OpenGL
{
class OGLBuffer : public Buffer
{
public:
    OGLBuffer(Device &device, std::span<const unsigned char> data, BufferRole bufferRole);
    OGLBuffer(Device &device, unsigned int size, BufferRole bufferRole);

    ~OGLBuffer();

    virtual void SetData(std::span<const unsigned char> data) override;
    virtual void SetSubData(std::span<const unsigned char> data, unsigned int offset) override;

    auto GetID() const { return m_ID; }
private:
    unsigned int m_ID;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_BUFFER_H