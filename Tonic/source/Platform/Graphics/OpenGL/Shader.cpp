#include "Tonic/Platform/Graphics/OpenGL/Shader.h"
#include <GL/gl3w.h>

namespace Tonic::Graphics::OpenGL
{
OGLShader::OGLShader(Device &device, const ShaderDesc &desc) : Shader(device, desc.vertexLayout)
{
    unsigned int vertex = 0, fragment = 0;
    const char *source = NULL;

    vertex = glCreateShader(GL_VERTEX_SHADER);    
    source = desc.vertex.c_str();
    glShaderSource(vertex, 1, &source, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    source = desc.fragment.c_str();
    glShaderSource(fragment, 1, &source, NULL);
    glCompileShader(fragment);

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertex);
    glAttachShader(m_ProgramID, fragment);
    glLinkProgram(m_ProgramID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
}