#ifndef Indicator_H
#define Indicator_H

#include "utils.h"
using namespace std;
using namespace BacktestEngine;

class IndicatorEngine {
    public:
        IndicatorEngine(){}
        
        virtual double CalculateIndicator(MarketSnapshot market_snapshot);
    };

class ObImblanceIndicator:public IndicatorEngine{};   

class VpinIndicator:public IndicatorEngine{
        vector<double>volume_list;
        vector<long> ts_list;
        double vpin = 0;
        double CalculateIndicator(MarketSnapshot market_snapshot);
    };  
#endif /* Indicator_H */