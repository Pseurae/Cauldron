set(TONIC_SOURCES 
    "Core/Window.cpp"
        
    "FileSystem/VFS/Drive.cpp"
    "FileSystem/VFS/NativeProvider.cpp"

    "Graphics/Blend.cpp"
    "Graphics/Device.cpp"

    "Input/Keyboard.cpp"
    "Input/Mouse.cpp"

    "Platform/Graphics/OpenGL/Buffer.cpp"
    "Platform/Graphics/OpenGL/Device.cpp"
    "Platform/Graphics/OpenGL/Shader.cpp"
    "Platform/Graphics/OpenGL/Texture.cpp"
)

set(TONIC_HEADERS
    "Tonic/Common/Assert.h"
    "Tonic/Common/Pointers.h"

    "Tonic/Core/Window.h"

    "Tonic/FileSystem/VFS/Drive.h"
    "Tonic/FileSystem/VFS/Provider.h"
    "Tonic/FileSystem/VFS/NativeProvider.h"

    "Tonic/Graphics/Blend.h"
    "Tonic/Graphics/Buffer.h"
    "Tonic/Graphics/Device.h"
    "Tonic/Graphics/Resource.h"
    "Tonic/Graphics/Shader.h"
    "Tonic/Graphics/Texture.h"

    "Tonic/Input/Keyboard.h"
    "Tonic/Input/Keys.h"
    "Tonic/Input/Mouse.h"

    "Tonic/Platform/Graphics/OpenGL/Buffer.h"
    "Tonic/Platform/Graphics/OpenGL/Device.h"
    "Tonic/Platform/Graphics/OpenGL/Shader.h"
    "Tonic/Platform/Graphics/OpenGL/Texture.h"
)