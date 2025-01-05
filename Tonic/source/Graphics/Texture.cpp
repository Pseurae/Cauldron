#include "Tonic/Graphics/Texture.h"
#include <GL/gl3w.h>
#include <Ethyl/Assert.h>

namespace Tonic::Graphics
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

    ETHYL_BREAK("Invalid TextureWrapMode given.");
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

    ETHYL_BREAK("Invalid TextureFilterType given.");
}

Texture::Texture(Device &device, const TextureDesc &desc) : Resource(device), mSize{desc.width, desc.height}, mYFlip{desc.yFlip}
{
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    unsigned int format = desc.numChannels == 3 ? GL_RGB : GL_RGBA; 

    if (desc.numChannels != 0)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, desc.width, desc.height, 0, format, GL_UNSIGNED_BYTE, desc.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapMode(desc.wrapMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapMode(desc.wrapMode));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getFilterType(desc.filterType));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getFilterType(desc.filterType));

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureID);
}
}