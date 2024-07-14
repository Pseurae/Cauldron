#ifndef POTION_CONTENT_LOADER_H
#define POTION_CONTENT_LOADER_H

#include <Tonic/FileSystem/VFS/Drive.h>
#include <Ethyl/Traits/Constructible.h>
#include <vector>
#include <Ethyl/Assert.h>

namespace Potion::Content
{
class Loader final
{
public:
    Loader(Tonic::FileSystem::VFS::Drive &drive);
    ~Loader();

    template<typename T>
    requires(Ethyl::Traits::Constructible<T, std::vector<char>>)
    T Load(const std::filesystem::path &path)
    {
        ETHYL_ASSERT(m_Drive.FileExists(), "File \"{}\" does not exists!", path.string().c_str());
        return T(m_Drive.ReadFile(path));
    }

private:
    Tonic::FileSystem::VFS::Drive &m_Drive;
};
}

#endif // POTION_CONTENT_LOADER_H