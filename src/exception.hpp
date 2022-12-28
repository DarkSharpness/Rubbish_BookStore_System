#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <string>
#include <iostream>

namespace dark {

class Exception {
  private:
    size_t isException; /* 1:Error || 0:Normal | -1 Exit */
    static std::string message; /* Only one Exception at once. */  
  public:

    /* No Exception */
    explicit Exception() {isException = false;}
    /* Set type of the No Exception. 1: Normal | 0:Exit */
    explicit Exception(bool type) {isException = -(size_t)(type);}
    /* With Exception. */
    Exception(const char *__m) {isException = true;message = __m;}
    /* With Exception. */
    Exception(std::string &&__m) {isException = true;message = std::move(__m);}
    /* With Exception. */
    Exception(const std::string &__m) {isException = true;message = __m;}
    /* Test whether Exception happened.(Only Normal is no exception.) */
    bool test() const {return isException;}
    /* Change to bool and write exception message. */
    operator bool() const{
        static size_t count = 0;
        if(isException == 1ULL)
            std::cout 
                // << "Invalid\n";
                << count++ << ':' <<  message << '\n';
        return ~isException;
    }
};
std::string Exception::message = "Invalid";

/* No Exception Case */
inline Exception No_Exception()   {return Exception();}
inline Exception Exit_Exception() {return Exception(true);}

}



#endif
