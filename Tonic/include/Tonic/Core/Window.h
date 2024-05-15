#ifndef TONIC_CORE_WINDOW_H
#define TONIC_CORE_WINDOW_H

#include <string>
#include <functional>

struct GLFWwindow;

namespace Tonic::Core
{
struct WindowDescriptor
{
    std::string title;
    int width, height;
};

class Window final
{
private:
    using KeyCallback = std::function<void(int key, int action, int mods)>;
    using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
    using CloseCallback = std::function<void()>;

public:
    Window() = default;
    ~Window() = default;

    bool Create(const WindowDescriptor &desc, bool createGLContext = true);
    void ShouldClose(bool shouldClose);

    void PumpEvents();
    void SwapBuffer();
    
    void SetKeyCallback(KeyCallback callback) { m_KeyCallback = callback; }
    void SetMouseButtonCallback(MouseButtonCallback callback) { m_MouseButtonCallback = callback; }
    void SetCloseCallback(CloseCallback callback) { m_CloseCallback = callback; }

    GLFWwindow *GetNativeHandle() const { return m_Window; }

private:
    GLFWwindow *m_Window;

    KeyCallback m_KeyCallback = nullptr;
    MouseButtonCallback m_MouseButtonCallback = nullptr;
    CloseCallback m_CloseCallback = nullptr;
};
}

#endif // TONIC_CORE_WINDOW_H