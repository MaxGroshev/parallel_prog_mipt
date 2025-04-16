import matplotlib.pyplot as plt
import numpy as np
# Чтение данных из файла
x = []
y1 = []
y2 = []

with open('res.txt', 'r') as file:
    for line in file:
        # Пропуск пустых строк и строк с комментариями (настроить при необходимости)
        if not line.strip() or line.startswith('#'):
            continue
        # Разделение строки на элементы (предполагается разделение пробелами/табуляцией)
        parts = line.split()
        if len(parts) >= 3:
            x.append(float(parts[0]))  # Первый столбец — X
            y1.append(float(parts[1])) # Второй столбец — Y1
            y2.append(float(parts[2])) # Третий столбец — Y2

# Построение графиков
plt.figure(figsize=(10, 5))
plt.plot(x, y1, label='parallel', marker='o', linestyle='-')
plt.plot(x, y2, label='single', marker='x', linestyle='--')
plt.xlabel('number of elements')
plt.ylabel('time')
plt.title('Graph from Text File Data')
plt.grid(True)
plt.legend()
plt.savefig("res.png")