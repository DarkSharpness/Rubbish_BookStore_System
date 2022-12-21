#ifndef _FileIO_H_
#define _FILEIO_H_

#include "utils.hpp"

#include <fstream>

namespace dark {

/* Custom file with limited wrapping */
class File : public std::fstream {
  private:
    FileName_t name;
  public:
    File() = delete;
    File(const char *_name): name(_name) {}
    ~File() = default;
    /* Directly open the given File. */
    void open(std::ios_base::openmode _mode = (std::ios_base::openmode)28) {
        this->std::fstream::open(name,_mode);
    }
    /* Try creating a new file.
       0 if success || 1 if existed.  */
    bool create() {
        open();
        if(good()) return false;
        close();
        open(std::ios::out);
        close();
        open();
        return true;
    }

    template <class T>
    void read(T &dst,size_t siz = sizeof(T)) {
        this->std::fstream::read(reinterpret_cast <char *> (&dst),siz);
    }

    template <class T>
    void write(const T &src,size_t siz = sizeof(T)) {
        this->std::fstream::write(reinterpret_cast <const char *> (&src),siz);
    }
};


}

#endif
