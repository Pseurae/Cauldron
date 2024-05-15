#ifndef TONIC_GRAPHICS_DRAW_H
#define TONIC_GRAPHICS_DRAW_H

#include "Tonic/Common/Pointers.h"

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
    Shared<Buffer> &vertices, &indices;
    unsigned int count, start;
    IndexElementType type;
};
}

#endif // TONIC_GRAPHICS_DRAW_H