#include <Ethyl/Assert.h>
#include "Tonic/FileSystem/VFS/Drive.h"

namespace Tonic::FileSystem::VFS
{
std::vector<char> Drive::ReadFile(const std::filesystem::path &filePath)
{
    for (const auto &[basePath, provider] : m_Providers)
    {
        auto resolvedPath = ResolvePath(basePath, filePath);
        if (!resolvedPath.empty() && provider->FileExists(resolvedPath)) return provider->ReadFile(resolvedPath);
    }

    return {};
}

void Drive::WriteFile(const std::filesystem::path &filePath, const std::vector<char> &content, bool overwrite)
{
    for (const auto &[basePath, provider] : m_Providers)
    {
        auto resolvedPath = ResolvePath(basePath, filePath);
        if (!resolvedPath.empty() && overwrite || !provider->FileExists(resolvedPath)) provider->WriteFile(resolvedPath, content);
    }
}

void Drive::IncludePath(const std::filesystem::path &folderPath)
{
    ETHYL_ASSERT(std::filesystem::is_directory(folderPath), "Path is not a directory!");
    ETHYL_ASSERT(std::find(m_SearchPaths.begin(), m_SearchPaths.end(), folderPath) == m_SearchPaths.end(), "Search path already exists!");
    m_SearchPaths.push_back(folderPath);
}

void Drive::CheckIfPathMounted(const std::filesystem::path &mountPoint)
{
    ETHYL_ASSERT(m_Providers.find(mountPoint) == m_Providers.end(), "A Provider has already been mounted at {}!", mountPoint.string());
}

bool Drive::FileExists(const std::filesystem::path &filePath)
{
    for (const auto &[basePath, provider] : m_Providers)
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
    m_Providers[mountPoint] = std::move(provider);
}
}