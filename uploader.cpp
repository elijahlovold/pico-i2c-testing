
#include <iostream>
#include <filesystem>
#include <fstream>

#ifndef S
#define S "/media/elovold/RPI-RP2"
#endif

#ifndef D
#define D "keyboard_firmware.uf2"
#endif

void copyFile(const std::string& source, const std::string& destination) {
    std::ifstream src(source, std::ios::binary);
    if (!src) {
        std::cerr << "Error: Could not open source file: " << source << std::endl;
        return;
    }

    std::ofstream dst(destination, std::ios::binary);
    if (!dst) {
        std::cerr << "Error: Could not open destination file: " << destination << std::endl;
        return;
    }

    dst << src.rdbuf();

    if (dst.bad()) {
        std::cerr << "Error: Failed to write to destination file: " << destination << std::endl;
    }
}


int main() {
    std::string tar_dir = D;
    // std::string fileToCopy = "keyboard_firmware.uf2"; // Replace with your file to copy
    std::string src_file = S;

    // Check if the directory exists
    if (std::filesystem::exists(tar_dir) && std::filesystem::is_directory(tar_dir)) {
        std::cout << "Directory exists: " << tar_dir << std::endl;

        // Define the destination path
        std::string dest_path = tar_dir + "/" + src_file;

        // Copy the file
        copyFile(src_file, dest_path);
        std::cout << "Copied " << src_file << " to " << dest_path << std::endl;
    } else {
        std::cout << "Directory does not exist: " << tar_dir << ". No action will be taken." << std::endl;
    }

    return 0;
}
