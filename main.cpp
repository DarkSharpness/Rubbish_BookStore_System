#include "src/command.hpp"
#include <filesystem>

signed main() {
    std::filesystem::create_directory("bin");
    dark::commandManager Conless; // I love Conless!
    freopen("bookstore-testcases\\robust\\testcase4.in","r",stdin);
    try {
        while(Conless.runCommand());
    } catch (const char *str) {
        std::cout << str;
    }
    return 0;
}