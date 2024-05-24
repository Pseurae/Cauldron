#include "Tonic/Core/Window.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Tonic::Core
{
static Input::Key convertFromGLFWKey(int key)
{
    return static_cast<Input::Key>(key);
}

static Input::MouseButton convertFromGLFWMouseButton(int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        return Input::MouseButton::Left;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return Input::MouseButton::Middle;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return Input::MouseButton::Right;
    default:
        throw std::runtime_error("Invalid mousebutton received.");
    }
}

static Input::Action convertFromGLFWAction(int action)
{
    if (action == GLFW_PRESS) return Input::Action::Press;
    else return Input::Action::Release;
}

static Input::KeyMod convertFromGLFWKeyMod(int mods)
{
    return Input::KeyMod{
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

    m_Window = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);
    if (m_Window == nullptr) return false;

    glfwSetWindowUserPointer(m_Window, this);

    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int, int action, int mods) {
        auto win = static_cast<Tonic::Core::Window *>(glfwGetWindowUserPointer(window));
        if (win->m_KeyCallback && action != GLFW_REPEAT) win->m_KeyCallback(convertFromGLFWKey(key), convertFromGLFWAction(action), convertFromGLFWKeyMod(mods));
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
        auto win = static_cast<Tonic::Core::Window *>(glfwGetWindowUserPointer(window));
        if (win->m_CloseCallback) win->m_CloseCallback();
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
        auto win = static_cast<Tonic::Core::Window *>(glfwGetWindowUserPointer(window));
        if (win->m_MouseButtonCallback && button >= GLFW_MOUSE_BUTTON_LEFT && button <= GLFW_MOUSE_BUTTON_MIDDLE) 
            win->m_MouseButtonCallback(convertFromGLFWMouseButton(button), convertFromGLFWAction(action), convertFromGLFWKeyMod(mods));
    });

    if (createGLContext)
    {
        glfwMakeContextCurrent(m_Window);   
        glfwSwapInterval(1);
    }

    return true;
}

void Window::ShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(m_Window, shouldClose);
}

void Window::PumpEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(m_Window);
}
}