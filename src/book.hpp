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
    Keyword_t  Keyword;     // Keyword

    size_t quantity;        // quantitiy of books
    double cost;            // cost of one book

    /* Simply clearing zero. */
    Book() {memset((void *)this,0,sizeof(*this));}

    /* Normal copy. */
    Book(const Book &rhs) {memcpy((void *)this,&rhs,sizeof(rhs));}

    /* Normal assign. */
    Book &operator =(const Book &rhs) {
        if(this != &rhs) {memcpy((void *)this,&rhs,sizeof(rhs));}
        return *this;
    }

    /* Compare by ISBN only */
    int Compare(const Book &rhs) const{
        return strcmp(ISBN,rhs.ISBN);
    }

    /* Compare by ISBN only */
    bool operator <(const Book &rhs) const {
        return strcmp(ISBN,rhs.ISBN) < 0;
    }

    /* Custom Output for a bookbase object.(With \\n) */
    friend std::ostream &operator <<(std::ostream &os, const Book &book) {
        os << book.ISBN   << '\t' << book.Name   << '\t'
           << book.Author << '\t' << book.Keyword;
        return os
                //  << '\t' << book.cost / 100 << '.' << book.cost % 100
                  << '\t' << std::fixed << std::setprecision(2) << book.cost
                  << '\t' << book.quantity << '\n';
    }
    /* Init a book with only ISBN. */
    void init(const char *__ISBN) {
        ISBN    = __ISBN;
        Name    = "";
        Author  = "";
        Keyword = "";
        cost    = quantity = 0;
    }
};


}

#endif
