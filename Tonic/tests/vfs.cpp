#include "Tonic/FileSystem/Drive.h"
#include "Tonic/FileSystem/Provider.h"
#include <iostream>

int main(int argc, char* const argv[])
{
    try {
        Tonic::FileSystem::Drive drive;
        drive.RegisterProvider<Tonic::FileSystem::NativeFileProvider>("test", "test_folder");
        std::cout << drive.FileExists("test/1.txt") << std::endl;
        for (const char i : drive.ReadFile("test/1.txt"))
            std::cout << i << std::endl;
        
        drive.WriteFile("test/2.txt", { 't', 'e', 's', 't' });
    } catch (std::runtime_error &e) {}
    return 0;
}