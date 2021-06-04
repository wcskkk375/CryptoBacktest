
#include <string>
#include <vector>
#include "JsonWrapper.h"
#include <iostream>
#include "Log.h"
#include "Enum.h"
#include "Order.h"
#include "Data.h"
#include "BacktestConfig.h"
#ifndef Utils_H
#define Utils_H
using namespace std;
namespace BacktestEngine {

    vector<TradeData> parseTrades(JsonWrapper trades,string symbol);

    StrategyDirection mergeDirection(StrategyDirection a,StrategyDirection b);

    inline double Str2Dou(string num,int precision = 1);

    void parseMarketDepth(JsonWrapper tick,MarketSnapshot * ms);

}    
#endif /* Utils_H */