
import numpy as np # импорт бибилиотеки numpy
import matplotlib.pyplot as plt # импорт модуля matplotlib.pyplot
from scipy import signal
from scipy.linalg import dft
import math

def find_zero_in_range(axis, signal, leftX, rightX, eps = 0.01): #for positive funcs only
    """

    Args:
        axis (nparr): array of x axis values
        signal (nparr): given signal
        leftX (float): left border of x axis values 
        rightX (_type_):right border of x axis values
        eps (float): max allowed interval for zero detection (0.01 by default)

    Returns:
        float: value among x axis, corresponding to zero signal value
        returns first zero from left 
    """
    for i in range(len(axis) - 1):
        x = axis[i]
        y = signal[i]
        if ((abs(y) < eps) and x > leftX and x < rightX):
            return x
    return None

def find_zero_cnt_in_range(axis, signal, leftX, rightX): #for positive funcs only
    """
    Args:
        axis (nparr): array of x axis values
        signal (nparr): given signal
        leftX (float): left border of x axis values 
        rightX (_type_):right border of x axis values

    Returns:
        int: zeros cnt in the given range
    """
    cnt = 0
    wasNotZero = True
    for i in range(len(axis) - 1):
        eps = 0.01
        x = axis[i]
        y = signal[i]
        if (y < eps and x > leftX and x < rightX and wasNotZero):
            cnt += 1
            wasNotZero = False
        else:
            wasNotZero = True
    return cnt


def find_max_with_index_in_range(axis, signal, leftX, rightX):
    """_summary_

    Args:
        axis (nparr): array of x axis values
        signal (nparr)): given signal
        leftX (float): left border of x axis values 
        rightX (_type_):right border of x axis values

    Returns:
        int: index
        float: max value
    """
    curMax = -1e9
    for i in range(len(axis) - 1):
        x = axis[i]
        y = signal[i]
        if (y > curMax and x > leftX and x < rightX):
            curMax = y
            index = i
    return index, curMax


# def get_val_by_axis
def get_power(spectre):
    deltaNu = 1 / (len(spectre) - 1)
    integral = 0
    for i in range(1, len(spectre)):
        ampl = ((spectre[i] + spectre[i - 1]) / 2)
        integral += ampl * np.conjugate(ampl) * deltaNu
    return integral

def compress(spectre):   #for odd array lengthes only and L = 2
    L = 2
    midID = np.ceil(len(spectre) / 2) - 1
    length = len(spectre)
    shift = 0
    res = np.array([], dtype=complex)
    if (length  // 2) % 2 == 0:
        res.resize(length // 2 + 1)
        shift = 0
    else:
        res.resize(length // 2 - 2 + 1)
        shift = 1
    
    for i in range(len(res)):
        res[i] = spectre[shift + i * 2]
        # print(res[i],'---', spectre[shift + i * 2])
        
    deltaNu = 1 / (len(spectre) - 1)
    axis = np.linspace(-0.5 / L + shift * deltaNu, 0.5 / L - shift * deltaNu, len(res))
    
    return axis, res


# def plot(x, y, xlabel, ylabel, title):
#     plt.figure(figsize=[8, 3])
#     plt.plot(x, y)
#     plt.title(title)
#     plt.xlabel(xlabel)
#     plt.ylabel(ylabel)
#     plt.tight_layout()
#     plt.grid(True)
#     plt.show()
    
# # for plotting several plots on the one figure

# def plot2(xsPack, ysPack, xlabel, ylabel, title, legendsPack):
#     plt.figure(figsize=[8, 3])
#     plt.grid(True)
#     for i in range(len(xsPack)):
#         plt.plot(xsPack[i], ysPack[i], label= "" if legendsPack[i] == None else legendsPack[i])
    
#     plt.title(title)
#     plt.xlabel(xlabel)
#     plt.ylabel(ylabel)
#     plt.tight_layout()
#     plt.legend()
#     plt.show()

def impulse_signal(k, N):
    return 1 if (k < N and k >= 0) else 0

def add_zeros(signal, N):
    res = np.zeros(len(signal) + N, dtype = complex)
    for i in range(len(signal)):
        res[i] = signal[i]
        
    return res

    