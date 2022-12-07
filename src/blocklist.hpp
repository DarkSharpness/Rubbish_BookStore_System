#ifndef _BLOCKLIST_HPP_
#define _BLOCKLIST_HPP_

#include "string.hpp"
#include "book.hpp"
#include "FileIO.hpp"
#include <memory>

namespace dark {



/**
 * Special Block List for Bookstore 
 * Data are stored in the following way:
 * 
 * nodeM <-> nodeM <-> nodeM <-> nodeM...
 *   |-> nodeL <-> nodeL <-> nodeL...
 *         |-> value_t
 * Free value_t nodes are stored.
 * 
*/
template <class key_t,class value_t>
class BlockList {
  private:
    // using value_t  = BookBase;
    // using key_t    = string <24>; // Debug use

 
    #define MIN_LEN  2
    #define MAX_LEN  6
    #define MAP_BIAS  8 + sizeof(nodeM) // Leave one vaccancy
    #define LIST_BIAS 0 + sizeof(nodeL) // Leave one vaccancy
    #define DATA_BIAS 0 
    #define BLOCK_SIZ 7

    /* Node of the map. */
    struct nodeM {
        int nxt;   // next pointer
        int pre;   // prev pointer
        int index; // index of the list first node
        int count; // count of the list nodes.
        key_t key; // maximum key of the block
        nodeM(key_t _k = "",int _n = -1,int _p = -1,int _i = -1,int _c = 0):
            key(_k),nxt(_n),pre(_p),index(_i),count(_c) {}
    };

    /* Node of the list. */
    struct nodeL {
        int nxt;   // next pointer
        int pre;   // prev pointer
        int index; // index of the data
        key_t key; // key of the data
        nodeL(key_t _k = "",int _n = -1,int _p = -1,int _i = -1):
            key(_k),nxt(_n),pre(_p),index(_i) {}
    };

    File <nodeM  ,MAP_BIAS > map ; // Map  of the nodes. 
    File <nodeL  ,LIST_BIAS> list; // List of the nodes.
    File <value_t,DATA_BIAS> data; // Data of the value.

    friend class Iterator;

  public:

    /* Iterator of one node's value. */
    class Iterator {
      private:
        BlockList *ptr; // Father BlockList
        int index[3];   // map/list/data index
      public:
        /* Initialize. */
        Iterator(BlockList *_ptr = nullptr,
                 int idx1 = -1,int idx2 = -1,int idx3 = -1):
            ptr(_ptr),index({idx1,idx2,idx3}) {} 
        ~Iterator() = default;
        /* Modify the value into val */
        void modify(const value_t &val) {
            ptr->data.locateO(index[2]);
            ptr->data.write(val);
        }
        /* Record the value into val. */
        void getValue(value_t &val) {
            ptr->data.locateI(index[2]);
            ptr->data.read(&val);
        }
        /* Test whether it's not null. */
        operator void *() const{return ptr;}
        /* Test whether it's null. */
        bool operator !(void) const{return !ptr;}
    };

    /* Forbidden! */
    BlockList() = delete;
    /* Provide names for 3 File_Manager. */
    BlockList(const char *c1,const char *c2,const char *c3): 
        map(c1),list(c2),data(c3) {
        if(map.create()) {
            map.seekg(0);
            map.write(0);
            map.write(-1);
            nodeM tmp;
            map.write(tmp);
        }
        if(list.create()) {
            nodeL tmp;
            map.write(tmp);
        }
        data.create();
    }
    /* Close File. */
    ~BlockList() {
        map.close();
        list.close();
        data.close();
    }


    /* Modify given key to given val. */
    void modify(const key_t &key,const value_t &val) {}
    /* Modify given key to another key. */ 
    void modify(const key_t &key,const key_t &val) {}
    /* Erase "count" key-value pair.
       True  if sucessfully erased.
       False if not existed within. */
    bool erase(const key_t &key,int count) {}
    /* Return the iterator of key.
       If not existed,it will return null-Iterator. */
    Iterator find(const key_t &key) {}

    #undef MIN_LEN
    #undef MAX_LEN
    #undef MAP_BIAS
    #undef LIST_BIAS
    #undef DATA_BIAS
    #undef BLOCK_SIZ
};


};


#endif
