#ifndef _LOG_HPP_
#define _LOG_HPP_

#include "utils.hpp"
#include "fileio.hpp"

#include <fstream>
#include <iomanip>

namespace dark {

/* Custom log writer. */
class LogWriter : private File{
  private:
  public:
    LogWriter(): File("hastin.log") { // I love hastin.
        if(create()) {
            size_t count = 0;
            write(count);
        }
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

            std::cout << lastin << " and " << lastout << '\n';

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
            if(num > count && ~num) return Exception("Invalid");
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
