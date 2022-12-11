#ifndef _BLOCKLIST_HPP_
#define _BLOCKLIST_HPP_

#include "book.hpp"
#include "FileIO.hpp"


#include <memory>
#include <vector>

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
// template <class key_t,class value_t>
class BlockList {
  private:
    using value_t  = Book;
    using key_t    = Keyword_t; // Debug use

    /* Node of the list. */
    struct List {
        int nxt;   // next pointer
        int pre;   // prev pointer
        int index; // index of the list first node
        int count; // count of the list nodes.
        key_t key; // maximum key of the block
        List(int _n = EOF,int _p = EOF,int _i = EOF,int _c = 0):
            nxt(_n),pre(_p),index(_i),count(_c) {}
        List(const key_t &_k,int _n = EOF,int _p = EOF,int _i = EOF,int _c = 0):
            key(_k),nxt(_n),pre(_p),index(_i),count(_c) {}
    };

    #define MIN_LEN  2
    #define MAX_LEN  6
    #define BLOCK_SIZ 7

    /* Node of the key-value pair. */
    struct Node {
        value_t val[BLOCK_SIZ];
        key_t   key[BLOCK_SIZ];
    };

    #define LIST_BIAS 8 + sizeof(List) // Leave one vaccancy
    #define NODE_BIAS 0

    File <List,LIST_BIAS> listFile; // List of the nodes.
    File <Node,NODE_BIAS> nodeFile; // Node of the values.

    friend class Iterator;

  public:

    /* Iterator of one node's value. */
    struct Iterator {
      public: 
        int index[2];   // list/node/pos index (in order to align)

      public:
        /* Initialize. */
        Iterator(int idx1 = EOF,int idx2 = EOF):
            index({idx1,idx2}) {}
        /* Do nothing. */ 
        ~Iterator() = default;

        /* Test whether it's not empty.*/
        operator void *() const {
            return (void *)(index[0] != EOF);
        }
        /* Test whether it's an empty iterator. */
        bool operator !()const {
            return index[0] == EOF;
        }
    };
    
  private:

    /* Get the absolute position of a value. */
    inline static int getLoc(int index,int pos) {
        return NODE_BIAS + index * sizeof(Node) + pos * sizeof(value_t); 
    }

    /* The first node equal to key. EOF if not found. */
    int find(const List &tmp,const key_t &key) {
        int index = tmp.index;
        if(index == EOF) return EOF;
        int count = tmp.count;
        Node cur;
        nodeFile.locateI(index);
        nodeFile.read(&cur);
        for(int i = 0 ; i < count ; ++i) {
            if(cur.key[i] < key) continue;
            if(key < cur.key[i]) return EOF;
            return i;
        }
        return EOF; // It won't happen.
    }

    /* The first/last node equal to key. EOF/EOF if not found. */
    Iterator findall(const List &tmp,const key_t &key) {
        int index = tmp.index;
        if(index == EOF) return EOF;
        int count = tmp.count;
        Node cur;
        nodeFile.locateI(index);
        nodeFile.read(&cur);
        int i,j; // first/last node
        for(i = 0 ; i < count ; ++i) {
            if(cur.key[i] < key) continue;
            if(key < cur.key[i]) return Iterator();
            break;
        }
        if(i == count) return Iterator();
        for(int j = count - 1 ; j > i ; --j) {
            if(key < cur.key[i]) continue;
            break;
        }
        return Iterator();
    }


  public:
    
    /* Forbidden! */
    BlockList() = delete;

    /* Provide names for 2 File_Manager. */
    BlockList(const char *c1,const char *c2): 
        listFile(c1),nodeFile(c2) {
        if(listFile.create()) {
            listFile.write(EOF); // Index
            listFile.write(0);  // Count
            List tmp;
            listFile.write(tmp);
        }
        nodeFile.create();
    }
    /* Close File. */
    ~BlockList() {
        listFile.close();
        nodeFile.close();
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
    Iterator find(const key_t &key) {
        listFile.seekg(0);
        int index;
        int count;
        listFile.read(&index);
        listFile.read(&count);
        List cur;
        while(index != EOF) {
            listFile.locateI(index);
            listFile.read(&cur);
            if(cur.key < key) { // Not reached
                index = cur.nxt;
            } else {
                int pos = find(cur,key);
                if(pos == EOF) return Iterator();
                else return Iterator(index,getLoc(cur.index,pos));
            }
        }
        return Iterator();
    }

    /* Return all iterators of the element. */
    std::vector <Iterator> findAll(const key_t & key) {
        listFile.seekg(0);
        int index;
        int count;
        listFile.read(&index);
        listFile.read(&count);
        List cur;
        std::vector <Iterator> ans;
        while(index != EOF) {
            listFile.locateI(index);
            listFile.read(&cur);
            if(cur.key < key) {
                index = cur.nxt;
            } else {
                Iterator pos = find(cur,key);
                if(!pos) {return ans;}
                for(int i = pos.index[0] ; i <= pos.index[1] ; ++i) {
                    ans.emplace_back(index,getLoc(cur.index,i));
                }
                index = cur.nxt;
            }
        }
        return ans;
    }
};


};


#endif
