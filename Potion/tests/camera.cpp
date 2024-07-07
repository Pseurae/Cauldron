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
#include <Tonic/Platform/Graphics/OpenGL/Device.h>

#include <Ethyl/Pointers.h>

#include <Tonic/FileSystem/VFS/Drive.h>

#include <Potion/Rendering/Camera.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

using namespace Tonic::Graphics;

static const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

layout (std140) uniform Uniforms {
    vec4 uColor;
    mat4 uViewProjectionMatrix;
};

out vec2 vUV;

void main()
{
    gl_Position = uViewProjectionMatrix * vec4(aPos, 0.0f, 1.0f);
    vUV = aUv;
}
)";

static const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 vUV;

layout (std140) uniform Uniforms {
    vec4 uColor;
    mat4 uViewProjectionMatrix;
};

uniform sampler2D tex0;

void main()
{
    FragColor = texture2D(tex0, vUV) * uColor;
} 
)";

// static const float VertexBufferData[] = {
//     -0.5f, -0.5f, 0.0, 0.0,
//     -0.5f,  0.5f, 0.0, 1.0,
//      0.5f, -0.5f, 1.0, 0.0,
//      0.5f,  0.5f, 1.0, 1.0,
// };

static const float VertexBufferData[] = {
    0.0f, 0.0f, 0.0, 0.0,
    0.0f, 600.0f, 0.0, 1.0,
    800.0f, 0.0f, 1.0, 0.0,
    800.0f, 600.0f, 1.0, 1.0,
};

static const unsigned int indices[] = {
    0, 1, 2,
    2, 1, 3
};

struct Uniforms
{
    glm::vec4 color;
    glm::mat4 ViewProjectionMatrix;
};

#include <iostream>

int main(int argc, char* const argv[])
{
    bool isRunning = true;

    Tonic::Input::Keyboard keyboard;
    Tonic::Input::Mouse mouse;
    Tonic::Graphics::Window window;

    window.SetCloseCallback([&]() { isRunning = false; });

    window.SetKeyCallback([&](Tonic::Input::Key key, Tonic::Input::Action action, Tonic::Input::KeyMod mods) {
        keyboard.Update(key, action, mods);
    });
    window.SetMouseButtonCallback([&](Tonic::Input::MouseButton button, Tonic::Input::Action action, Tonic::Input::KeyMod mods) {
        mouse.Update(button, action, mods);
    });
    window.Create({ "Test", 800, 600 });

    Ethyl::Unique<Tonic::Graphics::Device> device = Ethyl::CreateUnique<Tonic::Graphics::OpenGL::OGLDevice>(window);

    const unsigned char textureData[] = { 
        255, 255, 255, 255, 
          0,   0,   0, 255, 
          0,   0,   0, 255,
        255, 255, 255, 255, 
    };
    auto textureDesc = TextureDesc{ textureData, 2, 2, 4, TextureWrapMode::ClampBorder, TextureFilterType::Nearest };

    Potion::Rendering::Camera camera(*device, 800, 600);
    int x = 0, y = 0;

    Uniforms uboData = { glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), camera.GetViewProjectionMatrix() };

    auto vbo = device->CreateBufferFromArray(VertexBufferData, BufferRole::Vertex);
    auto ibo = device->CreateBufferFromArray(indices, BufferRole::Index);
    auto ubo = device->CreateBuffer(sizeof(Uniforms), BufferRole::Uniform);
    // ubo->SetSubData({ (unsigned char *)&uboData, sizeof(uboData) }, 0);

    auto texture = device->CreateTexture(textureDesc);
    auto shader = device->CreateShader({ vertexShaderSource, fragmentShaderSource, });

    Layout vertexLayout = {
        { DataType::Float, 2, 0 },
        { DataType::Float, 2, 0 },
    };

    device->SetTextures({ texture });
    device->SetPipeline({ shader, vertexLayout, ubo });
    device->SetClearColor({ 1.0, 1.0, 1.0, 1.0 });
    device->Clear();

    // device->DrawIndexed({ DrawMode::Triangles, vbo, ibo, sizeof(indices) / sizeof(*indices), 0, IndexElementType::Int });
    // device->Present();

    int i = 0;
    while (isRunning)
    {
        window.PumpEvents();
        device->Clear();

        if (keyboard.Pressed(Tonic::Input::Key::Left) || keyboard.Held(Tonic::Input::Key::Left))
        {
            x -= 5;
        }
        else if (keyboard.Pressed(Tonic::Input::Key::Right) || keyboard.Held(Tonic::Input::Key::Right))
        {
            x += 5;
        }

        if (keyboard.Pressed(Tonic::Input::Key::Up) || keyboard.Held(Tonic::Input::Key::Up))
        {
            y -= 5;
        }
        else if (keyboard.Pressed(Tonic::Input::Key::Down) || keyboard.Held(Tonic::Input::Key::Down))
        {
            y += 5;
        }

        camera.SetPosition(x, y);
        uboData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        ubo->SetSubData({ (unsigned char *)&uboData, sizeof(uboData) }, 0);

        device->DrawIndexed({ DrawMode::Triangles, vbo, ibo, sizeof(indices) / sizeof(*indices), 0, IndexElementType::Int });
        device->Present();
    }

    return 0;
}