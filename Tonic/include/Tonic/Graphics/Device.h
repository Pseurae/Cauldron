#ifndef TONIC_GRAPHICS_DEVICE_H
#define TONIC_GRAPHICS_DEVICE_H

#include <Ethyl/Pointers.h>
#include "Tonic/Graphics/Layout.h"

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

struct FrameBufferDesc;
class FrameBuffer;

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
    ~Device();

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
    [[nodiscard]] inline Ethyl::Shared<Buffer> CreateBufferFromStruct(const T &arr, BufferRole role)
    {
        return CreateBuffer(std::span<const unsigned char>{ (const unsigned char *)&arr, sizeof(T) }, role);
    }

    [[nodiscard]] Ethyl::Shared<Buffer> CreateBuffer(std::span<const unsigned char> data, BufferRole role); // Static
    [[nodiscard]] Ethyl::Shared<Buffer> CreateBuffer(unsigned int size, BufferRole role); // Dynamic

    /* Shaders */
    [[nodiscard]] Ethyl::Shared<Shader> CreateShader(const ShaderDesc &desc);

    /* Texture */
    [[nodiscard]] Ethyl::Shared<Texture> CreateTexture(const TextureDesc &desc);
    void SetTextures(const std::vector<Ethyl::Shared<Texture>> &textures);

    /* FrameBuffer */
    [[nodiscard]] Ethyl::Shared<FrameBuffer> CreateFrameBuffer(const FrameBufferDesc &desc);

    /* Render */
    void SetRenderTarget(const Ethyl::Shared<FrameBuffer> &fb);
    void SetViewport(const glm::ivec4 &viewport);
    void SetPipeline(const Pipeline &pipeline);
    void DrawIndexed(const DrawIndexedDesc &desc);

    void SetClearColor(const glm::vec4 &color);
    void Clear();
    void Present();

    Window &GetWindow() { return m_Window; }

private:
    Window &m_Window;
    unsigned int m_VertexArray;
    Layout m_Layout;
};
}

#endif // TONIC_GRAPHICS_DEVICE_H