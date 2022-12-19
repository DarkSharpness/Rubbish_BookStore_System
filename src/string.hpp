// Header-only library for special string
#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <iostream>
#include <cstring>
#include <utility>

namespace dark {

/**
 * @brief Custom fixed-length string for file output use.
 * 
 * @tparam len Length of the string.
 */
template <size_t len>
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
        if(this != &rhs) { memcpy(str,rhs.str,sizeof(str)); }
        return *this;
    }

    /* Basic copy from another cstr. */
    string &operator =(const char *ptr) {
        memset(str,0,sizeof(str));
        strcpy(str,ptr);
        return *this;
    }

    /* Test whether a string is empty */
    bool operator !(void) const {return str[0];}

    /* Test whether empty */
    bool empty() const {return !str[0];}

    /* Force to change into a cstr. */
    operator const char *() const {return str;}

    /* Reference to data. */

    inline char &operator [](size_t __pos)       {return str[__pos];}
    inline char  operator [](size_t __pos) const {return str[__pos];}

    /* Compare Functions*/

    /* Do as u think. */
    inline bool operator <(const string &rhs) const {return strcmp(str,rhs) < 0;}

    /* Do as u think. */
    inline bool operator ==(const char *rhs) const {return !strcmp(str,rhs);}

    /* Do as u think. */
    inline bool operator !=(const char *rhs) const {return strcmp(str,rhs);}

    /* Compare 2 strings. */
    friend int Compare(const string &lhs,const string &rhs) {
        return strcmp(lhs,rhs);
    }

    /* Input by istream */
    friend std::istream &operator >>(std::istream &is,string &tmp) {
        return is >> tmp.str;
    }

    /* Output by ostream. */
    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};



}

#endif