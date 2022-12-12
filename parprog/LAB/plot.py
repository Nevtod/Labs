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
plt.plot(df["NP"], df["Time"][0] / df["Time"])
plt.xlabel("Number of threads")
plt.ylabel(r"$S = \frac{Time_1}{Time_x}$")
# plt.ylabel(r"\frac{a}{b}")
plt.grid()
plt.title("Acceleration")
plt.savefig("Acceleration.png")