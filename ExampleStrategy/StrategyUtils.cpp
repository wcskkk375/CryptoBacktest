#include "StrategyUtils.h"
using namespace std;

namespace BacktestEngine {
    void filterOrderbookPriceBySpread(MarketSnapshot market_snapshot,double target_spread,double * ask_price,double * bid_price){
        int ask_i = 0,bid_i = 0;
        double ask_cum_volume = 0,bid_cum_volume = 0;       
        double target_spread_price = target_spread * market_snapshot.ask_price[0];  
        *ask_price = market_snapshot.ask_price[ask_i];
        *bid_price = market_snapshot.bid_price[bid_i];
        double spread = *ask_price - *bid_price;
        while (spread < target_spread_price || bid_i == market_snapshot.bid_price.size() || ask_i == market_snapshot.ask_price.size()){
            bid_i ++;
            *bid_price = market_snapshot.bid_price[bid_i];

            ask_i ++;
            *ask_price = market_snapshot.ask_price[ask_i];
            
            spread = *ask_price - *bid_price;
        }
    }


    void filterOrderbookPriceByVolume(MarketSnapshot market_snapshot,double target_spread,double * ask_price,double * bid_price){
        int ask_i = 0,bid_i = 0;
        double ask_cum_volume = 0,bid_cum_volume = 0;       
        double target_spread_price = target_spread * market_snapshot.ask_price[0];  
        *ask_price = market_snapshot.ask_price[ask_i];
        *bid_price = market_snapshot.bid_price[bid_i];
        double spread = *ask_price - *bid_price;
        while (spread < target_spread_price || bid_i == market_snapshot.bid_price.size() || ask_i == market_snapshot.ask_price.size()){
            if (market_snapshot.ask_volume[ask_i] > market_snapshot.bid_volume[bid_i]){          
                *bid_price = market_snapshot.bid_price[bid_i];
                bid_cum_volume += market_snapshot.bid_volume[bid_i];
                spread = *ask_price - *bid_price;
                bid_i ++;
            }
            else {              
                *ask_price = market_snapshot.ask_price[ask_i];
                ask_cum_volume += market_snapshot.ask_volume[ask_i];
                spread = *ask_price - *bid_price;
                ask_i ++;
            }
        }
    }

    void filterOrderbookPriceByCumvolume(MarketSnapshot market_snapshot,double target_spread,double * ask_price,double * bid_price){
        int ask_i = 0,bid_i = 0;
        double ask_cum_volume = 0,bid_cum_volume = 0;       
        double target_spread_price = target_spread * market_snapshot.ask_price[0];  
        *ask_price = market_snapshot.ask_price[ask_i];
        *bid_price = market_snapshot.bid_price[bid_i];
        double spread = *ask_price - *bid_price;
        while (spread < target_spread_price || bid_i == market_snapshot.bid_price.size() || ask_i == market_snapshot.ask_price.size()){
            if (ask_cum_volume + market_snapshot.ask_volume[ask_i] > bid_cum_volume + market_snapshot.bid_volume[bid_i]){               
                *bid_price = market_snapshot.bid_price[bid_i];
                bid_cum_volume += market_snapshot.bid_volume[bid_i];
                spread = *ask_price - *bid_price;
                bid_i ++;
            }
            else {               
                *ask_price = market_snapshot.ask_price[ask_i];
                ask_cum_volume += market_snapshot.ask_volume[ask_i];
                spread = *ask_price - *bid_price;
                ask_i ++;
            }
        }
    }

    StrategyDirection GetDirection(vector<Order> order_list,double current_exposure){
        int num_buy_order = 0;
        int num_sell_order = 0;
        StrategyDirection allow_trade = StrategyDirection::Both;
        StrategyDirection allow_trade_from_exposure = StrategyDirection::Both;
        StrategyDirection allow_trade_from_buy_orders = StrategyDirection::Both;
        StrategyDirection allow_trade_from_sell_orders = StrategyDirection::Both;
        for(auto sample_order = order_list.begin(); sample_order != order_list.end();sample_order++){
            if (sample_order->direction == Direction::Buy){num_buy_order++;}
            else if (sample_order->direction == Direction::Sell){num_sell_order++;}
        }
        if (current_exposure >= 2){
            allow_trade_from_exposure = StrategyDirection::Short;        
        }
        else if (current_exposure <= -2){
            allow_trade_from_exposure = StrategyDirection::Long;
        }
        
        if (num_buy_order > 0){
            allow_trade_from_buy_orders = StrategyDirection::Short;
        }
           
        if (num_sell_order > 0){
            allow_trade_from_sell_orders = StrategyDirection::Long;
        }
        allow_trade = mergeDirection(allow_trade_from_exposure,mergeDirection(allow_trade_from_buy_orders,allow_trade_from_sell_orders));
        return allow_trade;
    }
}