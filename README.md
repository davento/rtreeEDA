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

### Ejecución

Para poder hacer la ejecución, ofrecemos dos formas:

1. Usando el Makefile adjunto:
```
make build
```

2. Usando la línea de comando directamente:
```
g++ main.cpp Display.cpp structs/rtree.cpp -lSDL2
```

## Estructuras


## Funciones

### Inserción


### Búsqueda


### Borrado
