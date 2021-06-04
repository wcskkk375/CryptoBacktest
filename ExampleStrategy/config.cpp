#include "config.h"
using namespace std;

namespace BacktestEngine {

    string symbol_name = "BTC-USDT";

    string filename = "sample_data.txt";

    double tick_size = 0.1;

    int num_decimal_places = 1;

    int OPENLATENCY = 100;

    int CANCELLATENCY = 100;

    int FINISHLATENCY = 100;

    double commission_for_active = 0.00025;

    double commission_for_passive = -0.0002;

    inline string ExampleParseString(string line){
        // raw data format:
        // {'ch': 'market.BTC-USDT.depth.step0', 'ts': 1619107953317, 
        // 'depth': {'asks': [ [54655.2, 33],], 'bids': [[54624.6, 6309]], 
        // 'ch': 'market.BTC-USDT.depth.size_150.high_freq', 'event': 'update', 'id': 30321275143, 
        // 'mrid': 30321275143, 'ts': 1619107953316, 'version': 564839010}, 
        // 'trade_data': [[{"amount": 2, "quantity": 0.002, "trade_turnover": 109.3442,
        //  "ts": 1619107749227, "id": 303206983210000, "price": 54672.1, "direction": "sell"}]],
        //  'index_price': 54563.60666666667, 'contract_price': 54624.6, 
        //  'XAUUSD': [1618404125009, 1736.85], 'NASDAQ': [1618404125953, 13830.5], 
        //  'CHINA50': [1618404123358, 16877.9], 'DOW': [1618404123267, 33771.5]} ,     

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

    BacktestingConfig generateConfig(){
        struct ConfigData example_config = 
            { symbol_name : symbol_name,
            filename : filename,
            tick_size :tick_size,
            num_decimal_places : num_decimal_places,
            OPENLATENCY : OPENLATENCY,
            CANCELLATENCY : CANCELLATENCY,
            FINISHLATENCY : FINISHLATENCY,
            commission_for_active : commission_for_active,
            commission_for_passive : commission_for_passive
            };
        BacktestingConfig bc = BacktestingConfig(example_config);
        bc.pparse_string = ExampleParseString;
        return bc;
    }

}
