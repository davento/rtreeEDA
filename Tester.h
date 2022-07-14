#ifndef TESTER_H
#define TESTER_H


#include <ctime>
#include <cstdlib>
#include "Rtree.h"
#include "Display.h"


class Tester {

    public:
        typedef RtreeNode Node;

        Tester();
        Tester(int n_, int l_);

        void insert();
        void search();
        void remove();
        void knn();

    protected:
        friend class Rtree;
        friend class Display;
        friend class Figure;

    private:
        int n;
        int insertBatchSize;
        int searchBatchSize;
        int removeBatchSize;
        int knnBatchSize;
        int polygonLimit;

        int dim;
        clock_t start, end;

        double insertBatch(Rtree* &);
        double searchBatch(Rtree* &);
        double removeBatch(Rtree* &, std::vector<Figure> &);
        double knnBatch(Rtree* &, int);
};

#endif

