#include "Tonic/Window.h"
#include "Tonic/Graphics/Device.h"
#include "Tonic/Graphics/Texture.h"
#include "Tonic/Graphics/Blend.h"
#include "Tonic/Graphics/Pipeline.h"
#include "Tonic/Graphics/FrameBuffer.h"
#include "Tonic/Graphics/Shader.h"
#include "Tonic/Graphics/Draw.h"
#include <Ethyl/Assert.h>
#include <GL/gl3w.h>

#include <numeric>

namespace Tonic::Graphics
{
Device::Device(Window &window) : mWindow(window)
{
    if (gl3wInit()) throw ::std::runtime_error("Could not bind OpenGL functions!");

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    mPhysicalSize = GetWindow().GetWindowSize();
}

Device::~Device()
{
    glDeleteVertexArrays(1, &mVertexArray);
}

Ethyl::Shared<Buffer> Device::CreateBuffer(std::span<const unsigned char> data, BufferRole role)
{
    return Ethyl::CreateShared<Buffer>(*this, data, role);
}

Ethyl::Shared<Buffer> Device::CreateBuffer(unsigned int size, BufferRole role)
{
    return Ethyl::CreateShared<Buffer>(*this, size, role);
}

Ethyl::Shared<Shader> Device::CreateShader(const ShaderDesc &desc)
{
    return Ethyl::CreateShared<Shader>(*this, desc);
}

Ethyl::Shared<Texture> Device::CreateTexture(const TextureDesc &desc)
{
    return Ethyl::CreateShared<Texture>(*this, desc);
}

void Device::SetTextures(const std::vector<Ethyl::Shared<Texture>> &textures)
{
    for (auto i = 0; i < textures.size(); ++i)
    {
        const auto &texture = static_cast<Texture *>(textures[i].get());
        if (!texture) continue;

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());
    }
}

Ethyl::Shared<FrameBuffer> Device::CreateFrameBuffer(const FrameBufferDesc &desc)
{
    return Ethyl::CreateShared<FrameBuffer>(*this, desc);
}

void Device::SetViewport(const glm::vec4 &viewport)
{
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
    mPhysicalSize = { viewport.z - viewport.x, viewport.w - viewport.y };
}

void Device::SetRenderTarget(const Ethyl::Shared<FrameBuffer> &fb, bool clear)
{
    if (fb == nullptr)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        SetViewport({0, 0, GetWindow().GetWindowSize()});
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fb->GetID());
        SetViewport({0, 0, fb->GetViewportSize()});
        if (clear) Clear();
    }
}

static constexpr unsigned int getGLType(DataType type)
{
    switch (type)
    {
    case DataType::Byte:
        return GL_BYTE;
    case DataType::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case DataType::Short:
        return GL_SHORT;
    case DataType::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case DataType::Int:
        return GL_INT;
    case DataType::UnsignedInt:
        return GL_UNSIGNED_INT;
    case DataType::Float:
        return GL_FLOAT;
    }

    ETHYL_BREAK("Invalid DataType given.");
}

static constexpr unsigned int getGLSize(DataType type)
{
    switch (type)
    {
    case DataType::Byte:
    case DataType::UnsignedByte:
        return 1;
    case DataType::Short:
    case DataType::UnsignedShort:
        return 2;
    case DataType::Int:
    case DataType::UnsignedInt:
    case DataType::Float:
        return 4;
    }

    ETHYL_BREAK("Invalid DataType given.");
}

unsigned int getGLBlendFunction(BlendFunction func)
{
    switch (func)
    {
    case BlendFunction::Add:
        return GL_FUNC_ADD;
    case BlendFunction::Subtract:
        return GL_FUNC_SUBTRACT;
    case BlendFunction::ReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case BlendFunction::Min:
        return GL_MIN;
    case BlendFunction::Max:
        return GL_MAX;
    }

    ETHYL_BREAK("Invalid BlendFunction given.");
}

unsigned int getGLBlendFactor(BlendFactor factor)
{
    switch (factor)
    {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SourceColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSourceColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::SourceAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSourceAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DestinationColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDestinationColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::DestinationAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDestinationAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::SourceAlphaSaturated:
        return GL_SRC_ALPHA_SATURATE;
    }

    ETHYL_BREAK("Invalid BlendFactor given.");
}


void Device::SetPipeline(const Pipeline &pipeline)
{
    Shader *shader = static_cast<Shader *>(pipeline.shader.get());
    Buffer *uniformBuffer = static_cast<Buffer *>(pipeline.uniforms.get());

    if (shader)
    {
        auto shaderId = shader->GetID();
        glUseProgram(shaderId);

        if (uniformBuffer)
        {
            unsigned int index = glGetUniformBlockIndex(shaderId, "Uniforms");
            glUniformBlockBinding(shaderId, index, 1);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, uniformBuffer->GetID());
        }

        glUniform1i(glGetUniformLocation(shader->GetID(), "tex0"), 0);
    }

    mLayout = pipeline.vertexLayout;

    glBlendEquationSeparate(
        getGLBlendFunction(pipeline.blendState.ColorFunction), 
        getGLBlendFunction(pipeline.blendState.AlphaFunction)
    );

    glBlendFuncSeparate(
        getGLBlendFactor(pipeline.blendState.SourceColor), 
        getGLBlendFactor(pipeline.blendState.DestinationColor), 
        getGLBlendFactor(pipeline.blendState.SourceAlpha), 
        getGLBlendFactor(pipeline.blendState.DestinationAlpha)
    );
}

static constexpr unsigned int getGLTypeForIndexElement(IndexElementType type)
{
    switch (type)
    {
    case IndexElementType::Byte:
        return GL_UNSIGNED_BYTE;
    case IndexElementType::Short:
        return GL_UNSIGNED_SHORT;
    case IndexElementType::Int:
    default:
        return GL_UNSIGNED_INT;
    }

   ETHYL_BREAK("Invalid IndexElementType given.");
}

static constexpr unsigned int getGLSizeForIndexElement(IndexElementType type)
{
    switch (type)
    {
    case IndexElementType::Byte:
        return 1;
    case IndexElementType::Short:
        return 2;
    case IndexElementType::Int:
    default:
        return 4;
    }

    ETHYL_BREAK("Invalid IndexElementType given.");
}

void Device::DrawIndexed(const DrawIndexedDesc &desc)
{
    Buffer *vertices = static_cast<Buffer *>(desc.vertices.get());
    Buffer *indices = static_cast<Buffer *>(desc.indices.get());

    ETHYL_ASSERT(vertices, "DrawIndexed: vertices are null.");
    ETHYL_ASSERT(indices, "DrawIndexed: indices are null.");

    glBindBuffer(GL_ARRAY_BUFFER, vertices->GetID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->GetID());

    {
        unsigned int size = std::accumulate(mLayout.begin(), mLayout.end(), 0u, [](unsigned int last, VertexElement el) { return getGLSize(el.type) * el.count + last; });

        uintptr_t stride = 0;
        for (unsigned int i = 0; i < mLayout.size(); ++i)
        {
            const auto &el = mLayout[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, el.count, getGLType(el.type), el.normalized, size, (const void *)stride);

            stride += getGLSize(el.type) * el.count;
        }
    }

    glDrawElements(GL_TRIANGLES, desc.count, getGLTypeForIndexElement(desc.type), ((const void *)((uintptr_t)desc.start * getGLSizeForIndexElement(desc.type))));
}

void Device::SetClearColor(const glm::vec4 &color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Device::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Device::Present()
{
    GetWindow().SwapBuffers();
}
}