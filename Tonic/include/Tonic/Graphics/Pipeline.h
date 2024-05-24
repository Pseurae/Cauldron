#ifndef TONIC_GRAPHICS_PIPELINE_H
#define TONIC_GRAPHICS_PIPELINE_H

#include "Tonic/Common/Pointers.h"
#include "Tonic/Graphics/Blend.h"
#include "Tonic/Graphics/Layout.h"
#include "Tonic/Graphics/Shader.h"
#include "Tonic/Graphics/Buffer.h"
#include "Tonic/Graphics/Texture.h"

namespace Tonic::Graphics
{
struct Pipeline final
{
    const Shared<Shader> shader;
    const Layout vertexLayout;
    const Shared<Buffer> uniforms = nullptr;
    const BlendState blendState = BlendState::AlphaBlend;
};
}

#endif // TONIC_GRAPHICS_PIPELINE_H