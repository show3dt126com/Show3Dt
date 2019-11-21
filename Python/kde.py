import csv
import os
import matplotlib as mpl
from matplotlib import pyplot as plt
import numpy as np
from sklearn.neighbors import KernelDensity

mpl.use('TkAgg')
mpl.rcParams['font.sans-serif'] = ['SimHei']
mpl.rcParams['axes.unicode_minus'] = False

BASE_DIR = os.path.dirname(__file__)
file_path = os.path.join(BASE_DIR, 'train.csv')

def plotKDE(x, column, kernel='gaussian', algorithm='auto', breadth_first=1,
            bw=30, leaf_size=40, atol=0, rtol=1E-8, title="no title"):
    # kde
    x_plot = np.linspace(min(x), max(x), 1000).reshape(-1, 1)
    x = np.mat(x).reshape(-1, 1)
    fig, ax = plt.subplots()
    kde = KernelDensity(bandwidth=bw, algorithm=algorithm, kernel=kernel,
                        atol=atol, rtol=rtol, breadth_first=breadth_first,
                        leaf_size=leaf_size).fit(x)
    log_dens = kde.score_samples(x_plot)
    ax.hist(x, density=True, color='lightblue')
    ax.plot(x_plot[:, 0], np.exp(log_dens))

    plt.title(title[column])
    plt.show()

def loadData():
    x = []
    with open(file_path, 'rt') as csvfile:
        reader = csv.reader(csvfile)
        for line in reader:
            tmp = list(map(float, line[4:]))
            x.append(tmp)
    return x


def plotKDE2():
    x = 99+33
    plt.plot([1,2,1,3])
    plt.show()
    return x

def plotKDE3():
    return 100

