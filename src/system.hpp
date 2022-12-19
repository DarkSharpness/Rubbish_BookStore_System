#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include "book.hpp"
#include "account.hpp"
#include "blocklist.hpp"


#include <vector>
// #include <stack>

namespace dark {

/**
 * @brief Custom BookSystem for management.
 * 
 */
class BookSystem {
  private:
    BlockList <ISBN_t    ,size_t> libISBN;     // ISBN to LOCATION
    BlockList <Author_t  ,ISBN_t> libAuthor;
    BlockList <BookName_t,ISBN_t> libBookName;
    BlockList <Keyword_t ,ISBN_t> libKeyword;
    File BookData;
  public:
    ~BookSystem() = default;
    BookSystem(): 
        libISBN("b1.bin","b2.bin"),
        libAuthor("b3.bin","b4.bin"),
        libBookName("b5.bin","b6.bin"),
        libKeyword("b7.bin","b8.bin"),
        BookData("book.data") {
            
    
    }

    /* Show by ISBN. */
    Exception show(const ISBN_t &) {}
    /* Show by non-ISBN. */
    Exception show(const string <64> &,show_t opt){}
    /* Remove elements.(buy books) */
    Exception remove(const ISBN_t &,int count) {}
    /* Select one book. */
    Exception select(const ISBN_t &) {}
    /* Modify with given params. */
    Exception modify(const ISBN_t &,const Book &) {}
    /* Import books with given quantity and cost */
    Exception import(const ISBN_t &,size_t quantity,double cost) {}

};


class AccountSystem {
  private:
    using map = BlockList <UserID_t,Account>;
    map library;
    struct User {
        Level_t Level; // Level of the people
        ISBN_t   ISBN; // The ISBN of book selected
        UserID_t   ID; // User ID.
        User() = default;
        User(Level_t __L,const char *__I,const UserID_t &__ID):
            Level(__L),ISBN(__I),ID(__ID) {}
    };

    std::vector <User>           stack; // User stack.
    std::map <UserID_t,size_t> instack; // Count of user in stack.
    
    /* Initialize the vector by one element. */
    void init() {
        stack.emplace_back(0,"","");
        ++instack[""]; // A name that won't be identical.
    }
  
  public:
    /* Just clear the user stack. */
    ~AccountSystem() = default;
    /* Just do nothing.*/
    AccountSystem(): library("user1.bin","user2.bin") { init(); }
    /* Log in an account. */
    Exception login(const UserID_t &__ID,const Password_t &__PWD){
        static map::Return_Array arr;
        if(__PWD.empty() && stack.back().Level < 7) {
            return Exception("Invalid");
        }
        arr.clear();
        library.findAll(__ID,arr);
        if(arr.empty()) return Exception("Invalid");    
        if(__PWD.empty() || __PWD == arr.front().Password) {
            stack.emplace_back(arr.front().Level,"",__ID);
            ++instack[__ID];
            return No_Exception();
        } else {
            return Exception("Invalid");
        }
    }
    /* Log out an account. */
    Exception logout() {
        if(stack.size() == 1) return Exception("Invalid");
        --instack[stack.back().ID];
        stack.pop_back();
        if(stack.empty()) {init();}
        return No_Exception();
    }
    /* Register an account. */
    Exception registering(const UserID_t   &__ID,
                          const Password_t &__PWD,
                          const UserName_t &__Name){
        Account tmp(__Name,__PWD,Level_t::Customer);
        if(library.insert(__ID,tmp)) {

        } else {
            return Exception("Invalid");
        }
    }
    /* Change an account's password. */
    Exception changePassword(const UserID_t &,
                             const Password_t &,
                             const Password_t &){}
    /* Add a new user. */
    Exception insert(const UserID_t   &,
                     const Password_t &,
                     Level_t,
                     const UserName_t &){}
    /* Remove a user. */
    Exception remove(const UserID_t &){}
};






}


#endif
