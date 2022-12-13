import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import pandas as pd

df = pd.read_table("time.txt", sep="\t")#, index_col = None, header = 0)

df = df.groupby("NP").mean("Time").sort_values("NP").reset_index()
print(df)

plt.figure(figsize=[12, 9])
plt.plot(df["NP"], df["Time"])
plt.xlabel("number of threads")
plt.ylabel("Time, s")
plt.grid()
plt.title("Time")
plt.savefig("time.png")

plt.figure(figsize=[12, 9])
matplotlib.rcParams.update({'font.size': 30})

n = 10000
NPs = np.arange(6) + 1
theorS = n * n / (n * np.log2(NPs) + n * n / NPs)
plt.plot(df["NP"], df["Time"][0] / df["Time"], label="exp")
# plt.plot(NPs, theorS, label="theor")

plt.xlabel("Number of threads")
plt.ylabel(r"$S = \frac{Time_1}{Time_x}$")
# plt.ylabel(r"\frac{a}{b}")
plt.grid()
plt.title("Acceleration")
plt.legend()
plt.savefig("Acceleration.png")

plt.figure(figsize=[12, 9])
matplotlib.rcParams.update({'font.size': 30})
plt.plot(df["NP"], df["Time"][0] / df["Time"] / df["NP"])
plt.xlabel("Number of threads")
plt.ylabel(r"Efficiency")
# plt.ylabel(r"\frac{a}{b}")
plt.grid()
plt.title("Efficiency")
plt.savefig("Efficiency.png")