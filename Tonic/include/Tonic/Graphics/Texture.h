#ifndef TONIC_GRAPHICS_TEXTURE_H
#define TONIC_GRAPHICS_TEXTURE_H

#include "Tonic/Graphics/Resource.h"

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
    unsigned int width, height;
    unsigned numChannels;

    TextureWrapMode wrapMode = TextureWrapMode::ClampBorder;
    TextureFilterType filterType = TextureFilterType::Nearest;
};

class Texture : public Resource
{
public:
    explicit Texture(Device &device) : Resource(device) {}
private:
};
}

#endif // TONIC_GRAPHICS_TEXTURE_H