#ifndef TONIC_GRAPHICS_DEVICE_H
#define TONIC_GRAPHICS_DEVICE_H

#include <Ethyl/Pointers.h>

#include <vector>
#include <span>
#include <string>
#include <glm/vec4.hpp>

namespace Tonic::Graphics 
{
class Window;

class Buffer;
enum class BufferRole;

struct TextureDesc;
class Texture;

struct ShaderDesc;
class Shader;

struct Pipeline;

struct DrawIndexedDesc;

class Device 
{
public:
    struct Capabilities
    {
        int MaxTextureUnits;
        bool NPOTTexturesSupported:1;
    };

    explicit Device(Window &window);
    virtual ~Device() = default;

    /* Vertices */

    template<typename T, std::size_t N>
    [[nodiscard]] inline Ethyl::Shared<Buffer> CreateBufferFromSpan(std::span<const T, N> data, BufferRole role)
    {
        return CreateBuffer(std::span<const unsigned char>{ (const unsigned char *)data.data(), data.size_bytes() }, role);
    }

    template<typename T, std::size_t N>
    [[nodiscard]] inline Ethyl::Shared<Buffer> CreateBufferFromArray(const T (&arr)[N], BufferRole role)
    {
        return CreateBuffer(std::span<const unsigned char>{ (const unsigned char *)arr, N * sizeof(T) }, role);
    }

    template<typename T>
    [[nodiscard]] inline Ethyl::Shared<Buffer> CreateBufferFromType(const T &arr, BufferRole role)
    {
        return CreateBuffer(std::span<const unsigned char>{ (const unsigned char *)&arr, sizeof(T) }, role);
    }

    [[nodiscard]] virtual Ethyl::Shared<Buffer> CreateBuffer(std::span<const unsigned char> data, BufferRole role) = 0; // Static
    [[nodiscard]] virtual Ethyl::Shared<Buffer> CreateBuffer(unsigned int size, BufferRole role) = 0; // Dynamic

    /* Shaders */
    [[nodiscard]] virtual Ethyl::Shared<Shader> CreateShader(const ShaderDesc &desc) = 0;

    /* Texture */
    [[nodiscard]] virtual Ethyl::Shared<Texture> CreateTexture(const TextureDesc &desc) = 0;
    virtual void SetTextures(const std::vector<Ethyl::Shared<Texture>> &textures) = 0;

    /* FrameBuffer */

    /* Render */
    virtual void SetViewport(const glm::ivec4 &viewport) = 0;
    virtual void SetPipeline(const Pipeline &pipeline) = 0;
    virtual void DrawIndexed(const DrawIndexedDesc &desc) = 0;

    virtual void SetClearColor(const glm::vec4 &color) = 0;
    virtual void Clear() = 0;
    virtual void Present() = 0;

    Window &GetWindow() { return m_Window; }

private:
    Window &m_Window;
};
}

#endif // TONIC_GRAPHICS_DEVICE_H