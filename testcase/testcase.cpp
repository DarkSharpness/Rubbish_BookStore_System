#include <bits/stdc++.h>


// Header-only library for special string
#ifndef _STRING_HPP_
#define _STRING_HPP_

namespace dark {

/**
 * String of a fixed length.
 * With basic string functions.
 * 
 * @tparam len The maximum length of a string
 * 
 */
template <int len>
class string {
  private:
    char str[len];

  public:

    /* Basic functions. */

    /* Initialize by nothing. */
    string() {}
    /* Initialize as empty. */
    string(std::nullptr_t) {memset(str,0,sizeof(str));}
    /* Initialize by another cstr. */
    string(const char *ptr) {strcpy(str,ptr);}
    /* Nothing to do. */
    ~string() = default;

    /* Basic copy from another string. */
    string &operator =(const string &rhs) {
        if(this == &rhs) return *this;
        memcpy(str,rhs.str,sizeof(rhs));
        return *this;
    }

    /* Basic copy from another cstr. */
    string &operator =(const char *ptr) {
        memset(str,0,sizeof(str));
        strcpy(str,ptr);
        return *this;
    }

    /* Test whether a string is empty */
    bool operator !(void) const{return str[0];}


    /* Force to change into a cstr. */
    operator const char *() const{
        return str;
    }

    /* Reference to data. */

    inline char &operator [](int _pos)      {return str[_pos];}
    inline char  operator [](int _pos) const{return str[_pos];}

    /* Pointer as Iterators */

    inline char *begin()             {return str;}
    inline const char *begin()  const{return str;}
    inline const char *cbegin() const{return str;}
    inline char *end()             {return str + len;}
    inline const char *end()  const{return str + len;}
    inline const char *cend() const{return str + len;}

    /* Compare Functions*/

    template <int _len>
    inline bool operator <(const string<_len> &rhs) const{return strcmp(str,rhs) < 0;}
    
    
    template <int _len>
    friend int Compare(const string &lhs,const string<_len> &rhs) {
        return strcmp(lhs,rhs);
    }

    friend std::istream &operator >>(std::istream &is,const string &tmp) {
        return is >> tmp.str;
    }

    /* Output by ostream. */
    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};



}

#endif


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
        this->std::fstream::read(reinterpret_cast <char *>(&ptr),unit);
    }

    void read(int &ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(&ptr),sizeof(int));
    }

    /* Just write one of _T. */
    void write(const _T &val) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),unit);
    }
};


class BlockList {
  private:
    using key_t   = dark::string <72>;
    using value_t = dark::string <72>;

    /* Key-value pair type */
    struct pair_t {
        key_t   key; // key
        value_t val; // value

        /* Fast assign by memcpy. */
        const pair_t &operator =(const pair_t &rhs) {
            if(this != &rhs) {memcpy((void *)this,(void *)&rhs,sizeof(pair_t));}
            return *this;
        }

        /* compare X with Y */
        friend int Compare(const pair_t &X,const pair_t &Y) {
            int cmp = Compare(X.key,Y.key);
            if(cmp) return cmp;
            else return Compare(X.val,Y.val);
        }
        /* compare node with key-value */
        int cmp(const key_t &K,const value_t &V) const{
            int cmp = Compare(key,K);
            if(cmp) return cmp;
            else return Compare(val,V);
        }
        /* Initialize its key and value. */
        void get(const key_t &K,const value_t &V) {
            key = K;
            val = V;
        }

        pair_t() = default;
        pair_t(const key_t &_k,const value_t &_v) :key(_k),val(_v) {}

    };


    #define MIN_SIZE   200
    #define MAX_SIZE   500
    #define AMORT_SIZE 250
    #define BLOCK_SIZE 510

    struct Node {
        pair_t arr[BLOCK_SIZE]; // Node array
        operator pair_t *() {
            return arr;
        }
    };

    struct List {
        int nxt;   // Next node
        int pre;   // Previous node
        int index; // Index in the list and the node.(The same!)
        int count; // Count of nodes
        pair_t max;  // Maximum key-value pair
        List(int _p = 0,int _n = 0,int _i = 0,int _c = 0):
            nxt(_n),pre(_p),index(_i),count(_c) {}
        List(const key_t &key,const value_t &val,
            int _p = 0,int _n = 0,int _i = 0,int _c = 0):
            nxt(_n),pre(_p),index(_i),count(_c),max(key,val) {}
    };


    #define LIST_BIAS sizeof(List)
    #define NODE_BIAS 0
    

    File <List,LIST_BIAS> listFile; // File for list
    File <Node,NODE_BIAS> nodeFile; // File for node

    List header; // Store header information in the memory.

  protected:

    /* Read a Node at loc */
    inline void read(Node &tmp,int loc) {
        nodeFile.locateI(loc);
        nodeFile.read(tmp);
    }
    /* Read a List at loc */
    inline void read(List &tmp,int loc) {
        listFile.locateI(loc);
        listFile.read(tmp);
    }
    /* Special read(nxt) for an address! */
    inline void read(int &tmp,int loc) {
        listFile.locateI(loc);
        listFile.read(tmp);
    }
    /* Write a Node at loc. */
    inline void write(const Node &tmp,int loc) {
        nodeFile.locateO(loc);
        nodeFile.write(tmp);
    }
    /* Write a List at loc. */
    inline void write(const List &tmp,int loc) {
        listFile.locateO(loc);
        listFile.write(tmp);
    }


    /* Allocate one new list. */
    inline int newList() {
        int address = header.nxt;
        if(address) { // Not Empty
            read(header.nxt,address);
            return address;
        } else { // Allocate new space for node
            int count = ++header.count; // New List index.
            static List tmpList;
            tmpList.index = count;
            write(tmpList,count);
            static Node tmpNode;
            write(tmpNode,count);
            return count;
        }
    }

    /* Return one list. */
    void recycle(List &tmp) {
        tmp.nxt = header.nxt;
        tmp.count = 0;
        write(tmp,tmp.index);
        header.nxt = tmp.index;
    }

    /* Search in [0,len) for a pair. A[ans] >= val */
    int binary_search(pair_t *A,size_t len,
                      const key_t &key,const value_t &val) {
        int l = 0,r = len;
        while(l != r) {
            int mid = (l + r) >> 1;
            int cmp = A[mid].cmp(key,val);
            if(cmp < 0) { // A[mid] < k/v
                l = mid + 1;
            } else if(cmp > 0) { // A[mid] > k/v
                r = mid;
            } else {return mid;} // A[mid] = k/v
        } // l == r && A[l] >= k/v
        return l;
    }




    /* Merge 2 list.(tmp2 after tmp1) */
    void merge(List &tmp1,List &tmp2) {
        // int count1 = tmp1.count;   
        // int count2 = tmp2.count;
        // tmp1.nxt = tmp2.nxt;
        

        // write(tmp1);
        // recycle(tmp2);
    }



    /* Copy the data. */
    void copy(pair_t *dst,pair_t *src,int count) {
        memmove((void*)dst,(void*)src,count * sizeof(pair_t));
    }

    /**
     * @brief Try to amortize with another list.
     * 
     * @tparam type 0: nxt before cur || 1: nxt after cur
     * @param tmp The current list.
     * @param address The address of the current list.
     * @param cur The current node.
     * @param index The index of target list.
     * @return true if successfully amortized
     */
    template <bool type>
    bool amortize(List &tmp,Node &cur) {
        static List tar; // target list
        static Node nxt; // nxt operation node
        int bias;
        if(type) { // nxt is after cur
            read(tar,tmp.nxt);
            if(tar.count > AMORT_SIZE) return false;
            read(nxt,tmp.nxt);

            bias = (tmp.count - tar.count) >> 1;
            copy(nxt + bias,nxt,tar.count);
            copy(nxt,cur + tmp.count,bias);
            tmp.max = cur[tmp.count - bias - 1];
        } else { // nxt is before cur
            read(tar,tmp.pre);
            if(tar.count > AMORT_SIZE) return false;
            read(nxt,tmp.pre);

            bias = (tmp.count - tar.count) >> 1;
            copy(nxt + tar.count,cur,bias);
            copy(cur,cur + bias,(tmp.count - bias));
            tar.max = nxt[tar.count + bias - 1];
        }
        tmp.count -= bias;
        tar.count += bias;
        write(tmp,tmp.index);
        write(cur,tmp.index);
        write(tar,tar.index);
        write(nxt,nxt.index);
        return true;
    }

    /* Split current List and Node. */
    void split(List &tmp,Node &cur) {
        if(!amortize <0> (tmp,cur) // Try to adjust first.
        && !amortize <1> (tmp,cur)) {
            int half = tmp.count >> 1;
            static List tar;
            static Node nxt;
            tar.count = tmp.count - half;
            tmp.count = half;
            copy(nxt,cur + half,tar.count);
            tar.pre = tmp.index;
            tar.nxt = tmp.nxt;
            tar.max = tmp.max;
            tar.index = newList();        
            
            tmp.nxt = tar.index;
            tmp.max = cur[tmp.count - 1];
        }
    }

    /**
     * @brief Insert at certain list. 
     * 
     * @param tmp The current list.
     * @param key The key of the new node.
     * @param val The value of the new node.
     */
    void insert(List &tmp,const key_t &key,const value_t &val) {
        int &index = tmp.index; // Index of node
        int &count = tmp.count; // Count of node
        static Node tar;        // Target node
        read(tar,index);        // Work out answer

        /* Find the location and insert. */
        int pos = binary_search(tar,count,key,val);
        copy(tar + pos + 1,tar + pos,count - pos);
        tar[pos].get(key,val);

        /* Split iff count + 1 = MAX_SIZE  */
        if(++count <= MAX_SIZE) { 
            if(pos == count - 1) tmp.max = tar[pos];
            write(tmp,index);
            write(tar,index);
        } else {split(tmp,tar);}
    }

  public:

    BlockList() = delete;
    /* Initialize. */
    BlockList(const char *s1,const char *s2):
        listFile(s1),nodeFile(s2) {
        if(listFile.create()) {
            List tmp;
            tmp.index = -1;
            listFile.write(tmp);
            tmp.index = 0;
            tmp.count = MAX_SIZE << 1;
            listFile.write(tmp);
        }
        read(header,-1);
        if(nodeFile.create()) {
            Node tmp;
            nodeFile.write(tmp);
        }
    }
    /* Write Info. */
    ~BlockList() {
        write(header,-1); // Update only header.
    }

    /* Insert a key/value pair. */
    void insert(const key_t &key,const value_t &val) {
        static List cur; // current List
        read(cur,0);     // 0 is the empty node.
        int index = cur.nxt;    // current index.
        if(index == 0) { // Special judge : empty list.
            cur.nxt = newList();
            read(cur,cur.nxt);
            insert(cur,key,val);
            return;
        }

        while(index) { /* 0 is empty node. */
            read(cur,index);
            if(cur.max.cmp(key,val) < 0) {
                index = cur.nxt;
            } else { // inside the block
                insert(cur,key,val);
                return;
            }
        }

        /* Get to the end case. */
        read(cur,cur.nxt);
        insert(cur,key,val);
    }


    void remove() {

    }

    using Return_Array = std::vector <value_t>;

    void findall(int index,Return_Array &arr) {


    }

};


signed main() {


    return 0;
}