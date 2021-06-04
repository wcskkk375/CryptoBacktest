#ifndef Log_H
#define Log_H
#include <string>
#include <iostream>
#include <fstream>


// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"

using namespace std;
namespace BacktestEngine {        
    class Log{
        public:
            ofstream log; 
            Log();
            Log(string log_name);
            void writeString(string s);
    };
}
#endif /* Log_H */