#ifndef TONIC_FILESYSTEM_VFS_DRIVE_H
#define TONIC_FILESYSTEM_VFS_DRIVE_H

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <list>
#include <Ethyl/Pointers.h>
#include "Tonic/FileSystem/VFS/Provider.h"
#include <type_traits>
#include <optional>

namespace Tonic::FileSystem::VFS
{
class Drive final
{
public:
    Drive() = default;
    std::vector<char> ReadFile(const std::filesystem::path &filePath);
    void WriteFile(const std::filesystem::path &filePath, const std::vector<char> &content, bool overwrite = false);

    void IncludePath(const std::filesystem::path &folderPath);
    void CheckIfPathMounted(const std::filesystem::path &mountPoint);

    template<typename TProvider, typename... Args>
    requires(std::is_base_of_v<Provider, TProvider>)
    inline auto RegisterProvider(const std::filesystem::path &mountPoint, Args&&... args)
    {
        if constexpr (std::is_aggregate_v<TProvider>) RegisterProviderInternal(mountPoint, Ethyl::Shared<TProvider>(new TProvider{std::forward<Args>(args)...}));
        else RegisterProviderInternal(mountPoint, Ethyl::Shared<TProvider>(new TProvider(std::forward<Args>(args)...)));
    }

    inline void ClearAllProviders() { mProviders.clear(); }
    bool FileExists(const std::filesystem::path &path);

private:
    void RegisterProviderInternal(const std::filesystem::path &mountPoint, Ethyl::Shared<Provider> provider);

    std::filesystem::path ResolvePath(const std::filesystem::path &basePath, const std::filesystem::path &filePath);
    std::vector<std::filesystem::path> mSearchPaths;
    std::unordered_map<std::filesystem::path, Ethyl::Shared<Provider>> mProviders;
};
}

#endif // TONIC_FILESYSTEM_VFS_DRIVE_H