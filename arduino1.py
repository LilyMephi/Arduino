import matplotlib.pyplot as plt
import numpy as np



x =[1.1, 1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1,0.05]
y= [1.1, 0.966, 0.893, 0.794, 0.695, 0.6, 0.496, 0.397, 0.302, 0.203, 0.099, 0.052]

delta = 0.1
x1 = []
y1 = []
N = 12
k = 0.994297
d = 0.001244
for i in range(N):
            x1.append(i*delta + 0.05)
            y1.append(k*x1[i]+d)

plt.title("График зависимости считывания амплитуды от напрыжения")
plt.grid(True)
plt.xlabel('Подаваемые значения напряжения,V') #Подпись для оси х
plt.ylabel('Полученные значения напряжения, v') #Подпись для оси у
plt.scatter(x,y, s = 30, c = 'black')
plt.plot(x,y,'g--',label = 'полученные значения',alpha = 0.7)
plt.plot(x1, y1, 'b-', label = 'апроксимированная линия Y = 0.994X + 0.001')
plt.legend()
plt.show()
