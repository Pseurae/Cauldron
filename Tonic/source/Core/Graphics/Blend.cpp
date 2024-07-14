#include "Tonic/Graphics/Blend.h"

namespace Tonic::Graphics
{
BlendState BlendState::AlphaBlend = BlendState();

BlendState::BlendState(BlendFactor source, BlendFactor destination, BlendFunction function) :
    SourceColor(source), SourceAlpha(source), DestinationColor(destination), DestinationAlpha(destination), ColorFunction(function), AlphaFunction(function)
{}

BlendState::BlendState(BlendFactor sourceColor, BlendFactor destinationColor, BlendFactor sourceAlpha, BlendFactor destinationAlpha, BlendFunction colorFunction, BlendFunction alphaFunction) :
    SourceColor(sourceColor), DestinationColor(destinationColor), SourceAlpha(sourceAlpha), DestinationAlpha(destinationAlpha), ColorFunction(colorFunction), AlphaFunction(alphaFunction)
{}
}