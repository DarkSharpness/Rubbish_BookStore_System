## Project Name : Rubbish_BookStore_System

## Author : DarkSharpness

## !!! The following parts have not been completely implemented yet!


***

### Structure:

1. ReadCommand by commandManager.
2. CommandManager analyze the input command and therefore perform it.

a. Book Command.

Check with AccountSystem First. Then turn to BookSystem.

b. User Command.

Turn to AccountSystem.

c. Log Command and others.(Not Much)

Do it within the commandManager.



### Class Info & Function(with comments):

String: Custom string storing information.

```C++
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
    /* Judge Whether a string is empty */
    bool operator !(void) const{return str[0];}
    /* Basic copy from another cstr. */
    string &operator =(const char *ptr) {
        memset(str,0,sizeof(str));
        strcpy(str,ptr);
        return *this;
    }

    /* Force to change into a cstr. */
    operator const char *() const{
        return str;
    }

    /* Reference to data. */

    inline char &operator [](int _pos)      {return str[_pos];}
    inline char  operator [](int _pos) const{return str[_pos];}

    /* Pointers as Iterators */

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

    /* Output by ostream. */

    friend std::ostream &operator <<(std::ostream &os,const string &tmp) {
        return os << tmp.str;
    }

};
```

File: Manage file inout operation.

```C++
/* Custom File manager. */
template <class T,int BIAS,size_t unit = sizeof(T)>
class File : public std::fstream {
    FileName_t name;

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
        open();
        if(good() && peek() != EOF) return true;
        close(); // close first
        open(std::ios::out);
        close();
        open();
        return true;
    }

    /* Open the given File in binary mode(default). */
    void open(std::ios_base::openmode __mode = (std::ios_base::openmode)28) {
        this->std::fstream::open(name,__mode);
    }

    /* Just read one of any type.*/
    template <class _T>
    void read(_T *ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(ptr),sizeof(_T));
    }

    /* Just write one object of any type.*/
    template <class _T>
    void write(const _T &val,size_t siz = sizeof(_T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
    }

    /* Just overwrite one object of any type.
       Not that the cur pointer is not moved.*/
    template <class _T>
    void overwrite(const _T &val,size_t siz = sizeof(_T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
        seekp(-siz,std::ios::cur);
    }
};

```

LogWriter: Simply write logs.

```C++
/* Custom log writer. */
class LogWriter : public std::fstream{
  private:
    FileName_t name;
  public:
    LogWriter() = delete;
    LogWriter(const char *_name) : name(_name) {}
    LogWriter(const FileName_t &_name) : name(_name) {}
    ~LogWriter() {}
  
    /* Just read one of any type.*/
    template <class _T>
    void read(_T *ptr) {
        this->std::fstream::read(reinterpret_cast <char *>(ptr),sizeof(_T));
    }

    /* Just write one object of any type.*/
    template <class _T>
    void write(const _T &val,size_t siz = sizeof(_T)) {
        this->std::fstream::write(reinterpret_cast <const char *>(&val),siz);
    }

};

```

BlockList: Standard Data Container.

```C++
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


```

Book: Info of one book.

```C++

/* Book without ISBN. */
struct BookBase {

    BookName_t Name;        // Name
    Author_t   Author;      // Author
    Keyword_t  Keyword[60]; // Keywords

    size_t keywordNum;      // number of keywords
    size_t quantity;        // quantitiy of books 
    double cost;            // cost of one book
  
    /* Simply clearing zero. */
    BookBase() {memset(this,0,sizeof(*this));}

    /* Normal copy. */
    BookBase(const BookBase &_B) {memcpy(this,&_B,sizeof(_B));}
    /* Normal assign. */
    BookBase &operator =(const BookBase &_B) {
        if(this != &_B) {memcpy(this,&_B,sizeof(_B));}
        return *this;
    }
    /* Custom Output for a bookbase object. */
    friend std::ostream &operator <<(std::ostream &os, const BookBase &book) {
        os << book.Name   << '\t'
           << book.Author << '\t'
           << book.Keyword[0];
        for(int i = 1; i < book.keywordNum; ++i)
            os << '|' << book.Keyword[i];
        return os << std::fixed << std::setprecision(2)
                  << '\t' << book.cost << '\t' << book.quantity;
    }

};


```

Account: Info of one user.

```C++

/* Custom account. */
class Account {
  private:
    UserID_t   ID;       // User ID
    UserName_t Name;     // User Name
    Password_t Password; // Password of the Account
    Level_t    Level;    // Priority Level
    size_t     count;    // Times of log-in

    friend class AccountSystem;
  public:

    /* Must provide params */
    Account() = delete;
  
    /* Initialize function.*/
    Account(const UserID_t &_I,const UserName_t& _N,
            const Password_t &_P,Level_t _L,size_t _C):
        ID(_I),Name(_N),Password(_P),Level(_L),count(_C) {}
};


```

BookSystem: Book Management system.

```C++

class BookSystem {
  private:
    BlockList <ISBN_t,BookBase> library;
  public:
    ~BookSystem() = default;
    BookSystem(): library("book1.bin","book2.bin","book3.bin") {}

    /* Show by ISBN. */
    void show(const ISBN_t &) {}
    /* Show by non-ISBN. */
    void show(const string <64> &,show_t opt){}
    /* Remove elements.(buy books) */
    void remove(const ISBN_t &,int count) {}
    /* Select one book. */
    void select(const ISBN_t &) {}
    /* Modify with given params. */
    void modify(const ISBN_t &,const BookBase &) {}
    /* Import books with given quantity and cost */
    void import(const ISBN_t &,size_t quantity,double cost) {}

};

```

AccountSystem: User Management system.

```C++

class AccountSystem {
  private:
    BlockList <UserID_t,Account> library;
    struct User {  
        int     index; // Index in the vector
        Level_t Level; // Level of the people
        ISBN_t  ISBN;  // The ISBN of book selected
    };
    // std::stack <User,std::vector<User>> stack;

    std::vector <User> stack; // User stack.

  public:
    /* Just clear the user stack. */
    ~AccountSystem() = default;
    /* Just do nothing.*/
    AccountSystem(): library("user1.bin","user2.bin","user3.bin") {}

    /* Log in an account. */
    void login(const UserID_t &,const Password_t &){}
    /* Log out an account. */
    void logout(){}
    /* Register an account. */
    void registering(const UserID_t &,
                     const Password_t &,
                     const UserName_t &){}
    /* Change an account's password. */
    void changePassword(const UserID_t &,
                        const Password_t &,
                        const Password_t &){}
    /* Add a new user. */
    void insert(const UserID_t &,
                const Password_t &
                Level_t,
                const UserName_t &){}
    /* Remove a user. */
    void remove(const UserID_t &){}
};

```

commandManager: Managing input commands.

```C++
class commandManager {
    LogWriter log;
    BookSystem books;
    AccountSystem users;
  public:
    ~commandManager() {}
    commandManager():log("dark.log") {}
    bool runCommand() {}
};
```

main(): Just read commands and work.

```C++
signed main() {
    dark::commandManager _C;
    while(_C.runCommand());
    return 0;
}
```

Storage Info:
book1.bin: map  nodes info
book2.bin: list nodes info
book3.bin: data holder of bookbase

user1.bin: map  nodes info
user2.bin: list nodes info
user3.bin: data holder of user

dark.log: custom log file

Update History:
22/12/07 : First Draft.
