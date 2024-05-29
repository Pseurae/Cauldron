#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_DEVICE_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_DEVICE_H

#include "Tonic/Graphics/Device.h"
#include "Tonic/Graphics/Layout.h"

namespace Tonic::Graphics::OpenGL
{
class OGLDevice final : public Device
{
public:
    OGLDevice(const Window &window);
    ~OGLDevice();

    /* Vertices */
    Ethyl::Shared<Buffer> CreateBuffer(std::span<const unsigned char> data, BufferRole role) override; // Static
    Ethyl::Shared<Buffer> CreateBuffer(unsigned int size, BufferRole role) override; // Dynamic

    /* Shaders */
    Ethyl::Shared<Shader> CreateShader(const ShaderDesc &desc) override;

    /* Textures */
    Ethyl::Shared<Texture> CreateTexture(const TextureDesc &desc) override;
    void SetTextures(const std::vector<Ethyl::Shared<Texture>> &textures) override;

    /* Render */
    void SetViewport(const glm::ivec4 &view) override;
    void SetPipeline(const Pipeline &pipeline) override;
    void DrawIndexed(const DrawIndexedDesc &desc) override;

    void SetClearColor(const glm::vec4 &color) override;
    void Clear() override;
    void Present() override;

private:
    unsigned int m_VertexArray;

    Layout m_Layout;
    const Window &m_Window;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_DEVICE_H