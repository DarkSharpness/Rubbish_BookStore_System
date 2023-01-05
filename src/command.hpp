#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "utils.hpp"
#include "system.hpp"
#include "log.hpp"
#include <bitset>

namespace dark {


class commandManager {
  private:
    #define MAXN 512
    std::string input;       // Input buffer.
    std::string token[MAXN]; // Maximum string number.
    size_t count; // Count of tokens
    BookSystem    Library;
    AccountSystem Users;
    LogWriter     Hastin;

    Exception login() {
        if(count == 2) {
            if(!isValidUserID(token[1].data())) {
                return Exception("Invalid Login User ID");
            }
            Exception result = Users.login(token[1].data());
            if(!result.test()) {
                Hastin.writeLog("$User$: \"",token[1],"\" logged in.");
            }
            return result;
        } else if(count == 3) {
            if(!isValidUserID  (token[1].data()) 
            || !isValidPassword(token[2].data())) {
                return Exception("Invalid Login UserID or Invalid Login PWD");
            }
            Exception result = Users.login(token[1].data(),token[2].data());
            if(!result.test()) {
                Hastin.writeLog("$User$: \"",token[1],"\" logged in.");
            }
            return result;
        } else {
            return Exception("Invalid Login Command Length");
        }
    }

    Exception logout() {
        if(count == 1) {
            Exception result = Users.logout();
            if(!result.test()) {
                Hastin.writeLog("$User$: \"",Users.tmpString,"\" logged out.");
            }
            return result;
        } else {
            return Exception("Invalid Logout Command Length");
        }
    }

    Exception reg() {
        if(count == 4) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())
            || !isValidUserName(token[3].data())) {
                return Exception("Invalid Register UserID/PWD/Name");
            } else {
                Exception result =  Users.registering(token[1].data(),
                                                      token[2].data(),
                                                      token[3].data());
                if(!result.test()) {
                    Hastin.writeLog(Users.currentUser(),
                                    " registered a new account: \"",token[1],
                                    "\" with $Password$: \"",token[2],
                                    "\" and $UserName$: \"",token[3],"\".");   
                }
                return result;
            }
        } else {
            return Exception("Invalid Register Command Length");
        }
    }

    Exception changePWD() {
        if(count == 3) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())) {
                return Exception("Invalid ChangePWD UserID/PWD");
            } else {
                Exception result = Users.changePassword(token[1].data(),
                                                        token[2].data());
                if(!result.test()) {
                    Hastin.writeLog(Users.currentUser(),
                                    " changed the $Password$ of $User$: \"",token[1],
                                    "\" from \"",Users.tmpString,
                                    "\" into \"",token[2],"\".");
                }
                return result;
            }
        } else if(count == 4) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())
            || !isValidPassword(token[3].data())) {
                return Exception("Invalid ChangePWD UserID/PWD");
            } else {
                Exception result = Users.changePassword(token[1].data(),
                                                        token[2].data(),
                                                        token[3].data());
                if(!result.test()) {
                    Hastin.writeLog(Users.currentUser(),
                                    " changed the $Password$ of $User$: \"",token[1],
                                    "\" from \"",token[2],
                                    "\" into \"",token[3],"\".");
                }
                return result;
            }
        } else {
            return Exception("Invalid ChangePWD Command Length");
        }
    }

    Exception addUser() {
        if(count == 5) {
            if(!isValidUserID  (token[1].data())
            || !isValidPassword(token[2].data())
            || !isValidLevel   (token[3].data())
            || !isValidUserName(token[4].data())) {
                return Exception("Invalid AddUser UserID/PWD/Level/UserName");
            }
            Level_t L = getLevel(token[3].data());
            std::string lastCurrent = Users.currentUser();
            Exception result = Users.addUser(token[1].data(),token[2].data(),
                                           L,token[4].data());
            if(!result.test()) {
                Hastin.writeLog(lastCurrent,
                                " add a new $User$: \"",token[1],
                                "\" with $Password$: \"",token[2],
                                "\",$Level$: ",levelMap[L],
                                " and $UserName$: \"",token[4],"\".");
            }
            return result;
        } else {
            return Exception("Invalid AddUser Command Length");
        }
    }

    Exception remove() {
        if(count == 2) {
            if(!isValidUserID(token[1].data())) {
                return Exception("Invalid ID");
            } else {
                Exception result = Users.removeUser(token[1].data());
                if(!result.test()) {
                    Hastin.writeLog(Users.currentUser(),
                                    " removed a $User$: \"",token[1],"\".");
                }
                return result;
            }
        } else {
            return Exception("Invalid Remove Command");
        }
    }

    Exception show() {
        if(!Users.checkLevel(Level_t::Customer)) {
            return Exception("Invalid Show Level");
        }

        if(count == 1) {
            // return No_Exception();
            Exception result = Library.showAll();
            if(!result.test()) {
                Hastin.writeLog(Users.currentUser(),
                                " showed all the books.");
            }
            return result;
        } else if(count == 2) {
            char *str;
            regex_t opt = getType(token[1].data(),str);
            Exception result;
            switch(opt) {
                case regex_t::showISBN:     result = Library.showISBN(str);break;  
                case regex_t::showAuthor:   result = Library.showAuthor(str);break;
                case regex_t::showBookName: result = Library.showBookName(str);break;
                case regex_t::showKeyword:
                    result =
                        !isValidOneKeyword(str) ? // One keyword only
                        Exception("Too many Keywords") :
                        Library.showKeyword(str);
                    break;
                default: return Exception("Invalid Regex Command");
            }
            if(!result.test()) {
                Hastin.writeLog(Users.currentUser(),
                                " showed all the books with ",
                                regMap[opt],": \"",str,"\".");
            }
            return result;
        } else {
            return Exception("Invalid Show Command Length");
        }
    }

    Exception buy() {
        if(count == 3) {
            if(!Users.checkLevel(Level_t::Customer)
            || !isValidISBN(token[1].data())) {
                return Exception("Invalid Buy Level or Invalid ISBN");
            }

            auto pair1 = getQuantity(token[2].data());
            if(!pair1.first) {
                return Exception("Invalid Quantity");
            }

            Exception result = Library.remove(token[1].data(),pair1.second);
            if(!result.test()) {
                Hastin.add(Library.tradeMoney,0);
                std::cout << std::fixed << std::setprecision(2)
                          << Library.tradeMoney << '\n';
                Hastin.writeTrade(Users.currentUser(),
                                  " bought ",token[2],
                                  pair1.second == 1 ? " book" : " books",
                                  " with $ISBN$: \"",token[1],
                                  "\",costing ",
                                  doubleToString(Library.tradeMoney),
                                  " in all.");
            }
            return result;
        } else {
            return Exception("Invalid Buy Command Length");
        }
    }

    Exception select() {
        if(count == 2) {
            if(!Users.checkLevel(Level_t::Librarian)
            || !isValidISBN(token[1].data())) {
                return Exception("Invalid select Level or Invalid ISBN");
            } else {
                Exception result = Users.select(token[1].data());
                if(!result.test()) {
                    Library.select(token[1].data());
                    Hastin.writeLog(Users.currentUser(),
                                    " selected the book with $ISBN$: \"",
                                    token[1],"\".");
                }
                return result;
            }
        } else {
            return Exception("Invalid Select Command Length");
        }
    }

    Exception modify() {
        if(count <= 1 || count >= 8 || !Users.checkLevel(Level_t::Librarian)) {
            return Exception("Invalid Modify Length Length or Invalid Modify Level");
        }

        auto Iptr = Users.selected();
        if(Iptr == nullptr) return Exception("No ISBN selected yet");

        std::bitset <6> optMap = 1;
        Book &tmp = Library.getBook(*Iptr);

        for(size_t i = 1 ; i < count ; ++i) {
            char *str;
            regex_t opt = getType(token[i].data(),str);
            if(optMap.test(opt)) return Exception("Repeat Regex or Invalid Regex in Modify");
            optMap.set(opt);
            switch(opt) {
                case regex_t::showISBN:
                    if(tmp.ISBN == str) return Exception("The same Modified ISBN");
                    tmp.ISBN = str;
                    break;
                case regex_t::showAuthor:
                    tmp.Author = str;
                    break;
                case regex_t::showBookName:
                    tmp.Name = str;
                    break;
                case regex_t::showKeyword:
                    if(!checkRepeatKeyword(str)) return Exception("Repeated Keyword");
                    tmp.Keyword = str;
                    break;
                case regex_t::showPrice:
                    tmp.cost = std::stod(str);
                    break;
                default:
                    return Exception("This shouldn't happen");
            }
        }

        Exception result = Library.modify(tmp,optMap);

        if(!result.test()) {
            std::string info;
            if(optMap.test(1)) {
                info += "new $ISBN$: \"";
                info += (const char*)tmp.ISBN;
                info += "\",";
            }
            if(optMap.test(2)) {
                info += "new $Author$: \"";
                info += (const char *)tmp.Author;
                info += "\",";
            }
            if(optMap.test(3)) {
                info += "new $BookName$: \"";
                info += (const char *)tmp.Name;
                info += "\",";
            }
            if(optMap.test(4)) {
                info += "new $Keyword$: \"";
                info += (const char *)tmp.Keyword;
                info += "\",";
            }
            if(optMap.test(5)) {
                info += "new $Price$: \"";
                info += doubleToString(tmp.cost);
                info += "\",";
            }
            info.back() = '.';

            Hastin.writeLog(Users.currentUser(),
                            " changed the Book $ISBN$: \"",(const char *)(*Iptr),
                            "\" into the new Book with: ",
                            info);


            if(optMap.test(1)) Users.modifyISBN(*Iptr,tmp.ISBN);
        }



        return result;
    }

    Exception import() {
        if(count == 3 && Users.checkLevel(Level_t::Librarian)) {
            auto Iptr  = Users.selected();
            auto pair1 = getQuantity(token[1].data());
            auto pair2 = getMoney   (token[2].data());
            if(!Iptr || !pair1.first || !pair2.first || pair2.second <= 1e-7) 
                return Exception("No ISBN selected or Invalid quantity/money");
            Exception result = Library.import(*Iptr,pair1.second);
            if(!result.test()) {
                Hastin.add(0,pair2.second);
                Hastin.writeTrade(Users.currentUser(),
                                  " imported ",token[1],
                                  pair1.second == 1 ? " book" : " books",
                                  " with $ISBN$: \"",(const char *)(*Iptr),
                                  "\",costing ",
                                  doubleToString(pair1.second),
                                  " in all.");
            }
            return result;
        } else {
            return Exception("Invalid Import Command Length");
        }
    }

    Exception showLog() {
        if(!Users.checkLevel(Level_t::Manager)) {
            return Exception("Not Enough Level to show log");
        }
        return Hastin.showLog();
    }


    Exception showFinance() {
        if(count == 2) {
            if(!Users.checkLevel(Level_t::Manager))
                return Exception("Not enough Level to show Finance");
            return Hastin.query();
        } else if(count == 3) {
            if(!Users.checkLevel(Level_t::Manager))
                return Exception("Not enough Level to show Finance");
            auto pair = getQuantity(token[2].data());
            if(!pair.first && token[2] != "0") return Exception("Invalid Count");
            Exception result = Hastin.query(pair.second);
            if(!result.test()) {
                Hastin.writeLog(Users.currentUser()," showed finance.");
            }
            return result;
        } else {
            return Exception("Invalid Show Finance Command Length");
        }
    }
    Exception quit() noexcept {
        Hastin.writeLog(Users.currentUser()," shutdown this program.");
        for(auto it = Users.stack.rbegin() ; it != Users.stack.rend() ; ++it) {
            Hastin.writeLog((const char *)it->ID,"logged out.");
        }
        Hastin.writeLog("$System$: See you next time!");
        return Exit_Exception();
    }
  private:
    /* Just Split the string and get command. */
    Command_t split(std::string &str) noexcept {
        if(str.size() >= MAXN) return Command_t::unknown;
        count = 0;
        size_t i = 0;
        while(true) {
            while(str[i] == ' ') {++i;}
            token[count].clear();
            if(!str[i]) break;
            while(str[i] != ' ' && str[i]) {token[count].push_back(str[i++]);}
            if(++count == MAXN) return Command_t::unknown;
            if(!str[i]) break;
        }
        auto iter = commandMap.find(token[0]);
        // Not found.
        if(iter == commandMap.end()) {return Command_t::unknown;}
        else if(iter->second == Command_t::show) {
            return count >= 2 && token[1] == "finance" ? 
                                Command_t::show_f :
                                Command_t::show   ;
        } else {
            return iter->second;
        }
    }

  public:
    ~commandManager() = default;
    commandManager()  = default;

    /* Exception will be dealt within this function. */
    Exception runCommand() {
        std::getline(std::cin,input);
        if(!std::cin)  return Exit_Exception();
        Command_t opt = split(input);
        if(count == 0) return No_Exception(); // Space only/
        switch(opt) {
            case Command_t::exit:   return quit();
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
            default: return Exception("Unknown Command");
        }
    }
};


}

#endif
