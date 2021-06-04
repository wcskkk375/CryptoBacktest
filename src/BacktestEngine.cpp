#include "utils.h"
#include "BacktestEngine.h"

using namespace std;
namespace BacktestEngine {
    
    BacktestingEngine::BacktestingEngine(BacktestingConfig config){
        filename = config.filename;
        symbol = config.symbol_name;
        match_engine = MatchEngine(config);
        portfolio_engine = PortfolioEngine(config);         
        pparse_string = config.pparse_string;
        market_snapshot.symbol = symbol;
        market_snapshot.num_decimal_places = config.num_decimal_places;
        tick_size = config.tick_size;
    }

    string BacktestingEngine::parseString(string line){
        return pparse_string(line);
    }

    void BacktestingEngine::AppendIndicator(string indicator_name,IndicatorEngine * calculator){            
        indicator_operator[indicator_name] = calculator;
    }

    void BacktestingEngine::getMarketPrice(vector<TradeData> trade_list){
        if (trade_list.size() != 0){
            market_price = trade_list.back().price;
        }
    }

    void BacktestingEngine::getTs(JsonWrapper tick){
        market_snapshot.current_ts = tick.getLLong("ts");
    }

    void BacktestingEngine::getBasicInfo(JsonWrapper tick){        
        getTs(tick);
        parseMarketDepth(tick,&market_snapshot);
        JsonWrapper trades = tick.getJsonObjectOrArray("trade_data");
        market_snapshot.market_trades = parseTrades(trades,symbol);
        getMarketPrice(market_snapshot.market_trades);
    }

    void BacktestingEngine::printStrategyInfo(){
        cout<<"run to "<<run_times<<endl;
        cout<<"exposure : "<<portfolio_engine.position<<" profit : "<<
        portfolio_engine.portfolio_profit<<"match order num :"<<portfolio_engine.match_result_record.size()<<endl;
    }

    void BacktestingEngine::RunStrategy(){
        cout<<filename<<endl;
        ifstream myfile;
        myfile.open(filename.data());
        string temp;
            
        while(getline(myfile,temp)) 
        {                         
            run_times ++;               
            if (run_times % 100000 == 0){printStrategyInfo();match_engine.printOrderList();}
            JsonDocument djson;
            JsonWrapper tick = djson.parseFromString(parseString(temp).c_str());  
            getBasicInfo(tick);
            vector<MatchResult> match_result = match_engine.updateMatchEngine(market_snapshot);
            portfolio_engine.updatePortfolio(market_price,market_snapshot.current_ts,match_result);
            vector<Order> current_operation = OnData(tick);
            match_engine.AppendOrderList(current_operation);   
        } 
        myfile.close();
        match_engine.Clean();
    }
    // virtual ~BacktestingEngine(); 
}
