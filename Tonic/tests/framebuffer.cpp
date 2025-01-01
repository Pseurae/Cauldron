#include <Tonic/Graphics/Window.h>
#include <Tonic/Input/Keyboard.h>
#include <Tonic/Input/Mouse.h>
#include <Tonic/Graphics/Buffer.h>
#include <Tonic/Graphics/Shader.h>
#include <Ethyl/Pointers.h>
#include <Tonic/Graphics/Draw.h>
#include <Tonic/Graphics/Blend.h>
#include <Tonic/Graphics/Pipeline.h>
#include <Tonic/Graphics/Texture.h>
#include <Tonic/Graphics/FrameBuffer.h>
#include <Tonic/Graphics/Device.h>

#include <Ethyl/Pointers.h>

#include <Tonic/FileSystem/VFS/Drive.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>

#include <iostream>

static const float VertexBufferData[] = {
    -0.5f, -0.5f, 0.0, 0.0,
    -0.5f,  0.5f, 0.0, 1.0,
     0.5f, -0.5f, 1.0, 0.0,
     0.5f,  0.5f, 1.0, 1.0,
};


static const float VertexBufferData1[] = {
    -1.0f, -1.0f, 0.0, 0.0,
    -1.0f,  1.0f, 0.0, 1.0,
     1.0f, -1.0f, 1.0, 0.0,
     1.0f,  1.0f, 1.0, 1.0,
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

uniform sampler2D tex0;

void main()
{
    vec4 color = texture2D(tex0, vUV) * vec4(1.0f, 0.8f, 0.8f, 1.0f);
    FragColor = vec4(1.0f - color.rgb, color.a);
} 
)";

static const std::string fragmentShaderSource1 = R"(
#version 330 core
out vec4 FragColor;
in vec2 vUV;

uniform sampler2D tex0;

void main()
{
    vec4 color = texture2D(tex0, vUV) * vec4(0.8f, 1.0f, 0.8f, 1.0f);
    FragColor = vec4(1.0f - color.rbg, color.a);
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
    try {
        bool isRunning = true;

        Tonic::Graphics::Window window;

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
        auto vbo1 = device.CreateBufferFromArray(VertexBufferData1, BufferRole::Vertex);
        auto ibo = device.CreateBufferFromArray(indices, BufferRole::Index);

        auto offscreenTexture = device.CreateTexture({ nullptr, 800, 600, 4 });

        auto fb = device.CreateFrameBuffer({ offscreenTexture, nullptr });

        auto texture = device.CreateTexture(textureDesc);
        auto shader = device.CreateShader({ vertexShaderSource, fragmentShaderSource });
        auto shader1 = device.CreateShader({ vertexShaderSource, fragmentShaderSource1 });
        device.SetViewport({0, 0, 800, 600});

        Layout vertexLayout = {
            { DataType::Float, 2, 0 },
            { DataType::Float, 2, 0 },
        };

        device.SetRenderTarget(fb);

        device.SetViewport({0, 0, 800, 600});

        device.SetClearColor({ 0.0, 0.0, 0.0, 0.0 });
        device.Clear();
        device.SetTextures({ texture });
        device.SetPipeline({ shader1, vertexLayout });
        device.DrawIndexed({ DrawMode::Triangles, vbo1, ibo, sizeof(indices) / sizeof(*indices), 0, IndexElementType::Int });

        device.SetRenderTarget(nullptr);

        device.SetClearColor({ 0.5, 0.5, 0.5, 1.0 });
        device.Clear();
        device.SetTextures({ offscreenTexture });
        device.SetPipeline({ shader, vertexLayout });
        device.DrawIndexed({ DrawMode::Triangles, vbo, ibo, sizeof(indices) / sizeof(*indices), 0, IndexElementType::Int });

        device.Present();

        while (isRunning)
        {
            window.PumpEvents();
        }
    } catch (std::runtime_error &e) { std::cout << e.what() << std::endl; }
    return 0;
}