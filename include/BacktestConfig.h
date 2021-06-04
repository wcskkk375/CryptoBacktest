#ifndef BacktestConfig_H
#define BacktestConfig_H
#include <string>
#include "utils.h"
using namespace std;
namespace BacktestEngine{
    typedef string (*pf)(string);
    string parseString(string line);
    class BacktestingConfig{
        public:
            string symbol_name = "BTC-USDT";
            string filename = "";
            double tick_size = 0.1;
            int num_decimal_places = 1;
            int OPENLATENCY = 100;
            int CANCELLATENCY = 100;
            int FINISHLATENCY = 100;
            double commission_for_active = 0.00025;
            double commission_for_passive = -0.0002;
            typedef string (*pf)(string);
            pf pparse_string = parseString;
            BacktestingConfig(ConfigData cd);
            BacktestingConfig();
            // virtual string parseString(string line);
    };   
}
#endif