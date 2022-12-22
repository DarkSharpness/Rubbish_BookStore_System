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
    LogWriter log;           // Write log only.
    std::string input;       // Input buffer.
    std::string token[MAXN]; // Maximum string number.
    size_t count; // Count of tokens
    BookSystem    Library;
    AccountSystem Users;


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
        }
    }

    Exception show() {
        if(!Users.checkLevel(Level_t::Customer)) {
            return Exception("Invalid");
        }
        if(count == 1) {
            Library.showAll();
        } else if(count == 2) {
            char str[256];
            Show_t opt = getShowType(token[1].data(),str);
            switch(opt) {
                case Show_t::showISBN:    return Library.showISBN(str);  
                case Show_t::showAuthor:  return Library.showAuthor(str);
                case Show_t::showBookName:return Library.showBookName(str);
                case Show_t::showKeyword: return Library.showKeyword(str);
                default: return Exception("Invalid");
            }
        } else {
            return Exception("Invalid");
        }
    }

    Exception buy() {
        if(!Users.checkLevel(Level_t::Customer)) {
            return Exception("Invalid");
        }
        if(count == 3) {
            auto pair1 = getQuantity(token[1].data());
            auto pair2 = getMoney   (token[2].data());
            if(!pair1.first || !pair2.first) {
                return Exception("Invalid");
            }

            const ISBN_t *Iptr = Users.selected();
            if(Iptr == nullptr) return Exception("Invalid");

            return Library.remove(*Iptr,pair1.second);
        } else {
            return Exception("Invalid");
        }
    }

    Exception select() {
        if(!Users.checkLevel(Level_t::Customer)) {
            return Exception("Invalid");
        }
        if(count == 2) {
            if(!isValidISBN(token[1].data())) {
                return Exception("Invalid");
            } else {
                return Users.select(token[1].data());
            }
        } else {
            return Exception("Invalid");
        }
    }
    Exception modify() {

    }
    Exception import() {

    }
    
    Exception showLog() {

    }

        
    Exception showFinance() {


        
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
    ~commandManager() {}
    commandManager(): log() {}
    /* Exception will be dealt within this function. */
    bool runCommand() noexcept{
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
