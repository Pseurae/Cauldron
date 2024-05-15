#ifndef TONIC_GRAPHICS_RESOURCE_H
#define TONIC_GRAPHICS_RESOURCE_H

namespace Tonic::Graphics
{
class Device;

class Resource
{
protected:
    Device &m_Device;
public:
    explicit Resource(Device &device) : m_Device(device) {}

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
};
}

#endif // TONIC_GRAPHICS_RESOURCE_H
