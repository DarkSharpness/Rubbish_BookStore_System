#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include "book.hpp"
#include "account.hpp"

#include <vector>

namespace dark {

class AccountSystem {
    struct User { // simple wrapping
        int index; // Index in the vector
        int book;  // The book selected
    };
    std::vector <User> users;



};

class BookSystem {


};





}


#endif
