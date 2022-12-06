// Header-only library for BOOK
#ifndef _BOOK_HPP_
#define _BOOK_HPP_

#include "string.hpp"
#include <iomanip>

namespace dark {

struct Book {
    string <24> ISBN;        // ISBN
    string <64> Name;        // Name
    string <64> Author;      // Author
    string <64> Keyword[60]; // Keywords
    size_t keywordNum;       // number of keywords
    size_t quantity;         // quantitiy of books 
    double cost;             // cost of one book

    Book() = default;
    Book(const string <24> &_ISBN) {
        ISBN = _ISBN;
        quantity = keywordNum = 0;
        cost = 0.0;
    }

    bool exist() const{return ISBN[0];}

    friend bool operator <(const Book &lhs,const Book &rhs) {
        return lhs.ISBN < rhs.ISBN;
    }
    friend std::ostream &operator <<(std::ostream &os, const Book &book) {
        os << book.ISBN   << '\t'
           << book.Name   << '\t'
           << book.Author << '\t'
           << book.Keyword[0];
        for(int i = 1; i < book.keywordNum; ++i)
            os << '|' << book.Keyword[i];
        return os << std::fixed << std::setprecision(2)
                  << '\t' << book.cost << '\t' << book.quantity;
    }

};

// constexpr size_t t = sizeof(Book);


}

#endif
