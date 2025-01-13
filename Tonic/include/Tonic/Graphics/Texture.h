#ifndef TONIC_GRAPHICS_TEXTURE_H
#define TONIC_GRAPHICS_TEXTURE_H

#include "Tonic/Graphics/Resource.h"
#include <glm/vec2.hpp>

namespace Tonic::Graphics
{
enum class TextureWrapMode
{
    Repeat,
    MirroredRepeat,
    ClampEdge,
    ClampBorder
};

enum class TextureFilterType
{
    Nearest,
    Linear,
};

struct TextureDesc
{
    const unsigned char *data;
    int width, height;
    int numChannels;

    TextureWrapMode wrapMode = TextureWrapMode::ClampBorder;
    TextureFilterType filterType = TextureFilterType::Nearest;
    bool yFlip = false;
};

struct TextureSubDesc
{
    const void *data;
    int xoffset, yoffset; 
    int width, height;
};

class Texture : public Resource
{
public:
    Texture(Device &device, const TextureDesc &desc);
    ~Texture();

    void SetData(const TextureSubDesc &desc);

    const auto GetID() const { return mTextureID; }
    const auto GetSize() const { return mSize; }
    const bool GetYFlip() const { return mYFlip; }
private:
    bool mYFlip;
    glm::vec2 mSize;
    unsigned int mTextureID;
    unsigned int mFormat;
};
}

#endif // TONIC_GRAPHICS_TEXTURE_H