#include "Log.h"

using namespace std;
namespace BacktestEngine {
    Log::Log(){}
    Log::Log(string log_name){
        log.open(log_name, ios::app); 
    }
    void Log::writeString(string s){
        log<<s<<endl;
    }

}
