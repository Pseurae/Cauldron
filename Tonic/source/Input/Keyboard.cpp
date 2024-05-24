#include "Tonic/Input/Keyboard.h"

namespace Tonic::Input
{
void Keyboard::Update(Key key, Action action, KeyMod mods)
{
    m_CurrentKeys[(int)key] = action == Action::Press;
    m_Mods = mods;
}
}