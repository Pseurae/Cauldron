#ifndef POTION_RENDERING_CAMERA_H
#define POTION_RENDERING_CAMERA_H

#include <glm/mat4x4.hpp>
#include <Tonic/Graphics/Device.h>
#include <Tonic/Graphics/Resource.h>

namespace Potion::Rendering
{
struct Camera : public Tonic::Graphics::Resource
{
public:
    Camera(Tonic::Graphics::Device &device);
    Camera(Tonic::Graphics::Device &device, float width, float height);

    void SetSize(float width, float height);
    const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    void SetPosition(int x, int y);
    void SetRotation(float degrees);
    void SetScale(const glm::vec2 &scale);

private:
    void RebuildViewMatrix();
    void RebuildProjectionMatrix();

    glm::vec2 m_Size = glm::vec2(1.0f);

    glm::vec2 m_Position = glm::vec2(0.0f);
    glm::vec2 m_Scale = glm::vec2(1.0f);
    float m_Rotate = 0.0f;

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
};
}

#endif // POTION_RENDERING_CAMERA_H