#include "Potion/Rendering/Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Tonic/Graphics/Window.h>

namespace Potion::Rendering
{
Camera::Camera(Tonic::Graphics::Device &device) : Resource(device)
{
    SetSize(1.0f, 1.0f);
}

Camera::Camera(Tonic::Graphics::Device &device, float width, float height) : Resource(device)
{
    SetSize(width, height);
}

void Camera::SetSize(float width, float height)
{
    m_Size = { width, height };
    RebuildProjectionMatrix();
}

void Camera::RebuildViewMatrix()
{
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), { m_Position, 0.0f });
    glm::mat4 rotation = (
        glm::translate(glm::mat4(1.0f), { m_Size / 2.0f, 0.0f }) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotate), { 0.0f, 0.0f, 1.0f }) * 
        glm::translate(glm::mat4(1.0f), { -m_Size / 2.0f, 0.0f }));

    m_ViewMatrix = glm::inverse(translate * rotation) * glm::scale(glm::mat4(1.0f), { m_Scale, 1.0f });
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::RebuildProjectionMatrix()
{
    m_ProjectionMatrix = glm::ortho(0.0f, m_Size.x, m_Size.y, 0.0f);
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

void Camera::SetScale(const glm::vec2 &scale)
{
    m_Scale = scale;
    RebuildViewMatrix();
}
}