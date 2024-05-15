#ifndef TONIC_FILESYSTEM_VFS_DRIVE_H
#define TONIC_FILESYSTEM_VFS_DRIVE_H

#include <filesystem>
#include <unordered_map>
#include <list>
#include "Tonic/Common/Pointers.h"
#include "Tonic/FileSystem/VFS/Provider.h"

namespace Tonic::FileSystem::VFS
{
class Drive /* final */
{
public:
    Drive() = default;
    Drive(bool readOnly) : m_ReadOnly(readOnly) {}
    virtual ~Drive();
    void Mount(std::filesystem::path mountPath, Shared<Provider> provider);
    void Unmount(std::filesystem::path mountPath);

    [[nodiscard]] bool Exists(const std::filesystem::path &filePath);
    [[nodiscard]] Shared<File> OpenRead(const std::filesystem::path &filePath);
    [[nodiscard]] Shared<File> OpenWrite(const std::filesystem::path &filePath);
private:
    std::unordered_map<std::filesystem::path, Shared<Provider>> m_FileProviders;
    std::list<WeakRef<File>> m_OpenedFiles;

    const bool m_ReadOnly = false;
};
}

#endif // TONIC_FILESYSTEM_VFS_DRIVE_H