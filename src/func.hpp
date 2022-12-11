#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include "book.hpp"

namespace dark {
struct Keyword_Cmp {
    inline bool operator ()(const Book &X,const Book &Y) {
        int cmp = Compare(X.Keyword,Y.Keyword);
        if(cmp) return cmp;
        else    return X.ISBN < Y.ISBN; 
    }
};

struct ISBN_Cmp {
    inline bool operator ()(const Book &X,const Book &Y) {

    }
};




}


#endif
