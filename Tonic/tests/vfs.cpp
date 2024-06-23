#include "Tonic/FileSystem/VFS/Drive.h"
#include "Tonic/FileSystem/VFS/Provider.h"
#include <iostream>

int main(int argc, char* const argv[])
{
    try {
        Tonic::FileSystem::VFS::Drive drive;
        drive.RegisterProvider<Tonic::FileSystem::VFS::NativeFileProvider>("test", "test_folder");
        std::cout << drive.FileExists("test/1.txt") << std::endl;
        for (const char i : drive.ReadFile("test/1.txt"))
            std::cout << i << std::endl;
        
        drive.WriteFile("test/2.txt", { 't', 'e', 's', 't' });
    } catch (std::runtime_error &e) {}
    return 0;
}