#ifndef StrategyUtils_H
#define StrategyUtils_H
#include "utils.h"
using namespace std;

namespace BacktestEngine {
    
    void filterOrderbookPriceBySpread(MarketSnapshot market_snapshot,double target_spread,double * ask_price,double * bid_price);

    void filterOrderbookPriceByVolume(MarketSnapshot market_snapshot,double target_spread,double * ask_price,double * bid_price);

    void filterOrderbookPriceByCumvolume(MarketSnapshot market_snapshot,double target_spread,double * ask_price,double * bid_price);

    StrategyDirection GetDirection(vector<Order> order_list,double current_exposure);
}
#endif