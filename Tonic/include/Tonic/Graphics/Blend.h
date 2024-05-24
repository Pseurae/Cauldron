#ifndef TONIC_GRAPHICS_BLEND_H
#define TONIC_GRAPHICS_BLEND_H

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

struct BlendState final
{
    static BlendState AlphaBlend;

    BlendState() = default;
    BlendState(BlendFactor source, BlendFactor destination, BlendFunction function);
    BlendState(BlendFactor sourceColor, BlendFactor destinationColor, BlendFactor sourceAlpha, BlendFactor destinationAlpha, BlendFunction colorFunction, BlendFunction alphaFunction);

    const BlendFactor SourceColor = BlendFactor::SourceAlpha;
    const BlendFactor DestinationColor = BlendFactor::OneMinusSourceAlpha;
    const BlendFactor SourceAlpha = BlendFactor::SourceAlpha;
    const BlendFactor DestinationAlpha = BlendFactor::OneMinusSourceAlpha;

    const BlendFunction ColorFunction =  BlendFunction::Add;
    const BlendFunction AlphaFunction = BlendFunction::Add;
};
}

#endif // TONIC_GRAPHICS_BLEND_H