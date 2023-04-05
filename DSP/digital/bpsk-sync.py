import scipy.signal as sg
import numpy as np
import matplotlib.pyplot as plt
import commpy as cp

class Filter(object):
    def __init__(self, b, a = [1], init=[]):
        self.b = b
        self.a = a
        self.clear(init)
    def __call__(self, x):
        y, self.state = sg.lfilter(self.b, self.a, x, zi=self.state)
        return y
    def clear(self, init=[]):
        self.state = sg.lfiltic(self.b, self.a, [], init)
        

def access_bit(data, num):
    base = int(num // 8)
    shift = int(num % 8)
    return (data[base] >> shift) & 0x1


data = b'asnfajksdfnal'
bits = [access_bit(data,i) for i in range(len(data)*8)]
bits = np.array(bits, dtype=float)
print(bits)
#bits = np.array([1,0,1,1,1,0])
signal = 1 - 2*bits
signal_up = np.zeros(len(signal)*48)
signal_up[::48] = signal
t, rrc = cp.filters.rrcosfilter(481, 0.25, 1e-3, 48e3)
filter = Filter(rrc)
rx = np.real(filter(signal_up) * np.exp(1j* 2 * np.pi * 17 / 48 * np.arange(4992))) + np.random.normal(scale=0.1, size=signal_up.shape)
rx2 = filter(rx * np.exp(-1j* 2 * np.pi * 17 / 48 * np.arange(4992)))
result = rx2[::48]
result = result[10:] / 24
print (len(rx))

plt.figure(figsize=[8, 3])
plt.plot(np.real(result))
plt.plot(signal)

plt.savefig("bpsk.png")


