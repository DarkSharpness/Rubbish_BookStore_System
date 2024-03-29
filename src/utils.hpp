// Header-only library of utility
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "string.hpp"
#include "exception.hpp"
#include <vector>
#include <map>

namespace dark {



using ISBN_t     = string <24>;
using Author_t   = string <64>;
using BookName_t = string <64>;
using UserID_t   = string <32>;
using UserName_t = string <32>;
using Password_t = string <32>;
using FileName_t = string <32>;
using Keyword_t  = string <64>;

enum Level_t {
    Customer  = 1,
    Librarian = 3,
    Manager   = 7
};

enum Command_t{
    exit    = 0,  // quit/exit
    login   = 1,  // log in 
    logout  = 2,  // log out
    reg     = 3,  // register
    passwd  = 4,  // change password
    userad  = 5,  // add user
    remove  = 6,  // delete account
    show    = 7,  // show by certain tag
    buy     = 8,  // buy something
    select  = 9,  // select a book by ID
    modify  = 10, // modify selected book
    import  = 11, // import book
    show_f  = 12, // show finance
    log     = 13, // show log
    unknown = 114514 // unknown case
};

enum regex_t {
    showISBN     = 1,
    showAuthor   = 2,
    showBookName = 3,
    showKeyword  = 4,
    showPrice    = 5,
    showError    = 0
};


/* Record command. */
const std::map <std::string,Command_t> commandMap = {
    {"exit",Command_t::exit},
    {"quit",Command_t::exit},
    {"su",Command_t::login},
    {"logout",Command_t::logout},
    {"register",Command_t::reg},
    {"passwd",Command_t::passwd},
    {"useradd",Command_t::userad},
    {"delete",Command_t::remove},
    {"show",Command_t::show},
    {"buy",Command_t::buy},
    {"select",Command_t::select},
    {"modify",Command_t::modify},
    {"import",Command_t::import},
    {"finance",Command_t::show_f},
    {"log",Command_t::log}
};

/* Level-String Map */
const std::string levelMap[] = {
    "","Customer","","Librarian",
    "","","","Manager"
};

/* Regex-String Map */
const std::string regMap[] = {
    "","$ISBN$","$Author$","$BookName$","$Keyword$","$Price$"
};



/* Standard compare function for any type. */
template <class T>
int Compare(const T &x,const T &y) {
    if(x < y) return -1;
    else return (y < x);
}

/* Compare 2 char * strings. */
inline int Compare(const char *s1,const char *s2) {
    return strcmp(s1,s2);
}


/* Test whether it's a visible char  */
inline bool isVisibleChar(char __c) {
    return __c > 31 && __c < 127;
}


/* Test whether it's a UserID. */
inline bool isValidUserID(const char *str) {
    size_t count = 0;
    while(*str) {
        if(*str == '_'   || (*str >= 'a' && *str <= 'z')
        || isdigit(*str) || (*str >= 'A' && *str <= 'Z')) {
            ++str,++count;
            continue;
        }
        return false;
    }
    return count <= 30 && count;
}


/* Test whether it's a UserID. */
inline bool isValidPassword(const char *str) {
    return isValidUserID(str);
}

/* Test whether it's a UserName. */
inline bool isValidUserName(const char *str) {
    size_t count = 0;
    while(*str) {
        if(isVisibleChar(*str)) {++str,++count;continue;}
        return false;
    }
    return count <= 30 && count;
}

/* Check ISBN. */
inline bool isValidISBN(const char *str) {
    size_t count = 0;
    while(*str) {
        if(isVisibleChar(*str)) {++str,++count;continue;}
        return false;
    }
    return count <= 20 && count;
}

/* Check BookName. */
inline bool isValidBookName(const char *str) {
    int count = 0;
    while(*str) {
        if(isVisibleChar(*str) && *str != '\"') {++str,++count;continue;}
        return false;
    }
    return count <= 60 && count;
}

/* Check Author. */
inline bool isValidAuthor(const char *str) {
    return isValidBookName(str);
}

/* Check Keyword. */
inline bool isValidKeyword(const char *str) {
    int count  = 0;
    int length = 0; 
    while(*str) {
        ++count;
        if(*str == '|') {
            if(!length) return false;
            length = 0;
            ++str;
            continue;
        }
        if(isVisibleChar(*str) && *str != '\"') {
            ++str,++length;
            continue;
        }
        return false;
    }
    return count <= 60 && length; // Last string shouldn't be 0 in length.
}

/* Check Keyword. */
inline bool isValidOneKeyword(const char *str) {
    int count = 0;
    while(*str) {
        if(isVisibleChar(*str) && *str != '\"' && *str != '|') {
            ++str,++count;
            continue;
        }
        return false;
    }
    return count <= 60 && count;
}

/* Get quantity from a char string */
inline std::pair <bool,size_t> getQuantity(const char *str) {
    if(*str == '0') { // No 0 case is allowed.
        return std::make_pair(false,0ULL);
    }

    int count = 0;
    size_t quantity = 0;
    while(*str) {
        if(!isdigit(*str)) return std::make_pair(false,0ULL);
        quantity = quantity * 10 + (*str ^ '0'); 
        ++str,++count;
    }

    return std::make_pair(quantity < 2147483648ULL && count <= 10 && count,
                          quantity);
}

/* Test whether a char is money type. */
inline bool isValidMoney(const char *str) {
    int count1 = 0; // Digit number.
    int count2 = 0; // Digit number after dot.
    bool dot = false;  // Whether there is dot.

    if(*str == '.' || (*str == '0' && *(str + 1) != '.' && *(str + 1))) {
        return false; // if 0 then 0 or 0.0 or 0.00
    }

    while(*str) {
        ++count1;
        if(*str == '.' && !dot) {
            dot = true;
            ++str;
            continue;
        } else if(!isdigit(*str)) return false;
        if(dot) ++count2;
        ++str;
    }
    return count1 <= 13 && (!dot || count2);
}

/* Get money from a char string */
inline std::pair <bool,double> getMoney(const char *str) {
    bool is = isValidMoney(str);
    return std::make_pair(is,is ? std::stod(str) : 0.0);
}

inline bool isValidLevel(const char *str) {
    return (*str == '1' || *str == '3' || *str == '7') && !*(str + 1);
}


/* Get Priviledge from an str. */
inline Level_t getLevel(const char *str) {
    return Level_t((*str) ^ '0');
}

/* Get one Keyword.False if no more keyword.
   Store the answer in __K(first element). */
inline bool getKeyword(char *__K,const char *&str) {
    size_t index = 0;
    while(*str && *str != '|') {
        __K[index++] = *(str++);
    }
    __K[index] = '\0';
    if(!*str++) return false;
    else        return true; // skip this |
}



/* Check whether prefix of str is src. */
inline bool checkPrefix(const char *str,const char *src) {
    while(*src) {
        if(*(str++) != *(src++)) return false;
    }
    return true;
}

/**
 * @brief Get regex type and store store the answer string in ans .
 * 
 * @param str The string to match.
 * @param ans The string to store answer.
 * @return regex_t The regex matched.
 */
inline regex_t getType(char *str,char *&ans) {
    if(checkPrefix(str,"-ISBN=")) {
        str += 6;
        ans = str;
        if(isValidISBN(ans)) return regex_t::showISBN;
        else                 return regex_t::showError; 
    } else if(checkPrefix(str,"-name=\"")) {
        str += 7;
        size_t length = strlen(ans = str);
        if(ans[length - 1] != '\"') return regex_t::showError;
        ans[length - 1] = 0;
        if(isValidBookName(ans)) return regex_t::showBookName;
        else                     return regex_t::showError;
    } else if(checkPrefix(str,"-author=\"")) {
        str += 9;
        size_t length = strlen(ans = str);
        if(ans[length - 1] != '\"') return regex_t::showError;
        ans[length - 1] = 0;
        if(isValidAuthor(ans)) return regex_t::showAuthor;
        else                   return regex_t::showError;
    } else if(checkPrefix(str,"-keyword=\"")) {
        str += 10;
        size_t length = strlen(ans = str);
        if(ans[length - 1] != '\"') return regex_t::showError;
        ans[length - 1] = 0;
        if(isValidKeyword(ans)) return regex_t::showKeyword;
        else                    return regex_t::showError;
    } else if(checkPrefix(str,"-price=")) {
        str += 7;
        ans = str;
        if(isValidMoney(ans)) return regex_t::showPrice;
        else                  return regex_t::showError;      
    }
    return regex_t::showError;
}

/* Check whether there is repeated Keyword */
inline bool checkRepeatKeyword(const char *str) {
    std::vector <Keyword_t> keys;
    while(true) {
        bool isEnd = !getKeyword((char *)keys.emplace_back(),str);
        for(size_t i = 0 ; i != keys.size() - 1 ; ++i) {
            if(keys[i] == keys.back()) return false;
        }
        if(isEnd) break;
    }
    return true;
}

/* Force to change a double object into standard output string  */
std::string doubleToString(double num) {
    size_t output = num * 100 + 0.5;
    return std::to_string(output / 100) 
        + '.'
        + char((output % 100 / 10) ^ '0')
        + char((output % 10) ^ '0');
}


}


#endif
