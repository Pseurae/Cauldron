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
    
    void SetKeyCallback(KeyCallback callback) { mKeyCallback = callback; }
    void SetMouseButtonCallback(MouseButtonCallback callback) { mMouseButtonCallback = callback; }
    void SetCloseCallback(CloseCallback callback) { mCloseCallback = callback; }

    GLFWwindow *GetNativeHandle() const { return mWindow; }
    glm::vec2 GetWindowSize() const { return mWindowSize; }

private:
    GLFWwindow *mWindow;

    KeyCallback mKeyCallback = nullptr;
    MouseButtonCallback mMouseButtonCallback = nullptr;
    CloseCallback mCloseCallback = nullptr;

    glm::vec2 mWindowSize;
};
}

#endif // TONIC_WINDOW_H