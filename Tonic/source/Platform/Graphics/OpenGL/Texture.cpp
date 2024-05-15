#include "Tonic/Platform/Graphics/OpenGL/Texture.h"
#include <GL/gl3w.h>

namespace Tonic::Graphics::OpenGL
{
static int getWrapMode(TextureWrapMode mode)
{
    switch (mode)
    {
    case TextureWrapMode::Repeat:
        return GL_REPEAT;
    case TextureWrapMode::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrapMode::ClampEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrapMode::ClampBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return GL_CLAMP_TO_BORDER;
}

static int getFilterType(TextureFilterType type)
{
    switch (type)
    {
    case TextureFilterType::Nearest:
        return GL_NEAREST;
    case TextureFilterType::Linear:
        return GL_LINEAR;
    }

    return GL_LINEAR;
}

OGLTexture::OGLTexture(Device &device, const TextureDesc &desc) : Texture(device)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    unsigned int format = desc.numChannels == 3 ? GL_RGB : GL_RGBA; 
    glTexImage2D(GL_TEXTURE_2D, 0, format, desc.width, desc.height, 0, format, GL_UNSIGNED_BYTE, desc.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapMode(desc.wrapMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapMode(desc.wrapMode));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getFilterType(desc.filterType));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getFilterType(desc.filterType));
}
}