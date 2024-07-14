#ifndef POTION_CONTENT_FORMATS_TEXT_H
#define POTION_CONTENT_FORMATS_TEXT_H

#include <vector>
#include <string>

namespace Potion::Content
{
class Text
{
public:
    Text(const std::vector<char> &);
    ~Text();

    inline std::string GetText() const { return m_Text; };

private:
    std::string m_Text;
};
}

#endif // POTION_CONTENT_FORMATS_TEXT_H