#include <Ethyl/Assert.h>
#include <ranges>
#include "Tonic/FileSystem/Provider.h"
#include <fstream>
#include <iostream>

namespace Tonic::FileSystem
{
NativeFileProvider::NativeFileProvider(const std::filesystem::path &path) : mDirectory(path)
{
    ETHYL_ASSERT(std::filesystem::is_directory(path), "Argument was not a path to a directory! (Given \"{}\")", path.string());
    static auto fltr = [](const std::filesystem::directory_entry &dirEntry) { return dirEntry.is_regular_file() && dirEntry.exists(); };

    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator{path} | std::views::filter(fltr))
        mIndexedFiles.push_back(std::filesystem::relative(dirEntry.path(), path).string());
}

std::vector<char> NativeFileProvider::ReadFile(const std::filesystem::path &filePath)
{
    auto path = mDirectory / filePath;

    std::ifstream t(path, std::ios::binary);
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
    auto path = mDirectory / filePath;

    std::ofstream t(path, std::ios::binary);
    t.write(content.data(), content.size());
    t.close();
}

bool NativeFileProvider::FileExists(const std::filesystem::path &filePath)
{
    return std::find(mIndexedFiles.begin(), mIndexedFiles.end(), filePath) != mIndexedFiles.end();
}
}