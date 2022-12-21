#ifndef _BLOCKLIST_HPP_
#define _BLOCKLIST_HPP_

#include "FileIO.hpp"


#include <memory>
#include <list>
#include <vector>
#include <algorithm>


namespace dark {


/**
 * @brief 
 * 
 * @tparam key_t   Require Compare function.
 * @tparam value_t Require Compare function.
 */
template <class key_t,class value_t>
class BlockList {
  private:
    #define MIN_SIZE   64
    #define MAX_SIZE   255
    #define MOVE_SIZE  1280 // NOT AVAILABLE FEATURE
    #define MERGE_SIZE 200
    #define BLOCK_SIZE 256

    /* Key-Value pair wrapping. */
    struct pair_t {
        key_t key;
        value_t val;
        /* Compare first by key,then by value_t. */
        int cmp(const key_t &__k,const value_t &__v) {
            int __C = Compare(key,__k);
            if(__C) return __C;
            else return Compare(val,__v);
        }

        pair_t &get(const key_t &__k,const value_t &__v) {
            key = __k;
            val = __v;
            return *this;
        }

        /* Compare by key-only. */
        friend bool operator <(const key_t &lhs,const pair_t &rhs) {
            return Compare(lhs,rhs.key) < 0;
        }

        /* Compare by key-only. */        
        friend bool operator <(const pair_t &lhs,const key_t &rhs) {
            return Compare(lhs.key,rhs) < 0;
        }

        pair_t() = default;
        pair_t(const key_t &__k,const value_t &__v):
            key(__k),val(__v) {}
    };

    /* Only store necessary data. */
    struct Header {
        int index;  // Index of data.
        int count;  // Count of pair.
        pair_t max; // Max key-val pair.
        Header() = default;
        Header(int __i,int __c,const pair_t &__m): 
            index(__i),count(__c),max(__m) {}
        Header(int __i,int __c,const key_t &__k,const value_t &__v):
            index(__i),count(__c),max(__k,__v) {}
    };

    using Node = pair_t[BLOCK_SIZE];

    using Container = std::list <Header>; // Container of headers.
    // using Container = std::vector <Header>;



    Container list;  // Container of header.
    using iterator = Container::iterator;
    std::vector <int> memory; // COntainer of unused node index.
    std::vector <Header> data;
    Node Node_cache1; // Cache
    Node Node_cache2; // Cache


    File nodeFile;  // Data only node File.
    File listFile;  // Store list like a vector.
    bool isNewNode; // Whether to init a node.

  public:
    BlockList() = delete;
    /* Set path for cache file. */
    BlockList(const char *path1,const char *path2) : 
        nodeFile(path1),listFile(path2) {
        nodeFile.create();
        if(!listFile.create()) {initList();}
    }

    /* Deallocate. */
    ~BlockList() {
        listFile.seekg(0);
        listFile.write(list.size() + memory.size());
        listFile.write(list.size());
        data.assign(list.begin(),list.end());
        listFile.write(data.front(),sizeof(Header) * list.size());
        isNewNode = false;
        listFile.close();
        nodeFile.close();
    }

  protected:

    /* Initialize list file */
    void initList() {
        size_t total; // Total of occupied nodes.
        size_t count; // Count of nodes in list.

        listFile.seekg(0);
        listFile.read(total); 
        listFile.read(count); 
        memory.assign(total,0);
        data.resize(count);

        listFile.read(data.front(),sizeof(Header) * count);
        for(auto &tmp : data) {
            list.push_back(tmp);
            memory[tmp.index] = true;
        }

        total = 0;
        for(size_t i = 0 ; i < memory.size() ; ++i)
            if(!memory[i]) {memory[total++] = i;}
        memory.resize(total); // Total of available nodes.
    }


    /* Write just one Node.
       If type = true,new space will be initialized.*/
    void writeNode(iterator it,const Node &tmp) {
        nodeFile.seekp(it->index * sizeof(Node));
        if(isNewNode) nodeFile.write(tmp); // init space 
        else nodeFile.write(tmp,it->count * sizeof(pair_t));
        isNewNode = false;
    }

    /* Read Just one Node. */
    void readNode(iterator it,Node &tmp) {
        nodeFile.seekg(it->index * sizeof(Node));
        nodeFile.read(tmp,it->count * sizeof(pair_t));
    }

    /* Allocate one new Node. */
    int newNode() {
        if(memory.empty()) {
            isNewNode = true;
            return list.size();
        } else {
            int ans = memory.back();
            memory.pop_back();
            return ans;
        }
    }

    /* Deallocate one node. */
    void recycle(iterator it) {
        memory.push_back(it->index);
        list.erase(it);
    }

    /**
     * @brief Search in [0,len) for ans location,
     * where A[ans - 1] < val < A[ans] .
     * 
     * @param A   The array of the pair_t.
     * @param len The length of the array.
     * @param key Key of the pair.
     * @param val Value of the pair.
     * @return Ans in [0,len] if found. ||
     *         ~Ans if existing identical pair.
     */
    int binary_search(pair_t *A,int len,
                      const key_t &key,const value_t &val) {
        int l = 0,r = len;
        while(l != r) {
            int mid = (l + r) >> 1;
            int cmp = A[mid].cmp(key,val);
            if(cmp < 0) { // A[mid] < k/v
                l = mid + 1;
            } else if(cmp > 0) { // A[mid] > k/v
                r = mid;
            } else {return ~mid;} // A[mid] = k/v
        } // l == r && A[l] >= k/v
        return l;
    }

    /* Copy data by memmove. */
    void copy(pair_t *dst,const pair_t *src,int count) {
        memmove((void *)dst,(void *)src,count * sizeof(pair_t));
    }


  public:
    using Return_Array = std::vector <value_t>;

    void insert(const key_t &key,const value_t &val) {
        if(list.empty()) {
            list.emplace_back(newNode(),1,key,val);
            Node &cur = Node_cache1;
            cur[0].get(key,val);
            return writeNode(list.begin(),cur);
        }

        iterator it = list.begin();
        for( ; it != list.end() ; ++it) {
            int cmp = it->max.cmp(key,val);
            if(cmp < 0) {continue;}
            else {return insert(it,key,val);}
        }
        return insert(--it,key,val);
    }

    void erase(const key_t &key,const value_t &val) {
        iterator it = list.begin();
        for( ; it != list.end() ; ++it) {
            int cmp = it->max.cmp(key,val);
            if(cmp < 0) continue;
            else {return erase(it,key,val);}
        }
    }

    void findAll(const key_t &key,Return_Array &arr) {
        iterator it = list.begin();
        bool EQ = false;
        Node &cur = Node_cache1;
        for( ; it != list.end() ; ++it) {
            int cmp = Compare(it->max.key,key);
            if(cmp < 0) continue;
            else {
                readNode(it,cur);
                int l = EQ ?
                    0 : std::lower_bound(cur,cur + it->count,key) - cur;
                int r = !cmp ?
                    it->count : std::upper_bound(cur,cur + it->count,key) - cur;
                while(l != r) {arr.push_back(cur[l++].val);}
                EQ = true;
                if(cmp) return;
            }
        }
    }


  protected:

    /* Insert at iterator. */
    void insert(iterator it,const key_t &key,const value_t &val) {
        int &count = it->count;  // Will be changed.
        Node &cur  = Node_cache1;
        readNode(it,cur);

        int pos = binary_search(cur,count,key,val);
        if(pos < 0) return;

        if(pos == count) {it->max = cur[pos].get(key,val);} 
        else {copy(cur + pos + 1,cur + pos,count - pos);cur[pos].get(key,val);}

        if(++count <= MAX_SIZE) {writeNode(it,cur);}
        else {split(it,cur);}
    }

    /* Split target Node. */
    void split(iterator it,Node &cur) {
        Node &nxt   = Node_cache2;
        const int count1 = it->count >> 1; // Count of nxt Node.
        int &count2      = it->count;      // Count of current node.
    
        count2 -= count1;
        copy(nxt,cur + count2,count1);
        
        /* it newNode() ++it */

        it = list.emplace(++it,newNode(),count1,it->max);
        writeNode(it,nxt);

        (--it)->max = cur[count2 - 1];
        writeNode(it,cur);
    }


    void erase(iterator it,const key_t &key,const value_t &val) {
        if(it->count == 1) {
            if(it->max.cmp(key,val)) return;
            recycle(it);
            return;
        }

        int &count = it->count;   // Will be changed.
        Node &cur  = Node_cache1;
        readNode(it,cur);

        int pos = ~binary_search(cur,count,key,val);
        if(pos < 0) return;

        if(pos == count - 1) {it->max = cur[pos - 1];}
        else {copy(cur + pos,cur + pos + 1,count - pos - 1);}

        if(--count >= MIN_SIZE) {
            writeNode(it,cur);
        } else {merge(it,cur);}
    }

    void merge(iterator it,Node &cur) {
        if(it != list.begin()) {
            iterator pre = iterator(it._M_node->_M_prev);
            if(pre->count + it->count <= MERGE_SIZE) {
                Node &tmp = Node_cache2;
                readNode(pre,tmp);
                return merge(pre,tmp,it,cur);
            }
            if(pre->count - it->count >= MOVE_SIZE) {
                Node &tmp = Node_cache2;
                readNode(pre,tmp);
                int num = (pre->count - it->count) >> 1;
                copy(cur + num,cur,num);
                it->count  += num;
                pre->count -= num;
                pre->max = tmp[pre->count - 1];
                copy(cur,tmp + pre->count,num); 
                writeNode(it,cur);
                writeNode(pre,tmp);
                return;
            }
        }
        if(it != --list.end()) {
            iterator nxt = iterator(it._M_node->_M_next);
            if(nxt->count + it->count <= MERGE_SIZE) {
                Node &tmp = Node_cache2;
                readNode(nxt,tmp);
                return merge(it,cur,nxt,tmp);
            }
            if(nxt->count - it->count >= MOVE_SIZE) {
                Node &tmp = Node_cache2;
                readNode(nxt,tmp);
                int num = (nxt->count - it->count) >> 1;
                copy(cur + it->count,tmp,num);
                nxt->count -= num;
                it->count  += num;
                it->max = cur[it->count - 1];
                copy(tmp,tmp + num,num);
                writeNode(it,cur);
                writeNode(nxt,tmp);
                return;
            }
        } 
        writeNode(it,cur);
    }

    void merge(iterator pre,Node &cur,iterator nxt,Node &tmp) {
        copy(cur + pre->count,tmp,nxt->count);
        pre->count += nxt->count;
        pre->max    = nxt->max;  
        recycle(nxt);
        writeNode(pre,cur);
    }

};



};


#endif
