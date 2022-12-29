#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP


#include "utils.hpp"
#include "book.hpp"
#include "account.hpp"
#include "blocklist.hpp"

#include <vector>
#include <map>
#include <bitset>

namespace dark {

class AccountSystem {
  private:
    struct User {
        Level_t Level; // Level of the Account
        ISBN_t   ISBN; // The ISBN of book selected
        UserID_t   ID; // User ID.
        User() = default;
        User(Level_t __L,const char *__I,const char *__ID):
            Level(__L),ISBN(__I),ID(__ID) {}
    };

    std::vector <User>             stack; // User stack.
    
    std::map <UserID_t,size_t>   instack; // Count of user in stack.
    std::vector <Account>            arr; // Return array of Account.
    BlockList <UserID_t,Account> library; // Account library

    Account Account_cache1;
    friend class commandManager;


  public:

    /* Check the current level standard. */
    inline bool checkLevel(int __MIN) const noexcept {
        return (!stack.empty()) && stack.back().Level >= __MIN;
    }

    /* Get the iterator to selected ISBN */
    inline ISBN_t *selected() noexcept {
        if(stack.empty() || stack.back().ISBN.empty()) return nullptr;
        else return &stack.back().ISBN;
    }

    /* Tries to find the User according to __ID.
       Store the data in arr as the return value. */
    bool getUser(const UserID_t &__ID) {
        arr.clear();
        library.findAll(__ID,arr);
        if(arr.empty()) return false;

        // Debug use!
        if(arr.size() > 1) throw "Something unexpected happened in system.hpp";

        return true;
    }

    /* Just clear the user stack. */
    ~AccountSystem() = default;

    /* Just do initialization.*/
    AccountSystem(): library("bin/u1.bin","bin/u2.bin") {
        if(library.empty()) {
            UserID_t __I = "root";
            Account &__tmp = Account_cache1;
            __tmp.init("","sjtu",Level_t::Manager);
            library.insert(__I,__tmp);
        }
    }

    /* Log in an account with password. */
    Exception login(const UserID_t &__ID,const Password_t &__PWD) {
        if(!getUser(__ID))                return Exception("No such User");
        if(__PWD != arr.front().Password) return Exception("Wrong PWD");

        stack.emplace_back(arr.front().Level,"",__ID);
        ++instack[__ID];
        return No_Exception();
    }

    /* Log in an account without password. */
    Exception login(const UserID_t &__ID) {
        if(!checkLevel(Level_t::Librarian))    return Exception("No enough Level");
        if(!getUser(__ID))                     return Exception("No such User");
        if(!checkLevel(arr.front().Level + 1)) return Exception("No enough Level");

        stack.emplace_back(arr.front().Level,"",__ID);
        ++instack[__ID];
        return No_Exception();
    }

    /* Log out an account. */
    Exception logout() {
        if(!checkLevel(Level_t::Customer)) return Exception("Not logged in");

        auto iter = instack.find(stack.back().ID);
        stack.pop_back();
        if(!--iter->second) instack.erase(iter);
        return No_Exception();
    }

    /* Register an account. */
    Exception registering(const UserID_t   &__ID,
                          const Password_t &__PWD,
                          const UserName_t &__Name){
        Account &__tmp = Account_cache1; 
        __tmp.init(__Name,__PWD,Level_t::Customer);

        if(!library.insert(__ID,__tmp)) return Exception("UserID existed");
        else                            return No_Exception();
    }

    /* Change an account's password. */
    Exception changePassword(const UserID_t &__ID,
                             const Password_t &__OLD,
                             const Password_t &__NEW) {
        if(!checkLevel(Level_t::Customer)) return Exception("Not logged in");


        if(!library.modify_if(__ID,
                            [&](Account &X)->bool {
                                if(X.Password != __OLD) return false;
                                X.Password = __NEW;
                                return true;
                            })) {
               return Exception("No such User or Wrong Password");
        } else return No_Exception();
    }

    /* Change an account's password with no old password. */
    Exception changePassword(const UserID_t   &__ID,
                             const Password_t &__NEW) {
        if(!checkLevel(Level_t::Manager)) return Exception("Not enough Level");


        if(!library.modify_if(__ID,
                            [&](Account &__A)->bool { 
                                __A.Password = __NEW;
                                return true;
                            })) {
               return Exception("No such User");
        } else return No_Exception();
    }


    /* Add a new user. */
    Exception addUser(const UserID_t   &__ID,
                      const Password_t &__PWD,
                      Level_t           __L,
                      const UserName_t &__Name) {
        if(!checkLevel(std::max(__L + 1,3))) return Exception("Not enough Level");

        Account &__tmp = Account_cache1;
        __tmp.init(__Name,__PWD,__L);

        if(!library.insert(__ID,__tmp)) return Exception("UserID existed");
        else                            return No_Exception();
    }

    /* Remove a user. */
    Exception removeUser(const UserID_t &__ID) {
        if(!checkLevel(Level_t::Manager))       return Exception("Not enough Level");
        auto iter = instack.find(__ID);
        if(iter != instack.end() && iter->second) return Exception("Can't remove logged");

        Account &__tmp = Account_cache1;
        if(!library.erase(__ID,__tmp))  return Exception("No such User to delete");
        else                            return No_Exception();
    }

    /* Select an ISBN for current User. */
    Exception select(const ISBN_t &__I) {
        if(!checkLevel(Level_t::Librarian)) return Exception("Not enough Level");
        stack.back().ISBN = __I;
        return No_Exception();
    }

    /* Modify OLD ISBN when NEW ISBN is changed. */
    void modifyISBN(const ISBN_t &__OLD,const ISBN_t &__NEW) {
        for(auto &User : stack) {
            if(User.ISBN == __OLD) User.ISBN = __NEW;
        }
    }


};


/**
 * @brief Custom BookSystem for management.
 * There will be no exception for this class.
 * 
 */
class BookSystem {
  private:
    BlockList <ISBN_t    ,Book>   libISBN;
    BlockList <Author_t  ,ISBN_t> libAuthor;
    BlockList <BookName_t,ISBN_t> libBookName;
    BlockList <Keyword_t ,ISBN_t> libKeyword;
    std::vector <Book>   arrB; // Book Return List;
    std::vector <ISBN_t> arrI; // ISBN Return List
    Book Book_cache1;
    double tradeMoney; // Record last trade sum.

    friend class commandManager;

  public:
    ~BookSystem() = default;
    BookSystem(): 
        libISBN     ("bin/b1.bin","bin/b2.bin"),
        libAuthor   ("bin/b3.bin","bin/b4.bin"),
        libBookName ("bin/b5.bin","bin/b6.bin"),
        libKeyword  ("bin/b7.bin","bin/b8.bin") {}


    /* Print the lib. */
    Exception showAll() noexcept {
        libISBN.printAll();
        return No_Exception();
    }

    /* Show by ISBN. */
    Exception showISBN(const ISBN_t &__I) noexcept {
        arrB.clear();
        libISBN.findAll(__I,arrB);
        for(Book &book : arrB) std::cout << book;
        if(arrB.empty())       std::cout << '\n';
        return No_Exception();
    }

    /* Show by Author. */
    Exception showAuthor(const Author_t &__A) noexcept {
        arrI.clear();
        arrB.clear();
        libAuthor.findAll(__A,arrI);
        for(ISBN_t &__I : arrI) libISBN.findAll(__I,arrB);
        for(Book &book : arrB)  std::cout << book;
        if(arrB.empty())        std::cout << '\n';

        return No_Exception();
    }

    /* SHow by BookName. */
    Exception showBookName(const BookName_t &__B) noexcept {
        arrI.clear();
        arrB.clear();
        libBookName.findAll(__B,arrI);
        for(ISBN_t &__I : arrI) libISBN.findAll(__I,arrB);
        for(Book &book : arrB)  std::cout << book;
        if(arrB.empty())        std::cout << '\n';

        return No_Exception();
    }

    /* Show by one Keyword. */
    Exception showKeyword(const Keyword_t &__K) noexcept {
        arrI.clear();
        arrB.clear();
        libKeyword.findAll(__K,arrI);
        for(ISBN_t &__I : arrI) libISBN.findAll(__I,arrB);
        for(Book &book : arrB)  std::cout << book;
        if(arrB.empty())        std::cout << '\n';

        return No_Exception();
    }

    /* Remove elements.(buy books) */
    Exception remove(const ISBN_t &__I,size_t __q) {
        if(!libISBN.modify_if(__I,
                            [&](Book &__B)->bool { 
                                if(__B.quantity < __q) return false;
                                __B.quantity -= __q;
                                tradeMoney = __q * __B.cost; 
                                return true;
                            })) {
            return Exception("No such ISBN or Not enough books");
        } else {
            return No_Exception();
        }
    }

    /* Select one book and create if not existed. */
    Exception select(const ISBN_t &__I) noexcept {
        Book &cur = Book_cache1; // Empty book
        cur.init(__I);
        libISBN.insert(__I,cur);
        return No_Exception();
    }

    /* Get the book with given ISBN */
    Book &getBook(const ISBN_t &__I) noexcept {
        arrB.clear();
        libISBN.findAll(__I,arrB);
        return Book_cache1 = arrB.front();
    }


    /* Modify book with ISBN = _I to another Book. */
    Exception modify(const Book &__B,const std::bitset <6> &optMap) {
        // Now arrB == the Book with given ISBN __I
        // __B holds the Book which has gone through modification.

        const Book &cur = arrB.front();

        if(optMap.test(1)) {
            libISBN.findAll(__B.ISBN,arrB);
            if(arrB.size() != 1) return Exception("ISBN existed");

            libISBN.erase(cur.ISBN,cur);
            libISBN.insert(__B.ISBN,__B);
        } else { // cur.ISBN == __B.ISBN
            libISBN.modify(__B.ISBN,__B);
        }

        if(optMap.test(2)) {
            if(!cur.Author.empty()) libAuthor.erase(cur.Author,cur.ISBN);
            if(!__B.Author.empty()) libAuthor.insert(__B.Author,__B.ISBN);
        }

        if(optMap.test(3)){
            if(!cur.Name.empty()) libBookName.erase(cur.Name,cur.ISBN);
            if(!__B.Name.empty()) libBookName.insert(__B.Name,__B.ISBN);
        }

        if(optMap.test(4)) {
            Keyword_t key;

            if(!cur.Keyword.empty()) {
                const char *str = cur.Keyword;
                while(true) {
                    bool isEnd = !getKeyword((char *)key,str);
                    libKeyword.erase(key,cur.ISBN);
                    if(isEnd) break;
                }
            }

            if(!__B.Keyword.empty()) {
                const char *str = __B.Keyword;
                while(true) {
                    bool isEnd = !getKeyword((char *)key,str);
                    libKeyword.insert(key,__B.ISBN);
                    if(isEnd) break;
                }
            }
        }

        return No_Exception();
    }

    /* Import books with given quantity. */
    Exception import(const ISBN_t & __I,size_t __q) noexcept {
        libISBN.modify_if(__I,
                            [&](Book &__B)->bool {
                                __B.quantity += __q;
                                return true;
                            });
        return No_Exception();
    }

};


}


#endif
