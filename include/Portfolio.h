#include "utils.h"
#ifndef Portfolio_H
#define Portfolio_H

using namespace std;
namespace BacktestEngine {
    class PortfolioEngine {
    public:
        string symbol;
        vector<MatchResult> match_result_record;
        vector<double> portfolio_value_record;
        vector<long> ts_record;
        vector<double>market_price_record;
        vector<double>position_record;
        double portfolio_profit = 0;
        double position = 0;
        double current_market_price;
        double prev_market_price;
        double active_commission = 0.0003;
        double passive_commission = -0.00015;
        long current_ts;
        Log portfolio_log = Log("portfolio.csv");

        vector<Order> OrderList;

        PortfolioEngine();

        PortfolioEngine(BacktestingConfig config);

        // virtual ~PortfolioEngine();
                     
        double getFilledVolume(Order order);

        double getFinishedFilledVolume(Order order);

        void updatePortfolio(double market_price,long ts,vector<MatchResult> match_result_list);

        void resetMarketPrice();

        void Clean();
    };

}
#endif /* Portfolio_H */