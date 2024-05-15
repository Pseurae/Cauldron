#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_DEVICE_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_DEVICE_H

#include "Tonic/Graphics/Device.h"
#include "Tonic/Graphics/Layout.h"

namespace Tonic::Graphics::OpenGL
{
class OGLDevice : public Device
{
public:
    OGLDevice(Core::Window &window);
    ~OGLDevice();

    /* Vertices */
    virtual Shared<Buffer> CreateBuffer(std::span<const unsigned char> data, BufferRole role) override; // Static
    virtual Shared<Buffer> CreateBuffer(unsigned int size, BufferRole role) override; // Dynamic

    /* Shaders */
    virtual Shared<Shader> CreateShader(const ShaderDesc &desc) override;

    virtual Shared<Texture> CreateTexture(const TextureDesc &desc) override { return nullptr; }

    /* Render */
    virtual void SetPipelineState(const PipelineState &state) override;
    virtual void DrawIndexed(const DrawIndexedDesc &desc) override;

    virtual void SetClearColor(const glm::vec4 &color) override;
    virtual void Clear() override;
    virtual void Present() override;
private:
    unsigned int m_VertexArray;
    Layout m_Layout;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_DEVICE_H