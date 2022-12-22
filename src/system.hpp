#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include "utils.hpp"
#include "book.hpp"
#include "account.hpp"
#include "blocklist.hpp"


#include <vector>
// #include <stack>

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
    Account Account_cache2;
    friend class commandManager;


  public:

    /* Check the current level standard. */
    inline bool checkLevel(int __MIN) const {
        return (!stack.empty()) && stack.back().Level >= __MIN;
    }

    inline const ISBN_t *selected() {
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
    AccountSystem(): library("user1.bin","user2.bin") {
        if(library.empty()) {
            UserID_t __I = "root";
            Account &__tmp = Account_cache1;
            __tmp.init("","sjtu",Level_t::Manager);
            library.insert(__I,__tmp);
        }
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
        if(!checkLevel(Level_t::Librarian))    return Exception("Invalid");
        if(!getUser(__ID))                     return Exception("Invalid");
        if(!checkLevel(arr.front().Level + 1)) return Exception("Invalid");

        stack.emplace_back(arr.front().Level,"",__ID);
        ++instack[__ID];
        return No_Exception();
    }

    /* Log out an account. */
    Exception logout() {
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");

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

        if(!library.insert(__ID,__tmp)) return Exception("Invalid");
        else                            return No_Exception();
    }

    /* Change an account's password. */
    Exception changePassword(const UserID_t &__ID,
                             const Password_t &__OLD,
                             const Password_t &__NEW) {
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");


        if(!library.modify_if(__ID,
                            [&](Account &X)->bool {
                                if(X.Password != __OLD) return false;
                                X.Password = __NEW;
                                return true;
                            })) {
               return Exception("Invalid");
        } else return No_Exception();
    }

    /* Change an account's password with no old password. */
    Exception changePassword(const UserID_t   &__ID,
                             const Password_t &__NEW) {
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");

        Account &__new = Account_cache1;
        __new.Password = __NEW;

        if(!library.modify(__ID,__new)) {
               return Exception("Invalid");
        } else return No_Exception();
    }


    /* Add a new user. */
    Exception addUser(const UserID_t   &__ID,
                      const Password_t &__PWD,
                      Level_t           __L,
                      const UserName_t &__Name) {
        if(!checkLevel(__L + 1)) return Exception("Invalid");

        Account &__tmp = Account_cache1;
        __tmp.init(__Name,__PWD,__L);

        if(!library.insert(__ID,__tmp)) return Exception("Invalid");
        else                            return No_Exception();
    }

    /* Remove a user. */
    Exception removeUser(const UserID_t &__ID) {
        if(!checkLevel(Level_t::Manager))       return Exception("Invalid");
        if(instack.find(__ID) != instack.end()) return Exception("invalid");

        Account &__tmp = Account_cache1;
        if(!library.erase(__ID,__tmp))  return Exception("Invalid");
        else                            return No_Exception();
    }

    /* Select an ISBN for current User. */
    Exception select(const ISBN_t &__I) {
        if(!checkLevel(Level_t::Customer)) return Exception("Invalid");

        stack.back().ISBN = __I;
        return No_Exception();
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
    Book Book_cache2;
    friend class commandManager;

  public:
    ~BookSystem() = default;
    BookSystem(): 
        libISBN("b1.bin","b2.bin"),
        libAuthor("b3.bin","b4.bin"),
        libBookName("b5.bin","b6.bin"),
        libKeyword("b7.bin","b8.bin") {
    }


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
                            [=](Book &__B)->bool { 
                                if(__B.quantity < __q) return false;
                                __B.quantity -= __q;
                                return true;
                            })) {
            return Exception("Invalid");
        } else {
            return No_Exception();
        }
    }

    /* Select one book and create if not existed. */
    Exception select(const ISBN_t &__I) noexcept {
        Book &cur    = Book_cache1; // Empty book
        cur.init(__I);
        libISBN.insert(__I,cur);
        return No_Exception();
    }

    /* Modify book with ISBN = _I to another Book. */
    Exception modify(const ISBN_t &__I,const Book &__B) noexcept {
        if(__I == __B.ISBN) { // modify Book only.
            libISBN.modify(__I,__B);
        } else {
            arrB.clear();
            libISBN.findAll(__I,arrB);
            const Book &cur = arrB.front();

            libISBN.erase(__I,cur);
            libISBN.insert(__I,__B);

            libAuthor.erase(cur.Author,__I);
            libAuthor.insert(cur.Author,__B.ISBN);
            libBookName.erase(cur.Name,__I);
            libBookName.insert(cur.Name,__B.ISBN);

            Keyword_t key;
            const char *str = __B.Keyword;
            while(true) {
                bool isEnd = !getKeyword(key,str);
                libKeyword.erase(key,__I);
                libKeyword.insert(key,__B.ISBN);
                if(isEnd) break;
            }
        }
        return No_Exception();
    }

    /* Import books with given quantity. */
    Exception import(const ISBN_t & __I,size_t __q) noexcept {
        libISBN.modify_if(__I,
                            [=](Book &__B)->bool {
                                __B.quantity += __q;
                                return true;
                            });
        return No_Exception();
    }

};






}


#endif
