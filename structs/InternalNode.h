#ifndef INTERNALNODE_H
#define INTERNALNODE_H

#include "Node.h"
#include <vector>

template <typename T, unsigned ORDER>
class InternalNode final: public Node<T,ORDER>{
    public:
        InternalNode();
        inline bool isLeaf() const override;
    private:
        std::vector<Node<T,ORDER>*> regions;
};

#endif
