#include "utils.h"
#include "BacktestEngine.h"
#include "StrategyIndicator.h"
#include "StrategyUtils.h"
#include "config.h"
#include<ctime>

using namespace std;

namespace BacktestEngine {
    
    vector<Order> BacktestingEngine::OnData(JsonWrapper tick){
        vector<Order>  operation;
        auto order_list = match_engine.OrderList;
        double current_exposure = portfolio_engine.position;
        double trade_volume = 1;
        StrategyDirection allow_trade = GetDirection(order_list,current_exposure);

        double target_spread = 0.001;
        double target_sell_price;
        double target_buy_price;
        filterOrderbookPriceBySpread(market_snapshot,target_spread,&target_sell_price,&target_buy_price);
        target_sell_price -= tick_size;
        target_buy_price += tick_size;
        if (allow_trade != StrategyDirection::Stop){
            if (allow_trade != StrategyDirection::Long){

                // double target_sell_price = tick.getJsonObjectOrArray("depth").getJsonObjectOrArray("asks").getArrayAt(0).getDoubleAt(0);
                Order o = Order(symbol,target_sell_price,trade_volume,Direction::Sell,market_snapshot.current_ts,OrderType::PostOnly);
                operation.push_back(o);
                
            }
            if(allow_trade != StrategyDirection::Short){

                // double target_buy_price = tick.getJsonObjectOrArray("depth").getJsonObjectOrArray("bids").getArrayAt(0).getDoubleAt(0);
                Order o = Order(symbol,target_buy_price,trade_volume,Direction::Buy,market_snapshot.current_ts,OrderType::PostOnly);
                operation.push_back(o);
                
            }
        }
        
        for(auto sample_order = order_list.begin(); sample_order != order_list.end();sample_order++){
            if (market_snapshot.current_ts - sample_order->create_ts > 1000){
                (*sample_order).operation = Operation::Cancel;
                operation.push_back((*sample_order));
            }           
        }
        return operation;       
    }

};

int main()
{
    clock_t startTime,endTime;
    startTime = clock();
	BacktestingEngine be = BacktestingEngine(generateConfig());
    be.RunStrategy();
    endTime = clock();
    cout << "The run time is: " <<static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}


