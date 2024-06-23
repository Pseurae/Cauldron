#include <Ethyl/Assert.h>
#include <ranges>
#include "Tonic/FileSystem/VFS/Provider.h"
#include <fstream>
#include <iostream>

namespace Tonic::FileSystem::VFS
{
NativeFileProvider::
NativeFileProvider(const std::filesystem::path &path) : m_Directory(path)
{
    ETHYL_ASSERT(std::filesystem::is_directory(path), "Argument was not a path to a directory! (Given \"{}\")", path.string());
    static auto fltr = [](const std::filesystem::directory_entry &dirEntry) { return dirEntry.is_regular_file() && dirEntry.exists(); };

    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator{path} | std::views::filter(fltr))
        m_IndexedFiles.push_back(std::filesystem::relative(dirEntry.path(), path).string());
}

std::vector<char> NativeFileProvider::ReadFile(const std::filesystem::path &filePath)
{
    auto path = m_Directory / filePath;

    std::ifstream t(path);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();

    std::vector<char> contents(size, 0);

    t.seekg(0);
    t.read(contents.data(), size); 

    t.close();

    return contents;
}

void NativeFileProvider::WriteFile(const std::filesystem::path &filePath, const std::vector<char> &content)
{
    auto path = m_Directory / filePath;

    std::ofstream t(path);
    t.write(content.data(), content.size());
    t.close();
}

bool NativeFileProvider::FileExists(const std::filesystem::path &filePath)
{
    return std::find(m_IndexedFiles.begin(), m_IndexedFiles.end(), filePath) != m_IndexedFiles.end();
}
}