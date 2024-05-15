#ifndef TONIC_GRAPHICS_PIPELINESTATE_H
#define TONIC_GRAPHICS_PIPELINESTATE_H

#include "Tonic/Common/Pointers.h"
#include "Tonic/Graphics/Layout.h"
#include "Tonic/Graphics/Shader.h"
#include "Tonic/Graphics/BlendState.h"

#include <optional>

namespace Tonic::Graphics
{
struct PipelineState final
{
    Shared<Shader> shader;
    std::optional<BlendState> blendState;
};
}

#endif // TONIC_GRAPHICS_PIPELINESTATE_H