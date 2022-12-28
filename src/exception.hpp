#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <string>
#include <iostream>

namespace dark {

class Exception {
  private:
    bool   isException; /* 1:Error || 0:Normal */
    static std::string message; /* Only one Exception at once. */

  public:
    /* No Exception */
    Exception() {isException = false;}
    /* With Exception. */
    Exception(const char *__m) {isException = true;message = __m;}
    /* With Exception. */
    Exception(std::string &&__m) {isException = true;message = std::move(__m);}
    /* With Exception. */
    Exception(const std::string &__m) {isException = true;message = __m;}
    /* Test whether Exception happened. */
    bool test() const {return isException;}
    /* Change to bool and write exception message. */
    operator bool() const{
        if(test()) 
            std::cout 
                // << "Invalid\n";
                << message << '\n';
        return true;
    }
};
std::string Exception::message = "Invalid";

/* No Exception Case */
inline Exception No_Exception() {return Exception();}

}



#endif
