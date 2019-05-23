import numpy as np

num_obstaculos = 100

diametro_obstaculos = 20

lx = 4000
ly = 4000

obstaculos_x = np.random.rand(num_obstaculos)*lx-lx/2
obstaculos_y = np.random.rand(num_obstaculos)*ly-ly/2

robot = np.random.rand(2)
robot[0] = robot[0] * lx - lx/2
robot[1] = robot[1] * ly - ly/2
bola = np.random.rand(2)
bola[0] = bola[0] * lx - lx/2
bola[1] = bola[1] * ly - ly/2

# Generar nodos
tolerancia = 100
r = 1/5
n = 1000
count = 0
nodos = []
# Para pocos obstaculos...
#x = np.append([robot[0]], np.random.rand(n)*abs(bola[0]-robot[0]) + min(bola[0],robot[0]))
x = np.append([robot[0]], np.random.rand(n)*lx - lx/2)
x = np.append(x,[bola[0]])
# Para pocos obstaculos...
#y = np.append([robot[1]], np.random.rand(n)*abs(bola[1]-robot[1]) + min(bola[1],robot[1]))
y = np.append([robot[1]], np.random.rand(n)*ly - ly/2)
y = np.append(y,[bola[1]])
for i in range(len(x)):
    nodos.append([count,[x[i],y[i]],[], 0, np.sqrt(np.power(bola[0]-x[i],2)+np.power(bola[1]-y[i],2)),0])
    for j in range(len(x)):
        if j == i or np.sqrt(np.sum(np.power(x[i]-x[j],2)+np.power(y[i]-y[j],2)))>=500: continue
        x1 = np.linspace(x[j],x[i],20)
        y1 = np.linspace(y[j],y[i],20)
        s = [np.sum((np.abs(x1[i]-obstaculos_x)<=tolerancia)*(np.abs(y1[i]-obstaculos_y)<=tolerancia)) for i in range(len(x1))]
        if not np.sum(s):
            nodos[len(nodos)-1][2].append(j)
    count += 1

def sort(l):
    abierto = [nodos[l[i]] for i in range(len(l))]
    valores = np.array([g[3]+g[4] for g in abierto])
    orden = np.sort(valores)
    w = [np.where(valores==orden[i])[0][0] for i in range(len(orden))]
    o = [l[w[i]] for i in range(len(w))]
    return o

# A*
path = []
abierto = [0]
cerrado = []
actual = np.array([robot[0],robot[1]])
while abierto and np.prod(actual != bola):
    a = abierto.pop()
    if a in cerrado: continue
    nodo = nodos[a]
    cerrado.append(a)
    conectados = nodo[2]
    actual = np.array(nodo[1])
    for i in range(len(conectados)):
        if conectados[i] in cerrado or conectados[i] in abierto: continue
        nodos[conectados[i]][3] += nodo[3] + np.sqrt(np.sum(np.power(np.array(nodos[i][1])-actual,2)))
        nodos[conectados[i]][5] = nodo[0]
        abierto.append(conectados[i])
    abierto = sort(abierto)
while np.prod(actual != robot):
    path.append(nodo[0])
    nodo = nodos[nodo[5]]
    actual = np.array(nodo[1])
path.append(0)
