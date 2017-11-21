import datetime
from datetime import date
from tradeapi import *

now = datetime.datetime.now()

candles = Poloniex.returnChartData('USDT_BTC', date(2012,1,1), now, 30)

file = open("candles_raw.txt", "w")
for candle in candles:
    high   = candle['high']
    low    = candle['low']
    openn  = candle['open']
    close  = candle['close']
    outp = "{:.8f}\n{:.8f}\n{:.8f}\n{:.8f}\n".format(high,low,openn,close)
    file.write(outp)
file.close()



