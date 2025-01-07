#include "Tonic/Graphics/SpriteBatch.h"
#include "Tonic/Graphics/Device.h"
#include "Tonic/Graphics/Draw.h"
#include "Tonic/Window.h"
#include <glm/gtc/matrix_transform.hpp>

static const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec4 vColor;
out vec2 vTexCoords;

void main()
{
    gl_Position = aPosition;
    vColor = aColor;
    vTexCoords = aTexCoords;
}
)";

static const std::string fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoords;

uniform sampler2D tex0;

void main()
{
    FragColor = texture2D(tex0, vTexCoords) * vColor;
} 
)";

namespace Tonic::Graphics
{
static constexpr unsigned MaxQuads = 100;
static constexpr unsigned MaxVertices = MaxQuads * 4;
static constexpr unsigned MaxIndices = MaxQuads * 6;

SpriteBatch::SpriteBatch(Device &device) : Resource(device), mIndexCount(0)
{
    InitResources();
}

void SpriteBatch::BeginScene(void)
{
}

void SpriteBatch::EndScene(void)
{
    Flush();
}

static const glm::vec4 sTransformVectors[4] = {
    {  0.0f,  0.0f, 0.0f, 1.0f },
    {  1.0f,  0.0f, 0.0f, 1.0f },
    {  1.0f,  1.0f, 0.0f, 1.0f },
    {  0.0f,  1.0f, 0.0f, 1.0f },
};

void SpriteBatch::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
{
    glm::mat4 transform = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f) *
        glm::translate(glm::mat4(1.0f), { position / mDevice.GetPhysicalSize(), 0.0f }) *
        glm::scale(glm::mat4(1.0f), { size / mDevice.GetPhysicalSize(), 1.0f });

    DrawQuad(transform, mWhiteTexture, color, glm::vec4(0.0f));
}

void SpriteBatch::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ethyl::Shared<Texture> &texture, const glm::vec4 &crop)
{
    glm::mat4 transform = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f) *
        glm::translate(glm::mat4(1.0f), { position / mDevice.GetPhysicalSize(), 0.0f }) *
        glm::scale(glm::mat4(1.0f), { size / mDevice.GetPhysicalSize(), 1.0f });

    DrawQuad(transform, texture, glm::vec4(1.0f), crop);
}

void SpriteBatch::DrawQuad(const glm::mat4 &transform, const Ethyl::Shared<Texture> &texture, const glm::vec4 &color, const glm::vec4 &crop)
{
    if (mIndexCount >= MaxIndices)
        Flush();
    
    auto currentTexture = mTexture.lock();
    if (currentTexture != nullptr && currentTexture != texture)
        Flush();

    mTexture = texture;
    currentTexture = mTexture.lock();

    glm::vec2 texCoords[4];

    if (currentTexture->GetYFlip())
    {
        if (crop.x == crop.z || crop.y == crop.w)
        {
            texCoords[0] = { 0.0f, 1.0f };
            texCoords[1] = { 1.0f, 1.0f };
            texCoords[2] = { 1.0f, 0.0f };
            texCoords[3] = { 0.0f, 0.0f };
        }
        else
        {
            texCoords[0] = glm::vec2{ crop.x, crop.y } / currentTexture->GetSize();
            texCoords[1] = glm::vec2{ crop.z, crop.y } / currentTexture->GetSize();
            texCoords[2] = glm::vec2{ crop.z, crop.w } / currentTexture->GetSize();
            texCoords[3] = glm::vec2{ crop.x, crop.w } / currentTexture->GetSize();
        }
    }
    else
    {
        if (crop.x == crop.z || crop.y == crop.w)
        {
            texCoords[0] = { 0.0f, 0.0f };
            texCoords[1] = { 1.0f, 0.0f };
            texCoords[2] = { 1.0f, 1.0f };
            texCoords[3] = { 0.0f, 1.0f };
        }
        else
        {
            texCoords[0] = glm::vec2{ crop.x, crop.y } / currentTexture->GetSize();
            texCoords[1] = glm::vec2{ crop.z, crop.y } / currentTexture->GetSize();
            texCoords[2] = glm::vec2{ crop.z, crop.w } / currentTexture->GetSize();
            texCoords[3] = glm::vec2{ crop.x, crop.w } / currentTexture->GetSize();
        }
    }

    for (unsigned i = 0; i < 4; ++i)
    {
        mVertexDataPtr->position = transform * sTransformVectors[i];
        mVertexDataPtr->color = color;
        mVertexDataPtr->texcoords = texCoords[i];
        mVertexDataPtr++;
    }

    mIndexCount += 6;
}

void SpriteBatch::InitResources(void)
{
    auto quadIndices = new unsigned int[MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    mUniforms.cameraMatrix = glm::ortho(0.0f, 0.0f, 0.0f, 0.0f);

    mIndexBuffer = mDevice.CreateBufferFromSpan(std::span{quadIndices, MaxIndices * sizeof(unsigned int)}, BufferRole::Index);
    mVertexBuffer = mDevice.CreateBuffer(MaxVertices * sizeof(Vertex), BufferRole::Vertex);
    mUniformBuffer = mDevice.CreateBufferFromStruct(mUniforms, BufferRole::Uniform);
    mVertexDataBase = new Vertex[MaxVertices];
    mVertexDataPtr = mVertexDataBase;

    mShader = mDevice.CreateShader({ vertexShaderSource, fragmentShaderSource });
    mLayout = {
        { DataType::Float, 4, 0 },
        { DataType::Float, 4, 0 },
        { DataType::Float, 2, 0 },
    };

    const unsigned char textureData[] = { 
        255, 255, 255, 255,
    };

    mWhiteTexture = mDevice.CreateTexture({ textureData, 1, 1, 4 });
}

void SpriteBatch::Flush(void)
{
    if (mIndexCount == 0) 
        return;

    unsigned int dataSize = (unsigned int)((char *)mVertexDataPtr - (char *)mVertexDataBase);
    mVertexBuffer->SetData({ (unsigned char *)mVertexDataBase, dataSize });

    mDevice.SetPipeline({ mShader, mLayout, mUniformBuffer });
    mDevice.SetTextures({ mTexture.lock() });
    mDevice.DrawIndexed({ DrawMode::Triangles, mVertexBuffer, mIndexBuffer, mIndexCount, 0, IndexElementType::Int });

    mVertexDataPtr = mVertexDataBase;
    mIndexCount = 0;
}
}
