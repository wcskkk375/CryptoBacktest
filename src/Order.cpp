#include "Order.h"
using namespace std;
namespace BacktestEngine {
    Order::Order(string symbol_name,double p,double v,Direction d,long t,OrderType ot){       
        symbol = symbol_name;
        price = p;
        volume = v;
        direction = d;
        operation = Operation::Open;
        status = Status::Opening;
        create_ts = t;
        order_type = ot;
    }

    Order::Order(const Order& order){
        symbol = order.symbol;
        price = order.price;
        volume = order.volume;
        direction = order.direction;
        filled_volume = order.filled_volume;
        operation = order.operation;
        status = order.status;
        create_ts = order.create_ts;
        process_ts = order.process_ts;
        order_id = order.order_id;
        order_type = order.order_type;
    }

    Order::~Order(){
        // cout<<"in destroy log"<<(order_log == nullptr)<<endl;
        if (order_log != nullptr){
            string string_direction = getEnumDirection(direction);
            string string_status = getEnumStatus(status);
            string string_order_type = getEnumOrdertype(order_type);
            (*order_log)<<to_string(order_id)<<","<<symbol<<","<<to_string(volume)<<","
            <<to_string(price)<<","<<string_direction<<","<<to_string(filled_volume)<<","
            <<to_string(create_ts)<<","<<to_string(process_ts)<<","<<string_status
            <<","<<string_order_type<<endl;
        }
    }

    void Order::setLog(ofstream * log){            
        // cout<<"in set log"<<(order_log == nullptr)<<endl;
        order_log = log;
    }

    void Order::setProcessTs(long current_ts){
        process_ts = current_ts;
    }

    bool Order::isLegalStatus(Status s){
        bool legal = false;
        if (s > status){legal = true;}
        else{
            // cout<<"illegal status change from : "<<getEnumStatus(status)<<" to "<<getEnumStatus(s)<<endl;
        }
        return legal;
    }

    void Order::setStatus(Status s,long current_ts){
        if (isLegalStatus(s)){
            status = s;
            setProcessTs(current_ts);
        }           
    }

    void Order::setExecutionType(ExecutionType e){
        execution_type = e;
    }

    void Order::setVolumeAhead(double v)
    {
        volume_ahead = v;
    }

    bool Order::isAvailable(){
        if (status == Status::Cancelling || status == Status::Opened){
            return true;
        }
        else{
            return false;
        }
    }

    vector<MatchResult> Order::processLimitOrder(MarketSnapshot ms)
    {
        vector<MatchResult> match_result;
        double current_filled_volume = 0;
        if (isAvailable()){
            if (direction == Direction::Sell){
                for (size_t i = 0;i<ms.bid_price.size();i++){
                    if (price <= ms.bid_price[i]){
                        current_filled_volume = min(volume,ms.bid_volume[i]);
                        filled_volume += current_filled_volume;
                        volume = max(0.,volume - current_filled_volume);
                        volume_ahead = 0;
                    }
                    else{
                        break;
                    }
                    if (volume == 0) {break;}
                }                
            }
            if (direction == Direction::Buy){
                for (size_t i = 0;i<ms.ask_price.size();i++){
                    if (price >= ms.ask_price[i]){
                        current_filled_volume = min(volume,ms.ask_volume[i]);
                        filled_volume += current_filled_volume;
                        volume = max(0.,volume - current_filled_volume);
                        volume_ahead = 0;
                    }
                    else{
                        break;
                    }
                    if (volume == 0) {break;}
                }                
            }
            if (current_filled_volume != 0){
                MatchResult mr = MatchResult(order_id, price, current_filled_volume, direction,
                ms.current_ts,ExecutionType::Active);                   
                match_result.push_back(mr);
            }
            if (volume <= 0){setStatus(Status::Finishing,ms.current_ts);}
        }
        // cout<<"match order : "<<match_result.size()<<endl;
        return match_result;
    }


    vector<MatchResult> Order::processPostOnlyOrder(double market_price,double market_volume,Direction market_direction,long ts)
    {
        vector<MatchResult> match_result;
        double current_filled_volume = 0;
        if (isAvailable()){
            if (market_direction == Direction::Buy && direction == Direction::Sell){
                if (market_price > price){
                    current_filled_volume = volume;
                    filled_volume += current_filled_volume;
                    // cout<<"filled volume : "<<volume<<"v"<<filled_volume<<endl;
                    volume = 0;
                }
                else if (market_price == price){
                    double delta_volume = market_volume - volume_ahead;
                    if (delta_volume >= 0){
                        current_filled_volume = min(volume,delta_volume);
                        filled_volume += current_filled_volume; 
                        volume -= current_filled_volume;
                        // cout<<"partial filled volume : "<<current_filled_volume<<"v"<<filled_volume<<endl;
                    }
                    else{
                        volume_ahead -= market_volume;
                    }
                }
            }
            else if (market_direction == Direction::Sell && direction == Direction::Buy){
                if (market_price < price){
                    current_filled_volume = volume;
                    filled_volume += current_filled_volume;
                    volume = 0;
                }
                else if (market_price == price){
                    double delta_volume = market_volume - volume_ahead;
                    if (delta_volume >= 0){
                        current_filled_volume = min(volume,delta_volume);
                        filled_volume += current_filled_volume;
                        volume -= current_filled_volume;
                    }
                    else{
                        volume_ahead -= market_volume;
                    }
                }
            }
            if (current_filled_volume != 0){
                MatchResult mr = MatchResult(order_id,price,current_filled_volume,direction,ts,ExecutionType::Passive);                   
                match_result.push_back(mr);
            }
            if (volume <= 0){setStatus(Status::Finishing,ts);}
        }
        // cout<<"match order : "<<match_result.size()<<endl;
        return match_result;
    }
}

