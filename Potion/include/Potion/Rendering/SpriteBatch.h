#ifndef POTION_RENDERING_SPRITEBATCH_H
#define POTION_RENDERING_SPRITEBATCH_H

#include <Tonic/Graphics/Device.h>
#include <Tonic/Graphics/Buffer.h>
#include <Tonic/Graphics/Resource.h>

#include "Potion/Rendering/Camera.h"

namespace Potion::Rendering
{
class SpriteBatch final : public Tonic::Graphics::Resource
{
public:
    enum class BatchSorting
    {
        Immediate
    };

    SpriteBatch(Tonic::Graphics::Device &device, int maxBatches);
    void Begin(const Camera &, BatchSorting);
    void End();

    struct BatchItem
    {
        Ethyl::Shared<Tonic::Graphics::Texture> texture;
    };

    struct QuadVertex
    {
        glm::vec2 position;
        glm::vec2 size;
        glm::vec4 color;
    };

    void DrawQuad(const glm::vec3 &position, const glm::vec2 &size);

private:
    void FlushIfNeeded();
    void DoBatch();

    int m_MaxBatches;
    bool m_StartedBatch = false;

    BatchSorting m_BatchSorting;

    struct Uniforms
    {
        glm::mat4 ViewProjection;
    } m_Uniforms;

    Ethyl::Shared<Tonic::Graphics::Buffer> m_UniformBuffer;
    Ethyl::Shared<Tonic::Graphics::Buffer> m_Vertices;
    Ethyl::Shared<Tonic::Graphics::Buffer> m_Indices;
    std::vector<BatchItem> m_Batches;

    Ethyl::Shared<Tonic::Graphics::Texture> m_CurrentTex;
};
}

#endif // POTION_RENDERING_SPRITEBATCH_H