#ifndef _FileIO_H_
#define _FILEIO_H_

#include "utils.hpp"
#include <fstream>

namespace dark {

/* Custom File manager. */
template <class _T,int BIAS,size_t unit = sizeof(_T)>
class File : public std::fstream {
    using FileName_t = dark::string <24>;
    const FileName_t name;

  public:
    File() = delete;
    File(const char *_name) : name(_name) {}
    File(const FileName_t &_name) : name(_name) {}
    ~File() {}

    /* Locate read position. */
    void locateI(int __n) {
        seekg(BIAS + __n * unit);
    }

    /* Locate write position. */
    void locateO(int __n) {
        seekp(BIAS + __n * unit);
    }


    /* Open a new file and change to write/read mode. 
       True  if succesfully created. 
       False if already existed.*/
    bool create() {
        close();
        open();
        this->std::fstream::seekg(0);
        peek();
        if(good()) return false;
        close(); // close first
        open(std::ios::out);
        close();
        open();
        return true;
    }

    /* Open the given File in binary mode(default). */
    void open(std::ios_base::openmode mode = (std::ios_base::openmode)24) {
        this->std::fstream::open(name,mode);
    }

    /* Just read one of _T. */
    void read(_T &ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(&ptr),sizeof(_T));
    }

    void read(int &ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(&ptr),sizeof(int));
    }

    /* Just write one of _T. */
    void write(const _T &val,size_t siz = sizeof(_T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
    }
};


}

#endif
