#ifndef TONIC_GRAPHICS_SHADER_H
#define TONIC_GRAPHICS_SHADER_H

#include "Tonic/Graphics/Resource.h"
#include "Tonic/Graphics/Layout.h"
#include <string>
#include <vector>

namespace Tonic::Graphics
{
struct ShaderDesc
{
    std::string vertex;
    std::string fragment;

    Layout vertexLayout;
};

class Shader : public Resource 
{
public:
    explicit Shader(Device &device, Layout layout) : Resource(device), m_Layout(layout) {}
    const auto &GetLayout() const { return m_Layout; }
private:
    Layout m_Layout;
};
}

#endif // TONIC_GRAPHICS_SHADER_H