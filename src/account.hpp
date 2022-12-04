#ifndef _ACCOUNT_HPP_
#define _ACCOUNT_HPP_

#include "string.hpp"


namespace dark {

class Account {
  private:
    string <32> ID;       // User ID
    string <32> Name;     // User Name
    string <32> Password; // Password of the Account
    size_t Level;         // Priority Level

    friend class AccountSystem;
  public:
    Account() {Level = 0;}
    Account(const string <32> &_I,const string <32>& _N,
            const string <32> &_P,size_t _L) {
        ID = _I;
        Name = _N;
        Password = _P;
        Level = _L;
    }
};

}

#endif
