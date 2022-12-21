#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include "utils.hpp"
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
    BlockList <ISBN_t    ,Book> libISBN;     // ISBN to LOCATION
    BlockList <Author_t  ,Book> libAuthor;
    BlockList <BookName_t,Book> libBookName;
    BlockList <Keyword_t ,Book> libKeyword;
  public:
    ~BookSystem() = default;
    BookSystem(): 
        libISBN("b1.bin","b2.bin"),
        libAuthor("b3.bin","b4.bin"),
        libBookName("b5.bin","b6.bin"),
        libKeyword("b7.bin","b8.bin") {
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
    struct User {
        Level_t Level; // Level of the Account
        ISBN_t   ISBN; // The ISBN of book selected
        UserID_t   ID; // User ID.
        User() = default;
        User(Level_t __L,const char *__I,const UserID_t &__ID):
            Level(__L),ISBN(__I),ID(__ID) {}
    };

    std::vector <User>           stack;   // User stack.
    std::map <UserID_t,size_t> instack;   // Count of user in stack.
    std::vector <Account>          arr;   // Return array
    BlockList <UserID_t,Account> library; // Account library
    
    Account Account_cache1;
    Account Account_cache2;


  public:

    /* Check the current level standard. */
    inline bool checkLevel(int __MIN,bool type = false) const {
        if(type) return (!stack.empty()) && stack.back().Level <  __MIN;
        else     return (!stack.empty()) && stack.back().Level >= __MIN;
    }


    /* Tries to find the User according to __ID.
       Store the data in arr. */
    bool getUser(const UserID_t &__ID) {
        arr.clear();
        library.findAll(__ID,arr);
        if(arr.empty()) return false;
    }

    /* Just clear the user stack. */
    ~AccountSystem() = default;

    /* Just do nothing.*/
    AccountSystem(): library("user1.bin","user2.bin") {
        UserID_t ID("root");
        Account Init("","",Level_t::Manager);
        if(library.empty()) library.insert(ID,Init);
    }

    /* Log in an account with password. */
    Exception login(const UserID_t &__ID,const Password_t &__PWD){
        if(!getUser(__ID))                return Exception("Invalid");
        if(__PWD != arr.front().Password) return Exception("Invalid");

        stack.emplace_back(arr.front().Level,"",__ID);
        ++instack[__ID];
        return No_Exception();
    }

    /* Log in an account without password. */
    Exception login(const UserID_t &__ID) {
        if(stack.empty())                      return Exception("Invalid");
        if(!getUser(__ID))                     return Exception("Invalid");
        if(!checkLevel(arr.front().Level + 1)) return Exception("Invalid");

        stack.emplace_back(arr.front().Level,"",__ID);
        ++instack[__ID];
        return No_Exception();
    }

    /* Log out an account. */
    Exception logout() {
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");

        --instack[stack.back().ID];
        stack.pop_back();
        return No_Exception();
    }

    /* Register an account. */
    Exception registering(const UserID_t   &__ID,
                          const Password_t &__PWD,
                          const UserName_t &__Name){
        Account &__tmp = Account_cache1; 
        __tmp.init(__Name,__PWD,Level_t::Customer);

        if(!library.insert(__ID,__tmp)) return Exception("Invalid");
        else                            return No_Exception();
    }

    /* Change an account's password. */
    Exception changePassword(const UserID_t &__ID,
                             const Password_t &__OLD,
                             const Password_t &__NEW){
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");

        Account &__old = Account_cache1;
        __old.Password = __OLD;
        Account &__new = Account_cache2;
        __new.Password = __NEW;

        if(!library.modify_if(__ID,__old,__new,__old,
                            [](const Account &X,const Account &Y)->bool{
                                return X.PWD() == Y.PWD();
                            })) {
               return Exception("Invalid");
        } else return No_Exception();
    }

    /* Change an account's password. */
    Exception changePassword(const UserID_t &__ID,
                             const Password_t &__NEW){
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");
        
        Account &__old = Account_cache1;
        Account &__new = Account_cache2;
        __new.Password = __NEW;
    
        if(!library.modify(__ID,__old,__new)) {
               return Exception("Invalid");
        } else return No_Exception();
    }


    /* Add a new user. */
    Exception insert(const UserID_t   &__ID,
                     const Password_t &__PWD,
                     Level_t           __L,
                     const UserName_t &__Name) {
        if(!checkLevel(__L,true)) return Exception("Invalid");

        Account &__tmp = Account_cache1;
        __tmp.init(__Name,__PWD,__L);

        if(!library.insert(__ID,__tmp)) return Exception("Invalid");
        else                            return No_Exception();
    }


    /* Remove a user. */
    Exception remove(const UserID_t &__ID) {
        if(!checkLevel(Level_t::Manager)) return Exception("Invalid");

        Account &__tmp = Account_cache1;
        if(!library.erase(__ID,__tmp))  return Exception("Invalid");
        else                            return No_Exception();
    }

};






}


#endif
