#include "Tonic/FileSystem/VFS/Drive.h"

namespace Tonic::FileSystem::VFS
{
Drive::~Drive()
{
    for (const auto &file : m_OpenedFiles)
    {
        if (file.expired()) continue;
        file.lock()->Close();
    }
}

void Drive::Mount(std::filesystem::path mountPath, Ethyl::Shared<Provider> provider)
{
    m_FileProviders.emplace(mountPath, provider);
}

void Drive::Unmount(std::filesystem::path mountPath)
{
    m_FileProviders.erase(mountPath);
}

static std::filesystem::path TryResolvePath(const std::filesystem::path &filePath, const std::filesystem::path &basePath)
{
    auto relativePath = std::filesystem::relative(filePath, basePath);
    if (relativePath.empty() || relativePath.string().starts_with("../")) return "";
    return relativePath;
}

bool Drive::Exists(const std::filesystem::path &filePath)
{
    if (std::filesystem::is_directory(filePath)) return false;

    for (auto const &[path, provider] : m_FileProviders)
    {
        auto relativePath = TryResolvePath(filePath, path);
        if (relativePath.empty()) continue;
        if (provider->Exists(relativePath)) return true;
    }
    return false;
}

Ethyl::Shared<File> Drive::OpenRead(const std::filesystem::path &filePath)
{
    if (std::filesystem::is_directory(filePath)) return nullptr;

    for (auto const &[path, provider] : m_FileProviders)
    {
        auto relativePath = TryResolvePath(filePath, path);
        if (relativePath.empty()) continue;

        if (auto ptr = provider->OpenRead(relativePath))
        {
            m_OpenedFiles.push_back(ptr);
            return ptr;
        }
    }

    return nullptr;
}

Ethyl::Shared<File> Drive::OpenWrite(const std::filesystem::path &filePath)
{
    if (m_ReadOnly) return nullptr;

    if (std::filesystem::is_directory(filePath)) return nullptr;

    for (auto const &[path, provider] : m_FileProviders)
    {
        auto relativePath = TryResolvePath(filePath, path);
        if (relativePath.empty()) continue;

        if (auto ptr = provider->OpenWrite(relativePath))
        {
            m_OpenedFiles.push_back(ptr);
            return ptr;
        }
    }

    return nullptr;
}
}