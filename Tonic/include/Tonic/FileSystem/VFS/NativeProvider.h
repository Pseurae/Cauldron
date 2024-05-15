#ifndef TONIC_FILESYSTEM_VFS_TESTPROVIDER_H
#define TONIC_FILESYSTEM_VFS_TESTPROVIDER_H

#include <filesystem>
#include <span>
#include <stdio.h>
#include <fstream>
#include "Tonic/FileSystem/VFS/Provider.h"

namespace Tonic::FileSystem::VFS
{
class NativeInputFile : public File
{
public:
    NativeInputFile(std::ifstream &stream) : m_Stream(std::move(stream)) {}
    virtual void Close() override { m_Stream.close(); }
    virtual size_t Read(void *buffer, size_t elemSize, size_t count) override;
    virtual size_t Write(const void *buffer, size_t elemSize, size_t count) override { return 0; };
    virtual size_t Seek() override { return 0; };
    virtual size_t Tell() override { return m_Stream.tellg(); }
private:
    std::ifstream m_Stream;
};

class NativeOutputFile : public File
{
public:
    NativeOutputFile(std::ofstream &stream) : m_Stream(std::move(stream)) {}
    virtual void Close() override { m_Stream.close(); }
    virtual size_t Read(void *buffer, size_t elemSize, size_t count) override { return 0; };
    virtual size_t Write(const void *buffer, size_t elemSize, size_t count) override;
    virtual size_t Seek() override { return 0; };
    virtual size_t Tell() override { return m_Stream.tellp(); }
private:
    std::ofstream m_Stream;
};

class NativeProvider : public Provider
{
public:
    NativeProvider() = default;
    NativeProvider(const std::filesystem::path &basePath);
    [[nodiscard]] virtual bool Exists(const std::filesystem::path &relativePath) override;
    [[nodiscard]] virtual Shared<File> OpenRead(const std::filesystem::path &relativePath) override;
    [[nodiscard]] virtual Shared<File> OpenWrite(const std::filesystem::path &relativePath) override;
private:
    std::filesystem::path m_BasePath = ".";
};
}

#endif // TONIC_FILESYSTEM_VFS_TESTPROVIDER_H