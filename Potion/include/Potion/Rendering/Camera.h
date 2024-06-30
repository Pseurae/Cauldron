#ifndef POTION_RENDERING_CAMERA_H
#define POTION_RENDERING_CAMERA_H

#include <glm/mat4x4.hpp>

namespace Potion::Rendering
{
struct Camera
{
public:
    Camera() = default;
    Camera(int x, int y, int width, int height);
    Camera(int width, int height);

    void SetViewport(int x, int y, int width, int height);
    const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    void SetPosition(int x, int y);
    void SetRotation(float degrees);

private:
    void RebuildViewMatrix();
    void RebuildProjectionMatrix();

    glm::vec2 m_ViewportStart = glm::vec2(0.0f);
    glm::vec2 m_ViewportEnd = glm::vec2(1.0f);
    glm::vec2 m_Position = glm::vec2(0.0f);
    glm::vec2 m_Scale = glm::vec2(1.0f);
    float m_Rotate = 0.0f;

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
};
}

#endif // POTION_RENDERING_CAMERA_H