#ifndef Config_H
#define Config_H
#include "BacktestEngine.h"
using namespace std;

namespace BacktestEngine {
    // string BacktestingEngine::parseString(string line);

    // 1000 is 1second latency

    inline string ExampleParseString(string line);

    BacktestingConfig generateConfig();

}
#endif