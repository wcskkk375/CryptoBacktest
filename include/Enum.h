#ifndef Enum_H
#define Enum_H
#include <string>
using namespace std;
namespace BacktestEngine {
    enum Direction {Buy = 1,Sell = -1};

    enum StrategyDirection {Long = 1,Short = -1,Both = 2,Stop = -2};

    enum Status {Opening = 0,Opened = 1,Cancelling = 2,Canceled = 3,Finishing = 4,Finished = 5,Rejected = 6};
    
    enum Operation {Open,Cancel};

    enum OrderType {Limit=1,PostOnly = 0,Market = 2};

    enum ExecutionType {Active=1,Passive = 0};

    string getEnumStatus(Status s);

    string getEnumDirection(Direction d);

    string getEnumOrdertype(OrderType ot);
}
#endif /* Enum_H */