// Header-only library for BOOK
#ifndef _BOOK_HPP_
#define _BOOK_HPP_

#include "utils.hpp"
#include <iomanip>

namespace dark {

/* Book without ISBN. */
struct BookBase {

    BookName_t Name;        // Name
    Author_t   Author;      // Author
    Keyword_t  Keyword[60]; // Keywords

    size_t keywordNum;      // number of keywords
    size_t quantity;        // quantitiy of books 
    double cost;            // cost of one book
    
    /* Simply clearing zero. */
    BookBase() {memset(this,0,sizeof(*this));}

    /* Normal copy. */
    BookBase(const BookBase &_B) {memcpy(this,&_B,sizeof(_B));}
    /* Normal assign. */
    BookBase &operator =(const BookBase &_B) {
        if(this != &_B) {memcpy(this,&_B,sizeof(_B));}
        return *this;
    }
    /* Custom Output for a bookbase object. */
    friend std::ostream &operator <<(std::ostream &os, const BookBase &book) {
        os << book.Name   << '\t'
           << book.Author << '\t'
           << book.Keyword[0];
        for(int i = 1; i < book.keywordNum; ++i)
            os << '|' << book.Keyword[i];
        return os << std::fixed << std::setprecision(2)
                  << '\t' << book.cost << '\t' << book.quantity;
    }

};

// /* Book with ISBN. */
// struct Book : BookBase{
//     ISBN_t ISBN; // ISBN
//     Book() = delete; // must Initialize with ISBN.
// 
//     /* Normal copy. */
//     Book(const Book &_B) {memcpy(this,&_B,sizeof(_B));}
//     /* Normal assign. */
//     Book &operator =(const Book &_B) {
//         if(this != &_B) {memcpy(this,&_B,sizeof(_B));}
//         return *this;
//     }
// 
//     /* Initialize an empty book with a ISBN. */
//     Book(const ISBN_t &_I) : BookBase(),ISBN(_I) {}
//     /* Initialize by merging ISBN and BookBase. */
//     Book(ISBN_t &_I,const BookBase &_B) : ISBN(_I),BookBase(_B) {}
// 
// 
// };


// constexpr size_t t = sizeof(Book);


}

#endif
