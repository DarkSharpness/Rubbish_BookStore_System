#include <bits/stdc++.h>


/* Fresh new start. */
namespace dark {



/* Standard compare function for any type. */
template <class T>
int Compare(const T &x,const T &y) {
    if(x < y) return -1;
    else return (y < x);
}

/* Compare 2 char * strings. */
int Compare(const char *s1,const char *s2) {
    return strcmp(s1,s2);
}


/**
 * @brief Custom fixed-length string for file output use.
 * 
 * @tparam Length of the string.
 */
template <size_t len>
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
        if(this != &rhs) { memcpy(str,rhs.str,sizeof(str)); }
        return *this;
    }

    /* Basic copy from another cstr. */
    string &operator =(const char *ptr) {
        memset(str,0,sizeof(str));
        strcpy(str,ptr);
        return *this;
    }

    /* Test whether a string is empty */
    bool operator !(void) const {return str[0];}


    /* Force to change into a cstr. */
    operator const char *() const {return str;}

    /* Reference to data. */

    inline char &operator [](size_t __pos)       {return str[__pos];}
    inline char  operator [](size_t __pos) const {return str[__pos];}

    /* Compare Functions*/

    /* Do as u think. */
    // inline bool operator <(const string &rhs) const {return strcmp(str,rhs) < 0;}

    /* Do as u think. */
    inline bool operator ==(const char *rhs) const {return !strcmp(str,rhs);}

    /* Compare 2 strings. */
    friend int Compare(const string &lhs,const string &rhs) {
        return strcmp(lhs,rhs);
    }

    /* Input by istream */
    friend std::istream &operator >>(std::istream &is,string &tmp) {
        return is >> tmp.str;
    }

    /* Output by ostream. */
    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};





using fileName_t = string <64>;

/* Custom file with limited wrapping */
class File : public std::fstream {
  private:
    fileName_t name;
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

using key_t = string <72>;
using value_t = int;

class BlockList {
  private:
    #define MIN_SIZE   64
    #define MAX_SIZE   255
    #define MOVE_SIZE  1280
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



}

signed main() {
    const char str1[] = "a.txt";
    const char str2[] = "b.txt";
    
    std::ios::sync_with_stdio(false);

    dark::BlockList ans(str1,str2);
    
    dark::string <32> opt;
    dark::string <72> a;
    int b,n;
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