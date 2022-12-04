// Header-only library for special string
#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <iostream>
#include <cstring>
#include <utility>

namespace dark {

/**
 * String of a fixed length.
 * With basic string functions.
 * 
 * @tparam len The maximum lenght of a string
 */
template <int len>
class string {
  private:
    char str[len];

  public:

    /* Basic functions. */

    string() {memset(str,0,sizeof(str));}
    string(const char *ptr) :string() {strcpy(str,ptr);}

    ~string() = default;
    string &operator =(const string &rhs) {
        if(this == &rhs) return *this;
        memcpy(str,rhs.str,sizeof(rhs));
        return *this;
    }
    string &operator =(const char *ptr) {
        memset(str,0,sizeof(str));
        strcpy(str,ptr);
        return *this;
    }

    // explicit 
    operator const char *() {
        return str;
    }

    inline char &operator [](int _pos)      {return str[_pos];}
    inline char  operator [](int _pos) const{return str[_pos];}

    /* Pointer as Iterators */

    inline char *begin()             {return str;}
    inline const char *begin()  const{return str;}
    inline const char *cbegin() const{return str;}
    inline char *end()             {return str + len;}
    inline const char *end()  const{return str + len;}
    inline const char *cend() const{return str + len;}

    /* Compare Functions*/

    inline bool operator <(const string &rhs) const{return strcmp(str,rhs.str) < 0;}
    inline bool operator >(const string &rhs) const{return strcmp(str,rhs.str) > 0;}
    inline bool operator <=(const string &rhs)const{return strcmp(str,rhs.str) <= 0;}
    inline bool operator >=(const string &rhs)const{return strcmp(str,rhs.str) >= 0;}
    inline bool operator ==(const string &rhs)const{return strcmp(str,rhs.str) == 0;}
    inline bool operator !=(const string &rhs)const{return strcmp(str,rhs.str) != 0;}
    friend int Compare(const string &lhs,const string &rhs) {
        return strcmp(lhs.str,rhs.str);
    }

    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};



}
#endif