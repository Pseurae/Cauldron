#include "Tonic/Window.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <Ethyl/Assert.h>

namespace Tonic
{
static Key convertFromGLFWKey(int key)
{
    return static_cast<Key>(key);
}

static MouseButton convertFromGLFWMouseButton(int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return MouseButton::Middle;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return MouseButton::Right;
    }

    ETHYL_BREAK("Invalid mousebutton received.");
}

static Action convertFromGLFWAction(int action)
{
    if (action == GLFW_PRESS) return Action::Press;
    else return Action::Release;
}

static KeyMod convertFromGLFWKeyMod(int mods)
{
    return KeyMod{
        (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT,
        (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT,
        (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL,
        (mods & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK,
        (mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK,
    };
}

bool Window::Create(const WindowDescriptor &desc, bool createGLContext)
{
    if (glfwInit() == GLFW_FALSE) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (createGLContext) glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    else glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    mWindow = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);
    if (mWindow == nullptr) return false;

    mWindowSize = { desc.width, desc.height };

    glfwSetWindowUserPointer(mWindow, this);

    glfwSetKeyCallback(mWindow, [](GLFWwindow *window, int key, int, int action, int mods) {
        auto win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (win->mKeyCallback && action != GLFW_REPEAT) win->mKeyCallback(convertFromGLFWKey(key), convertFromGLFWAction(action), convertFromGLFWKeyMod(mods));
    });

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow *window) {
        auto win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (win->mCloseCallback) win->mCloseCallback();
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *window, int button, int action, int mods) {
        auto win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (win->mMouseButtonCallback && button >= GLFW_MOUSE_BUTTON_LEFT && button <= GLFW_MOUSE_BUTTON_MIDDLE) 
            win->mMouseButtonCallback(convertFromGLFWMouseButton(button), convertFromGLFWAction(action), convertFromGLFWKeyMod(mods));
    });

    if (createGLContext)
    {
        glfwMakeContextCurrent(mWindow);   
        glfwSwapInterval(1);
    }

    return true;
}

void Window::Close()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Window::ShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(mWindow, shouldClose);
}

void Window::PumpEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(mWindow);
}
}