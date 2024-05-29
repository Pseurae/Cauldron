#ifndef TONIC_INPUT_MOUSE_H
#define TONIC_INPUT_MOUSE_H

#include "Tonic/Input/Keys.h"
#include <array>

namespace Tonic::Input
{
class Mouse final
{
public:
    [[nodiscard]] const auto &GetMods() const { return m_Mods; }

    void Update(MouseButton button, Action action, KeyMod mods);
    void SwapState() { m_LastButtons = m_CurrentButtons; }

    bool Held(MouseButton button) const { return m_CurrentButtons[(int)button] && m_LastButtons[(int)button]; }
    bool Pressed(MouseButton button) const { return m_CurrentButtons[(int)button] && !m_LastButtons[(int)button]; }
    bool Released(MouseButton button) const { return !m_CurrentButtons[(int)button] && m_LastButtons[(int)button]; }

private:
    KeyMod m_Mods;
    std::array<bool, MouseButtonCount> m_CurrentButtons{false};
    std::array<bool, MouseButtonCount> m_LastButtons{false};
};
}

#endif // TONIC_INPUT_MOUSE_H