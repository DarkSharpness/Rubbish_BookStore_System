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
    #define MAXN 64
    LogWriter log;
    std::string input;       // Input buffer.
    std::string token[MAXN]; // Maximum string number.
    size_t count; // Count of tokens

    Exception login();
    Exception logout();
    Exception reg();
    Exception changePWD();
    Exception addUser();
    Exception remove();
    Exception buy();
    Exception select();
    Exception modify();
    Exception import();
    Exception showLog();
    Exception showFinance();
    Exception show();

    /* Just Split the string and get command. */
    Command_t split(std::string &str) noexcept{
        count = 0;
        size_t i = 0;
        while(true) {
            while(str[i] == ' ') {++i;}
            token[count].clear();
            if(!str[i]) break;
            while(str[i] != ' ') {token[count].push_back(str[i++]);}
            ++count;
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
    commandManager():log("dark.log") {}
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
