#ifndef Order_H
#define Order_H
#include "utils.h"
using namespace std;
namespace BacktestEngine {
    class Order{
        public:
        string symbol;
        double price;
        double volume;
        Direction direction;
        double filled_volume = 0;
        long create_ts;
        long process_ts = -1;
        Status status;
        Operation operation;
        int order_id;
        OrderType order_type;
        ExecutionType execution_type;
        ofstream * order_log = nullptr;

        Order(string symbol_name,double p,double v,Direction d,long t,OrderType ot = OrderType::Limit);
        

        Order(const Order& order);

        ~Order();

        void setLog(ofstream * log);

        void setProcessTs(long current_ts);

        bool isLegalStatus(Status s);

        void setStatus(Status s,long current_ts);

        void setExecutionType(ExecutionType e);

        void setVolumeAhead(double v);

        bool isAvailable();

        vector<MatchResult> processLimitOrder(MarketSnapshot ms);

        vector<MatchResult> processPostOnlyOrder(double market_price,double market_volume,Direction market_direction,long ts);

        private:
        double volume_ahead = 0;
        
    };
}

#endif /* Order_H */