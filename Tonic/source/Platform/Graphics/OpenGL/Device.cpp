#include "Tonic/Platform/Graphics/OpenGL/Device.h"
#include "Tonic/Platform/Graphics/OpenGL/Buffer.h"
#include "Tonic/Platform/Graphics/OpenGL/Shader.h"

#include "Tonic/Graphics/Draw.h"
#include "Tonic/Graphics/PipelineState.h"

#include <GL/gl3w.h>

#include <numeric>

namespace Tonic::Graphics::OpenGL
{
OGLDevice::OGLDevice(Core::Window& window) : Device(window)
{
    gl3wInit();

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);
}

OGLDevice::~OGLDevice()
{
    glDeleteVertexArrays(1, &m_VertexArray);
}

Shared<Buffer> OGLDevice::CreateBuffer(std::span<const unsigned char> data, BufferRole role)
{
    return CreateShared<OGLBuffer>(*this, role, data);
}

Shared<Buffer> OGLDevice::CreateBuffer(unsigned int size, BufferRole role)
{
    return CreateShared<OGLBuffer>(*this, role, size);
}

Shared<Shader> OGLDevice::CreateShader(const ShaderDesc &desc)
{
    return CreateShared<OGLShader>(*this, desc);
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

    return GL_UNSIGNED_INT;
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

    return GL_UNSIGNED_INT;
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

    return GL_FUNC_ADD;
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
    return GL_SRC_COLOR;
}


void OGLDevice::SetPipelineState(const PipelineState &state)
{
    OGLShader *shader = static_cast<OGLShader *>(state.shader.get());
    if (shader)
    {
        glUseProgram(shader->GetID());
        m_Layout = shader->GetLayout();

        // const auto &layout = shader->GetLayout();
        // unsigned int size = std::accumulate(layout.begin(), layout.end(), 0u, [](unsigned int last, VertexElement el) { return getGLSize(el.type) * el.count + last; });

        // uintptr_t stride = 0;
        // for (unsigned int i = 0; i < layout.size(); ++i)
        // {
        //     const auto &el = layout[i];

        //     glEnableVertexAttribArray(i);
        //     glVertexAttribPointer(i, el.count, getGLType(el.type), el.normalized, size, (const void *)stride);

        //     stride += getGLSize(el.type) * el.count;
        // }
    }

    if (state.blendState)
    {
        if (state.blendState->isBlendingEnabled) glEnable(GL_BLEND);
        glBlendEquationSeparate(getGLBlendFunction(state.blendState->rgbFunc), getGLBlendFunction(state.blendState->alphaFunc));
        glBlendFuncSeparate(
            getGLBlendFactor(state.blendState->sourceRGB), 
            getGLBlendFactor(state.blendState->destinationRGB), 
            getGLBlendFactor(state.blendState->sourceAlpha), 
            getGLBlendFactor(state.blendState->destinationAlpha)
        );
    }
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

    return GL_UNSIGNED_INT;
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

    return 4;
}

void OGLDevice::DrawIndexed(const DrawIndexedDesc &desc)
{
    OGLBuffer *vertices = static_cast<OGLBuffer *>(desc.vertices.get());
    OGLBuffer *indices = static_cast<OGLBuffer *>(desc.indices.get());

    glBindBuffer(GL_ARRAY_BUFFER, vertices->GetID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->GetID());

    {
        unsigned int size = std::accumulate(m_Layout.begin(), m_Layout.end(), 0u, [](unsigned int last, VertexElement el) { return getGLSize(el.type) * el.count + last; });

        uintptr_t stride = 0;
        for (unsigned int i = 0; i < m_Layout.size(); ++i)
        {
            const auto &el = m_Layout[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, el.count, getGLType(el.type), el.normalized, size, (const void *)stride);

            stride += getGLSize(el.type) * el.count;
        }
    }

    glDrawElements(GL_TRIANGLES, desc.count, getGLTypeForIndexElement(desc.type), ((const void *)((uintptr_t)desc.start * getGLSizeForIndexElement(desc.type))));
}

void OGLDevice::SetClearColor(const glm::vec4 &color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OGLDevice::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OGLDevice::Present()
{
}
}