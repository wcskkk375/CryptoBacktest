#include <string>
#include <vector>
#include "MatchEngine.h"

namespace BacktestEngine {
    MatchEngine::MatchEngine(BacktestingConfig config){
        symbol = config.symbol_name;
        if (match_engine_log.log.is_open()){
            match_engine_log.writeString(
            "order_id,symbol,volume,price,direction,filled_volume,create_ts,process_ts,status,order_type");
        }
        if (order_detailed.log.is_open()){
            order_detailed.writeString(
            "order_id,symbol,volume,price,direction,filled_volume,create_ts,process_ts,status,order_type");
        }
        open_order_latency = config.OPENLATENCY;
        close_order_latency = config.CANCELLATENCY;
        finish_order_latency = config.FINISHLATENCY;
    }
            
    MatchEngine::MatchEngine(){}


    double MatchEngine::getVolumeAhead(Order order){
        double volume_ahead = 0;
        if (order.direction == Direction::Buy){
            for (size_t i = 0;i < market_snapshot.bid_price.size();i++){
                if (market_snapshot.bid_price[i] == order.price){
                    volume_ahead = market_snapshot.bid_volume[i];
                    break;
                }
                else if (market_snapshot.bid_price[i] < order.price){
                    break;
                }
            }
        }
        if (order.direction == Direction::Sell){
            for (size_t i = 0;i < market_snapshot.ask_price.size();i++){
                if (market_snapshot.ask_price[i] == order.price){
                    volume_ahead = market_snapshot.ask_volume[i];
                    break;
                }
                else if (market_snapshot.ask_price[i] > order.price){
                    break;
                }
            }
        }
        return volume_ahead;
    }

    void MatchEngine::AppendOrder(Order append_order){
        append_order.order_id = order_id;    
        OrderList.push_back(append_order);
        order_id ++;       
    }

    void MatchEngine::AcceptOrder(vector<Order>::iterator accept_order){
        if (accept_order->order_type == OrderType::Limit){
            accept_order->setStatus(Status::Opened,market_snapshot.current_ts);
            accept_order->setVolumeAhead(getVolumeAhead(*accept_order));
        }
        else if(accept_order->order_type == OrderType::Market){
            accept_order->setStatus(Status::Opened,market_snapshot.current_ts);
        }
        else if(accept_order->order_type == OrderType::PostOnly){
            if (accept_order->direction == Direction::Buy && accept_order->price >= market_snapshot.ask_price[0]){
                accept_order->setStatus(Status::Rejected,market_snapshot.current_ts);
                // cout<<"reject order"<<endl;
            }
            else if(accept_order->direction == Direction::Sell && accept_order->price <= market_snapshot.bid_price[0]){
                accept_order->setStatus(Status::Rejected,market_snapshot.current_ts);
                // cout<<"reject order"<<endl;
            }
            else{
                accept_order->setStatus(Status::Opened,market_snapshot.current_ts);
                accept_order->setVolumeAhead(getVolumeAhead(*accept_order));
            }
        }    
    }

    void MatchEngine::RemoveOrder(Order order){
        int remove_id = order.order_id;
        for(auto sample_order = OrderList.begin(); sample_order != OrderList.end();sample_order++){
            if (sample_order->order_id == remove_id){    
                sample_order->setLog(&(match_engine_log.log));                 
                sample_order = OrderList.erase(sample_order); 
            }
            if(sample_order == OrderList.end()) break;
        }
    }

    void MatchEngine::CancelOrder(Order order){
        int cancel_id = order.order_id;
        for(auto sample_order = OrderList.begin(); sample_order != OrderList.end();sample_order++){
            if (sample_order->order_id == cancel_id){
                sample_order->setStatus(Status::Cancelling,market_snapshot.current_ts);
            }
            if(sample_order == OrderList.end()) break;
        }
    }

    void MatchEngine::preProcessOrder(){
        for(auto sample_order = OrderList.begin(); sample_order != OrderList.end();sample_order++){
            if (sample_order->status == Status::Opening){
                // cout<<"opening order"<<to_string(market_snapshot.current_ts - sample_order->create_ts)<<endl;
                if ((market_snapshot.current_ts - sample_order->create_ts) > open_order_latency){
                    AcceptOrder(sample_order);
                    // sample_order->setStatus(Status::Opened,market_snapshot.current_ts);
                    // sample_order->setVolumeAhead(getVolumeAhead((*sample_order)));
                    writeLog(*sample_order);
                }
                // cout<<(sample_order->status == Status::Opened)<<endl;
            }
            else if (sample_order->status == Status::Cancelling){
                // cout<<"canceling order"<<to_string(market_snapshot.current_ts - sample_order->create_ts)<<endl;
                if ((market_snapshot.current_ts - sample_order->process_ts) > close_order_latency){
                    sample_order->setStatus(Status::Canceled,market_snapshot.current_ts);
                    writeLog(*sample_order);
                    // RemoveOrder((*sample_order));
                }
                // cout<<(sample_order->status == Status::Canceled)<<endl;
            }
            else if (sample_order->status == Status::Finishing){
                // cout<<"finishing order"<<to_string(market_snapshot.current_ts - sample_order->create_ts)<<endl;
                if ((market_snapshot.current_ts - sample_order->process_ts) > finish_order_latency){
                    sample_order->setStatus(Status::Finished,market_snapshot.current_ts);
                    writeLog(*sample_order);
                    // RemoveOrder((*sample_order));
                }
                // cout<<(sample_order->status == Status::Finished)<<endl;
            }
        }
    }

    void MatchEngine::AppendOrderList(vector<Order> order_list){
        for(auto sample_order = order_list.begin(); sample_order != order_list.end();sample_order++){
            if ((*sample_order).operation == Operation::Open){AppendOrder(*sample_order);}
            else if ((*sample_order).operation == Operation::Cancel){
                CancelOrder((*sample_order));
            }
        }
    }

    void MatchEngine::CleanOrderList(){
        for(auto sample_order = OrderList.begin(); sample_order != OrderList.end();sample_order++){
            if (sample_order->status == Status::Finished || sample_order->status == Status::Canceled
                || sample_order->status == Status::Rejected){
                sample_order->setLog(&(match_engine_log.log)); 
                sample_order = OrderList.erase(sample_order);
            }
            if(sample_order == OrderList.end()) break;
        }
        // cout<<"size of order list : "<<OrderList.size()<<endl;
    }

    vector<MatchResult> MatchEngine::MatchOrder(){
        vector<MatchResult> order_match_result;
        for(auto sample_order = OrderList.begin(); sample_order != OrderList.end();sample_order++){
            if (sample_order->order_type == OrderType::Limit){
                auto temp = (*sample_order).processLimitOrder( market_snapshot);
                if (temp.size() != 0){
                    order_match_result.insert(order_match_result.end(),temp.begin(),temp.end());
                }
            }
            else if (sample_order->order_type == OrderType::PostOnly){
                for(vector<TradeData>::iterator td = market_snapshot.market_trades.begin(); td != market_snapshot.market_trades.end();td++){
                    auto temp = (*sample_order).processPostOnlyOrder( td->price, td->volume, td->direction,td->ts);
                    if (temp.size() != 0){
                        order_match_result.insert(order_match_result.end(),temp.begin(),temp.end());
                    }
                }  
            }
        }                       
        return order_match_result;
    }

    vector<Order> MatchEngine::getOrderList(){
        return OrderList;
    }

    void MatchEngine::printOrderList(){
        cout<<"print OrderList info : "<<endl;
        for(auto sample_order = OrderList.begin(); sample_order != OrderList.end();sample_order++){
            cout<<to_string(sample_order->order_id)<<getEnumDirection(sample_order->direction)<<
            getEnumStatus(sample_order->status)<<endl;
        }
    }

    vector<MatchResult> MatchEngine::updateMatchEngine(MarketSnapshot ms){
        market_snapshot.ask_price = ms.ask_price;
        market_snapshot.ask_volume = ms.ask_volume;
        market_snapshot.bid_price = ms.bid_price;
        market_snapshot.bid_volume = ms.bid_volume;
        market_snapshot.market_trades = ms.market_trades;
        market_snapshot.current_ts = ms.current_ts;
        preProcessOrder();
        CleanOrderList();
        return MatchOrder();
    }

    void MatchEngine::Clean(){
        if (match_engine_log.log.is_open()){match_engine_log.log.close();}         
        if (order_detailed.log.is_open()){order_detailed.log.close();}
    }

    void MatchEngine::writeLog(Order order){
        if (order_detailed.log.is_open()){
            string string_direction = getEnumDirection(order.direction);
            string string_status = getEnumStatus(order.status);
            string string_order_type = getEnumOrdertype(order.order_type);
            order_detailed.writeString(to_string(order_id) + "," + symbol + "," + to_string(order.volume) + ","
            + to_string(order.price) + "," + string_direction + "," + to_string(order.filled_volume) + ","
            + to_string(order.create_ts) + "," + to_string(order.process_ts) + "," + string_status + ","
            +string_order_type);
        }
        
    }
}
