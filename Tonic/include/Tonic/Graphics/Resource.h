#ifndef TONIC_GRAPHICS_RESOURCE_H
#define TONIC_GRAPHICS_RESOURCE_H

namespace Tonic::Graphics
{
class Device;

class Resource
{
protected:
    Device &mDevice;
public:
    explicit Resource(Device &device) : mDevice(device) {}

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
};
}

#endif // TONIC_GRAPHICS_RESOURCE_H
