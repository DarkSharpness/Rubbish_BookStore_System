#ifndef _LOG_HPP_
#define _LOG_HPP_

#include "utils.hpp"
#include "fileio.hpp"

#include <fstream>
#include <iomanip>

namespace dark {

/* Custom log writer. */
class LogWriter : private File {
  private:
    File logInfo;
    File tradeInfo;
    std::string buffer;
    
    /* Write a char string into buffer. */
    void writeInfo(const char *msg) {
        buffer += msg;
    }

    /* Write a cstring into buffer. */
    void writeInfo(const std::string &msg) {
        buffer += msg;
    }

    /* Write a char into buffer. */
    void writeInfo(char __ch) {
        buffer += __ch;
    }

    /* Clear the buffer and write the info by __F. */
    void writeInfo(File &__F) {
        __F.write(buffer.front(),buffer.size());
    }

    /* End of expansion with a new line.*/
    void writeLog() {
        buffer += '\n';
        writeInfo(logInfo);
        buffer.clear();
    }

    /* End of expansion with a new line.*/
    void writeTrade() {
        buffer += '\n';
        writeInfo(tradeInfo);
        writeInfo(logInfo);
        buffer.clear();
    }

    /* Write infos in the File. */
    void writeFile(File &__F) {
        size_t count = __F.tellp();
        __F.seekg(0);

        buffer.resize(count);
        __F.read(buffer.front(),count);

        std::cout << buffer;
        buffer.clear();

        __F.seekp(0,std::ios::end);
    }

  public:
    LogWriter(): 
        File("bin/Hastin.log"),
        logInfo("bin/Conless.log"),
        tradeInfo("bin/Dark.log"){ // I love them!.
        if(create()) {
            size_t count = 0;
            write(count);
        }
        logInfo.create();
        logInfo.seekp(0,std::ios::end);
        tradeInfo.create();
        tradeInfo.seekp(0,std::ios::end);
    }

    /* Write a line of log. */
    template <class T,class ...V>
    void writeLog(const T &msg,const V &...msgs) {
        writeInfo(msg);
        return writeLog(msgs...);
    }

    
    /* Write a line of trade. */
    template <class T,class ...V>
    void writeTrade(const T &msg,const V &...msgs) {
        writeInfo(msg);
        return writeTrade(msgs...);
    }


    /* Show the inner log. */
    Exception showLog() noexcept {
        std::cout << "---------------------------------------\n";
        std::cout << "The following part is about action of Users:\n";
        writeFile(logInfo);

        size_t count;
        seekg(0);
        read(count);
        std::cout << "---------------------------------------\n";
        std::cout << "The following part is about finance:\n"
                  << (count > 1 ? "Count of trades:" : "Count of trade:")
                  << count << '\n';

        writeFile(tradeInfo);
        std::cout << "---------------------------------------\n";
        return No_Exception();
    }

    /* Write income and outcome. */
    Exception add(double income,double outcome) noexcept {
        size_t count;
        seekg(0);
        read(count);
        if(++count == 1) {
            seekp(0);
            write(count);
            write(income);
            write(outcome);
        } else {
            double lastin;
            double lastout;

            read(lastin);
            read(lastout);

            // std::cout << lastin << " and " << lastout << '\n';

            income  += lastin ;
            outcome += lastout;

            seekp(0);

            write(count);
            write(income);
            write(outcome);

            seekp(0,std::ios::end);

            write(lastin);
            write(lastout);
        }
        return No_Exception();
    }
    /* Query last num trade.(With '\\n') */
    Exception query(size_t num = -1ULL) {
        if(!num) {
            std::cout << '\n';
        } else {
            size_t count;
            double sumin;
            double sumout;
            seekg(0);
            read(count);
            if(num > count && ~num) return Exception("Query over count.");
            read(sumin);
            read(sumout);
            if(~num && num != count) {
                double prein;
                double preout;
                seekg(-(num << 4),std::ios::end);
                read(prein);
                read(preout);
                sumin  -= prein ;
                sumout -= preout;
            }
            std::cout << "+ "  << std::fixed << std::setprecision(2) << sumin
                      << " - " << std::fixed << std::setprecision(2) << sumout
                      << '\n';
        }
        return No_Exception();
    }

};

}

#endif
