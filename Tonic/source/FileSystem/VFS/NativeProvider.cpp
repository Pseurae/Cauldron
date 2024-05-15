#include <iostream>
#include "Tonic/FileSystem/VFS/NativeProvider.h"

namespace Tonic::FileSystem::VFS
{
size_t NativeInputFile::Read(void *buffer, size_t elemSize, size_t count)
{
    m_Stream.read((char *)buffer, elemSize * count);
    return m_Stream.gcount();
}

size_t NativeOutputFile::Write(const void *buffer, size_t elemSize, size_t count)
{
    auto oldPos = m_Stream.tellp();
    m_Stream.write((const char *)buffer, elemSize * count);
    return m_Stream.tellp() - oldPos;
}

NativeProvider::NativeProvider(const std::filesystem::path &basePath) : m_BasePath(basePath)
{}

bool NativeProvider::Exists(const std::filesystem::path &relativePath)
{
    return std::filesystem::exists(m_BasePath / relativePath);
}

Shared<File> NativeProvider::OpenRead(const std::filesystem::path &relativePath)
{
    std::ifstream stream{m_BasePath / relativePath};
    if (!stream.is_open() || stream.bad()) return nullptr;

    return CreateShared<NativeInputFile>(stream);
}

Shared<File> NativeProvider::OpenWrite(const std::filesystem::path &relativePath)
{
    std::ofstream stream{m_BasePath / relativePath};
    if (!stream.is_open() || stream.bad()) return nullptr;

    return CreateShared<NativeOutputFile>(stream);
}
}