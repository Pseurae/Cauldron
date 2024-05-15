#ifndef TONIC_GRAPHICS_BLENDSTATE_H
#define TONIC_GRAPHICS_BLENDSTATE_H

namespace Tonic::Graphics
{
enum class BlendFactor
{
    Zero,
    One,

    SourceColor,
    OneMinusSourceColor,

    SourceAlpha,
    OneMinusSourceAlpha,

    DestinationColor,
    OneMinusDestinationColor,

    DestinationAlpha,
    OneMinusDestinationAlpha,

    SourceAlphaSaturated
};

enum class BlendFunction
{
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

struct BlendState
{
    bool isBlendingEnabled = true;

    BlendFactor sourceRGB;
    BlendFactor destinationRGB;
    BlendFactor sourceAlpha;
    BlendFactor destinationAlpha;

    BlendFunction rgbFunc;
    BlendFunction alphaFunc;
};
}

#endif // TONIC_GRAPHICS_BLENDSTATE_H