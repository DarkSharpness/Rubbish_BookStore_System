#ifndef _FileIO_H_
#define _FILEIO_H_

#include "string.hpp"
#include <fstream>

namespace dark {

/// TODO: Complete File Manager.
struct File : std::fstream {
    string <32> name;
    File() = delete;
    File(const char *_name) : name(_name) {}
    File(const string <32> &_name) : name(_name) {}
    ~File() {}

    

    /* Open a new file and change to write/read mode. 
       True  if succesfully created. 
       False if already existed.*/
    bool create() {
        open();
        if(!good()) {
            open(std::ios::out);
            close();
            open();
            return true;
        }
        bool flag = peek() == EOF;
        return flag;
    }


    /* Open the given File in binary mode(default). */
    void open(std::ios_base::openmode __mode = (std::ios_base::openmode)28) {
        this->std::fstream::open(name,__mode);
    }

    /* Just read into any type of ptr. */
    template <class T>
    void read(T *ptr,size_t siz = sizeof(T)) {
        this->std::fstream::read(reinterpret_cast <char *>(ptr),siz);
    }

    template <class T>
    void write(const T &val,size_t siz = sizeof(T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
    }


};



}

#endif
