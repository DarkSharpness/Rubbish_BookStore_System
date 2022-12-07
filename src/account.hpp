#ifndef _ACCOUNT_HPP_
#define _ACCOUNT_HPP_

#include "utils.hpp"


namespace dark {


/* Custom account. */
class Account {
  private:
    UserID_t   ID;       // User ID
    UserName_t Name;     // User Name
    Password_t Password; // Password of the Account
    Level_t    Level;    // Priority Level
    size_t     count;    // Times of log-in

    friend class AccountSystem;
  public:

    /* Must provide params */
    Account() = delete;
    
    /* Initialize function.*/
    Account(const UserID_t &_I,const UserName_t& _N,
            const Password_t &_P,Level_t _L,size_t _C):
        ID(_I),Name(_N),Password(_P),Level(_L),count(_C) {}
};

}

#endif
