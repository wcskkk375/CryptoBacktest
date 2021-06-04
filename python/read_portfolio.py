from matplotlib import pyplot as plt
import pandas as pd

def read_portfolio(filename,diff_timezone_hour = 0):
    df = pd.read_csv(filename)

    df['dt'] = pd.to_datetime(df['ts'] + 1000 * 3600 * diff_timezone_hour, unit='ms')

    plt.plot(df['dt'],df['profit'])
    
    plt.show()

if __name__ == '__main__':
    filename = '../ExampleStrategy/benchmark_strategy/build/portfolio.csv'
    read_portfolio(filename)
