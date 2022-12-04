#ifndef _FILE_H_
#define _FILE_H_

#include "string.hpp"
#include <fstream>

namespace dark {

class File {
  private:
    string <64> fileLoc;
    std::fstream data;
  public:
    File(const string <64> &_Loc) {fileLoc = _Loc;}

    /* Create a new file at given location. */
    void create() {
        data.open(fileLoc,std::ios::out);
        data.close();
    }

    /* Judge whether a file exists. */
    bool exist() {
        data.open(fileLoc);
        bool ans = data.good();
        data.close();
        return ans;
    }



};




}


#endif
