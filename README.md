# R-Tree

## Cómo correrlo

### Prerequisitos

> Recordar de que antes de instalar cualquier librería siempre es recomendable correr `sudo apt update` y `sudo apt upgrade` primero.

Para ejecutar el R-Tree es necesario contar con la librería **SDL2**, puesto que esta es la que se empleó para crear la interfaz gráfica.

Si se está empleando Linux, basta con utilizar los comandos mostrado a continuación para descargarla.
```
sudo apt-get install libdsl2-2.0
sudo apt-get install libdsl2-dev
```

Cabe mencionar también que se requiere de **g++** para la compilación. Si es que no se tiene este instalado, usar
```
sudo apt install g++
```
para hacerlo.

Una vez cubiertos estos prerequisitos, podemos ejecutarlo.

### Compilación

Para poder hacer la compilación, ofrecemos dos formas:

1. Usando el Makefile adjunto:
```
make build
```

2. Usando la línea de comando directamente:
```
g++ main.cpp Display.cpp structs/rtree.cpp -lSDL2
```

### Ejecución

Correr el ejecutable que resulta de la compilación.

Los comandos son sencillos:
- **Click izquierdo:** añadir un punto (sea independiente o para formar una figura)
- **Click derecho:** remover una figura
- **Presionar la tecla `z` en el teclado:** agregar una figura aleatoria
- **Posición del mouse:** colorear los k = 3 nodos mas cercanos

El R-Tree se actualiza automáticamente tras recibir estos inputs.
