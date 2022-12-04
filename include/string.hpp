// Header-only library for special string
#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <iostream>
#include <cstring>

namespace dark {


template <unsigned len>
class string {
  private:
    // static constexpr int len = 24;
    char str[len];
 
  public:
    inline char &operator [](int _pos) {return str[_pos];}
    inline char operator  [](int _pos) const{return str[_pos];}

    inline char *begin() {return str;}
    inline const char *begin() const{return str;}
    inline const char *cbegin() const{return str;}

    inline char *end() {return str + len;}
    inline const char *end() const{return str + len;}
    inline const char *cend() const{return str + len;}

    inline bool operator <(const string &rhs) const{return strcmp(str,rhs.str) < 0;}
    inline bool operator >(const string &rhs) const{return strcmp(str,rhs.str) > 0;}
    inline bool operator <=(const string &rhs)const{return strcmp(str,rhs.str) <= 0;}
    inline bool operator >=(const string &rhs)const{return strcmp(str,rhs.str) >= 0;}
    inline bool operator ==(const string &rhs)const{return strcmp(str,rhs.str) == 0;}
    inline bool operator !=(const string &rhs)const{return strcmp(str,rhs.str) != 0;}



};



}
#endif