import numpy as np 
import matplotlib.pyplot as plt 
import matplotlib.gridspec as gridspec
from itertools import repeat
from matplotlib.lines import Line2D

gs = gridspec.GridSpec(2, 2)

fig = plt.figure()
ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[0, 1])
ax3 = fig.add_subplot(gs[1, :])
fig.tight_layout()



data1 = np.loadtxt("logisticmap.dat", usecols=(0, 1), skiprows=1)
xCol1 = data1[:,0]
yCol1 = data1[:,1]

cycles = np.loadtxt("cycleLocations.dat")

feigenbaum = np.loadtxt("feigenbaum.dat")


with open('bifurcationFixedPoints.dat') as f:
    lines = [line.split() for line in f]


for n in range(len(lines)):
    lines[n] = list(map(float,lines[n]))
    

ax2.scatter(xCol1,yCol1,s=.5,color='k',marker=".",alpha=.1)
for n in cycles:
    ax2.axvline(n,color="red")



test = []
test.extend(repeat(3,50))

for bif in range(len(lines)):
    clr = ''
    for n in range(64):
        if(bif==0):
            clr='r'
        if(bif==1):
            clr='b'
        if(bif==2):
            clr='k'
        if(bif==3):
            clr='g'
        if(bif==4):
            clr='orange'
        if(bif==5):
            clr="purple"
        ax3.scatter(n,list(lines[bif]*(int(64/len(lines[bif]))))[n],s=3,color=clr)
        
ax2.set_title("Logistic Map with Onsets Shown")
ax2.set_xlabel("Lambda")
ax2.set_ylabel("x_10000")

ax3.set_title("Fixed Points")
ax3.set_xlabel("Last 64 Iterations")
ax3.set_ylabel("x_10000")

colors = ['red','blue','black','green','orange','purple']
lines = [Line2D([0], [0], color=c, linewidth=6) for c in colors]
labels = ['2-Cycle', '4-cycle', '8-cycle', '16-cycle', '32-cycle', '64-cycle']
ax3.legend(lines, labels,loc='upper right')
ax1.axhline(4.669201609,color="blue")

ax1.plot(feigenbaum)
ax1.set_xlabel("Cycles 4-64")
ax1.set_ylabel("Delta")
ax1.set_title("Feigenbaum Approximations")

plt.savefig('LogisticData.png',dpi=1200)

plt.show()
