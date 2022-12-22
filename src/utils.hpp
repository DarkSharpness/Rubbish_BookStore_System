// Header-only library of utility
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "string.hpp"
#include "exception.hpp"

namespace dark {

using ISBN_t     = string <24>;
using Author_t   = string <64>;
using BookName_t = string <64>;
using UserID_t   = string <32>;
using UserName_t = string <32>;
using Password_t = string <32>;
using FileName_t = string <32>;
using Keyword_t  = string <64>;

using Level_t = enum {
    Customer  = 1,
    Librarian = 3,
    Manager   = 7
};

enum class Command_t{
    exit    = 0,  // quit/exit
    login   = 1,  // log in 
    logout  = 2,  // log out
    reg     = 3,  // register
    passwd  = 4,  // change password
    userad  = 5,  // add user
    remove  = 6,  // delete account
    show    = 7,  // show by certain tag
    buy     = 8,  // buy something
    select  = 9,  // select a book by ID
    modify  = 10, // modify selected book
    import  = 11, // import book
    show_f  = 12, // show finance
    log     = 13, // show log
    unknown = 114514 // unknown case
};

/* Standard compare function for any type. */
template <class T>
int Compare(const T &x,const T &y) {
    if(x < y) return -1;
    else return (y < x);
}

/* Compare 2 char * strings. */
int Compare(const char *s1,const char *s2) {
    return strcmp(s1,s2);
}


/* Test whether it's a valid char  */
bool isValidChar(char __c) {
    return __c > 0 && __c < 128;
}

/* Test whether it's a UserID. */
bool isValidUserID(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str == '_'
        || (*str >= 'a' && *str <= 'z')
        || (*str >= 'A' && *str <= 'Z')
        || isdigit(*str)
        ) {++str,++count;continue;}
        return false;
    }
    return count <= 30;
}

/* Test whether it's a UserName. */
bool isValidUserName(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str >= 32 && *str <= 127) {++str,++count;continue;}
        return false;
    }
    return count <= 30;
}

/* Check ISBN. */
bool isValidISBN(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str >= 32 && *str <= 127) {++str,++count;continue;}
        return false;
    }
    return count <= 20;
}

/* Check BookName. */
bool isValidBookName(const char *str) {
    int count = 0;
    while(*str) {
        if(*str >= 32 && *str <= 127 && *str != '\"') {++str,++count;continue;}
        return false;
    }
    return count <= 60;
}

/* Check Author. */
bool isValidAuthor(const char *str) {
    return isValidBookName(str);
}

/* Check Keyword. */
bool isValidKeyword(const char *str) {
    int count  = 0;
    int length = 0; 
    while(*str) {
        if(*str == '|') {
            if(!length) return false;
            length = 0;
            ++str;
            continue;
        }
        if(*str >= 32 && *str <= 127 && *str != '\"') {
            ++str,++count,++length;
            continue;
        }
        return false;
    }
    return count <= 60 && length; // Last string shouldn't be 0 in length.
}

/* Get quantity from a char string */
std::pair <bool,size_t> getQuantity(const char *str) {
    if(*str == '0') {
        return std::make_pair(!*(str + 1),0ULL);
    }
    int count = 0;
    size_t quantity = 0;
    while(*str) {
        if(!isdigit(*str)) return std::make_pair(false,0ULL);
        quantity = quantity * 10 + (*str ^ '0'); 
        ++str,++count;
    }
    return std::make_pair(quantity < 2147483647ULL
                       && quantity && count <= 10,quantity);
}

/* Get money from a char string */
std::pair <bool,double> getMoney(const char *str) {
    int count1 = 0; // Digit number.
    int count2 = 0; // Digit number after dot.
    bool dot = false;  // Whether there is dot.
    if(*str == '.' || (*str == '0' && *(str + 1) != '.')) {
        return std::make_pair(false,0.0);
    }

    while(*str) {
        ++count1;
        if(*str == '.' && !dot) {
            dot = true;
            ++str;
            continue;
        } else if(!isdigit(*str)) return std::make_pair(false,0.0);
        if(dot) ++count2;
        ++str;
    }
    return std::make_pair(count1 <= 13 && count2 == 2,std::stod(str - count1));
}

/* Get Priviledge from an str. */
std::pair <bool,Level_t> getPrivilege(const char *str) {
    return std::make_pair((*str == '1' || *str == '3' || *str == '7') 
                       && !*(str + 1),Level_t(*str ^ '0'));
}

/* Get one Keyword.False if no more keyword. */
bool getKeyword(Keyword_t &__K,const char *&str) {
    size_t index = 0;
    while(*str && *str != '|') {
        __K[index++] = *(str++);
    }
    __K[index] = '\0';
    if(!*str++) return false;
    else        return true; // skip this |
}

}


#endif
