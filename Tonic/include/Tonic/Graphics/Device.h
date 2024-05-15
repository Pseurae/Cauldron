#ifndef TONIC_GRAPHICS_DEVICE_H
#define TONIC_GRAPHICS_DEVICE_H

#include "Tonic/Common/Pointers.h"

#include <span>
#include <string>
#include <glm/vec4.hpp>

namespace Tonic::Core
{
class Window;
}

namespace Tonic::Graphics 
{
class Buffer;
enum class BufferRole;

struct TextureDesc;
class Texture;

struct ShaderDesc;
class Shader;

class PipelineState;

struct DrawIndexedDesc;

class Device 
{
public:
    explicit Device(const Core::Window &window);

    /* Vertices */

    template<typename T>
    Shared<Buffer> CreateBufferGeneric(std::span<const T> data, BufferRole role)
    {
        return CreateBuffer(std::span<const unsigned char>{ (const unsigned char *)data.data(), data.size_bytes() }, role);
    }

    virtual Shared<Buffer> CreateBuffer(std::span<const unsigned char> data, BufferRole role) = 0; // Static
    virtual Shared<Buffer> CreateBuffer(unsigned int size, BufferRole role) = 0; // Dynamic

    /* Shaders */
    virtual Shared<Shader> CreateShader(const ShaderDesc &desc) = 0;

    /* Uniforms */

    /* Texture */
    virtual Shared<Texture> CreateTexture(const TextureDesc &desc) = 0;

    /* FrameBuffer */

    /* Render */
    virtual void SetPipelineState(const PipelineState &state) = 0;
    virtual void DrawIndexed(const DrawIndexedDesc &desc) = 0;

    virtual void SetClearColor(const glm::vec4 &color) = 0;
    virtual void Clear() = 0;
    virtual void Present() = 0;
};
}

#endif // TONIC_GRAPHICS_DEVICE_H