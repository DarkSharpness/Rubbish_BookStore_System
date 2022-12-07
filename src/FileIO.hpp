#ifndef _FileIO_H_
#define _FILEIO_H_

#include "utils.hpp"
#include <fstream>

namespace dark {

/* Custom File manager. */
template <class T,int BIAS,size_t unit = sizeof(T)>
class File : public std::fstream {
    FileName_t name;

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
        open();
        if(good() && peek() != EOF) return true;
        close(); // close first
        open(std::ios::out);
        close();
        open();
        return true;
    }

    /* Open the given File in binary mode(default). */
    void open(std::ios_base::openmode __mode = (std::ios_base::openmode)28) {
        this->std::fstream::open(name,__mode);
    }

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

    /* Just overwrite one object of any type.
       Not that the cur pointer is not moved.*/
    template <class _T>
    void overwrite(const _T &val,size_t siz = sizeof(_T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
        seekp(-siz,std::ios::cur);
    }
};



}

#endif
