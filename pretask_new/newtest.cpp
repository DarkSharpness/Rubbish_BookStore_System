#include <bits/stdc++.h>
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

    inline bool operator ==(const char *rhs) const{return !strcmp(str,rhs);}

    template <int _len>
    friend int Compare(const string &lhs,const string<_len> &rhs) {
        return strcmp(lhs,rhs);
    }

    friend std::istream &operator >>(std::istream &is,string &tmp) {
        return is >> tmp.str;
    }

    /* Output by ostream. */
    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};




/* Custom File manager. */
template <class _T,size_t BIAS,size_t unit = sizeof(_T)>
class File : public std::fstream {
    using FileName_t = dark::string <64>;
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

    void close() {
        this->std::fstream::close();
    }

    /* Open the given File in binary mode(default). */
    void open(std::ios_base::openmode mode = (std::ios_base::openmode)28) {
        this->std::fstream::open(name,mode);
    }

    /* Just read one of _T. */
    void read(_T &ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(&ptr),unit);
    }

    /* Just read one int object! */
    void read(int &ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(&ptr),sizeof(int));
    }

    /* Just write one of _T. */
    void write(const _T &val) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),unit);
    }
};

using key_t   = dark::string <72>;
using value_t = int;

int Compare(int x,int y) {
    if(x < y) return -1;
    else return x > y;
}

class BlockList {
  private:
    /* Key-value pair type */
    struct pair_t {
        key_t   key; // key
        value_t val; // value

        /* Fast assign by memcpy. */
        pair_t &operator =(const pair_t &rhs) {
            if(this != &rhs) {memcpy((void *)this,(void *)&rhs,sizeof(pair_t));}
            return *this;
        }

        /* Compare key-only. */
        friend bool operator <(const pair_t &lhs,const key_t &rhs) {
            return lhs.key < rhs;
        }

        /* Compare key-only. */
        friend bool operator <(const key_t &lhs,const pair_t &rhs) {
            return lhs < rhs.key;
        }

        /* Compare node with key-value.
           First compare by key,then compare by value */
        int cmp(const key_t &K,const value_t &V) const{
            int cmp = Compare(key,K);
            if(cmp) return cmp;
            else return Compare(val,V);
        }
        /* Initialize its key and value. */
        pair_t get(const key_t &K,const value_t &V) {
            key = K;
            val = V;
            return *this;
        }

        pair_t() = default;
        pair_t(const key_t &_k,const value_t &_v) :key(_k),val(_v) {}
        pair_t(const pair_t &rhs) {memcpy((void *)this,&rhs,sizeof(pair_t));} 
    };

    #define MIN_SIZE   160
    #define MAX_SIZE   639
    #define BLOCK_SIZE 640
    #define MERGE_SIZE 324
    #define AMORT_BIAS 80
    #define LIST_BIAS  sizeof(List)
    #define NODE_BIAS -sizeof(Node)

    struct Node {
        pair_t arr[BLOCK_SIZE]; // Node array
        operator pair_t *() { return arr; }
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

    File <List,LIST_BIAS> listFile; // File for list
    File <Node,NODE_BIAS> nodeFile; // File for node

    List memory;  // Store memory information.

    /* Allocate one new list.
       Return the list index. */
    inline int newList() {
        int index = memory.nxt;
        if(index) { /* Not empty case. */
            readnxt(memory.nxt,index); // Change memory.nxt
            return index;
        } else {   /* Allocate new space for node */
            int count = ++memory.count; // New List index.
            static List tmpList;
            tmpList.index = count;
            write(tmpList,count); // get space
            static Node tmpNode;
            write(tmpNode,count); // get space
            return count;
        }
    }

    /* Return one list for further use. */
    void recycle(List &tmp) {
        tmp.nxt = memory.nxt;
        tmp.count = 0;
        write(tmp,tmp.index);
        memory.nxt = tmp.index;
    }

    // Cache : To maximize speed!

    Node Node_cache1; // Cache variable for Node
    Node Node_cache2; // Cache variable for Node
    // Node Node_cache3; // Cache variable for Node

    List List_cache1; // Cache variable for List
    List List_cache2; // Cache variable for List
    // List List_cache3; // Cache variable for List

  public:
    BlockList() = delete;
    /* Initialize. */
    BlockList(const char *s1,const char *s2):
        listFile(s1),nodeFile(s2) {
        if(listFile.create()) {
            List &tmp = List_cache1; // count = index = pre = nxt = 0
            tmp.index = -1;
            listFile.write(tmp);
            tmp.index = 0;
            tmp.count = MAX_SIZE << 1; // Prevent potential amortization
            listFile.write(tmp);
        }
        read(memory,-1);
        nodeFile.create();
    }

    /* Write Info. */
    ~BlockList() {
        write(memory,-1);
        listFile.close();
        nodeFile.close();
    }

    /* Insert a key/value pair. */
    void insert(const key_t &key,const value_t &val) {
        List &tmp = List_cache1;   // Current List.
        read(tmp,0);
        int index = tmp.nxt;      // current index.
        if(index == 0) { /* Special judge: empty list. */
            List &cur = List_cache2;
            cur.index = tmp.pre = tmp.nxt = newList();
            write(tmp,tmp.index);
            cur.count = cur.pre = cur.nxt = 0;
            return insert(cur,key,val);
        }
        while(index) { /* 0 is empty node. */
            read(tmp,index);
            if(tmp.max.cmp(key,val) < 0) {
                index = tmp.nxt;
            } else { /* Inside the block */
                return insert(tmp,key,val);
            }
        }
        /* Get to the end case. */
        return insert(tmp,key,val);
    }


    void erase(const key_t &key,const value_t &val) {
        List &tmp = List_cache1;  // Current List.
        read(tmp,0);
        int index = tmp.nxt;     // current index.
        while(index) { /* 0 is empty node. */
            read(tmp,index);
            if(tmp.max.cmp(key,val) < 0) {
                index = tmp.nxt;
            } else { /* Inside the block */
                erase(tmp,key,val);
                return;
            }
        }
        /* Get to the end case. */
    }

    using Return_Array = std::vector <value_t>;

    /* Find all key and return the value. */
    void findAll(const key_t &key,Return_Array &arr) {
        List &tmp = List_cache1;  // Current List.
        Node &cur = Node_cache1;  // Current Node.
        read(tmp,0);
        int index = tmp.nxt;     // Current index.
        bool flag = false;       // Whether there is equal before.
        while(index) {
            read(tmp,index);
            int cmp = Compare(tmp.max.key,key);
            if(cmp > 0) {
                read(cur,tmp.index);
                int i = flag ? 0 : 
                    std::lower_bound(cur + 0,cur + tmp.count,key) - cur;
                int j = std::upper_bound(cur + 0,cur + tmp.count,key) - cur;
                return getArr(cur,i,j,arr);
            } else if(cmp == 0) {
                read(cur,tmp.index);
                int i = flag ? 0 :
                    std::lower_bound(cur + 0,cur + tmp.count,key) - cur;
                int j = tmp.count;
                getArr(cur,i,j,arr);
                flag = true;
            }
            index = tmp.nxt;
        }
        /* Get to the end case. */
    }

    



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
    inline void readnxt(int &tmp,int loc) {
        listFile.locateI(loc);
        listFile.read(tmp);
    }

    /* Special read(pre) for an address! */
    inline void readpre(int &tmp,int loc) {
        readnxt(tmp,loc);
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

    /* Merge L1 and L2.(L2 is right after L1.)
       Ensure L1.index != 0 , L2.index != 0 */
    void merge(List &L1,Node &pre,List &L2,Node &nxt) {
        copy(pre + L1.count,nxt,L2.count);
        /* Adjust List after L2. */
        
        static List tmp;
        read(tmp,L2.nxt);
        tmp.pre = L1.index;
        write(tmp,tmp.index);
        /* Adjust List L1 */
        L1.max = L2.max;
        L1.nxt = L2.nxt;
        L1.count += L2.count;
        write(L1,L1.index);
        write(pre,L1.index);
        /* L2 becomes useless now. */
        recycle(L2);
    }

     /**
     * @brief Try to merge with another list.
     * 
     * @param tmp The current list.
     * @param address The address of the current list.
     * @param cur The current node.
     * @param index The index of target list.
     * @param type 0/1 nxt before/after cur
     * @return true if successfully merged
     */
    bool merge(List &tmp,Node &cur,bool type) {
        List &tar = List_cache2; // Target List
        Node &nxt = Node_cache2; // Next-operated Node
        if(type) { /* nxt is after cur */
            if(!tmp.nxt) return false;
            read(tar,tmp.nxt);
            if(tar.count > MERGE_SIZE) return false;
            /* Now merge! */
            read(nxt,tar.index);
            merge(tmp,cur,tar,nxt);
        } else { /* nxt is before cur */
            if(!tmp.pre) return false;
            read(tar,tmp.pre);
            if(tar.count > MERGE_SIZE) return false;
            /* Now merge! */
            read(nxt,tar.index);
            merge(tar,nxt,tmp,cur);
        }
        return true;
    }

    /* Try to merge with prev and next nodes. */
    void merge(List &tmp,Node &cur) {
        if(!merge(tmp,cur,0) && !merge(tmp,cur,1)) {
            /* Try amortization instead. */
            /* Write in data first. */
            write(tmp,tmp.index);
            write(cur,tmp.index);
            if(tmp.pre) {
                read(tmp,tmp.pre);
                read(cur,tmp.index);
                amortize(tmp,cur,1);
            } else if(tmp.nxt) {
                read(tmp,tmp.nxt);
                read(cur,tmp.index);
                amortize(tmp,cur,0);
            } else { /* No amortize or merge. */ }
        }
    }

    /* Copy the data by memmove. */
    void copy(pair_t *dst,pair_t *src,int count) {
        memmove((void*)dst,(void*)src,count * sizeof(pair_t));
    }

    /**
     * @brief Try to amortize with another list.
     * 
     * @param tmp The current list.
     * @param address The address of the current list.
     * @param cur The current node.
     * @param index The index of target list.
     * @param type 0/1 nxt before/after cur
     * @return true if successfully amortized
     */
    bool amortize(List &tmp,Node &cur,bool type) {
        List &tar = List_cache2; // target list
        Node &nxt = Node_cache2; // next-operated node
        int bias;

        if(type) { /* nxt is after cur */
            if(tmp.nxt == 0) return false; // NULL node case.
            read(tar,tmp.nxt);

            /* Work out bias first. */
            bias = (tmp.count - tar.count) >> 1;
            if(bias < AMORT_BIAS) return false;

            /* Now move data! */
            read(nxt,tar.index);
            copy(nxt + bias,nxt,tar.count);
            tmp.count -= bias;
            tar.count += bias;
            copy(nxt,cur + tmp.count,bias);
            tmp.max = cur[tmp.count - 1]; // Update max
        } else { /* nxt is before cur */
            if(tmp.pre == 0) return false; // NULL node case.
            read(tar,tmp.pre);

            /* Work out bias first. */
            bias = (tmp.count - tar.count) >> 1;
            if(bias < AMORT_BIAS) return false;

            /* Now move data! */
            read(nxt,tar.index);
            copy(nxt + tar.count,cur,bias);
            tmp.count -= bias;
            tar.count += bias;
            copy(cur,cur + bias,tmp.count);
            tar.max = nxt[tar.count - 1]; // Update max
        }

        write(tmp,tmp.index);
        write(cur,tmp.index);
        write(tar,tar.index);
        write(nxt,tar.index);
        return true;
    }

    /* Split current List and Node. */
    void split(List &tmp,Node &cur) {
        if(!amortize(tmp,cur,0) && !amortize(tmp,cur,1)) {
            List &tar = List_cache2; // Target List
            Node &nxt = Node_cache2; // Next-operated Node
            tar.count = tmp.count - (tmp.count >> 1);
            tmp.count >>= 1;
            copy(nxt,cur + tmp.count,tar.count);

            /* Update tar */
            tar.pre = tmp.index;
            tar.nxt = tmp.nxt;
            tar.max = tmp.max;
            tar.index = newList();

            /* Update tmp */
            tmp.nxt = tar.index;
            tmp.max = cur[tmp.count - 1];
            write(tmp,tmp.index);
            write(cur,tmp.index);
            write(tar,tar.index);
            write(nxt,tar.index);

            /* Update node after tmp. */
            read(tar,tar.nxt);
            tar.pre = tmp.nxt;
            write(tar,tar.index);
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
        Node &cur = Node_cache1; // Target node
        read(cur,tmp.index);     // Work out answer

        /* Find the location and insert. */
        int pos = binary_search(cur,tmp.count,key,val);
        if(pos < 0) return; // The same element.

        if(pos == tmp.count) { /* Insert at the end. */
            tmp.max = cur[pos].get(key,val);
        } else { /* Insert in the middle. */
            copy(cur + pos + 1,cur + pos,tmp.count - pos);
            cur[pos].get(key,val);
        }
        /* Split iff count + 1 = MAX_SIZE  */
        if(++tmp.count <= MAX_SIZE) {
            write(tmp,tmp.index);
            write(cur,tmp.index);
        } else {split(tmp,cur);}
    }

    /**
     * @brief Erase at certain list. 
     * 
     * @param tmp The current list.
     * @param key The key of the new node.
     * @param val The value of the new node.
     */
    void erase(List &tmp,const key_t &key,const value_t &val) {
        if(tmp.count == 1) { /* Special judge for 1 element case. */
            if(tmp.max.cmp(key,val) != 0) return; // Not found.
            List &tar = List_cache2;
            /* Modify pre information. */
            read(tar,tmp.pre);
            tar.nxt = tmp.nxt;
            write(tar,tar.index);
            /* Modify nxt information. */
            read(tar,tmp.nxt);
            tar.pre = tmp.pre;
            write(tar,tar.index);
            /* tmp is useless now */
            recycle(tmp);
            return;
        }

        Node &cur = Node_cache1;    // Target node
        read(cur,tmp.index);        // Work out answer

        /* Find the location and insert. */
        int pos = ~binary_search(cur,tmp.count,key,val);
        if(pos < 0) return; // Not found.

        if(pos == tmp.count - 1) { /* Erase at the end. */
            tmp.max = cur[pos - 1];
        } else { /* Insert in the middle. */
            copy(cur + pos,cur + pos + 1,tmp.count - pos - 1);
        }
        /* Merge iff count - 1 = MIN_SIZ*/
        if(--tmp.count > MIN_SIZE) {
            write(tmp,tmp.index);
            write(cur,tmp.index);
        } else {merge(tmp,cur);}
    }

    /* Fill the array. */
    inline void getArr(Node &cur,int i,int j,Return_Array &arr) {
        while(i < j) {arr.push_back(cur[i++].val);}
    }

};


}

signed main() {
    dark::string <64> str1 = "a";
    dark::string <64> str2 = "b";
    std::ios::sync_with_stdio(false);
    dark::BlockList ans(str1,str2);
    dark::string <32> opt;
    dark::string <72> a;
    int b;
    int n;
    std::cin >> n;
    dark::BlockList::Return_Array A;

    while(n--) {
        std::cin >> opt >> a;
        if(opt == "find") {
            A.clear();
            ans.findAll(a,A);
            if(A.empty()) {
                std::cout << "null\n";
            } else {
                for(const auto &it : A) {
                    std::cout << it << ' ';
                }
                std::cout << '\n';
            }
            continue;
        }
        std::cin >> b;
        if(opt == "delete") {
            ans.erase(a,b);
        } else if(opt == "insert") {
            ans.insert(a,b);
        }
    }
    
    return 0;
}