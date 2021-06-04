#include "StrategyIndicator.h"

using namespace std;

namespace BacktestEngine {

    double MarketRangeIndicator::CalculateIndicator(MarketSnapshot market_snapshot) {
        for (auto sample = market_snapshot.market_trades.begin();sample != market_snapshot.market_trades.end();sample ++){
            ts_list.push_back(sample->ts);
            market_price_list.push_back(sample->price);
        }            
        size_t anchor_i = 0;
        double max_price = 0;
        double min_price = 0;
        if (market_price_list.size() != 0){
            max_price = market_price_list.back();
            min_price = market_price_list.back();
            for (size_t i = 0;i<market_price_list.size();i++){
                if (market_snapshot.current_ts - ts_list[i] > 10000){
                    anchor_i = i;
                }
            else{
                max_price = (max_price > market_price_list[i]) ? max_price : market_price_list[i];
                min_price = (max_price < market_price_list[i]) ? min_price : market_price_list[i];
                }
            }
            market_range = (max_price - min_price) / market_price_list.back();
            market_range = (market_range > 0.001) ? market_range:0.001;
        }           
        market_price_list.erase(market_price_list.begin(),market_price_list.begin()+anchor_i);
        ts_list.erase(ts_list.begin(),ts_list.begin()+anchor_i);
        return market_range;
    }

}