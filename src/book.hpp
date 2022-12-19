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

    size_t quantity;        // quantitiy of books
    double cost;            // cost of one book
                            // use size_t to avoid error!

    /* Simply clearing zero. */
    Book() {memset((void *)this,0,sizeof(*this));}

    /* Normal copy. */
    Book(const Book &rhs) {memcpy((void *)this,&rhs,sizeof(rhs));}

    Book(const ISBN_t &__I,const BookName_t __B,
         const Author_t &__A,const Keyword_t &__K,
         size_t __q,double __c): 
        ISBN(__I),Name(__B),Author(__A),Keyword(__K),
        quantity(__q),cost(__c)  {}

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

    /* Custom Output for a bookbase object.(With '\n') */
    friend std::ostream &operator <<(std::ostream &os, const Book &book) {
        os << book.ISBN   << '\t' << book.Name   << '\t'
           << book.Author << '\t' << book.Keyword[0];
        for(size_t i = 1; i < book.Keyword.size(); ++i) {
            os << '|' << book.Keyword[i];
        }
        return os 
                //   << '\t' << book.cost / 100 << '.' << book.cost % 100
                  << '\t' << std::fixed << std::setprecision(2) << book.cost
                  << '\t' << book.quantity << '\n';
    }
};


}

#endif
