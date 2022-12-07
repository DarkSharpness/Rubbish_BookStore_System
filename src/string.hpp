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
 * @tparam len The maximum length of a string
 * 
 */
template <int len>
class string {
  private:
    char str[len];

  public:

    /* Basic functions. */

    /* Initialize by nothing. */
    string() {}
    /* Initialize as empty. */
    string(std::nullptr_t) {memset(str,0,sizeof(str));}
    /* Initialize by another cstr. */
    string(const char *ptr) {strcpy(str,ptr);}
    /* Nothing to do. */
    ~string() = default;

    /* Basic copy from another string. */
    string &operator =(const string &rhs) {
        if(this == &rhs) return *this;
        memcpy(str,rhs.str,sizeof(rhs));
        return *this;
    }

    /* Test whether a string is empty */
    bool operator !(void) const{return str[0];}

    /* Basic copy from another cstr. */
    string &operator =(const char *ptr) {
        memset(str,0,sizeof(str));
        strcpy(str,ptr);
        return *this;
    }

    /* Force to change into a cstr. */
    operator const char *() const{
        return str;
    }

    /* Reference to data. */

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

    template <int _len>
    inline bool operator <(const string<_len> &rhs) const{return strcmp(str,rhs) < 0;}
    
    
    template <int _len>
    friend int Compare(const string &lhs,const string<_len> &rhs) {
        return strcmp(lhs,rhs);
    }

    /* Output by ostream. */

    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};



}

#endif