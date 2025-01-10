#include <Ethyl/Assert.h>
#include "Tonic/FileSystem/Drive.h"

namespace Tonic::FileSystem
{
std::vector<char> Drive::ReadFile(const std::filesystem::path &filePath)
{
    for (const auto &[basePath, provider] : mProviders)
    {
        auto resolvedPath = ResolvePath(basePath, filePath);
        if (!resolvedPath.empty() && provider->FileExists(resolvedPath)) return provider->ReadFile(resolvedPath);
    }

    return {};
}

void Drive::WriteFile(const std::filesystem::path &filePath, const std::vector<char> &content, bool overwrite)
{
    for (const auto &[basePath, provider] : mProviders)
    {
        auto resolvedPath = ResolvePath(basePath, filePath);
        if (!resolvedPath.empty() && overwrite || !provider->FileExists(resolvedPath)) provider->WriteFile(resolvedPath, content);
    }
}

void Drive::IncludePath(const std::filesystem::path &folderPath)
{
    ETHYL_ASSERT(std::filesystem::is_directory(folderPath), "Path is not a directory!");
    ETHYL_ASSERT(std::find(mSearchPaths.begin(), mSearchPaths.end(), folderPath) == mSearchPaths.end(), "Search path already exists!");
    mSearchPaths.push_back(folderPath);
}

void Drive::CheckIfPathMounted(const std::filesystem::path &mountPoint)
{
    ETHYL_ASSERT(mProviders.find(mountPoint) == mProviders.end(), "A Provider has already been mounted at {}!", mountPoint.string());
}

bool Drive::FileExists(const std::filesystem::path &filePath)
{
    for (const auto &[basePath, provider] : mProviders)
    {
        auto resolvedPath = ResolvePath(basePath, filePath);
        if (!resolvedPath.empty()) return provider->FileExists(resolvedPath);
    }

    return false;
}

std::filesystem::path Drive::ResolvePath(const std::filesystem::path &basePath, const std::filesystem::path &filePath)
{
    auto relPath = std::filesystem::relative(filePath, basePath);
    if (relPath.empty() || relPath.string().starts_with("..")) return "";
    return relPath;
}

void Drive::RegisterProviderInternal(const std::filesystem::path &mountPoint, Ethyl::Shared<Provider> provider)
{
    CheckIfPathMounted(mountPoint);
    mProviders[mountPoint] = std::move(provider);
}
}