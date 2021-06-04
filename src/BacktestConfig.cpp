#include <string>
#include "BacktestConfig.h"
using namespace std;
namespace BacktestEngine{   

    BacktestingConfig::BacktestingConfig(ConfigData cd){
        symbol_name = cd.symbol_name;
        filename = cd.filename;
        tick_size = cd.tick_size;
        num_decimal_places = cd.num_decimal_places;
        OPENLATENCY = cd.OPENLATENCY;
        CANCELLATENCY = cd.CANCELLATENCY;
        FINISHLATENCY = cd.FINISHLATENCY;
        commission_for_active = cd.commission_for_active;
        commission_for_passive = cd.commission_for_passive;
    }   

    BacktestingConfig::BacktestingConfig(){}

    string parseString(string line){
        line = line.substr(0, line.length() -3 );     
        
        while( true ) 
        {
                string::size_type pos(0);
                if((pos=line.find("'"))!=string::npos )
                {
                        line.replace(pos,1,"\"");
                }                                 
                else break ;                     
        }        
        return line;
    }
}
