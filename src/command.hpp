#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "system.hpp"
#include "log.hpp"
#include <map>



namespace dark {

/* Record command. */
const std::map <std::string,Command_t> commandMap = {
    {"exit",Command_t::exit},
    {"quit",Command_t::exit},
    {"su",Command_t::login},
    {"logout",Command_t::logout},
    {"register",Command_t::reg},
    {"passwd",Command_t::passwd},
    {"useradd",Command_t::userad},
    {"delete",Command_t::remove},
    {"show",Command_t::show},
    {"buy",Command_t::buy},
    {"select",Command_t::select},
    {"modify",Command_t::modify},
    {"import",Command_t::import},
    {"finance",Command_t::show_f},
    {"log",Command_t::log}
};


class commandManager {
  private:
    #define MAXN 256
    std::string input;       // Input buffer.
    std::string token[MAXN]; // Maximum string number.
    size_t count; // Count of tokens
    BookSystem    Library;
    AccountSystem Users;
    LogWriter     Hastin;

    Exception login() {
        if(count == 2) {
            if(!isValidUserID(token[1].data())) {
                return Exception("Invalid");
            } 
            return Users.login(token[1].data());
        } else if(count == 3) {
            if(!isValidUserID  (token[1].data()) 
            || !isValidPassword(token[2].data())) {
                return Exception("Invalid");
            }
            return Users.login(token[1].data(),token[2].data());
        } else {
            return Exception("Invalid");
        }
    }

    Exception logout() {
        if(count == 1) {
            return Users.logout();
        } else {
            return Exception("Invalid");
        }
    }

    Exception reg() {
        if(count == 4) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())
            || !isValidPassword(token[3].data())) {
                return Exception("Invalid");
            } else {
                return Users.registering(token[1].data(),
                                         token[2].data(),
                                         token[3].data());
            }
        } else {
            return Exception("Invalid");
        }
    }

    Exception changePWD() {
        if(count == 3) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())) {
                return Exception("Invalid");
            } else {
                return Users.changePassword(token[1].data(),
                                            token[2].data());
            }
        } else if(count == 4) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())
            || !isValidPassword(token[3].data())) {
                return Exception("Invalid");
            } else {
                return Users.changePassword(token[1].data(),
                                            token[2].data(),
                                            token[3].data());
            }
        } else {
            return Exception("Invalid");
        }
    }

    Exception addUser() {
        if(count == 5) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())
            || !isValidUserName(token[4].data())) {
                return Exception("Invalid");
            }
            auto pair = getPrivilege(token[3].data());
            if(!pair.first) {return Exception("Invalid");}
            return Users.addUser(token[1].data(),token[2].data(),
                                 pair.second,token[4].data());
        } else {
            return Exception("Invalid");
        }
    }

    Exception remove() {
        if(count == 2) {
            if(!isValidUserID(token[1].data())) {
                return Exception("Invalid");
            } else {
                return Users.removeUser(token[1].data());
            }
        } else {
            return Exception("Invalid");
        }
    }

    Exception show() {
        if(!Users.checkLevel(Level_t::Customer)) {
            return Exception("Invalid");
        }
        if(count == 1) {
            return Library.showAll();
        } else if(count == 2) {
            char *str;
            regex_t opt = getType(token[1].data(),str);
            switch(opt) {
                case regex_t::showISBN:    return Library.showISBN(str);  
                case regex_t::showAuthor:  return Library.showAuthor(str);
                case regex_t::showBookName:return Library.showBookName(str);
                case regex_t::showKeyword: 
                    if(!isValidOneKeyword(str)) // One keyword only
                         return Exception("Invalid");
                    else return Library.showKeyword(str);
                default: return Exception("Invalid");
            }
        } else {
            return Exception("Invalid");
        }
    }

    Exception buy() {
        if(count == 3) {
            if(!Users.checkLevel(Level_t::Customer)
            || !isValidISBN(token[1].data())) {
                return Exception("Invalid");
            }

            auto pair1 = getQuantity(token[2].data());
            if(!pair1.first) {
                return Exception("Invalid");
            }

            Exception result = Library.remove(token[1].data(),pair1.second);
            if(!result.test()) {
                Hastin.add(Library.tradeMoney,0);
            }
            return result;
        } else {
            return Exception("Invalid");
        }
    }

    Exception select() {
        if(count == 2) {
            if(!Users.checkLevel(Level_t::Librarian)
            || !isValidISBN(token[1].data())) {
                return Exception("Invalid");
            } else {
                Library.select(token[1].data());
                return Users.select(token[1].data());
            }
        } else {
            return Exception("Invalid");
        }
    }

    Exception modify() {
        if(count <= 1 || count >= 7 || !Users.checkLevel(Level_t::Librarian)) {
            return Exception("Invalid");
        }
        auto Iptr = Users.selected();
        if(Iptr == nullptr) return Exception("Invalid");
        bool optMap[6] = {1,0,0,0,0,0};
        Book tmp = Library.getBook(*Iptr);
        for(size_t i = 1 ; i < count ; ++i) {
            char *str;
            regex_t opt = getType(token[i].data(),str);
            if(optMap[opt]) return Exception("Invalid");
            optMap[opt] = true;
            switch(opt) {
                case regex_t::showISBN:
                    if(tmp.ISBN == str) return Exception("Invalid");
                    tmp.ISBN = str;
                    break;
                case regex_t::showAuthor:
                    tmp.Author = str;
                    break;
                case regex_t::showBookName:
                    tmp.Name = str;
                    break;
                case regex_t::showKeyword:
                    tmp.Keyword = str;
                    break;
                case regex_t::showPrice:
                    tmp.cost = std::stod(str);
                    break;
                default:
                    return Exception("Invalid");
            }
        }
        return Library.modify(*Iptr,tmp);
    }

    Exception import() {
        if(count == 3 && Users.checkLevel(Level_t::Librarian)) {
            auto Iptr  = Users.selected();
            auto pair1 = getQuantity(token[1].data());
            auto pair2 = getMoney   (token[2].data());
            if(!Iptr || !pair1.first || !pair2.first) 
                return Exception("Invalid");
            Exception result = Library.import(*Iptr,pair1.second);
            if(!result.test()) {
                Hastin.add(0,pair2.second);
            }
            return result;
        } else {
            return Exception("Invalid");
        }
    }
    
    Exception showLog() {
        return No_Exception();
    }


    Exception showFinance() {
        if(count == 2) {
            return Hastin.query();
        } else if(count == 3) {
            auto pair = getQuantity(token[2].data());
            if(!pair.first) return Exception("Invalid");
            return Hastin.query(pair.second);
        } else {
            return Exception("Invalid");
        }
    }

  private:
    /* Just Split the string and get command. */
    Command_t split(std::string &str) noexcept {
        count = 0;
        size_t i = 0;
        while(true) {
            while(str[i] == ' ') {++i;}
            token[count].clear();
            if(!str[i]) break;
            while(str[i] != ' ' && str[i]) {token[count].push_back(str[i++]);}
            ++count;
            if(!str[i]) break;
        }
        auto iter = commandMap.find(token[0]);
        // Not found.
        if(iter == commandMap.end()) {return Command_t::unknown;}
        else if(iter->second == Command_t::show) {
            return token[1] == "finance" ? Command_t::show_f :
                                           Command_t::show   ;
        } else {
            return iter->second;
        }
    }


  public:
    ~commandManager() = default;
    commandManager()  = default;
    /* Exception will be dealt within this function. */
    bool runCommand() noexcept {
        std::getline(std::cin,input);
        Command_t opt = split(input);
        switch(opt) {
            case Command_t::exit:   return false;
            case Command_t::login:  return login();
            case Command_t::logout: return logout();
            case Command_t::reg:    return reg();
            case Command_t::passwd: return changePWD();
            case Command_t::userad: return addUser();
            case Command_t::remove: return remove();
            case Command_t::show:   return show();
            case Command_t::buy:    return buy();
            case Command_t::select: return select();
            case Command_t::modify: return modify();
            case Command_t::import: return import();
            case Command_t::log:    return showLog();
            case Command_t::show_f: return showFinance();
            default: return Exception("Invalid\n");
        }
    }
    
};


}

#endif
