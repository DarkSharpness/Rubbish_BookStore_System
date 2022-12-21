#ifndef _ACCOUNT_HPP_
#define _ACCOUNT_HPP_

#include "utils.hpp"


namespace dark {


/* Custom account without userID. */
class Account {
  private:
    UserName_t Name;     // User Name
    Password_t Password; // Password of the Account
    Level_t    Level;    // Priority Level

    friend class AccountSystem;
  public:
    void init(const char * __N,const char *__P,Level_t __L) {
        Name     = __N;
        Password = __P;
        Level    = __L;
    }

    /* Must provide params */
    Account() = default;

    /* Initialize function.*/
    Account(const char * __N,const char *__P,Level_t __L):
        Name(__N),Password(__P),Level(__L) {}

    /* All equal. */
    friend int Compare(const Account &lhs,const Account &rhs) {
        return 0;
    }
    /* All equal. */
    friend bool operator < (const Account &lhs,const Account &rhs) {
        return false;
    }
    const Password_t &PWD() const {return Password;}
    Level_t LVL() const {return Level;}

};

}

#endif
