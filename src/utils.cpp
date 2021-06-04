#include"utils.h"
#include <cmath>
using namespace std;
namespace BacktestEngine{

vector<TradeData> parseTrades(JsonWrapper trades,string symbol){                     
    size_t size = trades.size(); 
      
    vector<TradeData> trade_list; 
    if (size != 0){
        
        for (size_t i = 0; i < size; i++) {
            JsonWrapper sample_trade = trades.getJsonObjectAt(i);
            size_t sub_size = sample_trade.size();
            // cout<<"step1.5_"<<sub_size<<endl;
            for (size_t j = 0; j < sub_size; j++) {
                JsonWrapper sub_trade = sample_trade.getJsonObjectAt(j);
                
                string d = sub_trade.getString("direction");
                
                Direction direction;
                if (d == "buy"){
                    direction = Direction::Buy;
                }
                else{
                    direction = Direction::Sell;
                }
                try{
                    TradeData trade = TradeData(symbol,Str2Dou(sub_trade.object["price"].GetString()),sub_trade.getDouble("amount"),direction,sub_trade.getLong("ts"));                        
                    // sub_trade.getDouble()
                    trade_list.push_back(trade);
                }
                catch(logic_error){
                    printf("exception");
                }  
                
            }
            
        }
    }
    return trade_list;
}

StrategyDirection mergeDirection(StrategyDirection a,StrategyDirection b){
    if (a == StrategyDirection::Both){return b;}
    if (a == StrategyDirection::Stop){return a;}
    if (a == StrategyDirection::Long){
        if (b == StrategyDirection::Both || b == StrategyDirection::Long){return a;}
        else {return StrategyDirection::Stop;}
    }
    if (a == StrategyDirection::Short){
        if (b == StrategyDirection::Both || b == StrategyDirection::Short){return a;}
        else {return StrategyDirection::Stop;}
    }
    return StrategyDirection::Stop;
}

inline double Str2Dou(string num,int precision) {
    const char *p = num.c_str();
    double r = 0.0;
    bool neg = false;
    if (*p == '-') {
        neg = true;
        ++p;
    }
    while (*p >= '0' && *p <= '9') {
        r = (r*10.0) + (*p - '0');
        ++p;
    }
    if (*p == '.') {
        double f = 0.0;
        int n = 0;
        ++p;
        while (*p >= '0' && *p <= '9') {
            f = (f*10.0) + (*p - '0');
            ++p;
            ++n;
            if (n == precision){break;}
        }
        r += f / std::pow(10.0, n);
    }
    if (neg) {
        r = -r;
    }
    return r;
}

void parseMarketDepth(JsonWrapper tick, MarketSnapshot * ms){
    int num_decimal_places = ms->num_decimal_places;
    ms->ask_price.clear();
    ms->bid_price.clear();
    ms->ask_volume.clear();
    ms->bid_volume.clear();
    JsonWrapper depth = tick.getJsonObjectOrArray("depth");
    JsonWrapper asks = depth.getJsonObjectOrArray("asks");
    JsonWrapper bids = depth.getJsonObjectOrArray("bids");
    
    size_t asks_size = asks.size();   
    for (size_t i = 0; i < asks_size; i++){
        ms->ask_price.push_back(Str2Dou(asks.object[i][0].GetString(),num_decimal_places));
        ms->ask_volume.push_back(Str2Dou(asks.object[i][1].GetString(),num_decimal_places));    
    }
    size_t bids_size = bids.size();
    for (size_t i = 0; i < bids_size; i++){
        ms->bid_price.push_back(Str2Dou(bids.object[i][0].GetString(),num_decimal_places));
        ms->bid_volume.push_back(Str2Dou(bids.object[i][1].GetString(),num_decimal_places));
    }
}

}