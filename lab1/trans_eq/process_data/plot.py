import numpy as np
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt


def plot_solution():
    dataframe = pd.read_csv("results.csv")
    x = dataframe["x"]
    t = dataframe["t"]
    u = dataframe["u"]
    # print(u)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    # ax.plot_trisurf(x, t, u, cmap="viridis")
    ax.scatter(x, t, u)

    ax.set_xlabel("x")
    ax.set_ylabel("t")
    ax.set_zlabel("u")
    ax.set_title("График функции u(x,t)")

    plt.savefig("solution.png")
    # plt.show()
    plt.clf()

if __name__ == "__main__":
    plot_solution()
