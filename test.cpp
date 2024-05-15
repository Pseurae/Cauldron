#include <Tonic/Core/Window.h>
#include <Tonic/Graphics/Buffer.h>
#include <Tonic/Graphics/Shader.h>
#include <Tonic/Graphics/PipelineState.h>
#include <Tonic/Graphics/Draw.h>
#include <Tonic/Platform/Graphics/OpenGL/Device.h>

#include <Tonic/FileSystem/VFS/Drive.h>
#include <Tonic/FileSystem/VFS/NativeProvider.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

static const float VertexBufferData[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 1.0f, 0.0f, 0.0f
};

using namespace Tonic::Graphics;

static const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    color = aColor;
}
)";

static const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 color;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 0.7f);
} 
)";

static const unsigned int indices[] = {
    0, 1, 2,
};  

#include <iostream>

int main(int argc, char* const argv[])
{
    bool isRunning = true;

    Tonic::FileSystem::VFS::Drive drive;
    drive.Mount("test", Tonic::CreateShared<Tonic::FileSystem::VFS::NativeProvider>("."));
    std::cout << drive.Exists("test/test.txt") << std::endl;

    if (auto file = drive.OpenRead("test/test.txt"))
    {
        char buffer[100];
        buffer[file->Read(buffer, 1, 100)] = '\0';

        std::cout << buffer << std::endl;
    }

    if (auto file = drive.OpenWrite("test/test1.txt"))
    {
        const char test[] = "TestFile1";
        file->Write(test, 1, sizeof(test) - 1);
    }

    Tonic::Core::Window window;
    window.SetCloseCallback([&]() { isRunning = false; });
    window.Create({ "Test", 800, 600 });

    Tonic::Graphics::OpenGL::OGLDevice device(window);

    auto vbo = device.CreateBufferGeneric(std::span<const float>(VertexBufferData), BufferRole::Vertex);
    auto ibo = device.CreateBufferGeneric(std::span<const unsigned int>(indices), BufferRole::Index);

    auto shader = device.CreateShader(ShaderDesc{ vertexShaderSource, fragmentShaderSource, {
        { DataType::Float, 2, 0 },
        { DataType::Float, 3, 0 },
    }});

    auto transparencyBlend = BlendState{ true, 
        BlendFactor::SourceAlpha,  BlendFactor::OneMinusSourceAlpha,
        BlendFactor::SourceAlpha, BlendFactor::OneMinusSourceAlpha,
        BlendFunction::Add, BlendFunction::Add
    };

    device.SetClearColor({ 1.0, 1.0, 1.0, 1.0 });
    device.SetPipelineState(PipelineState{ shader, transparencyBlend });

    while (isRunning)
    {
        device.Clear();

        device.DrawIndexed({ DrawMode::Triangles, vbo, ibo, 3, 0, IndexElementType::Int });
        window.SwapBuffer();
        device.Present();

        window.PumpEvents();
    }
    return 0;
}