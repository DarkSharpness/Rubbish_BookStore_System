#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "system.hpp"
#include "log.hpp"

namespace dark {

class commandManager {
    LogWriter log;
  public:
    ~commandManager() {}
    commandManager():log("dark.log") {}
    bool runCommand() {}
};


}

#endif
