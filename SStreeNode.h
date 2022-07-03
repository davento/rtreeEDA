#ifndef SSTREENODE_H
#define SSTREENODE_H

#include "Point.h"
#include "MBC.h"
#include <vector>

class SStreeNode{
    public:

        SStreeNode();
        SStreeNode(const SStreeNode&) = default;
        MBC getBound() const {return bound;}

    protected:
        MBC bound;
        std::vector<SStreeNode*> children;
};

class PointsNode final : public SStreeNode{
    public:

    private:
        std::vector<Point> points;
};

#endif

