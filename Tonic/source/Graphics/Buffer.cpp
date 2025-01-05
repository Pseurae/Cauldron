#include "Tonic/Graphics/Buffer.h"
#include <GL/gl3w.h>

#include <Ethyl/Assert.h>

namespace Tonic::Graphics
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

Buffer::Buffer(Device &device, std::span<const unsigned char> data, BufferRole bufferRole) : Resource(device), mBufferRole(bufferRole)
{
    auto target = GLTarget(bufferRole);

    glGenBuffers(1, &mID);

    glBindBuffer(target, mID);
    glBufferData(target, data.size(), data.data(), GL_STATIC_DRAW);
    glBindBuffer(target, 0);
}

Buffer::Buffer(Device &device, unsigned int size, BufferRole bufferRole) : Resource(device), mBufferRole(bufferRole)
{
    auto target = GLTarget(bufferRole);

    glGenBuffers(1, &mID);

    glBindBuffer(target, mID);
    glBufferData(target, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(target, 0);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &mID);
}

void Buffer::SetData(std::span<const unsigned char> data)
{
    auto target = GLTarget(GetRole());

    glBindBuffer(target, mID);
    glBufferData(target, data.size(), data.data(), GL_STATIC_DRAW);
    glBindBuffer(target, 0);
}

void Buffer::SetSubData(std::span<const unsigned char> data, unsigned int offset)
{
    auto target = GLTarget(GetRole());

    glBindBuffer(target, mID);
    glBufferSubData(target, offset, data.size(), data.data());
    glBindBuffer(target, 0);
}
}