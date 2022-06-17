#ifndef INTERNALNODE_H
#define INTERNALNODE_H

#include "Node.h"
#include <vector>

template <typename T, unsigned ORDER>
class InternalNode final: public Node<T,ORDER>{
    public:
        InternalNode();
    private:
        std::vector<Node<T,ORDER>*> regions;
};

#endif
