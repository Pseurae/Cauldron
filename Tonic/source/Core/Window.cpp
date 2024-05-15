#include "Tonic/Core/Window.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Tonic::Core
{
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
        if (win->m_KeyCallback != nullptr) win->m_KeyCallback(key, action, mods);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
        auto win = static_cast<Tonic::Core::Window *>(glfwGetWindowUserPointer(window));
        if (win->m_CloseCallback != nullptr) win->m_CloseCallback();
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
        auto win = static_cast<Tonic::Core::Window *>(glfwGetWindowUserPointer(window));
        if (win->m_MouseButtonCallback != nullptr) win->m_MouseButtonCallback(button, action, mods);
    });

    if (createGLContext) glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

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

void Window::SwapBuffer()
{
    glfwSwapBuffers(m_Window);
}
}