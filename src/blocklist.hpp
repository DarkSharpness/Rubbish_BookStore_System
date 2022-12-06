#ifndef _BLOCKLIST_HPP_
#define _BLOCKLIST_HPP_

#include "string.hpp"
#include "book.hpp"
#include "FileIO.hpp"

namespace dark {



/* Special Block List for Bookstore */
class BlockList {
  private:
    using value_t = Book;
    using key_t   = string <24>;
    
    #define MIN_LEN  2
    #define MAX_LEN  6
    #define MAP_BIAS  8
    #define DATA_BIAS 0 
    #define FREE_BIAS 4  
    #define BLOCK_SIZ  7
    struct node {
        int nxt;   // next pointer
        int pre;   // prev pointer
        int index; // index of the data
        key_t key; // maximum key of the block
        node(key_t _k = "",int _n = -1,int _p = -1,int _i = -1):
            key(_k),nxt(_n),pre(_p),index(_i) {}
    };

    /* True data container. */
    struct block {
        size_t  count;      // Count of value in the data
        value_t val[BLOCK_SIZ]; // Store reverse of ISBN
        block() {count = 0;}
    };

    File map;  // Map  of the nodes. 
    File data; // Data of the blocks.
    File free; // Free Block Index Recorder


    void insert(const node &iter,const value_t &val) {
        data.open();


        data.close();
    }

    void find(const node &iter,const value_t &tar) {
        data.open();
        data.seekg(iter.index * sizeof(block));
        /// TODO: Complete this part.

        data.close();
    }
  public:
    BlockList() :map("map.bin"),data("data.bin"),free("free.bin") {
        /// TODO: complete this part.
        if(map.create()) {
            map.seekg(0);
            map.write(0);
            map.write(-1);
        }
        // map.close()
        data.create();
        if(free.create()) {free.seekg(0);free.write(0);}

    }
    ~BlockList() {

    }

    void insert(const value_t &val) {
        

    }

    void find(const key_t &tar) {
        int blockCnt; // Count of all blocks.
        int index;    // Index of first node.
        // map.open();
        map.seekg(0);
        map.read(&blockCnt);
        map.read(&index);
        node cur;

        while(index != -1) {
            map.seekg(MAP_BIAS + index * sizeof(block));
            map.read(&cur);
            if(cur.key < tar) {
                index = cur.nxt;
            } else {
                find(cur,tar);
            }
        }


    }

    #undef MIN_LEN
    #undef MAX_LEN
    #undef MAP_BIAS
    #undef DATA_BIAS
    #undef FREE_BIAS 
    #undef BLOCK_SIZ
};



};


#endif
