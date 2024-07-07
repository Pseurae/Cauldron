#include "Potion/Rendering/SpriteBatch.h"
#include <Ethyl/Assert.h>
#include <Tonic/Graphics/Draw.h>

namespace Potion::Rendering
{
SpriteBatch::SpriteBatch(Tonic::Graphics::Device &device, int maxBatches) : Resource(device), m_MaxBatches(maxBatches)
{
    int *indices = new int[maxBatches * 6];
    m_Indices = device.CreateBuffer({ (unsigned char *)indices, sizeof(int) * maxBatches * 6 }, Tonic::Graphics::BufferRole::Index);
    m_Vertices = device.CreateBuffer(0, Tonic::Graphics::BufferRole::Vertex);
    m_UniformBuffer = device.CreateBuffer(sizeof(Uniforms), Tonic::Graphics::BufferRole::Uniform);
}

void SpriteBatch::Begin(const Camera &camera, BatchSorting sort)
{
    ETHYL_ASSERT(!m_StartedBatch, "A batch has already been started!");

    m_Uniforms.ViewProjection = camera.GetViewProjectionMatrix();
    m_UniformBuffer->SetSubData({ (const unsigned char *)&m_Uniforms, sizeof(Uniforms) }, 0);

    m_BatchSorting = sort;

    m_Batches.clear();

    m_StartedBatch = true;
}

void SpriteBatch::DrawQuad(const glm::vec3 &position, const glm::vec2 &size)
{}

void SpriteBatch::End()
{
    ETHYL_ASSERT(m_StartedBatch, "No batch has been started!");
    m_StartedBatch = false;
}
}