#ifndef StrategyIndicator_H
#define StrategyIndicator_H
#include "Indicator.h"

using namespace std;

namespace BacktestEngine {
    class MarketRangeIndicator:public IndicatorEngine{
        vector<double>market_price_list;
        vector<long> ts_list;
        double market_range = 0.001;
        double CalculateIndicator(MarketSnapshot market_snapshot);
    };  
}
#endif