#ifndef TONIC_GRAPHICS_SHADER_H
#define TONIC_GRAPHICS_SHADER_H

#include "Tonic/Graphics/Resource.h"
#include "Tonic/Graphics/Layout.h"
#include <string>
#include <vector>
#include <map>

namespace Tonic::Graphics
{
struct ShaderDesc
{
    std::string vertex;
    std::string fragment;
};

class Shader : public Resource 
{
public:
    explicit Shader(Device &device) : Resource(device) {}
};
}

#endif // TONIC_GRAPHICS_SHADER_H