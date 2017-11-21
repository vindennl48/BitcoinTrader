import random
import math

start = 0
end = 0

class Brain:
    def __init__(self, num_neurons=35):
        self.n_array = []
        self.create_neurons(num_neurons)

    def create_neurons(self, num_neurons):
        for i in range(num_neurons):
            self.n_array.append(Neuron(i, self.n_array))
        for n in self.n_array:
            n.set_weights()

    def run(self, iterations=2):
        for _ in range(iterations):
            for i in range(len(self.n_array)):
                self.n_array[i].prepare()
            for i in range(len(self.n_array)):
                self.n_array[i].fire()

    # x = binary string 8 bits
    def input(self, x):
        # for n in self.n_array:
        #     n.sum = 0
        self.n_array[0].sum = x['high']
        self.n_array[1].sum = x['low']
        self.n_array[2].sum = x['open']
        self.n_array[3].sum = x['close']


    def output(self):
        bit = 0
        self.n_array[-1].prepare()
        if self.n_array[-1].react > 0.5:
            bit = 1
        else:
            bit = 0
        return bit

    def Print(self):
        for n in self.n_array:
            print(n)


class Neuron:
    def __init__(self, index_id, n_array):
        self.index_id = index_id
        self.n_array  = n_array
        self.weights  = []
        self.sum      = 0
        self.react    = 0

    def set_weights(self):
        self.weights = []
        for _ in range(len(self.n_array)):
            self.weights.append(random.randrange(-100,100))

    def fire(self):
        if self.react != 0:
            for i in range(len(self.n_array)):
                if self.index_id != i:
                    result = self.react * self.weights[i]
                    self.n_array[i].sum += result

    def prepare(self):
        # self.react = self.sum
        try:
            self.react = round(1/(1+math.exp(-self.sum)), 4)
        except:
            if self.sum > 0: self.react = 1
            else:            self.react = 0
        self.sum = 0

    def __str__(self):
        result = "Neuron: {}, Sum: {:.2f}, React: {:.2f}".format(self.index_id, self.sum, self.react)
        return result



