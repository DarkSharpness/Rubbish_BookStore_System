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

    /* Must provide params */
    Account() = delete;

    /* Initialize function.*/
    Account(const UserName_t& _N,const Password_t &_P,Level_t _L):
        Name(_N),Password(_P),Level(_L) {}

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
