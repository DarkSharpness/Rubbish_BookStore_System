#include "src/command.hpp"
#include <filesystem>

signed main() {
    std::filesystem::create_directory("bin");
    dark::commandManager Conless; // I love Conless!
    // freopen("bookstore-testcases\\advanced\\testcase5\\10.in","r",stdin);
    // freopen("bookstore-testcases\\advanced\\testcase5\\0.ans","w",stdout);

    try {
        while(Conless.runCommand());
    } catch (const char *str) {
        std::cout << str;
    }
    return 0;
}