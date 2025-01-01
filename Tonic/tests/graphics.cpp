#include <Tonic/Window.h>
#include <Tonic/Graphics/Buffer.h>
#include <Tonic/Graphics/Shader.h>
#include <Ethyl/Pointers.h>
#include <Tonic/Graphics/Draw.h>
#include <Tonic/Graphics/Blend.h>
#include <Tonic/Graphics/Pipeline.h>
#include <Tonic/Graphics/Texture.h>
#include <Tonic/Graphics/Device.h>

#include <Ethyl/Pointers.h>

#include <Tonic/FileSystem/VFS/Drive.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>

static const float VertexBufferData[] = {
    -0.5f, -0.5f, 0.0, 0.0,
    -0.5f,  0.5f, 0.0, 1.0,
     0.5f, -0.5f, 1.0, 0.0,
     0.5f,  0.5f, 1.0, 1.0,
};

using namespace Tonic::Graphics;

static const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

out vec2 vUV;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    vUV = aUv;
}
)";

static const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 vUV;

layout (std140) uniform Uniforms {
    vec4 uColor;
};

uniform sampler2D tex0;

void main()
{
    FragColor = texture2D(tex0, vUV) * uColor;
} 
)";

static const unsigned int indices[] = {
    0, 1, 2,
    2, 1, 3
};

struct Uniforms
{
    glm::vec4 color;
};

#include <iostream>

int main(int argc, char* const argv[])
{
    bool isRunning = true;


    Tonic::Window window;

    window.SetCloseCallback([&]() { isRunning = false; });
    window.Create({ "Test", 800, 600 });

    Tonic::Graphics::Device device(window);

    const unsigned char textureData[] = { 
        255, 255, 255, 255, 
          0,   0,   0, 255, 
          0,   0,   0, 255,
        255, 255, 255, 255, 
    };
    auto textureDesc = TextureDesc{ textureData, 2, 2, 4, TextureWrapMode::ClampBorder, TextureFilterType::Nearest };

    const Uniforms uboData = { glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };

    auto vbo = device.CreateBufferFromArray(VertexBufferData, BufferRole::Vertex);
    auto ibo = device.CreateBufferFromArray(indices, BufferRole::Index);
    auto ubo = device.CreateBufferFromStruct(uboData, BufferRole::Uniform);

    auto texture = device.CreateTexture(textureDesc);
    auto shader = device.CreateShader({ vertexShaderSource, fragmentShaderSource, });
    device.SetViewport({0, 0, 800, 600});

    Layout vertexLayout = {
        { DataType::Float, 2, 0 },
        { DataType::Float, 2, 0 },
    };

    device.SetTextures({ texture });
    device.SetPipeline({ shader, vertexLayout, ubo });
    device.SetClearColor({ 1.0, 1.0, 1.0, 1.0 });
    device.Clear();

    device.DrawIndexed({ DrawMode::Triangles, vbo, ibo, sizeof(indices) / sizeof(*indices), 0, IndexElementType::Int });
    device.Present();

    while (isRunning)
        window.PumpEvents();

    return 0;
}