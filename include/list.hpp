// Header-only library for A unrolled linklist
#ifndef _LIST_HPP_
#define _LIST_HPP_

#include "string.hpp"
#include <vector>

namespace dark {


class List {
 public:
  List (const char *filename);
  ~List ();
  void insert (const string &key, const int &value);
  void remove (const string &key, const int &value);
  std::vector<int> find (const string &key);
  std::vector<std::pair<string, int>> findAll ();
  



};

}

#endif
