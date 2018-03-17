import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import fileinput

x = []
y = []

i = 0
for line in fileinput.input():
    print line
    i = i + 1
    x.append(float(line.split(';')[0]))
    y.append(float(line.split(';')[1]))

sns.set_style("darkgrid")
plt.plot(x, y)
plt.show()
