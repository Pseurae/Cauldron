#include "Potion/Rendering/Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Potion::Rendering
{
Camera::Camera(int x, int y, int width, int height)
{
    SetViewport(x, y, width, height);
    RebuildViewMatrix();
}

Camera::Camera(int width, int height)
{
    SetViewport(0, 0, width, height);
    RebuildViewMatrix();
}

void Camera::SetViewport(int x, int y, int width, int height)
{
    m_ViewportStart = { x, y };
    m_ViewportEnd = { x + width, y + height };
    RebuildProjectionMatrix();
}

void Camera::RebuildViewMatrix()
{
    glm::vec2 center = { m_ViewportEnd.x - m_ViewportStart.x, m_ViewportEnd.y - m_ViewportStart.y };

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), { m_Position, 0.0f });
    glm::mat4 rotation = (
        glm::translate(glm::mat4(1.0f), { center / 2.0f, 0.0f }) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotate), { 0.0f, 0.0f, 1.0f }) * 
        glm::translate(glm::mat4(1.0f), { -center / 2.0f, 0.0f }));

    m_ViewMatrix = glm::inverse(translate * rotation);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::RebuildProjectionMatrix()
{
    m_ProjectionMatrix = glm::ortho(m_ViewportStart.x, m_ViewportEnd.x, m_ViewportEnd.y, m_ViewportStart.y);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::SetPosition(int x, int y)
{
    m_Position = { x, y };
    RebuildViewMatrix();
}

void Camera::SetRotation(float degrees)
{
    m_Rotate = degrees;
    RebuildViewMatrix();
}
}