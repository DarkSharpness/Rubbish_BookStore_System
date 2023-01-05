#ifndef _ACCOUNT_HPP_
#define _ACCOUNT_HPP_

#include "utils.hpp"

namespace dark {

/* Custom account without userID. */
struct Account {
  public:
    UserName_t Name;     // User Name
    Password_t Password; // Password of the Account
    Level_t    Level;    // Priority Level


    /* Init an account with Name,Password and Level  */
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

};

}

#endif
