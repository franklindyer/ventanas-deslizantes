import matplotlib.pyplot as plt

f = open("datos.txt", 'r')
ts = []

for l in f.read().split("\n"):
    if l[0:3] == "Seg":
        ts = ts + [float(l[23:])]

ts = sorted(ts)

plt.plot(range(len(ts)), ts)
plt.show()
