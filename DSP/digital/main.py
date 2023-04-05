import numpy as np
import matplotlib.pyplot as plt 

N = 100
def p(l):
    cnts = np.zeros(2 * l)
    cnts[0] = 1
    answ = 0
    sum = 0
    for i in range(1, 2 * l + 1):
        newCnts = np.zeros(2 * l)
        for j in range(1, 2 * l - 1):
            newCnts[j] = cnts[j - 1] + cnts[j + 1]
            if j + i == 2 * l:
                answ += newCnts[j]
        cnts = newCnts.copy()
        if (i == 2 * l):
            for x in cnts:
                sum += x
        
    return answ / sum

answ = np.zeros(101)
for i in range(1, 100):
    answ[i - 1] = p(i)
    
print(answ)
answ[0] = 1
plt.plot(answ[2:95])

plt.savefig("pic.png")

        
        
    
    