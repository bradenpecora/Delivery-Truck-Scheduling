# converts routes from routes/*.txt to figures in figures/*.pdf
# must run from the root of the workspace
# Use -a or --arrows to draw routes with arrows
import matplotlib.pyplot as plt 
import sys
import os
color = "#0000FF"
depot_color = "#000000"
draw_arrow = False
prime = None
for filename in sys.argv:
    if filename == "showRoutes.py":
        continue
    if filename == "-a" or filename == "--arrows":
        draw_arrow = True
        continue
    with open(os.path.join(os.getcwd(), filename), 'r') as f: # open in readonly mode
        x = []
        y = []
        prime = []
        for line in f:
            address = line.split()
            x.append(int(address[0]))
            y.append(int(address[1]))
            if len(address) > 2:
                prime.append(int(address[2]))
        
        plt.plot(x, y, c=color, marker='o')

        #Draw Arrows
        if draw_arrow:
            p = 0.9
            for i in range(len(x)-1):
                plt.arrow(x[i], y[i], (x[i+1] - x[i])*p, (y[i+1] - y[i])*p,head_width=0.2, head_length=0.4, color=color)
        if len(prime) > 0:
            for xCoor, yCoor, isPrime in zip(x, y, prime):
                if isPrime:
                    prime = plt.scatter(xCoor, yCoor, c="#FF9900", zorder=10)
        title = filename.replace('routes/', '')
        title = title.replace('.txt', '')
        plt.title(title)
    color="#008800"
depot = plt.scatter(0, 0, c=depot_color, zorder=10)
if prime:
    plt.legend([depot, prime], ["Depot", "Prime customer"])
else:
    plt.legend([depot], ["Depot"])
plt.show()
savePath = 'figures/' + title + '.pdf'
plt.savefig(savePath)