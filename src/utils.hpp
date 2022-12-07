// Header-only library of utility
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "string.hpp"


namespace dark {

using ISBN_t     = string <24>;
using Author_t   = string <64>;
using BookName_t = string <64>;
using Keyword_t  = string <64>;
using UserID_t   = string <32>;
using UserName_t = string <32>;
using Password_t = string <32>;
using FileName_t = string <16>;

using Level_t = enum {
    Customer  = 1,
    Librarian = 3,
    Manager   = 7
};

using show_t = enum {
    BookName,
    Author,
    Keyword
};


}


#endif
