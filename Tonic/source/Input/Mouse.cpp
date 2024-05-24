#include "Tonic/Input/Mouse.h"

namespace Tonic::Input
{
void Mouse::Update(MouseButton button, Action action, KeyMod mods)
{
    m_CurrentButtons[(int)button] = action == Action::Press;
    m_Mods = mods;
}
}