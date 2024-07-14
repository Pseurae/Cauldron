#include "Tonic/Graphics/Window.h"
#include "Tonic/Graphics/Device.h"

namespace Tonic::Graphics
{
Device::Device(Window &window) : m_Window(window)
{
}
}