#include "Enum.h"
using namespace std;
namespace BacktestEngine{
    string getEnumStatus(Status s){
        string string_status = "";
        switch (s){
            case Status::Canceled:string_status = "Canceled";break;
            case Status::Finished:string_status = "Finished";break;
            case Status::Opened:string_status = "Opened";break;
            case Status::Cancelling:string_status = "Cancelling";break;
            case Status::Finishing:string_status = "Finishing";break;
            case Status::Opening:string_status = "Opening";break;
            case Status::Rejected:string_status = "Rejected";break;
        }
        return string_status;
    }
        
    string getEnumDirection(Direction d){
        string string_direction = "";              
        if (d == Direction::Buy){string_direction = "Buy";}
        else if (d == Direction::Sell){string_direction = "Sell";}
        return string_direction;
    }

    string getEnumOrdertype(OrderType ot){
        string string_order_type = "";
        switch (ot){
            case OrderType::Market:string_order_type = "Market";break;
            case OrderType::Limit:string_order_type = "Limit";break;
            case OrderType::PostOnly:string_order_type = "PostOnly";break;
        }
        return string_order_type;
    }
}