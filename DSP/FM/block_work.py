import time, math, sys
import scipy.signal as sig
import scipy.fftpack as fft
import numpy as np
import soundfile as sf
import matplotlib.pyplot as plt
import sounddevice as sd
import wavio

class Filter(object):
    def __init__(self, b, a = [1], init=[]):
        self.b = b
        self.a = a
        self.clear(init)
        
    def __call__(self, x):
        y, self.state = sig.lfilter(self.b, self.a, x, zi=self.state)
        return y
    
    def clear(self, init=[]):
        self.state = sig.lfiltic(self.b, self.a, [], init)
        
def remezord(d1, d2, df):
    return int(math.ceil((-d1 - d2 - 13) / (2.324 * df)))



def fix_diff(x):
    x[x > math.pi] -= 2 * math.pi
    x[x < -math.pi] += 2 * math.pi
    return x

def mult_by_exp(samples, Fc, Fs):
    # print("arange", np.arange(0, len(samples)))
    return samples * np.exp(-1j * np.arange(0, len(samples)) * 2 * np.pi * Fc / Fs)

def diff(samples):
    res = np.zeros(len(samples))
    for i in range(len(samples)):
        res[i] = samples[i] - samples[i - 1]
    return res

filter_diff = Filter([1, -1])

def amplify(samples, dev, Fs):
    return samples * Fs / dev

def reciever(samples, div, Fs, Fc, lpf):
    
    s1 =  mult_by_exp(samples, Fc, Fs)
    b, a = sig.iirdesign(div, 5 * div, 1, 100, fs=Fs)
    s2 = sig.lfilter(b, a, s1)
    s3 = np.angle(s2)
    s4 = diff(s3)
    s5 = fix_diff(s4)
    s6 = amplify(s5, div, Fs)
    
    return s6

def reciever_block(last_phase, samples, div, Fs, Fc, lpfilter):
    # we take into account one past sample
    s1 = np.exp(-1j * (last_phase + 1 * 2 * np.pi * Fc / Fs)) * mult_by_exp(samples, Fc, Fs)
    s2 = lpfilter(s1)
    s3 = np.angle(s2)
    s4 = filter_diff(s3)
    s5 = fix_diff(s4)
    s6 = amplify(s5, div, Fs)
    last_phase_new = (last_phase +  2 * np.pi * len(samples) / Fs * Fc) % (2 * np.pi)
    
    return s6, last_phase_new

def transmitter(samples, div, Fs, Fc):
    s = np.zeros(len(samples))
    time = np.arange(0, len(samples))
    integral = 0
    A0 = 1
    for t in time:
        s[t] = A0 * np.real(np.exp(1j * (2 * np.pi * t * Fc / Fs + div * integral)))
        integral += samples[t]/Fs
    return s

def transmitter_block(last_phase, last_integral, samples, div, Fs, Fc):
    s = np.zeros(len(samples))
    time = np.arange(0, len(samples))
    integral = last_integral
    A0 = 1
    for t in time:
        s[t] = A0 * np.real(np.exp(1j * (1 * 2 * np.pi * Fc / Fs   #we take into account here one past sample since previous cycle 
                                         + last_phase
                                         + 2 * np.pi * t * Fc / Fs
                                         + div * integral)))
        integral += samples[t] / Fs
    
    last_phase_new = (last_phase +  2 * np.pi * len(samples) / Fs * Fc) % (2 * np.pi)
    # last_integral_new = ite
    return s, last_phase_new, integral


def start_reception():
    with sf.SoundFile("channel.wav", mode="r") as f, sd.OutputStream(
        channels=1, samplerate=f.samplerate
    ) as ss:    

        div = 3000
        Fs = ss.samplerate
        Fc = 7000
                
        b,a = sig.iirdesign(div, 5 * div, 1, 100, fs=Fs)
        lpf = Filter(b, a)
        s = np.array([], dtype = complex)
        last_phase = 0
        N = 2**10
        for block in f.blocks(blocksize=N, dtype='float32',always_2d=False, fill_value=0): 
            sNew, last_phase = reciever_block(last_phase, block, div, Fs, Fc, lpf)
            s = np.concatenate([s, sNew])
        
        print("Block reception done")
        s = np.real(s)
        wavio.write("output.wav", s, Fs, sampwidth=2)
    


def start_transmission():
    with sf.SoundFile("input.wav", mode="r") as f, sd.OutputStream(
        channels=1, samplerate=f.samplerate
    ) as ss:    

        div = 3000
        Fs = ss.samplerate
        Fc = 7000
                
        s = np.array([])
        last_phase = 0
        last_integral = 0
        N = 2**10
        
        for block in f.blocks(blocksize=N, dtype='float32',always_2d=False, fill_value=0): 
            
            sNew, last_phase, last_integral = transmitter_block(last_phase, last_integral, block, div, Fs, Fc)
            s = np.concatenate([s, sNew])
        
        print("Block transmission done")

        wavio.write("channel.wav", s, Fs, sampwidth=2)
        
        

# program start

start_transmission()
start_reception()