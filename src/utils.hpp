// Header-only library of utility
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "string.hpp"


namespace dark {

using ISBN_t     = string <24>;
using Author_t   = string <64>;
using BookName_t = string <64>;
// using Keyword_t  = string <64>;
using UserID_t   = string <32>;
using UserName_t = string <32>;
using Password_t = string <32>;
using FileName_t = string <16>;

struct Keyword_t {
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
        for(int i = 0 ; i < 20 ; ++i) {
            int cmp = Compare(lhs.key[i],rhs.key[i]);
            if(cmp) return cmp;
        }
        return 0;
    }

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
    Customer  = 1,
    Librarian = 3,
    Manager   = 7
};

using show_t = enum {
    BookName,
    Author,
    Keyword
};


template <class key_t>
size_t binary_search(key_t *A,size_t len,const key_t &key) {
    size_t l = 0,r = len;
    while(l != r) {
        size_t mid = (l + r) >> 1;
        if(A[mid] < key) {
            l = mid + 1;
        } else if(key < A[mid]) {
            r = mid;
        } else {return mid;}
    }
    return EOF;
}






}


#endif
