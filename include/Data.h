#ifndef Data_H
#define Data_H
using namespace std;
namespace BacktestEngine {
    
    struct MatchResult{
        int from_order_id;
        double filled_volume;
        double price;
        Direction direction;
        long ts;
        ExecutionType execution_type;
        MatchResult(int oid,double p,double v,Direction d,long t,ExecutionType e){
            from_order_id = oid;
            price = p;
            filled_volume = v;
            direction = d;
            ts = t;
            execution_type = e;
        }
        MatchResult(){}
    };

    struct TradeData{
        string symbol;
        double price;
        double volume;
        Direction direction;
        long ts;
        OrderType order_type;
        TradeData(string s,double p,double v,Direction d,long t,OrderType ot = OrderType::Limit)
        {
            symbol = s;
            price = p;
            volume = v;
            direction = d;
            ts = t;
            order_type = ot;
        }
        TradeData(){}
    };

    struct MarketSnapshot{
        long current_ts;
        int num_decimal_places;
        string symbol;
        vector<double> ask_price;
        vector<double> bid_price;
        vector<double> ask_volume;
        vector<double> bid_volume;
        vector<TradeData> market_trades;
        MarketSnapshot(){};
        MarketSnapshot(vector<double> ap,vector<double> av,vector<double> bp,vector<double> bv,vector<TradeData> mt,long t){
            ask_price = ap;
            ask_volume = av;
            bid_price = bp;
            bid_volume = bv;
            market_trades = mt;
            current_ts = t;
        };
    };
    struct ConfigData{
        string symbol_name = "BTC-USDT";
        string filename = "";
        double tick_size = 0.1;
        int num_decimal_places = 1;
        int OPENLATENCY = 100;
        int CANCELLATENCY = 100;
        int FINISHLATENCY = 100;
        double commission_for_active = 0.00025;
        double commission_for_passive = -0.0002;
    };

}    
#endif /* Data_H */