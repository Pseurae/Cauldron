#include "Tonic/Platform/Graphics/OpenGL/Buffer.h"
#include <GL/gl3w.h>

#include <Ethyl/Assert.h>

namespace Tonic::Graphics::OpenGL
{
static unsigned int GLTarget(BufferRole role)
{
    switch (role)
    {
    case BufferRole::Index:
        return GL_ELEMENT_ARRAY_BUFFER;
    case BufferRole::Vertex:
        return GL_ARRAY_BUFFER;
    case BufferRole::Uniform:
        return GL_UNIFORM_BUFFER;
    }

    ETHYL_BREAK("Invalid BufferRole given.");
}

OGLBuffer::OGLBuffer(OGLDevice &device, std::span<const unsigned char> data, BufferRole bufferRole) : Buffer(device, bufferRole)
{
    auto target = GLTarget(bufferRole);

    glGenBuffers(1, &m_ID);

    glBindBuffer(target, m_ID);
    glBufferData(target, data.size(), data.data(), GL_STATIC_DRAW);
    glBindBuffer(target, 0);
}

OGLBuffer::OGLBuffer(OGLDevice &device, unsigned int size, BufferRole bufferRole) : Buffer(device, bufferRole)
{
    auto target = GLTarget(bufferRole);

    glGenBuffers(1, &m_ID);

    glBindBuffer(target, m_ID);
    glBufferData(target, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(target, 0);
}

OGLBuffer::~OGLBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void OGLBuffer::SetData(std::span<const unsigned char> data)
{
    auto target = GLTarget(GetRole());

    glBindBuffer(target, m_ID);
    glBufferData(target, data.size(), data.data(), GL_STATIC_DRAW);
    glBindBuffer(target, 0);
}

void OGLBuffer::SetSubData(std::span<const unsigned char> data, unsigned int offset)
{
    auto target = GLTarget(GetRole());

    glBindBuffer(target, m_ID);
    glBufferSubData(target, offset, data.size(), data.data());
    glBindBuffer(target, 0);
}
}