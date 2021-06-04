#include "Portfolio.h"
using namespace std;
namespace BacktestEngine {
    
    PortfolioEngine::PortfolioEngine(){ 
    }

    PortfolioEngine::PortfolioEngine(BacktestingConfig config){
        symbol = config.symbol_name;
        active_commission = config.commission_for_active;
        passive_commission = config.commission_for_passive;
        portfolio_log.writeString("ts,profit,position,price");        
    }
    
    // virtual ~PortfolioEngine();
            
    double PortfolioEngine::getFilledVolume(Order order){
        return order.filled_volume;
    }

    double PortfolioEngine::getFinishedFilledVolume(Order order){
        if (order.status == Status::Finished){
            return order.filled_volume;
        }
        else{
            return 0;
        }
            
    }
    void PortfolioEngine::updatePortfolio(double market_price,long ts,vector<MatchResult> match_result_list){
        current_market_price = market_price;
        current_ts = ts;
        double current_filled_volume = 0;
        double current_profit = 0;
        portfolio_profit += position * (market_price - prev_market_price);        
        for(auto sample_order = match_result_list.begin(); sample_order != match_result_list.end();sample_order++){
            double bonus = (sample_order->execution_type == ExecutionType::Active) ? active_commission : passive_commission;            
            current_filled_volume = sample_order->filled_volume * sample_order->direction;          
            current_profit = (market_price - (*sample_order).price) * current_filled_volume - bonus * (*sample_order).price * abs(current_filled_volume);
            position += current_filled_volume;
            portfolio_profit += current_profit;
            match_result_record.push_back((*sample_order));
        }   
        resetMarketPrice();         
    }

    void PortfolioEngine::resetMarketPrice(){
        market_price_record.push_back(current_market_price);
        portfolio_value_record.push_back(portfolio_profit);
        ts_record.push_back(current_ts);
        position_record.push_back(position);
        if (ts_record.size() % 1000 == 0){
            portfolio_log.writeString(to_string(current_ts) + "," + to_string(portfolio_profit)
             + "," + to_string(position) + "," + to_string(current_market_price));
        }    
        prev_market_price = current_market_price;
    }

    void PortfolioEngine::Clean(){
        portfolio_log.log.close();
    }
}
