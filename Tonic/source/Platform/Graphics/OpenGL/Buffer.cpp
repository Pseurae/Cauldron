#include "Tonic/Platform/Graphics/OpenGL/Buffer.h"
#include <GL/gl3w.h>

namespace Tonic::Graphics::OpenGL
{
OGLBuffer::OGLBuffer(Device &device, BufferRole bufferRole, std::span<const unsigned char> data) : Buffer(device, bufferRole)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW);
}

OGLBuffer::OGLBuffer(Device &device, BufferRole bufferRole, unsigned int size) : Buffer(device, bufferRole)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OGLBuffer::~OGLBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void OGLBuffer::SetData(std::span<const unsigned char> data)
{
    glBufferData(GL_ARRAY_BUFFER, data.size(), data.data(), GL_STATIC_DRAW);
}

void OGLBuffer::SetSubData(std::span<const unsigned char> data, unsigned int offset)
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, data.size(), data.data());
}
}