#ifndef TONIC_FILESYSTEM_VFS_PROVIDER_H
#define TONIC_FILESYSTEM_VFS_PROVIDER_H

#include <filesystem>
#include <vector>
#include <string>

namespace Tonic::FileSystem::VFS
{
struct Provider
{
    virtual std::vector<char> ReadFile(const std::filesystem::path &filePath) = 0;
    virtual void WriteFile(const std::filesystem::path &filePath, const std::vector<char> &content) = 0;
    virtual bool FileExists(const std::filesystem::path &filePath) = 0;
};

class NativeFileProvider : public Provider
{
public:
    NativeFileProvider(const std::filesystem::path &folderPath);
    std::vector<char> ReadFile(const std::filesystem::path &filePath) override;
    void WriteFile(const std::filesystem::path &filePath, const std::vector<char> &content) override;
    bool FileExists(const std::filesystem::path &filePath) override;
private:
    std::filesystem::path mDirectory;
    std::vector<std::string> mIndexedFiles;
};
}

#endif // TONIC_FILESYSTEM_VFS_PROVIDER_H