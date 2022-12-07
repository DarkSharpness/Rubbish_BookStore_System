#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include "book.hpp"
#include "account.hpp"
#include "blocklist.hpp"

#include <vector>
// #include <stack>

namespace dark {

class BookSystem {
  private:
    BlockList <ISBN_t,BookBase> library;
  public:
    ~BookSystem() = default;
    BookSystem(): library("book1.bin","book2.bin","book3.bin") {}

    /* Show by ISBN. */
    void show(const ISBN_t &) {}
    /* Show by non-ISBN. */
    void show(const string <64> &,show_t opt){}
    /* Remove elements.(buy books) */
    void remove(const ISBN_t &,int count) {}
    /* Select one book. */
    void select(const ISBN_t &) {}
    /* Modify with given params. */
    void modify(const ISBN_t &,const BookBase &) {}
    /* Import books with given quantity and cost */
    void import(const ISBN_t &,size_t quantity,double cost) {}

};



class AccountSystem {
  private:
    BlockList <UserID_t,Account> library;
    struct User {      
        int     index; // Index in the vector
        Level_t Level; // Level of the people
        ISBN_t  ISBN;  // The ISBN of book selected
    };
    // std::stack <User,std::vector<User>> stack;

    std::vector <User> stack; // User stack.

  public:
    /* Just clear the user stack. */
    ~AccountSystem() = default;
    /* Just do nothing.*/
    AccountSystem(): library("user1.bin","user2.bin","user3.bin") {}

    /* Log in an account. */
    void login(const UserID_t &,const Password_t &){}
    /* Log out an account. */
    void logout(){}
    /* Register an account. */
    void registering(const UserID_t &,
                     const Password_t &,
                     const UserName_t &){}
    /* Change an account's password. */
    void changePassword(const UserID_t &,
                        const Password_t &,
                        const Password_t &){}
    /* Add a new user. */
    void insert(const UserID_t &,
                const Password_t &
                Level_t,
                const UserName_t &){}
    /* Remove a user. */
    void remove(const UserID_t &){}
};






}


#endif
