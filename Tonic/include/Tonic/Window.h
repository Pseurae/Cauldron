#ifndef TONIC_WINDOW_H
#define TONIC_WINDOW_H

#include "Tonic/Keys.h"

#include <string>
#include <functional>
#include <glm/vec2.hpp>

struct GLFWwindow;

namespace Tonic
{
struct WindowDescriptor
{
    std::string title;
    int width, height;
};

class Window final
{
private:
    using KeyCallback = std::function<void(Key key, Action action, KeyMod mods)>;
    using MouseButtonCallback = std::function<void(MouseButton button, Action action, KeyMod mods)>;
    using CloseCallback = std::function<void()>;

public:
    Window() = default;
    ~Window() = default;

    bool Create(const WindowDescriptor &desc, bool createGLContext = true);
    void Close();
    void ShouldClose(bool shouldClose);

    void PumpEvents();
    void SwapBuffers() const;
    
    void SetKeyCallback(KeyCallback callback) { m_KeyCallback = callback; }
    void SetMouseButtonCallback(MouseButtonCallback callback) { m_MouseButtonCallback = callback; }
    void SetCloseCallback(CloseCallback callback) { m_CloseCallback = callback; }

    GLFWwindow *GetNativeHandle() const { return m_Window; }
    glm::vec2 GetWindowSize() const { return m_WindowSize; }

private:
    GLFWwindow *m_Window;

    KeyCallback m_KeyCallback = nullptr;
    MouseButtonCallback m_MouseButtonCallback = nullptr;
    CloseCallback m_CloseCallback = nullptr;

    glm::vec2 m_WindowSize;
};
}

#endif // TONIC_WINDOW_H