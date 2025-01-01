#include "Tonic/Graphics/Shader.h"
#include <GL/gl3w.h>
#include <Ethyl/Assert.h>

namespace Tonic::Graphics
{
static void CheckShaderCompileErrors(unsigned int shader, bool isVertex)
{
    int hasError;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasError);

    if (hasError == GL_FALSE)
    {
        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(shader);

        ETHYL_BREAK("{} shader compilation failed: {}", isVertex ? "Vertex" : "Fragment", std::string_view(infoLog.data()));
    }
}

Shader::Shader(Device &device, const ShaderDesc &desc) : Resource(device)
{
    unsigned int vertex = 0, fragment = 0;
    const char *source = NULL;

    vertex = glCreateShader(GL_VERTEX_SHADER);    
    source = desc.vertex.c_str();
    glShaderSource(vertex, 1, &source, NULL);
    glCompileShader(vertex);

    CheckShaderCompileErrors(vertex, true);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    source = desc.fragment.c_str();
    glShaderSource(fragment, 1, &source, NULL);
    glCompileShader(fragment);

    CheckShaderCompileErrors(fragment, false);

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertex);
    glAttachShader(m_ProgramID, fragment);
    glLinkProgram(m_ProgramID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_ProgramID);
    m_ProgramID = 0;
}
}