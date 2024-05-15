#ifndef TONIC_FILESYSTEM_VFS_PROVIDER_H
#define TONIC_FILESYSTEM_VFS_PROVIDER_H

#include <filesystem>
#include <span>
#include "Tonic/Common/Pointers.h"

namespace Tonic::FileSystem::VFS
{
class File
{
public:
    virtual void Close() = 0;

    virtual size_t Read(void *buffer, size_t elemSize, size_t count) = 0;
    virtual size_t Write(const void *buffer, size_t elemSize, size_t count) = 0;
    virtual size_t Seek() = 0;
    virtual size_t Tell() = 0;
};

class Provider
{
public:
    [[nodiscard]] virtual bool Exists(const std::filesystem::path &relativePath) = 0;
    [[nodiscard]] virtual Shared<File> OpenRead(const std::filesystem::path &relativePath) = 0;
    [[nodiscard]] virtual Shared<File> OpenWrite(const std::filesystem::path &relativePath) = 0;
};
}

#endif // TONIC_FILESYSTEM_VFS_PROVIDER_H