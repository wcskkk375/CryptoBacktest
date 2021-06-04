#ifndef MatchEngine_H
#define MatchEngine_H

#include <string>
#include <vector>
#include "utils.h"
// #include "rapidjson/document.h"
using namespace std;


namespace BacktestEngine {
    class MatchEngine {
    public:
        string symbol;
        int order_id = 0;
        double open_order_latency = 100;
        double close_order_latency = 100;
        double finish_order_latency = 100;
        MarketSnapshot market_snapshot;
        vector<Order> OrderList;
        Log match_engine_log = Log(); 
        Log order_detailed = Log("order_detailed.csv"); 

        MatchEngine(BacktestingConfig config);
                     
        MatchEngine();
        
        // virtual ~MatchEngine();

        double getVolumeAhead(Order order);

        void AppendOrder(Order append_order);

        void AcceptOrder(vector<Order>::iterator accept_order);

        void RemoveOrder(Order order);

        void CancelOrder(Order order);

        void preProcessOrder();

        void AppendOrderList(vector<Order> order_list);

        void CleanOrderList();

        vector<MatchResult> MatchOrder();

        vector<Order> getOrderList();

        void printOrderList();

        vector<MatchResult> updateMatchEngine(MarketSnapshot ms);

        void Clean();

        void writeLog(Order order);
    };
}
#endif /* MatchEngine_H */