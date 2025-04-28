import matplotlib.pyplot as plt
import numpy as np
# Чтение данных из файла
x = []
y1 = []
y2 = []

with open('res.txt', 'r') as file:
    for line in file:
        if not line.strip() or line.startswith('#'):
            continue
        parts = line.split()
        if len(parts) >= 3:
            x.append( np.log2(float(parts[0])))  
            y1.append((float(parts[1]))) 
            y2.append((float(parts[2])))

plt.figure(figsize=(10, 5))
plt.plot(x, y1, label='pool of threads', marker='o', linestyle='-')
plt.plot(x, y2, label='single thread', marker='x', linestyle='--')
plt.xlabel('number of elements, $log_2$')
plt.ylabel('time, sec')
plt.title('Graph from Text File Data')
plt.grid(True)
plt.legend()
plt.savefig("res.png")