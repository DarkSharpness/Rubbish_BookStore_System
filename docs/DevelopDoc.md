## Project Name : Rubbish_BookStore_System

## Author : [DarkSharpness](https://github.com/DarkSharpness)

---

### File Structure
```
├── RUBBISH_BOOKSTORE_SYSTEM
│   ├── docs
│   │   └── DevelopDoc.md
│   ├── src
│   │   ├── account.hpp
│   │   ├── blocklist.hpp
│   │   ├── book.hpp
│   │   ├── command.hpp
│   │   ├── exception.hpp
│   │   ├── fileio.hpp
│   │   ├── log.hpp
│   │   ├── string.hpp
│   │   ├── system.hpp
│   │   └── utils.hpp
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── README.md
```

### Code Standard

Most of the code will obey the following rules.

```C++

class Myclass; // Exception : string
class MySystem;

#define CONSTANT
MyClass  variableName;
MySystem System;
void functionName;

```

What's more,all source codes are in header-only files, which means you can directly use it by simply include hpp file(s).

### Structure:

1. Read a line of command by commandManager.
2. CommandManager analyzes the input command and therefore perform it.
    1. Book Command: Check with AccountSystem with Level or book selected first. Then turn to BookSystem. Use LogWriter to write Logs.
    2. User Command: Turn to AccountSystem directly. Use LogWriter to write Logs.
    3. Log Command: Turn to Logwriter directly.
3. CommandManager returns an Exception class. When the Exception object is forced to change into bool, it will first print out the Exception message inside(if exception did happen). In this way, I avoid the costly process of throw and try catch in C++.

### Class Info & Function(with comments):

string: Custom string storing information.

```C++
template <size_t len>
class string {
  private:
    char str[len];
  public:
    /* Basic construction functions. */

    string();
    string(std::nullptr_t);
    string(const char *ptr);

    /* Basic copy functions. */

    string &operator =(const string &rhs);
    string &operator =(const char *rhs);

    /* Type-related functions. */

    bool operator !(void) const;
    bool empty() const;
    operator const char *() const;
    explicit operator char *();

    /* Reference to data. */

    inline char &operator [](size_t __pos);
    inline char  operator [](size_t __pos) const;

    /* Compare functions*/

    inline bool operator <(const string &rhs) const;
    inline bool operator ==(const char *rhs) const;
    inline bool operator !=(const char *rhs) const;
    friend int Compare(const string &lhs,const string &rhs);

    /* IO functions. */
  
    friend std::istream &operator >>(std::istream &is,string &tmp);
    friend std::ostream &operator <<(std::ostream &os,const string &tmp);

};
```

Exception: Custom Exception class.(With some easy-to-use functions.)

```C++
class Exception {
  private:
    size_t isException; /* 1:Error || 0:Normal | -1 Exit */
    static std::string message; /* Only one Exception at once. */
  public:
    explicit Exception();
    explicit Exception(bool type);
    Exception(const char *__m);
    Exception(std::string &&__m);
    Exception(const std::string &__m);
    std::string &what() const;
    bool test() const;
    operator bool() const;
};
std::string Exception::message = "Invalid";

inline Exception No_Exception();   /* No Exception Case. */
inline Exception Exit_Exception(); /* Exit Case.*/
```

File: Manage file inout operation.

```C++
class File : public std::fstream {
  private:
    FileName_t name;
  public:
    File(const char *_name): name(_name) {}
    ~File();

    /* Try creating a new file.
       0 if success || 1 if existed.  */
    bool create();

    /* Basic overwrite of std::fstream functions */
    template <class T>
    void read(T &dst,size_t siz = sizeof(T));
    template <class T>
    void write(const T &src,size_t siz = sizeof(T));
    void open(std::ios_base::openmode _mode);
};
```

LogWriter: Simply write logs.

```C++
/* Custom log writer. */
class LogWriter : private File {
  private:
    File logInfo;
    File tradeInfo;
    std::string buffer;
  public:
    LogWriter();

    /* Write a line of information.*/ 

    template <class T,class ...V>
    void writeLog(const T &msg,const V &...msgs);
    template <class T,class ...V>
    void writeTrade(const T &msg,const V &...msgs);

    /* Output functions. */
    Exception showLog() noexcept;
    /* Write income and outcome. */
    Exception add(double income,double outcome) noexcept;
    /* Query last num trade.(Output with '\\n') */
    Exception query(size_t num);
};
```

BlockList: Standard Data Container.(Fully Commented)

```C++
template <class key_t,class value_t>
class BlockList {
  private: 
    /* Constants. */

    #define MIN_SIZE   64
    #define MAX_SIZE   255
    #define MERGE_SIZE 224
    #define BLOCK_SIZE 256

    /* Private structs. */

    struct pair_t; // Key-Value pair wrapping.
    struct Header; // Only store necessary data.
    using Node = pair_t[BLOCK_SIZE];
    using Container = std::list <Header>; // Container of headers.
    #define iterator typename Container::iterator
  
    /* Internal Variables. */

    Container list;  // Container of header.
    std::vector <int> memory; // Container of unused node index.
    std::vector <Header> data; // Header Data
    Node Node_cache1; // Cache
    Node Node_cache2; // Cache
    File nodeFile;    // Data only node File.
    File listFile;    // Store list like a vector.
    bool isNewNode;   // Whether to init a node.

  public:
    /* Public types. */ 
    using Return_Array = std::vector <value_t>;

    /* (De-)Construction related. */

    BlockList(const char *path1,const char *path2);
    ~BlockList();

    /* Basic Functions. */

    bool empty() const;
    /**
     * @brief Tries to insert a key-value pair.
     * @param key Key to be inserted.
     * @param val Value to be inserted.
     * 
     * @return bool True if successfully inserted.
     */
    bool insert(const key_t &key,const value_t &val);
    /**
     * @brief Tries to modify a key-value pair's value. 
     * Note that the value change shouldn't affect the 
     * rank of the value.
     * @param key Key to locate. 
     * @param Mfunc Modify function.
     * 
     * @return bool True if successfully modified.
     */
    template <class Modify_Type>
    bool modify_if(const key_t &key,Modify_Type Mfunc);
    /**
     * @brief Tries to modify key's value into val.
     * Note that the value change shouldn't affect the 
     * rank of the value.
     * @param key Key to locate. 
     * @param Mfunc Modify function.
     * 
     * @return bool True if successfully erased.
     */
    bool modify(const key_t &key,const value_t &val);
    /**
     * @brief Tries to erase a key-value pair.
     * 
     * @return bool True if successfully erased.
     */
    bool erase(const key_t &key,const value_t &val);
    /**
     * @brief Find all the values tied with given key.
     * 
     * @param key The key to match with.
     * @param arr The answer vector where answer will be appended.
     */
    void findAll(const key_t &key,Return_Array &arr);
    /* Print all elements.*/
    template <class Print_Type = std::ostream>
    void printAll(Print_Type &Pfunc = std::cout);

    /* Undefine some unneccesary MACROs*/
    #undef iterator
};
```

Book: Information of one book.

```C++
struct Book {
  public:
    ISBN_t     ISBN;        // Internal standard book number.
    BookName_t Name;        // Name of the book
    Author_t   Author;      // Author's name
    Keyword_t  Keyword;     // Keyword
    size_t quantity;        // quantitiy of books
    double cost;            // cost of one book

    /* Basic functions. */

    Book();
    Book(const Book &rhs);
    Book &operator =(const Book &rhs);
    friend int Compare(const Book &lhs,const Book &rhs);
    bool operator <(const Book &rhs) const;
    friend std::ostream &operator <<(std::ostream &os, const Book &book);
    void init(const char *__ISBN);
};
```

Account: Information of one user.

```C++
struct Account {
  public:
    UserName_t Name;     // User Name
    Password_t Password; // Password of the Account
    Level_t    Level;    // Priority Level

    /* Basic functions. */

    Account();
    Account(const char * __N,const char *__P,Level_t __L);
    void init(const char * __N,const char *__P,Level_t __L);
    friend int Compare(const Account &lhs,const Account &rhs);
    friend bool operator < (const Account &lhs,const Account &rhs);
};
```

BookSystem: Book Management system.

```C++
class BookSystem {
  private:
    BlockList <ISBN_t    ,Book>   libISBN;
    BlockList <Author_t  ,ISBN_t> libAuthor;
    BlockList <BookName_t,ISBN_t> libBookName;
    BlockList <Keyword_t ,ISBN_t> libKeyword;
    std::vector <Book>   arrB; // Book Return List;
    std::vector <ISBN_t> arrI; // ISBN Return List
    Book Book_cache1;
    double tradeMoney; // Record last trade sum.

    friend class commandManager;

  public:

    ~BookSystem();
    BookSystem();

    Exception showAll() noexcept;
    Exception showISBN(const ISBN_t &__I) noexcept;
    Exception showAuthor(const Author_t &__A) noexcept;
    Exception showBookName(const BookName_t &__B) noexcept;
    Exception showKeyword(const Keyword_t &__K) noexcept;
    Exception remove(const ISBN_t &__I,size_t __q);
    Exception select(const ISBN_t &__I) noexcept;
    Book &getBook(const ISBN_t &__I) noexcept;
    Exception modify(const Book &__B,const std::bitset <6> &optMap);
    Exception import(const ISBN_t & __I,size_t __q) noexcept;
};
```

AccountSystem: User Management system.

```C++
class AccountSystem {
  private:
    struct User;
    std::vector <User>             stack; // User stack.
    std::map <UserID_t,size_t>   instack; // Count of user in stack.
    std::vector <Account>            arr; // Return array of Account.
    BlockList <UserID_t,Account> library; // Account library
    Account Account_cache1;
    friend class commandManager;
    std::string tmpString; // Temperary string
  public:

    /* Basic functions. */

    ~AccountSystem();
    AccountSystem();

    Exception login(const UserID_t &__ID,const Password_t &__PWD);
    Exception login(const UserID_t &__ID);
    Exception logout();
    Exception registering(const UserID_t &__ID,const Password_t &__PWD,const UserName_t &__Name);
    Exception changePassword(const UserID_t &__ID,const Password_t &__OLD,const Password_t &__NEW);
    Exception changePassword(const UserID_t &__ID,const Password_t &__NEW);
    Exception addUser(const UserID_t &__ID,const Password_t &__PWD,Level_t __L,const UserName_t &__Name);
    Exception removeUser(const UserID_t &__ID);
    Exception select(const ISBN_t &__I);

    /* Other functions.*/

    void modifyISBN(const ISBN_t &__OLD,const ISBN_t &__NEW);
    inline std::string currentUser() const;
    inline bool checkLevel(int __MIN) const noexcept;
    inline ISBN_t *selected() noexcept;
};

```

commandManager: Managing input commands.

```C++
class commandManager {
  private:
    #define MAXN 512
    std::string input;       // Input buffer.
    std::string token[MAXN]; // Maximum string number.
    size_t count;            // Count of tokens
    BookSystem    Library;
    AccountSystem Users;
    LogWriter     Hastin;   // I invited Hastin as my log writer.
  public:
    commandManager();
    bool runCommand();
};
```

main(): Just create dictionary , read commands and work.

```C++
signed main() {
    std::filesystem::create_directory("bin");
    dark::commandManager Conless; // I love Conless! So I invited Conless as my manager.
    while(Conless.runCommand());
    return 0;
}
```

### Storage Info:

User Info:(1 BlockList)

--- bin/u1.bin

--- bin/u2.bin

Book Info:(4 BlockLists)

--- bin/b1.bin

--- bin/b2.bin

--- bin/b3.bin

--- bin/b4.bin

--- bin/b5.bin

--- bin/b6.bin

--- bin/b7.bin

--- bin/b8.bin

Log Info:

--- bin/Hastin.log: Finance Sum

--- bin/Conless.log: Operation Log

--- bin/Dark.log: Finance Log

### Features

1. The log is written in readable language. You can directly read the log at bin/Conless.log. You can safely clear bin/Conless.log and bin/Dark.log if you are short of disk space. The format of log is as below:
    1. Strings are protected within "" (e.g "P@ssW0rd").
    2. Numbers are directly written down. All prices keep two decimals (e.g 1919810.00,11451.40).
    3. Special nouns like User,ISBN... are protected within \$\$. By the way,Users should (not must) avoid using the character \$ in their Password or UserName or in other strings.
    4. If currently no one has logged in,the current \$ User \$ will be \$ Somebody not logged in \$.
    5. When the User exit the system.It will write an additional log recording the process of poping the User stack,written as \$ User \$ logout.
2. Add ``` #define DARK_ERROR true ``` at the beginning of main.cpp and a relatively more specified error info will be written.
3. The BlockList provides an extra modify_if functions.Users can provide a function: ```Mfunc``` , to help manipulate the internal data. As long as the value matched by key satisfies ```Mfunc(value) == true```,the modify_if will return true. You should integrate judgement function and modify function into Mfunc.Example:
```C++
BlockList <key_t,value_t> list;
key_t   key;
value_t target;
value_t newval;
list.modify_if(key,[&](value_t &val)->bool
{
    if(val != target) return false; // No modification.
    val = newval;
    return true;
})
/* NOTICE: The modification shouldn't change the ranking of the value. */
```

### Update History:
22/12/07 : First Draft.

23/01/05 : Second Draft. Clarify the functions.
