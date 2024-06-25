#ifndef POTION_RENDERING_SPRITEBATCH_H
#define POTION_RENDERING_SPRITEBATCH_H

#include "Tonic/Graphics/Device.h"
#include "Tonic/Graphics/Buffer.h"

#include "Potion/Rendering/Camera.h"

namespace Potion::Rendering
{
class SpriteBatch final
{
public:
    enum class BatchSorting
    {
        Immediate
    };

    SpriteBatch(Tonic::Graphics::Device &device, int maxBatches);
    void Begin(Camera &, BatchSorting);
    void End();

    struct SpriteBatchItem
    {};

private:
    void FlushIfNeeded();

    int m_MaxBatches;
    bool m_StartedBatch = false;
    Ethyl::Shared<Tonic::Graphics::Buffer> m_Vertices;
    std::vector<SpriteBatchItem> m_Batches;
};
}

#endif // POTION_RENDERING_SPRITEBATCH_H