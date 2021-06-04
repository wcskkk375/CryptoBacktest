#include "Indicator.h"
using namespace std;
using namespace BacktestEngine;
double ObImblanceIndicator::IndicatorEngine::CalculateIndicator(MarketSnapshot market_snapshot) {
    size_t depth_size = min(market_snapshot.ask_price.size(),static_cast<size_t>(50));
    double ask_cumulate_volume = 0;
    double bid_cumulate_volume = 0;
    for (size_t i = 0; i < depth_size; i++){
        ask_cumulate_volume += market_snapshot.ask_volume[i];
        bid_cumulate_volume += market_snapshot.bid_volume[i];
    }
    return ask_cumulate_volume / (ask_cumulate_volume + bid_cumulate_volume) - 0.5; 
}   

double VpinIndicator::CalculateIndicator(MarketSnapshot market_snapshot){
    for (auto sample = market_snapshot.market_trades.begin();sample != market_snapshot.market_trades.end();sample ++){
        ts_list.push_back(sample->ts);
        volume_list.push_back(sample->volume * sample->direction);
    }   
    double abs_volume = 0;
    double sum_volume = 0;         
    size_t anchor_i = 0;
    if (volume_list.size() != 0){
        for (size_t i = 0;i<volume_list.size();i++){
            if (market_snapshot.current_ts - ts_list[i] > 10000){
                anchor_i = i;
            }
        else{
                sum_volume += volume_list[i];
                abs_volume += abs(volume_list[i]);
            }
        }
        vpin = sum_volume / abs_volume;
    }           
    volume_list.erase(volume_list.begin(),volume_list.begin()+anchor_i);
    ts_list.erase(ts_list.begin(),ts_list.begin()+anchor_i);
    return vpin;
}
