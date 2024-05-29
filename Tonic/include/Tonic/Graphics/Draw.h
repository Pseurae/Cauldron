#ifndef TONIC_GRAPHICS_DRAW_H
#define TONIC_GRAPHICS_DRAW_H

#include <Ethyl/Pointers.h>

namespace Tonic::Graphics
{
class Buffer;

enum class DrawMode
{
    Triangles
};

enum class IndexElementType
{
    Byte,
    Short,
    Int
};

struct DrawIndexedDesc final
{
    DrawMode mode;
    Ethyl::Shared<Buffer> &vertices, &indices;
    unsigned int count, start;
    IndexElementType type;
};
}

#endif // TONIC_GRAPHICS_DRAW_H