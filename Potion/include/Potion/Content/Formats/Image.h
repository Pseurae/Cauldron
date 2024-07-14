#ifndef POTION_CONTENT_FORMATS_IMAGE_H
#define POTION_CONTENT_FORMATS_IMAGE_H

#include <vector>

namespace Potion::Content
{
class Image
{
public:
    Image(const std::vector<char> &);
    ~Image();
private:
    int m_Width, m_Height, n_NumChannels;
};
}

#endif // POTION_CONTENT_FORMATS_IMAGE_H