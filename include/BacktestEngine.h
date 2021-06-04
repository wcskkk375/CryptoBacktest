#include "utils.h"
#ifndef BacktestEngine_H
#define BacktestEngine_H
#include <map>
#include <fstream>
#include "MatchEngine.h"
#include "JsonDocument.h"
#include "Indicator.h"
#include "Portfolio.h"

using namespace std;
namespace BacktestEngine {

    class BacktestingEngine{
        public:
        string filename;
        string symbol;
        MatchEngine match_engine;
        PortfolioEngine portfolio_engine; 
        int run_times = 0;
        double market_price;
        double tick_size = 0.1;
        map<string,IndicatorEngine *>indicator_operator;
        MarketSnapshot market_snapshot;
        pf pparse_string;

        BacktestingEngine(BacktestingConfig config);

        string parseString(string line);

        vector<Order> OnData(JsonWrapper tick);

        void AppendIndicator(string indicator_name,IndicatorEngine * calculator);

        void getMarketPrice(vector<TradeData> trade_list);

        void getTs(JsonWrapper tick);

        void getBasicInfo(JsonWrapper tick);

        void printStrategyInfo();

        void RunStrategy();

        // virtual ~BacktestingEngine();

        private:
    }; 

    
}

#endif /* BacktestEngine_H */