#ifndef _LOG_HPP_
#define _LOG_HPP_

#include "utils.hpp"

#include <fstream>

namespace dark {

/* Custom log writer. */
class LogWriter : public std::fstream{
  private:
    FileName_t name;
  public:
    LogWriter() = delete;
    LogWriter(const char *_name) : name(_name) {}
    LogWriter(const FileName_t &_name) : name(_name) {}
    ~LogWriter() {}
    
    /* Just read one of any type.*/
    template <class _T>
    void read(_T *ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(ptr),sizeof(_T));
    }

    /* Just write one object of any type.*/
    template <class _T>
    void write(const _T &val,size_t siz = sizeof(_T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
    }

};


}

#endif
