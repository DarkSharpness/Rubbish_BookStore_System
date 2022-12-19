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

struct Keyword_t {
    size_t count;
    string <64> key[20];

    /* Compare 2 keyword object. */
    friend bool operator <(const Keyword_t &lhs,const Keyword_t &rhs) {
        for(int i = 0 ; i < 20 ; ++i) {
            int cmp = Compare(lhs.key[i],rhs.key[i]);
            if(cmp) return cmp < 0;
        }
        return false;
    }

    /* Compare 2 keyword object. */
    friend int Compare(const Keyword_t &lhs,const Keyword_t &rhs) {
        size_t count = std::min(lhs.count,rhs.count);
        for(int i = 0 ; i < count ; ++i) {
            int cmp = Compare(lhs.key[i],rhs.key[i]);
            if(cmp) return cmp;
        }
        if(lhs.count == rhs.count) return 0;
        else return lhs.count < rhs.count ? -1 : 1;
    }

    size_t size() const {return count;}

    /* Return the __n'th keyword. */
    string <64> &operator [](int __n) {
        return key[__n];
    }
    /* Return the __n'th keyword. */
    const string <64> &operator [](int __n) const{
        return key[__n];
    }

};


using Level_t = enum {
    Visitor   = 0,
    Customer  = 1,
    Librarian = 3,
    Manager   = 7
};

using show_t = enum {
    BookName,
    Author,
    Keyword
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

bool isValidISBN(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str >= 32 && *str <= 127) {++str,++count;continue;}
        return false;
    }
    return count <= 20;
}

bool isValidBookName(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str >= 32 && *str <= 127 && *str != '\"') {++str,++count;continue;}
        return false;
    }
    return count <= 60;
}

bool isValidKeyword(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str >= 32 && *str <= 127 && *str != '\"') {++str,++count;continue;}
        return false;
    }
    return count <= 60;
}

std::pair <bool,size_t> getQuantity(const char *str) {
    size_t count = 0;
    size_t quantity = 0;
    while(*str) {
        if(!isdigit(*str)) return std::make_pair(false,0ULL);
        quantity = quantity * 10 + ((*str) ^ '0'); 
        ++str,++count;
    }
    return std::make_pair(count <= 10,quantity);
}

std::pair <bool,double> getMoney(const char *str) {
    size_t count = 0;
    bool dot = false;
    // size_t money = 0.0;
    while(*str) {
        if(*str == '.' && !dot) {
            dot = true;
        } else if(!isdigit(*str)) return std::make_pair(false,0.0);
        // money = money * 10 + ((*str) ^ '0');
        ++str,++count;
    }
    return std::make_pair(count <= 13,std::stod(str));
}


/* Get Priviledge from an str. */
std::pair <bool,size_t> getPrivilege(char *str) {
    if(*str == '1' || *str == '3' || *str == '7' || *str == '0'
    && *(str + 1) == '\0') {
        return std::make_pair(true,*str ^ '0');
    }
    return std::make_pair(false,0ULL);
}


}


#endif
