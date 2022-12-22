#include <bits/stdc++.h>
#include "src/command.hpp"


signed main() {
    dark::commandManager Conless; // I love Conless!
    try {
        while(Conless.runCommand());
    } catch (const char *str) {
        std::cout << str;
    }
    return 0;
}