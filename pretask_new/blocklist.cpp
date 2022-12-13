#include <bits/stdc++.h>
// #include <Dark/inout>
using ll = long long;
using ull = unsigned long long;
// #define int long long

/**
 * String of a fixed length.
 * With basic string functions.
 * 
 * @tparam len The maximum length of a string
 * 
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
    // bool operator !(void) const{return str[0];}


    /* Force to change into a cstr. */
    operator const char *() const{
        return str;
    }

    /* Reference to data. */

    inline char &operator [](int _pos)      {return str[_pos];}
    inline char  operator [](int _pos) const{return str[_pos];}

    /* Pointer as Iterators */

    // inline char *begin()             {return str;}
    // inline const char *begin()  const{return str;}
    // inline const char *cbegin() const{return str;}
    // inline char *end()             {return str + len;}
    // inline const char *end()  const{return str + len;}
    // inline const char *cend() const{return str + len;}

    /* Compare Functions*/

    // template <size_t _len>
    inline bool operator <(const string &rhs) const{return strcmp(str,rhs) < 0;}

    inline bool operator ==(const char *rhs) const{return !strcmp(str,rhs);}

    // template <size_t _len>
    friend int Compare(const string &lhs,const string &rhs) {
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
    using FileName_t = string <64>;
    const FileName_t name;

  public:
    File() = delete;
    File(const char *_name) : name(_name) {}
    File(const FileName_t &_name) : name(_name) {}
    ~File() {}

    /* Locate read position. */
    void locateI(size_t __n) {
        seekg(BIAS + __n * unit);
    }

    /* Locate read position with extra bias. */
    void locateI(size_t __N,size_t __B) {
        seekg(BIAS + __n * unit + __B);
    }

    /* Locate write position. */
    void locateO(int __n) {
        seekp(BIAS + __n * unit);
    }

    /* Locate write position with extra bias. */
    void locateO(size_t __N,size_t __B) {
        seekp(BIAS + __n * unit + __B);
    }

    /* Open a new file and change to write/read mode. 
       True  if succesfully created. 
       False if already existed.*/
    bool create() {
        open();
        this->std::fstream::seekg(0);
        if(good()) return false;
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

using key_t   = string <72>;
using value_t = string <68>; 

class BlockList {
  private:
    // #define MAX_SIZE   509
    // #define BLOCK_SIZE 511

    #define MAX_SIZE   9
    #define BLOCK_SIZE 11
    using Return_List = std::vector <value_t>;

    struct pair_t {
        key_t   key;
        value_t val;
        pair_t() = default;
        pair_t(const key_t& _k,const value_t &_v) : key(_k),val(_v) {}
        int cmp(const key_t &_k,const value_t &_v) const{
            int _C = Compare(key,_k);
            if(_C) return _C;
            else return Compare(val,_v);
        }
        const pair_t &get(const key_t &_k,const value_t &_v) {
            key = _k;
            val = _v;
        }
    };
    struct Node {
        int pre;    // Prev Node
        int nxt;    // Next Node
        int index;  // Index of Node and 
        int count;  // Count of pairs
        pair_t max; // Maximum pair value
        Node() {pre = nxt = index = count = 0;}
        Node(int p,int n,int i,int c,const pair_t &m):
            pre(p),nxt(n),index(i),count(c),max(m) {}
    };
    struct Data {
        pair_t arr[BLOCK_SIZE];
        operator pair_t *() {return arr;}
    };
    
    File<Node,sizeof(Node) > NodeFile;
    File<Data,-sizeof(Data)> DataFile;
    Node header;// Header information.

    /** 
     * @brief  Allocate one new node. 
     * @return New Node's index 
    */
    inline int newNode() {return ++header.count;}

    /* Read one Node */
    inline void read(Node &tmp,int index) {
        NodeFile.locateI(index);
        NodeFile.read(tmp);
    }
    /* Read one Data */
    inline void read(Data &tmp,int index) {
        DataFile.locateI(index);
        DataFile.read(tmp);
    }
    /* Write one Node */
    inline void write(const Node &tmp,int index) {
        NodeFile.locateO(index);
        NodeFile.write(tmp);
    }
    /* Write one Data */
    inline void write(Data &tmp,int index) {
        DataFile.locateO(index);
        DataFile.write(tmp);
    }

  public:
    BlockList() = delete;
    BlockList(const char *c1,const char *c2):NodeFile(c1),DataFile(c2) {
        /* Create space for null node and header */
        if(NodeFile.create()) {
            Node tmp;
            NodeFile.write(tmp);
            NodeFile.write(tmp);
        }
        /* Empty space for data. */
        if(DataFile.create()) {
            Data tmp;
            DataFile.write(tmp);
        }
        read(header,-1);
    }
    ~BlockList() {
        write(header,-1);
    }


  protected:
    /* Memmove data. */
    void copy(pair_t *dst,pair_t *src,int count) {
        memmove((void *)src,dst,sizeof(pair_t) * count);
    }
    /* Find first index no less than key */
    int find_first(pair_t *A,const key_t &key,int len) {
        for(int i = 0 ; i < len ; ++i) {
            if(Compare(A[i].key,key) >= 0) {return i;}
        }
        return len;
    }
    /* Find first index larger than key */
    int find_last(pair_t *A,const key_t &key,int len) {
        for(int i = 0 ; i < len ; ++i) {
            if(Compare(A[i].key,key) > 0) {return i;}
        }
        return len;
    }
    /* Find first index no less than key/value */
    int find_first(pair_t *A,const key_t &key,const value_t &val,int len) {
        for(int i = 0 ; i < len ; ++i) {
            int cmp = A[i].cmp(key,val);
            if(cmp > 0)  return i;
            if(cmp == 0) return ~i;
        }
        return len;
    }
    /* Insert at certain node. */
    void insert_at(Node &cur,const key_t &key,const value_t &val) {
        Data tmp;
        read(tmp,cur.index);
        int pos = find_first(tmp,key,val,cur.count);
        if(pos < 0) return;
        
        if(pos == cur.count) {cur.max = tmp[pos].get(key,val);} 
        else {copy(tmp + pos + 1,tmp + pos,cur.count - pos);}
        if(++cur.count <= MAX_SIZE) {
            write(cur,cur.index);
            write(tmp,cur.index);
        } else {split(cur,tmp);}
    }
    /* Split at certain list. */
    void split(Node &cur,Data &tmp) {
        Node nxt(cur.index,cur.nxt,newNode(),cur.count >> 1,cur.max);
        cur.count -= nxt.count;
        cur.nxt    = nxt.index;
        Data ans;
        copy(ans,tmp + cur.count,nxt.count);
        write(cur,cur.index);
        write(tmp,cur.index);
        write(nxt,nxt.index);
        write(ans,nxt.index);
        read(cur,nxt.nxt);
        cur.pre = nxt.index;
        write(cur,cur.index); 
    }
    /* Erase at certain node. */
    void erase_at(Node &cur,const key_t &key,const value_t &val) {
        if(cur.count == 1) {
            if(cur.max.cmp(key,val)) return;     
            Node tmp;
            read(tmp,cur.pre);
            tmp.nxt = cur.nxt;
            write(tmp,tmp.index);
            read(tmp,cur.nxt);
            tmp.pre = cur.pre;
            write(tmp,tmp.index);
            return;
        }
        Data tmp;
        read(tmp,cur.index);

        int pos = ~find_first(tmp,key,val,cur.count);
        if(pos < 0) return; // Not found

        if(pos == cur.count - 1) {cur.max = tmp[pos - 1];}
        else {copy(tmp + pos,tmp + pos + 1,cur.count - pos);}

        write(cur,cur.index);
        write(tmp,cur.index);
    }


  public:
    void insert(const key_t &key,const value_t &val) {
        Node cur;
        read(cur,0);
        int index = cur.nxt;
        if(!index) { /* Empty node case. */
            index = cur.nxt = cur.pre = newNode();
            write(cur,0);

            cur.index = index;
            cur.pre = cur.nxt = 0;
            cur.max.get(key,val);            
            cur.count = 1;
            write(cur,index);

            Data tmp;
            tmp[0].get(key,val);
            return write(tmp,index);
        }
        while(index) {
            read(cur,index);
            int cmp = cur.max.cmp(key,val);
            if(cmp < 0) {
                index = cur.nxt;
            } else {
                return insert_at(cur,key,val);
            }
        } /* Reach the end. */
        insert_at(cur,key,val);
    }


    void erase(const key_t &key,const value_t &val) {
        Node cur;
        read(cur,0);
        int index = cur.nxt;
        while(index) {
            read(cur,index);
            int cmp = cur.max.cmp(key,val);
            if(cmp < 0) {
                index = cur.nxt;
            } else {
                return erase_at(cur,key,val);
            }
        } /* Reach the end. */
    }

    void find_all(const key_t &key,Return_List &vec) {
        vec.clear();
        Node cur;
        read(cur,0);
        int index = cur.nxt;
        while(index) {
            read(cur,index);
            int cmp = Compare(cur.max.key,key);
            if(cmp < 0) {
                index = cur.nxt;
            } else {
                Data tmp;
                int i = find_first(tmp,key,cur.count);
                int j = find_last (tmp,key,cur.count);
                while(i < j) {vec.emplace_back(vec[i++]);}
                if(j != cur.count) return;
            }
        }
    }

};







signed main() {
    string <64> str1 = "fuckULL";
                            // "F:/Code/Github/Rubbish_BookStore_System/pretasks/a.txt";
    string <64> str2 = "fuckPythonInterpreter";
                            // "F:/Code/Github/Rubbish_BookStore_System/pretasks/b.txt";
    BlockList ans(str1,str2);


    return 0;
}