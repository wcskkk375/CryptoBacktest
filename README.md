### CryptoBacktest

#### How to use

* mkdir build
* cd build
* cmake ..
* make
* cd ExampleStrategy/benchmark_strategy
* mkdir build
* cd build
* cmake ..
* make
* ./Strategy

#### Apply your market data
* modify the ExampleStrategy/config.cpp, set your data filename, tick size, and the function to parse the string
* the function string ExampleParseString(string line) will parse the raw data to a json string, you need to modify this function based on your data format
* write your trading strategy in main.cpp like the demo

#### Feature
* Fast. backtest 1000000 tick data for less than 1min.
* Order queuing. new order will be placed at the end of the orderbook based on the Ordrbook data.
* Postonly order type. support postonly order type like the crypto-exchange.
* Latency simulation. every order has latency to simulate the real environment. new order(Status::Opening) -> OPENLATENCY -> order(Status::Opened) -> cancel order -> CANCELLATENCY -> order(Status::Canceled)
* Commission. the commission for active order and passive order could be set individually.
