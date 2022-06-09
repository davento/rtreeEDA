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

El R-Tree se actualiza automáticamente tras recibir este input.

## Estructuras


## Funciones

### Inserción

El Rtree cuenta con el metodo "insert" el cual recibe una figura como argumento
y este va a llamar a la función insert que recibe un nodo y una figura.

Metodo insert:
```cpp
bool Rtree::insert(Figure *f){

    root = ::insert(root, f);
    return true;
}
```
La función insert es una función recursiva la cual tiene como caso de parada
si el nodo que se pasa como argumento es un nodo hoja, en caso contrario se 
buscará el sub-árbol al que puede pertenecer para así insertar la figura.
En caso exista un desbordamiento se va llamar la función handleOverflow.
Finalmente la función retorna la raíz del árbol.

Función insert:
```cpp
RNode* insert(RNode* node, Figure* figure){
    if(node->isLeaf()){
        node->myFigures.push_back(new Figure(*figure));
        //actualizas el mbb, agregando esta figura
        node->bound = MBB::merge(node->bound, figure->getBound());
        if(node->myFigures.size() == ORDER + 1)
            handleOverflow(node);
    }
    else{
        RNode* v = chooseSubtree(node, figure);
        insert(v, figure);
        // insertaste la figura entonces toca actualizar el mbb
        node->bound = RNode::regionsMbb(node->regions);
    }
    // si se ha generado una nueva raíz por desbordamiento retorna a la nueva raíz
    if(node->father)
        return node->father;
    return node;    
}
```

La función handleOverflow va a repartir el nodo desbordado en 2 nodos y en caso 
sea necesario va a crear un nodo padre para el par de nodos.

Función handleOverflow:
```cpp
void handleOverflow(RNode* nodeOverflowed){
    RNode* v = new RNode;
    split(nodeOverflowed, v);// reparte los elementos del nodo en si mismo y v de la forma más conveniente
    if(!nodeOverflowed->father){// si no hay nodo padre lo creas
        RNode* root = new RNode;
        addChildrenToFather(root, nodeOverflowed, v);
    }
    else{// si hay padre solo agregas v como hijo al padre
        RNode* w = nodeOverflowed->father;
        // update MBR(u) in w or whatever that means
        v->father = w;
        w->regions.push_back(v);
        w->bound = RNode::regionsMbb(w->regions);
        if(w->regions.size() == ORDER + 1)// revisas si el padre esta desbordado
            handleOverflow(w);
    }
}
```


### Búsqueda


### Borrado
