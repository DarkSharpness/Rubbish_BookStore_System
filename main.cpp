// #define DARK_ERROR true
#include "src/command.hpp"
#include <filesystem>

signed main() {
    std::filesystem::create_directory("bin");
    dark::commandManager Conless; // I love Conless!
    while(Conless.runCommand());
    return 0;
}