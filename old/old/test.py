from Genetics import *
from brain import *


candles       = []
fit_val       = 0


def agent_initialize(self):
    self.brain = Brain(num_neurons=75)
    self.dna   = self.brain.n_array
    self.bank  = 100.0
    self.trade = {'in_trade': False, 'buy_at': 0.0, 'amount': 0.0, 'sell_at': 0.0}

def agent_do_fitness(self):
    if self.fitness == -1:
        self.fitness = 0
        for i in range(len(candles)):

            # print("candle: {}".format(i))
            candle = candles[i]
            self.brain.input(candle)
            self.brain.run()
            result = self.brain.output()

            if result == 1 and self.trade['in_trade'] == False:
                self.trade['in_trade'] = True
                self.trade['buy_at']   = candle['close']
                self.trade['amount']   = self.bank

            elif result == 0 and self.trade['in_trade'] == True:
                self.trade['in_trade'] = False
                self.trade['sell_at']  = candle['close']
                ratio = (self.trade['sell_at']/self.trade['buy_at'])
                self.bank = ratio * self.trade['amount']

            if self.bank < 10.0:
                break

        self.fitness = self.bank

def agent_mutate(self):
    chance_amount = 0.6
    if random.uniform(0.0, 1.0) <= chance_amount:
        for i in range(len(self.dna)):
            if random.uniform(0.0, 1.0) <= chance_amount:
                self.dna[i].set_weights()


Agent.initialize = agent_initialize
Agent.do_fitness = agent_do_fitness
Agent.mutate     = agent_mutate



################################################################################



def run_program():
    get_candles()
    fit_val = 100/(len(candles)*4)
    ga = Genetics(population=20, chance_amount=0.05, num_of_children=4)
    ga.run()


def get_candles():
    candles_raw = open("candles_raw.txt", "r")
    new_candles = []
    while True:
        try:
            c_high  = float(candles_raw.readline())
            c_low   = float(candles_raw.readline())
            c_open  = float(candles_raw.readline())
            c_close = float(candles_raw.readline())
            new_candles.append({'high': c_high, 'low': c_low, 'open': c_open, 'close': c_close})
        except:
            break
    candles_raw.close()
    candles.extend(new_candles[-200:])


if __name__ == "__main__":
    run_program()
