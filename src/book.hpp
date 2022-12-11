// Header-only library for BOOK
#ifndef _BOOK_HPP_
#define _BOOK_HPP_

#include "utils.hpp"
#include <iomanip>

namespace dark {

/* Book Class */
struct Book {
  public:
    ISBN_t     ISBN;        // Internal standard book number.
    BookName_t Name;        // Name of the book
    Author_t   Author;      // Author's name
    Keyword_t  Keyword;     // Keywords class

    size_t keywordNum;      // number of keywords
    size_t quantity;        // quantitiy of books 
    double cost;            // cost of one book

    /* Simply clearing zero. */
    Book() {memset(this,0,sizeof(*this));}

    /* Normal copy. */
    Book(const Book &_B) {memcpy(this,&_B,sizeof(_B));}

    /* Normal assign. */
    Book &operator =(const Book &_B) {
        if(this != &_B) {memcpy(this,&_B,sizeof(_B));}
        return *this;
    }

    /* Custom Output for a bookbase object. */
    friend std::ostream &operator <<(std::ostream &os, const Book &book) {
        os << book.ISBN   << '\t' << book.Name   << '\t'
           << book.Author << '\t' << book.Keyword[0];
        for(int i = 1; i < book.keywordNum; ++i) {
            os << '|' << book.Keyword[i];
        }
        return os << std::fixed << std::setprecision(2)
                  << '\t' << book.cost << '\t' << book.quantity;
    }
};


}

#endif
