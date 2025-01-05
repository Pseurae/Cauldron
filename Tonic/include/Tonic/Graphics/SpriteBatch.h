#ifndef TONIC_GRAPHICS_SPRITEBATCH_H
#define TONIC_GRAPHICS_SPRITEBATCH_H

#include <Tonic/Graphics/Resource.h>
#include <Ethyl/Pointers.h>
#include <Tonic/Graphics/Pipeline.h>
#include <Tonic/Graphics/FrameBuffer.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Tonic::Graphics
{
class Buffer;
class Pipeline;
class Shader;

class SpriteBatch : public Resource
{
public:
    explicit SpriteBatch(Device &device);
    void BeginScene(const Ethyl::Shared<FrameBuffer> &fb);
    void EndScene(void);
    void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ethyl::Shared<Texture> &texture, const glm::vec4 &crop = glm::vec4(0.0f));
    void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

private:
    struct Vertex
    {
        glm::vec4 position;
        glm::vec4 color;
        glm::vec2 texcoords;
    };

    struct Uniforms
    {
        glm::mat4 cameraMatrix;
    };

    void DrawQuad(const glm::mat4 &transform, const Ethyl::Shared<Texture> &texture, const glm::vec4 &color, const glm::vec4 &crop);
    void InitResources(void);
    void Flush(void);

    Ethyl::Shared<Buffer> mIndexBuffer, mVertexBuffer, mUniformBuffer;
    Ethyl::Shared<Shader> mShader;
    Ethyl::WeakRef<Texture> mTexture;
    Ethyl::Shared<Texture> mWhiteTexture;
    Layout mLayout;
    Vertex *mVertexDataBase, *mVertexDataPtr;
    Uniforms mUniforms;
    unsigned int mIndexCount;
    glm::vec2 mScreenSize;
};
}

#endif // TONIC_GRAPHICS_SPRITEBATCH_H