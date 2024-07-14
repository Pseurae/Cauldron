#ifndef TONIC_PLATFORM_GRAPHICS_OPENGL_SHADER_H
#define TONIC_PLATFORM_GRAPHICS_OPENGL_SHADER_H

#include "Tonic/Platform/Graphics/OpenGL/Device.h"
#include "Tonic/Graphics/Shader.h"

namespace Tonic::Graphics::OpenGL
{
class OGLShader : public Shader
{
public:
    OGLShader(OGLDevice &device, const ShaderDesc &desc);
    ~OGLShader();

    auto GetID() const { return m_ProgramID; }
private:
    unsigned int m_ProgramID;
};
}

#endif // TONIC_PLATFORM_GRAPHICS_OPENGL_SHADER_H