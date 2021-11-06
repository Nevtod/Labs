import numpy as np
#Вход задачи
N = 101  # число точек
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