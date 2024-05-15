#ifndef TONIC_GRAPHICS_LAYOUT_H
#define TONIC_GRAPHICS_LAYOUT_H

#include <vector>

namespace Tonic::Graphics
{
enum class DataType
{
    Byte,
    UnsignedByte,

    Short,
    UnsignedShort,

    Int,
    UnsignedInt,

    Float,
};

struct VertexElement
{
    DataType type;
    unsigned int count;
    unsigned int normalized;
};

using Layout = std::vector<VertexElement>;
}

#endif // TONIC_GRAPHICS_LAYOUT_H
