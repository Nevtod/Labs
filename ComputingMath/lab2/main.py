import numpy as np
import matplotlib.pyplot as plt

betas = np.array([0.04, 3e7, 1e4], dtype=float)
N = 5000
T = 0.3
dim = 3
tau = T / N

def f(x, y0, coef):
    y = tau * coef * x + y0
    return np.array([-betas[0] * y[0] + betas[2] * y[1] * y[2],
              betas[0] * y[0] - betas[2] * y[1] * y[2] - betas[1] * y[1]**2,
              betas[1] * y[1]**2])

def MSI(x0, F, eps=1e-13):
    x = x0
    
    cnt = 0
    r = 1
    while r > eps:
        x = F(x)
        cnt += 1
        r = np.sum(np.abs(F(x) - x)) / np.sum(np.abs(x))
        # print("r:", r)
    
    print("cnt:", cnt)
    return x
    


curY = np.array([1, 0, 0])
ys = np.zeros((N + 1, dim))
ys[0] = curY
prevK1 = np.zeros(3)
prevK2 = np.zeros(3)
for i in range(N):
    g = lambda x: f(x, curY, 0.5)
    k1 = MSI(prevK1, g)
    
    h = lambda x: f(x, curY - 0.5 * tau * k1, 2)
    k2 = MSI(prevK2, h)
    
    ys[i + 1] = ys[i] + tau * (1.5 * k2 - 0.5 * k1)
    curY = ys[i + 1]
    prevK1 = k1
    prevK2 = k2
    
def plot2(xsPack, ysPack, xlabel, ylabel, title, legendsPack, picname):
    plt.figure(figsize=[8, 3])
    plt.grid(True)
    for i in range(len(xsPack)):
        plt.plot(xsPack[i], ysPack[i], label= "" if legendsPack[i] == None else legendsPack[i])
    
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.tight_layout()
    plt.legend()
    plt.savefig(f"{picname}.eps")
    

t = np.linspace(0, T, N + 1)
t = np.array([t, t, t])
ys = np.transpose(ys)
# ys[0] -= 1
print(ys[2])
plot2([t[0]], [ys[0]], "Время, c", "концентрация", "Зависимость концентрации от времени, A",
      ["A"], "a")

plot2([t[1]], [ys[1]], "Время, c", "концентрация", "Зависимость концентрации от времени B",
      ["B"], "b")

plot2([t[2]], [ys[2]], "Время, c", "концентрация", "Зависимость концентрации от времени C",
      ["C"], "c")
