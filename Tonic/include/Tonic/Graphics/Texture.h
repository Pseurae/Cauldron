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
};

class Texture : public Resource
{
public:
    Texture(Device &device, const TextureDesc &desc);
    ~Texture();

    const auto GetID() const { return m_TextureID; }
    const auto GetSize() const { return m_Size; }
private:
    glm::vec2 m_Size;
    unsigned int m_TextureID;
};
}

#endif // TONIC_GRAPHICS_TEXTURE_H