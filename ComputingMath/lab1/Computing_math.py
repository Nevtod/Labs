import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
from numpy.lib.shape_base import split
import math
import cmath


def get_MSI (matrix, f, tau, iterCnt):  #method of simple iterations
    n = np.size(f)
    B = np.diagflat([1] * n) - tau * matrix
    f = tau * f

    x = f
    for i in range (iterCnt):
        x = B.dot(x) + f

    return x


def get_MJ (matrix, f, iterCnt): # method of Jacobi
    u = f
    n = np.size(f)
    for cnt in range(iterCnt):
        uNext = np.zeros(n)
        for i in range (n):
            uNext[i] = (f[i] - matrix[i].dot(u)) / matrix[i][i]
        u = uNext

    return u


def get_MZ (matrix, f, iterCnt):
    u = f
    n = np.size(f)
    answ = np.zeros((n, iterCnt))
    for cnt in range(iterCnt):
        uNext = np.zeros(n)
        for i in range (n):
            uNext[i] = (f[i] - 
                                matrix[i][:i - 1].dot(uNext[:i - 1]) - 
                                                                        matrix[i][i + 1:].dot(u[i + 1:])) / matrix[i][i]
        u = uNext
        answ[:, cnt] = uNext

    return answ[:, -1]

eigenValuesFilename = "eigenValues15.dat"
#Вход задачи
N = 15  # число точек
h = 1 / (N - 1)
matrix = np.zeros (((N - 2) * (N - 2), N * N))


#построение матрицы
def elem (i, j):
            return N * j + i

for j in range (1, N - 1):
    for i in range (1, N - 1):
        eqID = (j - 1) * (N - 2) + i - 1  #equationID  - индекс строчки

        if (i > 1):
            matrix[eqID][elem(i - 1, j)] = -1

        if (j > 1):
            matrix[eqID][elem(i, j - 1)] = -1

        if (i < N - 2):
            matrix[eqID][elem(i + 1, j)] = -1
            
        if (j < N - 2):
            matrix[eqID][elem(i, j + 1)] = -1

        matrix[eqID][elem(i, j)] = 4

print ("half")

#Удаление пустых столбцов
zeroColumns = []
for i in range (N * N - 1, -1, -1):
    if i % N == 0 or \
            i < N or \
            i >= N * (N - 1) or \
            (i + 1) % N == 0:
        zeroColumns.append (i)

otherColumns = [x for x in range (N * N - 1) if x not in zeroColumns]
matrix = matrix[:, otherColumns]

print ("matrix is done")
# plt.spy(matrix)
# plt.show()

# поиск собственных чисел
# eigenValues = np.linalg.eigvals(matrix)
# print ("eighenvalues  are computed")
# with open (eigenValuesFilename, "w") as file:
#     for val in eigenValues:
#          file.write(str(val) + "\n")

minVal = 1e9
maxVal = -1e9
with open(eigenValuesFilename, 'r') as file:
    for line in file:
        val = cmath.polar(complex(line))[0]
        if (val > maxVal):
            maxVal = val
        elif val < minVal:
            minVal = val
# Формирование вектора правой части
v = np.zeros((N - 2) * (N - 2))
v[(N // 2) * (N - 2) + N // 2 - 1] = h**2
# v[N * N // 3 : N * N // 2] += h ** 2

#Решаем систему разными методами
# u = get_MSI(matrix, v, 2 / (maxVal + minVal), 100)
# u = get_MSI(matrix, v, 1, 100)
# print (matrix)
# u = get_MJ(matrix, v, 0)
u = get_MZ(matrix, v, 10)

# u = np.linalg.solve(matrix, v)

# Заполняем Матрицу Z для отображения
Z = np.zeros((N, N))
for (uVal, oldID) in zip(u, otherColumns):
    Z[oldID % N][oldID // N] = uVal


fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

w = v = np.linspace(0, 1, N)
X, Y = np.meshgrid(w, v)

surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)
ax.zaxis.set_major_locator(LinearLocator(10))
# A StrMethodFormatter is used automatically
# ax.zaxis.set_major_formatter('{x:.02f}')
# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)
plt.savefig("pic.png")
# plt.show()










