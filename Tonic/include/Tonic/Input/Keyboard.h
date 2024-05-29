#ifndef TONIC_INPUT_KEYBOARD_H
#define TONIC_INPUT_KEYBOARD_H

#include <array>
#include "Tonic/Input/Keys.h"

namespace Tonic::Input
{
class Keyboard final 
{
public:
    [[nodiscard]] const auto &GetMods() const { return m_Mods; }

    void Update(Key key, Action action, KeyMod mods);
    void SwapState() { m_LastKeys = m_CurrentKeys; }

    bool Held(Key key) const { return m_CurrentKeys[(int)key] && m_LastKeys[(int)key]; }
    bool Pressed(Key key) const { return m_CurrentKeys[(int)key] && !m_LastKeys[(int)key]; }
    bool Released(Key key) const { return !m_CurrentKeys[(int)key] && m_LastKeys[(int)key]; }

private:
    KeyMod m_Mods;
    std::array<bool, KeyCount> m_CurrentKeys{false};
    std::array<bool, KeyCount> m_LastKeys{false};
};
}

#endif // TONIC_INPUT_KEYBOARD_H